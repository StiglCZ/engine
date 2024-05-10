/*
   Platform specific graphics lib.
   Currently: Linux
*/

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <signal.h>

#include "native.hh"
#include "renderer.hh"
#include "scripting.hh"
#include "logging.hh"
#include "types.hh"
#include "file.hh"

#include <thread>
#include <string.h>

#ifndef __unix__
    #error Unix only!
#endif

int W, H;

Display *disp;
Window win;
GC gc;
Pixmap backBuffer;
// TODO: Add steering wheel support
// 16 mouse buttons
// 256 keys
u8 button[16];
u8  keys[256];
u8 keyTranslations[256];
Point   mouse;
bool isRunning = 1;
std::vector<void *> exitFuncs;
std::vector<Model>* modelBufferPtr;
void t2() {
    XEvent e;
    while(1){
        XNextEvent(disp, &e);
        if(e.type == DestroyNotify)break;
        switch(e.type){
            case ButtonPress:
                button[e.xbutton.button] = 1;
                break;
            case ButtonRelease:
                button[e.xbutton.button] = 0;
                break;
            case KeyPress:{
                int keyPressed = e.xkey.keycode;
                keys[keyPressed] = 1;
                }break;
            case KeyRelease:{
                int keyReleased = e.xkey.keycode;
                keys[keyReleased] = 0;}
                break;
            case MotionNotify:
                mouse.X = e.xmotion.x;
                mouse.Y = e.xmotion.y;
                break;
        }
    }
}
void Exiter();

int customXIOErrorHandler(Display *display) {
    (void)display;
    Info("XWindow Exit!");
    Exiter();
    return 0;
}
// For ^C from cmd
void Exiter(int arg) {
    (void)arg;
    Info("\nConsole exit!");
    Exiter();
}

u16 GetKeyFromString(char* str) {
    KeySym ks = XStringToKeysym(str);
    return XKeysymToKeycode(disp, ks);
}

void NativeInit(int w, int h, char* title) {
    W = w;
    H = h;
    // Initiate the window
    disp = XOpenDisplay(0);
    win = XCreateSimpleWindow(disp, DefaultRootWindow(disp), 10, 10, w, h, 1, 0, 0);
    // Resize the window
    XSizeHints sizeHints;
    XSetIOErrorHandler(customXIOErrorHandler);
    sizeHints.flags = PMinSize | PMaxSize;
    sizeHints.min_width = sizeHints.max_width = w;
    sizeHints.min_height = sizeHints.max_height = h;
    XSetWMNormalHints(disp, win, &sizeHints);
    // Configuring more X11 windowing
    gc = XCreateGC(disp, win, 0, 0);    
    XSelectInput(disp, win, ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
    XMapWindow(disp, win);
    XSetForeground(disp, gc, WhitePixel(disp, DefaultScreen(disp)));
    XSync(disp, True);
    XSetStandardProperties(disp, win, title, title, None, NULL, 0, NULL);
    // Intialize back buffer(for double-buffering)
    backBuffer = XCreatePixmap(disp, win, w, h, DefaultDepth(disp, DefaultScreen(disp)));

    signal(SIGINT, Exiter);
    
    // Wait for first expose event
    XEvent e;
    XNextEvent(disp, &e);
    std::thread(t2).detach();
}

void ChangeColor(Color col) {
    XSetForeground(disp, gc, col.B | col.G << 8 | col.R << 16);
}
void ChangeColor(long col) {
    XSetForeground(disp, gc, col);
}
void ClearScreen() {
    XSetForeground(disp, gc, BlackPixel(disp, DefaultScreen(disp)));
    XFillRectangle(disp, backBuffer, gc, 0, 0, W, H);
    XSetForeground(disp, gc, WhitePixel(disp, DefaultScreen(disp)));
}
void DrawLine(Point src, Point dst) {
    if(((src.X < 0 || src.X > SIZE_X) && (dst.X < 0 || dst.X > SIZE_X)) ||
       ((dst.Y < 0 || dst.Y > SIZE_Y) && (src.Y < 0 || src.Y > SIZE_Y))) return;
    XDrawLine(disp, backBuffer, gc, src.X, src.Y, dst.X, dst.Y);
}
void DrawText(Point pos, const char* str) {
    XDrawString(disp, backBuffer, gc, pos.X, pos.Y, str, strlen(str));
}
void FrameFinished(){
    XCopyArea(disp, backBuffer, win, gc, 0, 0, W, H, 0, 0);
    XSetForeground(disp, gc, 0x0000000000000000);
    XFillRectangle(disp, backBuffer, gc, 0, 0, W, H);
    XSetForeground(disp, gc, 0xFFFFFFFFFFFFFFFF);
    XFlush(disp);
}
void CenterMouse() {
    XWarpPointer(disp, None, win, 0, 0, 0, 0, HALFSIZE_X, HALFSIZE_Y);
    XFlush(disp);
}
void freeModels() {
    Info("Freeing models...");
    for(u32 i =0; i < (*modelBufferPtr).size(); i++){
        Debg("Freeing model " + std::to_string(i));
        if((*modelBufferPtr)[i].freed)continue;
        std::vector<Face>* cfs = &(*modelBufferPtr)[i].faces;
        for(u32 j =0; j < cfs->size(); j++)
            delete[] (*cfs)[j].m;
        cfs->clear();
        (*modelBufferPtr)[i].freed = 1;
    }
}

// Somehow, this works, no idea how
void Screenshot(const char* filename){
    Info("Saving screenshot " + std::string(filename) + "...");
    XImage *xi = XGetImage(disp, win, 0, 0, SIZE_X, SIZE_Y, AllPlanes, ZPixmap);
    u8 pixels[SIZE_X * SIZE_Y * 3];
    int* d = (int*)xi->data;
    for(int i =0; i < SIZE_Y * SIZE_X; i++){
        int a = *d++; // Why does this work?(Possibly alpha channel skip)
        pixels[i * 3+2] = a       & 0xFF;
        pixels[i * 3+1] = a >> 8  & 0xFF;
        pixels[i * 3+0] = a >> 16 & 0xFF;
    }
    SaveScreenshot((u8*)pixels, (Point){SIZE_X, SIZE_Y}, filename);
}
void Exiter() {
    isRunning = 0;
    for(u32 i =0; i < exitFuncs.size(); i++){
        Debg("Exiting function is being ran: " + std::to_string(i));
        ((void(*)())exitFuncs[i])();
    }
    closeAllScripts();
    freeModels();
    
    XFlush(disp);
    XFreePixmap(disp, backBuffer);
    XFreeGC(disp, gc);
    XDestroyWindow(disp, win);
    
    Info("Exiting");
    exit(0);
}

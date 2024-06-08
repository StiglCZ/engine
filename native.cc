#ifdef __unix__
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

int W, H;
u64 backColor = 0x0000000000000000;

Display *disp;
Window win;
GC gc;
Pixmap backBuffer;
// 16 mouse buttons
// 256 keys
u8 button[16];
u8 keys[256];
Point mouse;
bool  isRunning = 1;
std::vector<void *> exitFuncs;
std::vector<Model> *modelBufferPtr;
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

// Exit functions for both X11 and C-c
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
    XDrawString(disp, win, gc, pos.X, pos.Y, str, strlen(str));
    XDrawString(disp, backBuffer, gc, pos.X, pos.Y, str, strlen(str));
}
void FrameFinished(){
    XCopyArea(disp, backBuffer, win, gc, 0, 0, W, H, 0, 0);
    XSetForeground(disp, gc, backColor);
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
    for(u32 i =0; i < modelBufferPtr->size(); i++){
        Debg("Freeing model " + std::to_string(i));
        if((*modelBufferPtr)[i].freed)continue;
        std::vector<Face>* cfs = &(*modelBufferPtr)[i].faces;
        for(u32 j =0; j < cfs->size(); j++)
            delete[] (*cfs)[j].verts;
        cfs->clear();
    }
    // Since there are no models, we can safely remove all of them
    modelBufferPtr->clear();
}

void Screenshot(const char* filename){
    Info("Saving screenshot " + std::string(filename) + "...");
    XImage *xi = XGetImage(disp, win, 0, 0, SIZE_X, SIZE_Y, AllPlanes, ZPixmap);
    u8 pixels[SIZE_X * SIZE_Y * 3];
    int* d = (int*)xi->data;
    for(int i =0; i < SIZE_Y * SIZE_X; i++){
        int a = *d++; // Possibly alpha channel skip?
        pixels[i * 3+2] = a       & 0xFF;
        pixels[i * 3+1] = a >>  8 & 0xFF;
        pixels[i * 3+0] = a >> 16 & 0xFF;
    }
    SaveScreenshot((u8*)pixels, {SIZE_X, SIZE_Y}, filename);
}
void Exiter() {
    // The main thread is hopefully stopped by this
    isRunning = 0;
    for(u32 i =0; i < exitFuncs.size(); i++){
        Debg("Exiting function is being ran: " + std::to_string(i));
        ((void(*)())exitFuncs[i])();
    }
    // Try to make sure the main thread exits
    usleep(100);
    
    closeAllScripts();
    freeModels();
    
    XFlush(disp);
    XFreePixmap(disp, backBuffer);
    XFreeGC(disp, gc);
    XDestroyWindow(disp, win);
    
    Info("Exiting");
    exit(0);
}
#elif defined _WIN32
#include <windows.h>
#include "native.hh"
#include "renderer.hh"
#include "scripting.hh"
#include "logging.hh"
#include "types.hh"
#include "file.hh"

#include <thread>
#include <string.h>

int win;
HWND window;
u8 button[16];
u8 keys[256];
Point mouse;
bool  isRunning = 1;
std::vector<void *> exitFuncs;
std::vector<Model> *modelBufferPtr;
void freeModels() {
    Info("Freeing models...");
    for(u32 i =0; i < (*modelBufferPtr).size(); i++){
        Debg("Freeing model " + std::to_string(i));
        if((*modelBufferPtr)[i].freed)continue;
        std::vector<Face>* cfs = &(*modelBufferPtr)[i].faces;
        //for(u32 j =0; j < cfs->size(); j++)
            //delete[] (*cfs)[j].m;
        cfs->clear();
        (*modelBufferPtr)[i].freed = 1;
    }
}

void Exiter(){
    closeAllScripts();
    freeModels();
    Info("Exiting");
}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch (uMsg) {
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
            break;
        case WM_DESTROY:
            Exiter();
            PostQuitMessage(0);
            break;
        case WM_PAINT:
            PAINTSTRUCT ps;
            //HDC hdc = BeginPaint(hwnd, &ps);
            //HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
            //SelectObject(hdc, hPen);
            // Clear the screen
            //HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
            //FillRect(hdc, &ps.rcPaint, hBrush);
            /*for(int i =0; i < lines.size(); i++){
                MoveToEx(hdc, lines[i].start.X, lines[i].start.Y, NULL);
                LineTo(hdc, lines[i].end.X, lines[i].end.Y);
            }
            EndPaint(hwnd, &ps);*/
            break;
    }
    return 0;
}

void t2(){
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

u16 GetKeyFromString(char* str){ return *str; }

void NativeInit(int w, int h, char* title){
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASSEX wc;
    wc.cbSize         = sizeof(WNDCLASSEX);
    wc.style          = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc    = WindowProc;
    wc.cbClsExtra     = 0;
    wc.cbWndExtra     = 0;
    wc.hInstance      = hInstance;
    wc.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground  = (HBRUSH)(0x0001);
    wc.lpszMenuName   = NULL;
    wc.lpszClassName  = "Engine";
    ATOM result = RegisterClassEx(&wc);
    window = CreateWindowEx(0, "Engine", title,
                               WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                               CW_USEDEFAULT, CW_USEDEFAULT,
                               CW_USEDEFAULT, NULL, NULL, hInstance, NULL
                              );
    if (!window || !result){
        Err("Window creation failed!", 1);
        Exiter();
        exit(0);
    }
    ShowWindow(window, SW_SHOWNORMAL);
    UpdateWindow(window);
    
    // Get the first window create message
    MSG msg;
    GetMessage(&msg, NULL, 0, 0);
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    
    win = (int)msg.wParam;
    std::thread(t2).detach();
}
void ChangeColor(Color col) {

}
void Screenshot(const char* filename){

}

void ClearScreen(){

}
void DrawLine(Point src, Point dst){
    
}
void DrawText(Point pos, const char* str){

}
void FrameFinished(){

}

#endif

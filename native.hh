#ifndef NATIVE
#define NATIVE

#include "types.hh"
extern u8 button[16];
extern u8 keys[256];
extern bool isRunning;
extern std::vector<Model>* modelBufferPtr;
extern Point mouse;
extern u16 GetKeyFromString(char* str);
extern void NativeInit(int w, int h, char *title);
extern void ChangeColor(Color col);
extern void ChangeColor(long col);
extern void ClearScreen();
extern void CenterMouse();
extern void DrawLine(Point src, Point dest);
extern void DrawText(Point pos, const char *str);
extern void FrameFinished();
extern void Screenshot(const char *file);
extern void Exiter(int);
extern std::vector<void *> exitFuncs;
#endif

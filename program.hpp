#ifndef PROGRAM
#define PROGRAM
#include "types.hh"
#include <string>
#include <unistd.h>

#define STREAM_SIZE 8192
#define SLEEP_TIME 1000
const std::string
    scriptsDir = "scripts/",
    assetsDir  = "assets/",
    mainScript = "main.so";

const i8 Title[] = "Game",
         initFName[] = "Init",
         startFname[] = "Start",
         updateFname[] = "Update";

extern int main();
extern void d_init(void*, u32);
extern void d_other(u32);
extern inline u64 time();
#endif

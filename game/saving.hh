#ifndef SAVING
#define SAVING
#include "../types.hh"
// node = location of the save
struct SaveRequest {
    void* data;
    u32 len;
    u32 node;
};
struct LoadRequest {
    void* data;
    u32 len;
};

typedef struct SaveRequest SaveRequest;
typedef struct LoadRequest LoadRequest;
extern const u32 zero;

extern void Save();
extern void FileName(char *fileName);
extern void RequestSaveInfo(SaveRequest (*func)());
extern LoadRequest Load(u32 node);

#endif

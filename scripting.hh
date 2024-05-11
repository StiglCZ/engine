#ifndef SCRIPTING
#define SCRIPTING

extern void *loadScript(char *file);
extern void closeScript(void *script);
extern void scriptErr(int source);
extern void closeAllScripts();
template <typename T> T getFunction(void *script, char *funcName);
#ifdef __unix__
#include <dlfcn.h>

template <typename T>
T getFunction(void* script, char* funcName) {
    T func = reinterpret_cast<T>(dlsym(script, funcName));
    if(!func){scriptErr(1);}
    return func;
}
#endif
#endif

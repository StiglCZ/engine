#ifndef SCRIPTING
#define SCRIPTING
#include <dlfcn.h>

extern void *loadScript(char *file);
extern void closeScript(void *script);

extern void scriptErr(int source);
extern void closeAllScripts();
template <typename T>
T getFunction(void* script, char* funcName) {
    T func = reinterpret_cast<T>(dlsym(script, funcName));
    if(!func){scriptErr(1);}
    return func;
}
#endif

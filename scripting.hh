#ifndef SCRIPTING
#define SCRIPTING

extern void *loadScript(char *file);
extern void closeScript(void *script);

#include <dlfcn.h>
extern void scriptErr(int source);
template <typename T>
T getFunction(void* script, char* funcName) {
    T func = reinterpret_cast<T>(dlsym(script, funcName));
    if(!func){scriptErr(1);}
    return func;
}
extern void closeAllScripts();
#endif

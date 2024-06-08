#include "scripting.hh"
#include "logging.hh"

void scriptErr(int source) {
    Warn("Script error!");
}

#ifdef __unix__
#include <string>
#include <vector>
std::vector<void *> handlers;

void* loadScript(char* file) {
    Debg(std::string("Loading script ") + file);
    // RTLD_LAZY can be used if theres some unlinked function
    void* handle = dlopen(file, RTLD_NOW);
    if(!handle)scriptErr(0);
    handlers.push_back(handle);
    return handle;
}

void closeScript(void *script) {
    Debg("Closing script " + std::to_string((long)script) + "...");
    int status = dlclose(script);
    if(status)scriptErr(2);
}
#endif

#ifdef _WIN32
#include <string>
#include <vector>
#include <windows.h>
std::vector<HINSTANCE> handlers;

void* loadScript(char* file) {
    Debg(std::string("Loading script ") + file);
    // RTLD_LAZY can be used if theres some unlinked function
    HINSTANCE handle = LoadLibrary(file);
    if(!handle)scriptErr(0);
    handlers.push_back(handle);
    return (void*)handle;
}

void closeScript(HINSTANCE *script) {
    Debg("Closing script...");
    int status = 0; //FreeLibrary(script);
    if(status)scriptErr(2);
}

#endif
void closeAllScripts() {
    Info("Closing scripts...");
    for(unsigned int i =0; i < handlers.size();i++)
        closeScript(handlers[i]);
    handlers.clear();
}

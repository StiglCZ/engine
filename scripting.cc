#include "scripting.hh"
#include "logging.hh"
#include <string>
#include <vector>

#ifdef __unix__
void scriptErr(int source) {
    if(source == 1) // Missing function isn't critical
        Debg("Script  error: " + std::string(dlerror()));
    else
        Warn("Script  error: " + std::string(dlerror()));
    Warn("Script error!");
}

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
#include <windows.h>
void scriptErr(int source) {
    Warn("Script error!");
}
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

void closeScript(void *script) {
    closeScript((HINSTANCE*)script);
}
#endif
void closeAllScripts() {
    Info("Closing scripts...");
    for(unsigned int i =0; i < handlers.size();i++)
        closeScript(handlers[i]);
    handlers.clear();
}

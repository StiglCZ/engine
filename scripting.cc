#include "scripting.hh"
#include "logging.hh"

// PLATFORM SPECIFIC
// unix only

#ifndef __unix__
    #error Unix only!
#endif

#include <string>
#include <vector>
void scriptErr(int source) {
    if(source == 1) // Missing function isn't critical
        Debg("Script  error: " + std::string(dlerror()));
    else
        Warn("Script  error: " + std::string(dlerror()));
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
void closeAllScripts() {
    Info("Closing scripts...");
    for(unsigned int i =0; i < handlers.size();i++)
        closeScript(handlers[i]);
    handlers.clear();
}

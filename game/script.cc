// some example of script
#include "../types.hh"
#include "../logging.hh"
#include <string>

#include "audio.hh"

extern "C" { // Without this function naming is broken
    GameData *d; // Global variable with the pointer
    void Init(GameData *gd, u32 si){
        Info("Intializing script with index " + std::to_string(si));
        d = gd; // get the pointer to the global scope

        // load the script example.so
        d->loadScript((char*)(std::string("example.so")).c_str(), gd);

        GameObject go;
        go.position = go.rotation = go.coll = {1};
        go.scale = {1, 1, 1};
        go.model = 0; // no model(invisible)
        go.script = si; // asign THIS script to the object
        gd->gameObjects->push_back(go);
    }

    void Start(u32 index){
        Info("Object " + std::to_string(index) + "has been created in last frame");

        // Play some audio
        AudioControl* control = audioct(d);
        u32 b = control->LoadTrack(
             (i8*)std::string("example.mp3").c_str());
        control->Play(b, 1);
        control->Play3D({}, {}, {}, b, 1);
    }

    void Update(u32 index){
        // Move the object with speed depending on the frametime
        (*d->gameObjects)[index].position.X +=
            1.0 * (u32)getDelta(d);
    }
    
}

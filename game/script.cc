// Example script
#include "../types.hh"
#include "../logging.hh"
#include <string>

#include "audio.hh"

extern "C" { // Without this function naming is different
    GameData *d; // Global variable with the pointer
    
    void Init(GameData *gd, u32 si){
        Info("Intializing script with index " + std::to_string(si));

        // Load the script example.so
        gd->loadScript("example.so", gd);

        GameObject go;
        go.position = go.rotation = go.coll = {1};
        go.scale = {1, 1, 1};
        go.model = 0; // default(empty/invisible) model
        go.script = si; // asign THIS script to the object
        pushgo(gd, go);
        
        d = gd; // Get the pointer to the global scope
    }

    void Start(u32 index){
        Info("Object " + std::to_string(index) + "has been created in last frame");

        // Play some audio
        AudioControl* control = audioct(d);
        u32 b = control->LoadTrack("example.mp3");
        control->Play(b, 1);
        control->Play3D({}, {}, {}, b, 1);
    }

    void Update(u32 index){
        // Move the object with speed depending on the frametime
        (*d->gameObjects)[index].position.X +=
            1.0 * (u32)getDelta(d);
    }
    
}

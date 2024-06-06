// some example of script
#include "../types.hh"
#include "../logging.hh"
#include <string>

#include "audio.hh"

extern "C" { // Without this it doensn't work
    GameData *gd2;
    void Init(GameData *gd, u32 si){
        Info("Intializing script with index " + std::to_string(si));
        gd2 = gd; // get the pointer out of scope

        // load the script example.so
        gd->loadScript((char*)(std::string("example.so")).c_str(), gd);

        GameObject go;
        go.position = go.rotation = go.coll = {1};
        go.scale = {1, 1, 1};
        go.model = 0; // no object(empty)
        go.script = si; // asign THIS script to the object
        gd->gameObjects->push_back(go);
    }

    void Start(u32 index){
        Info("Object " + std::to_string(index) + "has been created");

        // Play some audio
        AudioControl* control = ((AudioControl**)gd2->stream)[3];
        u32 b = control->LoadTrack(
             (i8*)std::string("example.mp3").c_str());
        control->Play(b, 1);
        control->Play3D({}, {}, {}, b, 1);
    }

    void Update(u32 index){
        // Move the object with speeding depends on the frametime
        (*gd2->gameObjects)[index].position.X +=
            1.0 / (u32)*gd2->deltaTime;
    }
    
}

#include "../types.hh"
#include "physics.hh"
#include <string>
#include "scene.hh"
// EXAMPLE MAIN SCRIPT
GameData* d;
extern "C" {
    void Init(GameData* gd, u32 scriptIndex){
        gd->loadScript("movement.so", gd);
        gd->loadScript("physics.so", gd);
        // gd->loadScript("audio.so", gd);
        gd->loadScript("scenemgr.so", gd);
        
        u32 index = gd->loadModel("car.obj");
        GameObject go = emptyGameObj;
        go.color = {0xAA, 0x22, 0xCC};
        go.model = index;
        go.coll = {1, 3, 1};
        go.script = scriptIndex;
        gd->gameObjects->push_back(go);
        
        go.position.Y += 20;
        go.flags = 10;
        gd->gameObjects->push_back(go);
        d = gd;
    }
    void Start(u32 index){
        // Add collision
        std::vector<u32>* a = (std::vector<u32>*)((void**)d->stream)[2];
        a->push_back(index);
        if((*d->gameObjects)[index].flags == 10){
            // Make physics object
            PhysicsProps* pp = (PhysicsProps*)((void**)d->stream)[4];
            pp->physicsObjects->push_back(index);
        }
        LOAD_SCENE("assets/scene.bin");
    }
    void Update(uint index){        
        (void)index; // Unused
        if((*d->gameObjects)[index].flags == 10){
            d->drawText({1, 10}, ("dT: " +std::to_string(1000000.0 / (u32)*d->deltaTime)).c_str());
            d->drawText({1, 25}, ("x: " + std::to_string(d->cp->pos.X) + " y:" + std::to_string(d->cp->pos.Y) + " z:" + std::to_string(d->cp->pos.Z)).c_str());
        }
    }
}

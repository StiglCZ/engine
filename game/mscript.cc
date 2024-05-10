#include "../types.hh"
#include "physics.hh"
#include <string>
#include "scene.hh"

//EXAMPLE MAIN SCRIPT
extern "C" {
    GameData* d;
    double angleX = 0;
    void Init(GameData* gd, u32 scriptIndex){
        gd->loadScript("movement.so", gd);
        gd->loadScript("physics.so", gd);
        //gd->loadScript("audio.so", gd);
        //gd->loadScript("scenemgr.so", gd);
        
        u32 index = gd->loadObject("car.obj");
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
        // Make physics object
        std::vector<u32>* a = (std::vector<u32>*)((void**)d->stream)[2];
        a->push_back(index);
        if((*d->gameObjects)[index].flags == 10){
            PhysicsProps* pp = (PhysicsProps*)((void**)d->stream)[4];
            pp->physicsObjects->push_back(index);

            /*AudioControl* control = ((AudioControl**)gd2->stream)[3];
            uint b = control->LoadTrack("./assets/hello1.mp3");
            AudioSource as = control->Play(b, 0.9);*/
        }
        //AudioControl* ac = ((AudioControl**)gd2->stream)[3];
        //ac->Play3D({0, 0, 0}, {-1, 0, 0}, {0,0,0},ac->LoadTrack("./assets/stal2.mp3"), 1.0);
    }
    void Update(u32 index){
        (void)index; // Unused
        if((*d->gameObjects)[index].flags == 10){
            d->drawText({1, 10}, ("dT: " +std::to_string(1000000.0 / (u32)*d->deltaTime)).c_str());
            d->drawText({1, 25}, ("x: " + std::to_string(d->cp->pos.X) + " y:" + std::to_string(d->cp->pos.Y) + " z:" + std::to_string(d->cp->pos.Z)).c_str());
        }
    }
}

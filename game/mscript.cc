#include "../types.hh"
#include "physics.hh"
#include <string>
//#include "scene.hh"
// EXAMPLE MAIN SCRIPT
extern "C" {
    GameData* d;
    void Init(GameData* gd, u32 scriptIndex){
        //gd->loadScript("audio.so", gd);
        gd->loadScript("movement.so", gd);
        gd->loadScript("physics.so", gd);
        gd->loadScript("scenemgr.so", gd);
        d = gd;
        
        u32 index = gd->loadModel("car.obj");
        GameObject go = emptyGameObj;
        go.position.Y = -2;
        go.color = YELLOW;
        go.model = index;
        go.coll = {1, 3, 2.5};
        go.script = scriptIndex;
        go.bounce = 100;
        gd->gameObjects->push_back(go);

        go.position.Y += 20;
        go.flags = 10;
        gd->gameObjects->push_back(go);
    }
    void Start(u32 index){
        // Add collision to the object
        collisions(d)->push_back(index);
        if((*d->gameObjects)[index].flags == 10)
            // Make physics object
            physics(d)->physicsObjects->push_back(index);
    }
    void Update(u32 index){
        if((*d->gameObjects)[index].flags == 10){
            d->drawText({1, 10}, ("dT: " +std::to_string(1000000.0 / (u32)*d->deltaTime)).c_str());
            d->drawText({1, 25}, ("x: " + std::to_string(d->cp->pos.X) + " y:" + std::to_string(d->cp->pos.Y) + " z:" + std::to_string(d->cp->pos.Z)).c_str());
        }
        if((*d->gameObjects)[index].colliding)
            (*d->gameObjects)[index].color = RED;
        else (*d->gameObjects)[index].color = YELLOW;
    }
}

#include "physics.hh"
#include "../types.hh"
extern "C" {
    const fx bounce1Lvl = 1.0 / 255;
    fx gravity = -0.0001, tvelocity = 1;
    std::vector<u32> physicsObjects;
    GameData* d;
    PhysicsProps pp = {&physicsObjects, &gravity, &tvelocity};
    void Init(GameData* gd, u32 si){
        gd->loadScript("collision.so", gd);
        d = gd;
        GameObject go = emptyGameObj;
        go.script = si;
        d->gameObjects->push_back(go);
        ((PhysicsProps**)gd->stream)[4] = &pp;
    }
    void Update(u32 _ix){
        (void)_ix; // Unused
        for(u32 i =0; i < physicsObjects.size(); i++){
            GameObject* go2 = &(*d->gameObjects)[physicsObjects[i]];
            if(go2->colliding)
                if((*d->gameObjects)[go2->colliding].position.Y <= go2->position.Y){
                    go2->velocity.Y *= -1 * ((*d->gameObjects)[go2->colliding].bounce + go2->bounce) * bounce1Lvl;
                    continue;
                }
            if((go2->velocity.Magnitude() + gravity) >= tvelocity)
                continue;
            go2->velocity.Y += gravity;
        }
        for(u32 i =0; i < d->gameObjects->size(); i++)
            (*d->gameObjects)[i].position += (*d->gameObjects)[i].velocity;
    }
}

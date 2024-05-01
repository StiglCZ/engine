#include "../types.hh"
extern "C" {
    GameData* gd2;
    std::vector<u32> collidingObjects;
    void Init(GameData* gd, u32 scriptIndex){
        gd2 = gd;
        GameObject go = emptyGameObj;
        go.script = scriptIndex;
        gd->gameObjects->push_back(go);
        ((void**)gd2->stream)[2] =
            (void*)&collidingObjects;
    }
    // TODO: Add rotation support somehow
    void Start(u32 index){
        (void)index;
    }
    #define between(lo, hi, lo2) \
        ((lo2 >= lo && lo2 <= hi) || (lo2 <= lo && lo2 >= hi))
    
    bool AABB(Vector3 lo, Vector3 hi, Vector3 lo2, Vector3 hi2){
        // Fixes the zero scale object collisions
        if(lo == hi || lo2 == hi2)return false;
        
        return
           (between(lo.X, hi.X, lo2.X) ||
            between(lo.X, hi.X, hi2.X)) &&
           (between(lo.Y, hi.Y, lo2.Y) ||
            between(lo.Y, hi.Y, hi2.Y)) &&
           (between(lo.Z, hi.Z, lo2.Z) ||
            between(lo.Z, hi.Z, hi2.Z) );
    }
    void Update(u32 index){
        (void)index; // Unused
        for(u32 i =0; i < collidingObjects.size(); i++){
            u32 n = collidingObjects[i];
            (*gd2->gameObjects)[n].colliding = 0;
            for(u32 j =0; j < gd2->gameObjects->size();j++){
                if(n == j)continue;
                if(AABB(
                        (*gd2->gameObjects)[n].position,
                        (*gd2->gameObjects)[n].position + (*gd2->gameObjects)[n].coll,
                        (*gd2->gameObjects)[j].position,
                        (*gd2->gameObjects)[j].position + (*gd2->gameObjects)[j].coll
                   )){
                    (*gd2->gameObjects)[n].colliding = j;
                    (*gd2->gameObjects)[j].colliding = n;
                }
            }
        }
    }
}

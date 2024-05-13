#include "../types.hh"
#include "../logging.hh"
#include <fstream>
#include "scene.hh"
using std::string;
#define readStrWithSize(ifs) u8 size1;          \
    ifs.read((i8*)&size1, 1);                   \
    u8 str[size1];                              \
    ifs.read((i8*)str, size1);
#define readVector3(ifs, v)                                             \
    ifs.read((i8 *)&v.X, sizeof(fx));                                   \
    ifs.read((i8 *)&v.Y, sizeof(fx));                                   \
    ifs.read((i8 *)&v.Z, sizeof(fx));
extern "C" {
    GameData* d;
    void LoadSceneFile(Scene scene);
    void Init(GameData* gd, uint scriptIndex){
        (void)scriptIndex;
        ((void**)gd->stream)[0] = (void*)&LoadSceneFile;
        d = gd;
    }
    
    void LoadSceneFile(Scene scene){
        // Section 1 - Load models
        // Section 2 - Load scripts
        // Section 3 - Load gameObjects
        Debg("Loading scene...");
        std::ifstream ifs(scene.scenePath);
        u16 size0;
        // Section 1 - Load models
        ifs.read((i8*)&size0, 2);
        for(u32 i =0; i < size0; i++){
            readStrWithSize(ifs);
            d->loadModel((i8*)str);
        } size0 = 0;
        
        // Section 2 - Load scripts
        ifs.read((i8*)&size0, 2);
        for(u32 i =0; i < size0; i++){
            readStrWithSize(ifs);
            d->loadScript((i8*)str, d);
        }
        // Section 3 - Load gameObjects
        ifs.read((i8*)&size0, 2);
        for(u32 i =0; i < size0; i++){
            Vector3 v;
            GameObject go = emptyGameObj;
            readVector3(ifs, v); go.position = v;
            readVector3(ifs, v); go.rotation = v;
            readVector3(ifs, v); go.scale = v;
            readVector3(ifs, v); go.coll = v;
            readVector3(ifs, v); go.color = {50, 50, 50};//go.color = {(u16)v.X, (u16)v.Y, (u16)v.Z};
            u64 mID, flags;
            ifs.read((i8*)&mID, sizeof(u64));
            ifs.read((i8*)&flags, sizeof(u64));
            go.model = mID + (u64)d->getResource(RESOURCE_Model_Count, 0);
            go.flags = flags;
            d->gameObjects->push_back(go);
        }
    }
}

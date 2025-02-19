#include "../types.hh"
#include "../logging.hh"
#include <fstream>
using std::string;
#define readStr(ifs) u8 size1;                  \
    ifs.read((i8*)&size1, 1);                   \
    u8 str[size1];                              \
    ifs.read((i8*)str, size1);
#define readVector3(ifs, v)                                             \
    ifs.read((i8 *)&v.X, sizeof(fx));                                   \
    ifs.read((i8 *)&v.Y, sizeof(fx));                                   \
    ifs.read((i8 *)&v.Z, sizeof(fx));
extern "C" {
    GameData* d;
    void LoadSceneFile(const char* scene);
    void Init(GameData* gd, u32 scriptIndex){
        (void)scriptIndex;
        ((void**)gd->stream)[0] = (void*)&LoadSceneFile;
        d = gd;
    }
    struct Vector3i{
        int X, Y, Z;
    };
    
    void LoadSceneFile(const char* scene){
        // Section 1 - Load models
        // Section 2 - Load scripts
        // Section 3 - Load gameObjects
        Debg("Loading scene...");
        std::ifstream ifs(scene);
        u16 size0;
        u64 modelCount = (u64)d->getResource(RESOURCE_Model_Count, 0);
        
        // Section 1 - Load models
        ifs.read((i8*)&size0, 2);
        for(u32 i =0; i < size0; i++){
            readStr(ifs);
            d->loadModel((i8*)str);
        }
        
        // Section 2 - Load scripts
        ifs.read((i8*)&size0, 2);
        for(u32 i =0; i < size0; i++){
            readStr(ifs);
            d->loadScript((i8*)str, d);
        }
        
        // Section 3 - Load gameObjects
        ifs.read((i8*)&size0, 2);
        for(u32 i =0; i < size0; i++){
            Vector3 v;
            Vector3i u;
            
            GameObject go = emptyGameObj;
            readVector3(ifs, v); go.position = v;
            readVector3(ifs, v); go.rotation = v;
            readVector3(ifs, v); go.scale = v;
            readVector3(ifs, v); go.coll = v;
            readVector3(ifs, u); go.color =
                {(u16)u.X, (u16)u.Y, (u16)u.Z};
            u64 mID, flags;
            ifs.read((i8*)&mID, sizeof(u64));
            ifs.read((i8*)&flags, sizeof(u64));
            go.model = mID + modelCount;
            go.flags = flags;
            d->gameObjects->push_back(go);
        }
    }
}

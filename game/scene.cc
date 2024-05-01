#include "../types.hh"
#include "../logging.hh"
#include <fstream>
#include "scene.hh"
#include <iostream>
using std::string;
using std::vector;
#define readStrWithSize(ifs)                    \
    ifs.read((i8*)&size1, 1);                   \
    u8 str[size1];                              \
    ifs.read((i8*)str, size1);
#define readVector3(ifs, v)                                             \
    ifs.read((i8 *)&v.X, sizeof(fx));                                   \
    ifs.read((i8 *)&v.Y, sizeof(fx));                                   \
    ifs.read((i8 *)&v.Z, sizeof(fx));
extern "C" {
    GameData* d;
    uint si;
    void LoadSceneFile(Scene scene);
    void Init(GameData* gd, uint scriptIndex){
        d = gd;
        si = scriptIndex;
        void** ptr = (void**)gd->stream;
        ptr[0] = (u8*)&LoadSceneFile;
        GameObject go = {};
        go.script = si;
    }
    
    bool iSCL = false;
    std::vector<uint> toUnload;
    Scene scene;
    void LoadSceneFile(Scene scene){
        // Section 0 - Unload previous(subscene = no)
        // Section 1 - Load models
        // Section 2 - Load scripts
        // Section 3 - Load gameObjects
        Debg("Loading scene...");
        std::ifstream ifs(scene.scenePath);
        u16 size0, size1;

        // Section 0 - Unload previous(is not a subscene)
        u8 unload;
        ifs.read((i8*)&unload, 1);
        if(unload){
            if(iSCL)Warn("Unloading unloaded scene!");
            else if(scene.canBeUnloaded)
                Warn("Scene unloading not permited!");
            else{
                for(u32 i =0; i < toUnload.size();i++){
                    Model* m = (Model*)d->getResource(0, toUnload[i]);
                    for(u32 j =0; j < m->faces.size(); j++)
                        delete[] m->faces[j].m;
                    m->verticies.clear();
                    m->faces.clear();
                    m->freed = 1;
                    m->uId = 0;
                }
            }
        }
        
        // Section 1 - Load models
        ifs.read((i8*)&size0, 2);
        for(u32 i =0; i < size0; i++){
            readStrWithSize(ifs);
            u32 id = d->loadObject((i8*)str);
            Model* m = (Model*)d->getResource(3, id);
            toUnload.push_back(m->uId);
        }
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
            GameObject go;
            readVector3(ifs, v); go.position = v;
            readVector3(ifs, v); go.rotation = v;
            readVector3(ifs, v); go.scale = v;
            readVector3(ifs, v); go.coll = v;
            readVector3(ifs, v); go.color = {(u16)v.X, (u16)v.Y, (u16)v.Z}; // color
            u64 mID, flags;
            ifs.read((i8*)&mID, sizeof(u64));
            std::cout << mID << "\n";
            ifs.read((i8*)&flags, sizeof(u64));
            mID = (u64)d->getResource(2, mID);
            go.flags = flags;
            go.model = 1;
            d->gameObjects->push_back(go);
        }
    }
}

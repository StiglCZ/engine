#include "scripting.hh"
#include "file.hh"
#include "math.hh"
#include "program.hpp"
#include "native.hh"
#include "renderer.hh"
#include "logging.hh"
#include <signal.h>
#include <vector>
#include <chrono>

// Fallback script functions
void d_init(void *gd, uint scriptIndex) {
    (void)scriptIndex;
    (void)gd;
}
void d_other(uint index) {
    (void)index;
}

std::vector<Model> modelBuffer;
std::vector<Script> scriptBuffer;

uint loadObject(const char *name) {
    Debg("Loading model  " + std::string(name));
    Model m = loadModel(std::string(assetsDir + name));
    for(u32 i =0; i < modelBuffer.size(); i++)
        if(modelBuffer[i].freed){
            modelBuffer[i] = m;
            return i;
        }
    modelBuffer.push_back(m);
    return modelBuffer.size() - 1;
}

// Load script and get pointer to needed functions
uint loadScript(const char *name, void* gd) {
    Script s;
    s.handle = loadScript((char*)(scriptsDir + std::string(name)).c_str());
    s.init   = getFunction<void (*)(void*, uint)>(s.handle, (char*)initFName);
    s.start  = getFunction<void (*)(uint)>(s.handle, (char*)startFname);
    s.update = getFunction<void (*)(uint)>(s.handle, (char*)updateFname);

    // Check if something is missing
    if(!s.handle)return 0;
    if(!s.init  )s.init   = d_init;
    if(!s.start )s.start  = d_other;
    if(!s.update)s.update = d_other;

    scriptBuffer.push_back(s);
    s.init(gd, scriptBuffer.size()-1);
    
    return scriptBuffer.size() -1;
}


// TODO: Fix
// Currently not working with some Y rotation angles( > 90 - < 0)

// Determines if the object should be rendered based on its
// position and the camera position and rotation
bool CalcCull(CamProps* cullConf, Vector3 pos) {
    Vector2 cullAxis = {
        (fx)cos(cullConf->rot.Y),
        (fx)sin(cullConf->rot.Y)
    };
    Point results;
    results.X = cullAxis.X > 0;
    results.Y = cullAxis.Y > 0;
    Vector3 diff = pos - cullConf->pos;
    u8 Q1 = diff.X > 0, Q2 = diff.Y > 0;
    return
         (Q1 &&  results.X &&  Q2 &&  results.Y) ||
        (!Q1 && !results.X && !Q2 && !results.Y);
}

// Utils
void CamSync(CamProps* cp) {
    matrix3x3 m;
    rotateW(m, cp->rot);
    matrixCopy(viewMatrix, m);
}

/*
  Instead of adding everything to gamedata
  such as buffers, etc., I decided to make
  this function instead, not only making
  gamedata struct smaller, but also extending
  security since you cannot edit the
  vectors directly
*/
void* getResource(u8 type, u64 uID) {
    switch(type){
        case RESOURCE_Script:
            return &scriptBuffer[uID];
        case RESOURCE_Model_by_permanent_id:
            for(u32 i =0; i < modelBuffer.size(); i++)
                if(modelBuffer[i].uId == uID)
                    return &modelBuffer[i];
            break;
        case RESOURCE_Model_by_temporary_id:
            return &modelBuffer[uID];
        case RESOURCE_Keycode:
            return (void*)(u64)GetKeyFromString((char*)uID);
    }
    return 0;
}

int main() {
    Spec("Stigl Game Engine - c2024");
    bool mouseCentered = false;
    u32
        deltaTime = 0,
        lastObjectCount = 0,
        gameObjectUID = 0;
    std::vector<GameObject> gameObjects;
    CamProps camProps{{}, {}, CamSync, 0, FOV, FAR, NEAR};
    u8 stream[STREAM_SIZE];
    GameData gd = {
        .gameObjects = &gameObjects,
        .exitFuncs = &exitFuncs,
        
        .mouseCentered = &mouseCentered,
        .isGameRunning = &isRunning,

        .viewMatrix = &viewMatrix,
        .projMatrix = &projMatrix,
        
        .streamSize = STREAM_SIZE,

        .W = SIZE_X,
        .H = SIZE_Y,
        .AR = AR,

        .deltaTime = &deltaTime,
        .gameObjUID = &gameObjectUID,
        .cp = &camProps,

        .mouse = &mouse,
        .keys = keys,
        .btns = button,
        .stream = stream,

        .loadObject = loadObject,
        .loadScript = loadScript,
        .drawText = DrawText,
        .screenShot = Screenshot,
        .getResource = getResource,
    };
    modelBufferPtr = &modelBuffer;
    
    modelBuffer.push_back((Model){});                      // Fallback model
    scriptBuffer.push_back({d_init, d_other, d_other, 0}); // Fallback script
    
    signal(SIGINT, Exiter);
    
    Info("Loading scripts...");
    std::vector<std::string> scripts = getFiles(scriptsDir);
    
    if(!scripts.size()){
        Err("ERR: Script/s not found!", -1);
        exit(1);
    }
    
    // Main script at index 1
    // Loads all scripts and orders them
    for(auto str : scripts)
        if(str == "main.so" || str == "init.so")
            loadScript((char*)str.c_str(), &gd);
    if(scriptBuffer.size() == 1)
        Err("Main script not found!", 1);

    NativeInit(SIZE_X, SIZE_Y, (char*)Title);
    while(isRunning){
        u64 intialTime = time();
        if(mouseCentered) CenterMouse();
        { // Scripting function runners
            // Start function
            if(gameObjects.size() > lastObjectCount){
                for(u32 i = lastObjectCount; i < gameObjects.size();i++){
                    scriptBuffer[gameObjects[i].script].start(i);
                    gameObjects[i].id = gameObjectUID++;
                }
                lastObjectCount = gameObjects.size();
            }
            
            // Update function
            for(u32 i =0; i < gameObjects.size() && isRunning;i++)
                if(gameObjects[i].script < scriptBuffer.size())
                    scriptBuffer[gameObjects[i].script].update(i);
        }
        
        { // Rendering
            for(u32 i =0; i < gameObjects.size();i++){
                if(gameObjects[i].model == 0 || modelBuffer[gameObjects[i].model].freed ||
                   magnitude(gameObjects[i].position - camProps.pos) > FAR ||
                   (camProps.cullEnabled && CalcCull(&camProps, gameObjects[i].position))
                  )continue;
                ChangeColor(gameObjects[i].color);

                matrix4x4 modelMatrix = {};
                rotateW(modelMatrix, gameObjects[i].rotation);
                Vector3 pos = gameObjects[i].position + camProps.pos;
                modelMatrix[0][3] = pos.X;
                modelMatrix[1][3] = pos.Y;
                modelMatrix[2][3] = pos.Z;
                
                Vector3 scale = gameObjects[i].scale;
                // Fixes the scaling on X and Z rotation
                // Y rotation doesn't seem to be nearly as broken
                // Isn't perfect!
                scale.Y /=
                    (fabs(sin(gameObjects[i].rotation.X)) + 1) *
                    (fabs(sin(gameObjects[i].rotation.Z)) + 1) *
                    (fabs(sin(camProps.rot.X)) + 1) *
                    (fabs(sin(camProps.rot.Z)) + 1);
                scale.Z *=
                    (fabs(sin(gameObjects[i].rotation.X)) + 1) *
                    (fabs(sin(gameObjects[i].rotation.Z)) + 1) *
                    (fabs(sin(camProps.rot.X)) + 1) *
                    (fabs(sin(camProps.rot.Z)) + 1);
                
                DrawModel(&modelBuffer[gameObjects[i].model], modelMatrix, &scale);
            }
        }
        deltaTime = time() - intialTime;                            // Calculate the frametime
        FrameFinished();                                            // Switch the framebuffers
        usleep(min<u32>(SLEEP_TIME - deltaTime, SLEEP_TIME));       // Prevent GPU overload from too many frames(max in case of overflow)
    }
    // Wait for exit
    while(1);
}

u64 time() {
    using namespace std::chrono;
    return duration_cast<microseconds>
        (system_clock::now().time_since_epoch())
        .count();
}

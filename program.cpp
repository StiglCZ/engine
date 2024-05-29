#include "scripting.hh"
#include "renderer.hh"
#include "program.hpp"
#include "logging.hh"
#include "native.hh"
#include "file.hh"
#include "math.hh"

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

uint loadModel(const char *name) {
    Debg("Loading model  " + std::string(name));
    Model m = loadModel(std::string(assetsDir + name));
    // Defragments the modelbuffer
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

    u32 count = scriptBuffer.size();
    scriptBuffer.push_back(s);
    s.init(gd, count);
    
    return count;
}

// Utils
void CamSync(CamProps* cp, bool updateProjection) {
    matrix3x3 m;
    rotateW(m, cp->rot);
    matrixCopy(viewMatrix, m);
    if(updateProjection){
        const fx
            xp = 1.0 / tan(rad(FOV) / 2.0),
            yp = xp / AR,
            np = (-NEAR - FAR) / AR,
            fp = 2.0 * FAR * NEAR / AR;
        matrix4x4 newProjMatrix = {
            {xp, 00, 00, 00},
            {00, yp, 00, 00},
            {00, 00, np, fp},
            {00, 00, 01, 01},
        };
        matrixCopy(projMatrix, newProjMatrix);
    }
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
        case RESOURCE_Model_Count:
            return (void*)(u64)modelBuffer.size();
    }
    return 0;
}

void RenderGameObject(GameObject& go, CamProps& cp) {
    if(go.model == 0 || modelBuffer[go.model].freed ||
       magnitude(go.position - cp.pos) > FAR
      )return;
    ChangeColor(go.color);
    
    matrix4x4 modelMatrix = {};
    Vector3 pos = go.position + cp.pos;
    fillMat(modelMatrix, pos, go.rotation);
    
    Vector3 scale = go.scale;
    // After I've seen minecrafts scaling on rotation, I would consider this acceptable implementation
    // Fixes the scaling when X and Z rotation are non zero
    fx scaleFactor =
        (fabs(sin(go.rotation.X)) + 1) *
        (fabs(sin(go.rotation.Z)) + 1) *
        (fabs(sin(cp.rot.X)) + 1) *
        (fabs(sin(cp.rot.Z)) + 1);
    scale.Y /= scaleFactor;
    scale.Z *= scaleFactor;
                
    DrawModel(&modelBuffer[go.model], modelMatrix, &scale);
}

int main() {
    Spec("Stigl Game Engine - c2024");
    bool mouseCentered = false;
    u32
        deltaTime = 0,
        lastObjectCount = 0,
        gameObjectUID = 0;
    
    u8  stream[STREAM_SIZE];
    std::vector<GameObject> gameObjects;
    
    CamProps camProps = {
        .pos  = {0, 0, 0},
        .rot  = {0, 0, 0},
        .sync = CamSync,
        .fov  = FOV,
        .far  = FAR,
        .near = NEAR,
        .ar   = AR,
        .w    = SIZE_X,
        .h    = SIZE_Y,
    };
    
    GameData gd = {
        .gameObjects = &gameObjects,
        .exitFuncs = &exitFuncs,
        
        .mouseCentered = &mouseCentered,
        .isGameRunning = &isRunning,

        .viewMatrix = &viewMatrix,
        .projMatrix = &projMatrix,
        
        .streamSize = STREAM_SIZE,
        .sleepTime  = SLEEP_TIME,

        .deltaTime  = &deltaTime,
        .gameObjUID = &gameObjectUID,
        .backColor  = &backColor,
        .cp = &camProps,

        .mouse  = &mouse,
        .keys   = keys,
        .btns   = button,
        .stream = stream,

        .loadModel   = loadModel,
        .loadScript  = loadScript,
        .drawText    = DrawText,
        .screenShot  = Screenshot,
        .getResource = getResource,
    };
    modelBufferPtr = &modelBuffer;
    
    modelBuffer.push_back((Model){});                              // Fallback model
    scriptBuffer.push_back((Script){d_init, d_other, d_other, 0}); // Fallback script

    Info("Loading scripts...");
    if(eDirectory((scriptsDir + mainScript).c_str()) != 1)
        Err("ERR: Main script(main.so) hasn't been found!(Wrong directory?)", -1);
    
    // Main script at index 1
    loadScript((char*)mainScript.c_str(), &gd);

    if(scriptBuffer.size() <= 1 || !scriptBuffer[1].handle)
        Err("Main script could not be loaded!", 1);

    NativeInit(SIZE_X, SIZE_Y, (char*)Title);
    while(isRunning){
        u64 intialTime = time();
        if(mouseCentered) CenterMouse();
        { // Scripting function runners
            // Start function
            if(gameObjects.size() > lastObjectCount){
                for(u32 i = lastObjectCount; i < gameObjects.size();i++){
                    gameObjects[i].id = gameObjectUID++;
                    scriptBuffer[gameObjects[i].script].start(i);
                }
                lastObjectCount = gameObjects.size();
            }
            
            // Update function
            for(u32 i =0; i < gameObjects.size() && isRunning;i++)
                if(gameObjects[i].script < scriptBuffer.size())
                    scriptBuffer[gameObjects[i].script].update(i);
        }

        // Render all the objects
        for(u32 i =0; i < gameObjects.size() && isRunning;i++)
            RenderGameObject(gameObjects[i], camProps);
        
        deltaTime = time() - intialTime;                            // Calculate the frametime
        FrameFinished();                                            // Switch the framebuffers
        usleep(min<u32>(SLEEP_TIME - deltaTime, SLEEP_TIME));       // Prevent GPU overload from too many frames
    }
    // Wait for exit
    while(1);
}

inline u64 time() {
    using namespace std::chrono;
    return duration_cast<microseconds>
        (system_clock::now().time_since_epoch())
        .count();
}

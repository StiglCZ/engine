struct SceneRequest {
    bool shouldBeLoaded = false;
    bool shouldBeUnloaded = false;
    char fileName[64] = "";
};

struct SceneInfo {
    bool isSceneActive = false;
    char *fileName;
};

struct Scene {
    bool canBeUnloaded = false;
    char scenePath[256];
    void* toUnload;
};

typedef struct SceneRequest SceneRequest;
typedef struct SceneInfo SceneInfo;
typedef struct Scene Scene;

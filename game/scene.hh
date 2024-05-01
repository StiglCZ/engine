struct Scene {
    bool canBeUnloaded = false;
    char scenePath[256];
    void* toUnload;
};

typedef struct SceneRequest SceneRequest;
typedef struct SceneInfo SceneInfo;
typedef struct Scene Scene;

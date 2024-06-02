#ifndef SCENE
#define SCENE
#define LOAD_SCENE(file)                        \
    Scene scene = {file};                       \
    ((void(*)(const char*))*(void**)d->stream)(scene);

struct Scene {
    char scenePath[256];
};

typedef struct Scene Scene;

#endif

#ifndef AUDIO
#define AUDIO
#include "../types.hh"

#define AUDIO_CONTROL(GD) (AudioControl *)((void **)GD->stream)[3]

struct AudioSource {
    Vector3
        sourcePosition,
        targetPosition,
        velocity;
    float volume;
    u32 track, sourcen;
    bool is3D = true;
    void (*update)(AudioSource&);
};

struct AudioControl {
    u32 (*const LoadTrack)(const char *fileName);
    // Non 3D
    AudioSource (*const Play)(u32 track, float volume);
    AudioSource (*const Play3D)(Vector3 srcPos, Vector3 targetPos, Vector3 vel, u32 track, float volume);
    void (*const PlayFromSrc)(AudioSource& src);
    void (*const UpdateSource)(AudioSource&);
};

#endif

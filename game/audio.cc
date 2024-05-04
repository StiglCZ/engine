#include "audio.hh"
#include "../logging.hh"

#include <AL/al.h>
#include <AL/alc.h>
#include <sndfile.h>
#include <vector>
    
ALCdevice *device;
ALCcontext *context;

std::vector<ALuint> sources;
std::vector<ALuint> buffers;

void AudioInit() {
    device = alcOpenDevice(NULL);
    context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);
    alListenerf(AL_GAIN, 1.0);
}
u32 LoadTrack(const char *fileName) {
    SNDFILE *sndfile;
    SF_INFO sfinfo;
    sndfile = sf_open(fileName, SFM_READ, &sfinfo);
    if(!sndfile)
        Warn(std::string("Track ") + fileName + std::string(" not found!"));
    ALsizei
        num_samples = sfinfo.frames * sfinfo.channels,
        sample_rate = sfinfo.samplerate * 2, // TODO: Figure out why and fix
        buffer_size = num_samples * sizeof(u16);
    short *buffer = (short *)malloc(buffer_size);
    sf_read_short(sndfile, buffer, num_samples);
    ALuint al_buffer;
    alGenBuffers(1, &al_buffer);
    alBufferData(al_buffer, AL_FORMAT_MONO16, buffer, buffer_size, sample_rate);
    free(buffer);
    sf_close(sndfile);
    buffers.push_back(al_buffer);
    return al_buffer;
}

void UpdateAudioSource(AudioSource& as){
    if(!as.sourcen){
        alGenSources(1, &as.sourcen);
        sources.push_back(as.sourcen);
    }
    alSourcef(as.sourcen, AL_GAIN, as.volume);
    if(as.is3D){
        Vector3 position = as.targetPosition - as.sourcePosition;
        ALfloat
            alPos[] = {position.X, position.Y, position.Z},
            alVel[] = {as.velocity.X, as.velocity.Y, as.velocity.Z};
        alSourcefv(as.sourcen, AL_POSITION, alPos);
        alSourcefv(as.sourcen, AL_VELOCITY, alVel);
    }
}
AudioSource Play3D(Vector3 srcPos, Vector3 targetPos, Vector3 vel, u32 track, float volume) {
    AudioSource as {
        {srcPos}, {targetPos}, {vel},
        volume,
        track,
        0,
        true,
        UpdateAudioSource
    };
    UpdateAudioSource(as);
    
    alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
    alSourcei(as.sourcen, AL_BUFFER, track);
    alSourcePlay(as.sourcen);
    return as;
}
AudioSource Play(u32 track, float volume) {
    AudioSource as = {{}, {}, {},
        volume,
        track,
        0,
        false,
        UpdateAudioSource
    };
    UpdateAudioSource(as);
    
    alSourcei(as.sourcen, AL_BUFFER, track);
    alSourcePlay(as.sourcen);
    return as;
}
void PlayFromSource(AudioSource& as) {
    UpdateAudioSource(as);
    if(as.is3D)alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
    alSourcei(as.sourcen, AL_BUFFER, as.track);
    alSourcePlay(as.sourcen);
}
void AudioDestroy() {
    Info("Shuting down audio...");
    for(u32 i =0; i < sources.size(); i++)
        alDeleteSources(1, &sources[i]);
    for(u32 i =0; i < buffers.size(); i++)
        alDeleteBuffers(1, &buffers[i]);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

const AudioControl ac = {
    LoadTrack,
    Play,
    Play3D,
    PlayFromSource,
    UpdateAudioSource
};

extern "C" {
    void Init(GameData* d, u32 si) {
        (void)si; // Unused
        ((void**)d->stream)[3] = (void*)&ac;
        AudioInit();
        d->exitFuncs->push_back((void*)AudioDestroy);
    }
}

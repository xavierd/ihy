#ifndef DEF_AO
#define DEF_AO

#include <CoreFoundation/CoreFoundation.h>
#include <AudioToolbox/AudioQueue.h>

#define NBBUFFERS 3

typedef void (*DecodeFunction)(void *Data, int chunk, void *out);
typedef int (*NbChunk)(void *Data);

typedef struct ao_device
{
    AudioQueueRef			queue;
    bool				isPlaying;
    AudioQueueBufferRef			mBuffers[NBBUFFERS];
    AudioStreamBasicDescription		mDataFormat;
    void				*Data;
    int					Chunk;
    void				(*DecodeFunction)
					(void *Data, int chunk, void *out);
    int					(*NbChunk)(void *Data);
} ao_device;

ao_device *ao_init_device(int BitsPerSample, int NumChannels, int SampleRate);
void ao_play(ao_device *device);

#endif

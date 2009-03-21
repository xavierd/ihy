#ifndef DEF_AO
#define DEF_AO

#include <CoreFoundation/CoreFoundation.h>
#include <AudioToolbox/AudioQueue.h>

#define NBBUFFERS 3

typedef struct CallbackStruct
{
    AudioQueueRef			queue;
    UInt32				framecount;
    AudioQueueBufferRef			mBuffers[NBBUFFERS];
    AudioStreamBasicDescription		mDataFormat;
} ao_device;

ao_device *ao_init_device(int BitsPerSample, int NumChannels, int SampleRate);
void ao_close_device(ao_device *device);
void ao_play_samples(ao_device *device, void *array, int size);

#endif

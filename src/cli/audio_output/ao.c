#include "ao.h"

static void BufferCallback(void *in, AudioQueueRef inQ, AudioQueueBufferRef out)
{
    int i;

    /* get the device */
    ao_device *inData = (ao_device *)in;
    short *coreAudioBuffer = (short *)outQB->mAudioData;

}

void ao_play_samples(ao_device *device, void *array, int size)
{
}

void ao_close_device(ao_device *device)
{
    AudioQueueDispose(device->queue);
    free(device);
}

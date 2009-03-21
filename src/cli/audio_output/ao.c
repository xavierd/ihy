#include "ao.h"

static void BufferCallback(void *in, AudioQueueRef inQ, AudioQueueBufferRef out)
{
    int i;

    /* get the device */
    ao_device *inData = (ao_device *)in;
    short *coreAudioBuffer = (short *)outQB->mAudioData;

}

ao_device *ao_init_device(int BitsPerSample, int NumChannels, int SampleRate)
{
    ao_device *in;
    int i;
    UInt32 bufferBytes;

    in = malloc(sizeof(CallbackStruct));
    in->mDataFormat.mSampleRate = SampleRate;
    in->mDataFormat.mFormatID = kAudioFormatLinearPCM;
    in->mDataFormat.mFormatFlags = kLinearPCMFormatFLagIsSignedInteger;
    in->mDataFormat.mFormatFlags |= kAudioFormatFlagIsPacked;
    in->mDataFormat.mFramesPerPacket = 1;
    in->mDataFormat.mChannelsPerFrame = NumChannels;
    in->mDataFormat.mBitsPerChannel = BitsPerSample;
    in->mDataFormat.mBytesPerFrame = BitsPerSample * NumChannels / 8;
    in->mDataFormat.mBytesPerPacket = in->mDataFormat.mBytesPerFrame;

    AudioQueueNewOutput(&in->mDataFormat, BufferCallback, in, CFRunLoopGetCurrent(), kCFRunLoopCommonModes, 0, &in->queue);
    in->framecount = 65536;
    bufferBytes = in->framecount * in->mDataFormat.mBytesPerFrame;

    for (i = 0; i < NBBUFFERS; i++)
    {
	AudioQueueAllocateBuffer(in->queue, bufferBytes, &in->mBuffers[i]);
	AQBufferCallback(in, in->queue, in->mBuffers[i]);
    }
    AudioQueueSetParamater(in->queue, kAudioQueueParam_Volume, 1.0);

    return in;
}

void ao_play_samples(ao_device *device, void *array, int size)
{
    device->framecount = size;
}

void ao_close_device(ao_device *device)
{
    AudioQueueDispose(device->queue);
    free(device);
}

#include "ao.h"

static void Callback(void *in, AudioQueueRef inQ, AudioQueueBufferRef out)
{
    ao_device *dev = (ao_device *)in;
    short *audioBuffer = (short *)out->mAudioData;

    if (!dev->isPlaying)
	return;
    if (dev->Chunk < dev->NbChunk(dev->Data))
    {
	dev->DecodeFunction(dev->Data, dev->Chunk, audioBuffer);
	out->mAudioDataByteSize = 65536 * 2;
	AudioQueueEnqueueBuffer(inQ, out, 0, NULL);
	dev->Chunk++;
    }
    else
    {
	AudioQueueStop(dev->queue, false);
	dev->isPlaying = false;
    }
}

ao_device *ao_init_device(int BitsPerSample, int NumChannels, int SampleRate)
{
    ao_device *in;

    in = malloc(sizeof(ao_device));
    in->mDataFormat.mSampleRate = SampleRate;
    in->mDataFormat.mFormatID = kAudioFormatLinearPCM;
    in->mDataFormat.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger;
    in->mDataFormat.mFormatFlags |= kAudioFormatFlagIsPacked;
    in->mDataFormat.mFramesPerPacket = 1;
    in->mDataFormat.mChannelsPerFrame = NumChannels;
    in->mDataFormat.mBitsPerChannel = BitsPerSample;
    in->mDataFormat.mBytesPerFrame = BitsPerSample * NumChannels / 8;
    in->mDataFormat.mBytesPerPacket = in->mDataFormat.mBytesPerFrame;
    in->Chunk = 0;

    return in;
}

void ao_play(ao_device *device)
{
    int i;
    int bufferBytes;

    AudioQueueNewOutput(&device->mDataFormat, Callback, device,
	    CFRunLoopGetCurrent(), kCFRunLoopCommonModes, 0, &device->queue);

    device->isPlaying = true;
    bufferBytes = device->mDataFormat.mBitsPerChannel * 65536 / 8;
    for (i = 0; i < NBBUFFERS; i++)
    {
	AudioQueueAllocateBuffer(device->queue, bufferBytes,
							&device->mBuffers[i]);
	Callback(device, device->queue, device->mBuffers[i]);
    }
    AudioQueueSetParameter(device->queue, kAudioQueueParam_Volume, 1.0);

    AudioQueueStart(device->queue, NULL);
    while (device->isPlaying)
	CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.25, false);

    AudioQueueDispose(device->queue, true);
    free(device);
}

#include "ao.h"

void play_wav(wav_data *wav)
{
    ao_device *device;
    ao_sample_format format;
    int default_driver;

    ao_initialize();

    default_driver = ao_driver_id("oss");
    format.bits = wav->BitsPerSample;
    format.channels = wav->NumChannels;
    format.rate = wav->SampleRate;
    format.byte_format = AO_FMT_NATIVE;

    device = ao_open_live(default_driver, &format, NULL);
    if (!device)
	exit(0);

    ao_play(device, (char *) wav->Data, wav->DataBlocSize);

    ao_close(device);
    ao_shutdown();
}

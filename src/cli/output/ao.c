#include "ao.h"

/* return the device, to play sound in streaming */
ao_device *init_device(int BitsPerSample, int NumChannels, int SampleRate)
{
    ao_device *res;
    ao_sample_format format;
    int driver;

    ao_initialize();

#ifdef linux
    driver = ao_default_driver_id();
#else
    driver = ao_driver_id("oss");
#endif
    format.bits = BitsPerSample;
    format.channels = NumChannels;
    format.rate = SampleRate;
    format.byte_format = AO_FMT_NATIVE;
    res = ao_open_live(driver, &format, NULL);
    if (!res)
	exit(0);
    return res;
}

/* close the device, and do some cleaning */
void close_device(ao_device *device)
{
    ao_close(device);
    ao_shutdown();
}

/* play the content of array */
void play(ao_device *device, void *array, int size)
{
    ao_play(device, (char *)array, size);
}

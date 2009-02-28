#include "ao.h"

/* return the device, to play sound in streaming */
ao_device *ao_init_device(int BitsPerSample, int NumChannels, int SampleRate)
{
    ao_device *res;
    ao_sample_format format;
    int driver;

    ao_initialize();

#ifdef linux
    driver = ao_driver_id("alsa");
#else
# ifdef BSD
    driver = ao_driver_id("oss");
# else
    driver = ao_default_driver();
# endif
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
void ao_close_device(ao_device *device)
{
    ao_close(device);
    ao_shutdown();
}

void ao_play_samples(ao_device *device, void *array, int size)
{
    ao_play(device, array, size);
}

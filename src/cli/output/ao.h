#ifndef DEF_AO
#define DEF_AO

#include <ao/ao.h>

/*
#include <input/wav.h>
*/

/* return the device, to play sound in streaming */
ao_device *init_device(int BitsPerSample, int NumChannels, int SampleRate);

/* close the device, and do some cleaning */
void close_device(ao_device *device);

/* play the content of array */
void play(ao_device *device, void *array, int size);

/*
void play_wav(wav_data *wav);
*/

#endif

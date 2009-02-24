#include "wav_streaming.h"

struct wav_streaming_data
{
    t_buffer	buffer;
    wav_data	*wav;
};

static void *wav_playing(void *data)
{
    t_buffer buffer = ((struct wav_streaming_data *)data)->buffer; /* contains int8_t */
    wav_data *wav = ((struct wav_streaming_data *)data)->wav;
    ao_device *audio_device;
    int8_t *to_play;

    audio_device = ao_init_device(wav->BitsPerSample,
				  wav->NumChannels,
				  wav->SampleRate);
    /* il se peut que ca crée des soucis, si filling_buffer n'a rien ajouté */
    while (!buffer_isempty(buffer))
    {
	to_play = buffer_get(buffer);
	ao_play_samples(audio_device, to_play, 65536 /* pour tester */);
    };
    ao_close_device(audio_device);
    return NULL;
}

static void *wav_filling_buffer(void *data)
{
    t_buffer buffer = ((struct wav_streaming_data *)data)->buffer; /* contains int8_t */
    wav_data *wav = ((struct wav_streaming_data *)data)->wav;
    int8_t *to_add;
    unsigned int i = 0;

    while (i < wav->DataBlocSize)
    {
	to_add = wav->Data + i;
	buffer_add(to_add, buffer);
	i += 65536;
    }
    return NULL;
}

/* play the wav in streaming */
void play_wav_streaming(wav_data *wav)
{
    t_buffer buffer; /* contains int8_t */
    pthread_t playing_thread, filling_buffer_thread;
    struct wav_streaming_data data;

    buffer = buffer_init(10); /* max 10 elements */
    data.wav = wav;
    data.buffer = buffer;
    pthread_create(&filling_buffer_thread, NULL, &wav_filling_buffer, &data);
    pthread_create(&playing_thread, NULL, &wav_playing, &data);
    pthread_join(filling_buffer_thread, NULL);
    pthread_join(playing_thread, NULL);
}

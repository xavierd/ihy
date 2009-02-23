#include "wav.h"

/* return a new allocated wav */
wav_data *create_wav()
{
    wav_data *result;

    result = malloc(sizeof(wav_data));
    result->Data = NULL;

    return result;
}

/* read a wav PCM file from disk */
void read_wav(const char *filename, wav_data *data)
{
    FILE *file;

    /* Open file for reading */
    file = fopen(filename, "rb");
    if (!file)
	exit(1);
    /* Read RIFF header */
    fread(data->ChunkID, sizeof(char), 4, file);
    fread(&data->ChunkSize, sizeof(uint32_t), 1, file);
    fread(data->Format, sizeof(char), 4, file);
    /* Read fmt header */
    fread(data->FormatBlocID, sizeof(char), 4, file);
    fread(&data->FormatBlocSize, sizeof(uint32_t), 1, file);
    fread(&data->AudioFormat, sizeof(uint16_t), 1, file);
    fread(&data->NumChannels, sizeof(uint16_t), 1, file);
    fread(&data->SampleRate, sizeof(uint32_t), 1, file);
    fread(&data->ByteRate, sizeof(uint32_t), 1, file);
    fread(&data->BlockAlign, sizeof(uint16_t), 1, file);
    fread(&data->BitsPerSample, sizeof(uint16_t), 1, file);
    /* Read data */
    fread(data->DataBlocID, sizeof(char), 4, file);
    fread(&data->DataBlocSize, sizeof(uint32_t), 1, file);
    data->Data = malloc(data->DataBlocSize);
    fread(data->Data, data->DataBlocSize, sizeof(int8_t), file);

    fclose(file);
}

/* write a wav to the disk */
void write_wav(const wav_data *data, const char *filename)
{
    FILE *file;

    /* Open file for writing */
    file = fopen(filename,"wb");
    if (!file)
	exit(1);
    /* Write RIFF header */
    fwrite(data->ChunkID, sizeof(char), 4, file);
    fwrite(&data->ChunkSize, sizeof(uint32_t), 1, file);
    fwrite(data->Format, sizeof(char), 4, file);
    /* Write fmt header */
    fwrite(data->FormatBlocID, sizeof(char), 4, file);
    fwrite(&data->FormatBlocSize, sizeof(uint32_t), 1, file);
    fwrite(&data->AudioFormat, sizeof(uint16_t), 1, file);
    fwrite(&data->NumChannels, sizeof(uint16_t), 1, file);
    fwrite(&data->SampleRate, sizeof(uint32_t), 1, file);
    fwrite(&data->ByteRate, sizeof(uint32_t), 1, file);
    fwrite(&data->BlockAlign, sizeof(uint16_t), 1, file);
    fwrite(&data->BitsPerSample, sizeof(uint16_t), 1, file);
    /* Write data */
    fwrite(data->DataBlocID, sizeof(char), 4, file);
    fwrite(&data->DataBlocSize, sizeof(uint32_t), 1, file);
    fwrite(data->Data, data->DataBlocSize, sizeof(int8_t), file);

    fclose(file);
}

struct streaming_data
{
    t_buffer	buffer;
    wav_data	*wav;
};

static void *playing(void *data)
{
    t_buffer buffer = ((struct streaming_data *)data)->buffer; /* contains int8_t */
    wav_data *wav = ((struct streaming_data *)data)->wav;
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

static void *filling_buffer(void *data)
{
    t_buffer buffer = ((struct streaming_data *)data)->buffer; /* contains int8_t */
    wav_data *wav = ((struct streaming_data *)data)->wav;
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
    struct streaming_data data;

    buffer = buffer_init(10); /* max 10 elements */
    data.wav = wav;
    data.buffer = buffer;
    pthread_create(&filling_buffer_thread, NULL, &filling_buffer, &data);
    pthread_create(&playing_thread, NULL, &playing, &data);
    pthread_join(filling_buffer_thread, NULL);
    pthread_join(playing_thread, NULL);
}

/* release the memory used by a wav */
void destroy_wav(wav_data *data)
{
    free(data->Data);
    free(data);
}

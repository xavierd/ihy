#include <stdio.h>
#include <stdlib.h>

#include "pcm.h"

wav_file *create_pcm()
{
    wav_file *result;

    result = malloc(sizeof(wav_file));
    result->Data = NULL;
    return result;
}

void write_pcm(const wav_file *data, const char *filename)
{
    FILE *file;

    /* Open file for writing */
    file = fopen(filename, "wb");
    if (!file)
	exit(1);
    /* write RIFF header */
    fwrite(&data->ChunkID, sizeof(char), 4, file);
    fwrite(&data->ChunkSize, sizeof(uint32_t), 1, file);
    fwrite(&data->Format, sizeof(char), 4, file);
    /* write fmt header */
    fwrite(&data->FormatBlocID, sizeof(char), 4, file);
    fwrite(&data->FormatBlocSize, sizeof(uint32_t), 1, file);
    fwrite(&data->AudioFormat, sizeof(uint16_t), 1, file);
    fwrite(&data->NumChannels, sizeof(uint16_t), 1, file);
    fwrite(&data->SampleRate, sizeof(uint32_t), 1, file);
    fwrite(&data->ByteRate, sizeof(uint32_t), 1, file);
    fwrite(&data->BlockAlign, sizeof(uint16_t), 1, file);
    fwrite(&data->BitsPerSample, sizeof(uint16_t), 1, file);
    /* write data */
    fwrite(&data->DataBlocID, sizeof(char), 4, file);
    fwrite(&data->DataBlocSize, sizeof(uint32_t), 1, file);
    fwrite(&data->Data, sizeof(int8_t), data->DataBlocSize, file);

    fclose(file);
}

void destroy_pcm(wav_file *data)
{
    free(data->Data);
    free(data);
}

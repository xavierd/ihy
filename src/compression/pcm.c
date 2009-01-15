#include <stdio.h>
#include <stdlib.h>

#include "pcm.h"

static void read_data(wav_file *wav, FILE *file);

wav_file *read_wav(const pcm_data *data, const char *filename)
{
    FILE *file;
    wav_file *result;

    file = fopen(filename, "rb");
    if (!file)
	exit(1);
    result = malloc(sizeof(wav_file));
    /* RIFF header */
    fread(result->ChunkID, 1, 4 * sizeof(char), file);
    fread(&result->ChunkSize, 1, sizeof(uint32_t), file);
    fread(result->Format, 1, 4 * sizeof(char), file);
    /* fmt */
    fread(result->FormatBlocID, 1, 4 * sizeof(char), file);
    fread(&result->FormatBlocSize, 1, sizeof(uint32_t), file);
    fread(&result->AudioFormat, 1, sizeof(uint16_t), file);
    fread(&result->NumChannels, 1, sizeof(uint16_t), file);
    fread(&result->SampleRate, 1, sizeof(uint32_t), file);
    fread(&result->ByteRate, 1, sizeof(uint32_t), file);
    fread(&result->BlockAlign, 1, sizeof(uint16_t), file);
    fread(&result->BitsPerSample, 1, sizeof(uint16_t), file);
    /* data */
    fread(result->DataBlocID, 1, 4 * sizeof(char), file);
    fread(&result->DataBlocSize, 1, sizeof(uint32_t), file);

    read_data(result, file);

    fclose(file);
    return result;
}

void close_wav(wav_file *file)
{
    free(file->Data);
    free(file);
}

static void read_data(wav_file *wav, FILE *file)
{
    unsigned int i;
    unsigned int SampleSize;
    unsigned int NumSample;
    SampleSize = wav->BitsPerSample / 8;
    NumSample = wav->DataBlocSize / wav->NumChannels / SampleSize;
    wav->Data = malloc(wav->DataBlocSize);
    for (i = 0; i < wav->DataBlocSize; i = i + SampleSize)
	fread(&wav->Data[i], SampleSize, sizeof(int8_t), file);
}

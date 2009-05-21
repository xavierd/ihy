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
    if (data->NumChannels > 2)
    {
	printf("Wav file not supported : NumChannels > 2\n");
	exit(0);
    }
    fread(&data->SampleRate, sizeof(uint32_t), 1, file);
    fread(&data->ByteRate, sizeof(uint32_t), 1, file);
    fread(&data->BlockAlign, sizeof(uint16_t), 1, file);
    fread(&data->BitsPerSample, sizeof(uint16_t), 1, file);
    if (data->BitsPerSample != 16)
    {
	/* Never encounter such file, and we don't handle it "correctly" */
	printf("Wav file not supported : BitsPerSample > 16\n");
	exit(0);
    }
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

/* release the memory used by a wav */
void destroy_wav(wav_data *data)
{
    free(data->Data);
    free(data);
}

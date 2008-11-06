#include <stdio.h>
#include <stdlib.h>

#include "pcm.h"

void read_data(wav_file *wav, FILE *file);

wav_file *open_wav(const char *filename)
{
	FILE *file;
	wav_file *result;

	file = fopen(filename, "rb");
	if (file == NULL)
	{
		printf("file not exist\n");
		exit(1);
	}
	result = malloc(sizeof(wav_file));
	/* RIFF header */
	fread(&result->ChunkID, 1, 4 * sizeof(char), file);
	printf("%s\n", result->ChunkID);
	fread(&result->ChunkSize, 1, sizeof(unsigned int), file);
	fread(&result->Format, 1, 4 * sizeof(char), file);
	printf("%s\n", result->Format);
	/* fmt */
	fread(&result->FormatBlocID, 1, 4 * sizeof(char), file);
	printf("%s\n", result->FormatBlocID);
	fread(&result->FormatBlocSize, 1, sizeof(unsigned int), file);
	fread(&result->AudioFormat, 1, sizeof(unsigned short int), file);
	fread(&result->NumChannels, 1, sizeof(unsigned short int), file);
	fread(&result->SampleRate, 1, sizeof(unsigned int), file);
	fread(&result->ByteRate, 1, sizeof(unsigned int), file);
	fread(&result->BlockAlign, 1, sizeof(unsigned short int), file);
	fread(&result->BitsPerSample, 1, sizeof(unsigned short int), file);
	printf("taille d'un échantillon : %u\n", result->BitsPerSample);
	/* data */
	fread(&result->DataBlocID, 1, 4 * sizeof(char), file);
	printf("%s\n", result->DataBlocID);
	fread(&result->DataBlocSize, 1, sizeof(unsigned int), file);
	printf("taille des données : %u\n", result->DataBlocSize);

	read_data(result, file);

	fclose(file);
	return result;
}

void read_data(wav_file *wav, FILE *file)
{
	int i, j;
	int SampleSize;
	int NumSample;
	SampleSize = wav->BitsPerSample / 8;
	NumSample = wav->DataBlocSize / (wav->NumChannels * SampleSize);
	wav->Data = malloc(NumSample * sizeof(char *));
	for (i = 0; i <= NumSample; i++)
		wav->Data[i] = malloc(wav->NumChannels);
	for (i = 0; i <= NumSample; i++)
		for (j = 0; j <= wav->NumChannels; j++)
			wav->Data[i][j] = malloc(SampleSize);
	for (i = 0; i <= NumSample; i++)
		for (j = 0; j <= wav->NumChannels; j++)
			fread(wav->Data[i][j], 1, SampleSize, file);
#if 0
	/* not compiled */
	/* I've used this to test if the file was correctly read */
	if (feof(file))
		printf("EOF\n");
	else
	{
		int c = getc(file);
		i = 0;
		while (c != EOF) {
			i++;
			c = getc(file);
		};
		printf("not at EOF, %d\n",i);
	}
#endif
}

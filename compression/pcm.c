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
	printf("taille des données : %u\n", result->DataBlocSize+44);

	read_data(result, file);

	fclose(file);
	return result;
}

void read_data(wav_file *wav, FILE *file)
{
	int i, j;
	int size;
	int sizeofpos;
	size = ((wav->BitsPerSample / 8) * wav->DataBlocSize / wav->NumChannels);
	sizeofpos = (wav->BitsPerSample / 8) * sizeof(char);
	wav->Data = malloc(wav-> NumChannels);
	for (i = 0; i < wav->NumChannels; i++)
		wav->Data[i] = malloc(size);
	for (j = 0; j < size; j++)
		for (i = 0; i < wav->NumChannels; i++)
			fread(&wav->Data[i][j], 1, sizeofpos, file);
}

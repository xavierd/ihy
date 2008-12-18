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
	printf("taille d'un echantillon : %u\n", result->BitsPerSample);
	/* data */
	fread(&result->DataBlocID, 1, 4 * sizeof(char), file);
	printf("%s\n", result->DataBlocID);
	fread(&result->DataBlocSize, 1, sizeof(unsigned int), file);
	printf("taille des donnees : %u\n", result->DataBlocSize);

	read_data(result, file);

	fclose(file);
	return result;
}

void close_wav(wav_file *file)
{
	free(file->Data);
	free(file);
}

void read_data(wav_file *wav, FILE *file)
{
	unsigned int i;
	unsigned int SampleSize;
	unsigned int NumSample;
	SampleSize = wav->BitsPerSample / 8;
	NumSample = wav->DataBlocSize / wav->NumChannels / SampleSize;
	wav->Data = malloc(wav->DataBlocSize);
	for (i = 0; i < wav->DataBlocSize; i = i + SampleSize)
		fread(&wav->Data[i], SampleSize, sizeof(char), file);
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

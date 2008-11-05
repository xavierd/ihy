#include <stdio.h>
#include <stdlib.h>

#include "pcm.h"

wav_file *open_wav(const char *filename)
{
	FILE *wav;
	wav_file *result;

	wav = fopen(filename, "rb");
	if (wav == NULL)
	{
		printf("file not exist\n");
		exit(1);
	}
	result = malloc(sizeof(wav_file));
	/* RIFF header */
	fread(&result->ChunkID, 1, 4 * sizeof(char), wav);
	printf("%s\n", result->ChunkID);
	fread(&result->ChunkSize, 1, sizeof(unsigned int), wav);
	fread(&result->Format, 1, 4 * sizeof(char), wav);
	printf("%s\n", result->Format);
	/* fmt */
	fread(&result->FormatBlocID, 1, 4 * sizeof(char), wav);
	printf("%s\n", result->FormatBlocID);
	fread(&result->FormatBlocSize, 1, sizeof(unsigned int), wav);
	fread(&result->AudioFormat, 1, sizeof(unsigned short int), wav);
	fread(&result->NumChannels, 1, sizeof(unsigned short int), wav);
	fread(&result->SampleRate, 1, sizeof(unsigned int), wav);
	fread(&result->ByteRate, 1, sizeof(unsigned int), wav);
	fread(&result->BlockAlign, 1, sizeof(unsigned short int), wav);
	fread(&result->BitsPerSample, 1, sizeof(unsigned short int), wav);
	/* data */
	fread(&result->DataBlocID, 1, 4 * sizeof(char), wav);
	printf("%s\n", result->DataBlocID);
	fread(&result->DataBlocSize, 1, sizeof(unsigned int), wav);

	fclose(wav);
	return result;
}

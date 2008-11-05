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
	fread(&result->ChunkID, 1, sizeof(unsigned int), wav);

	fclose(wav);
	return result;
}

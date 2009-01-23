#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <caml/mlvalues.h>
#include <caml/callback.h>
#include <caml/bigarray.h>

#include "wav.h"
#include "ihy.h"
#include "interface.h"
#include "huffman.h"

int main(int argc, char **argv)
{
    caml_main(argv);
    if (argc < 3)
    {
	printf("%s: please specify input and output filename\n", argv[0]);

	return 1;
    }
    else
    {
	wav_data *input;
	ihy_data *output;
	int8_t *samples_to_compress;
	float *compressed_samples;
	huffman_tree *B;
	uint32_t i;

	/* wav reading */
	printf("Loading wav file ... ");
	fflush(stdout);
	input = create_wav();
	read_wav(argv[1], input);
	printf("DONE\n");
	fflush(stdout);

	/* waveletts */
	printf("Using wavelets on data ... ");
	fflush(stdout);

	for (i = 0; i < input->DataBlocSize / (input->BitsPerSample / 8); i++)
	{
	};
	compressed_samples = ondelette(
	    input->Data,
	    input->BitsPerSample / 8,
	    input->DataBlocSize
	);
	printf("DONE\n");
	fflush(stdout);

	/* ihy writing */
	printf("Writing ihy file ... ");
	fflush(stdout);
	output = create_ihy();
	output->FileID[0] = 'S';
	output->FileID[1] = 'N';
	output->FileID[2] = 'X';
	output->FileID[3] = 'T';
	output->FileSize = 0;
	output->CompressionType = 0;
	output->Channels = input->NumChannels;
	output->Frequency = input->SampleRate;
	output->Artist = malloc(5 * sizeof(char));
	output->Artist[4] = '\0';
	output->ArtistLength = strlen(output->Artist);
	output->Album = malloc(5 * sizeof(char));
	output->Album[4] = '\0';
	output->AlbumLength = strlen(output->Album);
	output->Track = malloc(5 * sizeof(char));
	output->Track[4] = '\0';
	output->TrackLength = strlen(output->Track);
	output->Year = 2009;
	output->Genre = 42;
	output->Comment = malloc(25 * sizeof(char));
	output->Comment[24] = '\0';
	output->CommentLength = strlen(output->Comment);
	output->NbChunk = 1;
	output->DataChunks = malloc(1 * sizeof(ihy_chunk));
	output->DataChunks[0].ChunkSize =
	    (input->DataBlocSize / (input->BitsPerSample / 8)) * sizeof(float);
	output->DataChunks[0].Values = compressed_samples;

	write_ihy(output, argv[2]);
	printf("DONE\n");

	/* huffman tree */
	printf("Creating Huffman tree ... ");
	fflush(stdout);
	B = build_huffman(input->Data, input->DataBlocSize);
	printf("DONE\n");
	fflush(stdout);
	destroy_huffman(B);

	destroy_wav(input);
	destroy_ihy(output);

	return 0;
    }
}

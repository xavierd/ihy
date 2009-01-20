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
    if (argc <= 1)
    {
	printf("%s: please specify filename\n", argv[0]);

	return 1;
    }
    else
    {
	wav_data *test;
	ihy_data *output;
	float *compressed;
	huffman_tree *B;

	/* wav */
	printf("opening wav");
	fflush(stdout);
	test = create_wav();
	printf(".");
	fflush(stdout);
	read_wav(argv[1], test);
	if (argc >= 3)
	    write_wav(test, argv[2]);
	printf(".. DONE\n");

	/* test ondelettes */
	printf("using wavelets on wav");
	fflush(stdout);
	compressed = ondelette(test->Data, test->BitsPerSample / 8,
		test->DataBlocSize);
	printf("... DONE\n");

	output = create_ihy();
	output->FileID[0] = 'S';
	output->FileID[1] = 'N';
	output->FileID[2] = 'X';
	output->FileID[3] = 'T';
	output->FileSize = 0;
	output->CompressionType = 0;
	output->Channels = test->NumChannels;
	output->Frequency = test->SampleRate;
	output->Artist = "Best Friend !";
	output->ArtistLength = strlen(output->Artist);
	output->Album = "My best album";
	output->AlbumLength = strlen(output->Album);
	output->Track = "You are my friend forevah";
	output->TrackLength = strlen(output->Track);
	output->Year = 2009;
	output->Genre = 42;
	output->Comment = "Oooooh you touch my tralala !";
	output->CommentLength = strlen(output->Comment);
	output->NbChunk = 1;
	output->DataChunks = malloc(1 * sizeof(ihy_chunk));
	output->DataChunks[0].ChunkSize =
	    /* this is not working : an float is not coded as Sample in
	     * a wav ar
	     * what it implies :
	     * ihy.c : 56, we can write to an non malloced memory, so it
	     * will crash here or on free */
	    test->DataBlocSize / (test->BitsPerSample / 8);
	output->DataChunks[0].Values = compressed;

	write_ihy(output, "caca.ihy");
	/*destroy_ihy(output);*/
	output = create_ihy();
	read_ihy("caca.ihy", output);
	write_ihy(output, "prout.ihy");
	/* segfault and I don't now why...
	 * I'll see later
	 *
	destroy_ihy(output);
	*/

	/* test huffman */
	printf("applying Huffman algorithm");
	fflush(stdout);
	B = build_huffman(test->Data, test->DataBlocSize);
	printf("... DONE\n");
	destroy_huffman(B);

	destroy_wav(test);

	return 0;
    }
}

#include <stdlib.h>
#include <stdio.h>
#include <caml/mlvalues.h>
#include <caml/callback.h>
#include <caml/bigarray.h>

#include "wav.h"
#include "ihy.h"
#include "interface.h"
#include "huffman.h"

int main(int argc, char ** argv)
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
	float *compressed;
	huffman_tree *B;

	/* wav */
	printf("opening wav");
	test = create_wav();
	printf(".");
	read_wav(argv[1], test);
	if (argc >= 3)
	    write_wav(test, argv[2]);
	printf(".. DONE\n");

	/* test ondelettes */
	printf("using wavelets on wav");
	compressed = ondelette(test->Data, test->BitsPerSample / 8,
		test->DataBlocSize);
	printf("... DONE\n");

	/* test huffman */
	printf("applying Huffman algorithm");
	B = build_huffman(test->Data, test->DataBlocSize);
	printf("... DONE\n");
	destroy_huffman(B);
	destroy_wav(test);

	return 0;
    }
}

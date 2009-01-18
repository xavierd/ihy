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
	unsigned char array[4000];
	int i;
	float *compressed;
	huffman_tree *B;

	/* wav */
	test = create_wav();
	read_wav(argv[1], test);

	/* test ondelettes */
	compressed = ondelette(test->Data, test->DataBlocSize);
	destroy_wav(test);

	/* test huffman */
	for (i = 0; i < 4000; i++)
	    array[i] = random() % 256;
	B = build_huffman(array, 4000);
	destroy_huffman(B);

	return 0;
    }
}

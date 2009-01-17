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
    if (argc <= 1)
    {
	printf("%s: please specify filename\n", argv[0]);

	return 1;
    }
    else
    {
	wav_data *test;
	test = create_wav();
	read_wav(argv[1], test);
	destroy_wav(test);

	unsigned char array[4000];
	int i;
	huffman_tree *B;
	for (i = 0; i < 4000; i++)
	    array[i] = random() % 256;
	B = build_huffman(array, 4000);

	return 0;
    }
}

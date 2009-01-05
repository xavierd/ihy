#include <stdlib.h>
#include <stdio.h>
#include <caml/mlvalues.h>
#include <caml/callback.h>
#include <caml/bigarray.h>

#include "pcm.h"
#include "ihy.h"

int main(int argc, char ** argv)
{
    if (argc <= 1)
    {
	printf("%s: please specify filename\n", argv[0]);
	return 1;
    }
    else
    {
	wav_file *test;
	test = open_wav(argv[1]);
	close_wav(test);
	return 0;
    }
}

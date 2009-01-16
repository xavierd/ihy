#include <stdlib.h>
#include <stdio.h>
#include <caml/mlvalues.h>
#include <caml/callback.h>
#include <caml/bigarray.h>

#include "wav.h"
#include "ihy.h"
#include "interface.h"

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
	read_wav(argv[1], test);
	destroy_wav(test);

	return 0;
    }
}

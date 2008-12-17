#include <stdlib.h>
#include <stdio.h>

#include "pcm.h"

int main(int argc, char ** argv)
{
	if (argc <= 1) {
		printf("error: please specify filename\n");
		return 1;
	} else {
		wav_file *test;
		test = open_wav(argv[1]);
		close_wav(test);
		return 0;
	}
}

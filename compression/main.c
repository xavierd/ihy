#include <stdlib.h>
#include <stdio.h>

#include "pcm.h"

int main()
{
	wav_file *test;
	test = open_wav("../../misc/wav/test.wav");
	close_wav(test);
	return 1;
}

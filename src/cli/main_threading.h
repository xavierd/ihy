#ifndef DEF_MAIN_THREADING
#define DEF_MAIN_THREADING

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <compression/wavelet.h>
#include <compression/huffman.h>
#include <compression/quantization.h>
#include <codecs/wav.h>
#include <codecs/ihy.h>
#include <utils/half.h>

void encode_ihy(int nbcpu, int nbchunks, int quality, wav_data *input,
						      ihy_data *output);

#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include <compression/wavelet.h>
#include <compression/huffman.h>
#include <codecs/wav.h>
#include <codecs/ihy.h>
#include <utils/half.h>

void encode_ihy(int nbcpu, int nbchunks, wav_data *input, ihy_data *output);

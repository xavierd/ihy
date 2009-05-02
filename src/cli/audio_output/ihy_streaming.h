#ifndef DEF_IHY_STREAMING
#define DEF_IHY_STREAMING

#include <codecs/ihy.h>
#include <utils/buffer.h>
#include <audio_output/ao.h>
#include <compression/wavelet.h>
#include <compression/huffman.h>
#include <utils/half.h>
#include <compression/quantization.h>

/* play the ihy in streaming */
void play_ihy_streaming(ihy_data *ihy);

#endif

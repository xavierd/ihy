#ifndef DEF_IHY_COMPRESSION
#define DEF_IHY_COMPRESSION

#include <string.h>
#include <codecs/ihy.h>
#include <compression/huffman.h>
#include <compression/quantization.h>
#include <compression/wavelet.h>
#include <utils/half.h>

/*
 * Uncompress this chunk, and put the uncompressed on samples,
 * samples should be alloced and had a sufficient size
 */
void uncompress_chunk(ihy_chunk *chunk, int8_t *samples, int channels);

void compress_chunk(int8_t *samples, size_t size, uint16_t ch, ihy_chunk *chunk);

#endif

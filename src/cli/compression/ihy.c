#include "ihy.h"

void uncompress_chunk(ihy_chunk *chunk, int8_t *samples, int channels)
{
    size_t size;
    void *tmp;

    /* Huffman */
    tmp = chunk->Values;
    chunk->ChunkSize = chunk->HUncompressedSize;
    chunk->Values = huffman_decode(tmp, chunk->ChunkSize);
    free(tmp);
    /* End Huffman */
    /* Quantification */
    size = chunk->ChunkSize;
    tmp = chunk->Values;
    tmp = dequantizate(tmp, &size, chunk->QScaleFactor, chunk->QBitsPerCoefs);
    free(chunk->Values);
    chunk->Values = tmp;
    /* End Quantification */
    /* Wavelets */
    wavelets_inverse(tmp, size, channels, samples);
    /* End Wavelets */
}

static size_t bitrate(ihy_quality q)
{
    switch(q)
    {
	case poor:
	    return 128;
	case medium:
	    return 192;
	case good:
	    return 256;
	case very_good:
	    return 320;
	default:
	    printf("Bad Quality\n");
	    exit(0);
    }
}

/*
 * Assuming chunk->ChunkSize == ihy->ChunkSize
 * Assuming chunk->QBitsPerCoefs == quality
 */
void compress_chunk(int8_t *samples, size_t size, uint16_t ch, ihy_chunk *chunk)
{
    int8_t *pow2_samples;
    void *tmp, *oldValue;
    int nbbits;
    ihy_quality quality = chunk->QBitsPerCoefs;
    float factor;
    float actual_bitrate;
    size_t chunk_size;

    chunk_size = chunk->ChunkSize;
    pow2_samples = calloc(chunk->ChunkSize * 2, 1);
    memcpy(pow2_samples, samples, size);
    chunk->Values = malloc((chunk->ChunkSize / 2) * sizeof(float));
    size = chunk->ChunkSize * 2;
    wavelets_direct(pow2_samples, size, ch, (float *)chunk->Values);
    free(pow2_samples);
    chunk->ChunkSize = (chunk->ChunkSize / 2) * sizeof(float);

    factor = 0.5f;
    do
    {
	factor *= 2;
	oldValue = chunk->Values;
	size = chunk->ChunkSize / sizeof(float);
	oldValue = quantizate(oldValue, &size, factor, &nbbits);
	tmp = oldValue;
	chunk->HUncompressedSize = size;
	oldValue = huffman_encode(tmp, &size);
	free(tmp);
	actual_bitrate = (size * 8) / ((float)chunk_size / 44100.0f);
	actual_bitrate /= ch / 1024;
    }
    while (actual_bitrate > bitrate(quality));
    chunk->QBitsPerCoefs = nbbits;
    chunk->QScaleFactor = factor;
    chunk->ChunkSize = size;
    chunk->Values = oldValue;
}

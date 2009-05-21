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

/*
 * Assuming chunk->ChunkSize == ihy->ChunkSize
 */
void compress_chunk(int8_t *samples, size_t size, uint16_t ch, ihy_chunk *chunk)
{
    int8_t *pow2_samples;
    void *tmp, *oldValue;
    int nbbits;

    pow2_samples = calloc(chunk->ChunkSize * 2, 1);
    memcpy(pow2_samples, samples, size);
    chunk->Values = malloc((chunk->ChunkSize / 2) * sizeof(float));
    size = chunk->ChunkSize * 2;
    wavelets_direct(pow2_samples, size, ch, chunk->Values);
    free(pow2_samples);
    chunk->ChunkSize = (chunk->ChunkSize / 2) * sizeof(float);
    do
    {
	oldValue = chunk->Values;
	size = chunk->ChunkSize / sizeof(float);
	oldValue = quantizate(oldValue, &size, 200.0f, &nbbits);
	tmp = oldValue;
	chunk->HUncompressedSize = size;
	oldValue = huffman_encode(tmp, &size);
	free(tmp);
    }
    while (!size);
    chunk->QBitsPerCoefs = nbbits;
    chunk->QScaleFactor = 200.0f;
    chunk->ChunkSize = size;
    chunk->Values = oldValue;
}

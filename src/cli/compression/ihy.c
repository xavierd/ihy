#include "ihy.h"

void uncompress_chunk(ihy_chunk *chunk, int8_t *samples, int ch)
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
    /* End Quantification */
    /* Wavelets */
    wavelets_inverse(tmp, size, ch, samples, 0);
    free(tmp);
    /* End Wavelets */
}

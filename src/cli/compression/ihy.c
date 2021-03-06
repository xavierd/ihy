#include "ihy.h"

void interpolate(int16_t *samples, size_t nb, size_t ch)
{
    static int16_t *last = NULL;
    size_t i;

    if (last)
    {
	for (i = 0; i < ch; i++)
	{
	    if (!last[i + 2])
		last[i + 2] = (last[i] + samples[i]) / 2;
	}
    }
    for (i = ch; i < nb - ch; i++)
    {
	if (samples[i - ch] && !samples[i] && samples[i + ch])
	    samples[i] = (samples[i - ch] + samples[i + ch]) / 2;
    }
    last = &samples[nb - 2 * ch];
}

void uncompress_chunk(ihy_chunk *chunk, int8_t *samples, int channels)
{
    size_t size;
    void *tmp;
    float factor;

    /* Huffman */
    tmp = chunk->Values;
    chunk->ChunkSize = chunk->HUncompressedSize;
    chunk->Values = huffman_decode(tmp, chunk->ChunkSize);
    free(tmp);
    /* End Huffman */
    /* Quantification */
    size = chunk->ChunkSize;
    tmp = chunk->Values;
    factor = half_to_float(chunk->QScaleFactor);
    tmp = dequantizate(tmp, &size, factor, chunk->QBitsPerCoefs);
    free(chunk->Values);
    chunk->Values = tmp;
    /* End Quantification */
    /* Wavelets */
    wavelets_inverse(tmp, size, channels, samples);
    /* End Wavelets */
    interpolate((int16_t *)samples, size * 2, channels);
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

static float init_factor(ihy_quality q)
{
    switch(q)
    {
	case poor:
	    return 50.0f;
	case medium:
	    return 25.0f;
	case good:
	    return 5.0f;
	case very_good:
	    return 1.0f;
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

    factor = init_factor(quality);
    do
    {
	factor += 2;
	oldValue = chunk->Values;
	size = chunk->ChunkSize / sizeof(float);
	oldValue = quantizate(oldValue, &size, factor, &nbbits);
	tmp = oldValue;
	chunk->HUncompressedSize = size;
	oldValue = huffman_encode(tmp, &size);
	free(tmp);
	actual_bitrate = (size * 8) / ((float)chunk_size / 44100.0f);
	actual_bitrate /= 1024 / ch;
    }
    while (actual_bitrate > bitrate(quality));
    chunk->QBitsPerCoefs = nbbits;
    chunk->QScaleFactor = float_to_half(factor);
    chunk->ChunkSize = size;
    chunk->Values = oldValue;
}

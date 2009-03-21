#include "ihy_streaming.h"

static int get_nb_chunk(void *Data)
{
    ihy_data *ihy = Data;

    return ihy->NbChunk;
}

static void DecodeIhy(void *Data, int chunk, void *out)
{
    ihy_data *ihy = Data;
    uint8_t *toDecode = ihy->DataChunks[chunk].Values;
    uint8_t *Values, *oldValues;
    int size = ihy->DataChunks[chunk].ChunkSize;

    Values = huffman_decode(toDecode, &size);
    oldValues = Values;
    Values = halfarray_to_float(Values, size / sizeof(uint16_t));
    free(oldValues);
    size *= 2;
    wavelets_inverse(Values, size / sizeof(float), ihy->Channels, out, 0);
    size /= 2;
}

void play_ihy_streaming(ihy_data *ihy)
{
    ao_device *dev;

    dev = ao_init_device(16, ihy->Channels, ihy->Frequency);
    dev->Data = ihy;
    dev->NbChunk = &get_nb_chunk;
    dev->DecodeFunction = &DecodeIhy;

    ao_play(dev);
}

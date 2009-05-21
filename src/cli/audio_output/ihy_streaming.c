#include "ihy_streaming.h"

static int get_nb_chunk(void *Data)
{
    ihy_data *ihy = Data;

    return ihy->NbChunk;
}

static int DecodeIhy(void *Data, int i, void *out)
{
    ihy_data *ihy = Data;
    ihy_chunk *chunk;

    chunk = &ihy->DataChunks[i];
    uncompress_chunk(chunk, out, ihy->Channels);
    return ihy->ChunkSize * 2;
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

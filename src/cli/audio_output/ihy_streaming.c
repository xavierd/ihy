#include "ihy_streaming.h"

#if 0
struct ihy_streaming_data
{
    t_buffer	buffer;
    ihy_data	*ihy;
};

struct ihy_buffer_content
{
    int8_t	*samples;
    uint32_t	samplesSize;
};

static void *ihy_playing(void *data)
{
    /* contains uncompressed data, (ihy_buffer_content *) */
    t_buffer buffer = ((struct ihy_streaming_data *)data)->buffer;
    ihy_data *ihy = ((struct ihy_streaming_data *)data)->ihy;
    ao_device *audio_device;
    struct ihy_buffer_content *to_play;

    audio_device = ao_init_device(16, ihy->Channels, ihy->Frequency);

    /* we need to ensure that the other thread has added,
     * at least, 1 element
     */
    while (buffer_isempty(buffer))
	usleep(50);
    while (!buffer_isempty(buffer))
    {
	to_play = buffer_get(buffer);
	ao_play_samples(audio_device, to_play->samples, to_play->samplesSize);
	free(to_play); /* hum quite dangerous */
    }
    ao_close_device(audio_device);
    return NULL;
}

static void *ihy_filling_buffer(void *data)
{
    /* contains uncompressed data, (ihy_buffer_content *) */
    t_buffer buffer = ((struct ihy_streaming_data *)data)->buffer;
    ihy_data *ihy = ((struct ihy_streaming_data *)data)->ihy;
    struct ihy_buffer_content *to_add;
    unsigned int i = 0;
    void *oldValues;

    while (i < ihy->NbChunk)
    {
	to_add = malloc(sizeof(struct ihy_buffer_content));
	to_add->samplesSize = ihy->DataChunks[i].ChunkSize;
	to_add->samples =
	    huffman_decode(ihy->DataChunks[i].Values, &to_add->samplesSize);
	oldValues = to_add->samples;
	to_add->samples = (int8_t *)halfarray_to_float(
		(uint16_t *)to_add->samples,
		to_add->samplesSize / sizeof(uint16_t));
	free(oldValues);
	to_add->samplesSize *= 2;
	wavelets_inverse((float *)to_add->samples,
			 to_add->samplesSize / sizeof(float),
			 ihy->Channels,
			 to_add->samples,
			 0);
	to_add->samplesSize = to_add->samplesSize / 2; /*samplesize = 16bits*/
	buffer_add(to_add, buffer);
	i++;
    }
    return NULL;
}

#endif
static int get_nb_chunk(void *Data)
{
    ihy_data *ihy = Data;

    return ihy->NbChunk;
}

static void DecodeIhy(void *Data, int chunk, void *out)
{
    ihy_data *ihy = Data;
    int8_t *toDecode = ihy->DataChunks[chunk].Values;
    int8_t *Values, *oldValues;
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
    printf("init\n");
    dev->NbChunk = &get_nb_chunk;
    dev->DecodeFunction = &DecodeIhy;

    printf("play\n");
    ao_play(dev);
}

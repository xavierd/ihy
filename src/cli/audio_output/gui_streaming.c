#include "gui_streaming.h"

/* General structure for played data */
struct gui_streaming_data
{
    t_buffer	buffer;
    ihy_data	*ihy;
    int		current_offset;
    pthread_t	playing_thread;
    pthread_t	filling_thread;
    int		pause_status;
    int		stop_status;
};

/* Structure and action made by the thread actually playing the sound */
struct s_playing_thread_data {
    t_buffer	buffer;
    int		*status;
    ihy_data	*ihy;
};
static void *playing_thread_action(void *data)
{
    int continue_playing;
    ao_device *playing_device;
    int *to_play;
    struct s_playing_thread_data *data2;

    data2 = data;

    continue_playing = 1;
    playing_device = ao_init_device(16, data2->ihy->Channels, data2->ihy->Frequency);
    to_play = NULL;

    while (continue_playing)
    {
	if (!(*data2->status))
	{
	    while (buffer_isempty(data2->buffer))
		usleep(50000);
	    to_play = buffer_get(((t_playdata)data)->buffer);
	    if (to_play)
	    {
		ao_play_samples(playing_device, to_play, data2->ihy->ChunkSize * 2);
		free(to_play);
	    }
	    else
	    {
		continue_playing = 0;
	    }
	}
	else
	{
	    usleep(50000);
	}
    }

    ao_close_device(playing_device);

    return NULL;
}

/* Structure and action made by the thread filling the buffer */
struct s_filling_thread_data {
    t_buffer	buffer;
    ihy_data	*ihy;
    int		*status;
    int		*current_offset;
};
static void *filling_thread_action(void *data)
{
    ihy_chunk *chunk;
    int *to_add;
    struct s_filling_thread_data *data2;

    data2 = data;

    while (!(*(data2->status)) && (*data2->current_offset <= data2->ihy->NbChunk))
    {
	chunk = &(data2->ihy->DataChunks[*data2->current_offset]);
	to_add = calloc(data2->ihy->ChunkSize * 2, 1);
	uncompress_chunk(chunk, (int8_t*)to_add, data2->ihy->Channels);
	buffer_add(to_add, data2->buffer);
	(*data2->current_offset)++;
    }

    buffer_add(NULL, data2->buffer);

    return NULL;
}

/* Function creating a gui_straming_data based on some ihy data */
t_playdata create_gui_streaming(ihy_data *ihy)
{
    t_playdata res;
    struct s_filling_thread_data *ft_data;
    struct s_playing_thread_data *pt_data;

    res = malloc(sizeof(struct gui_streaming_data));
    res->buffer = buffer_init(20);
    res->ihy = ihy;
    res->current_offset = 0;
    ft_data = malloc(sizeof(struct s_filling_thread_data));
    ft_data->buffer = res->buffer;
    ft_data->status = &(res->stop_status);
    ft_data->current_offset = &(res->current_offset);
    ft_data->ihy = ihy;
    pt_data = malloc(sizeof(struct s_playing_thread_data));
    pt_data->buffer = res->buffer;
    pt_data->status = &(res->pause_status);
    pt_data->ihy = ihy;
    res->pause_status = 1;
    res->stop_status = 0;
    pthread_create(&res->filling_thread, NULL, &filling_thread_action, ft_data);
    pthread_create(&res->playing_thread, NULL, &playing_thread_action, pt_data);

    return res;
}

/* Function controling the playback of the sound */
void play_gui_streaming(t_playdata played)
{
    played->pause_status = 0;
}

void pause_gui_streaming(t_playdata played)
{
    played->pause_status = 1;
}

void destroy_gui_streaming(t_playdata played)
{
    int *tmp;

    played->stop_status = 0;
    while (!buffer_isempty(played->buffer))
    {
	buffer_get(played->buffer);
	free(tmp);
    }
    buffer_add(NULL, played->buffer);
    pthread_join(played->playing_thread, NULL);
    pthread_join(played->filling_thread, NULL);
    buffer_destroy(played->buffer);
    free(played);
}
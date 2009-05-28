#ifndef DEF_GUI_STREAMING
#define DEF_GUI_STREAMING

#include <codecs/ihy.h>
#include <utils/buffer.h>
#include <audio_output/ao.h>
#include <compression/ihy.h>

typedef struct gui_streaming_data *t_playdata;

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
    float	percentage;
};

/* play the ihy in streaming */
t_playdata create_gui_streaming(ihy_data *ihy);
void play_gui_streaming(t_playdata played);
void pause_gui_streaming(t_playdata played);
void destroy_gui_streaming(t_playdata played);

#endif

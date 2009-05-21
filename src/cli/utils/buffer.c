#include "buffer.h"

struct s_buffer
{
    t_queue		queue;
    pthread_mutex_t	lock;
    int			max_size;
};

t_buffer buffer_init(const int max_size)
{
    t_buffer res;

    res = malloc(sizeof(struct s_buffer));
    res->max_size = max_size;
    pthread_mutex_init(&res->lock, NULL);
    res->queue = queue_create();
    return res;
}

/* add elem to buf, only if buf is not full.
 * This function is thread-safe and blocking,
 * it means that it will wait until elem has
 * been added to the buffer.
 */
void buffer_add(void *elem, t_buffer buf)
{
    while (queue_nbelement(buf->queue) == buf->max_size)
	usleep(100000); /* do nothing for 100ms */
    pthread_mutex_lock(&buf->lock);
    queue_enqueue(elem, buf->queue);
    pthread_mutex_unlock(&buf->lock);
}

/* return the next elem, this is thread-safe */
void *buffer_get(t_buffer buf)
{
    void *res;

    pthread_mutex_lock(&buf->lock);
    res = queue_dequeue(buf->queue);
    pthread_mutex_unlock(&buf->lock);
    return res;
}

int buffer_isempty(const t_buffer buf)
{
    return queue_isempty(buf->queue);
}

void buffer_destroy(t_buffer buf)
{
    queue_destroy(buf->queue);
    free(buf);
}

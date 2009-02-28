#ifndef DEF_BUFFER
#define DEF_BUFFER

#include <pthread.h>
#include <unistd.h>

#include <utils/queue.h>

/* A thread-safe buffer */

typedef struct s_buffer *t_buffer;
/* do not expose the content of the buffer */

t_buffer buffer_init(const int max_size);

/* add elem to buf, only if buf is not full.
 * This function is thread-safe and blocking,
 * it means that it will wait until elem has
 * been added to the buffer.
 */
void buffer_add(void *elem, t_buffer buf);

/* return the next elem, this is thread-safe */
void *buffer_get(t_buffer buf);

int buffer_isempty(const t_buffer buf);

void buffer_destroy(t_buffer buf);

#endif

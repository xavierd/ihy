#ifndef DEF_QUEUE
#define DEF_QUEUE

#include <stdio.h>
#include <stdlib.h>

/* do not expose the content of s_queue */
typedef struct s_queue *t_queue;

t_queue queue_create();
void queue_enqueue(void *elem, t_queue queue);
void *queue_dequeue(t_queue queue);
int queue_isempty(const t_queue queue);
int queue_nbelement(const t_queue queue);
void queue_destroy(t_queue queue);

#endif

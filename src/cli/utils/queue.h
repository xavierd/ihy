#ifndef DEF_QUEUE
#define DEF_QUEUE

#include <stdio.h>
#include <stdlib.h>

struct s_queue_elem {
    struct s_queue_elem *next;
    void *content;
};
struct s_queue {
    int nb_elem;
    struct s_queue_elem *rear;
};
typedef struct s_queue *t_queue;

t_queue queue_create();
void queue_enqueue(void *elem, t_queue queue);
void *queue_dequeue(t_queue queue);
int queue_isempty(t_queue queue);
void queue_destroy(t_queue queue);

#endif

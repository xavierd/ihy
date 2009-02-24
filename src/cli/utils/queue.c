#include "queue.h"

struct s_queue_elem {
    struct s_queue_elem *next;
    void *content;
};
struct s_queue {
    int nb_elem;
    struct s_queue_elem *rear;
};

t_queue queue_create()
{
    t_queue result;

    result = malloc(sizeof(struct s_queue));
    result->nb_elem = 0;
    result->rear = NULL;

    return result;
}

void queue_enqueue(void *elem, t_queue queue)
{
    struct s_queue_elem *new_elem;

    new_elem = malloc(sizeof(struct s_queue_elem));
    new_elem->content = elem;
    if (queue->nb_elem == 0)
    {
	queue->rear = new_elem;
	queue->rear->next = queue->rear;
	queue->nb_elem = 1;
    }
    else
    {
	new_elem->next = queue->rear->next;
	queue->rear->next = new_elem;
	queue->rear = new_elem;
	queue->nb_elem++;
    }
}

void *queue_dequeue(t_queue queue)
{
    void *result;

    if (queue->nb_elem == 0)
    {
	result = NULL;
    }
    else
    {
	struct s_queue_elem *head_save;

	head_save = queue->rear->next;
	result = head_save->content;
	queue->rear->next = head_save->next;
	free(head_save);
	queue->nb_elem--;
    }
    return result;
}

int queue_isempty(const t_queue queue)
{
    return (queue->nb_elem == 0);
}

int queue_nbelement(const t_queue queue)
{
    return queue->nb_elem;
}

void queue_destroy(t_queue queue)
{
    while (!queue_isempty(queue))
    {
	queue_dequeue(queue);
    }
    free(queue);
}

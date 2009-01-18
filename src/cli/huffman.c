#include "huffman.h"

static void swap(huffman_tree *a, huffman_tree *b)
{
    huffman_tree c;
    c = *a;
    *a = *b;
    *b = c;
}

typedef struct heap
{
    size_t		size;
    huffman_tree	*array;
} heap;

static void heap_add(heap *heap, huffman_tree *B)
{
    unsigned int i = heap->size;
    heap->array[heap->size] = *B;
    while (i > 0 && i < heap->size &&
	    B->frequency < heap->array[(i - 1) / 2].frequency)
    {
	swap(&heap->array[i], &heap->array[(i - 1) / 2]);
	i = (i - 1) / 2;
    }
    heap->size++;
}

static void heap_rm(heap *heap, huffman_tree *out)
{
    unsigned int i;
    unsigned int min;
    unsigned int l, r;

    *out = heap->array[0];
    swap(&heap->array[heap->size - 1], &heap->array[0]);
    heap->size--;
    i = 0;
    while (i < heap->size)
    {
	min = i;
	l = 2 * i + 1;
	r = 2 * i + 2;
	if (l < heap->size
		&& heap->array[i].frequency > heap->array[l].frequency)
	    min = l;
	if (r < heap->size
		&& heap->array[min].frequency > heap->array[r].frequency)
	    min = r;
	if (min != i)
	{
	    swap(&heap->array[i], &heap->array[min]);
	    i = min;
	}
	else
	    break;
    }
}

huffman_tree *build_huffman(const unsigned char *array, const size_t n)
{
    unsigned int i;
    huffman_tree *tree[256];
    huffman_tree *father;
    heap H;

    for (i = 0; i < 256; i++)
    {
	tree[i] = malloc(sizeof(huffman_tree));
	tree[i]->letter = i;
	tree[i]->frequency = 0;
	tree[i]->fd = NULL;
	tree[i]->fg = NULL;
    };
    for (i = 0; i < n; i++)
	tree[array[i]]->frequency++;

    /* put in the heap */
    H.size = 0;
    H.array = malloc(256 * sizeof(huffman_tree));
    for (i = 0; i < 256; i++)
	heap_add(&H, tree[i]);

    /* build the huffman tree */
    father = malloc(sizeof(huffman_tree));
    while (H.size > 1)
    {
	father->fd = malloc(sizeof(huffman_tree));
	father->fg = malloc(sizeof(huffman_tree));
	heap_rm(&H, father->fd);
	heap_rm(&H, father->fg);
	father->frequency = father->fd->frequency + father->fg->frequency;
	heap_add(&H, father);
    }
    heap_rm(&H, father);
    for (i = 0; i < 256; i++)
	free(tree[i]);
    free(H.array);
    return father;
}

void destroy_huffman(huffman_tree *B)
{
    if (B)
    {
	destroy_huffman(B->fd);
	destroy_huffman(B->fg);
	free(B);
    }
}

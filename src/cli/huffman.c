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
    huffman_tree	**array;
} heap;

static void heap_add(heap *heap, huffman_tree *B)
{
    int i = heap->size;
    heap->array[heap->size + 1] = B;
    while (i > 0 && B->frequency < heap->array[(i - 1) / 2]->frequency)
    {
	swap(heap->array[i], heap->array[(i - 1) / 2]);
	i = (i - 1) / 2;
    }
    heap->size++;
}

huffman_tree *build_huffman(const unsigned char *array, const size_t n)
{
    unsigned int i;
    heap *heap = malloc(sizeof(heap));/* tas */
    huffman_tree **tree = malloc(256 * sizeof(huffman_tree *));

    for (i = 0; i < 256; i++)
    {
	tree[i]->letter = i;
	tree[i]->frequency = 0;
	tree[i]->fd = NULL;
	tree[i]->fg = NULL;
    };
    for (i = 0; i < n; i++)
	tree[array[i]]->frequency++;

    /* put in the heap */
    heap->size = 0;
    heap->array = malloc(256 * sizeof(huffman_tree *));
    for (i = 0; i < 256; i++)
	heap_add(heap, tree[i]);
}

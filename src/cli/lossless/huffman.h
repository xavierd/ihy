#ifndef DEF_HUFFMAN
#define DEF_HUFFMAN

#include <stdlib.h>
#include <stdint.h>

typedef struct huffman_tree
{
    char			letter;
    int				frequency;
    struct huffman_tree		*fg;
    struct huffman_tree		*fd;
} huffman_tree;

/* build the huffman tree, and return it */
huffman_tree *build_huffman(const int8_t *array, const size_t n);

/* release the memory used by a huffman tree */
void destroy_huffman(huffman_tree *B);

#endif

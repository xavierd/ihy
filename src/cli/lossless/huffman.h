#ifndef DEF_HUFFMAN
#define DEF_HUFFMAN

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

typedef struct huffman_tree
{
    char			letter;
    int				occurrence;
    struct huffman_tree		*fg;
    struct huffman_tree		*fd;
} huffman_tree;

/* pretty print huffman */
void huffman_pretty(huffman_tree *B, int code, int code_size);

/* build the huffman tree, and return it */
huffman_tree *build_huffman(const void *varray, const size_t n);

/* release the memory used by a huffman tree */
void destroy_huffman(huffman_tree *B);

#endif

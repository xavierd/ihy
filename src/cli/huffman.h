#ifndef DEF_HUFFMAN
#define DEF_HUFFMAN

#include <stdlib.h>

typedef struct huffman_tree
{
    char			letter;
    int				frequency;
    struct huffman_tree		*fg;
    struct huffman_tree		*fd;
} huffman_tree;

huffman_tree *build_huffman(const unsigned char *array, const size_t n);

#endif

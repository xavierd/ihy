#ifndef DEF_HUFFMAN
#define DEF_HUFFMAN

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <utils/queue.h>

/*
 * Structure of the compressed data :
 * |uncompressed |huffman |                           |
 * |  size       | tree   |      Data                 |
 * |  4 bytes    |        |                           |
 */

typedef struct huffman_tree
{
    unsigned char		letter;
    int				occurrence;
    int				code;
    int				codelength;
    struct huffman_tree		*fg;
    struct huffman_tree		*fd;
} huffman_tree;

/* encode the data
 * n is the size of varray and will be, when the function returns, the
 * size of the array returned
 */
uint8_t *huffman_encode(const void *varray, size_t *n);

/* decode the data
 * *n will contain the size of the array returned
 */
void *huffman_decode(const void *varray, size_t *n);

/* release the memory used by a huffman tree */
void destroy_huffman(huffman_tree *B);

#endif

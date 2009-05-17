#ifndef DEF_QUANTIZATION
#define DEF_QUANTIZATION

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

void *quantizate(float *x, size_t *n, const float factor, int *nbbits);
float *dequantizate(uint8_t *x, size_t *n, const float factor, int nbbits);

#endif

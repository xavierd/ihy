#ifndef DEF_QUANTIZATION
#define DEF_QUANTIZATION

#include <stdlib.h>
#include <math.h>

void *quantizate(float *x, size_t *n, const float factor, int *nbbits);
float *dequantizate(float *x, size_t *n, const float factor, int nbbits);

#endif

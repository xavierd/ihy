#ifndef DEF_QUANTIZATION
#define DEF_QUANTIZATION

#include <stdlib.h>
#include <math.h>

void quantizate(float *x, const size_t n, const float factor);
void dequantizate(float *x, const size_t n, const float factor);

#endif

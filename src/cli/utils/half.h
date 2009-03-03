#ifndef DEF_HALF
#define DEF_HALF

#include <stdlib.h>
#include <stdint.h>

uint16_t float_to_half(float f);
float half_to_float(uint16_t h);
uint16_t* floatarray_to_half(float *fa, size_t size);
float* halfarray_to_float(uint16_t *ha, size_t size);

#endif

#ifndef DEF_HALF
#define DEF_HALF

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef uint16_t t_half;

t_half float_to_half(float f);
float half_to_float(t_half h);

#endif

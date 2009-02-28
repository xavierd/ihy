#ifndef DEF_HALF
#define DEF_HALF

#include <stdio.h>
#include <stdint.h>

struct s_half
{
    unsigned int mantissa:10;
    unsigned int exponent:5;
    unsigned int negative:1;
};
typedef union u_half t_half;
union u_half
{
    uint16_t h;
    struct s_half ieee;
};

struct s_float
{
    unsigned int mantissa:23;
    unsigned int exponent:8;
    unsigned int negative:1;
};
typedef union u_float t_float;
union u_float
{
    float f;
    struct s_float ieee;
};

uint16_t float_to_half(float f);
float half_to_float(uint16_t h);

#endif

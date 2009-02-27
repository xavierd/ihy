#ifndef DEF_HALF
#define DEF_HALF

#include <stdio.h>

struct s_half
{
    unsigned int mantissa:10;
    unsigned int exponent:5;
    unsigned int negative:1;
};
struct s_float
{
    unsigned int mantissa:23;
    unsigned int exponent:8;
    unsigned int negative:1;
};

typedef struct s_half t_half;
typedef struct s_float t_float;

t_half float_to_half(t_float f);
t_float half_to_float(t_half h);

#endif

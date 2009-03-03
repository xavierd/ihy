#include "half.h"

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

uint16_t float_to_half(float f)
{
    if (f == 0.)
    {
	return 0;
    }
    else
    {
	t_half h;
	t_float tempf;

	tempf.f = f;
	h.ieee.negative = tempf.ieee.negative;
	h.ieee.exponent = tempf.ieee.exponent - 112;
	h.ieee.mantissa = tempf.ieee.mantissa >> 13;

	return h.h;
    }
}

float half_to_float(uint16_t h)
{
    if (h == 0)
    {
	return 0.;
    }
    else
    {
	t_float f;
	t_half temph;

	temph.h = h;
	f.ieee.negative = temph.ieee.negative;
	f.ieee.exponent = temph.ieee.exponent + 112;
	f.ieee.mantissa = temph.ieee.mantissa << 13;

	return f.f;
    }
}

uint16_t* floatarray_to_half(float *fa, size_t size)
{
    uint16_t *res;
    unsigned int i;

    res = malloc(size * sizeof(uint16_t));
    for (i = 0; i < size; i++)
    {
	res[i] = float_to_half(fa[i]);
    }

    return res;
}

float* halfarray_to_float(uint16_t *ha, size_t size)
{
    float *res;
    unsigned int i;

    res = malloc(size * sizeof(float));
    for (i = 0; i < size; i++)
    {
	res[i] = half_to_float(ha[i]);
    }

    return res;
}

#include "half.h"

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

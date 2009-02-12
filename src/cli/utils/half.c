#include "half.h"

t_half float_to_half(float f)
{
    if (f == 0)
    {
	return 0;
    }
    else
    {
	t_half h;
	int32_t f2;
	int32_t s, e, m;

	memcpy(&f2, &f, sizeof(float));

	s = f2 >> 31;
	e = ((f2 << 1) >> 24) - 127;
	m = (f2 << 9) >> 9;

	h = s << 15;

	if (e < 0)
	{
	    e = -e;
	    e = (e << 27) >> 27;
	    h = h + ((-e + 5) << 10);
	}
	else
	{
	    e = (e << 27) >> 27;
	    h = h + (e + 15 << 10);
	}

	h = h + (m >> 13);

	return h;
    }
}

float half_to_float(t_half h)
{
    if (h == 0)
    {
	return 0.;
    }
    else
    {
	float f;
	int32_t s, e, m;

	s = h >> 15;
	e = ((h << 1) >> 10) - 15;
	m = (h << 6) >> 6;

	f = s << 31;
	f = h + ((e + 127) << 23);
	f = h + (m << 13);

	return f;
    }
}

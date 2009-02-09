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
	printf("%i\n", s);
	e = ((f2 << 1) >> 24) - 127;
	printf("%i\n", e);
	m = (f2 << 9) >> 9;
	printf("%i\n", m);

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

#include "quantization.h"

static int sign(float x)
{
    if (x > 0.0f)
	return 1;
    else
	return -1;
}

void quantizate(float *x, const size_t n, const float factor)
{
    size_t i;
    int min, max;

    for (i = 0; i < n; i++)
    {
	x[i] = sign(x[i]) * floorf(abs(x[i] / factor));
	if (x[i] > max)
	    max = x[i];
	else if (x[i] < min)
	    min = x[i];
    }
    printf("min : %d, max : %d\n", min, max);
}

void dequantizate(float *x, const size_t n, const float factor)
{
    size_t i;

    for (i = 0; i < n; i++)
	x[i] = sign(x[i]) * (abs(x[i]) + 0.5f) * factor;
}

#include "quantization.h"

static int sign(float x)
{
    if (x > 0.0f)
	return 1;
    else
	return -1;
}

static int nb_bits(int a)
{
    int nb = 1;
    int max = 1;

    while (abs(a) > max)
    {
	nb++;
	max *= 2;
    }
    return nb;
}

#define ith_bit(number, i)		\
    (((number) >> ((i) - 1)) & 1)

/* seems OK */
static void *encode(int *x, const size_t n, const int nb_bits, size_t *size)
{
    void *res;
    char *tab;
    size_t i;
    int j;
    char buf;
    int offset;

    *size = ((nb_bits * n) / 8) + 1;
    res = malloc(*size);
    tab = res;
    offset = 0;
    buf = 0;
    for (i = 0; i < n; i++)
    {
	/* sign */
	if (x[i] < 0)
	    x[i] = (~x[i] + 1) | ith_bit(x[i], 8 * sizeof(int)) << (nb_bits-1);
	for (j = 0; j < nb_bits; j++)
	{
	    buf |= ith_bit(x[i], nb_bits - j) << (7 - offset++);
	    if (offset == 8)
	    {
		*tab = buf;
		buf = 0;
		tab++;
		offset = 0;
	    }
	}
    }
    *tab = buf;
    return res;
}

static int is_pow2(int n)
{
    if (!n) /* 0 isn't power of 2 */
	return 0;
    while (n)
    {
	if (!n % 2)
	    return 0;
	n /= 2;
    }
    return 1;
}

void *quantizate(float *x, size_t *n, const float factor, int *nbbits)
{
    size_t i;
    int min, max;
    void *res;
    int *q;
    float real_factor = factor;

    q = malloc(*n * sizeof(int));
    min = max = 0;
    for (i = 0; i < *n; i++)
    {
	if ((i < *n / 2 && is_pow2(i)) || (i > *n / 2 && is_pow2(i - *n / 2)))
	    real_factor *= 1.0f;
	if (i == *n / 2)
	    real_factor = factor;
	/*q[i] = sign(x[i]) * sqrtf(abs(x[i]) / real_factor);*/
	q[i] = sign(x[i]) * powf(abs(x[i]) / real_factor, 3.0f / 4.0f);
	/*q[i] = sign(x[i]) * floorf(abs(x[i] / factor));*/
	if (q[i] > max)
	    max = q[i];
	else if (q[i] < min)
	    min = q[i];
    }
    min = abs(min);
    max = max > min ? max : min;
    min = nb_bits(max);
    res = encode(q, *n, min, n);
    *nbbits = min;
    free(q);
    return res;
}

static int next_nb(uint8_t **tab, int nbbits, int *offset)
{
    int res;
    int sign;

    res = 0;
    sign = ith_bit(**tab, 8 - *offset);
    *offset += 1;
    nbbits--;
    while (nbbits > 0)
    {
	if (*offset == 8)
	{
	    (*tab)++;
	    *offset = 0;
	}
	res = (res << 1) | ith_bit(**tab, 8 - *offset);
	*offset += 1;
	nbbits--;
    }
    if (*offset == 8)
    {
	(*tab)++;
	*offset = 0;
    }
    if (sign)
	res = -res;
    return res;
}

float *dequantizate(uint8_t *x, size_t *n, const float factor, int nbbits)
{
    size_t i;
    size_t nb_elmts = (((*n - 1) * 8) / nbbits);
    float *res = malloc(nb_elmts * sizeof(float));
    int nb;
    int offset;
    float real_factor = factor;

    offset = 0;
    for (i = 0; i < nb_elmts; i++)
    {
	if ((i < nb_elmts / 2 && is_pow2(i))
		|| (i > nb_elmts / 2 && is_pow2(i - nb_elmts / 2)))
	    real_factor *= 1.0f;
	if (i == nb_elmts / 2)
	    real_factor = factor;
	nb = next_nb(&x, nbbits, &offset);
	if (nb == 0)
	    res[i] = 0.0f;
	else
	    /*res[i] = sign(nb) * powf(abs(nb), 2) * real_factor;*/
	    res[i] = sign(nb) * powf(abs(nb), 4.0f / 3.0f) * real_factor;
	    /*res[i] = sign(nb) * (abs(nb) + 0.5f) * factor;*/
    }
    *n = nb_elmts;
    return res;
}

#include "wavelet.h"

/* just call the wavelets, defined in OCaml */
static value wavelets_direct_fun(const value array)
{
    static value *func_ptr = NULL;
    value res;

    if (!func_ptr)
	func_ptr = caml_named_value("Haar_Direct");
    res = callback(*func_ptr, array);
    return res;
}

static value wavelets_reverse_fun(const value array)
{
    static value *func_ptr = NULL;
    value res;

    if (!func_ptr)
	func_ptr = caml_named_value("Haar_Reverse");
    res = callback(*func_ptr, array);
    return res;
}

/* transform a C array to a OCaml bigarray */
static value c_array_to_caml(float *array, const size_t dim)
{
    return alloc_bigarray_dims(BIGARRAY_FLOAT32 | BIGARRAY_C_LAYOUT,
	    1, array, dim);
}

int get_nbChunk(const int chunk_size, const int nb)
{
    return (nb / chunk_size) + (nb % chunk_size != 0);
}

/* compute the result of the OCaml function "Haar_Direct"
 * compress the data and fill out
 * assuming dim = 2^n
 * real_size is quite particular, it is here to avoid getting values behind the
 * end of samples (should only happen on the last chunk)
 */
void wavelets_direct(const int8_t *samples,
		     const size_t dim, /* size of prec arg */
		     const uint16_t nbChannels,
		     float *out)
{
    const size_t sampleSize = 2;
    size_t resSize = (dim / sampleSize) / nbChannels;
    float *res = malloc(sizeof(float) * resSize);
    int16_t sample; /* je remettrai le switch plus tard */
    unsigned int i, j;
    value camlArray;

    for (j = 0; j < nbChannels; j++)
    {
	for (i = j; i < resSize * nbChannels; i += nbChannels)
	{
	    /*sample = 0;*/
	    memcpy(&sample, &samples[i * sampleSize], sampleSize);
	    res[i / nbChannels] = sample;
	}
	camlArray = c_array_to_caml(res, resSize);
	camlArray = wavelets_direct_fun(camlArray);
	memcpy(out + (j * resSize) / 2, Data_bigarray_val(camlArray),
						 (resSize / 2) * sizeof(float));
    }
    free(res);
}

/* just fill the necessary 0 at the end of the chunk */
static float *fill_zero(const float *chunk, const size_t nb)
{
    float *res;

    res = calloc(nb * 2, sizeof(float));
    memcpy(res, chunk, nb * sizeof(float));

    return res;
}

/* compute the result of the OCaml function "Haar_Reverse"
 * uncompress the data and fill out
 * assuming out->BitsPerSample == 16
 */
void wavelets_inverse(float *chunk,
		      const size_t nbelmts,
		      const uint8_t nbChannels,
		      int8_t *out)
{
    value camlArray;
    int16_t val;
    float *valf;
    unsigned int i, j, k;

    for (k = 0; k < nbChannels; k++)
    {
	valf = fill_zero(chunk + k * (nbelmts / nbChannels),
							(nbelmts / nbChannels));
	camlArray = c_array_to_caml(valf, (nbelmts / nbChannels));
	camlArray = wavelets_reverse_fun(camlArray);
	free(valf);

	j = k;
	valf = (float *)(Data_bigarray_val(camlArray));
	for (i = 0; i < nbelmts / nbChannels; i++)
	{
	    val = valf[i];
	    *(int16_t *)(out + (j * nbChannels)) = val;
	    j = j + sizeof(int16_t) * 2;
	}
    }
}

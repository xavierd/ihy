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

/* compute the result of the OCaml function "Haar_Direct"
 * compress the data and fill out
 * assuming dim = 2^n
 */
void wavelets_direct(const int8_t *samples,
		     const size_t dim, /* size of prec arg */
		     const size_t sampleSize, /* in bytes */
		     float *out)
{
    size_t resSize = dim / sampleSize;
    float *res = malloc(sizeof(float) * resSize);
    int16_t sample; /* je remettrai le switch plus tard */
    unsigned int i;
    value camlArray;

    for (i = 0; i < resSize; i++)
    {
	sample = 0;
	memcpy(&sample, &samples[i * sampleSize], sampleSize);
	res[i] = sample;
    }
    camlArray = c_array_to_caml(res, resSize);
    camlArray = wavelets_direct_fun(camlArray);
    memcpy(out, Data_bigarray_val(camlArray), resSize * sizeof(float));
}

/* compute the result of the OCaml function "Haar_Reverse"
 * uncompress the data and fill out
 * assuming out->BitsPerSample == 16
 */
void wavelets_inverse(float *chunk,
		      const size_t nbelmts,
		      int8_t *out,
		      const int offset)
{
    value camlArray;
    int16_t val;
    float *valf;
    unsigned int i, j;

    camlArray = c_array_to_caml(chunk, nbelmts);
    camlArray = wavelets_reverse_fun(camlArray);

    j = 0;
    valf = (float *)(Data_bigarray_val(camlArray));
    for (i = 0; i < nbelmts; i++)
    {
	val = valf[i];
	*(int16_t *)(out + offset + j) = val;
	j = j + 2;
    }
}

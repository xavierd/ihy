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

static void compute_chunk(float *arrayf, const size_t nb_elmt, float *out)
{
    value camlArray;

    camlArray = c_array_to_caml(arrayf, nb_elmt);
    camlArray = wavelets_direct_fun(camlArray);
    memcpy(out,
	    Data_bigarray_val(camlArray),
	    nb_elmt * sizeof(float));
}

/* just fill out.
 * memcpy is necessary because caml release, with the GC, the memory
 * used by camlArray
 */
#if 0
static void fill_data(const size_t size, ihy_data *out)
{
    unsigned int max, i, nbChunk;

    max = (size / CHUNK_SIZE);
    nbChunk = (max + (size % CHUNK_SIZE != 0));
    out->DataChunks = malloc(nbChunk * sizeof(ihy_chunk));
    for (i = 0; i < nbChunk; i++)
    {
	out->DataChunks[i].ChunkSize = CHUNK_SIZE * sizeof(float);
	out->DataChunks[i].Values = malloc(CHUNK_SIZE * sizeof(float));
    };
    out->NbChunk = nbChunk;
}
#endif

#if 0
static size_t next_multiple(const size_t nb)
{
    return ((nb / CHUNK_SIZE) + (nb % CHUNK_SIZE != 0)) * CHUNK_SIZE;
}
#endif

/* compute the result of the OCaml function "Haar_Direct"
 * compress the data and fill out
 * assuming dim = 2^n
 */
void wavelets_direct(const int8_t *array,
		     const size_t sampleSize,
		     const size_t dim,
		     float *out)
{
    unsigned int i, j;
    size_t size = dim / sampleSize;
    float *arrayf = malloc(size * sizeof(float));
    void *number = malloc(sampleSize);

    for (i = 0; i < dim; i += sampleSize)
    {
	j = sampleSize - 1;
	*(int32_t *)number = 0;
	memcpy(number, &array[i], sampleSize);
	switch (sampleSize)
	{
	    case 1:
		arrayf[i / sampleSize] = *(int8_t *)number;
		break;
	    case 2:
		arrayf[i / sampleSize] = *(int16_t *)number;
		break;
	    case 4:
	    default:
		arrayf[i / sampleSize] = *(int32_t *)number;
		break;
	}
    };
    free(number);
    compute_chunk(arrayf, size, out);
    free(arrayf);
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

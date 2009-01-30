#include "interface.h"

/* just call the wavelets, defined in OCaml */
static value ondelette_fun(const value array)
{
    static value *func_ptr = NULL;
    value res;

    if (!func_ptr)
	func_ptr = caml_named_value("ondelettes algo");
    res = callback(*func_ptr, array);
    return res;
}

/* transform a C array to a OCaml bigarray */
static value c_array_to_caml(float *array, const size_t dim)
{
    return alloc_bigarray_dims(BIGARRAY_FLOAT32 | BIGARRAY_C_LAYOUT,
	    1, array, dim);
}

/* compute the result of the OCaml function "ondelettes" */
void ondelette(const int8_t *array,
	       const size_t sampleSize,
	       const size_t dim,
	       ihy_data *out)
{
    unsigned int i, j, i_max, nbChunk;
    size_t size = dim / sampleSize;
    float *arrayf = malloc (size * sizeof(float));
    value camlArray;
    int32_t number;

    for (i = 0; i < dim; i += sampleSize)
    {
	j = sampleSize - 1;
	number = array[i];
	/* converting a "reel" sample to float */
	while (j > 0)
	{
	    number = number << 8;
	    number += array[i + j];
	    j--;
	};
	arrayf[i / 2] = number;
    };
    i_max = (size / NB_BY_O);
    nbChunk = ((size / NB_BY_O) + (size % NB_BY_O != 0));
    out->DataChunks = malloc(nbChunk * sizeof(ihy_chunk));
    for (i = 0; i < i_max; i++)
    {
	camlArray = c_array_to_caml(arrayf + (i * NB_BY_O), NB_BY_O);
	camlArray = ondelette_fun(camlArray);
	out->DataChunks[i].ChunkSize = NB_BY_O * sizeof(float);
	out->DataChunks[i].Values = malloc(NB_BY_O * sizeof(float));
	memcpy(out->DataChunks[i].Values,
		Data_bigarray_val(camlArray),
		NB_BY_O * sizeof(float));
    };
    out->NbChunk = i - 1;
    if (nbChunk != out->NbChunk)
    {
	camlArray = c_array_to_caml(arrayf + (i * NB_BY_O),
				    size - (i * NB_BY_O));
	camlArray = ondelette_fun(camlArray);
	out->NbChunk = i;
	out->DataChunks[i].ChunkSize = (size - (i * NB_BY_O)) * sizeof(float);
	out->DataChunks[i].Values = malloc(out->DataChunks[i].ChunkSize);
	memcpy(out->DataChunks[i].Values,
		Data_bigarray_val(camlArray),
		out->DataChunks[i].ChunkSize);
    }
    free(arrayf);
}

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
float **ondelette(const int8_t *array,
		  const size_t sampleSize,
		  const size_t dim,
		  size_t *array_size)
{
    unsigned int i, j, k;
    size_t size = dim / sampleSize;
    float *arrayf = malloc (0xffff * sizeof(float));
    *array_size = (size / 0xffff) + (size % 0xffff != 0);
    float **res = malloc(*array_size * sizeof(float **));
    value camlArray;
    int32_t number;

    for (k = 0; k < dim; k += 0xffff)
    {
	for (i = 0; i < 0xffff; i++)
	{
	    j = sampleSize - 1;
	    number = array[i + k];
	    /* converting a "reel" sample to float */
	    while (j > 0)
	    {
		number = number << 8;
		number += array[i + j + k];
		j--;
	    };
	    arrayf[i / 2] = number;
	};
	camlArray = c_array_to_caml(arrayf, 0xffff);
	camlArray = ondelette_fun(camlArray);
	res[k % 0xffff] = (float *) Data_bigarray_val(camlArray);
    };
    free(arrayf);
    return res;
}

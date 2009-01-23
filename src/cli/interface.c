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
float *ondelette(const int8_t *array, const size_t sampleSize, const size_t dim)
{
    unsigned int i, j;
    size_t array_size = dim / sampleSize;
    float *arrayf = malloc (array_size * sizeof(float));
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
    camlArray = c_array_to_caml(arrayf, array_size);
    camlArray = ondelette_fun(camlArray);
    free(arrayf);
    return (float *) Data_bigarray_val(camlArray);
}

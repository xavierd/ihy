#include "interface.h"

static value ondelette_fun(const value array)
{
    static value *func_ptr = NULL;
    value res;

    if (!func_ptr)
	func_ptr = caml_named_value("ondelettes algo");
    res = callback(*func_ptr, array);
    return res;
}

static value c_array_to_caml(float *array, const size_t dim)
{
	return alloc_bigarray_dims(BIGARRAY_FLOAT32 | BIGARRAY_C_LAYOUT,
		1, array, dim);
}

float *ondelette(const char *array, const size_t n)
{
    unsigned int i;
    float *arrayf = malloc (n * sizeof(float));
    value camlArray;

    for (i = 0; i < n; i++)
	arrayf[i] = array[i];
    camlArray = c_array_to_caml(arrayf, n);
    camlArray = ondelette_fun(camlArray);
    return (float *) Data_bigarray_val(camlArray);
}

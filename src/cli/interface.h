#ifndef DEF_INTERFACE
#define DEF_INTERFACE

#include <stdint.h>
#include <caml/mlvalues.h>
#include <caml/callback.h>
#include <caml/bigarray.h>

/* compute the result of the OCaml function "ondelettes" */
float **ondelette(const int8_t *array,
		  const size_t sampleSize,
		  const size_t dim,
		  size_t *array_size);

#endif

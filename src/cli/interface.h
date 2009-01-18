#ifndef DEF_INTERFACE
#define DEF_INTERFACE

#include <stdint.h>
#include <caml/mlvalues.h>
#include <caml/callback.h>
#include <caml/bigarray.h>

/* comoute the result of the OCaml function "ondelettes" */
float *ondelette(const char *array, const size_t sampleSize, const size_t dim);

#endif

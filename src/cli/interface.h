#ifndef DEF_INTERFACE
#define DEF_INTERFACE

#include <caml/mlvalues.h>
#include <caml/callback.h>
#include <caml/bigarray.h>

/* comoute the result of the OCaml function "ondelettes" */
float *ondelette(const int *array, const size_t n);

#endif

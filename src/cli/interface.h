#ifndef DEF_INTERFACE
#define DEF_INTERFACE

#include <stdint.h>
#include <pthread.h>
#include <caml/mlvalues.h>
#include <caml/callback.h>
#include <caml/bigarray.h>
#include <caml/memory.h>
#include <unistd.h>

#include "ihy.h"

#define NB_BY_O 65536
#define NB_THREADS 1

/* compute the result of the OCaml function "ondelettes" */
void ondelette(const int8_t *array,
	       const size_t sampleSize,
	       const size_t dim,
	       ihy_data *out);

#endif

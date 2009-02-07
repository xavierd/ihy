#ifndef DEF_INTERFACE
#define DEF_INTERFACE

#include <stdint.h>
#include <pthread.h>
#include <caml/mlvalues.h>
#include <caml/callback.h>
#include <caml/bigarray.h>
#include <caml/memory.h>
#include <unistd.h>

#include <ihy.h>
#include <input/wav.h>

#define NB_BY_O 65536
#define NB_THREADS 1

/* compute the result of the OCaml function "Haar_Direct"
 * compress the data and fill out
 */
void wavelets_direct(const int8_t *array,
		     const size_t sampleSize,
		     const size_t dim,
		     ihy_data *out);

/* compute the result of the OCaml function "Haar_Reverse"
 * uncompress the data and fill out
 * assuming out->BitsPerSample == 16
 */
void wavelets_inverse(float *chunk,
		      const size_t chunkSize,
		      wav_data *out,
		      const int offset);

#endif

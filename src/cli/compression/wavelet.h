#ifndef DEF_INTERFACE
#define DEF_INTERFACE

#include <stdint.h>
#include <caml/mlvalues.h>
#include <caml/callback.h>
#include <caml/bigarray.h>

#include <codecs/ihy.h>
#include <codecs//wav.h>
#include <compression/huffman.h>

#define CHUNK_SIZE 65536

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

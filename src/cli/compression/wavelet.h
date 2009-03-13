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

/* just return the number of chunk needed */
int get_nbChunk(const int chunk_size, const int nb);

/* compute the result of the OCaml function "Haar_Direct"
 * compress the data and fill out
 */
void wavelets_direct(const int8_t *samples,
		     const size_t dim, /* size of prec arg */
		     const size_t sampleSize, /* in bytes */
		     const uint16_t nbChannels,
		     float *out);

/* compute the result of the OCaml function "Haar_Reverse"
 * uncompress the data and fill out
 * assuming out->BitsPerSample == 16
 */
void wavelets_inverse(float *chunk,
		      const size_t chunkSize,
		      const uint8_t nbChannels,
		      int8_t *out,
		      const int offset);

#endif

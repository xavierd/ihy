#include "wavelet.h"

/* just call the wavelets, defined in OCaml */
static value wavelets_direct_fun(const value array)
{
    static value *func_ptr = NULL;
    value res;

    if (!func_ptr)
	func_ptr = caml_named_value("Haar_Direct");
    res = callback(*func_ptr, array);
    return res;
}

static value wavelets_reverse_fun(const value array)
{
    static value *func_ptr = NULL;
    value res;

    if (!func_ptr)
	func_ptr = caml_named_value("Haar_Reverse");
    res = callback(*func_ptr, array);
    return res;
}

/* transform a C array to a OCaml bigarray */
static value c_array_to_caml(float *array, const size_t dim)
{
    return alloc_bigarray_dims(BIGARRAY_FLOAT32 | BIGARRAY_C_LAYOUT,
	    1, array, dim);
}

/* return a boolean, that indicate if there is a bucket to compute, and
 * put the bucket to compute in toCompute
 * the ihy field ihy->NbChunk and ihy->DataChunks[i].ChunkSize also
 */
static int get_next_bucket(int *toCompute, ihy_data *ihy)
{
    static int actualBucket = -1;
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    int max_bucket = ihy->NbChunk;
    int res;

    pthread_mutex_lock(&mutex);
    actualBucket++;
    res = actualBucket < max_bucket;
    *toCompute = actualBucket;
    pthread_mutex_unlock(&mutex);
    return res;
}

/* this function assume that ihy is correctly malloc'ed */
static void compute_bucket(const int toCompute, float *arrayf, ihy_data *ihy)
{
    value camlArray;
    int size;

    size = ihy->DataChunks[toCompute].ChunkSize / sizeof(float);
    camlArray = c_array_to_caml(arrayf + (toCompute * NB_BY_O),  size);
    camlArray = wavelets_direct_fun(camlArray);
    memcpy(ihy->DataChunks[toCompute].Values,
	    Data_bigarray_val(camlArray),
	    size);
}

struct thread_data
{
    float *arrayf;
    ihy_data *ihy;
};

/* this function is executed by the thread */
static void *thread_function(void *thread_data)
{
    struct thread_data *data = thread_data;
    int bucket;

    while (get_next_bucket(&bucket, data->ihy))
	compute_bucket(bucket, data->arrayf, data->ihy);
    return NULL;
}

/* just fill out.
 * memcpy is necessary because caml release, with the GC, the memory
 * used by camlArray
 */
static void fill_data(const size_t size, ihy_data *out)
{
    unsigned int max, i, nbChunk;

    max = (size / NB_BY_O);
    nbChunk = (max + (size % NB_BY_O != 0));
    out->DataChunks = malloc(nbChunk * sizeof(ihy_chunk));
    for (i = 0; i < nbChunk; i++)
    {
	out->DataChunks[i].ChunkSize = NB_BY_O * sizeof(float);
	out->DataChunks[i].Values = malloc(NB_BY_O * sizeof(float));
    };
    out->NbChunk = nbChunk;
    /*
    if (nbChunk != out->NbChunk)
    {
	out->NbChunk = i;
	printf("%d\n", i);
	out->DataChunks[i].ChunkSize = (size - (i * NB_BY_O)) * sizeof(float);
	out->DataChunks[i].Values = malloc(out->DataChunks[i].ChunkSize);
    }
    */
}

static size_t next_multiple(const size_t nb)
{
    return ((nb / NB_BY_O) + (nb % NB_BY_O != 0)) * NB_BY_O;
}

/* compute the result of the OCaml function "Haar_Direct"
 * compress the data and fill out
 */
void wavelets_direct(const int8_t *array,
		     const size_t sampleSize,
		     const size_t dim,
		     ihy_data *out)
{
    unsigned int i, j;
    size_t size = next_multiple(dim / sampleSize);
    float *arrayf = malloc(size * sizeof(float));
    int16_t number;
    pthread_t *threads = malloc(NB_THREADS * sizeof(pthread_t));
    struct thread_data dat;
    /*pid_t pid;*/

    for (i = 0; i < dim; i += sampleSize)
    {
	j = sampleSize - 1;
	memcpy(&number, &array[i], sampleSize);
	//number = array[i + sampleSize];
	/* converting a "reel" sample to float */
	/*
	while (j > 0)
	{
	    number = number << 8;
	    number += array[i + j];
	    j--;
	};
	*/
	arrayf[i / 2] = number;
	//printf("%d, %f\n", number, arrayf[i/2]);
    };
    fill_data(size, out);
    dat.arrayf = arrayf;
    dat.ihy = out;
    /*
    pid = fork();
    if (pid > 0)
    {
	thread_function(&dat);
	wait(NULL);
    }
    else
    {
	thread_function(&dat);
	free(arrayf);
	exit(0);
    }
    */
    for(i = 0; i < NB_THREADS; i++)
	pthread_create(&threads[i], NULL, thread_function, &dat);
    for (i = 0; i < NB_THREADS; i++)
	pthread_join(threads[i], NULL);
    free(arrayf);
    return;
}

/* compute the result of the OCaml function "Haar_Reverse"
 * uncompress the data and fill out
 * assuming out->BitsPerSample == 16
 */
void wavelets_inverse(float *chunk,
		      const size_t nbelmts,
		      wav_data *out,
		      const int offset)
{
    value camlArray;
    int16_t val;
    float valf;
    unsigned int i, j;

    camlArray = c_array_to_caml(chunk, nbelmts);
    camlArray = wavelets_reverse_fun(camlArray);

    printf("%d\n", nbelmts);
    printf("offset : %d\n", offset);
    j = 0;
    for (i = 0; i < nbelmts; i++)
    {
	valf = ((float *)(Data_bigarray_val(camlArray)))[i];
	val = valf;
	/*
	printf("val : %d, data : %d\n", val,
		out->Data[i + offset] + (out->Data[i + 1 + offset] << 8));
		*/
	memcpy(out->Data + offset + j, &val, 2);
	j = j + 2;
    }
}

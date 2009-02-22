#ifndef DEF_WAV
#define DEF_WAV

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <utils/buffer.h>
#include <output/ao.h>

/* wav file structure */
/* http://ccrma.stanford.edu/courses/422/projects/WaveFormat/
 * for more precision
 */
typedef struct wav_data
{
    /* RIFF header */
    char		ChunkID[4]; /* "RIFF" */
    uint32_t		ChunkSize;
    char		Format[4]; /* WAVE */
    /* fmt header */
    char		FormatBlocID[4]; /* "fmt " */
    uint32_t		FormatBlocSize;
    uint16_t		AudioFormat;
    uint16_t		NumChannels;
    uint32_t		SampleRate;
    uint32_t		ByteRate;
    uint16_t		BlockAlign;
    uint16_t		BitsPerSample;
    /* data */
    char		DataBlocID[4]; /* "data" */
    uint32_t		DataBlocSize;
    int8_t		*Data;
    /* How to get sample at time T and in channel C :
     * Data[(BitsPerSample/8)*NumChannels*T+C]
     * This will point to a Sample
     */
} wav_data;

/* return a new allocated wav */
wav_data *create_wav();

/* read a wav PCM file from disk */
void read_wav(const char *filename, wav_data *data);

/* write a wav to the disk */
void write_wav(const wav_data *data, const char *filename);

/* play the wav in streaming */
void play_wav_streaming(wav_data *data);

/* release the memory used by a wav */
void destroy_wav(wav_data *data);

#endif

#ifndef DEF_PCM
#define DEF_PCM

#include <stdint.h>

/* wav file structure */
/* http://ccrma.stanford.edu/courses/422/projects/WaveFormat/
 * for more precision
 */
typedef struct
{
    /* RIFF header */
    char		ChunkID[4]; /* "RIFF" */
    uint32_t		ChunkSize;
    char		Format[4]; /* WAVE */
    /* fmt */
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
     * This will point to a Sample of (BitsPerSample/8)*sizeof(int8_t)
     */
} wav_file;

wav_file *open_wav(const char *filename);
void close_wav(wav_file *file);

#endif

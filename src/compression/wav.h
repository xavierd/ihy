#ifndef DEF_WAV
#define DEF_WAV

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
} wav_data;

wav_data *create_wav();
void read_wav(const char *filename, wav_data *data);
void write_wav(const wav_data *data, const char *filename);
void destroy_wav(wav_data *data);

#endif

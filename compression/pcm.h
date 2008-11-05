#ifndef DEF_PCM
#define DEF_PCM

/* wav file structure */
/* http://ccrma.stanford.edu/courses/422/projects/WaveFormat/
 * for more precision
 */
typedef struct {
	/* RIFF header */
	unsigned int		ChunkID; /* "RIFF" */
	unsigned int		ChunkSize;
	unsigned int		Format;
	/* fmt */
	unsigned int		Subchunk1ID; /* "fmt " */
	unsigned int		Subchunk1Size;
	unsigned short int	AudioFormat;
	unsigned short int	NumChannels;
	unsigned int		SampleRate;
	unsigned int		ByteRate;
	unsigned short int	BlockAlign;
	unsigned short int	BitsPerSample;
	/* data */
	unsigned int		Subchunk2ID; /* "data" */
	unsigned int		Subchunk2Size;
	char				**Data; /* mono/stereo matrix */
} wav_file;

wav_file *open_wav(const char *filename);

#endif

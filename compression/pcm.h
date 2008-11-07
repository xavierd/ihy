#ifndef DEF_PCM
#define DEF_PCM

/* wav file structure */
/* http://ccrma.stanford.edu/courses/422/projects/WaveFormat/
 * for more precision
 */
typedef struct {
	/* RIFF header */
	char				ChunkID[4]; /* "RIFF" */
	unsigned int		ChunkSize;
	char				Format[4]; /* WAVE */
	/* fmt */
	char				FormatBlocID[4]; /* "fmt " */
	unsigned int		FormatBlocSize;
	unsigned short int	AudioFormat;
	unsigned short int	NumChannels;
	unsigned int		SampleRate;
	unsigned int		ByteRate;
	unsigned short int	BlockAlign;
	unsigned short int	BitsPerSample;
	/* data */
	char				DataBlocID[4]; /* "data" */
	unsigned int		DataBlocSize;
	char				*Data;
	/* How to get sample at time T and in channel C :
	 * Data[(BitsPerSample/8)*NumChannels*T+C]
	 * This will point to a Sample of (BitsPerSample/8)*sizeof(char)
	 */
} wav_file;

wav_file *open_wav(const char *filename);

#endif

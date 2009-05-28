#include <stdlib.h>
#include <pthread.h>

#include <codecs/wav.h>
#include <codecs/ihy.h>
#include <compression/ihy.h>
#include <audio_output/ihy_streaming.h>

static void *thread_play_ihy(void *data)
{
    play_ihy_streaming(data);
    return NULL;
}

static void extract_ihy(char *input_filename, char *output_filename)
{
    ihy_data *input;
    wav_data *output;
    uint32_t offset;
    unsigned int i;
    ihy_chunk *chunk;

    input = create_ihy();
    read_ihy(input_filename, input);
    output = create_wav();
    output->ChunkID[0] = 'R';
    output->ChunkID[1] = 'I';
    output->ChunkID[2] = 'F';
    output->ChunkID[3] = 'F';
    output->ChunkSize = sizeof(wav_data) - 12;
    output->Format[0] = 'W';
    output->Format[1] = 'A';
    output->Format[2] = 'V';
    output->Format[3] = 'E';
    output->FormatBlocID[0] = 'f';
    output->FormatBlocID[1] = 'm';
    output->FormatBlocID[2] = 't';
    output->FormatBlocID[3] = ' ';
    output->FormatBlocSize = 16;
    output->AudioFormat = 1;
    output->NumChannels = input->Channels;
    output->SampleRate = input->Frequency;
    output->BitsPerSample = 16;
    output->BlockAlign = output->NumChannels * (output->BitsPerSample / 8);
    output->ByteRate = output->SampleRate * output->BlockAlign;
    output->DataBlocID[0] = 'd';
    output->DataBlocID[1] = 'a';
    output->DataBlocID[2] = 't';
    output->DataBlocID[3] = 'a';
    offset = 0;
    for (i = 0; i < input->NbChunk; i++)
	offset += input->ChunkSize * 2;
    output->DataBlocSize = offset;
    output->ChunkSize += output->DataBlocSize;
    output->Data = malloc(output->DataBlocSize * sizeof(char));
    offset = 0;
    for (i = 0; i < input->NbChunk; i++)
    {
	chunk = &input->DataChunks[i];
	uncompress_chunk(chunk, output->Data + offset, input->Channels);
	offset += input->ChunkSize * 2;
    };
    write_wav(output, output_filename);
    destroy_wav(output);
    destroy_ihy(input);
}

static void compress_wav(char *input_filename, char *output_filename,
			 int quality)
{
    ihy_data *output;
    wav_data *input;
    unsigned int i;
    ihy_chunk *chunk;

    output = create_ihy();
    input = create_wav();
    read_wav(input_filename, input);

    output->ChunkSize = CHUNK_SIZE; /* Default */
    output->NbChunk = get_nbChunk(output->ChunkSize,
	    input->DataBlocSize / (input->BitsPerSample / 8));
    output->DataChunks = malloc(sizeof(ihy_chunk) * output->NbChunk);
    for (i = 0; i < output->NbChunk; i++)
    {
	size_t size = 0;
	size_t real_size = 0;

	size = output->ChunkSize * (input->BitsPerSample / 8);
	/* avoid garbage on the last chunk */
	if (i == output->NbChunk - 1)
	    real_size = input->DataBlocSize % CHUNK_SIZE;
	else
	    real_size = size;
	chunk = &output->DataChunks[i];
	chunk->ChunkSize = output->ChunkSize;
	chunk->QBitsPerCoefs = quality;
	compress_chunk(input->Data + (i * size), real_size, input->NumChannels, chunk);
    };

    output->FileID[0] = 'S';
    output->FileID[1] = 'N';
    output->FileID[2] = 'X';
    output->FileID[3] = 'T';
    output->FileSize = 0;
    output->CompressionType = 0;
    output->Channels = input->NumChannels;
    output->Frequency = input->SampleRate;
    output->Artist = malloc(5 * sizeof(char));
    output->Artist[4] = '\0';
    output->ArtistLength = strlen(output->Artist);
    output->Album = malloc(5 * sizeof(char));
    output->Album[4] = '\0';
    output->AlbumLength = strlen(output->Album);
    output->Track = malloc(5 * sizeof(char));
    output->Track[4] = '\0';
    output->TrackLength = strlen(output->Track);
    output->Year = 2009;
    output->Genre = 42;
    output->Comment = malloc(25 * sizeof(char));
    output->Comment[24] = '\0';
    output->CommentLength = strlen(output->Comment);
    write_ihy(output, output_filename);
    destroy_ihy(output);
    destroy_wav(input);
}

static void print_help()
{
    printf("Usage : ihyconvert [options] [mode] [file ...]\n");
    printf("Official converter of the ihy codec\n\n");
    printf("  -c IN.wav OUT.ihy\t: compress IN into OUT\n");
    printf("  -x IN.ihy OUT.wav\t: extract OUT from IN\n");
    printf("  -r IN.ihy\t\t: play IN\n");
    printf("  -q N\t\t\t: set the quality of the output to N (default 3)\n");
    printf("  -h\t\t\t: display this help\n");
}

int main(int argc, char **argv)
{
    pthread_t play;
    int i;
    int is_thread_playing_ihy = 0;
    ihy_data *input_to_play = NULL;
    int nb_threads = 2;
    int q = 3;

    if (argc == 1)
    {
	print_help();
	return 1;
    };
    caml_main(argv);
    i = argc - 1;
    while (i > 0)
    {
	if (!strcmp(argv[argc - i],"-h"))
	{
	    print_help();
	    return 1;
	}
	else if (!strcmp(argv[argc - i], "-x"))
	{
	    printf("Extracting data ... ");
	    fflush(stdout);
	    extract_ihy(argv[argc - i + 1], argv[argc - i + 2]);
	    i -= 3;
	    printf("DONE\n");
	}
	else if (!strcmp(argv[argc - i], "-c"))
	{
	    printf("Compressing data ... ");
	    fflush(stdout);
	    compress_wav(argv[argc - i + 1], argv[argc - i + 2], q);
	    i -= 3;
	    printf("DONE\n");
	}
	else if (!strcmp(argv[argc - i], "-r"))
	{
	    is_thread_playing_ihy = 1;
	    input_to_play = create_ihy();
	    read_ihy(argv[argc - i + 1], input_to_play);
	    pthread_create(&play, NULL, thread_play_ihy, input_to_play);
	    i -= 2;
	}
	else if (!strcmp(argv[argc - i], "-q"))
	{
	    q = atoi(argv[argc - i + 1]);
	    i -= 2;
	}
	else
	{
	    printf("Not a valid argument.\n");
	    print_help();
	    return 1;
	}
    };
    if (is_thread_playing_ihy)
    {
	printf("Waiting for the reader to stop ... ");
	fflush(stdout);
	pthread_join(play, NULL);
	destroy_ihy(input_to_play);
	printf("DONE\n");
    }
    return 0;
}

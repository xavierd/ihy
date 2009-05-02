#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <caml/mlvalues.h>
#include <caml/callback.h>
#include <caml/bigarray.h>

#include <codecs/wav.h>
#include <codecs/ihy.h>
#include <compression/wavelet.h>
#include <compression/quantization.h>
#include <compression/huffman.h>
#include <audio_output/wav_streaming.h>
#include <audio_output/ihy_streaming.h>
#include <utils/half.h>
#include <main_threading.h>

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
    uint8_t *oldValue;
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
	offset +=
	    (((size_t *)input->DataChunks[i].Values)[0] / sizeof(uint16_t)) * 4;
    output->DataBlocSize = offset;
    output->ChunkSize += output->DataBlocSize;
    output->Data = malloc(output->DataBlocSize * sizeof(char));
    offset = 0;
    for (i = 0; i < input->NbChunk; i++)
    {
	chunk = &input->DataChunks[i];
	oldValue = chunk->Values;
	chunk->Values = huffman_decode(chunk->Values, &chunk->ChunkSize);
	free(oldValue);
	oldValue = chunk->Values;
	chunk->Values = (uint8_t *)halfarray_to_float(
	    (uint16_t *)oldValue, chunk->ChunkSize / sizeof(uint16_t));
	chunk->ChunkSize *= 2;
	free(oldValue);
	dequantizate(chunk->Values, chunk->ChunkSize / sizeof(float), 80.0f);
	wavelets_inverse((float *)chunk->Values,
		(chunk->ChunkSize / sizeof(float)),
		input->Channels,
		output->Data,
		offset);
	offset += (chunk->ChunkSize / sizeof(float)) * 4;
    };
    write_wav(output, output_filename);
    destroy_wav(output);
    destroy_ihy(input);
}

static void compress_wav(char *input_filename, char *output_filename,
			 int nb_threads)
{
    ihy_data *output;
    wav_data *input;

    output = create_ihy();
    input = create_wav();
    read_wav(input_filename, input);

    output->NbChunk = get_nbChunk(CHUNK_SIZE,
	    input->DataBlocSize / (input->BitsPerSample / 8));
    output->DataChunks = malloc(sizeof(ihy_chunk) * output->NbChunk);

    encode_ihy(nb_threads, output->NbChunk, input, output);

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
    printf(	"Usage : ihyconvert [options] [mode] [file ...]\n");
    printf(	"Official converter of the ihy codec\n\n");
    printf(	"  -c IN.wav OUT.ihy	: compress IN into OUT\n");
    printf(	"  -x IN.ihy OUT.wav	: extract OUT from IN\n");
    printf(	"  -r IN.ihy		: play IN\n");
    printf(	"  -j N			: set the number of created processes to N (default 2)\n");
    printf(	"  -h			: display this help\n");
}

int main(int argc, char **argv)
{
    pthread_t play;
    int i;
    int is_thread_playing_ihy = 0;
    ihy_data *input_to_play;
    int nb_threads = 2;

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
	    compress_wav(argv[argc - i + 1], argv[argc - i + 2], nb_threads);
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
	else if (!strcmp(argv[argc - i], "-j"))
	{
	    nb_threads = atoi(argv[argc - i + 1]);
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

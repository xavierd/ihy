#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <caml/mlvalues.h>
#include <caml/callback.h>
#include <caml/bigarray.h>

#include <input/wav.h>
#include <ihy.h>
#include <wavelet.h>
#include <lossless/huffman.h>
#include <output/ao.h>

static void *thread_play_wav(void *wav)
{
    play_wav((wav_data *)wav);
    return NULL;
}

struct thread_data
{
    char	**argv;
    wav_data	*wav;
};

static void uncompress_ihy(char *input_filename, char *output_filename)
{
    ihy_data *input;
    wav_data *output;
    uint32_t offset;
    unsigned int i;

    input = create_ihy();
    read_ihy(input_filename, input);
    output = create_wav();
    output->ChunkID[0] = 'R';
    output->ChunkID[1] = 'I';
    output->ChunkID[2] = 'F';
    output->ChunkID[3] = 'F';
    output->ChunkSize = 8 * sizeof(wav_data); /* temporary */
    output->Format[0] = 'W';
    output->Format[1] = 'A';
    output->Format[2] = 'V';
    output->Format[3] = 'E';
    output->FormatBlocID[0] = 'f';
    output->FormatBlocID[1] = 'm';
    output->FormatBlocID[2] = 't';
    output->FormatBlocID[3] = ' ';
    output->FormatBlocSize = 22 - 8; /* temporary */
    output->AudioFormat = 1;
    output->NumChannels = input->Channels;
    output->SampleRate = input->Frequency;
    output->BitsPerSample = 16;
    output->BlockAlign = output->NumChannels * (output->BitsPerSample / 8);
    output->ByteRate = output->SampleRate * output->BitsPerSample;
    output->DataBlocID[0] = 'd';
    output->DataBlocID[1] = 'a';
    output->DataBlocID[2] = 't';
    output->DataBlocID[3] = 'a';
    offset = 0;
    for (i = 0; i < input->NbChunk; i++)
	offset += (input->DataChunks[i].ChunkSize / sizeof(float)) * 3;
    output->DataBlocSize = offset;
    output->ChunkSize += offset;
    output->Data = malloc(output->DataBlocSize * sizeof(char));
    offset = 0;
    for (i = 0; i < input->NbChunk; i++)
    {
	wavelets_inverse(input->DataChunks[i].Values,
		(input->DataChunks[i].ChunkSize / sizeof(float)),
		output,
		offset);
	offset += (input->DataChunks[i].ChunkSize / sizeof(float)) * 2;
    };
    write_wav(output, output_filename);
    destroy_wav(output);
    destroy_ihy(input);
}

static void compress_wav(char *input_filename, char *output_filename)
{
    ihy_data *output;
    wav_data *input;

    output = create_ihy();
    input = create_wav();
    read_wav(input_filename, input);
    wavelets_direct(input->Data,
	    input->BitsPerSample / 8,
	    input->DataBlocSize,
	    output);
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

#if 0
static void *master_thread(void *dat)
{
    struct thread_data *data = dat;
    ihy_data *output;
    huffman_tree *B;
    wav_data *wav;
    size_t size;
    int8_t *encoded;

    /* wavelets */
    printf("Using wavelets on data ... ");
    fflush(stdout);

    output = create_ihy();
    wavelets_direct(data->wav->Data,
	    data->wav->BitsPerSample / 8,
	    data->wav->DataBlocSize,
	    output);
    printf("DONE\n");
    fflush(stdout);

    /* huffman tree */
    printf("Creating Huffman tree ... ");
    fflush(stdout);
    B = build_huffman(data->wav->Data, data->wav->DataBlocSize);
    size = data->wav->DataBlocSize;
    encoded = huffman_encode(data->wav->Data, &size, B);
    printf("size : %d, orig : %d ", size, data->wav->DataBlocSize);
    free(encoded);
    /*huffman_pretty(B, 0, 0);*/
    printf("DONE\n");
    fflush(stdout);
    destroy_huffman(B);

    printf("write wav...");
    fflush(stdout);
    write_wav(wav, data->argv[3]);
    destroy_wav(wav);
    printf("DONE\n");

    destroy_ihy(output);
    return NULL;
}
#endif

int main(int argc, char **argv)
{
    pthread_t play;
    int i;

    caml_main(argv);
    i = argc - 1;
    while (i > 0)
    {
	if (!strcmp(argv[argc - i],"-h"))
	{
	    printf("help\n");
	    return 1;
	}
	else if (!strcmp(argv[argc - i], "-x"))
	{
	    uncompress_ihy(argv[argc - i + 1], argv[argc - i + 2]);
	    i -= 3;
	}
	else if (!strcmp(argv[argc - i], "-c"))
	{
	    compress_wav(argv[argc - i + 1], argv[argc - i + 2]);
	    i -= 3;
	}
	else if (!strcmp(argv[argc - i], "-r"))
	{
	    wav_data *input;

	    input = create_wav();
	    read_wav(argv[argc - i + 1], input);
	    pthread_create(&play, NULL, thread_play_wav, input);
	    destroy_wav(input);
	    i -= 2;
	};
    };
    pthread_join(play, NULL);
    return 0;
}

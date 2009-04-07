#include <main_threading.h>

typedef struct s_son {
    pid_t pid;
    int numchunk;
    int pipe[2];
} t_son;

static void
proceed_chunk (int outfd, int chunkid, wav_data *input, ihy_data *output)
{
    size_t size = 0;
    size_t real_size = 0;
    unsigned int i;
    uint8_t *oldValue;
    int shmid;
    void *shmaddr;

    i = chunkid;

    size = CHUNK_SIZE * (input->BitsPerSample / 8);
    /* avoid garbage on the last chunk */
    if (i == output->NbChunk - 1)
	real_size = input->DataBlocSize % CHUNK_SIZE;
    else
	real_size = size;
    output->DataChunks[i].Values = malloc(sizeof(float) * CHUNK_SIZE);
    output->DataChunks[i].ChunkSize = CHUNK_SIZE * sizeof(float);
    wavelets_direct(input->Data + (i * size), size, real_size,
	input->BitsPerSample / 8, input->NumChannels,
	(float *)output->DataChunks[i].Values);
    oldValue = output->DataChunks[i].Values;
    output->DataChunks[i].Values = (uint8_t *)floatarray_to_half(
	(float *)oldValue,
	output->DataChunks[i].ChunkSize / sizeof(float)
    );
    output->DataChunks[i].ChunkSize /= 2;
    free(oldValue);
    oldValue = output->DataChunks[i].Values;
    output->DataChunks[i].Values =
	huffman_encode(
	output->DataChunks[i].Values,
	&output->DataChunks[i].ChunkSize
	);
    free(oldValue);

    shmid = shmget(
	IPC_PRIVATE,
	output->DataChunks[i].ChunkSize,
	IPC_CREAT | SHM_R | SHM_W
    );
    write(outfd, &shmid, sizeof(int));
    write(outfd, &(output->DataChunks[i].ChunkSize), sizeof(uint32_t));
    shmaddr = shmat(shmid, NULL, 0);
    memcpy(
	shmaddr,
	output->DataChunks[i].Values,
	output->DataChunks[i].ChunkSize
    );
}

void encode_ihy(int nbcpu, int nbchunks, wav_data *input, ihy_data *output)
{
    t_son *sons;
    int nbsons;
    int donechunks;
    int i;
    char isson;

    if (nbchunks < nbcpu)
	nbcpu = nbchunks;

    sons = malloc(nbcpu * sizeof(t_son));

    nbsons = 0;
    i = 0;
    isson = 0;
    donechunks = 0;
    while ((i < nbcpu) && (!isson))
    {
	sons[i].numchunk = donechunks;
	pipe(sons[i].pipe);
	if (!(sons[i].pid = fork()))
	{
	    isson = 1;
	}
	else
	{
	    nbsons++;
	    i++;
	    donechunks++;
	}
    }

    if (!isson)
    {
	while ((nbsons > 0) && (!isson))
	{
	    pid_t pid;
	    int shmid;
	    void *shmaddr;
	    int chunksize;
	    uint8_t *values;
	    int rpipe;

	    pid = wait(NULL);
	    nbsons--;
	    i = 0;
	    while (sons[i].pid != pid)
	    {
		i++;
	    }
	    rpipe = sons[i].pipe[0];
	    chunksize = sons[i].numchunk;
	    read(rpipe, &shmid, sizeof(int));
	    shmaddr = shmat(shmid, NULL, 0);
	    read(rpipe, &chunksize, sizeof(uint32_t));
	    values = malloc(chunksize);
	    output->DataChunks[sons[i].numchunk].Values = values;
	    memcpy(values, shmaddr, chunksize);
	    shmdt(shmaddr);
	    shmctl(shmid, IPC_RMID, NULL);
	    close(sons[i].pipe[0]);
	    close(sons[i].pipe[1]);
	    if (donechunks < nbchunks)
	    {
		sons[i].numchunk = donechunks;
		pipe(sons[i].pipe);
		if (!(sons[i].pid = fork()))
		{
		    isson = 1;
		    proceed_chunk(
			sons[i].pipe[1],
			sons[i].numchunk,
			input,
			output
		    );
		    _exit(0);
		}
		else
		{
		    donechunks++;
		    nbsons++;
		}
	    }
	}
    }
    else
    {
	close(sons[i].pipe[0]);
	proceed_chunk(sons[i].pipe[1], sons[i].numchunk, input, output);
	_exit(0);
    }
}

#include <main_threading.h>

typedef struct s_son {
    pid_t pid;
    int numchunk;
    int pipe[2];
} t_son;

static void
proceed_chunk (int outfd, int chunkid, int quality, wav_data *input,
						    ihy_data *output)
{
    size_t size = 0;
    size_t real_size = 0;
    unsigned int i;
    uint8_t *oldValue;
    int shmid;
    int nbbits;
    void *shmaddr;
    ihy_chunk *chunk;

    i = chunkid;

    chunk = &output->DataChunks[i];
    size = CHUNK_SIZE * (input->BitsPerSample / 8);
    /* avoid garbage on the last chunk */
    if (i == output->NbChunk - 1)
	real_size = input->DataBlocSize % CHUNK_SIZE;
    else
	real_size = size;
    chunk->Values = malloc(sizeof(float) * (CHUNK_SIZE / 2));
    chunk->ChunkSize = (CHUNK_SIZE / 2)* sizeof(float);
    wavelets_direct(input->Data + (i * size), size, real_size,
	input->BitsPerSample / 8, input->NumChannels,
	(float *)chunk->Values);
    /* While size is not good */
    do
    {
	void *tmp;

	oldValue = chunk->Values;
	size = chunk->ChunkSize / sizeof(float);
	oldValue = quantizate((float *)oldValue, &size, 200.0f, &nbbits);
	/*
	tmp = floatarray_to_half((float *)oldValue, size);
	size /= 2;
	free(oldValue);
	oldValue = chunk->Values;
	*/
	tmp = oldValue;
	chunk->HUncompressedSize = size;
	oldValue = huffman_encode(tmp, &size);
	free(tmp);
    }
    while (!size /* is not acceptable */);
    chunk->QBitsPerCoefs = nbbits;
    chunk->QScaleFactor = 200.0f;
    chunk->ChunkSize = size;
    chunk->Values = oldValue;

    shmid = shmget(IPC_PRIVATE, chunk->ChunkSize, IPC_CREAT | SHM_R | SHM_W);
    write(outfd, &shmid, sizeof(int));
    write(outfd, &chunk->ChunkSize, sizeof(uint32_t));
    write(outfd, &chunk->QBitsPerCoefs, sizeof(uint8_t));
    write(outfd, &chunk->QScaleFactor, sizeof(uint16_t));
    write(outfd, &chunk->HUncompressedSize, sizeof(uint32_t));
    shmaddr = shmat(shmid, NULL, 0);
    memcpy(shmaddr, chunk->Values, chunk->ChunkSize);
}

void encode_ihy(int nbcpu, int nbchunks, int quality, wav_data *input,
						      ihy_data *output)
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
	    int chunksize, nbbits, factor, size;
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
	    read(rpipe, &shmid, sizeof(int));
	    shmaddr = shmat(shmid, NULL, 0);
	    read(rpipe, &chunksize, sizeof(uint32_t));
	    read(rpipe, &nbbits, sizeof(uint8_t));
	    read(rpipe, &factor, sizeof(uint16_t));
	    read(rpipe, &size, sizeof(uint32_t));
	    values = malloc(chunksize);
	    output->DataChunks[sons[i].numchunk].Values = values;
	    output->DataChunks[sons[i].numchunk].ChunkSize = chunksize;
	    output->DataChunks[sons[i].numchunk].QBitsPerCoefs = nbbits;
	    output->DataChunks[sons[i].numchunk].QScaleFactor = factor;
	    output->DataChunks[sons[i].numchunk].HUncompressedSize = size;
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
			quality,
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
	proceed_chunk(sons[i].pipe[1], sons[i].numchunk, quality, input, output);
	_exit(0);
    }
}

#include <main_threading.h>

typedef struct s_son {
	pid_t pid;
	int numchunk;
	int pipe[2];
} t_son;

static void proceed_chunk(int outfd, int sonid)
{
    char buf[128];	

    strcpy(buf, "mfvp");
    buf[4] = sonid + 48;
    buf[5] = 0;
    write(outfd, buf, 128);
}

void encode_ihy(int nbcpu, int nbchunks)
{
    t_son *sons;
    int nbsons;
    int i;
    char isson;
    int donechunks;
    char buf[128];

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

	    pid = wait(NULL);
	    nbsons--;
	    i = 0;
	    while (sons[i].pid != pid)
	    {
		i++;
	    }
	    read(sons[i].pipe[0], buf, 128);
	    close(sons[i].pipe[0]);
	    close(sons[i].pipe[1]);
	    printf("%s\n", buf);
	    if (donechunks < nbchunks)
	    {
		sons[i].numchunk = donechunks;
		pipe(sons[i].pipe);
		if (!(sons[i].pid = fork()))
		{
		    isson = 1;
		    proceed_chunk(sons[i].pipe[1], i);
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
	proceed_chunk(sons[i].pipe[1], i);
    }
}

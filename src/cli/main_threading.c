static void encode_ihy(int nbcpu)
{
    pid_t *children;
    int *pipes;
    int i;
    char buf[128];

    children = malloc(nbcpu * sizeof(pid_t));
    pipes = malloc(nbcpu * 2 * 2 * sizeof(int));

    for (i = 0; i < nbcpu; i++)
    {
	pipe(&pipes[4 * i + 2 * 0]);
	pipe(&pipes[4 * i + 2 * 1]);
    }

    i = 0;
    while ((i < nbcpu) && (children[i] = fork()))
    {
	i++;
    }

    if (i == nbcpu)
    {
	for (i = 0; i < nbcpu; i++)
	{
	    close(pipes[4 * i + 2 * 0 + 1]);
	    close(pipes[4 * i + 2 * 1 + 0]);
	}

	strcpy(buf, "mpvf");
	for (i = 0; i < nbcpu; i++)
	{
	    buf[4] = i + 48;
	    buf[5] = 0;
	    write(pipes[4 * i + 2 * 1 + 1], buf, 5);
	    printf("le pere a envoye au fils %i : %s\n", i, buf);
	    close(pipes[4 * i + 2 * 1 + 1]);
	}
	for (i = 0; i < nbcpu; i++)
	{
	    wait(NULL);
	}
    }
    else
    {
	int nbchild;

	nbchild = 0;
	while (children[nbchild])
	{
	    nbchild++;
	}

	for (i = 0; i < nbcpu; i++)
	{
	    if (i == nbchild)
	    {
		close(pipes[4 * i + 2 * 0 + 0]);
		close(pipes[4 * i + 2 * 1 + 1]);
	    }
	    else
	    {
		close(pipes[4 * i + 0 * 2 + 0]);
		close(pipes[4 * i + 0 * 2 + 1]);
		close(pipes[4 * i + 1 * 2 + 0]);
		close(pipes[4 * i + 1 * 2 + 1]);
	    }
	}

	srandom(nbchild);
	usleep(random() / 1000);
	read(pipes[4 * nbchild + 2 * 1 + 0], buf, 5);
	close(pipes[4 * nbchild + 2 * 1 + 0]);
	buf[5] = 0;
	printf("le fils %i a recu : %s\n", nbchild, buf);
    }
}

int main()
{
	encode_ihy(8);
	return 0;
}

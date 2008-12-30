#include <stdio.h>
#include <stdlib.h>

#include "ihy.h"

ihy_file *open_ihy(const char *filename)
{
    FILE *file;
    ihy_file *result;

    file = fopen(filename, "rb");
    if (!file)
    {
	printf("file not exist\n");
	exit(1);
    }
    result = malloc(sizeof(ihy_file));

    fclose(file);
    /* return result */
    return NULL;
}

void close_ihy(ihy_file *file)
{
    free(file->Data->Sample);
    free(file->Data);
    free(file);
}

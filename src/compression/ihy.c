#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ihy.h"

ihy_data *create_ihy()
{
    ihy_data *result;

    result = malloc(sizeof(ihy_data));
    result->Artist = "";
    result->Album = "";
    result->Track = "";
    result->Comment = "";
    result->DataChunks = NULL;
    return result;
}

void write_ihy(const ihy_data *data, const char *filename)
{
    FILE *file;
    uint64_t i;

    /* Open file for writing */
    file = fopen(filename, "wb");
    if (!file)
    {
	exit(1);
    }
    /* Write header */
    fwrite(&data->FileID, sizeof(char), 4, file);
    fwrite(&data->FileSize, sizeof(uint64_t), 1, file);
    fwrite(&data->CompressionType, sizeof(uint16_t), 1, file);
    fwrite(&data->Channels, sizeof(uint8_t), 1, file);
    fwrite(&data->Frequency, sizeof(uint32_t), 1, file);
    /* Write tags */
    fwrite(&data->ArtistLength, sizeof(uint16_t), 1, file);
    fwrite(&data->Artist, sizeof(char), strlen(data->Artist), file);
    fwrite(&data->AlbumLength, sizeof(uint16_t), 1, file);
    fwrite(&data->Album, sizeof(char), strlen(data->Album), file);
    fwrite(&data->TrackLength, sizeof(uint16_t), 1, file);
    fwrite(&data->Track, sizeof(char), strlen(data->Track), file);
    fwrite(&data->Year, sizeof(uint8_t), 1, file);
    fwrite(&data->Genre, sizeof(uint8_t), 1, file);
    fwrite(&data->CommentLength, sizeof(uint32_t), 1, file);
    fwrite(&data->Comment, sizeof(char), strlen(data->Comment), file);
    /* Write data */
    /* this code doesn't work */
    /* see http://en.wikipedia.org/wiki/Sizeof#Using_sizeof_with_arrays */
    for (i = 0; i < (sizeof(data->DataChunks) / sizeof(ihy_chunk)); i++)
    {
	/* fix me */
    }
    /* Close file */
    fclose(file);
}

void destroy_ihy(ihy_data *data)
{
    uint64_t i;

    /* idem */
    for (i = 0; i < sizeof(data->DataChunks->D); i++)
    {
	free(data->DataChunks->D[i]);
    }
    free(data->DataChunks->D);
    free(data->DataChunks);
    free(data);
}

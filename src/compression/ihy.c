#include <stdio.h>
#include <stdlib.h>

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
    fwrite(&data->FileID, 1, 4, file);
    fwrite(&data->FileSize, 8, 1, file);
    fwrite(&data->CompressionType, 2, 1, file);
    fwrite(&data->Channels, 1, 1, file);
    fwrite(&data->Frequency, 4, 1, file);
    /* Write tags */
    fwrite(&data->ArtistLength, 2, 1, file);
    fwrite(data->Artist, 1, sizeof(data->Artist) / 1, file);
    fwrite(&data->AlbumLength, 2, 1, file);
    fwrite(data->Album, 1, sizeof(data->Album) / 1, file);
    fwrite(&data->TrackLength, 2, 1, file);
    fwrite(data->Track, 1, sizeof(data->Track) / 1, file);
    fwrite(&data->Year, 1, 1, file);
    fwrite(&data->Genre, 1, 1, file);
    fwrite(&data->CommentLength, 4, 1, file);
    fwrite(data->Comment, 1, sizeof(data->Comment) / 1, file);
    /* Write data */
    for (i=0; i < (sizeof(data->DataChunks) / sizeof(ihy_chunk)); i++) {
	/* fix me */
    }
    /* Close file */
    fclose(file);
}

void destroy_ihy(const ihy_data *data)
{
    uint64_t i;

    for (i=0; i < sizeof(data->DataChunks->D); i++) {
	free(data->DataChunks->D[i]);
    }
    free(data->DataChunks->D);
    free(data->DataChunks);
    free(data);
}

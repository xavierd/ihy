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

void read_ihy(const char *filename, ihy_data *data)
{
    FILE *file;
    uint32_t i;

    /* Open file for reading */
    file = fopen(filename,"rb");
    if (!file)
	exit(1);
    /* Read header */
    fread(data->FileID, sizeof(char), 4, file);
    fread(&data->FileSize, sizeof(uint64_t), 1, file);
    fread(&data->CompressionType, sizeof(uint16_t), 1, file);
    fread(&data->Channels, sizeof(uint8_t), 1, file);
    fread(&data->Frequency, sizeof(uint32_t), 1, file);
    /* Read tags */
    fread(&data->ArtistLength, sizeof(uint16_t), 1, file);
    fread(data->Artist, sizeof(char), strlen(data->Artist), file);
    fread(&data->AlbumLength, sizeof(uint16_t), 1, file);
    fread(data->Album, sizeof(char), strlen(data->Album), file);
    fread(&data->TrackLength, sizeof(uint16_t), 1, file);
    fread(data->Track, sizeof(char), strlen(data->Track), file);
    fread(&data->Year, sizeof(uint8_t), 1, file);
    fread(&data->Genre, sizeof(uint8_t), 1, file);
    fread(&data->CommentLength, sizeof(uint32_t), 1, file);
    fread(data->Comment, sizeof(char), strlen(data->Comment), file);
    /* Read data */
    fread(&data->NbChunk, sizeof(uint32_t), 1, file);
    for (i = 0; i < data->NbChunk; i++)
    {
	fread(&data->DataChunks[i].ChunkSize, sizeof(uint32_t), 1, file);
	fread(
	    data->DataChunks[i].Values,
	    sizeof(uint8_t),
	    data->DataChunks[i].ChunkSize,
	    file
	);
    }
    fclose(file);
}

void write_ihy(const ihy_data *data, const char *filename)
{
    FILE *file;
    uint32_t i;

    /* Open file for writing */
    file = fopen(filename, "wb");
    if (!file)
	exit(1);
    /* Write header */
    fwrite(data->FileID, sizeof(char), 4, file);
    fwrite(&data->FileSize, sizeof(uint64_t), 1, file);
    fwrite(&data->CompressionType, sizeof(uint16_t), 1, file);
    fwrite(&data->Channels, sizeof(uint8_t), 1, file);
    fwrite(&data->Frequency, sizeof(uint32_t), 1, file);
    /* Write tags */
    fwrite(&data->ArtistLength, sizeof(uint16_t), 1, file);
    fwrite(data->Artist, sizeof(char), strlen(data->Artist), file);
    fwrite(&data->AlbumLength, sizeof(uint16_t), 1, file);
    fwrite(data->Album, sizeof(char), strlen(data->Album), file);
    fwrite(&data->TrackLength, sizeof(uint16_t), 1, file);
    fwrite(data->Track, sizeof(char), strlen(data->Track), file);
    fwrite(&data->Year, sizeof(uint8_t), 1, file);
    fwrite(&data->Genre, sizeof(uint8_t), 1, file);
    fwrite(&data->CommentLength, sizeof(uint32_t), 1, file);
    fwrite(data->Comment, sizeof(char), strlen(data->Comment), file);
    /* Write data */
    fwrite(&data->NbChunk, sizeof(uint32_t), 1, file);
    for (i = 0; i < data->NbChunk; i++)
    {
	fwrite(&data->DataChunks[i].ChunkSize, sizeof(uint32_t), 1, file);
	fwrite(
	    data->DataChunks[i].Values,
	    sizeof(uint8_t),
	    data->DataChunks[i].ChunkSize,
	    file
	);
    }
    /* Close file */
    fclose(file);
}

void destroy_ihy(ihy_data *data)
{
    uint32_t i;

    for (i = 0; i < sizeof(data->NbChunk); i++)
    {
	free(data->DataChunks[i].Values);
    }
    free(data->DataChunks);
    free(data->Artist);
    free(data->Album);
    free(data->Track);
    free(data->Comment);
    free(data);
}

#include "ihy.h"

/* alloc a ihy data and return it */
ihy_data *create_ihy()
{
    ihy_data *result;

    result = malloc(sizeof(ihy_data));
    result->Artist = NULL;
    result->Album = NULL;
    result->Track = NULL;
    result->Comment = NULL;
    result->DataChunks = NULL;

    return result;
}

/* read a ihy and put it on data */
void read_ihy(const char *filename, ihy_data *data)
{
    FILE *file;
    ihy_chunk chunk;
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
    data->Artist = malloc((data->ArtistLength + 1) * sizeof(char));
    fread(data->Artist, sizeof(char), data->ArtistLength, file);
    fread(&data->AlbumLength, sizeof(uint16_t), 1, file);
    data->Artist[data->ArtistLength] = '\0';
    data->Album = malloc((data->AlbumLength + 1) * sizeof(char));
    fread(data->Album, sizeof(char), data->AlbumLength, file);
    data->Album[data->AlbumLength] = '\0';
    fread(&data->TrackLength, sizeof(uint16_t), 1, file);
    data->Track = malloc((data->TrackLength + 1) * sizeof(char));
    fread(data->Track, sizeof(char), data->TrackLength, file);
    data->Track[data->TrackLength] = '\0';
    fread(&data->Year, sizeof(uint16_t), 1, file);
    fread(&data->Genre, sizeof(uint8_t), 1, file);
    fread(&data->CommentLength, sizeof(uint32_t), 1, file);
    data->Comment = malloc((data->CommentLength + 1) * sizeof(char));
    fread(data->Comment, sizeof(char), data->CommentLength, file);
    data->Comment[data->CommentLength] = '\0';
    /* Read data */
    fread(&data->ChunkSize, sizeof(uint32_t), 1, file);
    fread(&data->NbChunk, sizeof(uint32_t), 1, file);
    data->DataChunks = malloc(data->NbChunk * sizeof(ihy_chunk));
    for (i = 0; i < data->NbChunk; i++)
    {
	chunk = data->DataChunks[i];
	fread(&chunk.ChunkSize, sizeof(uint32_t), 1, file);
	chunk.Values = malloc(chunk.ChunkSize);
	fread(&chunk.QBitsPerCoefs, sizeof(uint8_t), 1, file);
	fread(&chunk.QScaleFactor, sizeof(uint16_t), 1, file);
	fread(&chunk.HUncompressedSize, sizeof(uint32_t), 1, file);
	fread(chunk.Values, sizeof(uint8_t), chunk.ChunkSize / sizeof(uint8_t),
									file);
	data->DataChunks[i] = chunk;
    }
    fclose(file);
}

/* write a ihy to a file */
void write_ihy(const ihy_data *data, const char *filename)
{
    FILE *file;
    uint32_t i;
    ihy_chunk chunk;

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
    fwrite(&data->Year, sizeof(uint16_t), 1, file);
    fwrite(&data->Genre, sizeof(uint8_t), 1, file);
    fwrite(&data->CommentLength, sizeof(uint32_t), 1, file);
    fwrite(data->Comment, sizeof(char), strlen(data->Comment), file);
    /* Write data */
    fwrite(&data->ChunkSize, sizeof(uint32_t), 1, file);
    fwrite(&data->NbChunk, sizeof(uint32_t), 1, file);
    for (i = 0; i < data->NbChunk; i++)
    {
	chunk = data->DataChunks[i];
	fwrite(&chunk.ChunkSize, sizeof(uint32_t), 1, file);
	fwrite(&chunk.QBitsPerCoefs, sizeof(uint8_t), 1, file);
	fwrite(&chunk.QScaleFactor, sizeof(uint16_t), 1, file);
	fwrite(&chunk.HUncompressedSize, sizeof(uint32_t), 1, file);
	fwrite(chunk.Values, sizeof(uint8_t), chunk.ChunkSize / sizeof(uint8_t),
									file);
    }
    /* Close file */
    fclose(file);
}

/* release the memory used by a ihy */
void destroy_ihy(ihy_data *data)
{
    uint32_t i;

    for (i = 0; i < data->NbChunk; i++)
	free(data->DataChunks[i].Values);
    free(data->DataChunks);
    free(data->Artist);
    free(data->Album);
    free(data->Track);
    free(data->Comment);
    free(data);
}

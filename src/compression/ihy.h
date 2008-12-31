#ifndef DEF_IHY
#define DEF_IHY

#include <stdint.h>

typedef struct
{
    uint8_t		**D;
    uint8_t		S;
} ihy_chunk;

typedef struct
{
    /* Header */
    char		FileID[4]; /* "SNXT" */
    uint64_t		FileSize;
    uint16_t		CompressionType;
    uint8_t		Channels;
    uint32_t		Frequency;
    /* Tags */
    uint16_t		ArtistLength;
    char		*Artist;
    uint16_t		AlbumLength;
    char		*Album;
    uint16_t		TrackLength;
    char		*Track;
    uint8_t		Year;
    uint8_t		Genre;
    uint32_t		CommentLength;
    char		*Comment;
    /* Data */
    ihy_chunk		*DataChunks;
} ihy_data;

ihy_data *create_ihy();
void write_ihy(const ihy_data *data, const char *filename);
void destroy_ihy(ihy_data *data);

#endif

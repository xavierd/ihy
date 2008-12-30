#ifndef DEF_IHY
#define DEF_IHY

#include <stdint.h>

typedef struct
{
    uint8_t		*Sample;
} ihy_chunk;

typedef struct
{
    /* Header */
    char		ChunkID[4]; /* "SNXT" */
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
    ihy_chunk		*Data;
} ihy_file;

ihy_file *open_ihy(const char *filename);
void close_ihy(ihy_file *file);

#endif

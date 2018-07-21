#ifndef PGN_H
#define PGN_H

#include <glib.h>
#include "game.h"

#define MAX_TAG_NAME_LENGTH 256
#define MAX_TAG_VALUE_LENGTH 256

typedef enum _EndGame
{
    UNKNWON,
    WHITE_WIN,
    BLACK_WIN,
    DRAW
} EndGame;

typedef struct _PGN_Tag
{
    gchar name[MAX_TAG_NAME_LENGTH];
    gchar value[MAX_TAG_VALUE_LENGTH];
} PGN_Tag;

typedef struct _PGN
{
    EndGame result;
    gchar fen[90];
    Game *game;
} PGN;


gboolean   chess_read_pgn    (PGN *pgn, const gchar *filename, GError **error);

#endif
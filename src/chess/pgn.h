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
    gchar event[256];
    gchar site[256];
    gchar date[256];
    gchar round[256];
    gchar white[256];
    gchar black[256];
    EndGame result;
    
    gboolean has_fen;
    gchar fen[90];
    Move *move_list;
} PGN;


gboolean   chess_read_pgn    (PGN *pgn, const gchar *filename, GError **error);
Game      *pgn_to_game       (const PGN *pgn);

void       pgn_free          (PGN *pgn);

#endif
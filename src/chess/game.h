#ifndef GAME_H
#define GAME_H

#include <glib.h>
#include "board.h"
#include "moves.h"

// Games are represented as trees where each node has an arbitrary number of
// children. This is to make variations easy to store. Linked lists are used
// to store the children for ease of inserting new elements.

typedef struct _Game
{
	Move move;
	Board *board;

	struct _Game *parent;
	struct _Game *children;
	struct _Game *sibling;
} Game;

Game		*game_new			(void);
Game		*game_new_prepared	(Move move, Board *board, Game *parent);
Game		*game_new_startpos	(void);
Game        *game_new_fen       (const gchar *fen);

Game 		*add_child			(Game *game, Move move);
Game 		*first_child		(Game *game);
Game 		*root_node			(Game *game);
Game 		*last_node			(Game *game);
gboolean 	 has_children		(Game *game);
void 	 	 free_game			(Game *game);

#endif

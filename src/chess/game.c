#include "board.h"
#include "game.h"

Game*
game_new (void)
{
	Game *game = g_new0 (Game, 1);

	game->move     = NULL_MOVE;
	game->board    = board_new ();
	game->parent   = NULL;
	game->children = NULL;
	game->sibling  = NULL;

	return game;
}


Game* 
game_new_prepared (Move move, Board *board, Game *parent)
{
	Game *game = g_new0 (Game, 1);

	game->move     = move;
	game->board    = board;
	game->parent   = parent;
	game->children = NULL;
	game->sibling  = NULL;

	return game;
}

Game*
game_new_fen (const gchar *fen)
{
	Game *game = game_new ();
	GError *error = NULL;
	gboolean success = board_from_fen (game->board, fen, &error);
	if (!success)
	{
		g_error_free (error);
		free_game (game);
		return NULL;
	}

	game->move     = NULL_MOVE;
	game->parent   = NULL;
	game->children = NULL;
	game->sibling  = NULL;

	return game;
}

Game*
game_new_startpos (void)
{
	Game *game = game_new ();
	GError *error = NULL;
	gboolean success = board_from_fen (game->board, start_board_fen, &error);
	g_assert (success);

	game->move     = NULL_MOVE;
	game->parent   = NULL;
	game->children = NULL;
	game->sibling  = NULL;

	return game;
}

Game*
add_child (Game *game, Move move)
{
	Board *board = board_new ();
	copy_board (board, game->board);
	perform_move (board, move);

	Game *children = game->children;

	while (children != NULL && 
		   children->sibling != NULL &&
		  (children->move != move))
		children = children->sibling;

	if (children != NULL && (children->move == move))
	{
		g_free(board);
		return children;
	} 
	else
	{
		Game *new_node = game_new_prepared (move, board, game);

		if (children == NULL)
			game->children = new_node;
		else
			children->sibling = new_node;

		return new_node;
	}
}

Game*
first_child(Game *game)
{
	return game->children;
}

Game*
root_node (Game *game)
{
	while (game->parent != NULL)
		game = game->parent;

	return game;
}

Game*
last_node (Game *game)
{
	while (game->children != NULL)
		game = game->children;

	return game;
}

gboolean
has_children (Game *game)
{
	return game->children != NULL;
}

void 
free_game (Game *game)
{
	g_free (game->board);

	if (game->sibling != NULL)
		free_game (game->sibling);
	if (game->children != NULL)
		free_game (game->children);

	g_free (game);
}

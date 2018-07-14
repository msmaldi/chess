#include "board.h"
#include "game.h"

Game*
game_new (void)
{
	Game *game = g_new (Game, 1);

	game->move     = NULL_MOVE ();
	game->board    = g_new (Board, 1);
	game->parent   = NULL;
	game->children = NULL;
	game->sibling  = NULL;

	return game;
}


Game* 
game_new_prepared (Move move, Board *board, Game *parent)
{
	Game *game = g_new (Game, 1);

	game->move     = move;
	game->board    = board;
	game->parent   = parent;
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

	game->move     = NULL_MOVE ();
	game->parent   = NULL;
	game->children = NULL;
	game->sibling  = NULL;

	return game;
}

Game*
add_child (Game *game, Move move)
{
	Board *board = g_new (Board, 1);
	copy_board (board, game->board);
	perform_move (board, move);

	Game *children = game->children;

	while (children != NULL && 
		   children->sibling != NULL &&
		  !eq_move (children->move, move))
		children = children->sibling;

	if (children != NULL && eq_move (children->move, move))
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
}

#include "../src/chess/chess.h"
#include <glib.h>

GError *error = NULL;

int
main(int argc, char const *argv[])
{
    g_print ("File:     %ld Bytes\n", sizeof (File));
    g_print ("Rank:     %ld Bytes\n", sizeof (Rank));
    g_print ("Square:   %ld Bytes\n", sizeof (Square));
    g_print ("Piece:    %ld Bytes\n", sizeof (Piece));
    g_print ("Board:    %ld Bytes\n", sizeof (Board));

    PGN *pgn = g_new0 (PGN, 1);
    gboolean success = chess_read_pgn (pgn, "./data/pgn/ding_liren_li_chao_2017.pgn", &error);
    if (!success)
        return 1;
    
    Game *game = game_new_startpos ();

    Move *move_p = pgn->move_list;
    Game *g = game;
    Move m;
    while ((m = *move_p++) != NULL_MOVE)
    {
        print_board (g->board);
        chess_print_move (m);
        g = add_child (g, m);
    }

    return 0;
}

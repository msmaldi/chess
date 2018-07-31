#include "../src/chess/moves.h"
#include "../src/chess/board.h"
#include <glib.h>


int 
main (void)
{
    Square e2 = SQUARE (FILE_E, RANK_2);
    Square e4 = SQUARE (FILE_E, RANK_4);

    Move m = MOVE (e2, e4);
    chess_print_move (m);

    Square expected_e2 = START_SQUARE (m);
    Square expected_e4 = END_SQUARE (m);
    PieceType queen = PROMOTION (m);

    g_assert (e2 == expected_e2);
    g_assert (e4 == expected_e4);

    g_assert (queen == QUEEN);

    m = PROMOTE (m, BISHOP);
    chess_print_move (m);

    PieceType bishop = PROMOTION (m);
    g_assert (bishop == BISHOP);

    m = PROMOTE (m, KNIGHT);
    chess_print_move (m);

    PieceType knight = PROMOTION (m);
    g_assert (knight == KNIGHT);

    m = PROMOTE (m, ROOK);
    chess_print_move (m);
    
    PieceType rook = PROMOTION (m);
    g_assert (rook == ROOK);

    g_print ("Moves OK\n\n\n");

    return 0;
}
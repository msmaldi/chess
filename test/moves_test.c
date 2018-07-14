#include "../src/thirds/chess/moves.h"
#include "../src/thirds/chess/board.h"
#include <glib.h>


int 
main (void)
{
    Square a1 = SQUARE (FILE_A, RANK_1);
    Square b2 = SQUARE (FILE_B, RANK_2);

    Mov m = MOV (a1, b2);

    Square expected_a1 = START_SQUARE(m);
    Square expected_b2 = END_SQUARE(m);


    g_print ("%llu %llu\n", expected_a1, a1);
    g_assert (expected_a1 == a1);
    g_print ("%llu %llu\n", expected_b2, b2);
    g_assert (expected_b2 == b2);


    Mov p = MOV_PROMOTION (m, QUEEN);

    Square pexpected_a1 = START_SQUARE(p);
    Square pexpected_b2 = END_SQUARE(p);
    Piece_type queen = PROMOTION (p);

    g_print ("%llu %llu\n", pexpected_a1, a1);
    g_assert (pexpected_a1 == a1);
    g_print ("%llu %llu\n", pexpected_b2, b2);
    g_assert (pexpected_b2 == b2);
    g_print ("%llu %llu\n", queen, QUEEN);
    g_assert (queen == QUEEN);


    return 0;
}
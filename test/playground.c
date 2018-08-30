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

    return 0;
}

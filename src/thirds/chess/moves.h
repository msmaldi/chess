#ifndef MOVES_H
#define MOVES_H

#define FILE_CHAR(file) ('a' + (file))
#define CHAR_FILE(c)    ((c) - 'a')
#define RANK_CHAR(rank) ('1' + (rank))
#define CHAR_RANK(c)    ((c) - '1')
//
//

typedef struct _Move
{
    Square start;
    Square end;
    Piece_type promotion;
} Move;

Move MOVE (Square start, Square end);
Move MOVE_WITH_PROMOTION (Square start, Square end, Piece_type promotion);
Move NULL_MOVE ();

gboolean    eq_move (Move x, Move y);


void        perform_move            (Board *board, Move move);
gboolean    legal_move              (Board *board, Move move, 
                                     gboolean check_for_check);
gboolean    gives_check             (Board *board, Move move, Player player);

// Longest possible length of a move in algebraic notation.
// e.g. Raxd1+\0
#define MAX_ALGEBRAIC_NOTATION_LENGTH 7
void        algebraic_notation_for   (Board *board, Move move, char *str);

#endif
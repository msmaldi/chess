#ifndef MOVES_H
#define MOVES_H

//
// A move is represented as 4 bytes, with the start square in the two most
// significant bytes, and the end square in the two least significant
// bytes. Each pair of bytes has the file in the most significant byte, and
// the rank in the least significant byte.
//typedef uint_fast64_t Move;
//#define MOVE(start, end) ((Move)(((start) << 16) | (end)))
//#define START_SQUARE(m)  ((m) >> 16)
//#define END_SQUARE(m)    ((m) & 0xFFFF)

//#define NULL_MOVE ((Move)(~((Move)0)))

#define FILE_CHAR(file) ('a' + (file))
#define CHAR_FILE(c)    ((c) - 'a')
#define RANK_CHAR(rank) ('1' + (rank))
#define CHAR_RANK(c)    ((c) - '1')

//
//

typedef struct _MMove
{
    Square start;
    Square end;
    Piece_type promotion;
} Move;

Move MOVE (Square start, Square end);
Move MOVE_WITH_PROMOTION (Square start, Square end, Piece_type promotion);

Square START_SQUARE (Move m);

Square END_SQUARE (Move m);
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
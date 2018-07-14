#ifndef MOVES_H
#define MOVES_H

#include "board.h"

//
// A move is represented as 4 bytes, with the start square in the two most
// significant bytes, and the end square in the two least significant
// bytes. Each pair of bytes has the file in the most significant byte, and
// the rank in the least significant byte.
//
// A move is represented as 8 bytes, 

//  |    2 bytes    |    2 bytes    |    2 bytes    |    2 bytes    |                            
//  63------------48----------------32--------------16-------------0
//  |     empty     |   Piece Type  |  Square Start |  Square end   |                                               
//
//
//




typedef u_int64_t Mov;

#define MOV(start, end)     ((Mov) (((start) << 16) | (end)))
#define MOVE_PROMOTE(m, piece) ((Mov)(((u_int64_t)(piece) << 32) | (m)))
#define START_SQUARE(m)     ((Square)(((m) & 0xFFFFFFFF) >> 16))
#define END_SQUARE(m)    (((m)) & 0xFFFF)
#define PROMOTION(m)     ((Piece_type) (((m) & 0xFFFFFFFFFFFFul) >> 32))
#define NULL_MOV ((Mov)(~((Mov)0)))


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
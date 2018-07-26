#ifndef MOVES_H
#define MOVES_H

#include "board.h"

// A move is represented as 8 bytes, represented by:
//
//  |    2 bytes    |    2 bytes    |    2 bytes    |    2 bytes    |                            
//  63--------------48--------------32--------------16--------------0
//  |     empty     |   Piece Type  |  Square Start |  Square end   |                                               
//
// Default PieceType to promote is QUEEN, if you want change, use
// PROMOTE to change piece.
typedef uint_fast64_t Move;

#define MOVE(start, end)     ((Move)(((u_int64_t)QUEEN << 32) | (start << 16) | (end)))
#define PROMOTE(move, piece) ((Move)(((u_int64_t)piece << 32) | (move & 0xFFFFFFFF)))
#define START_SQUARE(move)   ((Square)(((move) & 0xFFFFFFFF) >> 16))
#define END_SQUARE(move)     ((Square)((move) & 0xFFFF))
#define PROMOTION(move)      ((PieceType) (((move) >> 32)))
#define NULL_MOVE            ((Move)(~((Move)0)))

#define FILE_CHAR(file) ('a' + (file))
#define CHAR_FILE(c)    ((c) - 'a')
#define RANK_CHAR(rank) ('1' + (rank))
#define CHAR_RANK(c)    ((c) - '1')

void        chess_print_move        (Move m);
void        perform_move            (Board *board, Move move);
gboolean    legal_move              (Board *board, Move move, 
                                     gboolean check_for_check);
gboolean    gives_check             (Board *board, Move move, Player player);
gboolean    gives_mate              (Board *board, Move move, Player player);

// Longest possible length of a move in algebraic notation.
// From FEN Q1bq1bnr/2pkpppp/8/Q2p3Q/8/4P3/2PP1PPP/RNB1KBNR w KQ - 6 15
// Here White have 3 Queens, a8, a5 and h5, and black pawn in d5 can be
// captured by 3 Queens, moves: Q8xd5+ Qhxd5+ Qa5xd5+ 
// This move needs is ambiguous with file and rank and give check in
// Black King, the max length Qa5xd5+ and a '\0' and end.
#define MAX_ALGEBRAIC_NOTATION_LENGTH 8
void        algebraic_notation_for  (Board *board, Move move, gchar *str);

#endif
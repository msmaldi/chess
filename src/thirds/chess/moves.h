#ifndef MOVES_H
#define MOVES_H

#include "board.h"

//
// A move is represented as 4 bytes, with the start square in the two most
// significant bytes, and the end square in the two least significant
// bytes. Each pair of bytes has the file in the most significant byte, and
// the rank in the least significant byte.
//
// A move is represented as 8 bytes, represented by:
// 
//  |    2 bytes    |    2 bytes    |    2 bytes    |    2 bytes    |                            
//  63------------48----------------32--------------16-------------0
//  |     empty     |   Piece Type  |  Square Start |  Square end   |                                               
//
// Default PieceType to promote is QUEEN, if you want change, use
// MOVE_PROMOTE to change piece.
typedef u_int64_t Move;
//#define MOVE(start, end)       ((Move) (((start) << 16) | (end) ))
//#define MOVE_PROMOTE(m, piece) ((Move)(((u_int64_t)(piece) << 32) | (m)))
#define MOVE(start, end)       ((Move)(((u_int64_t)QUEEN << 32) | (start << 16) | (end)))
#define MOVE_PROMOTE(m, piece) ((Move)(((u_int64_t)piece << 32) | (m & 0xFFFFFFFF)))
#define START_SQUARE(m)        ((Square)(((m) & 0xFFFFFFFF) >> 16))
#define END_SQUARE(m)          ((Square)((m) & 0xFFFF))
#define PROMOTION(m)           ((PieceType) (((m) & 0xFFFFFFFFFFFFul) >> 32))
#define NULL_MOVE              ((Move)(~((Move)0)))

#define FILE_CHAR(file) ('a' + (file))
#define CHAR_FILE(c)    ((c) - 'a')
#define RANK_CHAR(rank) ('1' + (rank))
#define CHAR_RANK(c)    ((c) - '1')
//
//

//typedef struct _Move
//{
//    Square start;
//    Square end;
//    Piece_type promotion;
//} Move;

//Move MOVE (Square start, Square end);
//Move MOVE_WITH_PROMOTION (Square start, Square end, Piece_type promotion);
//Move NULL_MOVE ();

//gboolean    eq_move (Move x, Move y);

void        chess_print_move        (Move m);
void        perform_move            (Board *board, Move move);
gboolean    legal_move              (Board *board, Move move, 
                                     gboolean check_for_check);
gboolean    gives_check             (Board *board, Move move, Player player);

// Longest possible length of a move in algebraic notation.
// From FEN Q1bq1bnr/2pkpppp/8/Q2p3Q/8/4P3/2PP1PPP/RNB1KBNR w KQ - 6 15
// Here White have 3 Queens, a8, a5 and h5, and black pawn in d5 can be
// captured by 3 Queens, moves: Q8xd5+ Qhxd5+ Qa5xd5+ 
// This move needs is ambiguous with file and rank and give check in
// Black King, the max length Qa5xd5+ and a '\0' and end.
// e.g. 
#define MAX_ALGEBRAIC_NOTATION_LENGTH 8
void        algebraic_notation_for   (Board *board, Move move, char *str);

#endif
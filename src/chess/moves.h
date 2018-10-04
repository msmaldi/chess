#ifndef MOVES_H
#define MOVES_H

#include "board.h"

// A move is represented as 8 bytes, represented by:
//
//  |    1 bytes    |    1 bytes    |    1 bytes    |    1 bytes    |                            
//  32--------------24--------------16--------------8---------------0
//  |     empty     |   Piece Type  |  Square Start |  Square end   |                                               
//
// Default PieceType to promote is QUEEN, if you want change, use
// PROMOTE to change piece.
typedef int_fast32_t Move;

#define MOVE(start, end)     ((Move)((start << 8) | (end)))
#define PROMOTE(move, piece) ((Move)((piece << 16) | (move & 0xFFFF)))
#define START_SQUARE(move)   ((Square)(((move) & 0xFFFF) >> 8))
#define END_SQUARE(move)     ((Square)((move) & 0xFF))
#define PROMOTION(move)      ((PieceType) (((move) >> 16)))
#define NULL_MOVE            ((Move)(~((Move)0)))

#define FILE_CHAR(file) ('a' + (file))
#define CHAR_FILE(c)    ((c) - 'a')
#define RANK_CHAR(rank) ('1' + (rank))
#define CHAR_RANK(c)    ((c) - '1')

void        chess_print_move        (Move m);
void        perform_move            (Board *board, Move move);
gboolean    legal_move              (Board *board, Move move, 
                                     gboolean check_for_check); // pseudo_legal_move
gboolean    pseudo_legal_move       (Board *board, Move move);

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

// const Square a1 = SQUARE (FILE_A, RANK_1);
// const Square a2 = SQUARE (FILE_A, RANK_2);
// const Square a3 = SQUARE (FILE_A, RANK_3);
// const Square a4 = SQUARE (FILE_A, RANK_4);
// const Square a5 = SQUARE (FILE_A, RANK_5);
// const Square a6 = SQUARE (FILE_A, RANK_6);
// const Square a7 = SQUARE (FILE_A, RANK_7);
// const Square a8 = SQUARE (FILE_A, RANK_8);

// const Square b1 = SQUARE (FILE_B, RANK_1);
// const Square b2 = SQUARE (FILE_B, RANK_2);
// const Square b3 = SQUARE (FILE_B, RANK_3);
// const Square b4 = SQUARE (FILE_B, RANK_4);
// const Square b5 = SQUARE (FILE_B, RANK_5);
// const Square b6 = SQUARE (FILE_B, RANK_6);
// const Square b7 = SQUARE (FILE_B, RANK_7);
// const Square b8 = SQUARE (FILE_B, RANK_8);

// const Square c1 = SQUARE (FILE_C, RANK_1);
// const Square c2 = SQUARE (FILE_C, RANK_2);
// const Square c3 = SQUARE (FILE_C, RANK_3);
// const Square c4 = SQUARE (FILE_C, RANK_4);
// const Square c5 = SQUARE (FILE_C, RANK_5);
// const Square c6 = SQUARE (FILE_C, RANK_6);
// const Square c7 = SQUARE (FILE_C, RANK_7);
// const Square c8 = SQUARE (FILE_C, RANK_8);

// const Square d1 = SQUARE (FILE_D, RANK_1);
// const Square d2 = SQUARE (FILE_D, RANK_2);
// const Square d3 = SQUARE (FILE_D, RANK_3);
// const Square d4 = SQUARE (FILE_D, RANK_4);
// const Square d5 = SQUARE (FILE_D, RANK_5);
// const Square d6 = SQUARE (FILE_D, RANK_6);
// const Square d7 = SQUARE (FILE_D, RANK_7);
// const Square d8 = SQUARE (FILE_D, RANK_8);

// const Square e1 = SQUARE (FILE_E, RANK_1);
// const Square e2 = SQUARE (FILE_E, RANK_2);
// const Square e3 = SQUARE (FILE_E, RANK_3);
// const Square e4 = SQUARE (FILE_E, RANK_4);
// const Square e5 = SQUARE (FILE_E, RANK_5);
// const Square e6 = SQUARE (FILE_E, RANK_6);
// const Square e7 = SQUARE (FILE_E, RANK_7);
// const Square e8 = SQUARE (FILE_E, RANK_8);

// const Square f1 = SQUARE (FILE_F, RANK_1);
// const Square f2 = SQUARE (FILE_F, RANK_2);
// const Square f3 = SQUARE (FILE_F, RANK_3);
// const Square f4 = SQUARE (FILE_F, RANK_4);
// const Square f5 = SQUARE (FILE_F, RANK_5);
// const Square f6 = SQUARE (FILE_F, RANK_6);
// const Square f7 = SQUARE (FILE_F, RANK_7);
// const Square f8 = SQUARE (FILE_F, RANK_8);

// const Square g1 = SQUARE (FILE_G, RANK_1);
// const Square g2 = SQUARE (FILE_G, RANK_2);
// const Square g3 = SQUARE (FILE_G, RANK_3);
// const Square g4 = SQUARE (FILE_G, RANK_4);
// const Square g5 = SQUARE (FILE_G, RANK_5);
// const Square g6 = SQUARE (FILE_G, RANK_6);
// const Square g7 = SQUARE (FILE_G, RANK_7);
// const Square g8 = SQUARE (FILE_G, RANK_8);

// const Square h1 = SQUARE (FILE_H, RANK_1);
// const Square h2 = SQUARE (FILE_H, RANK_2);
// const Square h3 = SQUARE (FILE_H, RANK_3);
// const Square h4 = SQUARE (FILE_H, RANK_4);
// const Square h5 = SQUARE (FILE_H, RANK_5);
// const Square h6 = SQUARE (FILE_H, RANK_6);
// const Square h7 = SQUARE (FILE_H, RANK_7);
// const Square h8 = SQUARE (FILE_H, RANK_8);

#endif
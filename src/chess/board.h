#ifndef BOARD_H
#define BOARD_H

#include "chessconfig.h"
#include <stdint.h>
#include <stdbool.h>
#include <glib.h>

#define BOARD_SIZE 8
#define PLAYERS 2

typedef int_fast8_t File;
typedef int_fast8_t Rank;

#define RANK_1 0
#define RANK_2 1
#define RANK_3 2
#define RANK_4 3
#define RANK_5 4
#define RANK_6 5
#define RANK_7 6
#define RANK_8 7

#define RANK_WHITE_PAWN RANK_2
#define RANK_BLACK_PAWN RANK_7
#define RANK_WHITE_PROMOTION RANK_8
#define RANK_BLACK_PROMOTION RANK_1
#define RANK_BLACK_CASTLE RANK_8
#define RANK_WHITE_CASTLE RANK_1

#define FILE_CASTLE_KINGSIDE FILE_G
#define FILE_CASTLE_QUEENSIDE FILE_C

#define FILE_A 0
#define FILE_B 1
#define FILE_C 2
#define FILE_D 3
#define FILE_E 4
#define FILE_F 5
#define FILE_G 6
#define FILE_H 7

// Files and Ranks can be stored in 3 bit, and square can use 8 bits
// to store a file and rank, but in my tests use 16 bits for store
// gain a little performance.
//
// |      1 byte      |      1 byte      |
// 16-----------------8------------------0
// |       File       |       Rank       |
//
typedef int_fast8_t Square;
#define SQUARE(file, rank)  ((Square)(((rank) << 3) | (file)))
#define SQUARE_X(square)    ((File)((square) & 0x7))
#define SQUARE_Y(square)    ((Rank)((square) >> 3))
#define SQUARE_FILE(square) ((File)((square) & 0x7))
#define SQUARE_RANK(square) ((Rank)((square) >> 3))

#define NULL_SQUARE ((Square)(~((Square)0)))

enum Squares {
  SQ_A1, SQ_B1, SQ_C1, SQ_D1, SQ_E1, SQ_F1, SQ_G1, SQ_H1,
  SQ_A2, SQ_B2, SQ_C2, SQ_D2, SQ_E2, SQ_F2, SQ_G2, SQ_H2,
  SQ_A3, SQ_B3, SQ_C3, SQ_D3, SQ_E3, SQ_F3, SQ_G3, SQ_H3,
  SQ_A4, SQ_B4, SQ_C4, SQ_D4, SQ_E4, SQ_F4, SQ_G4, SQ_H4,
  SQ_A5, SQ_B5, SQ_C5, SQ_D5, SQ_E5, SQ_F5, SQ_G5, SQ_H5,
  SQ_A6, SQ_B6, SQ_C6, SQ_D6, SQ_E6, SQ_F6, SQ_G6, SQ_H6,
  SQ_A7, SQ_B7, SQ_C7, SQ_D7, SQ_E7, SQ_F7, SQ_G7, SQ_H7,
  SQ_A8, SQ_B8, SQ_C8, SQ_D8, SQ_E8, SQ_F8, SQ_G8, SQ_H8,
  SQ_NONE,

  SQUARE_NB = 64
};

// Pieces are represented as 8 bits, with the MSB used to store the color, and
// the rest equal to one of a bunch of constants for the type of piece.
typedef uint_fast8_t Piece;

enum Players
{
	BLACK = 0,
	WHITE = 1,
};

typedef uint8_t Player;

#define OTHER_PLAYER(p) ((p) == WHITE ? BLACK : WHITE)

// The order of these matters
typedef enum _PieceType
{
	EMPTY,
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING,
} PieceType;

#define PIECE_TYPES 6

#define PLAYER(x) ((Player)(x >> 7))
#define PIECE_TYPE(x) ((PieceType)((x) & ~(1 << 7)))
#define PIECE(p, t) ((Piece)(((p << 7) | (t))))
#define NULL_PIECE ((uint_fast8_t)-1)
#define IS_PROMOTABLE(p_type) ((p_type >= KNIGHT) && (p_type <= QUEEN))

#define BLACK_PAWN   PIECE(BLACK, PAWN)
#define BLACK_KNIGHT PIECE(BLACK, KNIGHT)
#define BLACK_BISHOP PIECE(BLACK, BISHOP)
#define BLACK_ROOK   PIECE(BLACK, ROOK)
#define BLACK_QUEEN  PIECE(BLACK, QUEEN)
#define BLACK_KING   PIECE(BLACK, KING)

#define WHITE_PAWN   PIECE(WHITE, PAWN)
#define WHITE_KNIGHT PIECE(WHITE, KNIGHT)
#define WHITE_BISHOP PIECE(WHITE, BISHOP)
#define WHITE_ROOK   PIECE(WHITE, ROOK)
#define WHITE_QUEEN  PIECE(WHITE, QUEEN)
#define WHITE_KING   PIECE(WHITE, KING)


typedef struct Castling
{
	bool kingside;
	bool queenside;
} Castling;


// The board is an array of pieces, plus some other information:
// * Whose turn it is
// * Castling availibility
// * En passant target square (if any)
// * Halfmoves since the last capture or pawn advance
// * Move number
//
// This is similar to FEN.
// The intention behind having all this information in the board object is so
// that no other information is required to determine any necessary information
// about a board position.
//
// However, this structure does not contain information necessary to determine
// if a draw can be claimed by threefold repetition. This is unlikely to
// matter, as the most common case of threefold repetition is perpetual check,
// in which case coming in part-way through does not matter. The other cases
// are rare enough that it doesn't seem worthwhile to overly complicate the
// board representation over them.
typedef struct Board
{
	Player turn;
	Castling castling[PLAYERS];
	Square en_passant;
	uint8_t half_move_clock;
	uint16_t move_number;
	Piece pieces[BOARD_SIZE * BOARD_SIZE];
	Square king[PLAYERS];
} Board;

#define PIECE_AT(board, file, rank) ((board)->pieces[(SQUARE (file, rank))])
#define PIECE_AT_SQUARE(board, square) ((board)->pieces[(square)])

Board	   *board_new				(void);
void		board_free				(Board *board);

void 		copy_board				(Board *dst, Board *src);
Piece 		piece_from_char			(gchar c);
gchar 		char_from_piece			(Piece p);
gboolean 	board_from_fen			(Board *board, 
									 const gchar *fen_str, 
									 GError **error);
gint 		to_fen 					(Board *board, gchar *fen);
void 		print_board				(Board *board);
gboolean 	in_check				(Board *board, Player p);
gboolean 	checkmate				(Board *board, Player p);
gboolean    stalemate               (Board *board, Player p);
gboolean 	can_castle_kingside		(Board *board, Player p);
gboolean 	can_castle_queenside	(Board *board, Player p);

Square		find_king				(Board *board, Player p);
gboolean    under_attack			(Board *board, 
                                     Square square, 
                                     Player attacker);

extern const gchar *start_board_fen;

#endif

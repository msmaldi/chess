#ifndef BOARD_H_
#define BOARD_H_

#include <stdbool.h>
#include <stdint.h>
#include <glib.h>
#include "misc.h"

#define BOARD_SIZE 8
#define PLAYERS 2

typedef uint_fast8_t File;
typedef uint_fast8_t Rank;

#define RANK_8 7
#define RANK_7 6
#define RANK_6 5
#define RANK_5 4
#define RANK_4 3
#define RANK_3 2
#define RANK_2 1
#define RANK_1 0

#define FILE_A 0
#define FILE_B 1
#define FILE_C 2
#define FILE_D 3
#define FILE_E 4
#define FILE_F 5
#define FILE_G 6
#define FILE_H 7


typedef uint_fast16_t Square;
#define SQUARE(x, y) (((x) << 8) | (y))
#define SQUARE_X(s) ((s) >> 8)
#define SQUARE_Y(s) ((s) & 0xF)
#define SQUARE_FILE(s) ((s) >> 8)
#define SQUARE_RANK(s) ((s) & 0xF)

#define NULL_SQUARE ((Square)(~((Square)0)))

// Pieces are represented as shorts, with the MSB used to store the color, and
// the rest equal to one of a bunch of constants for the type of piece.
typedef unsigned short Piece;

typedef enum Player
{
	BLACK = 0,
	WHITE = 1,
} Player;

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

#define PLAYER(x) ((Player)((x) >> (sizeof(Piece) * 8 - 1)))
#define PIECE_TYPE(x) ((PieceType)((x) & ~(1 << (sizeof(Piece) * 8 - 1))))
#define PIECE(p, t) ((Piece)(((p) << (sizeof(Piece) * 8 - 1)) | (t)))
#define NULL_PIECE ((unsigned short)-1)
#define IS_PROMOTABLE(p_type) ((p_type >= KNIGHT) && (p_type <= QUEEN))

#define BLACK_PAWN PIECE(BLACK, PAWN)
#define BLACK_KNIGHT PIECE(BLACK, KNIGHT)
#define BLACK_BISHOP PIECE(BLACK, BISHOP)
#define BLACK_ROOK PIECE(BLACK, ROOK)
#define BLACK_QUEEN PIECE(BLACK, QUEEN)
#define BLACK_KING PIECE(BLACK, KING)

#define WHITE_PAWN PIECE(WHITE, PAWN)
#define WHITE_KNIGHT PIECE(WHITE, KNIGHT)
#define WHITE_BISHOP PIECE(WHITE, BISHOP)
#define WHITE_ROOK PIECE(WHITE, ROOK)
#define WHITE_QUEEN PIECE(WHITE, QUEEN)
#define WHITE_KING PIECE(WHITE, KING)


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
	uint half_move_clock;
	uint move_number;

	Piece pieces[BOARD_SIZE * BOARD_SIZE];
} Board;

#define PIECE_AT(b, x, y) ((b)->pieces[((y) * BOARD_SIZE) + (x)])
#define PIECE_AT_SQUARE(b, square) PIECE_AT(b, SQUARE_X(square), SQUARE_Y(square))

void 		copy_board				(Board *dst, Board *src);
Piece 		piece_from_char			(char c);
char 		char_from_piece			(Piece p);
gboolean 	board_from_fen			(Board *board, 
									 const gchar *fen_str, 
									 GError **error);
gint 		to_fen 					(Board *board, gchar *fen);
void 		print_board				(Board *b);
gboolean 	in_check				(Board *board, Player p);
gboolean 	checkmate				(Board *board, Player p);
gboolean 	can_castle_kingside		(Board *board, Player p);
gboolean 	can_castle_queenside	(Board *board, Player p);

extern char *start_board_fen;

#endif // include guard

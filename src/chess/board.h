#ifndef BOARD_H
#define BOARD_H

#include "chessconfig.h"
#include <stdint.h>
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
#define RANK_BLACK_PROMOTION RANK_0

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
typedef uint_fast16_t Square;
#define SQUARE(file, rank)  ((Square)(((file) << 8) | (rank)))
#define SQUARE_X(square)    ((File)((square) >> 8))
#define SQUARE_Y(square)    ((Rank)((square) & 0xFF))
#define SQUARE_FILE(square) ((File)((square) >> 8))
#define SQUARE_RANK(square) ((Rank)((square) & 0xFF))

#define NULL_SQUARE ((Square)(~((Square)0)))


// Pieces are represented as 8 bits, with the MSB used to store the color, and
// the rest equal to one of a bunch of constants for the type of piece.
typedef uint_fast8_t Piece;

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
	gboolean kingside;
	gboolean queenside;
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
	guint half_move_clock;
	guint move_number;

	Piece pieces[BOARD_SIZE * BOARD_SIZE];
} Board;

#define PIECE_AT(board, file, rank) ((board)->pieces[((rank) * BOARD_SIZE) + (file)])
#define PIECE_AT_SQUARE(board, square) PIECE_AT(board, SQUARE_X (square), SQUARE_Y (square))

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
gboolean 	can_castle_kingside		(Board *board, Player p);
gboolean 	can_castle_queenside	(Board *board, Player p);

Square		find_king				(Board *board, Player p);
gboolean    under_attack			(Board *board, 
                                     Square square, 
                                     Player attacker);

extern const gchar *start_board_fen;

#endif

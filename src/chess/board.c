#include "chessconfig.h"
#include "board.h"
#include "moves.h"
#include <string.h>

Board*
board_new (void)
{
	return g_new0 (Board, 1);
}

void
board_free (Board *board)
{
	g_free (board);
}

inline void
copy_board (Board *dst, Board *src)
{
	// dst->turn = src->turn;
	// dst->castling[BLACK] = src->castling[BLACK];
	// dst->castling[WHITE] = src->castling[WHITE];
	// dst->en_passant = src->en_passant;
	// dst->half_move_clock = src->half_move_clock;
	// dst->move_number = src->move_number;
	
	// for(Square square = SQ_A1; square <= SQ_H8; square++)
	// 	PIECE_AT_SQUARE(dst, square) = PIECE_AT_SQUARE(src, square);

	// dst->king[BLACK] = src->king[BLACK];
	// dst->king[WHITE] = src->king[WHITE];	
	memcpy (dst, src, sizeof (Board));
}

// Converts a piece into a char, using the standard used in PGN and FEN
gchar
char_from_piece (Piece p)
{
	switch (PIECE_TYPE (p))
	{
		case PAWN:   return 'P';
		case KNIGHT: return 'N';
		case BISHOP: return 'B';
		case ROOK:   return 'R';
		case QUEEN:  return 'Q';
		case KING:   return 'K';
		default:     return ' ';
	}
}

const gchar *start_board_fen =
	"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

gint
to_fen (Board *board, gchar *fen)
{
	gchar *active = (board->turn == BLACK ? "b" : "w");

	gchar castling[5] = { '-', '\0', '\0', '\0', '\0' };
	guint c = 0;
	if (board->castling[WHITE].kingside)
		castling[c++] = 'K';
	if (board->castling[WHITE].queenside)
		castling[c++] = 'Q';
	if (board->castling[BLACK].kingside	)
		castling[c++] = 'k';
	if (board->castling[BLACK].queenside)
		castling[c++] = 'q';

	gchar en_passant[3] = { '-', '\0', '\0' };
	gchar x = SQUARE_X(board->en_passant) + 'a';
	gchar y = SQUARE_Y(board->en_passant) + '1';
	if (y == '3' || y == '6')
	{
		en_passant[0] = x;
		en_passant[1] = y;
	}

	gchar board_fen[64 + 8 + 1];
	int board_fen_index = 0;
	for (Rank rank = RANK_8; rank >= RANK_1; rank--)
	//for (int i = 56; i >= 0; i -= 8)
	{
		int no_piece = 0;
		for (File file = FILE_A; file <= FILE_H; file++)
		//for (int aux = 0; aux < 8; aux++)
		{	
			//int index = i + aux;
			Piece piece = PIECE_AT (board, file, rank);
			//Piece piece = board->pieces[index];
			gchar ca = char_from_piece (piece);
			gchar ch = PLAYER (piece) == BLACK ? g_ascii_tolower (ca) : ca;

			if (ch != ' ')
			{
				if (no_piece != 0)
				{
					board_fen[board_fen_index++] = no_piece + '0';
					no_piece = 0;
				}
				board_fen[board_fen_index++] = ch;
			}
			else
			{
				no_piece++;
			}
		}
		if (no_piece != 0)
		{
			board_fen[board_fen_index++] = no_piece + '0';
			no_piece = 0;
		}
		board_fen[board_fen_index++] = '/';
	}
	board_fen[--board_fen_index] = '\0';

	guint half_move_clock = board->half_move_clock;
	guint move_number = board->move_number;

	return g_snprintf (fen, 100, "%s %s %s %s %d %d", 
		board_fen, active, castling, en_passant, half_move_clock, move_number);
}

// Initializes a Board given a string containing Forsyth-Edwards Notation (FEN)
// See <http://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation> for a
// description, and <http://kirill-kryukov.com/chess/doc/fen.html> for the spec
/*
---------1---------2---------3---------4---------5---------6---------7---------8---------9---------0
rnbqkbnr/pppppppp/pppppppp/pppppppp/PPPPPPPP/PPPPPPPP/PPPPPPPP/RNBQKBNR w KQkq e3 40 1100

Fen string can have maximous 89 caracters + 1 for NULL, gchar fen[90] is ideial
*/

static gboolean 
is_piece (gchar c)
{
	return c == 'r' || c == 'n' || c == 'b' || c == 'q' || c == 'k' || c == 'p'
		|| c == 'R' || c == 'N' || c == 'B' || c == 'Q' || c == 'K' || c == 'P';
}

static gboolean
is_number_fen_rank (gchar c)
{
	return ('1' <= c && c <= '8');
}

Piece
piece_from_char (gchar c)
{
	switch (c)
	{
		case 'P': return WHITE_PAWN;
		case 'N': return WHITE_KNIGHT;
		case 'B': return WHITE_BISHOP;
		case 'R': return WHITE_ROOK;
		case 'Q': return WHITE_QUEEN;
		case 'K': return WHITE_KING;
		case 'p': return BLACK_PAWN;
		case 'n': return BLACK_KNIGHT;
		case 'b': return BLACK_BISHOP;
		case 'r': return BLACK_ROOK;
		case 'q': return BLACK_QUEEN;
		case 'k': return BLACK_KING;
		default: return EMPTY;
	}
}

gboolean 
board_from_fen (Board *board, const gchar *fen_str, GError **error)
{
	glong fen_len = g_utf8_strlen (fen_str, 90);
	if (fen_len >= 90)
	{
		g_set_error (error, 1, 1, "Fen length %ld is too long.", fen_len);
		return FALSE;
	}

	gint backslash_count = 0;
	for (gint i = 0; i < fen_len; i++)
	{
		if (fen_str[i] == '/')
			backslash_count++;
	}
	if (backslash_count != 7)
	{
		g_set_error (error, 1, 1, 
					 "Fen must have 7 backslashs (/) found %d.", 
					 backslash_count);
		return FALSE;
	}

	//gchar* fen_ptr = fen_str;
	gint index = 0;
	gint w_king_count = 0;
	gint b_king_count = 0;
	for (Rank rank = RANK_8; rank >= RANK_1; rank--)
	{
		File file = FILE_A;
		while (fen_str[index] != '/' && fen_str[index] != ' ')
		{
			if (is_number_fen_rank (fen_str[index]))
			{
				gint empty_squares = fen_str[index] - '0';
				if (file + empty_squares > 8)
				{
					g_set_error (error, 1, 1, 
						"Rank %d length > 8", rank + 1);
					return FALSE;
				}				
				for(gint i = 0; i < empty_squares; i++)
				{		
					PIECE_AT(board, file + i, rank) = EMPTY;
				}
				file += empty_squares;
			}
			else if (is_piece (fen_str[index]))
			{
				if (file >= BOARD_SIZE)
				{
					g_set_error (error, 1, 1, 
						"Rank %d length > 8", rank + 1);
					return FALSE;
				}		
				Piece piece = piece_from_char (fen_str[index]);
				if (piece == BLACK_KING)
				{
					b_king_count++;
					board->king[BLACK] = SQUARE (file, rank);
				}
				else if (piece == WHITE_KING)
				{
					w_king_count++;
					board->king[WHITE] = SQUARE (file, rank);
				}

				PIECE_AT (board, file, rank) = piece;
				file++;
			}
			else
			{
				g_set_error (error, 1, 1, 
					 "Found '%c', character must be [rnbqkpRNBQKP] or [1-8].", 
					 fen_str[index]);
				return FALSE;
			}
			index++;
		}
		index++;
	}
	if (b_king_count != 1)
	{
		g_set_error (error, 1, 1, 
			"Found %d Black King, must be 1.", 
			 b_king_count);
		return FALSE;
	}
	if (w_king_count != 1)
	{
		g_set_error (error, 1, 1, 
			"Found %d White King, must be 1.", 
			 w_king_count);
		return FALSE;
	}
	gchar active_player = fen_str[index++];
	if (active_player == 'w')
	{
		board->turn = WHITE;
	}
	else if (active_player == 'b')
	{
		board->turn = BLACK;
	}
	else
	{
		g_set_error (error, 1, 1, 
			"Active player found '%c', must be 'w' or 'b'.", 
			 active_player);
		return FALSE;
	}
	gchar space;
	if ((space = fen_str[index++]) != ' ')
	{
		g_set_error (error, 1, 1, 
			"Found '%c', expected a whitespace (' ') after active player.", 
			 space);
		return FALSE;
	}

	Castling b_castling = { FALSE, FALSE };
	Castling w_castling = { FALSE, FALSE };	
	gchar casling_str[5] = { '-', '\0', '\0', '\0', '\0' };
	for(gint i = 0; i < 4; i++)
	{
		if (fen_str[index] != ' ')
			casling_str[i] = fen_str[index++];
	}
	//g_print ("%s'%c'\n", casling_str, fen_str[index]);

#if FEN_CASTLING_ORDERED == TRUE
	if (g_strcmp0 (casling_str, "KQkq") == 0)
	{
		w_castling.kingside = TRUE;
		w_castling.queenside = TRUE;
		b_castling.kingside = TRUE;
		b_castling.queenside = TRUE;
	}
	else if (g_strcmp0 (casling_str, "KQk") == 0)
	{
		w_castling.kingside = TRUE;
		w_castling.queenside = TRUE;
		b_castling.kingside = TRUE;
		b_castling.queenside = FALSE;
	}
	else if (g_strcmp0 (casling_str, "KQq") == 0)
	{
		w_castling.kingside = TRUE;
		w_castling.queenside = TRUE;
		b_castling.kingside = FALSE;
		b_castling.queenside = TRUE;
	}
	else if (g_strcmp0 (casling_str, "Kkq") == 0)
	{
		w_castling.kingside = TRUE;
		w_castling.queenside = FALSE;
		b_castling.kingside = TRUE;
		b_castling.queenside = TRUE;
	}
	else if (g_strcmp0 (casling_str, "Qkq") == 0)
	{
		w_castling.kingside = FALSE;
		w_castling.queenside = TRUE;
		b_castling.kingside = TRUE;
		b_castling.queenside = TRUE;
	}
	else if (g_strcmp0 (casling_str, "KQ") == 0)
	{
		w_castling.kingside = TRUE;
		w_castling.queenside = TRUE;
		b_castling.kingside = FALSE;
		b_castling.queenside = FALSE;
	}
	else if (g_strcmp0 (casling_str, "Kk") == 0)
	{
		w_castling.kingside = TRUE;
		w_castling.queenside = FALSE;
		b_castling.kingside = TRUE;
		b_castling.queenside = FALSE;
	}
	else if (g_strcmp0 (casling_str, "Kq") == 0)
	{
		w_castling.kingside = TRUE;
		w_castling.queenside = FALSE;
		b_castling.kingside = FALSE;
		b_castling.queenside = TRUE;
	}
	else if (g_strcmp0 (casling_str, "Qk") == 0)
	{
		w_castling.kingside = FALSE;
		w_castling.queenside = TRUE;
		b_castling.kingside = TRUE;
		b_castling.queenside = FALSE;
	}
	else if (g_strcmp0 (casling_str, "Qq") == 0)
	{
		w_castling.kingside = FALSE;
		w_castling.queenside = TRUE;
		b_castling.kingside = FALSE;
		b_castling.queenside = TRUE;
	}
	else if (g_strcmp0 (casling_str, "kq") == 0)
	{
		w_castling.kingside = FALSE;
		w_castling.queenside = FALSE;
		b_castling.kingside = TRUE;
		b_castling.queenside = TRUE;
	}
	else if (g_strcmp0 (casling_str, "K") == 0)
	{
		w_castling.kingside = TRUE;
		w_castling.queenside = FALSE;
		b_castling.kingside = FALSE;
		b_castling.queenside = FALSE;
	}
	else if (g_strcmp0 (casling_str, "Q") == 0)
	{
		w_castling.kingside = FALSE;
		w_castling.queenside = TRUE;
		b_castling.kingside = FALSE;
		b_castling.queenside = FALSE;
	}
	else if (g_strcmp0 (casling_str, "k") == 0)
	{
		w_castling.kingside = FALSE;
		w_castling.queenside = FALSE;
		b_castling.kingside = TRUE;
		b_castling.queenside = FALSE;
	}
	else if (g_strcmp0 (casling_str, "q") == 0)
	{
		w_castling.kingside = FALSE;
		w_castling.queenside = FALSE;
		b_castling.kingside = FALSE;
		b_castling.queenside = TRUE;
	}
	else if (g_strcmp0 (casling_str, "-") == 0)
	{
		w_castling.kingside = FALSE;
		w_castling.queenside = FALSE;
		b_castling.kingside = FALSE;
		b_castling.queenside = FALSE;
	}
	else
	{
		g_set_error (error, 1, 1, 
			"Found expected - or \"KQkq\" must be ordered.");
			return FALSE;
	}
#else
	for(gint i = 0; i < 4; i++)
	{
		if (casling_str[i] == '-' || casling_str[i] == '\0')
			break;
		else if (casling_str[i] == 'K')
			w_castling.kingside = TRUE;
		else if (casling_str[i] == 'Q')
			w_castling.queenside = TRUE;
		else if (casling_str[i] == 'k')
			b_castling.kingside = TRUE;
		else if (casling_str[i] == 'q')
			b_castling.queenside = TRUE;
		else
		{
			g_set_error (error, 1, 1, 
			"Found '%c', expected - or KQkq", 
			 casling_str[i]);
			return FALSE;
		}
	}
#endif

	board->castling[BLACK] = b_castling;
	board->castling[WHITE] = w_castling;

	space = fen_str[index++];
	if (space != ' ')
	{
		g_set_error (error, 1, 1, 
			"Found '%c', expected a whitespace (' ') after castling.", 
			 space);
		return FALSE;
	}

	gchar file_char = fen_str[index++];
	if (file_char == '-')
	{
		board->en_passant = NULL_SQUARE;
	}
	else
	{
		if ('a' <= file_char && file_char <= 'h')
		{
			gchar rank_char = fen_str[index++];
			if (rank_char == '3' || rank_char == '6')
			{
				if ((rank_char == '6' && active_player == 'b') ||
					(rank_char == '3' && active_player == 'w'))
				{
					g_set_error (error, 1, 1, 
						"Found '%c%c' en passant, for player %s must be rank %c.", 
						file_char, rank_char, 
						active_player == 'b' ? "Black": "White",
						rank_char == '3' ? '6': '3');
					return FALSE;
				}
				gint rank = CHAR_RANK (rank_char);
				gint file = CHAR_FILE (file_char);

				if (active_player == 'b')
				{
					Piece pawn = PIECE_AT_SQUARE (board, SQUARE (file, RANK_4));
					Piece target = PIECE_AT_SQUARE (board, SQUARE (file, rank));
					Piece empty = PIECE_AT_SQUARE (board, SQUARE (file, RANK_2));
					if (pawn != WHITE_PAWN || PIECE_TYPE(target) != EMPTY || PIECE_TYPE(empty) != EMPTY)
					{
						g_set_error (error, 1, 1, 
							"Found '%c%c' en passant, is invalid.",
							file_char, rank_char);
						return FALSE;
					}
				}
				else
				{
					Piece pawn = PIECE_AT_SQUARE (board, SQUARE (file, RANK_5));
					Piece target = PIECE_AT_SQUARE (board, SQUARE (file, rank));
					Piece empty = PIECE_AT_SQUARE (board, SQUARE (file, RANK_7));
					if (pawn != BLACK_PAWN || PIECE_TYPE(target) != EMPTY || PIECE_TYPE(empty) != EMPTY)
					{
						g_set_error (error, 1, 1, 
							"Found '%c%c' en passant, is invalid.",
							file_char, rank_char);
						return FALSE;
					}
				}
				board->en_passant = SQUARE (file, rank);
			}
			else
			{
				g_set_error (error, 1, 1, 
					"Found '%c', expected 3 or 6 for rank.", 
					rank_char);
				return FALSE;
			}
		}
		else
		{
			g_set_error (error, 1, 1, 
			"Found '%c', expected a [a-h] on en passant file.", 
			 file_char);
			return FALSE;
		}
	}
	space = fen_str[index++];
	if (space != ' ')
	{
		g_set_error (error, 1, 1, 
			"Found '%c', expected a whitespace (' ') after en passant.", 
			 space);
		return FALSE;
	}

	
	gchar half_move_clock_sz[4] = { '\0', '\0', '\0', '\0' };	
	for(gint i = 0; i < 3; i++)
	{
		if (fen_str[index] != ' ')
		{
			half_move_clock_sz[i] = fen_str[index];
			index++;
		}
	}
	if (half_move_clock_sz[2] != '\0')
	{
		g_set_error (error, 1, 1, 
			"Found '%s', it must be number 0 to 50.", 
			 half_move_clock_sz);
		return FALSE;
	}
	gchar *endptr = NULL;
	guint half_move_clock = g_ascii_strtoull (half_move_clock_sz, &endptr, 10);
	if (endptr == half_move_clock_sz)
	{
		g_set_error (error, 1, 1, 
			"Found '%s', it must be number 0 to 99.", 
			 half_move_clock_sz);
		return FALSE;
	}
	board->half_move_clock = half_move_clock;

	space = fen_str[index++];
	if (space != ' ')
	{
		g_set_error (error, 1, 1, 
			"Found '%c', expected a whitespace (' ') after en passant.", 
			 space);
		return FALSE;
	}
	
	gchar move_number_sz[6] = { '\0', '\0', '\0', '\0', '\0', '\0' };	
	for(gint i = 0; i < 5; i++)
	{
		if ('0' <= fen_str[index] && fen_str[index] <= '9')
		{
			move_number_sz[i] = fen_str[index];
			index++;
		}
	}
	if (fen_str[index] != '\0')
	{
		g_set_error (error, 1, 1, 
			"Expected end of string, but found '%c'.", 
			 fen_str[index]);
		return FALSE;
	}
	if (move_number_sz[4] != '\0')
	{
		g_set_error (error, 1, 1, 
			"Expected end of string, but found '%c'.", 
			 move_number_sz[4]);
		return FALSE;
	}
	endptr = NULL;
	guint move_number = g_ascii_strtoull (move_number_sz, &endptr, 10);
	if (endptr == move_number_sz)
	{
		g_set_error (error, 1, 1, 
			"Found '%s', it must be number 0 to 9999.", 
			 move_number_sz);
		return FALSE;
	}
	if (move_number < half_move_clock)
	{
		g_set_error (error, 1, 1, 
			"Total Moves (%d) < Half Move Clock (%d).", 
			 move_number, half_move_clock);
		return FALSE;
	}
	board->move_number = move_number;

	return TRUE;
}

// For debugging
void
print_board(Board *board)
{
	g_print ("\n    a   b   c   d   e   f   g   h\n");
	g_print ("  +---+---+---+---+---+---+---+---+\n");
	for (int y = BOARD_SIZE - 1; y >= 0; y--) {
		g_print(" %d|", y + 1);
		for (guint x = 0; x < BOARD_SIZE; x++) 
		{
			Piece p = PIECE_AT(board, x, y);
			gchar c = char_from_piece(p);
			g_print(" %c |", PLAYER(p) == BLACK ? g_ascii_tolower (c) : c);

		}
		g_print("%d\n", y + 1);
		g_print ("  +---+---+---+---+---+---+---+---+\n");
	}
	g_print ("    a   b   c   d   e   f   g   h\n\n");
	gchar fen[90];
	to_fen (board, fen);
	g_print ("FEN: %s\n\n", fen);
}

static inline Square
find_piece_looking_at (Board *board, Square square, Player piece_owner)
{
	// We need to make sure we don't have infinite recursion in legal_move.
	// This can happen with looking for checks - we need to see if there are
	// any moves that put us in check to decide if the move is legal, but to
	// see if we are in check we need to look at all the moves our opponent
	// can make. And checking those moves will follow the same process.
	for(Square s = SQ_A1; s <= SQ_H8; s++)
	{
		Piece p = PIECE_AT_SQUARE(board, s);
		if (p == EMPTY || PLAYER (p) != piece_owner)
			continue;
		Move m = MOVE(s, square);

		if (pseudo_legal_move(board, m))
			return s;
	}

	return NULL_SQUARE;
}

static inline gboolean
exist_two_piece_looking_at (Board *board, Square square, Player piece_owner)
{
	uint8_t check_count = 0;
	for(Square s = SQ_A1; s <= SQ_H8; s++)
	{
		Piece p = PIECE_AT_SQUARE(board, s);
		if (p == EMPTY || PLAYER (p) != piece_owner)
			continue;
		Move m = MOVE(s, square);

		if (pseudo_legal_move(board, m))
			check_count++;
	}
	return check_count >= 2;
}

// static Square
// find_attacking_piece(Board *board, Square square, Player attacker)
// {
// 	// The easiest way to do this without duplicating logic from legal_move
// 	// is to put an enemy piece there and then check if moving there is legal.
// 	// This will trigger the logic in legal_move for pawn captures.	
// 	Square s = find_piece_looking_at(board, square, attacker);

// 	return s;
// }

gboolean
under_attack(Board *board, Square square, Player attacker)
{
	return find_piece_looking_at (board, square, attacker) != NULL_SQUARE;
}

Square
find_king(Board *board, Player p)
{
	// Piece king = PIECE(p, KING);
	
	// for(Square square = SQ_A1; square <= SQ_H8; square++)
	// {
	// 	if (PIECE_AT_SQUARE (board, square) == king)
	// 		return square;
	// }
	// //print_board (board);

	return board->king[p];
}

gboolean
in_check (Board *board, Player p)
{
	Square king_location = board->king[p];
	//g_assert (king_location != NULL_SQUARE); // both players should have a king
	return under_attack (board, king_location, OTHER_PLAYER(p));
}

gboolean
in_double_check (Board *board, Player p)
{
	Square king_location = board->king[p];
	
	return exist_two_piece_looking_at (board, king_location, OTHER_PLAYER (p));
}

// This could be more simply written as "number of legal moves = 0", if the
// enumeration of all legal moves is implemented at some point.
// As it is we don't have that, so this is simpler.
gboolean
checkmate(Board *board, Player p)
{
	// We must be in check
	if (!in_check(board, p))
		return FALSE;

	Square king_location = board->king[p];
	Player other = OTHER_PLAYER (p);
	File x = SQUARE_X (king_location);
	Rank y = SQUARE_Y (king_location);

	// Can the king move out of check?
	for (File dx = -1; dx < 2; dx++) 
	{
		for (Rank dy = -1; dy < 2; dy++) 
		{
			if (x + dx < 0 || x + dx >= BOARD_SIZE ||
					y + dy < 0 || y + dy >= BOARD_SIZE || (dx == 0 && dy == 0))
				continue;
			Move m = MOVE(king_location, SQUARE(x + dx, y + dy));
			if (legal_move (board, m, TRUE))
				return FALSE;
		}
	}

	// King cannot move, if is in double check, it is checkmake,
	// is impossible block or capture 2 pieces in 1 move
	if (in_double_check (board, p))
		return TRUE;

	// Can the attacking piece be captured?
	Square attacker = find_piece_looking_at (board, king_location, other);
	if (under_attack (board, attacker, p))
		return FALSE;

	// Can we block?
	PieceType type = PIECE_TYPE(PIECE_AT_SQUARE(board, attacker));
	if (type != KNIGHT) {
		File dx = SQUARE_X(attacker) - x;
		Rank dy = SQUARE_Y(attacker) - y;

		File ax = ABS(dx);
		Rank ay = ABS(dy);

		File x_direction = ax == 0 ? 0 : dx / ax;
		Rank y_direction = ay == 0 ? 0 : dy / ay;

		File x = SQUARE_X(king_location) + x_direction;
		Rank y = SQUARE_Y(king_location) + y_direction;
		while (!(x == SQUARE_X(attacker) &&
					y == SQUARE_Y(attacker))) 
		{
			Square blocker = find_piece_looking_at(board, SQUARE(x, y), p);
			if (blocker != NULL_SQUARE &&
					PIECE_TYPE(PIECE_AT_SQUARE(board, blocker)) != KING)
				return FALSE;

			x += x_direction;
			y += y_direction;
		}
	}

	// All outta luck
	return TRUE;
}

gboolean
can_castle_kingside(Board *board, Player p)
{
	Rank rank = p == WHITE ? RANK_1 : RANK_8;
	Player other = OTHER_PLAYER(p);

	return board->castling[p].kingside && 
		   PIECE_AT (board, FILE_F, rank) == EMPTY &&
		   PIECE_AT (board, FILE_G, rank) == EMPTY &&
		   !in_check (board, p) &&
		   !under_attack (board, SQUARE(FILE_F, rank), other) &&
		   !under_attack (board, SQUARE(FILE_G, rank), other);
}

gboolean
can_castle_queenside (Board *board, Player p)
{
	Rank rank = p == WHITE ? RANK_1 : RANK_8;
	Player other = OTHER_PLAYER (p);

	return board->castling[p].queenside && 
		   PIECE_AT (board, FILE_D, rank) == EMPTY &&
		   PIECE_AT (board, FILE_C, rank) == EMPTY &&
		   PIECE_AT (board, FILE_B, rank) == EMPTY &&
	       !in_check (board, p) &&
		   !under_attack (board, SQUARE(FILE_D, rank), other) &&
		   !under_attack (board, SQUARE(FILE_C, rank), other) &&
		   !under_attack (board, SQUARE(FILE_B, rank), other);
}

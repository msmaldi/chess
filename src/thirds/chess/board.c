#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <glib.h>
#include "board.h"
#include "moves.h"

void copy_board(Board *dst, Board *src)
{
	dst->turn = src->turn;
	dst->castling[0] = src->castling[0];
	dst->castling[1] = src->castling[1];
	dst->en_passant = src->en_passant;
	dst->half_move_clock = src->half_move_clock;
	dst->move_number = src->move_number;

	for (uint x = 0; x < BOARD_SIZE; x++)
		for (uint y = 0; y < BOARD_SIZE; y++)
			PIECE_AT(dst, x, y) = PIECE_AT(src, x, y);
}

// Converts a character into a piece, using the standard used in PGN and FEN.
Piece piece_from_char(char c)
{
	Player player = isupper(c) ? WHITE : BLACK;
	switch (tolower(c)) {
	case 'p': return PIECE(player, PAWN);
	case 'n': return PIECE(player, KNIGHT);
	case 'b': return PIECE(player, BISHOP);
	case 'r': return PIECE(player, ROOK);
	case 'q': return PIECE(player, QUEEN);
	case 'k': return PIECE(player, KING);
	default: return EMPTY;
	}
}

// Converts a piece into a char, using the standard used in PGN and FEN
char char_from_piece(Piece p)
{
	switch (PIECE_TYPE(p)) {
	case PAWN:   return 'P';
	case KNIGHT: return 'N';
	case BISHOP: return 'B';
	case ROOK:   return 'R';
	case QUEEN:  return 'Q';
	case KING:   return 'K';
	default:     return ' ';
	}
}

char *start_board_fen =
	"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

gint to_fen (Board *board, gchar *fen)
{
	gchar *curr_player = (board->turn == BLACK ? "b" : "w");

	gchar castling[5] = { '-', '\0', '\0', '\0', '\0' };
	int c = 0;
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
	for (int i = 56; i >=0; i -= 8)
	{
		int no_piece = 0;
		for (int aux = 0; aux < 8; aux++)
		{	
			int index = i + aux;
			Piece piece = board->pieces[index];
			gchar ca = char_from_piece (piece);
			gchar ch = PLAYER(piece) == BLACK ? tolower(ca) : ca;

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

	uint half_move_clock = board->half_move_clock;
	uint move_number = board->move_number;

	return g_snprintf (fen, 100, "%s %s %s %s %d %d", board_fen, curr_player, castling, en_passant, half_move_clock, move_number);
}

// Initializes a Board given a string containing Forsyth-Edwards Notation (FEN)
// See <http://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation> for a
// description, and <http://kirill-kryukov.com/chess/doc/fen.html> for the spec
// TODO: this is way too fucking long
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

Piece piece_from_char_n (gchar c)
{	
	switch (c)
	{
		case 'p': return PIECE(BLACK, PAWN);
		case 'n': return PIECE(BLACK, KNIGHT);
		case 'b': return PIECE(BLACK, BISHOP);
		case 'r': return PIECE(BLACK, ROOK);
		case 'q': return PIECE(BLACK, QUEEN);
		case 'k': return PIECE(BLACK, KING);
		case 'P': return PIECE(WHITE, PAWN);
		case 'N': return PIECE(WHITE, KNIGHT);
		case 'B': return PIECE(WHITE, BISHOP);
		case 'R': return PIECE(WHITE, ROOK);
		case 'Q': return PIECE(WHITE, QUEEN);
		case 'K': return PIECE(WHITE, KING);
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
	for (int rank = RANK_8; rank >= RANK_1; rank--)
	{
		gint file = 0;
		while (fen_str[index] != '/' && fen_str[index] != ' ')
		{
			if (is_number_fen_rank (fen_str[index]))
			{
				gint empty_squares = fen_str[index] - '0';
				//g_print ("\tFile: %c\t Rank: %d\t Empty: %d\n", file + 'a', rank + 1, empty_squares);	
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
				if (file + 1 > 8)
				{
					g_set_error (error, 1, 1, 
						"Rank %d length > 8", rank + 1);
					return FALSE;
				}	
				//g_print ("\tFile: %c\t Rank: %d\t Piece: %c\n", file + 'a', rank + 1, fen_str[index]);	
				Piece piece = piece_from_char_n (fen_str[index]);
				if (piece == PIECE(BLACK, KING))
					b_king_count++;
				if (piece == PIECE(WHITE, KING))
					w_king_count++;

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

	Castling b_castling = { false, false };
	Castling w_castling = { false, false };	
	gchar casling_str[5] = { '-', '\0', '\0', '\0', '\0' };
	for(gint i = 0; i < 4; i++)
	{
		if (fen_str[index] != ' ')
			casling_str[i] = fen_str[index++];
	}
	//g_print ("%s'%c'\n", casling_str, fen_str[index]);
	for(gint i = 0; i < 4; i++)
	{
		if (casling_str[i] == '-' || casling_str[i] == '\0')
			break;
		else if (casling_str[i] == 'K')
			w_castling.kingside = true;
		else if (casling_str[i] == 'Q')
			w_castling.queenside = true;
		else if (casling_str[i] == 'k')
			b_castling.kingside = true;
		else if (casling_str[i] == 'q')
			b_castling.queenside = true;
		else
		{
			g_set_error (error, 1, 1, 
			"Found '%c', expected - or KQkq", 
			 casling_str[i]);
			return FALSE;
		}
	}
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
		board->en_passant == NULL_SQUARE;
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
		if ('0' <= fen_str[index] && fen_str[index] <= '9')//(fen_str[index] != '\0')
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
void print_board(Board *b)
{
	puts("..........");
	for (int y = BOARD_SIZE - 1; y >= 0; y--) {
		putchar('.');
		for (uint x = 0; x < BOARD_SIZE; x++) {
			Piece p = PIECE_AT(b, x, y);
			char c = char_from_piece(p);
			putchar(PLAYER(p) == BLACK ? tolower(c) : c);
		}

		puts(".");
	}
	puts("..........");
	printf("%s to move\n", b->turn == WHITE ? "White" : "Black");
}

static Square find_piece_looking_at(Board *board, Square square, Player piece_owner)
{
	// We need to make sure we don't have infinite recursion in legal_move.
	// This can happen with looking for checks - we need to see if there are
	// any moves that put us in check to decide if the move is legal, but to
	// see if we are in check we need to look at all the moves our opponent
	// can make. And checking those moves will follow the same process.
	//
	// However, we don't actually need to see if the moves put us into check in
	// this case, as it doesn't matter if taking their king puts us in check;
	// we've already won.
	gboolean care_about_check = PIECE_TYPE(PIECE_AT_SQUARE(board, square)) != KING;

	Player initial_turn = board->turn;
	board->turn = piece_owner;

	Square ret = NULL_SQUARE;

	for (uint y = 0; y < BOARD_SIZE; y++) {
		for (uint x = 0; x < BOARD_SIZE; x++) {
			Piece p = PIECE_AT(board, x, y);
			Square s = SQUARE(x, y);
			Move m = MOVE(s, square);

			if (PLAYER(p) == piece_owner &&
					legal_move(board, m, care_about_check)) {
				ret = s;
				goto cleanup;
			}
		}
	}

cleanup:
	board->turn = initial_turn;
	return ret;
}

static Square find_attacking_piece(Board *board, Square square, Player attacker)
{
	// The easiest way to do this without duplicating logic from legal_move
	// is to put an enemy piece there and then check if moving there is legal.
	// This will trigger the logic in legal_move for pawn captures.
	Piece initial_piece = PIECE_AT_SQUARE(board, square);
	PIECE_AT_SQUARE(board, square) =
		PIECE(OTHER_PLAYER(attacker), PAWN);

	Square s = find_piece_looking_at(board, square, attacker);

	PIECE_AT_SQUARE(board, square) = initial_piece;

	return s;
}

static gboolean under_attack(Board *board, Square square, Player attacker)
{
	return find_attacking_piece(board, square, attacker) != NULL_SQUARE;
}

static Square find_king(Board *board, Player p)
{
	Piece king = PIECE(p, KING);

	for (uint y = 0; y < BOARD_SIZE; y++)
		for (uint x = 0; x < BOARD_SIZE; x++)
			if (PIECE_AT(board, x, y) == king)
				return SQUARE(x, y);

	return NULL_SQUARE;
}

gboolean in_check(Board *board, Player p)
{
	Square king_location = find_king (board, p);
	g_assert (king_location != NULL_SQUARE); // both players should have a king
	return under_attack (board, king_location, OTHER_PLAYER(p));
}

// This could be more simply written as "number of legal moves = 0", if the
// enumeration of all legal moves is implemented at some point.
// As it is we don't have that, so this is simpler.
gboolean checkmate(Board *board, Player p)
{
	// We must be in check
	if (!in_check(board, p))
		return false;

	Square king_location = find_king(board, p);
	Player other = OTHER_PLAYER(p);
	int x = SQUARE_X(king_location);
	int y = SQUARE_Y(king_location);

	// Can the king move out of check?
	for (int dx = -1; dx < 2; dx++) {
		for (int dy = -1; dy < 2; dy++) {
			if (x + dx < 0 || x + dx >= BOARD_SIZE ||
					y + dy < 0 || y + dy >= BOARD_SIZE)
				continue;
			Move m = MOVE(king_location, SQUARE(x + dx, y + dy));
			if (legal_move(board, m, true))
				return false;
		}
	}

	// Can the attacking piece be captured?
	Square attacker = find_attacking_piece(board, king_location, other);
	if (under_attack(board, attacker, p))
		return false;

	// Can we block?
	Piece_type type = PIECE_TYPE(PIECE_AT_SQUARE(board, attacker));
	if (type != KNIGHT) {
		int dx = SQUARE_X(attacker) - x;
		int dy = SQUARE_Y(attacker) - y;

		int ax = abs(dx);
		int ay = abs(dy);

		int x_direction = ax == 0 ? 0 : dx / ax;
		int y_direction = ay == 0 ? 0 : dy / ay;

		uint x = SQUARE_X(king_location) + x_direction;
		uint y = SQUARE_Y(king_location) + y_direction;
		while (!(x == SQUARE_X(attacker) &&
					y == SQUARE_Y(attacker))) {
			Square blocker = find_piece_looking_at(board, SQUARE(x, y), p);
			if (blocker != NULL_SQUARE &&
					PIECE_TYPE(PIECE_AT_SQUARE(board, blocker)) != KING)
				return false;

			x += x_direction;
			y += y_direction;
		}
	}

	// All outta luck
	return true;
}

gboolean can_castle_kingside(Board *board, Player p)
{
	uint y = p == WHITE ? 0 : BOARD_SIZE - 1;
	Player other = OTHER_PLAYER(p);

	return board->castling[p].kingside && !in_check(board, p) &&
		PIECE_AT(board, 5, y) == EMPTY &&
		PIECE_AT(board, 6, y) == EMPTY &&
		!under_attack(board, SQUARE(5, y), other) &&
		!under_attack(board, SQUARE(6, y), other);
}

gboolean
can_castle_queenside (Board *board, Player p)
{
	guint y = p == WHITE ? 0 : BOARD_SIZE - 1;
	Player other = OTHER_PLAYER(p);

	return board->castling[p].queenside && 
	       !in_check	(board, p) &&
		   PIECE_AT(board, 3, y) == EMPTY &&
		   PIECE_AT(board, 2, y) == EMPTY &&
		   PIECE_AT(board, 1, y) == EMPTY &&
		   !under_attack(board, SQUARE(3, y), other) &&
		   !under_attack(board, SQUARE(2, y), other) &&
		   !under_attack(board, SQUARE(1, y), other);
}

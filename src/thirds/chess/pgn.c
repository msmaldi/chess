
#line 1 "src/chess/pgn.rl"
#include <assert.h>
#include <ctype.h>
#include <gio/gio.h>
#include <glib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "moves.h"
#include "pgn.h"

// TODO: error messages


#line 19 "src/chess/pgn.c"
static const char _pgn_tokenizer_actions[] = {
	0, 1, 2, 1, 5, 1, 6, 1, 
	7, 1, 8, 1, 9, 1, 10, 1, 
	11, 1, 12, 1, 13, 1, 14, 1, 
	15, 1, 16, 1, 17, 2, 0, 1, 
	2, 3, 4
};

static const char _pgn_tokenizer_key_offsets[] = {
	0, 0, 3, 5, 24, 27, 29
};

static const char _pgn_tokenizer_trans_keys[] = {
	34, 32, 126, 48, 57, 32, 34, 36, 
	40, 41, 42, 46, 60, 62, 91, 93, 
	9, 13, 48, 57, 65, 90, 97, 122, 
	34, 32, 126, 48, 57, 35, 43, 45, 
	61, 95, 47, 58, 65, 90, 97, 122, 
	0
};

static const char _pgn_tokenizer_single_lengths[] = {
	0, 1, 0, 11, 1, 0, 5
};

static const char _pgn_tokenizer_range_lengths[] = {
	0, 1, 1, 4, 1, 1, 3
};

static const char _pgn_tokenizer_index_offsets[] = {
	0, 0, 3, 5, 21, 24, 26
};

static const char _pgn_tokenizer_trans_targs[] = {
	4, 1, 3, 5, 0, 3, 1, 2, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 6, 6, 6, 0, 4, 1, 3, 
	5, 3, 6, 6, 6, 6, 6, 6, 
	6, 6, 3, 3, 3, 3, 3, 0
};

static const char _pgn_tokenizer_trans_actions[] = {
	32, 0, 27, 0, 0, 3, 0, 0, 
	13, 15, 7, 5, 17, 19, 9, 11, 
	3, 0, 0, 0, 0, 32, 0, 23, 
	0, 25, 0, 0, 0, 0, 0, 0, 
	0, 0, 21, 27, 23, 25, 21, 0
};

static const char _pgn_tokenizer_to_state_actions[] = {
	0, 0, 0, 29, 0, 0, 0
};

static const char _pgn_tokenizer_from_state_actions[] = {
	0, 0, 0, 1, 0, 0, 0
};

static const char _pgn_tokenizer_eof_trans[] = {
	0, 36, 0, 0, 37, 38, 39
};

static const int pgn_tokenizer_start = 3;
static const int pgn_tokenizer_first_final = 3;
static const int pgn_tokenizer_error = 0;

static const int pgn_tokenizer_en_main = 3;


#line 18 "src/chess/pgn.rl"


typedef enum Token_type
{
	// Tokens with different values
	STRING, INTEGER, NAG, SYMBOL,
	// Fixed tokens - always just a single character
	DOT, ASTERISK, L_SQUARE_BRACKET, R_SQUARE_BRACKET,
	L_BRACKET, R_BRACKET, L_ANGLE_BRACKET, R_ANGLE_BRACKET, 
} Token_type;

typedef union Token_value {
	char *string;
	uint integer;
} Token_value;

typedef struct Token
{
	Token_type type;
	Token_value value;
} Token;

void print_token(Token *t)
{
	switch (t->type) {
	case STRING: printf("STRING: %s\n", t->value.string); break;
	case SYMBOL: printf("SYMBOL: %s\n", t->value.string); break;
	case NAG: printf("NAG: %s\n", t->value.string); break;
	case INTEGER: printf("INTEGER: %d\n", t->value.integer); break;
	case DOT: puts("DOT"); break;
	case ASTERISK: puts("DOT"); break;
	case L_SQUARE_BRACKET: puts("L_SQUARE_BRACKET"); break;
	case R_SQUARE_BRACKET: puts("R_SQUARE_BRACKET"); break;
	case L_BRACKET: puts("L_BRACKET"); break;
	case R_BRACKET: puts("R_BRACKET"); break;
	case L_ANGLE_BRACKET: puts("L_ANGLE_BRACKET"); break;
	case R_ANGLE_BRACKET: puts("R_ANGLE_BRACKET"); break;
	}
}

static bool symbol_is_integer(Token *t)
{
	char *str = t->value.string;
	for (size_t i = 0; str[i] != '\0'; i++)
		if (!isdigit(str[i]))
			return false;

	return true;
}
		

static char *read_escaped_string(char *str, size_t length)
{
	char *out = malloc(length + 1);
	size_t j = 0;
	bool escaping = false;

	for (size_t i = 0; i < length; i++) {
		char c = str[i];
		switch (c) {
		case '\\':
			if (escaping) {
				out[j++] = '\\';
				escaping = false;
			} else {
				escaping = true;
			}

			break;
		case '"':
			// We should never come across an unescaped quote in a string, as
			// the tokenizer should have terminated the string in this case.
			assert(escaping);

			out[j++] = '"';
			escaping = false;
			break;
		default:
			out[j++] = c;
			escaping = false;
			break;
		}
	}

	out[j] = '\0';

	return out;
}

static GArray *tokenize_pgn(char *buf, gsize length)
{
	// The initial size (140) is just a rough estimate of the average number of
	// tokens, based on 4 tokens for each of the 7 required tags, plus 4 tokens
	// for each of 30 moves.
	GArray *tokens = g_array_sized_new(FALSE, FALSE, sizeof(Token), 140);

	// Variables that Ragel needs
	char *p = buf, *pe = buf + length;
	char *eof = NULL;
	char *ts, *te;
	int cs, act;
	// act is initialized for scanners, so it must be declared, but in this
	// scanner we don't actually use it. This silences the compiler warning.
	IGNORE(act);

	
#line 195 "src/chess/pgn.c"
	{
	cs = pgn_tokenizer_start;
	ts = 0;
	te = 0;
	act = 0;
	}

#line 124 "src/chess/pgn.rl"
	
#line 205 "src/chess/pgn.c"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_acts = _pgn_tokenizer_actions + _pgn_tokenizer_from_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 2:
#line 1 "NONE"
	{ts = p;}
	break;
#line 226 "src/chess/pgn.c"
		}
	}

	_keys = _pgn_tokenizer_trans_keys + _pgn_tokenizer_key_offsets[cs];
	_trans = _pgn_tokenizer_index_offsets[cs];

	_klen = _pgn_tokenizer_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _pgn_tokenizer_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
_eof_trans:
	cs = _pgn_tokenizer_trans_targs[_trans];

	if ( _pgn_tokenizer_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _pgn_tokenizer_actions + _pgn_tokenizer_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 3:
#line 1 "NONE"
	{te = p+1;}
	break;
	case 4:
#line 125 "src/chess/pgn.rl"
	{act = 3;}
	break;
	case 5:
#line 173 "src/chess/pgn.rl"
	{te = p+1;}
	break;
	case 6:
#line 177 "src/chess/pgn.rl"
	{te = p+1;{ Token t = { DOT,              { 0 } }; g_array_append_val(tokens, t); }}
	break;
	case 7:
#line 178 "src/chess/pgn.rl"
	{te = p+1;{ Token t = { ASTERISK,         { 0 } }; g_array_append_val(tokens, t); }}
	break;
	case 8:
#line 179 "src/chess/pgn.rl"
	{te = p+1;{ Token t = { L_SQUARE_BRACKET, { 0 } }; g_array_append_val(tokens, t); }}
	break;
	case 9:
#line 180 "src/chess/pgn.rl"
	{te = p+1;{ Token t = { R_SQUARE_BRACKET, { 0 } }; g_array_append_val(tokens, t); }}
	break;
	case 10:
#line 181 "src/chess/pgn.rl"
	{te = p+1;{ Token t = { L_BRACKET,        { 0 } }; g_array_append_val(tokens, t); }}
	break;
	case 11:
#line 182 "src/chess/pgn.rl"
	{te = p+1;{ Token t = { R_BRACKET,        { 0 } }; g_array_append_val(tokens, t); }}
	break;
	case 12:
#line 183 "src/chess/pgn.rl"
	{te = p+1;{ Token t = { L_ANGLE_BRACKET,  { 0 } }; g_array_append_val(tokens, t); }}
	break;
	case 13:
#line 184 "src/chess/pgn.rl"
	{te = p+1;{ Token t = { R_ANGLE_BRACKET,  { 0 } }; g_array_append_val(tokens, t); }}
	break;
	case 14:
#line 135 "src/chess/pgn.rl"
	{te = p;p--;{
			// + 1 for null terminator
			size_t length = te - ts + 1;
			char *token = malloc(length); 
			strncpy(token, ts, length - 1);
			token[length - 1] = '\0';

			Token t = { SYMBOL, { token } };

			g_array_append_val(tokens, t);
		}}
	break;
	case 15:
#line 125 "src/chess/pgn.rl"
	{te = p;p--;{
			// - 2 for quotes
			size_t length = te - ts - 2;
			char *token = read_escaped_string(ts + 1, length);

			Token t = { STRING, { token } };

			g_array_append_val(tokens, t);
		}}
	break;
	case 16:
#line 147 "src/chess/pgn.rl"
	{te = p;p--;{
			// + 1 for null terminator
			size_t length = te - ts + 1;
			char *token = malloc(length); 
			strncpy(token, ts, length - 1);
			token[length - 1] = '\0';

			Token t = { NAG, { token } };

			g_array_append_val(tokens, t);
		}}
	break;
	case 17:
#line 1 "NONE"
	{	switch( act ) {
	case 0:
	{{cs = 0; goto _again;}}
	break;
	case 3:
	{{p = ((te))-1;}
			// - 2 for quotes
			size_t length = te - ts - 2;
			char *token = read_escaped_string(ts + 1, length);

			Token t = { STRING, { token } };

			g_array_append_val(tokens, t);
		}
	break;
	}
	}
	break;
#line 395 "src/chess/pgn.c"
		}
	}

_again:
	_acts = _pgn_tokenizer_actions + _pgn_tokenizer_to_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 0:
#line 1 "NONE"
	{ts = 0;}
	break;
	case 1:
#line 1 "NONE"
	{act = 0;}
	break;
#line 412 "src/chess/pgn.c"
		}
	}

	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	if ( _pgn_tokenizer_eof_trans[cs] > 0 ) {
		_trans = _pgn_tokenizer_eof_trans[cs] - 1;
		goto _eof_trans;
	}
	}

	_out: {}
	}

#line 189 "src/chess/pgn.rl"


	// Integers are a subset of symbols, and unfortunately Ragel scanners
	// attempt to match longer patterns before shorter ones.
	// So we do a second pass to look for symbols that are integers.
	for (size_t i = 0; i < tokens->len; i++) {
		Token *t = &g_array_index(tokens, Token, i);
		if (t->type == SYMBOL && symbol_is_integer(t)) {
			int n;
			sscanf(t->value.string, "%d", &n);

			t->type = INTEGER;
			t->value.integer = n;
		}
	}

	return tokens;
}

// It is impossible to parse a move without a reference to a particular board,
// as something like Bd5 could start from any square on that diagonal.
static Move parse_move(Board *board, char *notation)
{
	// First we remove 'x's, '+'s, and '#'s, as we don't need them and they only
	// complicate parsing.
	char stripped[6]; // max length without 'x#+'s, + 1 for null terminator
	size_t j = 0;
	for (size_t i = 0; notation[i] != '\0'; i++)
		if (notation[i] != 'x' && notation[i] != '#' && notation[i] != '+')
		{
			if (notation[i] == '=')
			{
				i++;
				continue;
			}
			stripped[j++] = notation[i];
		}
	stripped[j] = '\0';

	if (strcmp(notation, "O-O") == 0) {
		uint y = board->turn == WHITE ? 0 : 7;
		return MOVE(SQUARE(4, y), SQUARE(6, y));
	}
	if (strcmp(notation, "O-O-O") == 0) {
		uint y = board->turn == WHITE ? 0 : 7;
		return MOVE(SQUARE(4, y), SQUARE(2, y));
	}

	size_t i = 0;
	Piece_type type;
	// If it's a pawn move, the target square starts at the beginning of the
	// string
	if (islower(stripped[0])) {
		type = PAWN;

		// An exception: if it's a move like exd5, the target square starts one 
		// char further on
		if (islower(stripped[1]))
			i++;
	} else {
		type = PIECE_TYPE(piece_from_char(stripped[0]));
		i++;
	}
	
	char disambig = 0;
	if (j == 4)
	{		
		// If it's this long, there must be a disambiguation char in there
		disambig = stripped[i++];
	}

	
	uint target_file = CHAR_FILE(stripped[i++]);
	uint target_rank = CHAR_RANK(stripped[i++]);

	if (disambig != 0) {
		uint x = 0, y = 0, dx = 0, dy = 0;
		if (disambig >= 'a' && disambig <= 'g') {
			x = CHAR_FILE(disambig);
			dy = 1;
		} else if (disambig >= '1' && disambig <= '8') {
			y = CHAR_RANK(disambig);
			dx = 1;
		} else {
			return NULL_MOVE();
		}

		for (; x < BOARD_SIZE && y < BOARD_SIZE; x += dx, y += dy) {
			Piece p = PIECE_AT(board, x, y);
			if (PIECE_TYPE(p) != type || PLAYER(p) != board->turn)
				continue;

			//Move m// = MOVE(SQUARE(x, y), SQUARE(target_file, target_rank));

			bool has_promotion = false;
			Piece_type promotion = QUEEN;
			
			{
				for (size_t i = 0; notation[i] != '\0'; i++)
				{
					if (notation[i] == '=')
					{
						has_promotion = true;
						i++;
						if (notation[i] == 'Q')
							promotion = QUEEN;
						else if (notation[i] == 'R')
							promotion = ROOK;
						else if (notation[i] == 'N')
							promotion = KNIGHT;
						else if (notation[i] == 'B')
							promotion = BISHOP;
						else
							return NULL_MOVE();
					}
				}				
			}

			Move m;// = MOVE(SQUARE(x, y), SQUARE(target_file, target_rank));
			if (has_promotion)
				m = MOVE_WITH_PROMOTION (SQUARE(x, y), SQUARE(target_file, target_rank), promotion);
			else
				m = MOVE(SQUARE(x, y), SQUARE(target_file, target_rank));

			//g_print ("%s %d %d %d %d %d\n", notation, x, y, target_file, target_rank, promotion);



			if (legal_move(board, m, true))
				return m;
		}

		return NULL_MOVE();
	}

	for (uint x = 0; x < BOARD_SIZE; x++) {
		for (uint y = 0; y < BOARD_SIZE; y++) {
			Piece p = PIECE_AT(board, x, y);
			if (PIECE_TYPE(p) != type || PLAYER(p) != board->turn)
				continue;
			

			bool has_promotion = false;
			Piece_type promotion = QUEEN;
			
			{
				for (size_t i = 0; notation[i] != '\0'; i++)
				{
					if (notation[i] == '=')
					{
						has_promotion = true;
						i++;
						if (notation[i] == 'Q')
							promotion = QUEEN;
						else if (notation[i] == 'R')
							promotion = ROOK;
						else if (notation[i] == 'N')
							promotion = KNIGHT;
						else if (notation[i] == 'B')
							promotion = BISHOP;
						else 
							return NULL_MOVE();
					}
				}				
			}

			Move m;// = MOVE(SQUARE(x, y), SQUARE(target_file, target_rank));
			if (has_promotion && PIECE_TYPE(p) == PAWN)
				m = MOVE_WITH_PROMOTION (SQUARE(x, y), SQUARE(target_file, target_rank), promotion);
			else
				m = MOVE(SQUARE(x, y), SQUARE(target_file, target_rank));
			
			if (legal_move(board, m, true))
				return m;
		}
	}

	return NULL_MOVE();
}

static Result parse_game_termination_marker(Token *t)
{
	if (t->type == ASTERISK)
		return OTHER;
	if (t->type != SYMBOL)
		return NULL_RESULT;

	char *symbol = t->value.string;
	if (strcmp(symbol, "1-0") == 0)
		return WHITE_WINS;
	if (strcmp(symbol, "0-1") == 0)
		return BLACK_WINS;
	if (strcmp(symbol, "1/2-1/2") == 0)
		return DRAW;
	
	return NULL_RESULT;
}

static char *game_termination_marker(Result r)
{
	switch (r) {
	case WHITE_WINS: return "1-0";
	case BLACK_WINS: return "0-1";
	case DRAW: return "1/2-1/2";
	default: return "*";
	}
}

static bool parse_tokens(PGN *pgn, GArray *tokens, GError **err)
{
	// Start with tags
	pgn->tags = g_hash_table_new(g_str_hash, g_str_equal);

	size_t i = 0;
	while ((&g_array_index(tokens, Token, i))->type == L_SQUARE_BRACKET) {
		Token *tag_name_token = &g_array_index(tokens, Token, ++i);
		if (tag_name_token->type != SYMBOL) {
			if (tag_name_token->type == INTEGER) {
				g_set_error(err, 0, 0,
						"Unexpected token: %d", tag_name_token->value.integer);
			} else {
				g_set_error(err, 0, 0,
						"Unexpected token: %s", tag_name_token->value.string);
			}

			return false;
		}

		char *tag_name = tag_name_token->value.string;

		if (g_hash_table_contains(pgn->tags, tag_name)) {
			g_set_error(err, 0, 0, "Duplicate tag: %s", tag_name);
			return false;
		}

		Token *tag_value_token = &g_array_index(tokens, Token, ++i);
		if (tag_value_token->type != STRING) {
			if (tag_name_token->type == INTEGER) {
				g_set_error(err, 0, 0, "Tag values must be strings, %d is not",
						tag_value_token->value.integer);
			} else {
				g_set_error(err, 0, 0, "Tag values must be strings, %s is not",
						tag_value_token->value.string);
			}

			return false;
		}

		char *tag_value = tag_value_token->value.string;

		char *tag_name_copy = malloc(strlen(tag_name) + 1);
		strcpy(tag_name_copy, tag_name);
		char *tag_value_copy = malloc(strlen(tag_value) + 1);
		strcpy(tag_value_copy, tag_value);

		g_hash_table_insert(pgn->tags, tag_name_copy, tag_value_copy);

		Token *close_square_bracket_token = &g_array_index(tokens, Token, ++i);
		if (close_square_bracket_token->type != R_SQUARE_BRACKET) {
			g_set_error(err, 0, 0,
					"Tag %s has no matching close bracket", tag_name);
			return false;
		}

		i++;
	}

	// Now the movetext section
	uint half_move_number = 2;

	Game *game = game_new ();
	pgn->game = game;
	GError *error = NULL;
	// TODO: Use value in start board tag if present.

	//from_fen(game->board, start_board_fen);
	board_from_fen (game->board, start_board_fen, &error);

	// TODO: variations, NAG
	while (i < tokens->len) {
		Token *t = &g_array_index(tokens, Token, i++);
		if (t->type == INTEGER) {
			if (t->value.integer != half_move_number / 2) {
				g_set_error(err, 0, 0,
						"Incorrect move number %d (should be %d)",
						t->value.integer, half_move_number / 2);
				return false;
			}

			while ((t = &g_array_index(tokens, Token, i++))->type == DOT)
				;
		}

		if (t->type != SYMBOL && t->type != ASTERISK) {
			if (t->type == INTEGER) {
				g_set_error(err, 0, 0, "Expected a move, got %d",
						t->value.integer);
			} else {
				g_set_error(err, 0, 0, "Expected a move, got %s",
						t->value.string);
			}

			return false;
		}

		Result r;
		if ((r = parse_game_termination_marker(t)) != NULL_RESULT) {
			pgn->result = r;

			// If we didn't see a result tag, try to fill it in with the value
			// in the game termination marker

			if (!g_hash_table_contains(pgn->tags, "Result")) {
				char *tmp = malloc(strlen(t->value.string));
				strcpy(tmp, t->value.string);
				g_hash_table_insert(pgn->tags, "Result", tmp);
			}

			return true;
		}

		Move m = parse_move(game->board, t->value.string);
		if (( eq_move (m, NULL_MOVE()))) {
			g_set_error(err, 0, 0, "Expected a move, got %s", t->value.string);
			
			return false;
		}

		game = add_child(game, m);

		half_move_number++;
	}

	return true;
}

static void free_tokens(GArray *tokens)
{
	for (size_t i = 0; i < tokens->len; i++) {
		Token *t = &g_array_index(tokens, Token, i);
		if (t->type == SYMBOL || t->type == STRING || t->type == NAG)
			free(t->value.string);
	}

	g_array_free(tokens, TRUE);
}


bool read_pgn(PGN *pgn, const char *input_filename, GError **error)
{
	assert(*error == NULL);

	bool ret = true;
	char *buf;
	gsize length;

	// We just load the whole file into memory.
	// PGN files are typically very short: even the longest game in tournament
	// history is only about 3.5KB.
	// If someone tries to load a 2GB PGN and complains about it crashing, they
	// need to take a long, hard look at themself.
	GFile *file = g_file_new_for_path(input_filename);
	if (!g_file_load_contents(file, NULL, &buf, &length, NULL, error)) {
		ret = false;
		goto cleanup;
	}

	GArray *tokens = tokenize_pgn(buf, length);
	ret = parse_tokens(pgn, tokens, error);
	free_tokens(tokens);

cleanup:
	g_free(buf);
	g_object_unref(file);

	return ret;
}

static const char *seven_tag_roster[] =
{
	"Event", "Site", "Date", "Round", "White", "Black", "Result"
};

static bool in_seven_tag_roster(char *tag_name)
{
	size_t size = sizeof(seven_tag_roster) / sizeof(seven_tag_roster[0]);
	for (size_t i = 0; i < size; i++)
		if (strcmp(tag_name, seven_tag_roster[i]) == 0)
			return true;
	
	return false;
}

static void write_tag(FILE *file, const char *tag_name, const char *tag_value)
{
	// TODO: handle IO errors
	fprintf(file, "[%s \"" , tag_name);

	for (size_t i = 0; tag_value[i] != '\0'; i++) {
		char c = tag_value[i];
		if (c == '\\' || c == '"')
			putc('\\', file);

		putc(c, file);
	}

	fputs("\"]\n", file);
}

static char *default_tag_value(const char *tag_name)
{
	if (strcmp(tag_name, "Date") == 0)
		return "????.??.??";
	if (strcmp(tag_name, "Result") == 0)
		return "*";
	else
		return "?";
}

static void process_tag(gpointer key, gpointer value, gpointer user_data)
{
	FILE *file = (FILE *)user_data;
	char *tag_name = (char *)key;
	char *tag_value = (char *)value;

	// We print the standard seven tags first, in a specified order, so don't
	// print them again.
	if (in_seven_tag_roster(tag_name))
		return;

	write_tag(file, tag_name, tag_value);
}

// TODO: handle IO errors
bool write_pgn(PGN *pgn, FILE *file)
{
	size_t size = sizeof(seven_tag_roster) / sizeof(seven_tag_roster[0]);
	for (size_t i = 0; i < size; i++) {
		const char *tag_name = seven_tag_roster[i];
		const char *tag_value = g_hash_table_contains(pgn->tags, tag_name) ?
			g_hash_table_lookup(pgn->tags, tag_name) :
			default_tag_value(tag_name);
		write_tag(file, tag_name, tag_value);
	}
	g_hash_table_foreach(pgn->tags, process_tag, (void *)file);

	putc('\n', file);
	
	Game *game = pgn->game->children;
	do {
		// A lot of the logic in here seems reversed/offset by one. This is
		// because the board associated with a particular move actually lives
		// at the parent node for that board
		if (game->board->turn == BLACK)
			fprintf(file, game->board->move_number == 1 ?
				"%d." :
				" %d.", game->board->move_number);

		char move_str[MAX_ALGEBRAIC_NOTATION_LENGTH];
		algebraic_notation_for(game->parent->board, game->move, move_str);
		fprintf(file, " %s", move_str);
		
		game = first_child(game);
	} while (game != NULL);

	fprintf(file, " %s\n", game_termination_marker(pgn->result));

	return true;
}

void free_pgn(PGN *pgn)
{
	g_hash_table_destroy(pgn->tags);
	free_game(pgn->game);
}

#include "pgn.h"
#include <gio/gio.h>

#define VALID_START_TAG_NAME(c) (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))
#define SKIPABLE_CHAR(c) ((c == ' ' || c == '\n' || c == '\r' || c == '\0'))


static gboolean read_tags (PGN *pgn, const gchar *buffer, GError **error);
static guint read_number_dot (guint move_number, gchar *buffer, GError **error);
static guint read_number_dot_dot_dot (guint move_number, gchar *buffer, GError **error);
static guint skip_space_or_newline (gchar *buffer, GError **error);
static guint copy_move (gchar *buffer, gchar *target, GError **error);


static void remove_comment (gchar *buffer);
static void remove_comment_parents (gchar *buffer);
static void remove_nag (gchar *buffer);

static gboolean is_end_game (PGN *pgn, gchar *buffer, GError **error);

static Move parse_move (Board *board, gchar *notation);



gboolean
chess_read_pgn (PGN *pgn, const gchar *filename, GError **error)
{
    g_assert (*error == NULL);

	gboolean result = TRUE;
	gchar *buffer;
	gsize length;



	// We just load the whole file into memory.
	// PGN files are typically very short: even the longest game in tournament
	// history is only about 3.5KB.
	// If someone tries to load a 2GB PGN and complains about it crashing, they
	// need to take a long, hard look at themself.
	GFile *file = g_file_new_for_path(filename);
	if (!g_file_load_contents(file, NULL, &buffer, &length, NULL, error)) 
    {
		result = FALSE;
        g_print ("Problem to read\n");
		goto cleanup;
	}


    guint index = 0;
    gint tag_index = 0;
    guint skip = 0;

    gboolean has_fen = FALSE;
    gboolean has_tag = TRUE;
    gchar c;
    while ((c = buffer[index++]) != '1' && c != '\0')
    {
        if (c == '[')
        {
            if (g_str_has_prefix (&buffer[index], "FEN"))
            {
                index += 3;
                skip = skip_space_or_newline (&buffer[index], error);
                index += skip;
                if (buffer[index++] == '"')
                {
                    gint i = 0;
                    for(; i < 89; i++)
                    {
                        if (buffer[index] != '"')
                        {
                            pgn->fen[i] = buffer[index++];
                        }
                        else
                        {
                            break;
                        }
                    }
                    pgn->fen[i] = '\0';  
                    has_fen = TRUE;            
                }
            }

            while (buffer[index++] != ']')
            {

            }
        }
    }
    --index;

    Game *game;
    if (has_fen)
    {
        game = game_new ();
        gboolean success = board_from_fen (game->board, pgn->fen, error);
        if (!success)
        {
            g_print ("Fen: \"%s\" is Invalid!\n", pgn->fen);
            goto cleanup;
        }
    }
    else
    {
        game = game_new_startpos ();
    }
    pgn->game = game;

    remove_comment (&buffer[index]);
    remove_comment_parents (&buffer[index]);
    remove_nag (&buffer[index]);

    gboolean found_end_game = FALSE;
    guint move_number = 1;
    Move move = NULL_MOVE;
    //g_print ("%s\n", &buffer[index]);
    while (index < length && (skip = read_number_dot (move_number, &buffer[index], error)) != 0)
    {
        index += skip;
        skip = skip_space_or_newline (&buffer[index], error);
        index += skip;

        found_end_game = is_end_game (pgn, &buffer[index], error);
        if (found_end_game)
            break;

        gchar white_move[MAX_ALGEBRAIC_NOTATION_LENGTH];
        skip = copy_move (&buffer[index], white_move, error);

        //print_board (game->board);
        //g_print ("white move: %s\n", white_move);
        move = parse_move (game->board, white_move);
        if (move == NULL_MOVE)
        {
            g_set_error (error, 1, 1, 
	 		    "White Move: %d. %s is invalid!\n", move_number, white_move);
            
            goto cleanup;
        }
        game = add_child (game, move);

        
        index += skip;
        skip = skip_space_or_newline (&buffer[index], error);
        index += skip;

        found_end_game = is_end_game (pgn, &buffer[index], error);
        if (found_end_game)
            break;
        
        skip = read_number_dot_dot_dot (move_number, &buffer[index], error);
        index += skip;
        skip = read_number_dot (move_number, &buffer[index], error);
        index += skip;

        skip = skip_space_or_newline (&buffer[index], error);
        index += skip;

        gchar black_move[MAX_ALGEBRAIC_NOTATION_LENGTH];
        skip = copy_move (&buffer[index], black_move, error);
        
        move = parse_move (game->board, black_move);
        if (move == NULL_MOVE)
        {
            g_set_error (error, 1, 1, 
	 		    "Black Move: %d... %s is invalid!\n", move_number, black_move);
            goto cleanup;
        }
        game = add_child (game, move);

        index += skip;
        skip = skip_space_or_newline (&buffer[index], error);
        index += skip;

        found_end_game = is_end_game (pgn, &buffer[index], error);
        if (found_end_game)
            break;

        if (index >= length)
            break;
        
        move_number++;
    }



cleanup:
    g_free(buffer);
	g_object_unref(file);


    if (*error != NULL)
        return FALSE;

    return result;
}

static gboolean 
read_tags (PGN *pgn, const gchar *buffer, GError **error)
{

}

static guint 
read_number_dot (guint move_number, gchar *buffer, GError **error)
{
    if (move_number == 0 || move_number > 9999)
    {
        g_set_error (error, 1, 1, 
	 		"Move number must be 1 to 9999.");		
        return 0;        
    }

    gchar number_str[6];
    gint len = g_snprintf (number_str, 5, "%d.", move_number);

    gint skip = 0;
    while ((SKIPABLE_CHAR(buffer[skip])) && skip < 5)
        skip++;
    if (skip == 5)
    {
        g_set_error (error, 1, 1, 
	 		"Preven buffer overflow.");
        return 0;
    }
    if (g_str_has_prefix (&buffer[skip], number_str))
    {
        return len + skip;
    }
    else
    {		
        return 0;        
    }
}

static guint 
read_number_dot_dot_dot (guint move_number, gchar *buffer, GError **error)
{
    if (move_number == 0 || move_number > 9999)
    {
        g_set_error (error, 1, 1, 
	 		"Move number must be 1 to 9999.");		
        return 0;        
    }
    //g_print ("%s\n", buffer);

    gchar number_str[8];
    gint len = g_snprintf (number_str, 7, "%d...", move_number);

    gint skip = 0;
    while ((SKIPABLE_CHAR(buffer[skip])) && skip < 5)
        skip++;
    if (skip == 7)
    {
        g_set_error (error, 1, 1, 
	 		"Preven buffer overflow.");
        return 0;
    }
    if (g_str_has_prefix (&buffer[skip], number_str))
    {
        return len + skip;
    }
    else
    {		
        return 0;        
    }
}

static guint
skip_space_or_newline (gchar *buffer, GError **error)
{
    guint skip = 0;
    while (SKIPABLE_CHAR(buffer[skip]))
        skip++;
    return skip;
}

static
guint copy_move (gchar *buffer, gchar *target, GError **error)
{
    guint index = 0;
    while ((!SKIPABLE_CHAR(buffer[index])) && index < MAX_ALGEBRAIC_NOTATION_LENGTH)
    {
        target[index] = buffer[index];
        index++;
    }
    if (index == MAX_ALGEBRAIC_NOTATION_LENGTH)
    {
        g_set_error (error, 1, 1, 
	 		"Move must have max length 7.");
        return 0;
    }
    target[index] = '\0';
    return index;
}

static void 
remove_comment (gchar *buffer)
{
    gint i = 0;
    while (buffer[i] != '\0')
    {
        if (buffer[i] == '{')
        {
            while (buffer[i] != '}')
            {
                buffer[i] = ' ';
                i++;
            }
            buffer[i] = ' ';
        }
        i++;
    }
}

static void 
remove_comment_parents (gchar *buffer)
{
    gint i = 0;
    while (buffer[i] != '\0')
    {
        if (buffer[i] == '(')
        {
            remove_comment_parents (&buffer[i + 1]); 
            while (buffer[i] != ')')
            {
                buffer[i] = ' ';
                i++;
            }
            buffer[i] = ' ';
        }
        i++;
    }
}

static void
remove_nag (gchar *buffer)
{
    gint i = 0;
    while (buffer[i] != '\0')
    {
        if (buffer[i] == '$')
        {
            buffer[i++] = ' ';
            while ('0' <= buffer[i] && buffer[i] <= '9')
            {
                buffer[i++] = ' ';
            }
        }
        i++;
    }
}


static gboolean
is_end_game (PGN *pgn, gchar *buffer, GError **error)
{
    if (*buffer == '\0')
    {
        pgn->result = UNKNWON;
        return TRUE;
    }
    if (g_str_has_prefix (buffer, "*"))
    {
        pgn->result = UNKNWON;
        return TRUE;
    }
    else if (g_str_has_prefix (buffer, "1/2-1/2"))
    {
        pgn->result = DRAW;
        return TRUE;        
    }
    else if (g_str_has_prefix (buffer, "1-0"))
    {
        pgn->result = WHITE_WIN;
        return TRUE;        
    }
    else if (g_str_has_prefix (buffer, "0-1"))
    {
        pgn->result = BLACK_WIN;
        return TRUE;        
    }
    return FALSE;
}

static Move
parse_move (Board *board, gchar *notation)
{
    // First we remove 'x's, '+'s, '#'s '!'s, and '?'s, as we don't need them
	//  and they only complicate parsing.
    // max length without 'x#+'s, + 1 for null terminator
	gchar stripped[6] = { '\0', '\0', '\0', '\0', '\0', '\0'};
	size_t stripped_len = 0;

    gboolean has_promotion = FALSE;
    PieceType promotion = QUEEN;

	for (size_t i = 0; notation[i] != '\0'; i++)
	{
        if (notation[i] == '=')
        {
            i++;
            has_promotion = TRUE;

            if (notation[i] == 'Q')
                promotion = QUEEN;
            else if (notation[i] == 'R')
                promotion = ROOK;
            else if (notation[i] == 'N')
                promotion = KNIGHT;
            else if (notation[i] == 'B')
                promotion = BISHOP;
            else
                NULL_MOVE;

            continue;
        }
		if (notation[i] != 'x' && notation[i] != '#' && notation[i] != '+' && 
            notation[i] != '!' && notation[i] != '?')
		{
			stripped[stripped_len++] = notation[i];
		}
	}
	stripped[stripped_len] = '\0';

    if (g_ascii_strcasecmp (stripped, "O-O") == 0) 
    {
        guint y = board->turn == WHITE ? 0 : 7;
        return MOVE(SQUARE(4, y), SQUARE(6, y));
    }
    if (g_ascii_strcasecmp (stripped, "O-O-O") == 0) 
    {
        guint y = board->turn == WHITE ? 0 : 7;
        return MOVE(SQUARE(4, y), SQUARE(2, y));
    }

	size_t i = 0;
	gchar disambig = 0;

	PieceType type;

    // if stripped first character is a lower char, it is a pawn
    // e.g e4 e5
	if (g_ascii_islower(stripped[0])) 
    {
		type = PAWN;
        // Here move is a pawn, if stripped_len is 3, it is a move with capture
        // capture with pawn always have a desambig and it is the first char
        // e.g exd5 after stripped ed5
		if (stripped_len == 3)
	 		disambig = stripped[i++];
            // TODO: Here we can get imediate move, is just get, the desambig 
            // and add +1 for RANK if pawn is black and -1 is White
        
	} 
    else 
    {
        // All others move is not pawn have a char for represent
        // 'R'ook, k'N'ight, 'B'ishop, 'Q'ueen, 'K'ing
		type = PIECE_TYPE (piece_from_char (stripped[i++]));
	}

	// If it's this long, there must be a disambiguation char in there	
	if (stripped_len == 4)
	{		
		disambig = stripped[i++];
	}
	// if stripped_len equals 5, we found ambiguous rank and file
	else if (stripped_len == 5)
	{
	    File start_file = CHAR_FILE (stripped[i++]);
		Rank start_rank = CHAR_RANK (stripped[i++]);
		Square start = SQUARE (start_file, start_rank);

		File end_file = CHAR_FILE (stripped[i++]);
		Rank end_rank = CHAR_RANK (stripped[i++]);

		Square end = SQUARE (end_file, end_rank);

		Move m = MOVE (start, end);
					
		if (legal_move(board, m, TRUE))
			return m;
		else
			return NULL_MOVE;
	}
	
	guint target_file = CHAR_FILE(stripped[i++]);
	guint target_rank = CHAR_RANK(stripped[i++]);

	if (disambig != 0) 
    {
		guint x = 0, y = 0, dx = 0, dy = 0;
		if (disambig >= 'a' && disambig <= 'h') {
			x = CHAR_FILE(disambig);
			dy = 1;
		} else if (disambig >= '1' && disambig <= '8') {
			y = CHAR_RANK(disambig);
			dx = 1;
		} else {
			return NULL_MOVE;
		}

		for (; x < BOARD_SIZE && y < BOARD_SIZE; x += dx, y += dy) {
			Piece p = PIECE_AT(board, x, y);
			if (PIECE_TYPE(p) != type || PLAYER(p) != board->turn)
				continue;

			Move m = MOVE(SQUARE(x, y), SQUARE(target_file, target_rank));
			if (has_promotion)
				m = PROMOTE (m, promotion);

			if (legal_move(board, m, TRUE))
				return m;
		}
		return NULL_MOVE;
	}

	for (guint x = 0; x < BOARD_SIZE; x++) {
		for (guint y = 0; y < BOARD_SIZE; y++) {
			Piece p = PIECE_AT(board, x, y);
			if (PIECE_TYPE(p) != type || PLAYER(p) != board->turn)
				continue;
			
			Move m = MOVE(SQUARE(x, y), SQUARE(target_file, target_rank));
			if (has_promotion)
				m = PROMOTE (m, promotion);
			
			if (legal_move(board, m, TRUE))
				return m;
		}
	}

	return NULL_MOVE;
}
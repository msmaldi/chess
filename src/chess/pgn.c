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

    Move move_buffer[10000];
    gint move_index = 0;

    Board board;

    if (!g_file_get_contents (filename, &buffer, &length, error))
    {   
        result = FALSE;
        goto cleanup;
    }

    guint index = 0;
    gint tag_index = 0;
    guint skip = 0;

    pgn->has_fen = FALSE;
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
                    pgn->has_fen = TRUE;            
                }
            }

            while (buffer[index++] != ']')
            {

            }
        }
    }
    --index;
    //Game *game;
    if (pgn->has_fen)
    {
        //game = game_new ();
        //gboolean success = board_from_fen (game->board, pgn->fen, error);
        
        gboolean success = board_from_fen (&board, pgn->fen, error);
        if (!success)
        {
            // g_print ("Fen: \"%s\" is Invalid!\n", pgn->fen);
            goto cleanup;
        }
    }
    else
    {        
        //game = game_new_startpos ();
        board_from_fen (&board, start_board_fen, error);
    }
    //pgn->game = game;

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
        //move = parse_move (game->board, white_move);
        move = parse_move (&board, white_move);
        if (move == NULL_MOVE)
        {
            g_set_error (error, 1, 1, 
	 		    "White Move: %d. %s is invalid!\n", move_number, white_move);
            
            goto cleanup;
        }
        //game = add_child (game, move);
        perform_move (&board, move);
        move_buffer[move_index++] = move;
        
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
        
        //move = parse_move (game->board, black_move);
        move = parse_move (&board, black_move);
        if (move == NULL_MOVE)
        {
            g_set_error (error, 1, 1, 
	 		    "Black Move: %d... %s is invalid!\n", move_number, black_move);
            goto cleanup;
        }
        //game = add_child (game, move);   
        perform_move (&board, move);
        move_buffer[move_index++] = move;     

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
    move_buffer[move_index++] = NULL_MOVE;
    pgn->move_list = g_memdup (move_buffer, sizeof(Move) * move_index);
    //g_new0 (Move, move_index);
    //g_memmove (pgn->move_list, move_buffer, sizeof(Move) * move_index);

    g_free(buffer);
	//g_object_unref(file);

    if (*error != NULL)
        return FALSE;

    return result;
}

Game*
pgn_to_game (const PGN *pgn)
{
    Game *game;
    if (pgn->has_fen)
        game = game_new_fen (pgn->fen);
    else
        game = game_new_startpos ();

    Move *move_ptr = pgn->move_list;
    Move move = NULL_MOVE;
    Game *g = game;
    while ((move = *move_ptr++) != NULL_MOVE)
        g = add_child (g, move);

    return game;
}

void
pgn_free (PGN *pgn)
{
    if (pgn == NULL)
        return;
    g_free (pgn->move_list);
    g_free (pgn);    
}

static gboolean 
read_tags (PGN *pgn, const gchar *buffer, GError **error)
{
    return TRUE;
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

    gchar number_str[8];
    gint len = g_snprintf (number_str, 7, "%d...", move_number);

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

    if (g_strcmp0 (stripped, "O-O") == 0) 
    {
        guint y = board->turn == WHITE ? 0 : 7;
        return MOVE(SQUARE(4, y), SQUARE(6, y));
    }
    if (g_strcmp0 (stripped, "O-O-O") == 0) 
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
        {
	 		disambig = stripped[i++];
            
            File start_file = CHAR_FILE (disambig);

            File end_file = CHAR_FILE (stripped[i++]);
            Rank end_rank = CHAR_RANK (stripped[i++]);

            Rank start_rank = board->turn == BLACK ? end_rank + 1 : end_rank - 1;

            Move m = MOVE (SQUARE (start_file, start_rank), SQUARE (end_file, end_rank));
            if (has_promotion)
				m = PROMOTE (m, promotion);
            if (legal_move(board, m, TRUE))
			    return m;
		    else
			    return NULL_MOVE;

            // TODO: Here we can get imediate move, is just get, the desambig 
            // and add +1 for RANK if pawn is black and -1 is White
        }   
        // Else, is a move without capture, rank start and end are equals    
        else
        {
            File end_file = CHAR_FILE (stripped[i++]);
            Rank end_rank = CHAR_RANK (stripped[i++]);

            File start_file = end_file;
            if (board->turn == BLACK)
            {
                File start_rank = end_rank + 1;
                Move m = MOVE (SQUARE (start_file, start_rank), SQUARE (end_file, end_rank));
                if (has_promotion)
				    m = PROMOTE (m, promotion);
                if (legal_move(board, m, TRUE))
			        return m;
                else
                {
                    File start_rank = end_rank + 2;
                    Move m = MOVE (SQUARE (start_file, start_rank), SQUARE (end_file, end_rank));
                    if (has_promotion)
                        m = PROMOTE (m, promotion);
                    if (legal_move(board, m, TRUE))
			            return m;
                    else
                        return NULL_MOVE;
                }
            }
            else
            {
                File start_rank = end_rank -1;
                Move m = MOVE (SQUARE (start_file, start_rank), SQUARE (end_file, end_rank));
                if (has_promotion)
				    m = PROMOTE (m, promotion);
                if (legal_move(board, m, TRUE))
			        return m;
                else
                {
                    File start_rank = end_rank - 2;
                    Move m = MOVE (SQUARE (start_file, start_rank), SQUARE (end_file, end_rank));
                    if (has_promotion)
                        m = PROMOTE (m, promotion);
                    if (legal_move(board, m, TRUE))
			            return m;
                    else
                        return NULL_MOVE;
                }
            }
        }
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
			if (legal_move(board, m, TRUE))
				return m;
		}
		return NULL_MOVE;
	}

    Square target = SQUARE(target_file, target_rank);
    for(Square square = SQ_A1; square <= SQ_H8; square++)
    {
        Piece p = PIECE_AT_SQUARE (board, square);
        if (PIECE_TYPE(p) != type || PLAYER(p) != board->turn)
            continue;
        
        Move m = MOVE(square, target);        
        if (legal_move(board, m, TRUE))
            return m;
    }
    

	return NULL_MOVE;
}
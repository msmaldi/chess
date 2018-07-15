#include "chessboard.h"
#include "../notebook/page_home.h"
#include "fen_box.h"
#include <librsvg/rsvg.h>
#include <string.h>
#include "../thirds/chess/moves.h"

RsvgHandle *piece_svgs[2][6];


ChessBoard*
chessboard_new (void)
{
    ChessBoard *result = g_new (ChessBoard, 1);

    result->game = game_new_startpos ();
    result->drag_source = NULL_SQUARE;
    result->flipped = FALSE;

    GtkWidget *frame_outside = gtk_aspect_frame_new (NULL, 0.5, 0.5, 1, FALSE);
    g_object_set (G_OBJECT (frame_outside), "expand", TRUE, NULL);

    GtkWidget *grid = gtk_grid_new ();
    gtk_grid_set_column_homogeneous (GTK_GRID (grid), TRUE);
    gtk_grid_set_row_homogeneous (GTK_GRID (grid), TRUE);

    gtk_container_add (GTK_CONTAINER (frame_outside), GTK_WIDGET (grid));

    GtkWidget *frame_inside = gtk_aspect_frame_new (NULL, 0.5, 0.5, 1, FALSE);

    gtk_grid_attach (GTK_GRID (grid), GTK_WIDGET (frame_inside), 1, 1, 16, 16);

    result->board_area = gtk_drawing_area_new ();

    gtk_container_add (GTK_CONTAINER (frame_inside), GTK_WIDGET (result->board_area));

	
	for(size_t i = 0; i < BOARD_SIZE; i++)
	{
		result->start_label[i] = gtk_label_new ("s");
		result->end_label[i] = gtk_label_new ("e");
		result->left_label[i] = gtk_label_new ("l");
		result->right_label[i] = gtk_label_new ("r");

    	gtk_grid_attach (GTK_GRID (grid), 
			GTK_WIDGET (result->start_label[i]), (2*i)+1, 0, 2, 1);

    	gtk_grid_attach (GTK_GRID (grid), 
			GTK_WIDGET (result->end_label[i]), (2*i)+1, 17, 2, 1);

    	gtk_grid_attach (GTK_GRID (grid), 
			GTK_WIDGET (result->left_label[i]), 0, (2*i)+1, 1, 2);		

    	gtk_grid_attach (GTK_GRID (grid), 
			GTK_WIDGET (result->right_label[i]), 17, (2*i)+1, 1, 2);
	}

	chessboard_update_board_labels (result);

    gtk_widget_add_events(result->board_area,
			GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK |
			GDK_POINTER_MOTION_MASK);
	g_signal_connect(G_OBJECT(result->board_area), "draw",
			         G_CALLBACK(chessboard_draw_callback), result);
	g_signal_connect(G_OBJECT(result->board_area), "button-press-event",
    		         G_CALLBACK(chessboard_mouse_down_callback), result);
	g_signal_connect(G_OBJECT(result->board_area), "button-release-event",
			         G_CALLBACK(chessboard_mouse_up_callback), result);
	g_signal_connect(G_OBJECT(result->board_area), "motion-notify-event",
			         G_CALLBACK(chessboard_mouse_move_callback), result);

    result->board = frame_outside;
    return result;
}

void
chessboard_set_startpos (ChessBoard *chessboard)
{
	free_game (chessboard->game);
	chessboard->game = game_new_startpos ();
	gtk_widget_queue_draw (chessboard->board_area);
}

void
chessboard_set_game (ChessBoard *chessboard, Game *game)
{
	free_game (chessboard->game);
	chessboard->game = game;
	gtk_widget_queue_draw (chessboard->board_area);
}


void
chessboard_flip (ChessBoard *chessboard)
{
	chessboard->flipped = !chessboard->flipped;
	chessboard_update_board_labels (chessboard);
	gtk_widget_queue_draw (chessboard->board_area);
}

void
free_chessboard (ChessBoard *chessboard)
{
    free_game (chessboard->game);
}

void 
chessboard_load_svgs(char *dir, GError **err)
{
	uint len = strlen(dir) + 8; // e.g.: "w_k.svg\0"
	char str[len];
	char piece_letters[] = "pnbrqk";
	char side_letters[] = "bw";

	for (uint i = 0; i < 2; i++) {
		char side = side_letters[i];

		for (uint j = 0; piece_letters[j] != '\0'; j++) {
			sprintf(str, "%s%c_%c.svg", dir, side, piece_letters[j]);

			piece_svgs[i][j] = rsvg_handle_new_from_file(str, err);
			if (*err != NULL)
				return;
		}
	}
}


static guint 
get_square_size_n (GtkWidget *board)
{
	guint width  = gtk_widget_get_allocated_width(board);
	guint height = gtk_widget_get_allocated_height(board);

	guint max_square_width = width / BOARD_SIZE;
	guint max_square_height = height / BOARD_SIZE;
	return max_square_width < max_square_height ?
		max_square_width :
		max_square_height;
}

static void 
draw_piece_n(cairo_t *cr, Piece p, guint size)
{
	RsvgHandle *piece_image =
		piece_svgs[PLAYER(p)][PIECE_TYPE(p) - 1];

	// 0.025 is a bit of a magic number. It's basically just the factor by
	// which the pieces must be scaled in order to fit correctly with the
	// default square size. We then scale that based on how big the squares
	// actually are.
	double scale = 0.025 * size / DEFAULT_SQUARE_SIZE;
	cairo_scale(cr, scale, scale);

	rsvg_handle_render_cairo(piece_image, cr);

	cairo_scale(cr, 1 / scale, 1 / scale);
}


// This should be divisible by 2 so as not to leave a one pixel gap
#define HIGHLIGHT_LINE_WIDTH 4

gboolean
chessboard_draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data)
{
    ChessBoard *chessboard = (ChessBoard*)data;

	// Unless the width/height of the drawing area is exactly a multiple of 8,
	// there's going to be some leftover space. We want to have the board
	// completely centered, so we pad by half the leftover space.
	// We rely on the widget being perfectly square in these calculations, as
	// this is enforced by its containing aspect frame.
	uint square_size = get_square_size_n(widget);
	uint leftover_space =
		gtk_widget_get_allocated_width(widget) - square_size * BOARD_SIZE;
	uint padding = leftover_space / 2;
	cairo_translate(cr, padding, padding);

	// Color light squares one-by-one
	cairo_set_line_width(cr, 0);
	for (uint file = 0; file < BOARD_SIZE; file++) {
		uint x = chessboard->flipped ? BOARD_SIZE - file - 1 : file;

		for (int rank = BOARD_SIZE - 1; rank >= 0; rank--) {
			int y = chessboard->flipped ? BOARD_SIZE - rank - 1 : rank;
			if ((y + x) % 2 == 0) {
				// dark squares
				cairo_set_source_rgb(cr, 0.450980, 0.537255, 0.713725);
				cairo_rectangle(cr, 0, 0, square_size, square_size);
				cairo_fill(cr);
			} else {
				// light squares
				cairo_set_source_rgb(cr, 0.952941, 0.952941, 0.952941);
				cairo_rectangle(cr, 0, 0, square_size, square_size);
				cairo_fill(cr);
			}

			// Highlight the source and target squares of the last move
			Move last_move = chessboard->game->move;
			Square s = SQUARE(x, y);
			if ( last_move != NULL_MOVE &&
					(s == START_SQUARE (last_move) || s == END_SQUARE (last_move))) {
				cairo_set_source_rgb(cr, 0.225, 0.26, 0.3505);
				cairo_set_line_width(cr, HIGHLIGHT_LINE_WIDTH);
				cairo_translate(cr, HIGHLIGHT_LINE_WIDTH / 2, HIGHLIGHT_LINE_WIDTH / 2);
				cairo_rectangle(cr, 0, 0, square_size - HIGHLIGHT_LINE_WIDTH,
						square_size - HIGHLIGHT_LINE_WIDTH);
				cairo_stroke(cr);

				cairo_set_line_width(cr, 1);
				cairo_translate(cr, -HIGHLIGHT_LINE_WIDTH / 2, -HIGHLIGHT_LINE_WIDTH / 2);
			}

			// Draw the piece, if there is one
			Piece p;
			if ((p = PIECE_AT(chessboard->game->board, x, y)) != EMPTY &&
					(chessboard->drag_source == NULL_SQUARE || SQUARE(x, y) != chessboard->drag_source)) {
				draw_piece_n(cr, p, square_size);
			}

			cairo_translate(cr, 0, square_size);
		}

		cairo_translate(cr, square_size, -square_size * BOARD_SIZE);
	}

	if (chessboard->drag_source != NULL_SQUARE) 
    {
		cairo_identity_matrix(cr);
		cairo_translate(cr, padding + chessboard->mouse_x - square_size / 2,
				padding + chessboard->mouse_y - square_size / 2);
		draw_piece_n(cr, PIECE_AT_SQUARE(chessboard->game->board, chessboard->drag_source), square_size);
	}

	return FALSE;
}


static Square 
chessboard_coords_to_square(GtkWidget *drawing_area, uint x, uint y, gboolean flipped)
{
	uint square_size = get_square_size_n(drawing_area);
	uint board_x = x / square_size;
	uint board_y = y / square_size;
	if (!flipped) {
		board_y = BOARD_SIZE - 1 - board_y;
	} else {
		board_x = BOARD_SIZE - 1 - board_x;
	}

	return SQUARE(board_x, board_y);
}

// Redraw if we're dragging a piece
gboolean 
chessboard_mouse_move_callback (GtkWidget *widget, 
								GdkEvent  *event,
								gpointer   data)
{
	ChessBoard *chessboard = (ChessBoard*)data;
	GdkEventButton *e = (GdkEventButton *)event;

	// e->x and e->y are relative to the window, but we want coordinates
	// relative to the chessboard drawing area.
	// So we figure out where it is, and add that on.
	gint wx, wy;
	gtk_widget_translate_coordinates(widget, gtk_widget_get_toplevel(widget),
			0, 0, &wx, &wy);

	chessboard->mouse_x = e->x + wx;
	chessboard->mouse_y = e->y + wy;

	if (chessboard->drag_source != NULL_SQUARE)
		gtk_widget_queue_draw(widget);

	return FALSE;
}

// Start dragging a piece if the mouse is over one.
gboolean
chessboard_mouse_down_callback (GtkWidget *widget, 
								GdkEvent  *event,
								gpointer   data)
{
	ChessBoard *chessboard = (ChessBoard*)data;
	GdkEventButton *e = (GdkEventButton *)event;

	if (e->button != 1)
		return FALSE;

	Square clicked_square = 
		chessboard_coords_to_square(widget, e->x, e->y, chessboard->flipped);
	if (PIECE_AT_SQUARE(chessboard->game->board, clicked_square) != EMPTY) {
		chessboard->drag_source = clicked_square;
	}

	return FALSE;
}

// Try to move a piece if we're currently dragging one
gboolean 
chessboard_mouse_up_callback (GtkWidget *widget, 
							  GdkEvent  *event,
							  gpointer   data)
{
	ChessBoard *chessboard = (ChessBoard*)data;	
	GdkEventButton *e = (GdkEventButton *)event;

	if (e->button != 1 || chessboard->drag_source == NULL_SQUARE)
		return FALSE;

	Square drag_target = 
		chessboard_coords_to_square(widget, e->x, e->y, chessboard->flipped);

	// open widget to request a piecie to promote
	

	//
	
	Move m = MOVE(chessboard->drag_source, drag_target);
	m = MOVE_PROMOTE (m, QUEEN);
	if (legal_move(chessboard->game->board, m, true)) {
		char notation[MAX_ALGEBRAIC_NOTATION_LENGTH];
		algebraic_notation_for(chessboard->game->board, m, notation);

		// TODO: Stop printing this when we have a proper move list GUI
		if (chessboard->game->board->turn == WHITE)
			printf("%d. %s", chessboard->game->board->move_number, notation);
		else
			printf(" %s\n", notation);

		chessboard->game = add_child(chessboard->game, m);

		set_button_sensitivity ();
    	update_fen_input ();
	}

	chessboard->drag_source = NULL_SQUARE;
	gtk_widget_queue_draw(widget);

	return FALSE;
}

static gchar *files[BOARD_SIZE] = { "a", "b", "c", "d", "e", "f", "g", "h" };
static gchar *ranks[BOARD_SIZE] = { "1", "2", "3", "4", "5", "6", "7", "8" };

void chessboard_update_board_labels (ChessBoard *chessboard)
{
	if (!chessboard->flipped)
	{
		for(size_t i = 0, e = BOARD_SIZE - 1; i < BOARD_SIZE; i++, e--)
		{
			gtk_label_set_label (GTK_LABEL (chessboard->start_label[i]), files[i]);
			gtk_label_set_label (GTK_LABEL (chessboard->end_label[i]), files[i]);
			gtk_label_set_label (GTK_LABEL (chessboard->left_label[i]), ranks[e]);
			gtk_label_set_label (GTK_LABEL (chessboard->right_label[i]), ranks[e]);
		}
	}
	else
	{
		for(size_t i = 0, e = BOARD_SIZE - 1; i < BOARD_SIZE; i++, e--)
		{
			gtk_label_set_label (GTK_LABEL (chessboard->start_label[i]), files[e]);
			gtk_label_set_label (GTK_LABEL (chessboard->end_label[i]), files[e]);
			gtk_label_set_label (GTK_LABEL (chessboard->left_label[i]), ranks[i]);
			gtk_label_set_label (GTK_LABEL (chessboard->right_label[i]), ranks[i]);
		}
	}
}
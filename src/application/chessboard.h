#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <gtk/gtk.h>
#include "../thirds/chess/game.h"
#include "../thirds/chess/board.h"
#include "../thirds/board_display.h"

typedef struct _ChessBoard
{
    Game *game;
    GtkWidget *board;
    GtkWidget *board_area;
    gboolean flipped;

    Square drag_source;
    gint mouse_x;
    gint mouse_y;
} ChessBoard;


ChessBoard      *chessboard_new         (void);
void             chessboard_new_game    (ChessBoard *chessboard);
void             chessboard_flip        (ChessBoard *chessboard);
void             free_chessboard        (ChessBoard *chessboard);



gboolean         chessboard_draw_callback       (GtkWidget *widget, cairo_t *cr, gpointer data);
void             chessboard_load_svgs           (char *dir, GError **err);
gboolean         chessboard_mouse_up_callback   (GtkWidget *widget, 
                    							 GdkEvent  *event,
							                     gpointer   data);
gboolean
chessboard_mouse_down_callback (GtkWidget *widget, 
								GdkEvent  *event,
								gpointer   data);
gboolean 
chessboard_mouse_move_callback (GtkWidget *widget, 
								GdkEvent  *event,
								gpointer   data);

#endif
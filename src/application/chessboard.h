#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <gtk/gtk.h>
#include "../chess/game.h"
#include "../chess/board.h"

#define DEFAULT_SQUARE_SIZE 50

typedef struct _ChessBoard
{
    Game *game;
    GtkWidget *board;
    GtkWidget *board_area;
    GtkWidget *start_label[BOARD_SIZE];
    GtkWidget *end_label[BOARD_SIZE];
    GtkWidget *right_label[BOARD_SIZE];
    GtkWidget *left_label[BOARD_SIZE];
    
    GtkWidget *prev_move_button;
    GtkWidget *next_move_button;

    gboolean flipped;

    Square drag_source;
    gint mouse_x;
    gint mouse_y;
} ChessBoard;


ChessBoard    *chessboard_new            (void);
void           chessboard_new_game       (ChessBoard *chessboard);
void           chessboard_set_startpos   (ChessBoard *chessboard);
//void           chessboard_set_fen        (ChessBoard *chessboard, 
//                                          gchar      *fen,
//                                          GError     **error);
//void            chessboard_set_pgn       (ChessBoard *chessboard,
//                                          PGN        pgn,
//                                          GError     **error);
void           chessboard_set_game       (ChessBoard *chessboard, Game *game);


void           chessboard_flip           (ChessBoard *chessboard);
void           free_chessboard           (ChessBoard *chessboard);



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

void chessboard_update_board_labels (ChessBoard *chessboard);

#endif
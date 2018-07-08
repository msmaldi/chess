#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include <gtk/gtk.h>

void         set_margin   (GtkWidget *widget, gint margin);
GtkWidget*   notebook_new ();


extern  GtkWidget   *home_button_open_pgn;
extern  GtkWidget   *home_button_pasteposition;
extern  GtkWidget   *home_button_copygame;
extern  GtkWidget   *home_button_copyposition;
extern  GtkWidget   *home_box_clipboard_label;

extern  GtkWidget   *home_button_newgame;
extern  GtkWidget   *home_button_undo;
extern  GtkWidget   *home_button_redo;
extern  GtkWidget   *home_button_enginemove;
extern  GtkWidget   *home_box_game_label;

extern  GtkWidget   *home_button_flipboard;
extern  GtkWidget   *home_button_base;
extern  GtkWidget   *home_button_piece;
extern  GtkWidget   *home_button_blindness;
extern  GtkWidget   *home_box_board_label;

#endif
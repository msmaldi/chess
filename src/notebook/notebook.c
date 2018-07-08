#include <gtk/gtk.h>
#include "notebook.h"
#include "page_home.h"
#include "page_engine.h"

GtkWidget   *home_button_open_pgn;
GtkWidget   *home_button_pasteposition;
GtkWidget   *home_button_copygame;
GtkWidget   *home_button_copyposition;
GtkWidget   *home_box_clipboard_label;

GtkWidget   *home_button_newgame;
GtkWidget   *home_button_undo;
GtkWidget   *home_button_redo;
GtkWidget   *home_button_enginemove;
GtkWidget   *home_box_game_label;

GtkWidget   *home_button_flipboard;
GtkWidget   *home_button_base;
GtkWidget   *home_button_piece;
GtkWidget   *home_button_blindness;
GtkWidget   *home_box_board_label;


void set_margin (GtkWidget *widget, gint margin)
{
    gtk_widget_set_margin_start (widget, margin);
    gtk_widget_set_margin_end (widget, margin);
    gtk_widget_set_margin_top (widget, margin);
    gtk_widget_set_margin_bottom (widget, margin);
}

GtkWidget*
notebook_new()
{
    GtkWidget* notebook = gtk_notebook_new ();

    GtkWidget *home_label = gtk_label_new ("Home");
    GtkWidget *home_page = page_home_box ();
    gtk_notebook_insert_page (GTK_NOTEBOOK (notebook), home_page, home_label, 0);

    GtkWidget *engine_label = gtk_label_new ("Engine");
    GtkWidget *engine_page = page_engine_box ();
    gtk_notebook_insert_page (GTK_NOTEBOOK (notebook), engine_page, engine_label, 1);

    return notebook;
}
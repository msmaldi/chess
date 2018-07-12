#include "application.h"
#include "../thirds/chess/board.h"
#include "../thirds/chess/game.h"
#include "../thirds/chess/pgn.h"
#include "../notebook/page_home.h"

ChessBoard *chessboard;

GtkWidget *window;
GtkWidget *notebook;
GtkWidget *box;
GtkWidget *mainbox;
GtkWidget *rightbox;


void
initialize_widgets (void)
{
    chessboard = chessboard_new ();
	// Main Window
	//
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width (GTK_CONTAINER (window), 0);
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
    gtk_window_set_default_size (GTK_WINDOW (window), 1280, 720);

    // Main Box
	//
	box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);

    // Notebook
    //
    notebook = notebook_new ();
    
	mainbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    rightbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  
    initialize_fen_box_widget ();
}

GtkWidget* gtk_label_new_aligned (const gchar *str)
{
    GtkWidget *label = gtk_label_new (str);
    return label;
}

void
pack_widgets_and_show (void)
{
    gtk_container_add (GTK_CONTAINER (box), GTK_WIDGET (notebook));
    gtk_container_add (GTK_CONTAINER (box), GTK_WIDGET (mainbox));

    gtk_container_add (GTK_CONTAINER (mainbox), GTK_WIDGET (chessboard->board));

    gtk_container_add (GTK_CONTAINER (rightbox), GTK_WIDGET (fen_box));
    gtk_container_add (GTK_CONTAINER (mainbox), GTK_WIDGET (rightbox));

    gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET (box));

	gtk_widget_show_all (window);

    set_button_sensitivity ();
}

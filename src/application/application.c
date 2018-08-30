#include "application.h"
#include "../chess/board.h"
#include "../chess/game.h"
#include "../chess/pgn.h"
#include "../notebook/page_home.h"
#include "../engine/uci_engine.h"

ChessBoard *chessboard;

GtkWidget *window;
GtkWidget *notebook;
GtkWidget *box;
GtkWidget *mainbox;
GtkWidget *rightbox;
uci_engine *engine;

gpointer
on_getmove (gpointer data)
{
    if (legal_move(chessboard->game->board, engine->bestmove, TRUE)) 
	{
		gchar notation[MAX_ALGEBRAIC_NOTATION_LENGTH];
		algebraic_notation_for(chessboard->game->board, engine->bestmove, notation);

		// TODO: Stop printing this when we have a proper move list GUI
		if (chessboard->game->board->turn == WHITE)
			g_print("%d. %s", chessboard->game->board->move_number, notation);
		else
			g_print(" %s\n", notation);

		chessboard->game = add_child(chessboard->game, engine->bestmove);

		set_button_sensitivity ();
    	update_fen_input ();
	    gtk_widget_queue_draw(chessboard->board_area);
	}
}

void
initialize_widgets (void)
{
    engine = uci_engine_new ("stockfish");
    engine->on_getmove_callback = &on_getmove;
    GError *engine_error;
    init_engine (engine, &engine_error);

    chessboard = chessboard_new ();
	// Main Window
	//
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width (GTK_CONTAINER (window), 0);
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
    gtk_window_maximize (GTK_WINDOW (window));
    //gtk_window_set_default_size (GTK_WINDOW (window), 1280, 900);

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

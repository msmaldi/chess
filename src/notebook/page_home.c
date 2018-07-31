#include <gtk/gtk.h>
#include "notebook.h"
#include "../engine/uci_engine.h"
#include "../application/application.h"
#include "../chess/pgn.h"

void set_button_sensitivity ();
void open_pgn_callback (GtkWidget *widget, gpointer data);
void new_game_callback (GtkWidget *widget, gpointer data);
void go_next_button_click_callback (GtkWidget *widget, gpointer data);
void go_back_button_click_callback (GtkWidget *widget, gpointer data);
void flip_button_click_callback (GtkWidget *widget, gpointer data);
void copy_position_click_callback (GtkWidget *widget, gpointer data);
void paste_position_click_callback (GtkWidget *widget, gpointer data);

GtkWidget*
page_home_box ()
{
    GtkWidget *home_box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_set_border_width (GTK_CONTAINER (home_box), 10);

    GtkWidget *home_box_clipboard_grid = gtk_grid_new ();

    home_button_open_pgn = gtk_button_new_with_label ("Open PGN");
    home_button_pasteposition = gtk_button_new_with_label ("Paste Position");
    home_button_copygame = gtk_button_new_with_label ("Copy Game");
    home_button_copyposition = gtk_button_new_with_label ("Copy Position");
    home_box_clipboard_label = gtk_label_new ("Clipboard");

    set_margin (home_button_open_pgn, 2);
    set_margin (home_button_pasteposition, 2);
    set_margin (home_button_copygame, 2);
    set_margin (home_button_copyposition, 2);
    set_margin (home_box_clipboard_label, 2);

    gtk_grid_attach (GTK_GRID (home_box_clipboard_grid), home_button_open_pgn, 0, 0, 1, 3);
    gtk_grid_attach (GTK_GRID (home_box_clipboard_grid), home_button_pasteposition, 1, 0, 1, 1);
    gtk_grid_attach (GTK_GRID (home_box_clipboard_grid), home_button_copygame, 1, 1, 1, 1);
    gtk_grid_attach (GTK_GRID (home_box_clipboard_grid), home_button_copyposition, 1, 2, 1, 1);
    gtk_grid_attach (GTK_GRID (home_box_clipboard_grid), home_box_clipboard_label, 0, 3, 2, 1);

    gtk_container_add (GTK_CONTAINER (home_box), GTK_WIDGET (home_box_clipboard_grid));


    GtkWidget *home_box_game_grid = gtk_grid_new ();

    home_button_newgame = gtk_button_new_with_label ("New Game");
    home_button_undo = gtk_button_new_with_label ("Undo");
    home_button_redo = gtk_button_new_with_label ("Redo");
    home_button_enginemove = gtk_button_new_with_label ("Engine Move");
    home_box_game_label = gtk_label_new ("Game");

    set_margin (home_button_newgame, 2);
    set_margin (home_button_undo, 2);
    set_margin (home_button_redo, 2);
    set_margin (home_button_enginemove, 2);
    set_margin (home_box_game_label, 2);

    gtk_grid_attach (GTK_GRID (home_box_game_grid), home_button_newgame, 0, 0, 1, 3);
    gtk_grid_attach (GTK_GRID (home_box_game_grid), home_button_undo, 1, 0, 1, 1);
    gtk_grid_attach (GTK_GRID (home_box_game_grid), home_button_redo, 1, 1, 1, 1);
    gtk_grid_attach (GTK_GRID (home_box_game_grid), home_button_enginemove, 1, 2, 1, 1);
    gtk_grid_attach (GTK_GRID (home_box_game_grid), home_box_game_label, 0, 3, 2, 1);

    gtk_container_add (GTK_CONTAINER (home_box), GTK_WIDGET (home_box_game_grid));

    GtkWidget *home_box_board_grid = gtk_grid_new ();

    home_button_flipboard = gtk_button_new_with_label ("Flip Board");
    home_button_base = gtk_button_new_with_label ("Color Board");
    home_button_piece = gtk_button_new_with_label ("Color Piece");
    home_button_blindness = gtk_button_new_with_label ("Blindness");
    home_box_board_label = gtk_label_new ("Board");

    set_margin (home_button_flipboard, 2);
    set_margin (home_button_base, 2);
    set_margin (home_button_piece, 2);
    set_margin (home_button_blindness, 2);

    gtk_grid_attach (GTK_GRID (home_box_board_grid), home_button_flipboard, 0, 0, 1, 3);
    gtk_grid_attach (GTK_GRID (home_box_board_grid), home_button_base, 1, 0, 1, 1);
    gtk_grid_attach (GTK_GRID (home_box_board_grid), home_button_piece, 1, 1, 1, 1);
    gtk_grid_attach (GTK_GRID (home_box_board_grid), home_button_blindness, 1, 2, 1, 1);
    gtk_grid_attach (GTK_GRID (home_box_board_grid), home_box_board_label, 0, 3, 2, 1);

    gtk_container_add (GTK_CONTAINER (home_box), GTK_WIDGET (home_box_board_grid));

    g_signal_connect(G_OBJECT(home_button_newgame), "clicked",
		    G_CALLBACK(new_game_callback), chessboard);

    g_signal_connect(G_OBJECT(home_button_open_pgn), "clicked",
		    G_CALLBACK(open_pgn_callback), window);

    g_signal_connect(G_OBJECT(home_button_redo), "clicked",
	    G_CALLBACK(go_next_button_click_callback), chessboard);
    
    g_signal_connect(G_OBJECT(home_button_undo), "clicked",
	    G_CALLBACK(go_back_button_click_callback), chessboard);

    //g_signal_connect(G_OBJECT(home_button_enginemove), "clicked",
	//	    G_CALLBACK(cb_execute), chessboard);

    g_signal_connect(G_OBJECT(home_button_flipboard), "clicked",
		G_CALLBACK(flip_button_click_callback), NULL);

    g_signal_connect(G_OBJECT(home_button_copyposition), "clicked",
		G_CALLBACK(copy_position_click_callback), fen_input);

    g_signal_connect(G_OBJECT(home_button_pasteposition), "clicked",
		G_CALLBACK(paste_position_click_callback), fen_input);

        

    return home_box;
}

void
set_button_sensitivity ()
{
	gtk_widget_set_sensitive (home_button_undo, chessboard->game->parent != NULL);
	gtk_widget_set_sensitive (home_button_redo, has_children(chessboard->game));
}

void
open_pgn_callback(GtkWidget *widget, gpointer data)
{
	GtkWindow *parent = (GtkWindow*)data;

	GtkWidget *dialog = gtk_file_chooser_dialog_new("Open PGN", GTK_WINDOW (parent),
			GTK_FILE_CHOOSER_ACTION_OPEN,
			"Cancel", GTK_RESPONSE_CANCEL,
			"Open", GTK_RESPONSE_ACCEPT,
			NULL);

	GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);

	gtk_file_chooser_set_local_only(chooser, TRUE);

	GtkFileFilter *just_pgns = gtk_file_filter_new();
	gtk_file_filter_set_name(just_pgns, "PGN files");
	gtk_file_filter_add_pattern(just_pgns, "*.pgn");
	gtk_file_chooser_add_filter(chooser, just_pgns);

	int result = gtk_dialog_run(GTK_DIALOG(dialog));
	if (result == GTK_RESPONSE_ACCEPT) {
		gchar *filename = gtk_file_chooser_get_filename(chooser);
		PGN *pgn = g_new0 (PGN, 1);
		GError *error = NULL;
		gboolean success = chess_read_pgn (pgn, filename, &error);

		g_free(filename);

		if (!success) {
			// TODO: Display an error dialog
			puts("Failed to read the PGN");
		}
        else
        {
            
		    chessboard_set_game (chessboard, pgn_to_game(pgn));
        }
        pgn_free (pgn);

		set_button_sensitivity();
	}

	gtk_widget_destroy(dialog);

    update_fen_input ();
}

void
new_game_callback (GtkWidget *widget, gpointer data)
{
    free_game (chessboard->game);
	chessboard->game = game_new_startpos ();

	gtk_widget_queue_draw(chessboard->board_area);

	set_button_sensitivity();
    update_fen_input ();
}

void
go_next_button_click_callback (GtkWidget *widget, gpointer data)
{
	chessboard->game = first_child (chessboard->game);
	gtk_widget_queue_draw (chessboard->board_area);

	set_button_sensitivity();
    update_fen_input ();
}

void
go_back_button_click_callback (GtkWidget *widget, gpointer data)
{
	chessboard->game = chessboard->game->parent;
	gtk_widget_queue_draw (chessboard->board_area);

	set_button_sensitivity();
    update_fen_input ();
}

void
flip_button_click_callback (GtkWidget *widget, gpointer data)
{
	chessboard_flip (chessboard);
}

void
paste_received (GtkClipboard *clipboard,
                const gchar  *text,
                gpointer      user_data)
{
    Board *board = g_new (Board, 1);
    g_print ("Pasted %s\n", text);

    GError *error = NULL;
    gboolean is_success = board_from_fen (board, text, &error);
    if (is_success)
    {
        g_print ("OK\n");
        free_game(chessboard->game);
        chessboard->game = game_new ();
        copy_board (chessboard->game->board, board);

        gtk_widget_queue_draw (chessboard->board_area);

        set_button_sensitivity ();
        update_fen_input ();
    }
    else
    {
        g_print ("%s\n", error->message);     
    }

    g_free (board);

}

void
paste_position_click_callback (GtkWidget *widget, gpointer data)
{
    GtkClipboard *clipboard = gtk_widget_get_clipboard (GTK_WIDGET (fen_input),
                                        GDK_SELECTION_CLIPBOARD);

    gtk_clipboard_request_text (clipboard, paste_received, NULL);

}

void
copy_position_click_callback (GtkWidget *widget, gpointer data)
{
    GtkClipboard *clipboard = 
        gtk_widget_get_clipboard (GTK_WIDGET (fen_input), 
                                  GDK_SELECTION_CLIPBOARD);
    gtk_clipboard_set_text (clipboard, 
                            gtk_entry_get_text (GTK_ENTRY (fen_input)), 100);
    g_print ("Coppied FEN: %s\n", fen);
}
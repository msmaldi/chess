#include <gtk/gtk.h>
#include "../engine/uci_engine.h"
#include "notebook.h"
#include "../thirds/board_display.h"
#include "../application/application.h"

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
		    G_CALLBACK(new_game_callback), NULL);

    g_signal_connect(G_OBJECT(home_button_open_pgn), "clicked",
		    G_CALLBACK(open_pgn_callback), window);

    g_signal_connect(G_OBJECT(home_button_redo), "clicked",
		    G_CALLBACK(go_next_button_click_callback), NULL);
    
    g_signal_connect(G_OBJECT(home_button_undo), "clicked",
		    G_CALLBACK(go_back_button_click_callback), NULL);

    g_signal_connect(G_OBJECT(home_button_enginemove), "clicked",
		    G_CALLBACK(cb_execute), NULL);

    g_signal_connect(G_OBJECT(home_button_flipboard), "clicked",
		    G_CALLBACK(flip_button_click_callback), NULL);


    return home_box;
}
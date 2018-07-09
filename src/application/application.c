#include "application.h"
#include "../thirds/chess/board.h"
#include "../thirds/chess/game.h"
#include "../thirds/chess/pgn.h"
#include "../thirds/board_display.h"

GtkWidget *window;
GtkWidget *notebook;
GtkWidget *box;
GtkWidget *frame;
GtkWidget *mainbox;
GtkWidget *rightbox;

GtkWidget *aspect_frame;
GtkWidget *grid_frame;

void
initialize_widgets (void)
{
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


    aspect_frame = gtk_aspect_frame_new (NULL, 0.5, 0.5, 1, false);

    grid_frame = gtk_grid_new ();

    frame = gtk_aspect_frame_new (NULL, 0.5, 0.5, 1, false);
    
	mainbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    rightbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    // Board
    //
    board_display = gtk_drawing_area_new ();
    
    g_object_set (G_OBJECT (mainbox),  "expand", TRUE, NULL);  
    g_object_set (G_OBJECT (frame),  "expand", TRUE, NULL);   
    
    gtk_widget_add_events(board_display,
			GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK |
			GDK_POINTER_MOTION_MASK);
	g_signal_connect(G_OBJECT(board_display), "draw",
			         G_CALLBACK(board_draw_callback), NULL);
	g_signal_connect(G_OBJECT(board_display), "button-press-event",
    		         G_CALLBACK(board_mouse_down_callback), NULL);
	g_signal_connect(G_OBJECT(board_display), "button-release-event",
			         G_CALLBACK(board_mouse_up_callback), NULL);
	g_signal_connect(G_OBJECT(board_display), "motion-notify-event",
			         G_CALLBACK(board_mouse_move_callback), NULL);

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

    gtk_grid_set_column_homogeneous (GTK_GRID (grid_frame), TRUE);
    gtk_grid_set_row_homogeneous (GTK_GRID (grid_frame), TRUE);

    gtk_container_add (GTK_CONTAINER (aspect_frame), GTK_WIDGET (grid_frame));
    gtk_container_add (GTK_CONTAINER (mainbox), GTK_WIDGET (aspect_frame));


    ChessBoard *chessboard = chessboard_new ();
    gtk_container_add (GTK_CONTAINER (rightbox), GTK_WIDGET (fen_box));
    chessboard_flip (chessboard);
    gtk_container_add (GTK_CONTAINER (rightbox), GTK_WIDGET (chessboard->board));

    gtk_container_add (GTK_CONTAINER (mainbox), GTK_WIDGET (rightbox));

    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (frame), 1, 1, 16, 16);
           
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("8")), 0, 1, 1, 2);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("7")), 0, 3, 1, 2);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("6")), 0, 5, 1, 2);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("5")), 0, 7, 1, 2);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("4")), 0, 9, 1, 2);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("3")), 0, 11, 1, 2);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("2")), 0, 13, 1, 2);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("1")), 0, 15, 1, 2);

    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("8")), 17, 1, 1, 2);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("7")), 17, 3, 1, 2);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("6")), 17, 5, 1, 2);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("5")), 17, 7, 1, 2);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("4")), 17, 9, 1, 2);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("3")), 17, 11, 1, 2);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("2")), 17, 13, 1, 2);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("1")), 17, 15, 1, 2);

    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("a")), 1, 17, 2, 1);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("b")), 3, 17, 2, 1);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("c")), 5, 17, 2, 1);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("d")), 7, 17, 2, 1);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("e")), 9, 17, 2, 1);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("f")), 11, 17, 2, 1);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("g")), 13, 17, 2, 1);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("h")), 15, 17, 2, 1);

    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("a")), 1, 0, 2, 1);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("b")), 3, 0, 2, 1);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("c")), 5, 0, 2, 1);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("d")), 7, 0, 2, 1);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("e")), 9, 0, 2, 1);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("f")), 11, 0, 2, 1);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("g")), 13, 0, 2, 1);
    gtk_grid_attach (GTK_GRID (grid_frame), GTK_WIDGET (gtk_label_new_aligned ("h")), 15, 0, 2, 1);

    gtk_container_add (GTK_CONTAINER (frame), GTK_WIDGET (board_display));

    gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET (box));

	gtk_widget_show_all (window);

    set_button_sensitivity ();
}

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

    frame = gtk_aspect_frame_new (NULL, 0.5, 0.5, 1, true);
    
	mainbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    rightbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    // Board
    //
    board_display = gtk_drawing_area_new ();
    
    g_object_set (G_OBJECT (mainbox),  "expand", true, NULL);  
    g_object_set (G_OBJECT (frame),  "expand", true, NULL);  
    g_object_set (G_OBJECT (rightbox),  "expand", true, NULL);  
    
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

void
pack_widgets_and_show (void)
{
    gtk_container_add (GTK_CONTAINER (box), GTK_WIDGET (notebook));
    gtk_container_add (GTK_CONTAINER (box), GTK_WIDGET (mainbox));
    gtk_container_add (GTK_CONTAINER (mainbox), GTK_WIDGET (frame));
    gtk_container_add (GTK_CONTAINER (rightbox), GTK_WIDGET (fen_box));
    gtk_container_add (GTK_CONTAINER (mainbox), GTK_WIDGET (rightbox));
    gtk_container_add (GTK_CONTAINER (frame), GTK_WIDGET (board_display));
    gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET (box));

	gtk_widget_show_all (window);

    set_button_sensitivity ();
}

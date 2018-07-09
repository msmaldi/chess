#include <gtk/gtk.h>
#include <stdlib.h>
#include <assert.h>

#include "application/application.h"

char *piece_svgs_path = "pieces/merida/";

int main(int argc, char *argv[])
{
    gtk_init (&argc, &argv);

    GError *err = NULL;
    chessboard_load_svgs (piece_svgs_path, &err);
	if (err != NULL) {
		printf("Error loading SVGs:\n%s\n", err->message);
		return 1;
	}

    initialize_widgets ();
    pack_widgets_and_show ();

    gtk_main ();

    return 0;
}
#include <gtk/gtk.h>
#include <stdlib.h>
#include <assert.h>

#include "application/application.h"

char *piece_svgs = "pieces/merida/";

int main(int argc, char *argv[])
{
    gtk_init (&argc, &argv);

    GError *err = NULL;
	load_svgs(piece_svgs, &err);
	if (err != NULL) {
		printf("Error loading SVGs:\n%s\n", err->message);
		return 1;
	}

    current_game = new_game();
    current_game->board = malloc(sizeof(Board));

    bool success = from_fen(current_game->board, start_board_fen);
    // This is a fixed string, it should never fail to be parsed
    assert(success);

    initialize_widgets ();
    pack_widgets_and_show ();

    gtk_main ();

    return 0;
}
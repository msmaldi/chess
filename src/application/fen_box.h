#ifndef FEN_BOX_H
#define FEN_BOX_H

#include <gtk/gtk.h>
#include "../notebook/notebook.h"
#include "../chess/board.h"

extern  GtkWidget    *fen_box;
extern  GtkWidget    *fen_input;
extern  gchar         fen[90];

void    initialize_fen_box_widget   (void);
void    update_fen_input            (void);

#endif
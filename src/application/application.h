#ifndef APPLICATION_H
#define APPLICATION_H

#include <gtk/gtk.h>
#include "chessboard.h"
#include "fen_box.h"
#include "engine_box.h"
#include "../engine/uci_engine.h"

extern  ChessBoard  *chessboard;
extern  GtkWidget   *window;
extern  GtkWidget   *notebook;
extern  GtkWidget   *box;
extern  GtkWidget   *frame;
extern  GtkWidget   *mainbox;
extern  GtkWidget   *rightbox;
extern  UciEngine   *engine;

void    initialize_widgets      (void);
void    pack_widgets_and_show   (void);

#endif
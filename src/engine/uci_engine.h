#ifndef UCI_ENGINE_H
#define UCI_ENGINE_H

#include <glib.h>
#include "../chess/chess.h"

gboolean        cb_execute          (GtkWidget *widget, gpointer *user_data);

typedef struct
{
    gchar      *engine_full_path;
    GPid        pid;
    gint        in;
    gint        out;
    gint        err;
    GIOChannel *in_ch;
    GIOChannel *out_ch;
    GIOChannel *err_ch;
    gboolean    ret;
    gboolean    is_initialized;
    gchar      *engine_description;
    gboolean    lock;
    gboolean    thinking;
    Move        bestmove;
    Move        ponder;
    gpointer   (*on_getmove_callback) (gpointer data);
    GMutex      mutex;
} UciEngine;

UciEngine*      uci_engine_new         (gchar *engine);

void            init_engine            (UciEngine *engine, GError **error);
void            uci_engine_setup_fen   (UciEngine *engine, gchar *fen);
void            uci_engine_requestmove (UciEngine *engine);

void            uci_engine_free        (UciEngine *engine);


#endif
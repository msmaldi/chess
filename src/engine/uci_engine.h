#ifndef UCI_ENGINE_H
#define UCI_ENGINE_H

#include <glib.h>
#include "../chess/chess.h"

gboolean        cb_execute          (GtkWidget *widget, gpointer *user_data);

typedef struct _uci_engine
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
} uci_engine;

uci_engine*     uci_engine_new      (gchar *engine);

void            init_engine            (uci_engine *engine, GError **error);
void            uci_engine_init_fen    (uci_engine *engine, gchar *fen);
void            uci_engine_requestmove (uci_engine *engine);

void            uci_engine_free        (uci_engine *engine);

typedef enum _uci_option_type
{
    SPIN,
    CHECK,
    COMBO
} uci_option_type;



typedef struct _uci_option
{
    GString *name;
    uci_option_type type;
    gpointer dfault;

} uci_option;

typedef struct _uci_options
{

} uci_options;



#endif
#include <gtk/gtk.h>
#include "uci_engine.h"
#include "../chess/chess.h"
#include "../application/application.h"
#include "../notebook/page_home.h"

gboolean
cb_execute( GtkWidget *widget,
            gpointer  *user_data )
{
    uci_engine_init_fen (engine, fen);

    uci_engine_requestmove (engine);
    set_button_sensitivity ();
}

uci_engine*
uci_engine_new (gchar *engine)
{
    uci_engine* result = g_new0 (uci_engine, 1);
    result->engine_full_path = g_strdup (engine);
    result->is_initialized = FALSE;
    result->thinking = FALSE;
    result->bestmove = NULL_MOVE;
    result->ponder = NULL_MOVE;
    return result;
}

static void
child_watch (GPid      pid,
             gint      status,
             gpointer *data )
{
    g_spawn_close_pid (pid);
}

static gboolean
out_watch (GIOChannel   *channel,
           GIOCondition  cond,
           gpointer     *data )
{
    gchar *buffer;
    gsize  size;
    uci_engine *engine = (uci_engine*)data;

    if (cond == G_IO_HUP)
    {
        g_io_channel_unref (channel);
        return FALSE;
    }

    if (engine->engine_description == NULL)
    {
        g_io_channel_read_line (channel, &buffer, &size, NULL, NULL);
        engine->engine_description = g_strdup (buffer);
            
        g_print ("%s", buffer);
        g_free (buffer);

        return TRUE;
    }
    if (engine->thinking)
    {
        g_io_channel_read_line (channel, &buffer, &size, NULL, NULL);
        if (g_str_has_prefix (buffer, "info"))
        {
            g_print ("%s", buffer);
            g_free (buffer);
        }

        if (g_str_has_prefix (buffer, "bestmove (none)"))
        {
            g_print ("%s", buffer);
            g_free (buffer);
            engine->thinking = FALSE;
            engine->bestmove = NULL_MOVE;

            return TRUE;
        }
        if (g_str_has_prefix (buffer, "bestmove "))
        {
            File start_file = CHAR_FILE (buffer[9]);
            Rank start_rank = CHAR_RANK (buffer[10]);
            File end_file = CHAR_FILE (buffer[11]);
            Rank end_rank = CHAR_RANK (buffer[12]);

            Move m = MOVE (SQUARE (start_file, start_rank), SQUARE (end_file, end_rank));
            if (buffer[13] != ' ')
            {          
                PieceType promotion = PIECE_TYPE (piece_from_char (buffer[13]));
                m = PROMOTE (m, promotion);
            }

            g_print ("%s", buffer);
            g_free (buffer);

            //Move m = MOVE (SQUARE (start_file, start_rank), SQUARE (end_file, end_rank));
            chess_print_move (m);
            engine->bestmove = m;
            engine->thinking = FALSE;

            if (engine->on_getmove_callback != NULL)
                engine->on_getmove_callback (NULL);

            return TRUE;
        }
    }



    return TRUE;
}

void
init_engine(uci_engine *engine, GError **error)
{
    // TODO assert not null engine;

    gchar *argv[] = { engine->engine_full_path, NULL };
    engine->ret = g_spawn_async_with_pipes (NULL,
                              argv,
                              NULL,
                              G_SPAWN_SEARCH_PATH | G_SPAWN_DO_NOT_REAP_CHILD,
                              NULL,
                              NULL, &engine->pid, &engine->in, &engine->out, &engine->err, 
                              error);

    if(engine->ret)
    {
        engine->is_initialized = TRUE;
        g_print ("Initialize engine success...");
    }
    else
        g_error ("Initialize engine failed...");

    g_child_watch_add (engine->pid, (GChildWatchFunc)child_watch, NULL);

#ifdef G_OS_WIN32
    engine->out_ch = g_io_channel_win32_new_fd(engine->out);
    engine->err_ch = g_io_channel_win32_new_fd(engine->err);
    engine->in_ch = g_io_channel_unix_new (engine->in);
#else
    engine->out_ch = g_io_channel_unix_new (engine->out);
    engine->err_ch = g_io_channel_unix_new (engine->err);
    engine->in_ch = g_io_channel_unix_new (engine->in);
#endif

    g_io_add_watch (engine->out_ch, G_IO_IN | G_IO_HUP, (GIOFunc)out_watch, engine);
}

void
uci_engine_init_fen (uci_engine *engine, gchar *fen)
{
    gchar *uci_command = g_strdup_printf ("position fen %s\n", fen);

    GError *error = NULL;
    gsize bytes_written;

    g_io_channel_write_chars (engine->in_ch, uci_command, g_utf8_strlen (uci_command, 100), &bytes_written, &error);
    if (error != NULL)
        g_print("%s\n", error->message);

    //g_io_channel_write_chars (engine->in_ch, "go\n", 3, &bytes_written, &error);

    g_io_channel_flush (engine->in_ch, &error);
}

void
uci_engine_requestmove (uci_engine *engine)
{    
    GError *error = NULL;
    gsize bytes_written;
    gchar *uci_command = g_strdup_printf ("go depth 20\n");
    
    g_io_channel_write_chars (engine->in_ch, uci_command, g_utf8_strlen (uci_command, 100), &bytes_written, &error);
    if (error != NULL)
        g_print("%s\n", error->message);
    g_io_channel_flush (engine->in_ch, &error);

    engine->thinking = TRUE;
}

void
uci_engine_get_uci_options_new (uci_engine *engine)
{
    while (engine->lock)
        g_usleep (100);
    engine->lock = TRUE;
}

void
uci_engine_free  (uci_engine *engine)
{
    if (engine->is_initialized)
        g_spawn_close_pid(engine->pid);

    g_free (engine->engine_full_path);
    g_free (engine->engine_description);
    g_free (engine);
}
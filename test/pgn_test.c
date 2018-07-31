#include <glib.h>
#include <gio/gio.h>
#include <sys/sysinfo.h>
#include "../src/chess/pgn.h"

static gchar *data_dir = "/home/msmaldi/Projects/dotnet/ChessBaseDownloader/pgn";
static GDir *dir;

typedef struct
{
    gchar full_path[1024];
    const gchar *filename;
    PGN *pgn;
    GError *error;
} PgnTestData;

gpointer
do_work (gpointer data)
{
    PgnTestData *pgn_data_test = (PgnTestData*)data;
    pgn_data_test->error = NULL;

    while ((pgn_data_test->filename = g_dir_read_name (dir)))
    {
        pgn_data_test->pgn = g_new0 (PGN, 1);
        g_snprintf (pgn_data_test->full_path, 1024, "%s/%s", data_dir, pgn_data_test->filename);

        gboolean success = chess_read_pgn (pgn_data_test->pgn, pgn_data_test->full_path, &pgn_data_test->error);

        if (!success)
        {
            g_printerr ("%s\n", pgn_data_test->full_path);
            g_printerr ("%s\n", pgn_data_test->error->message);

            pgn_data_test->error = NULL;
        }

        pgn_free (pgn_data_test->pgn);
    }
    return NULL;
}

int 
main(int argc, char const *argv[])
{   
    GError *error;
    gint nproc = get_nprocs ();

    dir = g_dir_open (data_dir, 0, &error);
    g_print ("Found Threads: %d\nPGN Directory: %s\n", nproc, data_dir);

    PgnTestData *data[nproc];
    GThread *thread[nproc];

    for(gint i = 0; i < nproc; i++)
    {
        data[i] =  g_new0 (PgnTestData, 1); 
        thread[i] =  g_thread_new (NULL, do_work, data[i]);
    }
    for(gint i = 0; i < nproc; i++)
    {
        g_thread_join (thread[i]);
        g_free (data[i]);
    }

    g_dir_close (dir);
        
    return 0;
}



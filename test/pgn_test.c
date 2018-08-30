#include <glib.h>
#include <gio/gio.h>
#include <sys/sysinfo.h>
#include "../src/chess/chess.h"

static gchar *data_dir = "/home/msmaldi/Projects/dotnet/ChessBaseDownloader/pgn/";//"/home/msmaldi/Projects/c/chess/data/pgn/";// "/home/msmaldi/Projects/dotnet/ChessBaseDownloader/pgn/";//
static GDir *dir;
static GMutex mutex;

typedef struct
{
    gchar full_path[256];
    PGN *pgn;
    GError *error;
} PgnTestData;
 

static const gchar*
get_filename (PgnTestData *pgn_data_test)
{
    g_mutex_lock (&mutex);
    const gchar *filename = g_dir_read_name (dir);
    g_snprintf (pgn_data_test->full_path, 256, "%s%s", data_dir, filename);
    g_mutex_unlock (&mutex);
    return filename;
}

gpointer
do_work (gpointer data)
{
    PgnTestData *pgn_data_test = (PgnTestData*)data;
    pgn_data_test->error = NULL;
    gchar *filename;

    while (get_filename (pgn_data_test))
    {
        pgn_data_test->pgn = g_new0 (PGN, 1);
        
        gboolean success = chess_read_pgn (pgn_data_test->pgn, pgn_data_test->full_path, &pgn_data_test->error);

        if (!success)
        {
            g_printerr ("%s\n", pgn_data_test->full_path);
            g_printerr ("%s\n", filename);
            g_printerr ("%s\n", pgn_data_test->error->message);

            pgn_data_test->error = NULL;
        }
        pgn_free (pgn_data_test->pgn);
    }    

    return NULL;
}

int 
main()
{   
    GError *error;
    gint nproc = get_nprocs ();
    
    g_mutex_init (&mutex);

    dir = g_dir_open (data_dir, 0, &error);
    if (dir == NULL)
    {
        g_print ("%s", error->message);
        return 1;
    }

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



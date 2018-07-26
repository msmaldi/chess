#include <glib.h>
#include <gio/gio.h>
#include "../src/chess/pgn.h"



int 
main(int argc, char const *argv[])
{   
    PGN *pgn = g_new0 (PGN, 1);


    // gchar *filename = "./horowitz_reshevsky_1936_id_1145208.pgn";

    // GError *error = NULL;
    // gboolean success = chess_read_pgn (pgn, filename, &error);
    // if (success)
    //     g_print ("OK\n");
    // else
    //     g_printerr ("%s", error->message);


    GDir *dir;
    GError *error;
    const gchar *filename;
    gchar full_path[1024];

    gchar *data_dir = "/home/msmaldi/Projects/dotnet/ChessBaseDownloader/pgn"; //"./data/pgn";//
    dir = g_dir_open(data_dir, 0, &error);
    int count = 1;
    while ((filename = g_dir_read_name(dir)))
    {
        error = NULL;
        g_snprintf (full_path, 1024, "%s/%s", data_dir, filename);
        
        //PGN pgn;
        //g_print ("OK %7d -> %s\n", count++ ,full_path);
        gboolean success = chess_read_pgn (pgn, full_path, &error);
        if (success)
        {
            //g_print (".");
        }
        else
        {
            g_printerr ("%u %s\n", count++, full_path);
            g_printerr ("%s\n", error->message);
        }        
        free_game (pgn->game);
    }
    g_free (pgn);
    //g_print ("\n");

    return 0;
}



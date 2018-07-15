#include <glib.h>
#include "../src/thirds/chess/pgn.h"
#include <assert.h>
#include <ctype.h>
#include <gio/gio.h>
#include <glib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


int
main (void)
{
    GDir *dir;
    GError *error;
    const gchar *filename;
    gchar full_path[1024];

    dir = g_dir_open("./data/pgn", 0, &error);
    int count = 1;
    while ((filename = g_dir_read_name(dir)))
    {
        error = NULL;
        g_snprintf (full_path, 1024, "%s/%s", "./data/pgn", filename);
        

        PGN pgn;
        gboolean success = read_pgn (&pgn, full_path, &error);
        if (success)
        {
            g_print ("OK %7d -> %s\n", count++ ,full_path);
        }
        else
        {
            g_print ("Fail -> %s\n", full_path);
        }
    }




    return 0;
}
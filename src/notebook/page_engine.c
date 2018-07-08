#include <gtk/gtk.h>
#include "notebook.h"

GtkWidget*
page_engine_box ()
{
    GtkWidget *engine_box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_set_border_width (GTK_CONTAINER (engine_box), 10);

    GtkWidget *engine_box_clipboard_grid = gtk_grid_new ();

    





    gtk_container_add (GTK_CONTAINER (engine_box), GTK_WIDGET (engine_box_clipboard_grid));

    return engine_box;
}
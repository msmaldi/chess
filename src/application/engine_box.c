#include "engine_box.h"


GtkWidget    *engine_box;

void
initialize_engine_box_widget (void)
{    
    engine_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);  
    //g_object_set (G_OBJECT (engine_box), "expand", TRUE, NULL); 

    GtkWidget *label = gtk_label_new ("Engine Details");



    gtk_container_add (GTK_CONTAINER (engine_box), GTK_WIDGET (label)); 
}
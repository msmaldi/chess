#include "fen_box.h"
#include "application.h"

GtkWidget   *fen_box;
GtkWidget   *fen_input;
gchar       fen[90];

void 
initialize_fen_box_widget ()
{
    fen_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    //g_object_set (G_OBJECT (fen_box), "expand", TRUE, NULL);

    GtkWidget *label = gtk_label_new ("Fen");

    fen_input = gtk_entry_new ();
    gtk_widget_set_margin_start (fen_input, 10);
    gtk_widget_set_margin_end (fen_input, 10);
    gtk_widget_set_margin_top (fen_input, 0);
    gtk_widget_set_margin_bottom (fen_input, 10);

    g_object_set (G_OBJECT (fen_input), "xalign", 0.5, NULL);
    g_object_set (G_OBJECT (fen_input), "editable", FALSE, NULL);
    const PangoFontDescription *font = 
        pango_font_description_from_string ("Monospace Regular 10");

    PangoAttribute *p_attrib = pango_attr_font_desc_new (font);
    PangoAttrList *attr_list = pango_attr_list_new ();
    pango_attr_list_insert (attr_list, p_attrib);

    gtk_entry_set_attributes (GTK_ENTRY (fen_input), attr_list);

    gtk_container_add (GTK_CONTAINER (fen_box), GTK_WIDGET (label));
    gtk_container_add (GTK_CONTAINER (fen_box), GTK_WIDGET (fen_input));

    update_fen_input ();
}

void
update_fen_input ()
{
    to_fen (chessboard->game->board, fen);
    gtk_entry_set_text (GTK_ENTRY (fen_input), fen);
}
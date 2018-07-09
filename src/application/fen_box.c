#include "fen_box.h"
#include "application.h"

GtkWidget   *fen_box;
GtkWidget   *fen_input;
gchar       fen[100];

void 
initialize_fen_box_widget ()
{
    fen_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 10);

    fen_input = gtk_entry_new ();
    set_margin (fen_input, 10);
    g_object_set (G_OBJECT (fen_box), "expand", TRUE, NULL);

    g_object_set (G_OBJECT (fen_input), "xalign", 0.5, NULL);
    g_object_set (G_OBJECT (fen_input), "editable", FALSE, NULL);

    const PangoFontDescription *font = 
        pango_font_description_from_string ("Monospace Regular 10");

    PangoAttribute *p_attrib = pango_attr_font_desc_new (font);
    PangoAttrList *attr_list = pango_attr_list_new ();
    pango_attr_list_insert (attr_list, p_attrib);

    gtk_entry_set_attributes (GTK_ENTRY (fen_input), attr_list);


    gtk_container_add (GTK_CONTAINER (fen_box), GTK_WIDGET (fen_input));

    update_fen_input ();
}

void
update_fen_input ()
{
    to_fen (chessboard->game->board, fen);
    gtk_entry_set_text (GTK_ENTRY (fen_input), fen);
}
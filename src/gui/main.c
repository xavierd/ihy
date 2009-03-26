#include <stdlib.h>
#include <gtk/gtk.h>

void OnPlay(GtkWidget *pWidget, gpointer data);
void OnPause(GtkWidget *pWidget, gpointer data);
void OnStop(GtkWidget *pWidget, gpointer data);


int main(int argc, char **argv)
{
    GtkWidget   *pWindow;
    GtkWidget   *pTable;
    GtkWidget   *pButton[3];
    GtkWidget   *pProgress;
    GtkWidget   *pToolbar;
    GtkWidget   *image;
    cairo_t     *cr;
    GdkColormap *cmap;
    GdkPixmap   *pmap;
    GdkPixbuf   *pbuf;

    gint w,h;

    w = 400;
    h = 400;

    gtk_init(&argc, &argv);

    pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(pWindow), 820, 500);
    gtk_window_set_title(GTK_WINDOW(pWindow), "Ihy Player");
    g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* Creation et insertion de la table 16 lignes 10 colonnes */
    pTable=gtk_table_new(16,10,TRUE);
    gtk_container_add(GTK_CONTAINER(pWindow), GTK_WIDGET(pTable));

    /* Creation de la barre d'outil*/
    pToolbar = gtk_toolbar_new();

    /* Remplissage de la barre d'outil*/
    gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolbar),
	    GTK_STOCK_NEW,
	    "Nouveau",
	    NULL,
	    NULL,
	    NULL,
	    -1);
    gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolbar),
	    GTK_STOCK_OPEN,
	    "Ouvrir",
	    NULL,
	    NULL,
	    NULL,
	    -1);
    gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolbar),
	    GTK_STOCK_SAVE,
	    "Enregistrer",
	    NULL,
	    NULL,
	    NULL,
	    -1);

    gtk_toolbar_insert_stock(GTK_TOOLBAR(pToolbar),
	    GTK_STOCK_QUIT,
	    "Fermer",
	    NULL,
	    G_CALLBACK(gtk_main_quit),
	    NULL,
	    -1);

    /* get a default GdkColormap */
    cmap = gdk_colormap_get_system();
    /*create a new GdkPixmap for drawing */
    pmap = gdk_pixmap_new(NULL, w, h, gdk_colormap_get_visual(cmap)->depth);    /*create a cairo context from the pixmap*/
    cr = gdk_cairo_create(GDK_DRAWABLE(pmap));

    /*fill the background red*/
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_rectangle(cr, 0, 0, w, h);
    cairo_fill(cr);

    /*create a new GdkPixbuf from the pixmap*/
    pbuf = gdk_pixbuf_get_from_drawable(NULL, pmap, cmap, 0, 0, 0, 0, w, h);
    /*create a new GtkImage from the pixbuf*/
    image = gtk_image_new_from_pixbuf(pbuf);

    /*Creation des boutons*/
    pButton[0] = gtk_button_new_from_stock(GTK_STOCK_MEDIA_PLAY);
    pButton[1] = gtk_button_new_from_stock(GTK_STOCK_MEDIA_PAUSE);
    pButton[2] = gtk_button_new_from_stock(GTK_STOCK_MEDIA_STOP);

    /* Creation de la barre de progression */
    pProgress = gtk_progress_bar_new();
    gtk_table_attach_defaults(GTK_TABLE(pTable), pProgress,
	    5, 9, 11, 12);

    /* Insertion des boutons */
    gtk_table_attach_defaults(GTK_TABLE(pTable), pToolbar,
	    0, 10, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(pTable), image,
	    0, 5, 0, 10);
    gtk_table_attach_defaults(GTK_TABLE(pTable), pButton[0],
	    1, 2, 11, 12);
    gtk_table_attach_defaults(GTK_TABLE(pTable), pButton[1],
	    2, 3, 11, 12);
    gtk_table_attach_defaults(GTK_TABLE(pTable), pButton[2],
	    3, 4, 11, 12);    

    /* Connection des boutons */
    g_signal_connect_swapped(G_OBJECT(pButton[0]), "clicked", G_CALLBACK(OnPlay), pProgress);
    g_signal_connect_swapped(G_OBJECT(pButton[1]), "clicked", G_CALLBACK(OnPause), pProgress);
    g_signal_connect_swapped(G_OBJECT(pButton[2]), "clicked", G_CALLBACK(OnStop), pProgress);

    gtk_widget_show_all(pWindow);

    gtk_main();

    return EXIT_SUCCESS;
}

void OnPlay(GtkWidget *pWidget, gpointer data)
{
    gdouble dFraction;
    gint i;
    gint iTotal = 2000;

    /* Initialisation */
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(pWidget), 0.0);

    for(i = 0 ; i < iTotal ; ++i)
    {
	dFraction = (gdouble)i / (gdouble)iTotal;

	/* Modification de la valeur de la barre de progression */
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(pWidget), dFraction);

	/* On donne la main a GTK+ */
	gtk_main_iteration ();
    }

} 

void OnPause(GtkWidget *pWidget, gpointer data)
{
    /* Tu fixe la fonction de du bouton "pause" ici*/
}

void OnStop(GtkWidget *pWidget, gpointer data)
{
    /* Tu fixe la fonction de du bouton "stop" ici*/
}

#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>
#include <cairo.h>

int points[11][2] = { 
    { 0, 85 }, 
    { 75, 75 }, 
    { 100, 10 }, 
    { 125, 75 }, 
    { 200, 85 },
    { 150, 125 }, 
    { 160, 190 },
    { 100, 150 }, 
    { 40, 190 },
    { 50, 125 },
    { 0, 85 } 
};


void OnPlay(GtkWidget *pWidget, gpointer data);
void OnPause(GtkWidget *pWidget, gpointer data);
void OnStop(GtkWidget *pWidget, gpointer data);

    static gboolean
on_expose_event(GtkWidget *widget,
	GdkEventExpose *event,
	gpointer data) /* dernier argument g_signal_connect */
{
    cairo_t *cr;

    gint width, height;

    /* you can set w and h to whatever you want */

    static gdouble angle = 0;
    static gdouble scale = 1;
    static gdouble delta = 0.01;

    static gdouble angle2 = 0;
    static gdouble scale2 = 1;
    static gdouble delta2 = 0.01;

    gtk_window_get_size(GTK_WINDOW(widget), &width, &height);

    /* je dit a cairo de dessiner sur data qui est un GdkPixMap */
    cr = gdk_cairo_create(GDK_DRAWABLE(data));

    /* fill the background black */
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);

    cairo_set_source_rgb(cr, 0, 0.44, 0.7);
    cairo_set_line_width(cr, 1);

    cairo_set_source_rgb(cr, 0.44, 1, 1);
    cairo_set_line_width(cr, 1);

    cairo_save(cr);
    cairo_translate(cr, width / 2, height / 2 );
    cairo_rotate(cr, angle);
    //cairo_scale(cr, scale, scale);

    cairo_save(cr);
    cairo_translate(cr, width / 4, height / 4 );
    cairo_rotate(cr, angle);
    cairo_scale(cr, scale, scale);

    gint i;

    for ( i = 0; i < 10; i++ ) {
	cairo_line_to(cr, points[i][0], points[i][1]);
    }

    cairo_close_path(cr);
    cairo_stroke_preserve(cr);
    cairo_fill(cr);
    cairo_restore(cr);

    gint j; 

    for ( j = 0; j < 10; j++ ) {
	cairo_line_to(cr, points[j][0], points[j][1]);
    }

    cairo_close_path(cr);
    cairo_stroke_preserve(cr);
    cairo_fill(cr);
    cairo_restore(cr);

    if ( scale < 0.01 ) {
	delta = -delta;
    } else if (scale > 0.99) {
	delta = -delta;
    }

    scale += delta;
    angle += 0.01;

    scale += delta;
    angle += 0.01;

    if ( scale2 < 0.01 ) {
	delta2 = -delta2;
    } else if (scale2 > 0.99) {
	delta2 = -delta2;
    }

    scale2 += delta2;
    angle2 += 0.01;

    cairo_destroy(cr);

    return FALSE;
}

    static gboolean
time_handler (GtkWidget *widget)
{
    if (widget->window == NULL) return FALSE;
    gtk_widget_queue_draw(widget);
    return TRUE;
}



int main(int argc, char **argv)
{
    GtkWidget   *pWindow;
    GtkWidget   *pTable;
    GtkWidget   *pButton[3];
    GtkWidget   *pScale;
    GtkWidget   *pToolbar;
    GtkWidget   *image;
    GtkWidget   *pProgress;
    cairo_t     *cr;
    GdkColormap *cmap = NULL;
    GdkPixmap   *pmap = NULL;
    GdkPixbuf   *pbuf;
    GtkWidget   *pHBox;

    int width, height;

    width = 500;
    height = 500;

    gtk_init(&argc, &argv);

    static gdouble angle = 0;
    static gdouble scale = 1;
    static gdouble delta = 0.01;

    pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_add_events (pWindow, GDK_BUTTON_PRESS_MASK);
    gtk_window_set_default_size(GTK_WINDOW(pWindow), 600, 700);
    gtk_window_set_title(GTK_WINDOW(pWindow), "Ihy Player");
    g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* Creation et insertion de la table 8 lignes 9 colonnes */
    pTable=gtk_table_new(9,9,FALSE);
    gtk_container_add(GTK_CONTAINER(pWindow), GTK_WIDGET(pTable));

    /*GtkBox horizontale*/
    pHBox = gtk_hbox_new(TRUE, 0);
    /* Ajout de la GtkHBox dans la fenetre */
    /*gtk_container_add(GTK_CONTAINER(pWindow), pHBox);*/


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
    pmap = gdk_pixmap_new(NULL, width, height, gdk_colormap_get_visual(cmap)->depth);    /*create a cairo context from the pixmap*/

    g_signal_connect(pWindow, "expose-event",
	    G_CALLBACK(on_expose_event), pmap);

    image = gtk_pixmap_new(pmap, NULL);

    /*Creation des boutons*/
    pButton[0] = gtk_button_new_from_stock(GTK_STOCK_MEDIA_PLAY);
    pButton[1] = gtk_button_new_from_stock(GTK_STOCK_MEDIA_PAUSE);
    pButton[2] = gtk_button_new_from_stock(GTK_STOCK_MEDIA_STOP);

    /* Insertion des boutons dans la pHBox*/
    gtk_box_pack_start(GTK_BOX(pHBox), pButton[0], TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox), pButton[1], TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox), pButton[2], TRUE, FALSE, 0);
    

    /* Creation de la barre progress bar */
    pProgress = gtk_progress_bar_new();
    gtk_table_attach(GTK_TABLE(pTable), pProgress,
	    1, 8, 5, 6,
	    GTK_EXPAND | GTK_FILL, GTK_EXPAND,
	    0, 0);

    /*pScale*/
    pScale = gtk_hscale_new_with_range(0, 10, 1);
    gtk_table_attach(GTK_TABLE(pTable), pScale,
	    4, 8, 7, 8,
	    GTK_EXPAND | GTK_FILL, GTK_EXPAND,
	    0, 0);

    /* Insertion des barres des tâches, des images, et de la Box */
    gtk_table_attach_defaults(GTK_TABLE(pTable), pToolbar,
	    0, 9, 0, 1);
    gtk_table_attach(GTK_TABLE(pTable), image,
	    0, 9, 1, 5,
            GTK_EXPAND, GTK_EXPAND | GTK_FILL,
            0, 0);
    gtk_table_attach_defaults(GTK_TABLE(pTable), pHBox,
	    1, 3, 7, 8);   

    /* Connection des boutons */
    g_signal_connect_swapped(G_OBJECT(pButton[0]), "clicked", G_CALLBACK(OnPlay), pProgress);
    g_signal_connect_swapped(G_OBJECT(pButton[1]), "clicked", G_CALLBACK(OnPause), pProgress);
    g_signal_connect_swapped(G_OBJECT(pButton[2]), "clicked", G_CALLBACK(OnStop), pProgress);

    gtk_widget_show_all(pWindow);

    gtk_widget_set_app_paintable(pWindow, TRUE);

    g_timeout_add(5, (GSourceFunc) time_handler, (gpointer) pWindow);

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

    for(i = 0 ; i <= iTotal ; ++i)
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

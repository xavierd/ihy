#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>
#include <cairo.h>
#include <gdk/gdkkeysyms.h>


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
	gpointer data) /* last argument of g_signal_connect */
{
    cairo_t *cr;

    gint width, height;

    static gdouble angle = 0;
    static gdouble scale = 1;
    static gdouble delta = 0.01;

    static gdouble angle2 = 0;
    static gdouble scale2 = 1;
    static gdouble delta2 = 0.01;

    gdk_drawable_get_size(GDK_PIXMAP(data), &width, &height);

    /* Cairo draw on data which is a GdkPixMap */
    cr = gdk_cairo_create(GDK_DRAWABLE(data));

    /* fill the background black */
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);

    /*Choose the color of the stars*/
    cairo_set_source_rgb(cr, 0, 0.44, 0.7);
    cairo_set_line_width(cr, 1);

    /*Here we make move the first star*/
    cairo_save(cr);
    cairo_translate(cr, width / 2, height / 2 );
    cairo_rotate(cr, angle);

    /*Here we make move the second star*/
    cairo_save(cr);
    cairo_translate(cr, width / 4, height / 4 );
    cairo_rotate(cr, angle);
    cairo_scale(cr, scale, scale);

    /*We shift the first star into the middle of the window. Rotate it and scale it.*/ 
    gint i;

    for ( i = 0; i < 10; i++ ) {
	cairo_line_to(cr, points[i][0], points[i][1]);
    }

    cairo_close_path(cr);
    cairo_stroke_preserve(cr);
    cairo_fill(cr);
    cairo_restore(cr);

    /*We shift the second star into the middle of the window. Rotate it and scale it.*/ 
    gint j; 

    for ( j = 0; j < 10; j++ ) {
	cairo_line_to(cr, points[j][0], points[j][1]);
    }

    cairo_close_path(cr);
    cairo_stroke_preserve(cr);
    cairo_fill(cr);
    cairo_restore(cr);

    /*Here we draw the first star*/
    if ( scale < 0.01 ) {
	delta = -delta;
    } else if (scale > 0.99) {
	delta = -delta;
    }

    scale += delta;
    angle += 0.01;
     
    /*Here we draw the second star*/
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
    GtkWidget   *image;
    GtkWidget   *pProgress;
    GdkColormap *cmap = NULL;
    GdkPixmap   *pmap = NULL;
    GtkWidget   *pHBox;
    GtkWidget   *menubar;
    GtkWidget   *filemenu;
    GtkWidget   *file;
    GtkWidget   *new;
    GtkWidget   *open;
    GtkWidget   *quit;
    GtkWidget   *sep;
    
    GtkAccelGroup *accel_group = NULL; /*For the MenuBar*/

    int width, height;

    width = 500;
    height = 500;

    gtk_init(&argc, &argv);

    pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_add_events (pWindow, GDK_BUTTON_PRESS_MASK);
    gtk_window_set_default_size(GTK_WINDOW(pWindow), 500, 500);
    gtk_window_set_title(GTK_WINDOW(pWindow), "Ihy Player");
    g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /*The MenuBar*/
    menubar = gtk_menu_bar_new();
    filemenu = gtk_menu_new();

    accel_group = gtk_accel_group_new();
    gtk_window_add_accel_group(GTK_WINDOW(pWindow), accel_group);

    file = gtk_menu_item_new_with_mnemonic("_File");
    new = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW, NULL);
    open = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, NULL);
    sep = gtk_separator_menu_item_new();
    quit = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, accel_group);

    gtk_widget_add_accelerator(quit, "activate", accel_group, 
	    GDK_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE); 

    /*The buttons for the MenuBar*/
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), new);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), open);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), sep);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), quit);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file);

    /* The Table */
    pTable=gtk_table_new(9,9,FALSE);
    gtk_container_add(GTK_CONTAINER(pWindow), GTK_WIDGET(pTable));

    /*GtkBox horizontale*/
    pHBox = gtk_hbox_new(TRUE, 0);

    /* get a default GdkColormap */
    cmap = gdk_colormap_get_system();

    /*create a new GdkPixmap for drawing */
    pmap = gdk_pixmap_new(NULL, width, height, gdk_colormap_get_visual(cmap)->depth);    /*create a cairo context from the pixmap*/

    g_signal_connect(pWindow, "expose-event",G_CALLBACK(on_expose_event), pmap);

    image = gtk_pixmap_new(pmap, NULL);

    /*Media buttons*/
    pButton[0] = gtk_button_new_from_stock(GTK_STOCK_MEDIA_PLAY);
    pButton[1] = gtk_button_new_from_stock(GTK_STOCK_MEDIA_PAUSE);
    pButton[2] = gtk_button_new_from_stock(GTK_STOCK_MEDIA_STOP);

    /* Buttons in the GtkBox*/
    gtk_box_pack_start(GTK_BOX(pHBox), pButton[0], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox), pButton[1], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox), pButton[2], TRUE, TRUE, 0);


    /* Progress bar in the table */
    pProgress = gtk_progress_bar_new();
    gtk_table_attach(GTK_TABLE(pTable), pProgress,
	    2, 9, 7, 8,
	    GTK_EXPAND | GTK_FILL, GTK_EXPAND,
	    0, 0);

    /* menu bar, image and GtkBox in the table */
    gtk_table_attach(GTK_TABLE(pTable), menubar,
	    0, 9, 0, 1,
	    GTK_EXPAND | GTK_FILL, GTK_FILL,
	    0, 0);
    gtk_table_attach(GTK_TABLE(pTable), image,
	    0, 9, 1, 5,
	    GTK_EXPAND, GTK_EXPAND | GTK_FILL,
	    0, 0);
    gtk_table_attach(GTK_TABLE(pTable), pHBox,
	    0, 2, 7, 8,
	    0,0,
	    0,0);   

    /* Buttons connect */
    g_signal_connect_swapped(G_OBJECT(pButton[0]), "clicked", G_CALLBACK(OnPlay), pProgress);
    g_signal_connect_swapped(G_OBJECT(pButton[1]), "clicked", G_CALLBACK(OnPause), pProgress);
    g_signal_connect_swapped(G_OBJECT(pButton[2]), "clicked", G_CALLBACK(OnStop), pProgress);
    g_signal_connect(G_OBJECT(quit), "activate",G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(pWindow);

    gtk_widget_set_app_paintable(pWindow, TRUE);

    /*speed of the stars*/
    g_timeout_add(10, (GSourceFunc) time_handler, (gpointer) pWindow);

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

	/* Modification of the progress bar */
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(pWidget), dFraction);


	/* we give the hand to GTK+ */
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

#include <stdlib.h>
#include <gtk/gtk.h>

void OnPlay(GtkWidget *pWidget, gpointer data);
void OnPause(GtkWidget *pWidget, gpointer data);
void OnStop(GtkWidget *pWidget, gpointer data);

static GtkWidget *pToolbar = NULL;
 //Declare une zone de dessin
static GdkPixmap *pixmap = NULL;

//Initialize la zone de dessin, avec la taille qu'elle doit avoir ainsi que sa position
static gint
configure_event (GtkWidget *widget, GdkEventConfigure *event)
{
    if (pixmap)
    gdk_pixmap_unref(pixmap);

    pixmap = gdk_pixmap_new(widget->window,
    widget->allocation.width,
    widget->allocation.height,
    -1);//Creer une fenetre avec la zone de dessins
    gdk_draw_rectangle (pixmap, //Alloue la taille et la position de la fenetre
    widget->style->black_gc, //Fond noir
    TRUE,
    0, 0,
    widget->allocation.width, //Largeur
    widget->allocation.height); //Heuteur

    return TRUE;
}

 //Redessine la zone de dessin en fonction du tracer
static gint
expose_event (GtkWidget *widget, GdkEventExpose *event)
{
    //Permet de dessiner le tracer fait avec la souris
    gdk_draw_pixmap(widget->window,
    widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
    pixmap,
    event->area.x, event->area.y, //Evenement 1 (x, y)
    event->area.x, event->area.y, //Evenement 2 (x, y)
    event->area.width, event->area.height); //Largeur et hauteur

    return FALSE;
 }

static gint
motion_notify_event (GtkWidget *widget, GdkEventMotion *event)
{
    int x, y;
    GdkModifierType state;

    if (event->is_hint)
    gdk_window_get_pointer (event->window, &x, &y, &state);
    else
{
    x = event->x;
    y = event->y;
    state = event->state;
}

   return TRUE;
}

//void
//quit ()
//{
//    gtk_exit (0); //Quitte l'aplication
//}

int main(int argc, char **argv)
{
    GtkWidget *pWindow;
    GtkWidget *pTable;
    GtkWidget *drawing_area;
    GtkWidget *pButton[3];
    GtkWidget *pProgress;

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

     /* Create the drawing area */

    drawing_area = gtk_drawing_area_new (); //Crer une zone de dessins
    gtk_drawing_area_size (GTK_DRAWING_AREA (drawing_area), 50, 50); //Initialise la zone de dessins
    //gtk_box_pack_start (GTK_BOX (vbox), drawing_area, TRUE, TRUE, 0); //Ajoute la zone de dessin a la boute

    gtk_widget_show (drawing_area); //Affiche la zone de dessin

     //Les differents signaux a connecteer
 gtk_signal_connect (GTK_OBJECT (drawing_area), "expose_event",
 (GtkSignalFunc) expose_event, NULL);
 gtk_signal_connect (GTK_OBJECT(drawing_area),"configure_event",
 (GtkSignalFunc) configure_event, NULL);


 gtk_signal_connect (GTK_OBJECT (drawing_area), "motion_notify_event",
 (GtkSignalFunc) motion_notify_event, NULL);

 gtk_widget_set_events (drawing_area, GDK_EXPOSURE_MASK
 | GDK_LEAVE_NOTIFY_MASK
 | GDK_BUTTON_PRESS_MASK
 | GDK_POINTER_MOTION_MASK
 | GDK_POINTER_MOTION_HINT_MASK);


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
    gtk_table_attach_defaults(GTK_TABLE(pTable), drawing_area,
        0, 7, 1, 10);
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

   gtk_grab_add(pWidget);

   for(i = 0 ; i < iTotal ; ++i)
   {
      dFraction = (gdouble)i / (gdouble)iTotal;

      /* Modification de la valeur de la barre de progression */
      gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(pWidget), dFraction);

      /* On donne la main a GTK+ */
      gtk_main_iteration ();
   }

   /* On supprime le grab sur la barre de progression */
   gtk_grab_remove(pWidget);
} 

void OnPause(GtkWidget *pWidget, gpointer data)
{
    /* Tu fixe la fonction de du bouton "pause" ici*/
}

void OnStop(GtkWidget *pWidget, gpointer data)
{
    /* Tu fixe la fonction de du bouton "stop" ici*/
}

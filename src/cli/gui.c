#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <math.h>
#include <cairo.h>
#include <gdk/gdkkeysyms.h>
#include <glib/gprintf.h>

#define TAILLE_MAX 78


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
char* title(const char* chaine);
char* GetFirst();
void OnAdd(GtkWidget *pWidget, gpointer data);
static void OnRemove(GtkWidget *pWidget, gpointer data);
static void OnDown(GtkWidget *pWidget, gpointer data);
static void OnClear(GtkWidget *pWidget, gpointer data);
void OnBefore(GtkWidget *pWidget, gpointer data);
void OnAfter(GtkWidget *pWidget, gpointer data);
void OnQuit(GtkWidget *pWidget, gpointer data);
GtkListStore *pListStore;
GtkListStore *pListStore2;
gboolean stop = TRUE;
gboolean pause = TRUE;
gboolean play = FALSE;
GtkWidget   *quit;
GtkTreeIter pIter;
GtkTreeIter pIter2;
GtkWidget   *pListView;
GtkWidget   *pListView2;
gchar *sTitle;
gchar *sChemin;
static gdouble angle = 0;
gint j = 0;








enum {
    STRING_COLUMN,
    TOGGLE_COLUMN,
    N_COLUMN
};

    static gboolean
on_expose_event(GtkWidget *widget,
	GdkEventExpose *event,
	gpointer data) /* last argument of g_signal_connect */
{

    cairo_t *cr;
    gint width, height;
    gint i;
    static gdouble scale = 1;
    static gdouble delta = 0.01;

    widget = widget;
    event = event;

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

    /*We shift the first star into the middle of the window. Rotate it and scale it.*/ 

    for ( i = 0; i < 10; i++ ) {
	cairo_line_to(cr, points[i][0], points[i][1]);
    }

    cairo_close_path(cr);
    cairo_stroke_preserve(cr);
    cairo_fill(cr);

    /*cairo_restore(cr);*/

    /*Here we draw the first star*/
    if ( scale < 0.01 ) {
	delta = -delta;
    } else if (scale > 0.99) {
	delta = -delta;
    }

    scale += delta;
    if (play && pause && stop) 
    {
	angle += 0.01;
    }
    else if (!pause)
    {
	cairo_save(cr);
    }
    else if ((!stop) || (j==0))
    {
	angle = 0;
    }

    /*cairo_destroy(cr);*/

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
    GdkPixmap   *pmap = NULL;
    GtkWidget   *pWindow;
    GtkWidget   *image;
    GtkWidget   *pTable;
    GtkWidget   *pButton[7];
    GtkWidget   *pProgress;
    GtkWidget   *pHBox;
    GtkWidget   *menubar;
    GtkWidget   *filemenu;
    GtkWidget   *file;
    GtkWidget   *new;
    GtkWidget   *open;
    GtkWidget   *sep;
    GtkWidget   *pNotebook;
    gchar       *sTabLabel;
    GtkWidget   *pTabLabel;
    GtkWidget   *pTabLabel2;
    GdkColormap *cmap = NULL;
    GtkWidget   *pVBox;
    GtkWidget   *pVBox2;
    GtkWidget   *pHBox2;
    GtkWidget   *pScrollbar;
    GtkTreeViewColumn   *pColumn;
    GtkCellRenderer   *pCellRenderer;
    GtkTreeViewColumn   *pColumn2;
    GtkCellRenderer   *pCellRenderer2;
    GtkTreeSelection *data;
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

    /* Creation de la GtkBox verticale */
    pVBox = gtk_vbox_new(FALSE, 0);
    /* Ajout de la GtkVBox dans la fenetre */
    gtk_container_add(GTK_CONTAINER(pWindow), pVBox);

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

    gtk_widget_add_accelerator(quit, "destroy", accel_group, 
	    GDK_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE); 

    /*The buttons for the MenuBar*/
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), new);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), open);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), sep);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), quit);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file);

    gtk_box_pack_start(GTK_BOX(pVBox), menubar, FALSE, FALSE, 0);

    /* Creation du GtkNotebook */
    pNotebook = gtk_notebook_new();
    gtk_box_pack_start(GTK_BOX(pVBox), pNotebook, TRUE, TRUE, 0);
    /* Position des onglets : en haut */
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(pNotebook), GTK_POS_TOP);
    /* Ajout des boutons de navigation */
    gtk_notebook_set_scrollable(GTK_NOTEBOOK(pNotebook), TRUE);
    sTabLabel = g_strdup_printf("Spectrum");
    pTabLabel = gtk_label_new(sTabLabel);

    g_free(sTabLabel);

    sTabLabel = g_strdup_printf("Playlist");
    pTabLabel2 = gtk_label_new(sTabLabel);

    g_free(sTabLabel);

    /* The Table */
    pTable=gtk_table_new(9,9,FALSE);

    /* Creation de la GtkBox verticale */
    pVBox2 = gtk_vbox_new(FALSE, 0);
    /* Ajout de la GtkVBox dans la fenetre */
    gtk_container_add(GTK_CONTAINER(pWindow), pVBox2);

    /* Insertion de la page */
    gtk_notebook_append_page(GTK_NOTEBOOK(pNotebook), pTable, pTabLabel);
    gtk_notebook_append_page(GTK_NOTEBOOK(pNotebook), pVBox2, pTabLabel2);

    /* Playlist */
    pHBox2 = gtk_hbox_new(TRUE, 0);
    gtk_box_pack_start(GTK_BOX(pVBox2), pHBox2, FALSE, FALSE, 0);
    /* Creation du modele */
    pListStore = gtk_list_store_new(N_COLUMN, G_TYPE_STRING, G_TYPE_BOOLEAN);

    /* Creation de la vue */
    pListView = gtk_tree_view_new_with_model(GTK_TREE_MODEL(pListStore));

    /* Creation du modele */
    pListStore2 = gtk_list_store_new(N_COLUMN, G_TYPE_STRING, G_TYPE_BOOLEAN);

    /* Creation de la vue */
    pListView2 = gtk_tree_view_new_with_model(GTK_TREE_MODEL(pListStore2));

    /* Creation de la premiere colonne */
    pCellRenderer = gtk_cell_renderer_text_new();
    pColumn = gtk_tree_view_column_new_with_attributes("Titre",
	    pCellRenderer,
	    "text", STRING_COLUMN,
	    NULL);

    /* Creation de la premiere colonne */
    pCellRenderer2 = gtk_cell_renderer_text_new();
    pColumn2 = gtk_tree_view_column_new_with_attributes("Chemin",
	    pCellRenderer,
	    "text", STRING_COLUMN,
	    NULL);

    /* Ajout de la colonne à la vue */
    gtk_tree_view_append_column(GTK_TREE_VIEW(pListView), pColumn);

    /* Ajout de la vue a la fenetre */
    pScrollbar = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(pVBox2), pScrollbar, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(pScrollbar),
	    GTK_POLICY_AUTOMATIC,
	    GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(pScrollbar), pListView);
    gtk_container_add(GTK_CONTAINER(pWindow), pScrollbar);

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
    pButton[3] = gtk_button_new_from_stock(GTK_STOCK_ADD);
    pButton[4] = gtk_button_new_from_stock(GTK_STOCK_REMOVE);
    pButton[5] = gtk_button_new_from_stock(GTK_STOCK_GO_DOWN);
    pButton[6] = gtk_button_new_from_stock(GTK_STOCK_CLEAR);

    /* Buttons in the GtkBox*/
    gtk_box_pack_start(GTK_BOX(pHBox), pButton[0], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox), pButton[1], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox), pButton[2], TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(pHBox2), pButton[3], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox2), pButton[4], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox2), pButton[5], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox2), pButton[6], TRUE, TRUE, 0);

    /* Progress bar in the table */
    pProgress = gtk_progress_bar_new();
    gtk_table_attach(GTK_TABLE(pTable), pProgress,
	    2, 9, 7, 8,
	    GTK_EXPAND | GTK_FILL, GTK_EXPAND,
	    0, 0);

    gtk_table_attach(GTK_TABLE(pTable), image,
	    0, 9, 1, 5,
	    GTK_EXPAND, GTK_EXPAND | GTK_FILL,
	    0, 0);
    gtk_table_attach(GTK_TABLE(pTable), pHBox,
	    0, 2, 7, 8,
	    0,0,
	    0,0);   

    data  = gtk_tree_view_get_selection(GTK_TREE_VIEW(pListView));

    /* Buttons connect */
    g_signal_connect_swapped(G_OBJECT(pButton[0]), "clicked", G_CALLBACK(OnPlay), pProgress);
    g_signal_connect_swapped(G_OBJECT(pButton[1]), "clicked", G_CALLBACK(OnPause), pProgress);
    g_signal_connect_swapped(G_OBJECT(pButton[2]), "clicked", G_CALLBACK(OnStop), pProgress);
    g_signal_connect(G_OBJECT(quit), "destroy",G_CALLBACK(OnQuit), NULL);
    g_signal_connect(G_OBJECT(pButton[3]), "clicked", G_CALLBACK(OnAdd), NULL);
    g_signal_connect(G_OBJECT(pButton[4]), "clicked", G_CALLBACK(OnRemove), data);
    g_signal_connect(G_OBJECT(pButton[6]), "clicked", G_CALLBACK(OnClear), NULL);
    g_signal_connect(G_OBJECT(pButton[5]), "clicked", G_CALLBACK(OnDown), data);

    gtk_widget_show_all(pWindow);

    gtk_widget_set_app_paintable(pWindow, TRUE);

    if (gtk_notebook_get_current_page(GTK_NOTEBOOK(pNotebook)) == 0)
    { 
        /*speed of the stars*/
        g_timeout_add(30, (GSourceFunc) time_handler, (gpointer) pWindow);
    }
    else
    {
        g_source_remove(30);
    }

    gtk_main();

    return EXIT_SUCCESS;
}

char* title(const char* chaine)
{
    char *chaine2;
    long i = 0;
    long j = 0;

    chaine2 = malloc(78* sizeof(char)); 
    while (chaine[i] != '\0')
    {
	if (chaine[i] != '/')
	{
	    chaine2[j] = chaine[i];
	    j++;
	    i++;       
	}
	else 
	{
	    j = 0;
	    i++;
	}
    }
    chaine2[j]='\0';
    return chaine2;
}

void OnAdd(GtkWidget *pWidget, gpointer data)
{
    GtkWidget *pFileSelection;
    GtkWidget *pParent;


    pParent = NULL;

    pWidget = pWidget;
    data = data;

    /* Creation de la fenetre de selection */
    pFileSelection = gtk_file_chooser_dialog_new("Ouvrir...",
	    GTK_WINDOW(pParent),
	    GTK_FILE_CHOOSER_ACTION_OPEN,
	    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	    GTK_STOCK_OPEN, GTK_RESPONSE_OK,
	    NULL);
    /* On limite les actions a cette fenetre */
    gtk_window_set_modal(GTK_WINDOW(pFileSelection), TRUE);



    /* Affichage fenetre */
    switch(gtk_dialog_run(GTK_DIALOG(pFileSelection)))
    {
	case GTK_RESPONSE_OK:

	    /* Recuperation du chemin */
	    sChemin = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(pFileSelection));
	    sTitle = title(sChemin);

	    /* Creation de la nouvelle ligne */
	    gtk_list_store_append(pListStore, &pIter);

	    /* Mise a jour des donnees */
	    gtk_list_store_set(pListStore, &pIter,
		    STRING_COLUMN, sTitle,
		    TOGGLE_COLUMN, TRUE,
		    -1);

	    /* Creation de la nouvelle ligne */
	    gtk_list_store_append(pListStore2, &pIter2);

	    /* Mise a jour des donnees */
	    gtk_list_store_set(pListStore2, &pIter2,
		    STRING_COLUMN, sChemin,
		    TOGGLE_COLUMN, TRUE,
		    -1);


	    g_free(sChemin);
	    g_free(sTitle);
	    break;
	default:
	    break;
    }
    gtk_widget_destroy(pFileSelection);
}

char* GetFirst()
{
    gboolean valid;
    gint row_count;

    row_count = 0;

    /* Get the first iter in the list */
    valid = gtk_tree_model_get_iter_first (GTK_TREE_MODEL(pListStore2), &pIter2);

    if (valid) 
    {
	gtk_tree_model_get (GTK_TREE_MODEL(pListStore2), &pIter2,
		STRING_COLUMN, &sChemin,
		-1);
    }
    return sChemin;
}


static void OnRemove(GtkWidget *pWidget, gpointer data)
{

    GtkTreeModel *model;

    pWidget = pWidget;
    data = data;

    model = gtk_tree_view_get_model (GTK_TREE_VIEW(pListView));

    if (gtk_tree_model_get_iter_first(model, &pIter) == FALSE) 
	return;

    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(data), &model,
		&pIter)) {
	gtk_list_store_remove(pListStore, &pIter);
	gtk_list_store_remove(pListStore2, &pIter2);
	gtk_tree_selection_select_iter(GTK_TREE_SELECTION(data), &pIter);
    } 
}

/* fonction pour passé à la musique suivante */
static void OnDown(GtkWidget *pWidget, gpointer data)
{

    GtkTreeModel *model;

    pWidget = pWidget;
    model = gtk_tree_view_get_model (GTK_TREE_VIEW(pListView));

    if (gtk_tree_model_get_iter_first(model, &pIter) == FALSE) 
	return;

    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(data), &model,
		&pIter)) {
	gtk_tree_model_iter_next(GTK_TREE_MODEL(pListStore), &pIter);
	gtk_tree_selection_select_iter(GTK_TREE_SELECTION(data), &pIter);
    }
}

static void OnClear(GtkWidget *pWidget, gpointer data)
{
    pWidget = pWidget;
    data = data;
    gtk_list_store_clear(pListStore);
    gtk_list_store_clear(pListStore2);
}



void OnQuit(GtkWidget *pWidget, gpointer data)
{
    pWidget = pWidget;
    data = data;
    stop = !stop;
    g_signal_connect(G_OBJECT(quit), "destroy",G_CALLBACK(gtk_main_quit), NULL);
}


void OnPlay(GtkWidget *pWidget, gpointer data)
{

    gint k;
    gint iTotal = 2000;
    gdouble dFraction;

    data = data;

    stop = TRUE;
    pause = TRUE;
    play = TRUE;

    /* Initialisation */
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(pWidget), 0.0);

    for(k = j ; k <= iTotal ; ++k)
    {
	if (stop && pause)
	{
	    dFraction = (gdouble)k / (gdouble)iTotal;

	    /* Modification of the progress bar */
	    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(pWidget), dFraction);
	    /* we give the hand to GTK+ */
	    gtk_main_iteration ();
	    j=j+1;

	}
	else if (!pause)
	{
	    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(pWidget), dFraction);
	}
	else if (!stop)
	{
	    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(pWidget), 0.0);
	    j=0;
	}
 
    }
    if (stop && pause)
    {
    	j=0;
        angle=0;
        play = !play;
    }
} 

void OnPause(GtkWidget *pWidget, gpointer data)
{
    pWidget = pWidget;
    data = data;
    pause = !pause;
}

void OnStop(GtkWidget *pWidget, gpointer data)
{
    pWidget = pWidget;
    data = data;
    stop = !stop;
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(pWidget), 0.0);
    j=0;
    angle=0;
}

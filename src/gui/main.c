#include <stdlib.h>
#include <gtk/gtk.h>

void OnDestroy(GtkWidget *pWidget, gpointer pData);
void OnPlay(GtkWidget *pWidget, gpointer data);
void OnPause(GtkWidget *pWidget, gpointer data);
void OnStop(GtkWidget *pWidget, gpointer data);
void OnRewind(GtkWidget *pWidget, gpointer data);
void OnForward(GtkWidget *pWidget, gpointer data);




int main(int argc,char **argv)
{
    GtkWidget *pWindow;
    GtkWidget *pVBox;
    GtkWidget *pHBox;
    GtkWidget *pButton[5];
    GtkWidget *pProgress;

    gtk_init(&argc,&argv);

    /* Creation de la fenetre */
    pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    /* Definition de la position */
    gtk_window_set_position(GTK_WINDOW(pWindow), GTK_WIN_POS_NONE);
    /* Definition de la taille de la fenetre */
    gtk_window_set_default_size(GTK_WINDOW(pWindow), 620, 400);
    /* Titre de la fenetre */
    gtk_window_set_title(GTK_WINDOW(pWindow), "Lecteur Ihy");

    /* Connexion du signal "destroy" */
    g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(OnDestroy), NULL);

    /* Creation de la GtkBox verticale */
    pVBox = gtk_vbox_new(TRUE, 0);
    /* Ajout de la GtkHBox dans la fenetre */
    gtk_container_add(GTK_CONTAINER(pWindow), pVBox);

    /* Creation de la GtkBox horizontale */
    pHBox = gtk_hbox_new(TRUE, 0);
    /* Ajout de la GtkHBox dans la GtkVBox */
    gtk_box_pack_start(GTK_BOX(pVBox), pHBox, TRUE, FALSE, 0);

    /* Creation de la barre de progression */
    pProgress = gtk_progress_bar_new();
    gtk_box_pack_start(GTK_BOX(pVBox), pProgress, TRUE, FALSE, 0);

    /* Creation des boutons */
    pButton[0] = gtk_button_new_from_stock(GTK_STOCK_MEDIA_PLAY);
    pButton[1] = gtk_button_new_from_stock(GTK_STOCK_MEDIA_PAUSE);
    pButton[2] = gtk_button_new_from_stock(GTK_STOCK_MEDIA_STOP);
    pButton[3] = gtk_button_new_from_stock(GTK_STOCK_MEDIA_REWIND);
    pButton[4] = gtk_button_new_from_stock(GTK_STOCK_MEDIA_FORWARD);


    /* Ajout des Boutons dans la GtkHBox */
    gtk_box_pack_start(GTK_BOX(pHBox), pButton[0], FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox), pButton[1], FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox), pButton[2], FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox), pButton[3], FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox), pButton[4], FALSE, FALSE, 0);

    /* Connection des boutons */
    g_signal_connect_swapped(G_OBJECT(pButton[0]), "clicked", G_CALLBACK(OnPlay), pProgress);
    g_signal_connect_swapped(G_OBJECT(pButton[1]), "clicked", G_CALLBACK(OnPause), pProgress);
    g_signal_connect_swapped(G_OBJECT(pButton[2]), "clicked", G_CALLBACK(OnStop), pProgress);
    g_signal_connect_swapped(G_OBJECT(pButton[3]), "clicked", G_CALLBACK(OnRewind), pProgress);
    g_signal_connect_swapped(G_OBJECT(pButton[4]), "clicked", G_CALLBACK(OnForward), pProgress);
    
    /* Affichage de la fenetre */
    gtk_widget_show_all(pWindow);

    /* Demarrage de la boucle evenementielle */
    gtk_main();

    return EXIT_SUCCESS;
}

void OnDestroy(GtkWidget *pWidget, gpointer pData)
{
    /* Arret de la boucle evenementielle */
    gtk_main_quit();
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

void OnRewind(GtkWidget *pWidget, gpointer data)
{
    /* Tu fixe la fonction de du bouton "rewind" ici*/
}

void OnForward(GtkWidget *pWidget, gpointer data)
{
    /* Tu fixe la fonction de du bouton "forward" ici*/
}


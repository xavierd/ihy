#include <stdlib.h>
#include <gtk/gtk.h>

void OnDestroy(GtkWidget *pWidget, gpointer pData);
void OnAboutBtn(GtkWidget *pBtn, gpointer data);
void OnQuitBtn(GtkWidget *pBtn, gpointer data);

int main(int argc,char **argv)
{
    GtkWidget *pWindow;
    GtkWidget *pVBox;
    GtkWidget *pGoodBtn;
    GtkWidget *pBadBtn;

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
    /* Ajout de la GtkVBox dans la fenetre */
    gtk_container_add(GTK_CONTAINER(pWindow), pVBox);

    /* Creation des boutons */
    pGoodBtn = gtk_button_new_with_label("Rap music");
    pBadBtn = gtk_button_new_with_label("Rock");

    /* Ajout de Bouton 1 dans la GtkVBox */
    gtk_box_pack_start(GTK_BOX(pVBox), pGoodBtn, TRUE, FALSE, 0);
    g_signal_connect(
	    G_OBJECT(pGoodBtn),
	    "clicked",
	    G_CALLBACK(OnAboutBtn),
	    (GtkWidget*) pWindow
	    );

    /* Ajout des boutons 2 dans la GtkVBox */
    gtk_box_pack_start(GTK_BOX(pVBox), pBadBtn, TRUE, FALSE, 0);
    g_signal_connect(
	    G_OBJECT(pBadBtn),
	    "clicked",
	    G_CALLBACK(OnQuitBtn),
	    (GtkWidget*) pWindow
	    );

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

void OnAboutBtn(GtkWidget *pBtn, gpointer data)

{
    GtkWidget *pGoodBtn;

    /* Creation de la boite de message */
    /* Bouton : 1 OK -> GTK_BUTTONS_OK */
    pGoodBtn = gtk_message_dialog_new (GTK_WINDOW(data),
	    GTK_DIALOG_MODAL,
	    GTK_MESSAGE_INFO,
	    GTK_BUTTONS_OK,
	    "Roi Heenok\n%s",
	    "Du rap dose");

    /* Affichage de la boite de message */
    gtk_dialog_run(GTK_DIALOG(pGoodBtn));

    /* Destruction de la boite de message */
    gtk_widget_destroy(pGoodBtn);
}


void OnQuitBtn(GtkWidget* widget, gpointer data)
{
    GtkWidget *pQuestion;

    /* Creation de la boite de message */
    /* Boutons : 1 OUI, 1 NON -> GTK_BUTTONS_YES_NO */
    pQuestion = gtk_message_dialog_new (GTK_WINDOW(data),
	    GTK_DIALOG_MODAL,
	    GTK_MESSAGE_QUESTION,
	    GTK_BUTTONS_YES_NO,
	    "Voulez vous vraiment\nécouter ce genre de musique?");

    /* Affichage et attente d une reponse */
    switch(gtk_dialog_run(GTK_DIALOG(pQuestion)))
    {
	case GTK_RESPONSE_YES:
	    /* OUI -> on quitte l application */
	    gtk_main_quit();
	    break;
	case GTK_RESPONSE_NO:
	    /* NON -> on detruit la boite de message */
	    gtk_widget_destroy(pQuestion);
	    break;
    }
}

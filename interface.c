#include <stdlib.h>
#include <gtk/gtk.h>
 
int main(int argc, char **argv)
{
    	//mes variables
    	GtkWidget *window, *vbox, *image, *quitImg, *quitBtn, *dialog;
    	//initialisation
    	gtk_init(&argc, &argv);
 
    	//ouverture de ma fenetre+parametres
    	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    	gtk_window_set_default_size(GTK_WINDOW(window), 320, 200);
    	gtk_window_set_title(GTK_WINDOW(window), "ReadMe");
    	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    	//init ma vbox+ajout
    	vbox = gtk_vbox_new(FALSE, 0);
    	gtk_container_add(GTK_CONTAINER(window), vbox);
 
    	//init dialog
        dialog = gtk_file_chooser_dialog_new("Choose the image wanted", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
        //ouverture dialog+work
	gtk_widget_show_all(dialog);
        gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), g_get_home_dir());
        gint resp = gtk_dialog_run(GTK_DIALOG(dialog));
        //si fichier choisi alors afficher l'image sinon fermer dialog
	if(resp == GTK_RESPONSE_OK)
	{
		image = gtk_image_new_from_file(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
    		gtk_box_pack_start(GTK_BOX(vbox), image, FALSE, FALSE, 5);
	}
        else
                g_print("Cancel\n");
	//fermer le dialog
        gtk_widget_destroy(dialog);

    	gtk_box_pack_start(GTK_BOX(vbox), image, FALSE, FALSE, 5);
 
    	quitBtn = gtk_button_new();
    	gtk_box_pack_start(GTK_BOX(vbox), quitBtn, TRUE, FALSE, 5);
    	g_signal_connect(G_OBJECT(quitBtn), "clicked", G_CALLBACK(gtk_main_quit), NULL);
 
    	/* Chargement d'une image a partir d'un GtkStockItem */
    	quitImg = gtk_image_new_from_stock(GTK_STOCK_QUIT, GTK_ICON_SIZE_LARGE_TOOLBAR);
    	gtk_container_add(GTK_CONTAINER(quitBtn), quitImg);
 
    	gtk_widget_show_all(window);
 
    	gtk_main();
 
    	return EXIT_SUCCESS;
}


#include <stdlib.h>
#include <gtk/gtk.h>
#include "img.c"

static void *get_filename(GtkWidget *button, gpointer data)
{
	GtkWidget *image = GTK_WIDGET(data);
	GtkWidget *toplevel = gtk_widget_get_toplevel (image);
	GtkWidget *dialog;
	gchar* filename;

	//init dialog
        dialog = gtk_file_chooser_dialog_new("Choose the image wanted", 
																							GTK_WINDOW(toplevel), 
																							GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);

        gint resp = gtk_dialog_run(GTK_DIALOG(dialog));
        //si fichier choisi alors afficher l'image sinon fermer dialog
        if(resp == GTK_RESPONSE_ACCEPT)
        {
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		gtk_image_set_from_file(GTK_IMAGE(image), (filename));
        }
	gtk_widget_destroy(dialog);

}


int main(int argc, char **argv)
{

	GtkWidget *window, *btn1, *btn2, *quitBtn, *quitImg, *image, *box;
	char *filename;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window), 320, 200);
	gtk_window_set_title(GTK_WINDOW(window), "ReadMe");
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	box = gtk_vbox_new(GTK_ORIENTATION_VERTICAL, 5);

	image = gtk_image_new();
	gtk_box_pack_start(GTK_BOX (box), image, FALSE, FALSE, 0);	

	btn1 = gtk_button_new_with_label("pick a pic");
	gtk_box_pack_start(GTK_BOX (box), btn1, FALSE, FALSE, 0);

  btn2 = gtk_button_new_with_label("ocr it");
  gtk_box_pack_start(GTK_BOX (box), btn2, FALSE, FALSE, 0);

  gtk_container_add(GTK_CONTAINER(window), box);

	g_signal_connect(G_OBJECT(btn1), "clicked", G_CALLBACK(get_filename), image);

	g_signal_connect(G_OBJECT(btn2), "clicked", G_CALLBACK(call), image);

  gtk_widget_show_all(window);

	gtk_main();

	return 0;
}

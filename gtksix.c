#include <gtk/gtk.h>

int main( int argc, char *argv[])
{
    GtkWidget *okno, *label;
    GtkWidget *obraz;

    gtk_init(&argc, &argv);

    okno = gtk_window_new(GTK_WINDOW_POPUP);
    gtk_window_move(GTK_WINDOW(okno), 1710, 880);
    gtk_window_set_default_size(GTK_WINDOW(okno), 100, 100);
    label = gtk_label_new("20");
#if 0
    gtk_container_add(GTK_CONTAINER(okno), label);
#else
    obraz = gtk_image_new_from_file("/home/pi/worksGTK/pobrane.png");
    gtk_container_add(GTK_CONTAINER(okno), obraz);
#endif
    g_signal_connect_swapped(G_OBJECT(okno), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(okno);

    gtk_main();

    return 0;
}

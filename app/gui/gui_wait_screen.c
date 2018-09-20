#include <gtk/gtk.h>
#include "gui_wait_screen.h"

GtkWidget *spin_window, *spin;
int active = 0;

void wait_screen_stop()
{
    printf("++ %s\n", __func__);
    if(active == 1)
    {
        gtk_widget_destroy(GTK_WIDGET(spin_window));
        active = 0;
    }
    printf("-- %s\n", __func__);
}

void wait_screen(GtkApplication *app, gpointer user_data)
{
    printf("++ %s\n", __func__);
    spin_window = gtk_application_window_new (app);
    spin = gtk_spinner_new();

    gtk_window_set_decorated (GTK_WINDOW (spin_window), FALSE);
    gtk_window_fullscreen (GTK_WINDOW (spin_window));
    gtk_window_set_default_size(GTK_WINDOW(spin_window), 1380, 768);
    gtk_container_set_border_width (GTK_CONTAINER(spin_window), 30);
    gtk_container_add(GTK_CONTAINER(spin_window), spin);
    gtk_spinner_start (GTK_SPINNER (spin));

    gtk_widget_show_all(spin_window);
    printf("-- %s\n", __func__);
}

int wait_screen_start()
{
    printf("++ %s\n", __func__);
    int status;
    GtkApplication *app;

    active = 1;
    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (wait_screen), NULL);
    status = g_application_run (G_APPLICATION (app), 0, 0);
    g_object_unref (app);

    printf("-- %s\n", __func__);
    
    return status;
}
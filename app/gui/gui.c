#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "gui.h"

GtkWidget *window, *window1, *window2, *window3, *spin;
gboolean active;
int rect_height, gui_hided, window_active;
char value[5];

gboolean cback(gpointer u)
{
    gtk_main_quit();
    return FALSE;
}

gboolean draw_rect(GtkWidget *widget, cairo_t *cr)
{
    cairo_surface_t *image;

    cairo_rectangle(cr, 0, 0, 60, 200);
    cairo_clip (cr);
    cairo_new_path (cr);
    image = cairo_image_surface_create_from_png ("app/gui/volumebar_background_img.png");
    cairo_image_surface_get_height (image);
    cairo_set_source_surface (cr, image, 0, 0);
    cairo_paint (cr);

    return FALSE;
}

gboolean draw_rect2 (GtkWidget *widget, cairo_t *cr)
{
    int gap = 200 - rect_height;
    cairo_surface_t *image;

    cairo_rectangle(cr, 0, gap, 60, rect_height);
    cairo_clip (cr);
    cairo_new_path (cr);
    image = cairo_image_surface_create_from_png ("app/gui/volumebar_foreground_img.png");
    cairo_set_source_surface (cr, image, 0, 0);
    cairo_paint (cr);

    return FALSE;
}

gboolean draw_vol_value (GtkWidget *widget, cairo_t *cr)
{
    cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size (cr, 29.0);

    if(rect_height > 199)
    {
        cairo_move_to (cr, -1.0, 110.0);
    }
    else if(rect_height < 20)
    {
        cairo_move_to (cr, 20.0, 110.0);
    }
    else
    {
        cairo_move_to (cr, 9.0, 110.0);
    }

    cairo_text_path (cr, value);
    cairo_set_source_rgba (cr, 0, 0, 0, 1);
    cairo_fill_preserve(cr);
    cairo_set_source_rgb (cr, 1, 1, 1);
    cairo_set_line_width (cr, 0.5);
    cairo_stroke (cr);

    return FALSE;
}

void gui_init()
{
    gtk_init(0, 0);
    window = gtk_window_new(GTK_WINDOW_POPUP);
    gtk_window_move(GTK_WINDOW(window), 1280, 548);
    gtk_window_set_default_size(GTK_WINDOW(window), 60, 200);
    gtk_widget_set_app_paintable(window, TRUE);

    g_signal_connect(G_OBJECT(window), "draw", G_CALLBACK(draw_rect), NULL);
    g_signal_connect(G_OBJECT(window), "draw", G_CALLBACK(draw_rect2), NULL);
    g_signal_connect(G_OBJECT(window), "draw", G_CALLBACK(draw_vol_value), NULL);
}

void gui_hide()
{
    if(gui_hided == 0)
    {
        gtk_init(0, 0);
        gtk_widget_hide(window);
        g_timeout_add(5, cback, NULL);
        gtk_main();
        gui_hided = 1;
    }
}

void gui_show_volumebar(int height, char inscription[5])
{
    rect_height = height*2;
    strncpy(value, inscription, 5);
    gui_hided = 0;

    gtk_widget_queue_draw(window);
    gtk_widget_set_visible(window, TRUE);
    g_timeout_add(10, cback, NULL);
    gtk_main();
}

void draw_statement(char statement[40])
{
    GtkWidget *fixed, *button, *label; 

    g_object_get (GTK_SPINNER (spin), "active", &active, NULL);
    if(active)
    {
        gtk_spinner_stop (GTK_SPINNER (spin));
        if(window_active == 3)
        {
            gtk_widget_destroy(window3);
        }
    }

    window_active = 1;
    gtk_init(0, 0);
    window1 = gtk_window_new(GTK_WINDOW_POPUP);
    fixed = gtk_fixed_new();
    label = gtk_label_new(NULL);
    button = gtk_button_new_with_label("OK");

    gtk_window_set_default_size(GTK_WINDOW(window1), 1380, 768);
    gtk_container_add(GTK_CONTAINER(window1), fixed);
    gtk_label_set_markup(GTK_LABEL(label), statement);
    gtk_fixed_put (GTK_FIXED (fixed), label, 590, 345);
    gtk_fixed_put (GTK_FIXED (fixed), button, 645, 385);

    g_timeout_add(50, cback, NULL);
    gtk_widget_set_visible(window1, TRUE);
    gtk_widget_show_all(window1);
    gtk_main();
}

void draw_st_disconnect(char statement[40])
{
    GtkWidget *fixed, *button, *label, *label1; 

    g_object_get (GTK_SPINNER (spin), "active", &active, NULL);
    if(active)
    {
        gtk_spinner_stop (GTK_SPINNER (spin));
        if(window_active == 3)
        {
            gtk_widget_destroy(window3);
        }
    }

    window_active = 2;
    gtk_init(0, 0);
    window2 = gtk_window_new(GTK_WINDOW_POPUP);
    fixed = gtk_fixed_new();
    label = gtk_label_new(NULL);
    label1 = gtk_label_new(NULL);
    button = gtk_button_new_with_label("OK");

    gtk_window_set_default_size(GTK_WINDOW(window2), 1380, 768);
    gtk_container_add(GTK_CONTAINER(window2), fixed);
    gtk_label_set_markup(GTK_LABEL(label), statement);
    gtk_label_set_markup(GTK_LABEL(label1), "To connect press WPS button on your router and press OK");
    gtk_fixed_put (GTK_FIXED (fixed), label, 600, 295);
    gtk_fixed_put (GTK_FIXED (fixed), label1, 438, 345);
    gtk_fixed_put (GTK_FIXED (fixed), button, 645, 385);

    g_timeout_add(50, cback, NULL);
    gtk_widget_set_visible(window2, TRUE);
    gtk_widget_show_all(window2);
    gtk_main();
}

void wait_screen(GtkApplication *app, gpointer user_data)
{
    if(window_active == 2)
    {
        gtk_widget_destroy(window2);
    }
    window_active = 3;
    window3 = gtk_application_window_new (app);
    spin = gtk_spinner_new();

    gtk_window_set_decorated (GTK_WINDOW (window3), FALSE);
    gtk_window_fullscreen (GTK_WINDOW (window3));
    gtk_window_set_default_size(GTK_WINDOW(window3), 1380, 768);
    gtk_container_set_border_width (GTK_CONTAINER(window3), 30);
    gtk_container_add(GTK_CONTAINER(window3), spin);
    gtk_spinner_start (GTK_SPINNER (spin));

    gtk_widget_show_all(window3);
}

int wait_screen_start()
{
  GtkApplication *app;
  int status;
 
  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (wait_screen), NULL);
  status = g_application_run (G_APPLICATION (app), 0, 0);
  g_object_unref (app);
 
  return status;
}

void draw_st_destroy()
{
    switch(window_active)
    {
        gtk_init(0, 0);
        case 1: if(window1 != NULL)
        {
            gtk_widget_destroy(window1);
        }
        break;

        case 2: if(window2 != NULL)
        {
            gtk_widget_destroy(window2);
        }
        break;
        g_timeout_add(5, cback, NULL);
        gtk_main();
    }
}
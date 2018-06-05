#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "gui.h"

GtkWidget *window;
int rect_height, gui_hided;
char value[5];

gboolean draw_rect(GtkWidget *widget, cairo_t *cr)
{
    int img_width = 60, img_height = 200;
    cairo_surface_t *image;

    cairo_rectangle(cr, 0, 0, 60, 200);
    cairo_clip (cr);
    cairo_new_path (cr);
    image = cairo_image_surface_create_from_png ("/home/pi/project/main/volumebar_red.png");
    img_width = cairo_image_surface_get_width (image);
    img_height = cairo_image_surface_get_height (image);
    cairo_set_source_surface (cr, image, 0, 0);
    cairo_paint (cr);

    return FALSE;
}

gboolean draw_rect2 (GtkWidget *widget, cairo_t *cr)
{
    int gap, max_rect_height = 200, img_width = 60;
    gap = max_rect_height - rect_height;
    cairo_surface_t *image;

    cairo_rectangle(cr, 0, gap, 60, rect_height);
    cairo_clip (cr);
    cairo_new_path (cr);
    image = cairo_image_surface_create_from_png ("/home/pi/project/main/volumebar_blue.png");
    img_width = cairo_image_surface_get_width (image);
    max_rect_height = cairo_image_surface_get_height (image);
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
    cairo_set_source_rgba (cr, 0.1, 0.6, 0.1, 1);
    cairo_fill_preserve(cr);

    return FALSE;
}

gboolean cback(gpointer u)
{
    gtk_main_quit();
    return FALSE;
}

void set_gui_hided(int gui_hided_init)
{
    gui_hided = gui_hided_init;
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


#include <gtk/gtk.h>
#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gui_volumebar.h"

GtkWidget *volumebar_window;
int rect_height, gui_hided;
char value[5];

gboolean cback(gpointer u)
{
    printf("++ %s\n", __func__);
    gtk_main_quit();
    printf("-- %s\n", __func__);
    return FALSE;
}

gboolean draw_rect(GtkWidget *widget, cairo_t *cr)
{
    cairo_surface_t *image;

    cairo_rectangle(cr, 0, 0, 60, 200);
    cairo_clip (cr);
    cairo_new_path (cr);
    image = cairo_image_surface_create_from_png ("/home/pi/project/YouTube-Player/app/gui/volumebar_background_img.png");
    cairo_image_surface_get_height (image);
    cairo_set_source_surface (cr, image, 0, 0);
    cairo_paint (cr);
    cairo_surface_destroy (image);

    return FALSE;
}

gboolean draw_rect2 (GtkWidget *widget, cairo_t *cr)
{
    int gap = 200 - rect_height;
    cairo_surface_t *image;

    cairo_rectangle(cr, 0, gap, 60, rect_height);
    cairo_clip (cr);
    cairo_new_path (cr);
    image = cairo_image_surface_create_from_png ("/home/pi/project/YouTube-Player/app/gui/volumebar_foreground_img.png");
    cairo_set_source_surface (cr, image, 0, 0);
    cairo_paint (cr);
    cairo_surface_destroy (image);

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
    printf("++ %s\n", __func__);
    XInitThreads();
    volumebar_window = gtk_window_new(GTK_WINDOW_POPUP);
    gtk_window_move(GTK_WINDOW(volumebar_window), 1820, 840);
    gtk_window_set_default_size(GTK_WINDOW(volumebar_window), 60, 200);
    gtk_widget_set_app_paintable(volumebar_window, TRUE);

    g_signal_connect(G_OBJECT(volumebar_window), "draw", G_CALLBACK(draw_rect), NULL);
    g_signal_connect(G_OBJECT(volumebar_window), "draw", G_CALLBACK(draw_rect2), NULL);
    g_signal_connect(G_OBJECT(volumebar_window), "draw", G_CALLBACK(draw_vol_value), NULL);
    printf("-- %s\n", __func__);
}

void gui_hide()
{
    printf("++ %s\n", __func__);
    if(gui_hided == 0)
    {
        gtk_widget_hide(volumebar_window);
        g_timeout_add(50, cback, NULL);
        gtk_main();
        gui_hided = 1;
    }
    printf("-- %s\n", __func__);
}

void gui_show_volumebar(int height, char inscription[5])
{
    printf("++ %s\n", __func__);
    rect_height = height*2;
    strncpy(value, inscription, 5);
    gui_hided = 0;

    printf("++ before queue draw %s\n", __func__);

    gtk_widget_queue_draw(volumebar_window);
    printf("++ before set visible %s\n", __func__);
    gtk_widget_set_visible(volumebar_window, TRUE);
    printf("++ before timeout add %s\n", __func__);
    g_timeout_add(50, cback, NULL);
    printf("++ before main %s\n", __func__);
    gtk_main();
    printf("-- %s\n", __func__);
}
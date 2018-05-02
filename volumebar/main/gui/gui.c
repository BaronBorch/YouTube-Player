#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gui.h"

bool gui(void);
gboolean draw_rect(GtkWidget *widget, cairo_t *new_cr);
gboolean draw_rect2(GtkWidget *widget, cairo_t *new_cr);
gboolean callback(gpointer u);

char volume_value[5] = "10";


bool gui(void)
{
    int argc; char **argv;
    gtk_init(&argc, &argv);

    GtkWidget *text_view;
    GtkWidget *window = gtk_window_new(GTK_WINDOW_POPUP);
    gtk_window_move(GTK_WINDOW(window), 1280, 548);
    gtk_window_set_default_size(GTK_WINDOW(window), 60, 200);
    gtk_widget_set_app_paintable(window, TRUE);

    g_signal_connect(G_OBJECT(window), "draw", G_CALLBACK(draw_rect), NULL);
    g_signal_connect(G_OBJECT(window), "draw", G_CALLBACK(draw_rect2), NULL);


    gtk_widget_show_all(window);

    g_timeout_add(2000, callback, window);

    gtk_main();

    return 0;
}

gboolean draw_rect(GtkWidget *widget, cairo_t *cr)
{
    GdkRGBA color;
    cairo_rectangle(cr, 0, 0, 60, 200);
    gtk_style_context_get_color (gtk_widget_get_style_context (widget), 0, &color);
    cairo_set_source_rgba (cr, 0.1, 0.2, 0.9, 1.0);
    cairo_fill (cr);

    return FALSE;
}

gboolean draw_rect2 (GtkWidget *widget, cairo_t *cr)
{
    double gap, max_rect_height = 200, rect_height = 20;

    gap = max_rect_height - rect_height;

    GdkRGBA color;
    cairo_rectangle(cr, 0, gap, 60, rect_height);
    gtk_style_context_get_color (gtk_widget_get_style_context (widget), 0, &color);
    cairo_set_source_rgba (cr, 0.1, 0.6, 0.1, 0.6);
    cairo_fill (cr);

    cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size (cr, 30.0);
    cairo_move_to (cr, 8.0, 110.0);
    cairo_text_path (cr, volume_value);
    cairo_set_source_rgb (cr, 1, 0.3, 0);
    cairo_fill_preserve (cr);

    return FALSE;
}

gboolean callback(gpointer u)
{
    //GtkWidget *window;
    //gtk_widget_hide(window);
    gtk_main_quit();
    //return FALSE;

}



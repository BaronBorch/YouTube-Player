#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "gui.h"

gpointer w;

bool gui(int height, char inscription[5]);
gboolean draw_rect(GtkWidget *widget, cairo_t *new_cr);
gboolean draw_rect2(GtkWidget *widget, cairo_t *new_cr);
gboolean cback(gpointer u);
gboolean cback2(gpointer w);

int rect_height;
char value[5];


bool gui(int height, char inscription[5])
{
    rect_height = height*2;
    strncpy(value, inscription, 5);

    gtk_init(0, 0);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_POPUP);
    gtk_window_move(GTK_WINDOW(window), 1280, 548);
    gtk_window_set_default_size(GTK_WINDOW(window), 60, 200);
    gtk_widget_set_app_paintable(window, TRUE);
    //gpointer w = &window;

    g_signal_connect(G_OBJECT(window), "draw", G_CALLBACK(draw_rect), NULL);
    g_signal_connect(G_OBJECT(window), "draw", G_CALLBACK(draw_rect2), NULL);
    //g_signal_connect(G_OBJECT(window), "delete-event", G_CALLBACK(cback2), NULL);
    gtk_widget_show_all(window);

    g_timeout_add(20, cback, NULL);
    //g_timeout_add(10, cback2, NULL);

    //gtk_widget_hide(window);    // dziala, ale musi być przed gtk_main.
    //gtk_window_close(GTK_WINDOW(window));     //dziala, ale musi być przed gtk_main.

    gtk_main();

    return true;
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
    int gap, max_rect_height = 200;

    gap = max_rect_height - rect_height;

    GdkRGBA color;
    cairo_rectangle(cr, 0, gap, 60, rect_height);
    gtk_style_context_get_color (gtk_widget_get_style_context (widget), 0, &color);
    cairo_set_source_rgba (cr, 0.1, 0.6, 0.1, 1.0);
    cairo_fill (cr);

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
    cairo_set_source_rgb (cr, 1, 0.3, 0);
    cairo_fill_preserve (cr);

    return FALSE;
}

gboolean cback(gpointer u)
{
    gtk_main_quit();
    return FALSE;
}

gboolean cback2(gpointer w)
{

    //g_usleep (2000000);
    //gtk_widget_destroy(widget);
    //gtk_widget_hide(widget);
    //gtk_window_close(GTK_WINDOW(w));
    cairo_destroy(w);

    return TRUE;
}




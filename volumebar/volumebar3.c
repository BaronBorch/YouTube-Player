#include <gtk/gtk.h>
#include <stdlib.h>

static gboolean draw_rect(GtkWidget *widget, cairo_t *new_cr);
static gboolean draw_rect2(GtkWidget *widget, cairo_t *new_cr, double volume_bar_height);
static gboolean callback(gpointer userdata);

char volume_value[5] = "10";


int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);

    GtkWidget *text_view;
    GtkWidget *window = gtk_window_new(GTK_WINDOW_POPUP);
    gtk_window_move(GTK_WINDOW(window), 1280, 548);
    gtk_window_set_default_size(GTK_WINDOW(window), 60, 200);
    gtk_widget_set_app_paintable(window, TRUE);

    g_signal_connect(G_OBJECT(window), "draw", G_CALLBACK(draw_rect), NULL);
    g_signal_connect(G_OBJECT(window), "draw", G_CALLBACK(draw_rect2), NULL);


    gtk_widget_show_all(window);

    g_timeout_add(5000, callback, NULL);

    gtk_main();

    return 0;
}

static gboolean draw_rect(GtkWidget *widget, cairo_t *cr)
{
    GdkRGBA color;
    cairo_rectangle(cr, 0, 0, 60, 200);
    gtk_style_context_get_color (gtk_widget_get_style_context (widget), 0, &color);
    cairo_set_source_rgba (cr, 0.1, 0.2, 0.9, 1.0);
    cairo_fill (cr);

    return FALSE;
}

static gboolean draw_rect2 (GtkWidget *widget, cairo_t *cr, double volume_bar_height)
{
    double volume_bar_gap, rect_height = 200;
    volume_bar_height = 20;
    volume_bar_gap = rect_height - volume_bar_height;

    GdkRGBA color;
    cairo_rectangle(cr, 0, volume_bar_gap, 60, volume_bar_height);
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

static gboolean callback(gpointer userdata)
{
    //gtk_main_quit();
    gtk_widget_hide(window);
}



#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gui.h"
#include "gui_treelist.h"

GtkWidget *window, *window2, *window3, *spin;
gboolean active;
event_cb button_OK_connected_cb; 
event_cb connect_with_wps;
int rect_height, gui_hided, window_active, a;
char value[5];

void register_button_OK_connected_callback(event_cb a)
{
    button_OK_connected_cb = a;
}

void register_button_OK_connect_with_wps_callback(event_cb a)
{
    connect_with_wps = a;
}

void call_callbacks(event_cb a)
{
    printf("++ %s\n", __func__);
    if(a != NULL)
    {
        printf(" inside callbac gui\n");
        a();
    }
    printf("-- %s\n", __func__);
}

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
    image = cairo_image_surface_create_from_png ("/home/pi/project/YTPlayer/YouTube-Player/app/gui/volumebar_background_img.png");
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
    image = cairo_image_surface_create_from_png ("/home/pi/project/YTPlayer/YouTube-Player/app/gui/volumebar_foreground_img.png");
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
    gtk_init(0, 0);
    window = gtk_window_new(GTK_WINDOW_POPUP);
    gtk_window_move(GTK_WINDOW(window), 1280, 548);
    gtk_window_set_default_size(GTK_WINDOW(window), 60, 200);
    gtk_widget_set_app_paintable(window, TRUE);

    g_signal_connect(G_OBJECT(window), "draw", G_CALLBACK(draw_rect), NULL);
    g_signal_connect(G_OBJECT(window), "draw", G_CALLBACK(draw_rect2), NULL);
    g_signal_connect(G_OBJECT(window), "draw", G_CALLBACK(draw_vol_value), NULL);
    printf("-- %s\n", __func__);
}

void gui_hide()
{
    printf("++ %s\n", __func__);
    if(gui_hided == 0)
    {
        gtk_widget_hide(window);
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

    gtk_widget_queue_draw(window);
    gtk_widget_set_visible(window, TRUE);
    g_timeout_add(100, cback, NULL);
    gtk_main();
    printf("-- %s\n", __func__);
}

gboolean button1_clicked(GtkWidget *widget, gpointer a)
{
    printf("++ %s\n", __func__);
    int i = GPOINTER_TO_INT(a);
    printf("int i = %d\n", GPOINTER_TO_INT(a));
    if(i == 0)
    {
        call_callbacks(button_OK_connected_cb);
        gtk_widget_destroy(window2);
        gtk_main_quit();
    }
    else if(i == 1)
    {
        gtk_widget_destroy(window2);
        gtk_main_quit();
        draw_statement("", 2);
    }
    else if(i == 2)
    {
        call_callbacks(connect_with_wps);
        gtk_widget_destroy(window2);
        gtk_main_quit();
        wait_screen_start();
    }
    printf("-- %s\n", __func__);
    return FALSE;
}

gboolean button2_clicked()
{
    gtk_widget_destroy(window2);
    gtk_main_quit();
    treelist();
    return FALSE;
}

void draw_statement(char statement[40], int wps_dc)
{
    GtkWidget *fixed, *button1, *button2, *label, *label1, *label2;
    printf("++ %s\n", __func__);

    if(window_active == 3)
    {
        gtk_spinner_stop (GTK_SPINNER (spin));
        gtk_widget_destroy(window3);
        gtk_main_quit();
        printf("   %s spin stop\n", __func__);
    }

    window_active = 2;
    gtk_init(0, 0);
    window2 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    fixed = gtk_fixed_new();
    label = gtk_label_new(NULL);
    label1 = gtk_label_new(NULL);
    label2 = gtk_label_new(NULL);
    button1 = gtk_button_new_with_label("OK");
    button2 = gtk_button_new_with_label("Enter password");
    a = strlen(statement);

    gtk_window_fullscreen (GTK_WINDOW (window2));
    gtk_widget_set_app_paintable(window2, FALSE);
    gtk_window_set_decorated (GTK_WINDOW (window2), FALSE);
    gtk_container_add(GTK_CONTAINER(window2), fixed);

    gtk_label_set_markup(GTK_LABEL(label), statement);
    gtk_fixed_put (GTK_FIXED (fixed), label, 690-(a*5), 295);
    gtk_fixed_put (GTK_FIXED (fixed), button1, 645, 385);

    if(wps_dc == 1)
    {
        gtk_label_set_markup(GTK_LABEL(label1), "Select the connection method");
        gtk_fixed_put(GTK_FIXED (fixed), label1, 550, 345);
        gtk_button_set_label(GTK_BUTTON(button1), "WPS");
        gtk_widget_set_size_request(button1, 200, 50);
        gtk_widget_set_size_request(button2, 200, 50);
        gtk_fixed_put (GTK_FIXED (fixed), button1, 327, 385);
        gtk_fixed_put (GTK_FIXED (fixed), button2, 854, 385);
    }

    if(wps_dc == 2)
    {
        gtk_label_set_markup(GTK_LABEL(label2), "To connect with WiFi press WPS button on your router and press OK");
        gtk_fixed_put (GTK_FIXED (fixed), label2, 405, 345);
    }

    gtk_widget_grab_focus(button1);
    printf("   %s window setup\n", __func__);

    g_signal_connect(button1, "clicked", G_CALLBACK(button1_clicked), (gpointer)GINT_TO_POINTER(wps_dc));
    printf("g signal 1 \n");
    g_signal_connect(button2, "clicked", G_CALLBACK(button2_clicked), NULL);
    printf("g signal 2 \n");

    gtk_widget_queue_draw(window2);
    gtk_widget_set_visible(window2, TRUE);
    printf("set visible \n");
    gtk_widget_show_all(window2);
    printf("   %s window show\n", __func__);
    gtk_main();
    printf("   %s gtk main\n", __func__);
    printf("-- %s\n", __func__);
}

void wait_screen(GtkApplication *app, gpointer user_data)
{
    printf("++ %s\n", __func__);
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
    printf("-- %s\n", __func__);
}

int wait_screen_start()
{
    int status;
    if(window_active != 3)
    {
        printf("++ %s\n", __func__);
        GtkApplication *app;

        app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
        g_signal_connect (app, "activate", G_CALLBACK (wait_screen), NULL);
        status = g_application_run (G_APPLICATION (app), 0, 0);
        g_object_unref (app);

        printf("-- %s\n", __func__);
    }
    return status;
}
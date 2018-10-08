#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "gui_draw_state.h"
#include "gui_treelist.h"
#include "gui_keyboard.h"
#include "gui_wait_screen.h"

event_cb button_OK_connected_cb; 
event_cb connect_with_wps;
event_cb password_connect;
pthread_t thread1;
int window_if_value, refresh = 0;

void register_button_OK_connected_callback(event_cb a)
{
    button_OK_connected_cb = a;
}

void register_button_OK_connect_with_wps_callback(event_cb a)
{
    connect_with_wps = a;
}

void register_connect_with_password(event_cb a)
{
    password_connect = a;
}

void call_callbacks(event_cb a)
{
    if(a != NULL)
    {
        a();
    }
}

void connect_with_password()
{
    call_callbacks(password_connect);
}

void *another_password_connect_func(void *vargp)
{
    call_callbacks(password_connect);
    return 0;
}

void focus_change_color(GtkWidget *widget)
{
    char focused_button_color[100];

    strcpy(focused_button_color, "window #");
    strcat(focused_button_color, gtk_widget_get_name(widget));
    if(gtk_widget_is_focus(widget) == TRUE)
        strcat(focused_button_color, " {background: cyan;}");
    else
    {
        strcat(focused_button_color, " {background: white;}");
    }

    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_data (GTK_CSS_PROVIDER (cssProvider), 
    focused_button_color, -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
            GTK_STYLE_PROVIDER(cssProvider),
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

gboolean allocate_widgets(gpointer a)
{
    int window_width = 1920, window_height = 1080, label_width, label1_width, label2_width;
    GtkWidget *label = g_object_get_data(G_OBJECT(a), "label_data");
    GtkWidget *fixed = g_object_get_data(G_OBJECT(a), "fixed_data");
    GtkWidget *button1 = g_object_get_data(G_OBJECT(a), "button1_data");

    label_width = gtk_widget_get_allocated_width(label);

    if(window_if_value == 0)
    {
        gtk_fixed_move(GTK_FIXED(fixed), label, ((window_width/2) - (label_width/2)), (window_height*2)/5);
        gtk_fixed_move(GTK_FIXED(fixed), button1, ((window_width/2) - 150), (window_height*3)/5);
    }
    else if(window_if_value == 1)
    {
        GtkWidget *label1 = g_object_get_data(G_OBJECT(a), "label1_data");
        GtkWidget *button2 = g_object_get_data(G_OBJECT(a), "button2_data");

        label1_width = gtk_widget_get_allocated_width(label1);

        gtk_fixed_move(GTK_FIXED(fixed), label, ((window_width/2) - (label_width/2)), (window_height*2)/5);
        gtk_fixed_move(GTK_FIXED(fixed), label1, ((window_width/2) - (label1_width/2)), (window_height*5)/10);
        gtk_fixed_move(GTK_FIXED(fixed), button1, ((window_width-600)/3), (window_height*3)/5);
        gtk_fixed_move(GTK_FIXED(fixed), button2, ((((window_width-600)/3)*2)+300), (window_height*3)/5);
    }
    else if(window_if_value == 2)
    {
        GtkWidget *label2 = g_object_get_data(G_OBJECT(a), "label2_data");

        label2_width = gtk_widget_get_allocated_width(label2);

        gtk_fixed_move(GTK_FIXED(fixed), label2, ((window_width/2) - (label2_width/2)), (window_height*2)/5);
        gtk_fixed_move(GTK_FIXED(fixed), button1, ((window_width/2) - 150), (window_height*3)/5);
    }
    refresh = refresh + 1;

    if(refresh >= 3)
    {
        refresh = 0;
        printf("return true");
        return FALSE;
    }
    else
    {
        printf("return false");
        return TRUE;
    }
}

gboolean button1_clicked(GtkWidget *widget, gpointer a)
{
    printf("++ %s\n", __func__);
    printf("int window_if_value = %d\n", window_if_value);
    if(window_if_value == 0)
    {
        call_callbacks(button_OK_connected_cb);
        gtk_widget_destroy(a);
        gtk_main_quit();
    }
    else if(window_if_value == 1)
    {
        gtk_widget_destroy(a);
        gtk_main_quit();
        draw_statement("", 2);
    }
    else if(window_if_value == 2)
    {
        call_callbacks(connect_with_wps);
        gtk_widget_destroy(a);
        gtk_main_quit();
        printf("button 1 clicked if 2 waitscreen start\n");
        wait_screen_start();
    }
    printf("-- %s\n", __func__);
    return FALSE;
}

gboolean button2_clicked(GtkWidget *widget, gpointer a)
{
    printf("++ %s\n", __func__);
    system("wpa_cli scan");
    sleep(1);
    gtk_widget_destroy(a);
    gtk_main_quit();
    treelist();
    pthread_create(&thread1, NULL, another_password_connect_func, NULL);
    wait_screen_start();
    printf("-- %s\n", __func__);
    return FALSE;
}

void draw_statement(char statement[40], int wps_dc)
{
    wait_screen_stop();
    sleep(1);

    GtkWidget *window2, *fixed, *button1, *label;
    GtkCssProvider *cssProvider, *cssProvider1, *cssProvider2; 
    printf("++ %s\n", __func__);

    window2 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    fixed = gtk_fixed_new();
    label = gtk_label_new(NULL);
    button1 = gtk_button_new_with_label("OK");
    cssProvider = gtk_css_provider_new();
    cssProvider1 = gtk_css_provider_new();
    cssProvider2 = gtk_css_provider_new();
    int a = strlen(statement);

    gtk_window_fullscreen(GTK_WINDOW(window2));
    gtk_widget_set_app_paintable(window2, FALSE);
    gtk_window_set_decorated(GTK_WINDOW(window2), FALSE);
    gtk_window_set_deletable(GTK_WINDOW(window2), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW (window2), TRUE);
    gtk_container_add(GTK_CONTAINER(window2), fixed);

    gtk_label_set_markup(GTK_LABEL(label), statement);
    gtk_widget_set_size_request(button1, 300, 75);
    gtk_fixed_put(GTK_FIXED (fixed), label, 690-(a*5), 295);
    gtk_fixed_put(GTK_FIXED (fixed), button1, 645, 385);
    gtk_widget_set_name(button1, "button_2");
    gtk_widget_set_name(label, "label");
    g_object_set_data(G_OBJECT(window2), "label_data", label);
    g_object_set_data(G_OBJECT(window2), "fixed_data", fixed);
    g_object_set_data(G_OBJECT(window2), "button1_data", button1);
    gtk_css_provider_load_from_data(GTK_CSS_PROVIDER (cssProvider), "#label {font-size: 30px;}", -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                               GTK_STYLE_PROVIDER(cssProvider),
                               GTK_STYLE_PROVIDER_PRIORITY_USER);

    if(wps_dc == 1)
    {
        GtkWidget *label1, *button2;
        label1 = gtk_label_new(NULL);
        button2 = gtk_button_new_with_label("Enter password");

        gtk_label_set_markup(GTK_LABEL(label1), "Please select connection method");
        gtk_fixed_put(GTK_FIXED(fixed), label1, 550, 345);
        gtk_button_set_label(GTK_BUTTON(button1), "WPS");
        gtk_widget_set_size_request(button2, 300, 75);
        gtk_fixed_move(GTK_FIXED(fixed), button1, 327, 385);
        gtk_fixed_put(GTK_FIXED(fixed), button2, 854, 385);
        gtk_widget_set_name(button2, "button_3");
        gtk_widget_set_name(label1, "label1");
        g_object_set_data(G_OBJECT(window2), "label1_data", label1);
        g_object_set_data(G_OBJECT(window2), "button2_data", button2);
        gtk_css_provider_load_from_data(GTK_CSS_PROVIDER (cssProvider1), "#label1 {font-size: 30px;}", -1, NULL);
        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                               GTK_STYLE_PROVIDER(cssProvider1),
                               GTK_STYLE_PROVIDER_PRIORITY_USER);
        g_signal_connect(button2, "clicked", G_CALLBACK(button2_clicked), (gpointer)window2);
        g_signal_connect(button1, "event-after", G_CALLBACK(focus_change_color), NULL);
        g_signal_connect(button2, "event-after", G_CALLBACK(focus_change_color), NULL);
    }

    if(wps_dc == 2)
    {
        GtkWidget *label2;
        label2 = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(label2), "To connect with WiFi press WPS button on your router and press OK");
        gtk_fixed_put(GTK_FIXED(fixed), label2, 405, 345);
        gtk_widget_set_name(label2, "label2");
        g_object_set_data(G_OBJECT(window2), "label2_data", label2);
        gtk_css_provider_load_from_data(GTK_CSS_PROVIDER (cssProvider2), "#label2 {font-size: 30px;}", -1, NULL);
        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                               GTK_STYLE_PROVIDER(cssProvider2),
                               GTK_STYLE_PROVIDER_PRIORITY_USER);
    }

    gtk_widget_grab_focus(button1);
    window_if_value = wps_dc;

    g_signal_connect(button1, "clicked", G_CALLBACK(button1_clicked), (gpointer)window2);
    g_timeout_add(100, allocate_widgets, (gpointer)window2);

    gtk_widget_set_visible(window2, TRUE);
    gtk_widget_show_all(window2);
    gtk_main();
    printf("-- %s\n", __func__);
}
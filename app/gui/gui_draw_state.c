#include <gtk/gtk.h>
#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "gui_draw_state.h"
#include "gui_treelist.h"
#include "gui_keyboard.h"

event_cb button_OK_connected_cb, connect_with_wps, password_connect;
GtkWidget *draw_state_window, *spin_window;
pthread_t thread1, thread2;
int window_if_value, refresh = 0, active = 0, thread_status;

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

void *another_password_connect_func(void *vargp)
{
    call_callbacks(password_connect);
    pthread_exit(NULL);
}

void *another_password_connect_func2(void *vargp)
{
    call_callbacks(connect_with_wps);
    pthread_exit(NULL);
}

void focus_change_color(GtkWidget *widget)
{
    char focused_button_color[100];

    strcpy(focused_button_color, "window #");
    strcat(focused_button_color, gtk_widget_get_name(widget));
    if(gtk_widget_is_focus(widget) == TRUE)
        strcat(focused_button_color, " {background: cyan;}");
    else
        strcat(focused_button_color, " {background: white;}");

    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_data (GTK_CSS_PROVIDER (cssProvider), 
    focused_button_color, -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
            GTK_STYLE_PROVIDER(cssProvider),
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

gboolean allocate_widgets(gpointer a)
{
    int window_width = 1920, window_height = 1080, label_width, label1_width;
    GtkWidget *label = g_object_get_data(G_OBJECT(a), "label_data");
    GtkWidget *fixed = g_object_get_data(G_OBJECT(a), "fixed_data");
    GtkWidget *button1 = g_object_get_data(G_OBJECT(a), "button1_data");

    label_width = gtk_widget_get_allocated_width(label);

    if(window_if_value == 0 || window_if_value == 2)
    {
        gtk_fixed_move(GTK_FIXED(fixed), label, ((window_width/2) - (label_width/2)), (window_height*2)/5);
        gtk_fixed_move(GTK_FIXED(fixed), button1, ((window_width/2) - 150), (window_height*3)/5);
    }
    else if(window_if_value == 1)
    {
        GtkWidget *label1 = g_object_get_data(G_OBJECT(a), "label1_data");
        GtkWidget *button2 = g_object_get_data(G_OBJECT(a), "button2_data");
        GtkWidget *button3 = g_object_get_data(G_OBJECT(a), "button3_data");

        label1_width = gtk_widget_get_allocated_width(label1);

        gtk_fixed_move(GTK_FIXED(fixed), label, ((window_width/2) - (label_width/2)), (window_height*2)/5);
        gtk_fixed_move(GTK_FIXED(fixed), label1, ((window_width/2) - (label1_width/2)), (window_height*5)/10);
        gtk_fixed_move(GTK_FIXED(fixed), button3, ((window_width-600)/3), (window_height*3)/5);
        gtk_fixed_move(GTK_FIXED(fixed), button2, ((((window_width-600)/3)*2)+300), (window_height*3)/5);
    }

    refresh = refresh + 1;

    if(refresh >= 3)
    {
        refresh = 0;
        return FALSE;
    }
    else
        return TRUE;
}

void wait_screen_stop()
{
    printf("++ %s\n", __func__);   
    gtk_window_close(GTK_WINDOW(spin_window));
    active = 0;    
    printf("-- %s\n", __func__);
}

gboolean pulse(gpointer data)
{
    if(active == 1)
    {
        gtk_progress_bar_pulse(GTK_PROGRESS_BAR(data));
        return TRUE;
    }
    else
        return FALSE;
}

int wait_screen_start()
{
    printf("++ %s\n", __func__);
    active = 1;
    spin_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *progressBar = gtk_progress_bar_new();
    GtkWidget *spin_fixed = gtk_fixed_new();
    GtkWidget *spin_label = gtk_label_new(NULL);
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    GtkStyleContext *context = gtk_widget_get_style_context(GTK_WIDGET(progressBar));
    
    gtk_window_fullscreen(GTK_WINDOW(spin_window));
    gtk_container_add(GTK_CONTAINER(spin_window), spin_fixed);
    gtk_fixed_put(GTK_FIXED(spin_fixed), progressBar, 210, 700);
    gtk_style_context_add_class(context, GTK_STYLE_CLASS_PROGRESSBAR);
    gtk_widget_set_name(progressBar, "progressBar");
    gtk_label_set_markup(GTK_LABEL(spin_label), "Connecting...\n\nPlease wait");
    gtk_fixed_put(GTK_FIXED (spin_fixed), spin_label, 850, 400);
    gtk_widget_set_name(spin_label, "spin_label");
    gtk_css_provider_load_from_data(GTK_CSS_PROVIDER(cssProvider), "#progressBar {min-height: 50px; min-width: 1500px; color: blue;}"
        "#spin_label {font-size: 40px;}", -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                               GTK_STYLE_PROVIDER(cssProvider),
                               GTK_STYLE_PROVIDER_PRIORITY_USER);
    
    g_timeout_add(90,(GSourceFunc)pulse, progressBar);
    gtk_widget_show_all(spin_window);
    gtk_main();
    printf("-- %s\n", __func__);
    
    return 0;
}

gboolean button1_clicked(GtkWidget *widget)
{
    printf("++ %s\n", __func__);
    printf("int window_if_value = %d\n", window_if_value);
    if(window_if_value == 0)
    {
        printf("thread status = %d\n", thread_status);
        call_callbacks(button_OK_connected_cb);
        gtk_widget_destroy(draw_state_window);
        gtk_main_quit();
    }
    else if(window_if_value == 2)
    {
        pthread_join(thread1, NULL);
        thread_status = pthread_create(&thread2, NULL, another_password_connect_func2, NULL);
        gtk_main_quit();
        wait_screen_start();
    }
    printf("-- %s\n", __func__);
    return FALSE;
}

gboolean button2_clicked(GtkWidget *widget)
{
    printf("++ %s\n", __func__);
    system("wpa_cli scan");
    sleep(1);
    gtk_main_quit();
    treelist();
    pthread_join(thread2, NULL);
    thread_status = pthread_create(&thread1, NULL, another_password_connect_func, NULL);
    wait_screen_start();
    printf("-- %s\n", __func__);
    return FALSE;
}

gboolean button3_clicked(GtkWidget *widget)
{
    draw_statement("", 2);
    return FALSE;
}

void draw_statement_init()
{
    printf("++ %s\n", __func__);
    XInitThreads();
    gtk_init(0, 0);

    draw_state_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *fixed = gtk_fixed_new();
    GtkWidget *label = gtk_label_new(NULL);
    GtkWidget *label1 = gtk_label_new(NULL);
    GtkWidget *button1 = gtk_button_new_with_label("OK");
    GtkWidget *button2 = gtk_button_new_with_label("Password");
    GtkWidget *button3 = gtk_button_new_with_label("WPS");
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    GtkCssProvider *cssProvider1 = gtk_css_provider_new();
  
    gtk_window_fullscreen(GTK_WINDOW(draw_state_window)); 
    gtk_container_add(GTK_CONTAINER(draw_state_window), fixed);

    gtk_widget_set_size_request(button1, 300, 75);
    gtk_widget_set_size_request(button2, 300, 75);
    gtk_widget_set_size_request(button3, 300, 75);

    gtk_widget_set_visible(fixed, TRUE);
    gtk_fixed_put(GTK_FIXED(fixed), label, 850, 295);
    gtk_fixed_put(GTK_FIXED(fixed), label1, 550, 345);
    gtk_fixed_put(GTK_FIXED(fixed), button1, 645, 385);
    gtk_fixed_put(GTK_FIXED(fixed), button2, 854, 385);
    gtk_fixed_put(GTK_FIXED(fixed), button3, 654, 385);

    gtk_widget_set_name(label, "label");
    gtk_widget_set_name(label1, "label1");
    gtk_widget_set_name(button1, "button_1");
    gtk_widget_set_name(button2, "button_2");
    gtk_widget_set_name(button3, "button_3");

    gtk_widget_set_visible(draw_state_window, TRUE);
    gtk_widget_set_visible(label, TRUE);
    
    g_object_set_data(G_OBJECT(draw_state_window), "fixed_data", fixed);
    g_object_set_data(G_OBJECT(draw_state_window), "label_data", label);
    g_object_set_data(G_OBJECT(draw_state_window), "label1_data", label1);
    g_object_set_data(G_OBJECT(draw_state_window), "button1_data", button1);
    g_object_set_data(G_OBJECT(draw_state_window), "button2_data", button2);
    g_object_set_data(G_OBJECT(draw_state_window), "button3_data", button3);

    gtk_css_provider_load_from_data(GTK_CSS_PROVIDER (cssProvider), "#label {font-size: 40px;}", -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                           GTK_STYLE_PROVIDER(cssProvider),
                           GTK_STYLE_PROVIDER_PRIORITY_USER); 
    gtk_css_provider_load_from_data(GTK_CSS_PROVIDER (cssProvider1), "#label1 {font-size: 30px;}", -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                           GTK_STYLE_PROVIDER(cssProvider1),
                           GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_signal_connect(button1, "clicked", G_CALLBACK(button1_clicked), (gpointer)draw_state_window);
    g_signal_connect(button2, "clicked", G_CALLBACK(button2_clicked), (gpointer)draw_state_window);
    g_signal_connect(button3, "clicked", G_CALLBACK(button3_clicked), (gpointer)draw_state_window);
    g_signal_connect(button2, "event-after", G_CALLBACK(focus_change_color), NULL);
    g_signal_connect(button3, "event-after", G_CALLBACK(focus_change_color), NULL);
}

void draw_statement(char statement[40], int wps_dc)
{
    printf("++ %s\n", __func__);
    if(active == 1)
    {
        wait_screen_stop();
        active = 0;
    }

    printf("++ before get data %s\n", __func__);

    GtkWidget *label = g_object_get_data(G_OBJECT(draw_state_window), "label_data");
    GtkWidget *label1 = g_object_get_data(G_OBJECT(draw_state_window), "label1_data");
    GtkWidget *button1 = g_object_get_data(G_OBJECT(draw_state_window), "button1_data");
    GtkWidget *button2 = g_object_get_data(G_OBJECT(draw_state_window), "button2_data");
    GtkWidget *button3 = g_object_get_data(G_OBJECT(draw_state_window), "button3_data");
    window_if_value = wps_dc;

    printf("++ before if 0 %s\n", __func__);

    if(wps_dc == 0)
    {
        gtk_label_set_markup(GTK_LABEL(label), statement);
        gtk_widget_set_visible(label1, FALSE);
        gtk_widget_set_visible(button1, TRUE);
        gtk_widget_set_visible(button2, FALSE);
        gtk_widget_set_visible(button3, FALSE);
        gtk_widget_grab_focus(GTK_WIDGET(button1));
    }

    if(wps_dc == 1)
    {
        printf("++ 7 %s\n", __func__);
        gtk_label_set_markup(GTK_LABEL(label), statement);
        gtk_label_set_markup(GTK_LABEL(label1), "Please select connection method");
        gtk_widget_set_visible(label1, TRUE);
        gtk_widget_set_visible(button1, FALSE);
        gtk_widget_set_visible(button2, TRUE);
        gtk_widget_set_visible(button3, TRUE);
        gtk_widget_grab_focus(GTK_WIDGET(button3));
        printf("++ 14 %s\n", __func__);
    }

    if(wps_dc == 2)
    {
        gtk_label_set_markup(GTK_LABEL(label), "To connect with WiFi press WPS button on your router and press OK");
        gtk_widget_set_visible(label1, FALSE);
        gtk_widget_set_visible(button1, TRUE);
        gtk_widget_set_visible(button2, FALSE);
        gtk_widget_set_visible(button3, FALSE);
        gtk_widget_grab_focus(GTK_WIDGET(button1));
    }

    printf("++ before timeout add %s\n", __func__);
    g_timeout_add(50, allocate_widgets, (gpointer)draw_state_window);

    printf("++ before main %s\n", __func__);

    gtk_main();
    printf("-- %s\n", __func__);
}
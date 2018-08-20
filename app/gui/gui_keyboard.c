#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "gui_keyboard.h"

GtkWidget *window, *fixed, *label;
int shift_counter = 0, caps_check = 0, alt_check = 0;
char text[30] = "", ssid[20];
const gchar *text_label;

void chage_button_bg();

void button_clicked(gpointer a)
{
    text_label = gtk_button_get_label(a);

    if(strcmp("<-", text_label) == 0)
	text[strlen(text)-1] = 0;

    else if(strcmp("Space", text_label) == 0)
	strcat(text, " ");

    else if(strcmp("Tab", text_label) == 0)
	strcat(text, "    ");

    else if(strcmp("Alt", text_label) == 0)
	    alt_check = 1;

    else if(strcmp("Shift", text_label) == 0)
    {
	if(shift_counter != 2)
	    shift_counter = 1;
    }

    else if(strcmp("Caps", text_label) == 0)
    {
	if(shift_counter != 2)
	    shift_counter = 2;
	else 
	    shift_counter = 0;
    }

    else if(shift_counter == 1 || shift_counter == 2)
    {
	if(alt_check == 1)
	{
	    if(strcmp("A", text_label) == 0)
		strcat(text, "Ą");
	    else if(strcmp("C", text_label) == 0)
		strcat(text, "Ć");
	    else if(strcmp("E", text_label) == 0)
		strcat(text, "Ę");
	    else if(strcmp("L", text_label) == 0)
		strcat(text, "Ł");
	    else if(strcmp("N", text_label) == 0)
		strcat(text, "Ń");
	    else if(strcmp("O", text_label) == 0)
		strcat(text, "Ó");
	    else if(strcmp("S", text_label) == 0)
		strcat(text, "Ś");
	    else if(strcmp("X", text_label) == 0)
		strcat(text, "Ź");
	    else if(strcmp("Z", text_label) == 0)
		strcat(text, "Ż");
	    else
		strcat(text, text_label);
	    alt_check = 0;
	}
	else if(alt_check == 0)
	    strcat(text, text_label);

	chage_button_bg();
	if(shift_counter == 1)
	    shift_counter = 0;
    }

    else
    {
	if(alt_check == 1)
	{
	    if(strcmp("A", text_label) == 0)
		strcat(text, "ą");
	    else if(strcmp("C", text_label) == 0)
		strcat(text, "ć");
	    else if(strcmp("E", text_label) == 0)
		strcat(text, "ę");
	    else if(strcmp("L", text_label) == 0)
		strcat(text, "ł");
	    else if(strcmp("N", text_label) == 0)
		strcat(text, "ń");
	    else if(strcmp("O", text_label) == 0)
		strcat(text, "ó");
	    else if(strcmp("S", text_label) == 0)
		strcat(text, "ś");
	    else if(strcmp("X", text_label) == 0)
		strcat(text, "ź");
	    else if(strcmp("Z", text_label) == 0)
		strcat(text, "ż");
	    else
	    {
		char buffor[2];
		strcat(buffor, text_label);
		buffor[1] = tolower(buffor[1]);
		strcat(text, &buffor[1]);
	    }
	    alt_check = 0;
	    chage_button_bg();
	}

	else
	{
	    char buffor[2];
	    strcat(buffor, text_label);
	    buffor[1] = tolower(buffor[1]);
	    strcat(text, &buffor[1]);
	}
    }

    g_print("%s\n", text);
    gtk_label_set_markup(GTK_LABEL(label), text);
}

void double_button_clicked(gpointer a)
{
    text_label = gtk_button_get_label(a);

    if(shift_counter == 1)
    {
	char buffor[5];
	strcpy(buffor, text_label);
	strcpy(&buffor[1], "");

	if(strcmp("&\n7", text_label) == 0)
	    strcat(text, "&amp;");

	else
	    strcat(text, buffor);

	chage_button_bg();
	shift_counter = 0;
    }

    else
	strcat(text, &text_label[2]);

    g_print("%s\n", text);
    gtk_label_set_markup(GTK_LABEL(label), text);
}

void chage_button_bg()
{
    char color[50];

    if((strcmp("Shift", text_label) == 0) && (shift_counter != 2))
    {
	GtkCssProvider *cssProvider = gtk_css_provider_new();
	gtk_css_provider_load_from_data (GTK_CSS_PROVIDER (cssProvider), 
	"window #button_shift_left {background: cyan;}" "window #button_shift_right {background: cyan;}", 
	-1, NULL);
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
			       GTK_STYLE_PROVIDER(cssProvider),
			       GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }

    else if((shift_counter == 1 || shift_counter == 2) && alt_check == 0)
    {
	GtkCssProvider *cssProvider = gtk_css_provider_new();
	gtk_css_provider_load_from_data (GTK_CSS_PROVIDER (cssProvider), 
	"window #button_shift_left {background: gainsboro;}" "window #button_shift_right {background: gainsboro;}", 
	-1, NULL);
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
			       GTK_STYLE_PROVIDER(cssProvider),
			       GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }

    if(strcmp("Caps", text_label) == 0)
    {
	if(caps_check == 0)
	{
	    strcpy(color, "window #button_caps {background: cyan;}");
	    caps_check = 1;
	}
	else
	{
	    strcpy(color, "window #button_caps {background: gainsboro;}");
	    caps_check = 0;
	}
	GtkCssProvider *cssProvider = gtk_css_provider_new();
	gtk_css_provider_load_from_data (GTK_CSS_PROVIDER (cssProvider), 
	color, -1, NULL);
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
			    GTK_STYLE_PROVIDER(cssProvider),
			    GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }

    if((strcmp("Alt", text_label) == 0) || alt_check == 0)
    {
	if(alt_check == 0)
	    strcpy(color, "window #button_alt_right {background: gainsboro;}");
	else
	    strcpy(color, "window #button_alt_right {background: cyan;}");

	GtkCssProvider *cssProvider = gtk_css_provider_new();
	gtk_css_provider_load_from_data (GTK_CSS_PROVIDER (cssProvider), 
	color, -1, NULL);
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
			    GTK_STYLE_PROVIDER(cssProvider),
			    GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }
}

void on_focus_change_color(GtkWidget *widget)
{
    char focused_button_color[100];

    strcpy(focused_button_color, "window #");
    strcat(focused_button_color, gtk_widget_get_name(widget));

    if(gtk_widget_is_focus(widget) == TRUE)
        strcat(focused_button_color, " {background: cyan;}");
    else
    {
        strcat(focused_button_color, " {background: gainsboro;}");
    }

    if(((strcmp("button_caps", gtk_widget_get_name(widget)) == 0) && caps_check != 0) ||
            ((strcmp("button_shift_left", gtk_widget_get_name(widget)) == 0) && shift_counter != 0) ||
            ((strcmp("button_shift_left", gtk_widget_get_name(widget)) == 0) && shift_counter != 0) ||
            ((strcmp("button_alt_right", gtk_widget_get_name(widget)) == 0) && alt_check != 0))
            printf("dont change color now\n");
    else
    {
        GtkCssProvider *cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_data (GTK_CSS_PROVIDER (cssProvider), 
        focused_button_color, -1, NULL);
        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                GTK_STYLE_PROVIDER(cssProvider),
                GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }
}

void enter_clicked()
{
    int error;
    FILE *wpa = fopen("/home/pi/wpa.conf", "w");
    if (wpa == NULL)
	printf("Error opening file!\n");

    char wpaconf[200] =     "network={\n	ssid=\"";
    strcat(wpaconf, ssid);
    strcat(wpaconf, "\"\n	psk=\"");
    strcat(wpaconf, text);
    strcat(wpaconf, "\"\n}");
    error = fprintf(wpa, wpaconf);
    printf("Error z gtk keyboard  = %i\n", error);
    fclose(wpa);

    g_print("Enter clicked, password is: %s\n", text);
    gtk_widget_destroy(window);
    gtk_main_quit();
    call(password);
}

int gui_keyboard(char login[20])
{
    strcpy(ssid, login);

    GtkWidget *b_tylda, *b_1, *b_2, *b_3, *b_4, *b_5, *b_6, *b_7, *b_8, *b_9, *b_0, *b_dash, *b_equal, *b_bspc,
	    *b_tab, *b_q, *b_w, *b_e, *b_r, *b_t, *b_y, *b_u, *b_i, *b_o, *b_p, *b_lbracket, *b_rbracket, *b_rslash,
	    *b_caps, *b_a, *b_s, *b_d, *b_f, *b_g, *b_h, *b_j, *b_k, *b_l, *b_semicolon, *b_quote, *b_enter,
	    *b_lshift, *b_z, *b_x, *b_c, *b_v, *b_b, *b_n, *b_m, *b_comma, *b_dot, *b_lslash, *b_rshift,
	    *b_lctrl, *b_lalt, *b_space, *b_ralt, *b_rctrl;


    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    fixed = gtk_fixed_new();
    label = gtk_label_new(NULL);

    b_tylda = gtk_button_new_with_label("~\n`");
    b_1 = gtk_button_new_with_label("!\n1");
    b_2 = gtk_button_new_with_label("@\n2");
    b_3 = gtk_button_new_with_label("#\n3");
    b_4 = gtk_button_new_with_label("$\n4");
    b_5 = gtk_button_new_with_label("%\n5");
    b_6 = gtk_button_new_with_label("^\n6");
    b_7 = gtk_button_new_with_label("&\n7");
    b_8 = gtk_button_new_with_label("*\n8");
    b_9 = gtk_button_new_with_label("(\n9");
    b_0 = gtk_button_new_with_label(")\n0");
    b_dash = gtk_button_new_with_label("_\n-");
    b_equal = gtk_button_new_with_label("+\n=");
    b_bspc = gtk_button_new_with_label("<-");

    b_tab = gtk_button_new_with_label("Tab");
    b_q = gtk_button_new_with_label("Q");
    b_w = gtk_button_new_with_label("W");
    b_e = gtk_button_new_with_label("E");
    b_r = gtk_button_new_with_label("R");
    b_t = gtk_button_new_with_label("T");
    b_y = gtk_button_new_with_label("Y");
    b_u = gtk_button_new_with_label("U");
    b_i = gtk_button_new_with_label("I");
    b_o = gtk_button_new_with_label("O");
    b_p = gtk_button_new_with_label("P");
    b_lbracket = gtk_button_new_with_label("{\n[");
    b_rbracket = gtk_button_new_with_label("}\n]");
    b_rslash = gtk_button_new_with_label("|\n\\");

    b_caps = gtk_button_new_with_label("Caps");
    b_a = gtk_button_new_with_label("A");
    b_s = gtk_button_new_with_label("S");
    b_d = gtk_button_new_with_label("D");
    b_f = gtk_button_new_with_label("F");
    b_g = gtk_button_new_with_label("G");
    b_h = gtk_button_new_with_label("H");
    b_j = gtk_button_new_with_label("J");
    b_k = gtk_button_new_with_label("K");
    b_l = gtk_button_new_with_label("L");
    b_semicolon = gtk_button_new_with_label(":\n;");
    b_quote = gtk_button_new_with_label("\"\n\'");
    b_enter = gtk_button_new_with_label("Enter");

    b_lshift = gtk_button_new_with_label("Shift");
    b_z = gtk_button_new_with_label("Z");
    b_x = gtk_button_new_with_label("X");
    b_c = gtk_button_new_with_label("C");
    b_v = gtk_button_new_with_label("V");
    b_b = gtk_button_new_with_label("B");
    b_n = gtk_button_new_with_label("N");
    b_m = gtk_button_new_with_label("M");
    b_comma = gtk_button_new_with_label("<\n,");
    b_dot = gtk_button_new_with_label(">\n.");
    b_lslash = gtk_button_new_with_label("?\n/");
    b_rshift = gtk_button_new_with_label("Shift");

    b_lctrl = gtk_button_new_with_label("Ctrl");
    b_lalt = gtk_button_new_with_label("Alt");
    b_space = gtk_button_new_with_label("Space");
    b_ralt = gtk_button_new_with_label("Alt");
    b_rctrl = gtk_button_new_with_label("Ctrl");

//---------------------------------------------------------------------

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 300);
    gtk_widget_set_app_paintable(window, FALSE);
    gtk_window_set_decorated (GTK_WINDOW (window), FALSE);
    gtk_container_add(GTK_CONTAINER(window), fixed);
    gtk_label_set_markup(GTK_LABEL(label), text);
    gtk_fixed_put (GTK_FIXED (fixed), label, 350, 20);


    gtk_widget_set_name (b_tylda, "button_tylda");
    gtk_widget_set_name (b_1, "button_1");
    gtk_widget_set_name (b_2, "button_2");
    gtk_widget_set_name (b_3, "button_3");
    gtk_widget_set_name (b_4, "button_4");
    gtk_widget_set_name (b_5, "button_5");
    gtk_widget_set_name (b_6, "button_6");
    gtk_widget_set_name (b_7, "button_7");
    gtk_widget_set_name (b_8, "button_8");
    gtk_widget_set_name (b_9, "button_9");
    gtk_widget_set_name (b_0, "button_0");
    gtk_widget_set_name (b_dash, "button_dash");
    gtk_widget_set_name (b_equal, "button_equal");
    gtk_widget_set_name (b_bspc, "button_backspace");

    gtk_widget_set_name (b_tab, "button_tab");
    gtk_widget_set_name (b_q, "button_q");
    gtk_widget_set_name (b_w, "button_w");
    gtk_widget_set_name (b_e, "button_e");
    gtk_widget_set_name (b_r, "button_r");
    gtk_widget_set_name (b_t, "button_t");
    gtk_widget_set_name (b_y, "button_y");
    gtk_widget_set_name (b_u, "button_u");
    gtk_widget_set_name (b_i, "button_i");
    gtk_widget_set_name (b_o, "button_o");
    gtk_widget_set_name (b_p, "button_p");
    gtk_widget_set_name (b_lbracket, "button_left_bracket");
    gtk_widget_set_name (b_rbracket, "button_right_bracket");
    gtk_widget_set_name (b_rslash, "button_right_slash");

    gtk_widget_set_name (b_caps, "button_caps");
    gtk_widget_set_name (b_a, "button_a");
    gtk_widget_set_name (b_s, "button_s");
    gtk_widget_set_name (b_d, "button_d");
    gtk_widget_set_name (b_f, "button_f");
    gtk_widget_set_name (b_g, "button_g");
    gtk_widget_set_name (b_h, "button_h");
    gtk_widget_set_name (b_j, "button_j");
    gtk_widget_set_name (b_k, "button_k");
    gtk_widget_set_name (b_l, "button_l");
    gtk_widget_set_name (b_semicolon, "button_semicolon");
    gtk_widget_set_name (b_quote, "button_quote");
    gtk_widget_set_name (b_enter, "button_enter");

    gtk_widget_set_name (b_lshift, "button_shift_left");
    gtk_widget_set_name (b_z, "button_z");
    gtk_widget_set_name (b_x, "button_x");
    gtk_widget_set_name (b_c, "button_c");
    gtk_widget_set_name (b_v, "button_v");
    gtk_widget_set_name (b_b, "button_b");
    gtk_widget_set_name (b_n, "button_n");
    gtk_widget_set_name (b_m, "button_m");
    gtk_widget_set_name (b_comma, "button_comma");
    gtk_widget_set_name (b_dot, "button_dot");
    gtk_widget_set_name (b_lslash, "button_left_slash");
    gtk_widget_set_name (b_rshift, "button_shift_right");

    gtk_widget_set_name (b_lctrl, "button_left_control");
    gtk_widget_set_name (b_lalt, "button_alt_left");
    gtk_widget_set_name (b_space, "button_space");
    gtk_widget_set_name (b_ralt, "button_alt_right");
    gtk_widget_set_name (b_rctrl, "button_right_control");

//--------------------------------------------------------------------

    gtk_widget_set_size_request(b_tylda, 50, 50);
    gtk_widget_set_size_request(b_1, 50, 50);
    gtk_widget_set_size_request(b_2, 50, 50);
    gtk_widget_set_size_request(b_3, 50, 50);
    gtk_widget_set_size_request(b_4, 50, 50);
    gtk_widget_set_size_request(b_5, 50, 50);
    gtk_widget_set_size_request(b_6, 50, 50);
    gtk_widget_set_size_request(b_7, 50, 50);
    gtk_widget_set_size_request(b_8, 50, 50);
    gtk_widget_set_size_request(b_9, 50, 50);
    gtk_widget_set_size_request(b_0, 50, 50);
    gtk_widget_set_size_request(b_dash, 50, 50);
    gtk_widget_set_size_request(b_equal, 50, 50);
    gtk_widget_set_size_request(b_bspc, 85, 50);

    gtk_widget_set_size_request(b_tab, 75, 50);
    gtk_widget_set_size_request(b_q, 50, 50);
    gtk_widget_set_size_request(b_w, 50, 50);
    gtk_widget_set_size_request(b_e, 50, 50);
    gtk_widget_set_size_request(b_r, 50, 50);
    gtk_widget_set_size_request(b_t, 50, 50);
    gtk_widget_set_size_request(b_y, 50, 50);
    gtk_widget_set_size_request(b_u, 50, 50);
    gtk_widget_set_size_request(b_i, 50, 50);
    gtk_widget_set_size_request(b_o, 50, 50);
    gtk_widget_set_size_request(b_p, 50, 50);
    gtk_widget_set_size_request(b_lbracket, 50, 50);
    gtk_widget_set_size_request(b_rbracket, 50, 50);
    gtk_widget_set_size_request(b_rslash, 61, 50);

    gtk_widget_set_size_request(b_caps, 85, 50);
    gtk_widget_set_size_request(b_a, 50, 50);
    gtk_widget_set_size_request(b_s, 50, 50);
    gtk_widget_set_size_request(b_d, 50, 50);
    gtk_widget_set_size_request(b_f, 50, 50);
    gtk_widget_set_size_request(b_g, 50, 50);
    gtk_widget_set_size_request(b_h, 50, 50);
    gtk_widget_set_size_request(b_j, 50, 50);
    gtk_widget_set_size_request(b_k, 50, 50);
    gtk_widget_set_size_request(b_l, 50, 50);
    gtk_widget_set_size_request(b_semicolon, 50, 50);
    gtk_widget_set_size_request(b_quote, 50, 50);
    gtk_widget_set_size_request(b_enter, 102, 50);

    gtk_widget_set_size_request(b_lshift, 110, 50);
    gtk_widget_set_size_request(b_z, 50, 50);
    gtk_widget_set_size_request(b_x, 50, 50);
    gtk_widget_set_size_request(b_c, 50, 50);
    gtk_widget_set_size_request(b_v, 50, 50);
    gtk_widget_set_size_request(b_b, 50, 50);
    gtk_widget_set_size_request(b_n, 50, 50);
    gtk_widget_set_size_request(b_m, 50, 50);
    gtk_widget_set_size_request(b_comma, 50, 50);
    gtk_widget_set_size_request(b_dot, 50, 50);
    gtk_widget_set_size_request(b_lslash, 50, 50);
    gtk_widget_set_size_request(b_rshift, 128, 50);

    gtk_widget_set_size_request(b_lctrl, 65, 50);
    gtk_widget_set_size_request(b_lalt, 55, 50);
    gtk_widget_set_size_request(b_space, 300, 50);
    gtk_widget_set_size_request(b_ralt, 55, 50);
    gtk_widget_set_size_request(b_rctrl, 65, 50);

//---------------------------------------------------------------

    gtk_fixed_put (GTK_FIXED (fixed), b_tylda, 5, 60);
    gtk_fixed_put (GTK_FIXED (fixed), b_1, 56, 60);
    gtk_fixed_put (GTK_FIXED (fixed), b_2, 107, 60);
    gtk_fixed_put (GTK_FIXED (fixed), b_3, 158, 60);
    gtk_fixed_put (GTK_FIXED (fixed), b_4, 209, 60);
    gtk_fixed_put (GTK_FIXED (fixed), b_5, 260, 60);
    gtk_fixed_put (GTK_FIXED (fixed), b_6, 311, 60);
    gtk_fixed_put (GTK_FIXED (fixed), b_7, 362, 60);
    gtk_fixed_put (GTK_FIXED (fixed), b_8, 413, 60);
    gtk_fixed_put (GTK_FIXED (fixed), b_9, 464, 60);
    gtk_fixed_put (GTK_FIXED (fixed), b_0, 515, 60);
    gtk_fixed_put (GTK_FIXED (fixed), b_dash, 567, 60);
    gtk_fixed_put (GTK_FIXED (fixed), b_equal, 619, 60);
    gtk_fixed_put (GTK_FIXED (fixed), b_bspc, 670, 60);

    gtk_fixed_put (GTK_FIXED (fixed), b_tab, 5, 111);
    gtk_fixed_put (GTK_FIXED (fixed), b_q, 81, 111);
    gtk_fixed_put (GTK_FIXED (fixed), b_w, 132, 111);
    gtk_fixed_put (GTK_FIXED (fixed), b_e, 183, 111);
    gtk_fixed_put (GTK_FIXED (fixed), b_r, 234, 111);
    gtk_fixed_put (GTK_FIXED (fixed), b_t, 285, 111);
    gtk_fixed_put (GTK_FIXED (fixed), b_y, 336, 111);
    gtk_fixed_put (GTK_FIXED (fixed), b_u, 387, 111);
    gtk_fixed_put (GTK_FIXED (fixed), b_i, 438, 111);
    gtk_fixed_put (GTK_FIXED (fixed), b_o, 489, 111);
    gtk_fixed_put (GTK_FIXED (fixed), b_p, 540, 111);
    gtk_fixed_put (GTK_FIXED (fixed), b_lbracket, 591, 111);
    gtk_fixed_put (GTK_FIXED (fixed), b_rbracket, 642, 111);
    gtk_fixed_put (GTK_FIXED (fixed), b_rslash, 693, 111);

    gtk_fixed_put (GTK_FIXED (fixed), b_caps, 5, 162);
    gtk_fixed_put (GTK_FIXED (fixed), b_a, 91, 162);
    gtk_fixed_put (GTK_FIXED (fixed), b_s, 142, 162);
    gtk_fixed_put (GTK_FIXED (fixed), b_d, 193, 162);
    gtk_fixed_put (GTK_FIXED (fixed), b_f, 244, 162);
    gtk_fixed_put (GTK_FIXED (fixed), b_g, 295, 162);
    gtk_fixed_put (GTK_FIXED (fixed), b_h, 346, 162);
    gtk_fixed_put (GTK_FIXED (fixed), b_j, 397, 162);
    gtk_fixed_put (GTK_FIXED (fixed), b_k, 448, 162);
    gtk_fixed_put (GTK_FIXED (fixed), b_l, 499, 162);
    gtk_fixed_put (GTK_FIXED (fixed), b_semicolon, 550, 162);
    gtk_fixed_put (GTK_FIXED (fixed), b_quote, 601, 162);
    gtk_fixed_put (GTK_FIXED (fixed), b_enter, 652, 162);

    gtk_fixed_put (GTK_FIXED (fixed), b_lshift, 5, 213);
    gtk_fixed_put (GTK_FIXED (fixed), b_z, 116, 213);
    gtk_fixed_put (GTK_FIXED (fixed), b_x, 167, 213);
    gtk_fixed_put (GTK_FIXED (fixed), b_c, 218, 213);
    gtk_fixed_put (GTK_FIXED (fixed), b_v, 269, 213);
    gtk_fixed_put (GTK_FIXED (fixed), b_b, 320, 213);
    gtk_fixed_put (GTK_FIXED (fixed), b_n, 371, 213);
    gtk_fixed_put (GTK_FIXED (fixed), b_m, 422, 213);
    gtk_fixed_put (GTK_FIXED (fixed), b_comma, 473, 213);
    gtk_fixed_put (GTK_FIXED (fixed), b_dot, 524, 213);
    gtk_fixed_put (GTK_FIXED (fixed), b_lslash, 575, 213);
    gtk_fixed_put (GTK_FIXED (fixed), b_rshift, 626, 213);

    gtk_fixed_put (GTK_FIXED (fixed), b_lctrl, 108, 264);
    gtk_fixed_put (GTK_FIXED (fixed), b_lalt, 174, 264);
    gtk_fixed_put (GTK_FIXED (fixed), b_space, 230, 264);
    gtk_fixed_put (GTK_FIXED (fixed), b_ralt, 531, 264);
    gtk_fixed_put (GTK_FIXED (fixed), b_rctrl, 587, 264);

//-------------------------------------------------------------------------------------------------------------

    g_signal_connect(b_tylda, "clicked", G_CALLBACK(double_button_clicked), (gpointer)b_tylda);
    g_signal_connect(b_1, "clicked", G_CALLBACK(double_button_clicked), (gpointer)b_1);
    g_signal_connect(b_2, "clicked", G_CALLBACK(double_button_clicked), (gpointer)b_2);
    g_signal_connect(b_3, "clicked", G_CALLBACK(double_button_clicked), (gpointer)b_3);
    g_signal_connect(b_4, "clicked", G_CALLBACK(double_button_clicked), (gpointer)b_4);
    g_signal_connect(b_5, "clicked", G_CALLBACK(double_button_clicked), (gpointer)b_5);
    g_signal_connect(b_6, "clicked", G_CALLBACK(double_button_clicked), (gpointer)b_6);
    g_signal_connect(b_7, "clicked", G_CALLBACK(double_button_clicked), (gpointer)b_7);
    g_signal_connect(b_8, "clicked", G_CALLBACK(double_button_clicked), (gpointer)b_8);
    g_signal_connect(b_9, "clicked", G_CALLBACK(double_button_clicked), (gpointer)b_9);
    g_signal_connect(b_0, "clicked", G_CALLBACK(double_button_clicked), (gpointer)b_0);
    g_signal_connect(b_dash, "clicked", G_CALLBACK(double_button_clicked), (gpointer)b_dash);
    g_signal_connect(b_equal, "clicked", G_CALLBACK(double_button_clicked), (gpointer)b_equal);
    g_signal_connect(b_bspc, "clicked", G_CALLBACK(button_clicked), (gpointer)b_bspc);

    g_signal_connect(b_tab, "clicked", G_CALLBACK(button_clicked), (gpointer)b_tab);
    g_signal_connect(b_q, "clicked", G_CALLBACK(button_clicked), (gpointer)b_q);
    g_signal_connect(b_w, "clicked", G_CALLBACK(button_clicked), (gpointer)b_w);
    g_signal_connect(b_e, "clicked", G_CALLBACK(button_clicked), (gpointer)b_e);
    g_signal_connect(b_r, "clicked", G_CALLBACK(button_clicked), (gpointer)b_r);
    g_signal_connect(b_t, "clicked", G_CALLBACK(button_clicked), (gpointer)b_t);
    g_signal_connect(b_y, "clicked", G_CALLBACK(button_clicked), (gpointer)b_y);
    g_signal_connect(b_u, "clicked", G_CALLBACK(button_clicked), (gpointer)b_u);
    g_signal_connect(b_i, "clicked", G_CALLBACK(button_clicked), (gpointer)b_i);
    g_signal_connect(b_o, "clicked", G_CALLBACK(button_clicked), (gpointer)b_o);
    g_signal_connect(b_p, "clicked", G_CALLBACK(button_clicked), (gpointer)b_p);
    g_signal_connect(b_lbracket, "clicked", G_CALLBACK(double_button_clicked), (gpointer)b_lbracket);
    g_signal_connect(b_rbracket, "clicked", G_CALLBACK(double_button_clicked), (gpointer)b_rbracket);
    g_signal_connect(b_rslash, "clicked", G_CALLBACK(double_button_clicked), (gpointer)b_rslash);

    g_signal_connect(b_caps, "clicked", G_CALLBACK(button_clicked), (gpointer)b_caps);
    g_signal_connect(b_a, "clicked", G_CALLBACK(button_clicked), (gpointer)b_a);
    g_signal_connect(b_s, "clicked", G_CALLBACK(button_clicked), (gpointer)b_s);
    g_signal_connect(b_d, "clicked", G_CALLBACK(button_clicked), (gpointer)b_d);
    g_signal_connect(b_f, "clicked", G_CALLBACK(button_clicked), (gpointer)b_f);
    g_signal_connect(b_g, "clicked", G_CALLBACK(button_clicked), (gpointer)b_g);
    g_signal_connect(b_h, "clicked", G_CALLBACK(button_clicked), (gpointer)b_h);
    g_signal_connect(b_j, "clicked", G_CALLBACK(button_clicked), (gpointer)b_j);
    g_signal_connect(b_k, "clicked", G_CALLBACK(button_clicked), (gpointer)b_k);
    g_signal_connect(b_l, "clicked", G_CALLBACK(button_clicked), (gpointer)b_l);
    g_signal_connect(b_semicolon, "clicked", G_CALLBACK(double_button_clicked), (gpointer)b_semicolon);
    g_signal_connect(b_quote, "clicked", G_CALLBACK(double_button_clicked), (gpointer)b_quote);
    g_signal_connect(b_enter, "clicked", G_CALLBACK(enter_clicked), NULL);

    g_signal_connect(b_lshift, "clicked", G_CALLBACK(button_clicked), (gpointer)b_lshift);
    g_signal_connect(b_z, "clicked", G_CALLBACK(button_clicked), (gpointer)b_z);
    g_signal_connect(b_x, "clicked", G_CALLBACK(button_clicked), (gpointer)b_x);
    g_signal_connect(b_c, "clicked", G_CALLBACK(button_clicked), (gpointer)b_c);
    g_signal_connect(b_v, "clicked", G_CALLBACK(button_clicked), (gpointer)b_v);
    g_signal_connect(b_b, "clicked", G_CALLBACK(button_clicked), (gpointer)b_b);
    g_signal_connect(b_n, "clicked", G_CALLBACK(button_clicked), (gpointer)b_n);
    g_signal_connect(b_m, "clicked", G_CALLBACK(button_clicked), (gpointer)b_m);
    g_signal_connect(b_comma, "clicked", G_CALLBACK(double_button_clicked), (gpointer)b_comma);
    g_signal_connect(b_dot, "clicked", G_CALLBACK(double_button_clicked), (gpointer)b_dot);
    g_signal_connect(b_lslash, "clicked", G_CALLBACK(double_button_clicked), (gpointer)b_lslash);
    g_signal_connect(b_rshift, "clicked", G_CALLBACK(button_clicked), (gpointer)b_rshift);

    g_signal_connect(b_space, "clicked", G_CALLBACK(button_clicked), (gpointer)b_space);
    g_signal_connect(b_ralt, "clicked", G_CALLBACK(button_clicked), (gpointer)b_ralt);

    g_signal_connect(b_lshift, "clicked", G_CALLBACK(chage_button_bg), NULL);
    g_signal_connect(b_rshift, "clicked", G_CALLBACK(chage_button_bg), NULL);
    g_signal_connect(b_caps, "clicked", G_CALLBACK(chage_button_bg), NULL);
    g_signal_connect(b_ralt, "clicked", G_CALLBACK(chage_button_bg), NULL);


    g_signal_connect(b_tylda, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_1, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_2, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_3, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_4, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_5, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_6, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_7, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_8, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_9, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_0, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_dash, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_equal, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_bspc, "event-after", G_CALLBACK(on_focus_change_color), NULL);

    g_signal_connect(b_tab, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_q, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_w, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_e, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_r, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_t, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_y, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_u, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_i, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_o, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_p, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_lbracket, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_rbracket, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_rslash, "event-after", G_CALLBACK(on_focus_change_color), NULL);

    g_signal_connect(b_caps, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_a, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_s, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_d, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_f, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_g, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_h, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_j, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_k, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_l, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_semicolon, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_quote, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_enter, "event-after", G_CALLBACK(on_focus_change_color), NULL);

    g_signal_connect(b_lshift, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_z, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_x, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_c, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_v, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_b, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_n, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_m, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_comma, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_dot, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_lslash, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_rshift, "event-after", G_CALLBACK(on_focus_change_color), NULL);

    g_signal_connect(b_lctrl, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_lalt, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_space, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_ralt, "event-after", G_CALLBACK(on_focus_change_color), NULL);
    g_signal_connect(b_rctrl, "event-after", G_CALLBACK(on_focus_change_color), NULL);

//---------------------------------------------------------------------------------------------------------------------

    gtk_widget_grab_focus(b_a);
    gtk_widget_set_visible(window, TRUE);
    gtk_widget_show_all(window);
    printf("   %s window show\n", __func__);
    gtk_main();
    return 0;
}
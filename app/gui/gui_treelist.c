#include <gtk/gtk.h>
#include "gui_treelist.h"
#include "gui_keyboard.h"

gchar *value;

enum
{
    LIST_ITEM = 0,
    N_COLUMNS
};

gboolean button_click(GtkWidget *widget, GdkEvent *event, gpointer a)
{
    printf("++ %s\n", __func__);
    guint keyval;
    GtkWidget *label;

    label = g_object_get_data(G_OBJECT(widget), "label");
    gdk_event_get_keyval(event, &keyval);

    if(keyval == GDK_KEY_Return)
    {
        g_print ("%s\n", value);
        gtk_widget_hide(a);
        gtk_label_set_markup(GTK_LABEL(label), "Enter password: ");

        gui_keyboard(value);
        gtk_widget_destroy(GTK_WIDGET(widget));
        gtk_main_quit();
    }
    return FALSE;
    printf("-- %s\n", __func__);
}

void init_list(GtkWidget *list)
{
    printf("++ %s\n", __func__);
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkListStore *store;

    renderer = gtk_cell_renderer_text_new ();
    store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
    column = gtk_tree_view_column_new_with_attributes("List Items",
                                renderer, "text", LIST_ITEM, NULL);

    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);
    gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));
    g_object_unref(store);
    printf("-- %s\n", __func__);
}

void add_to_list(GtkWidget *list, const gchar *str)
{
    printf("++ %s\n", __func__);
    GtkListStore *store;
    GtkTreeIter iter;

    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));

    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter, LIST_ITEM, str, -1);
    printf("-- %s\n", __func__);
}

void on_changed(GtkWidget *widget)
{
    printf("++ %s\n", __func__);
    GtkTreeIter iter;
    GtkTreeModel *model;

    if(gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), &model, &iter))
    {
    gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
    g_print ("activated: %s\n", value);
    }
    printf("-- %s\n", __func__);
}

int treelist()
{
    printf("++ %s\n", __func__);

    GtkWidget *window, *list, *fixed, *label;
    GtkTreeSelection *selection;
    GtkCssProvider *cssProvider;
    FILE *file_d;
    char web[20], web1[20], web2[20], web3[20], web4[20], web5[20], web6[20], web7[20], web8[20], web9[20];
    int not, not1, not2, not3, not4, not5, not6, not7, not8, not9;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    list = gtk_tree_view_new();
    fixed = gtk_fixed_new();
    label = gtk_label_new(NULL);
    cssProvider = gtk_css_provider_new();

    gtk_window_fullscreen (GTK_WINDOW (window));
    gtk_widget_set_app_paintable(window, FALSE);
    gtk_window_set_decorated (GTK_WINDOW (window), FALSE);

    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);
    gtk_widget_set_size_request(list, 300, 10);
    gtk_fixed_put (GTK_FIXED (fixed), list, 500, 220);
    gtk_label_set_markup(GTK_LABEL(label), "Select network: ");
    gtk_fixed_put (GTK_FIXED (fixed), label, 550, 160);
    gtk_widget_set_name (label, "label");
    gtk_css_provider_load_from_data (GTK_CSS_PROVIDER (cssProvider),
        "#label {font-size: 25px;}", -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                               GTK_STYLE_PROVIDER(cssProvider),
                               GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_container_add(GTK_CONTAINER(window), fixed);

    init_list(list);

    file_d = popen("wpa_cli scan_results |sed 's/^.*]/]/' | tr -d '[] ]'|sed -n 3p", "r");
    not = fscanf(file_d, "%s", web);
    file_d = popen("wpa_cli scan_results |sed 's/^.*]/]/' | tr -d '[] ]'|sed -n 4p", "r");
    not1 = fscanf(file_d, "%s", web1);
    file_d = popen("wpa_cli scan_results |sed 's/^.*]/]/' | tr -d '[] ]'|sed -n 5p", "r");
    not2 = fscanf(file_d, "%s", web2);
    file_d = popen("wpa_cli scan_results |sed 's/^.*]/]/' | tr -d '[] ]'|sed -n 6p", "r");
    not3 = fscanf(file_d, "%s", web3);
    file_d = popen("wpa_cli scan_results |sed 's/^.*]/]/' | tr -d '[] ]'|sed -n 7p", "r");
    not4 = fscanf(file_d, "%s", web4);
    file_d = popen("wpa_cli scan_results |sed 's/^.*]/]/' | tr -d '[] ]'|sed -n 8p", "r");
    not5 = fscanf(file_d, "%s", web5);
    file_d = popen("wpa_cli scan_results |sed 's/^.*]/]/' | tr -d '[] ]'|sed -n 9p", "r");
    not6 = fscanf(file_d, "%s", web6);
    file_d = popen("wpa_cli scan_results |sed 's/^.*]/]/' | tr -d '[] ]'|sed -n 10p", "r");
    not7 = fscanf(file_d, "%s", web7);
    file_d = popen("wpa_cli scan_results |sed 's/^.*]/]/' | tr -d '[] ]'|sed -n 11p", "r");
    not8 = fscanf(file_d, "%s", web8);
    file_d = popen("wpa_cli scan_results |sed 's/^.*]/]/' | tr -d '[] ]'|sed -n 12p", "r");
    not9 = fscanf(file_d, "%s", web9);

    if(not >= 0)
        add_to_list(list, web);
    if(not1 >= 0)
        add_to_list(list, web1);
    if(not2 >= 0)
        add_to_list(list, web2);
    if(not3 >= 0)
        add_to_list(list, web3);
    if(not4 >= 0)
        add_to_list(list, web4);
    if(not5 >= 0)
        add_to_list(list, web5);
    if(not6 >= 0)
        add_to_list(list, web6);
    if(not7 >= 0)
        add_to_list(list, web7);
    if(not8 >= 0)
        add_to_list(list, web8);
    if(not9 >= 0)
        add_to_list(list, web9);

    pclose(file_d);

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));

    g_object_set_data(G_OBJECT(window), "label", label);
    g_signal_connect(G_OBJECT(selection), "changed", G_CALLBACK(on_changed), NULL);
    g_signal_connect(G_OBJECT(window), "key-press-event", G_CALLBACK (button_click), (gpointer)list);

    gtk_widget_show_all(window);
    gtk_main();

    printf("-- %s\n", __func__);

    return 0;
}
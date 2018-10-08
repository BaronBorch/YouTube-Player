#include <gtk/gtk.h>
#include <string.h>
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

    gdk_event_get_keyval(event, &keyval);

    if(keyval == GDK_KEY_Return)
    {
        gtk_widget_destroy(GTK_WIDGET(widget));
        gui_keyboard(value);
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

gboolean allocate_widget(gpointer a)
{
    int window_width = 1920, window_height = 1080, label_width, list_width, list_height;
    GtkWidget *label = g_object_get_data(G_OBJECT(a), "label_data");
    GtkWidget *fixed = g_object_get_data(G_OBJECT(a), "fixed_data");
    GtkWidget *list = g_object_get_data(G_OBJECT(a), "list_data");

    label_width = gtk_widget_get_allocated_width(label);
    list_width = gtk_widget_get_allocated_width(list);
    list_height = gtk_widget_get_allocated_height(list);

    gtk_fixed_move(GTK_FIXED(fixed), list, ((window_width/2) - (list_width/2)), (window_height/2)-(list_height/2));
    gtk_fixed_move(GTK_FIXED(fixed), label, ((window_width/2) - (label_width/2)), ((window_height/2)-(list_height/2)) - 60);

    if(label_width > 1)
        return FALSE;
    else
        return TRUE;
}

int treelist()
{
    printf("++ %s\n", __func__);

    GtkWidget *window, *list, *fixed, *label;
    GtkTreeSelection *selection;
    GtkCssProvider *cssProvider;
    FILE *file_d;
    char web[50][50], char_i[5], command[80];
    int i, nr_of_lines, not[50];

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
    g_object_set_data(G_OBJECT(window), "label_data", label);
    g_object_set_data(G_OBJECT(window), "fixed_data", fixed);
    g_object_set_data(G_OBJECT(window), "list_data", list);
    gtk_css_provider_load_from_data (GTK_CSS_PROVIDER (cssProvider),
        "#label {font-size: 30px;}", -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                               GTK_STYLE_PROVIDER(cssProvider),
                               GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_container_add(GTK_CONTAINER(window), fixed);

    init_list(list);

    FILE *file_e = popen("wpa_cli scan_results |wc -l", "r");
    fscanf(file_e, "%i", &nr_of_lines);
    printf("number of lines = %d\n", nr_of_lines);

    for(i = 0; i < (nr_of_lines - 2); i++)
    {
        strcpy(command, "wpa_cli scan_results |sed 's/^.*]/]/' | tr -d '[] ]'|sed -n ");
        sprintf(char_i,"%d", i + 3);
        strcat(command, char_i);
        strcat(command, "p"); 
        file_d = popen(command, "r");
        not[i] = fscanf(file_d, "%s", web[i]);
    }

    for(i = 0; i < (nr_of_lines - 2); i++)
    {
        if(not[i] >= 0)
            add_to_list(list, web[i]);
    }

    pclose(file_d);
    pclose(file_e);

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));

    g_signal_connect(G_OBJECT(selection), "changed", G_CALLBACK(on_changed), NULL);
    g_signal_connect(G_OBJECT(window), "key-press-event", G_CALLBACK (button_click), (gpointer)list);
    g_timeout_add(100, allocate_widget, (gpointer)window);

    gtk_widget_show_all(window);
    gtk_main();

    printf("-- %s\n", __func__);

    return 0;
}
#ifndef gui_H
#define gui_H

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool gui(double height, char inscription[5]);
gboolean draw_rect(GtkWidget *widget, cairo_t *new_cr);
gboolean draw_rect2(GtkWidget *widget, cairo_t *new_cr);



#endif

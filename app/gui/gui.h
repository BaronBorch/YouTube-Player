#ifndef gui_H
#define gui_H

typedef void (*event_cb)();

void gui_init();
void gui_show_volumebar(int height, char inscription[5]);
void gui_hide();
void draw_statement(char statement[40], int wps_dc);
int wait_screen_start();
void register_button_OK_connected_callback(event_cb a);
void register_button_OK_connect_with_wps_callback(event_cb a);
void register_connect_with_password(event_cb a);

#endif
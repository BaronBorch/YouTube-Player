#ifndef gui_draw_state_H
#define gui_draw_state_H

typedef void (*event_cb)();

void draw_statement_init();
void draw_statement(char statement[40], int wps_dc);
int wait_screen_start();
void register_button_OK_connected_callback(event_cb a);
void register_button_OK_connect_with_wps_callback(event_cb a);
void register_connect_with_password(event_cb a);

#endif
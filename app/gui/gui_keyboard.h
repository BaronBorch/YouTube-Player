#ifndef gui_keyboard_H
#define gui_keyboard_H

typedef void (*event_cb)();

int gui_keyboard(char login[20]);
void register_password_ready(event_cb a);

#endif
#ifndef gui_H
#define gui_H

void gui_init();
void gui_show_volumebar(int height, char inscription[5]);
void gui_hide();
void draw_statement(char statement[40]);
void draw_st_disconnect(char statement[40]);
int wait_screen_start();
void draw_st_destroy();

#endif
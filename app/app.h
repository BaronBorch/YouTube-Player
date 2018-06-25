#ifndef app_H
#define app_H

typedef void (*event_cb)();
void app();
void register_internet_conection(event_cb a);

#endif

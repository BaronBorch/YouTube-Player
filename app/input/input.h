#ifndef input_H
#define input_H

typedef void (*event_cb)();
void input_read_start();
void register_volume_up_callback(event_cb a);
void register_volume_down_callback(event_cb a);
void register_volume_mute_callback(event_cb a);
void register_power_callback(event_cb a);
void register_enter_callback(event_cb a);

#endif

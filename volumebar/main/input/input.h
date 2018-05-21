#ifndef input_H
#define input_H

typedef bool (*event_cb)();
void input_read(void);
event_cb volume_changed_callback;

#endif

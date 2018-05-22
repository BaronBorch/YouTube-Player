#define key ev.code

#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "input.h"


pthread_t thread;
event_cb global_cb;

void register_volume_up_callback(event_cb a)
{
    global_cb = a;
}
void register_volume_down_callback(event_cb a)
{
    global_cb = a;
}
void register_volume_mute_callback(event_cb a)
{
    global_cb = a;
}
void register_power_callback(event_cb a)
{
    global_cb = a;
}

void *input_read(void *vargp)
{
    char devname[] = "/dev/input/event0";
    int device = open(devname, O_RDONLY);
    struct input_event ev;

    while(1)
    {
        read(device,&ev, sizeof(ev));

        switch(key)
        {
            case KEY_VOLUMEDOWN: global_cb(register_volume_down_callback);
            break;
            case KEY_VOLUMEUP: global_cb(register_volume_up_callback);
            break;
            case KEY_MUTE: global_cb(register_volume_mute_callback);
            break;
            case KEY_SLEEP: global_cb(register_power_callback);
            break;
        }
    }
}

void input_read_start()
{
    pthread_create(&thread, NULL, input_read, NULL);
}
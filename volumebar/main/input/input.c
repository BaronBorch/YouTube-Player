#define key ev.code
#define key_state ev.value

#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "input.h"

pthread_t thread;
event_cb key_volume_up_cb;
event_cb key_volume_down_cb;
event_cb key_mute_cb;
event_cb key_power_cb;

void register_volume_up_callback(event_cb a)
{
    key_volume_up_cb = a;
}
void register_volume_down_callback(event_cb a)
{
    key_volume_down_cb = a;
}
void register_volume_mute_callback(event_cb a)
{
    key_mute_cb = a;
}
void register_power_callback(event_cb a)
{
    key_power_cb = a;
}

void call_callback(event_cb a)
{
    if(a != NULL)
    {
        a();
    }
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
            case KEY_VOLUMEUP:
            if(key_state != 1)
            {call_callback(key_volume_up_cb);}
            break;
            case KEY_VOLUMEDOWN:
            if(key_state != 1)
            {call_callback(key_volume_down_cb);}
            break;
            case KEY_MUTE:
            if(key_state != 1)
            {call_callback(key_mute_cb);}
            break;
            case KEY_SLEEP:
            if(key_state != 1)
            {call_callback(key_power_cb);}
            break;
        }
    }
}

void input_read_start()
{
    pthread_create(&thread, NULL, input_read, NULL);
}
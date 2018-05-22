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

void register_volume_up_callback(event_cb a)
{
    if(a != NULL)
    {
    a();
    }
}
void register_volume_down_callback(event_cb a)
{
    if(a != NULL)
    {
    a();
    }
}
void register_volume_mute_callback(event_cb a)
{
    if(a != NULL)
    {
    a();
    }
}
void register_power_callback(event_cb a)
{
    if(a != NULL)
    {
    a();
    }
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
            case KEY_VOLUMEDOWN: call_callback(register_volume_down_callback);
            break;
            case KEY_VOLUMEUP: call_callback(register_volume_up_callback);
            break;
            case KEY_MUTE: call_callback(register_volume_mute_callback);
            break;
            case KEY_SLEEP: call_callback(register_power_callback);
            break;
        }
    }
}

void input_read_start()
{
    pthread_create(&thread, NULL, input_read, NULL);
}
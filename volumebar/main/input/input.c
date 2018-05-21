#define key ev.code

#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include "input.h"

int c;

bool my_callback()
{
    if(c == 1) {return true;}
    else {return false;}
}

event_cb volume_changed_callback = my_callback;

bool volume_change_register(event_cb cb)
{
    volume_changed_callback = cb;
}

void input_read(void)
{
    char devname[] = "/dev/input/event0";
    int device = open(devname, O_RDONLY);
    struct input_event ev;

    while(1)
    {
        c = 0;
        read(device,&ev, sizeof(ev));

        switch(key)
        {
            case KEY_VOLUMEDOWN: {c = 1; volume_change_register(my_callback);}
            break;
            case KEY_VOLUMEUP: {c = 1; volume_change_register(my_callback);}
            break;
            case KEY_MUTE: {c = 1; volume_change_register(my_callback);}
            break;
            case KEY_SLEEP: system("shutdown -h now");
            break;
        }
    }
}
#define key ev.code
#define pressed ev.value

#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include "input.h"

bool input_read(void)
{
    char devname[] = "/dev/input/event0";
    int device = open(devname, O_RDONLY);
    struct input_event ev;

    while(1)
    {
        read(device,&ev, sizeof(ev));

        if((key == KEY_VOLUMEDOWN && pressed == 0) || (key == KEY_VOLUMEUP && pressed == 0) || (key == KEY_MUTE && pressed == 0))
        {
            return true;
        }
        else if((key == KEY_VOLUMEDOWN && pressed == 2) || (key == KEY_VOLUMEUP && pressed == 2) || (key == KEY_MUTE && pressed == 2))
        {
            return true;
        }
        else if(key == KEY_SLEEP && pressed == 0)
        {
            system("shutdown -h now");
        }
    }
}
#define key ev.code
#define pressed ev.value
#define up 0
#define hold 2
#define mute 113
#define volume_down 114
#define volume_up 115

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

        if((key == volume_down && pressed == up) || (key == volume_up && pressed == up) || (key == mute && pressed == up))
        {
            return true;
        }
        else if((key == volume_down && pressed == hold) || (key == volume_up && pressed == hold) || (key == mute && pressed == hold))
        {
            return true;
        }
        else if(key == 142 && pressed == up)
        {
            system("shutdown -h now");
        }
    }
}
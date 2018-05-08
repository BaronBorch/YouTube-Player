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

                if((ev.code == 114 && ev.value == 1) || (ev.code == 115 && ev.value == 1) || (ev.code == 113 && ev.value == 1))
                {
                    return true;
                }
                else if(ev.code == 142 && ev.value == 1)
                {
                    system("shutdown -h now");
                }
        }
}



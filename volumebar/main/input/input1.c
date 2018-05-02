#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include "input1.h"


bool input_read(void);

bool input_read(void)
{
        char devname[] = "/dev/input/event3";
        int device = open(devname, O_RDONLY);
        struct input_event ev;

        while(1)
        {
                read(device,&ev, sizeof(ev));

                if((ev.code == 114 && ev.value == 1) || (ev.code == 115 && ev.value == 1))
                {
                    printf("input read here\n");
                    return true;
                }


        }
}



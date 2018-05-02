#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include "input1.h"

int pev;

bool volume_change(void); // tutaj dzi³a jak powinna, sprawdzone.

int input()
{
        char devname[] = "/dev/input/event3";
        int device = open(devname, O_RDONLY);
        struct input_event ev;

        while(1)
        {
                read(device,&ev, sizeof(ev));
                pev = ev.code;

        }
}

bool volume_change(void)
{
    if(pev == 114 || pev == 115)
    {
    return true;
    }
}

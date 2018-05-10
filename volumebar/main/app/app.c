#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include "input.h"
#include "gui.h"
#include "app.h"

int c;
char buf[5];

void *hide_gui(void *vargp)
{
    if(usleep(2000000) == 0)
    {
        gui_hide();
        printf("%s\n%d\n", "gui hide", gui_hide());
    }
}


void app()
{
    FILE *fp;
    double a, b, d;
    pthread_t thread_id;


    while(1)
    {
        if(input_read() == true)
        {
            fp = popen("amixer get PCM | awk '$0~/%/{print $5}' | tr -d '[dB]'", "r" );
            d = fscanf(fp, "%lf", &a);
            b = (a+102.4)*0.94;
            c = (int)b;

            if(c < 0)
            {
                c = 0;
            }
            snprintf(buf, 5, "%d", c);
            printf( "%s\n", buf); // tymczasowy sprawdzian czy dzia³a jak powinno.

            gui_start(c, buf);
            pthread_create(&thread_id, NULL, hide_gui, NULL);
        }
    }
}

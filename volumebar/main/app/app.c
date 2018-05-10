#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "input.h"
#include "gui.h"
#include "app.h"

int c;
char buf[5];

void app()
{
    FILE *fp;
    double a, b, d;


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
        }
    }
}

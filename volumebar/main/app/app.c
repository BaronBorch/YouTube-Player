#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input1.h"
#include "gui.h"
#include "app.h"

void app()
{
    while(1)
    {
    FILE *fp;
	double a, b, c, d;
	char buf[3];

    if(input_read() == true)
    {
	fp = popen("amixer get PCM | awk '$0~/%/{print $5}' | tr -d '[dB]'", "r" );
	d = fscanf(fp, "%lf", &a);
	b = (a+102.4)*0.94;

	snprintf(buf, 3, "%f", b);
	printf( "%s\n", buf); // tymczasowy sprawdzian czy dzia³a jak powinno.
    gui(b, buf);
    }
    }
}

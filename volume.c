#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{


    FILE *fp;
    double a,b,c,d,e;
    char buf[3],to[20]="notify-send ";
    fp = popen("amixer get PCM | awk '$0~/%/{print $5}' | tr -d '[dB]'", "r");
    d = fscanf(fp, "%lf", &a);
    b = a+102.4;
    c = b*0.94;

    snprintf(buf, sizeof(buf), "%f",c);
    strcat(to, buf);

    system(to);

    return 0;
}

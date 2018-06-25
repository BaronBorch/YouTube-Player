#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "app/app.h"

pthread_t thread;

void *run_chromium(void *vargp)
{
    system("chromium-browser --kiosk --disable-infobars youtube.com/tv");
    return 0;
}

void run_chrom()
{
    pthread_create(&thread, NULL, run_chromium, NULL);
}

int main(int argc, char *argv[])
{
    if(argc == 1)
    {
        register_internet_conection(run_chrom);
    }

    app();
    pthread_join(thread, NULL);
    return 0;
}
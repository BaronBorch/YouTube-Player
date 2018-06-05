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
}

int main(int argc, char *argv[])
{
    if(argc == 1)
    {
        pthread_create(&thread, NULL, run_chromium, NULL);
    }

    app();
    pthread_join(thread, NULL);
    return 0;
}

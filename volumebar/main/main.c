#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "app.h"

pthread_t thread;

void *run_chromium(void *vargp)
{
    system("chromium-browser --start-fullscreen youtube.com/tv");
}

int main(int argc, char *argv[])
{
    if(strcmp ("test", argv[1]) == 0)
    {
        printf("%s\n", "test mode, chromium won't start");
        app();
        pthread_join(thread, NULL);
        return 0;
    }
    else
    {
        pthread_create(&thread, NULL, run_chromium, NULL);
        app();
        pthread_join(thread, NULL);
        return 0;
    }
}

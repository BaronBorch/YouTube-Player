#include <stdlib.h>
#include <pthread.h>
#include "app.h"

void *run_chromium(void *vargp)
{
   system("chromium-browser --start-fullscreen youtube.com/tv");
}

int main()
{
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, run_chromium, NULL);
    app();

    return 0;
}

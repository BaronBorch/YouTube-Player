#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "app.h"

    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_t thread;

void *run_chromium(void *vargp)
{
    pthread_mutex_lock(&lock);
    //system("chromium-browser --start-fullscreen youtube.com/tv");

}

int main()
{
    pthread_create(&thread, NULL, run_chromium, NULL);
    app();
    pthread_join(thread, NULL);
    return 0;
}

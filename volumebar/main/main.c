#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "app.h"

    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_t thread[2];

void *run_chromium(void *vargp)
{
    pthread_mutex_lock(&lock);
    //system("chromium-browser --start-fullscreen youtube.com/tv");

}

int main()
{
    pthread_create(&thread[0], NULL, run_chromium, NULL);
    app();
    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);
    return 0;
}

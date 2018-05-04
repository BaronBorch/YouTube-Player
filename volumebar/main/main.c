#include <stdlib.h>
#include <pthread.h>
#include "app.h"

int main()
{
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, app, NULL);
    system("chromium-browser --start-fullscreen youtube.com/tv");


	return 0;

}

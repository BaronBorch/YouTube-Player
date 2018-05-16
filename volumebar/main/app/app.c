#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>
#include "input.h"
#include "gui.h"
#include "app.h"

int converted_volume_val, hide_gui_test = 0;
char volume_to_show[5];
pthread_mutex_t Mutex;

void *wait_thread(void *vargp)
{
    hide_gui_test = 1;
    int timeInMs = 2000;
    pthread_mutex_lock(&Mutex);
    struct timeval tv;
    struct timespec ts;

    gettimeofday(&tv, NULL);
    ts.tv_sec = time(NULL) + timeInMs / 1000;
    ts.tv_nsec = tv.tv_usec * 1000 + 1000 * 1000 * (timeInMs % 1000);
    ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
    ts.tv_nsec %= (1000 * 1000 * 1000);

    printf("%s\n", "wait_thread here");
    int n = pthread_mutex_timedlock(&Mutex, &ts);

    if(n == ETIMEDOUT && hide_gui_test == 1)
    {
        set_gui_hided(1);
        gui_hide();
        set_gui_hided(0);
        printf("%s\n", "HIDEEeeeeeeeeeeeeeeeeeeeeeeeeeeEE");
        pthread_mutex_unlock(&Mutex);
    }

    else
    {
        wait_thread;
    }
}

void app()
{
    pthread_t thread1;
    FILE *file_p;
    double read_vol, convert_vol, convert_file;

    while(1)
    {
        if(input_read() == true)
        {
            file_p = popen("amixer get PCM | awk '$0~/%/{print $5}' | tr -d '[dB]'", "r" );
            convert_file = fscanf(file_p, "%lf", &read_vol);
            convert_vol = (read_vol+102.4)*0.94;
            converted_volume_val = (int)convert_vol;

            if(converted_volume_val < 0)
            {
                converted_volume_val = 0;
            }

            snprintf(volume_to_show, 5, "%d", converted_volume_val);
            printf( "%s\n", volume_to_show); // tymczasowy sprawdzian czy dzia³a jak powinno.
            hide_gui_test = 0;
            set_gui_hided(0);
            gui_show(converted_volume_val, volume_to_show);
            set_gui_hided(1);
            pthread_create(&thread1, NULL, wait_thread, NULL);
        }
    }
}
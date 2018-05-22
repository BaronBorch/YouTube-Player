#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include "input.h"
#include "gui.h"
#include "app.h"

int converted_volume_val;
char volume_to_show[5];
static pthread_mutex_t Mutex;
pthread_t thread1;

void foo_init()
{
    pthread_mutex_init(&Mutex, NULL);
}

void *wait_thread(void *vargp)
{
    printf("%s\n", "thread start here");
    int timeInMs = 2000;
    pthread_mutex_trylock(&Mutex);
    struct timeval tv;
    struct timespec ts;

    gettimeofday(&tv, NULL);
    ts.tv_sec = time(NULL) + timeInMs / 1000;
    ts.tv_nsec = tv.tv_usec * 1000 + 1000 * 1000 * (timeInMs % 1000);
    ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
    ts.tv_nsec %= (1000 * 1000 * 1000);

    printf("%s\n", "wait_thread here");
    int n = pthread_mutex_timedlock(&Mutex, &ts);

    if(n == ETIMEDOUT)
    {
        //gui_hide();
        printf("%s\n", "HIDEEeeeeeeeeeeeeeeeeeeeeeeeeeeEE");
    }
}

void handle_volume_change()
{
    FILE *file_p;
    double read_vol, convert_vol, convert_file;

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

    pthread_mutex_unlock(&Mutex);
    //gui_show_volumebar(converted_volume_val, volume_to_show);
    pthread_create(&thread1, NULL, wait_thread, NULL);
}

void set_HDMI()
{
    system("shutdown -h now");// Wkrótce będzie tu HDMI on/off
}

void app()
{
    //gui_init();
    input_read_start();

    register_volume_up_callback(handle_volume_change);
    register_volume_down_callback(handle_volume_change);
    register_volume_mute_callback(handle_volume_change);
    register_power_callback(set_HDMI);

    //pthread_mutex_destroy(&Mutex);
}
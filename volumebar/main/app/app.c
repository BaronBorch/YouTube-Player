#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <semaphore.h>
#include <errno.h>
#include "input.h"
#include "gui.h"
#include "app.h"

int converted_volume_val, hide_gui_test, counting_in_progres;
char volume_to_show[5];

pthread_mutex_t Mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t Cond = PTHREAD_COND_INITIALIZER;

void wait(int timeInMs)
{
    struct timeval tv;
    struct timespec ts;

    gettimeofday(&tv, NULL);
    ts.tv_sec = time(NULL) + timeInMs / 1000;
    ts.tv_nsec = tv.tv_usec * 1000 + 1000 * 1000 * (timeInMs % 1000);
    ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
    ts.tv_nsec %= (1000 * 1000 * 1000);

    int n = pthread_cond_timedwait(&Cond, &Mutex, &ts);
    if (n == ETIMEDOUT && hide_gui_test == 1)
    {
        printf("%d\n%s\n", hide_gui_test, "HIDEEeeeeeeeeeeeeeeeeeeeeeeeeeeEE");
        //gui_hide  // jak będzie dziłało
    }
    else
    {
        wait(2000);
    }
}

void* hide_gui(void* arg)
{
    printf("%d\n%s\n", hide_gui_test, "waiting ...");
    counting_in_progres = 1;
    wait(2000);
    counting_in_progres = 0;
}

void draw_volume_bar(void)
{
    //gui_start(converted_volume_val, volume_to_show);
    printf("%s\n", "draw");
}

void *wait2(void *vargp)
{
    while(1)
    {
        //printf("%s\n", " adfgadfgadfgadfgadfgadfgfg"); // ten print spowalnia while'a ?
        if(counting_in_progres == 1)
        {
            if(sleep(1) == 0 && hide_gui_test == 0)
            {
                printf("%s\n", "setting hide_gui_test = 1");
                hide_gui_test = 1;
            }
        }
    }
}

void *input_thread(void *vargp)
{
    FILE *file_p;
    double read_vol, convert_vol, convert_file;

    while(1)
    {
        if(input_read() == true)
        {
            if(hide_gui_test == 1)
            {
                printf("%s\n", "setting hide_gui_test = 0");
                hide_gui_test = 0;
            }

            file_p = popen("amixer get PCM | awk '$0~/%/{print $5}' | tr -d '[dB]'", "r" );
            convert_file = fscanf(file_p, "%lf", &read_vol);
            convert_vol = (read_vol+102.4)*0.94;
            converted_volume_val = (int)convert_vol;

            if(converted_volume_val < 0)
            {
                converted_volume_val = 0;
            }
            snprintf(volume_to_show, 5, "%d", converted_volume_val);
            printf( "%s\n%d\n", volume_to_show, hide_gui_test); // tymczasowy sprawdzian czy dzia³a jak powinno.

            //gui_start(converted_volume_val, volume_to_show);
            draw_volume_bar();
        }
    }
}

void app()
{
    
    pthread_t thread1, thread2, thread3;
    void *ret;
    pthread_create(&thread2, NULL, input_thread, NULL);
    while(1)
    {
        if(input_read() == true && counting_in_progres == 0)
        {
            pthread_create(&thread1, NULL, hide_gui, NULL);
            pthread_create(&thread3, NULL, wait2, NULL);
            //pthread_join(t1,&ret);
        }
    }
}

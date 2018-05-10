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

int c, e, f;
char buf[5];

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
    if (n == ETIMEDOUT && e == 1)
    {
        printf("%d\n%s\n", e, "HIDEEeeeeeeeeeeeeeeeeeeeeeeeeeeEE");
    }
    else
    {
        wait(2000);
    }
}

void* hide(void* arg)
{
    printf("%d\n%s\n", e, "waiting ...");
    f = 1;
    wait(2000);
    f = 0;
}

void draw(void)
{
    gui_start(c, buf);
    printf("%s\n", "draw");
}

void *wait2(void *vargp)
{
    while(1)
    {
        if(f == 1)
        {
            if(sleep(1) == 0 && e == 0)
            {
                printf("%s\n", "setting e = 1");
                e = 1;
            }
        }
    }
}

void *input_thread(void *vargp)
{
    FILE *fp;
    double a, b, d;

    while(1)
    {
        if(input_read() == true)
        {
            if(e == 1)
            {
                printf("%s\n", "setting e = 0");
                e = 0;
            }

            fp = popen("amixer get PCM | awk '$0~/%/{print $5}' | tr -d '[dB]'", "r" );
            d = fscanf(fp, "%lf", &a);
            b = (a+102.4)*0.94;
            c = (int)b;

            if(c < 0)
            {
                c = 0;
            }
            snprintf(buf, 5, "%d", c);
            printf( "%s\n%d\n", buf, e); // tymczasowy sprawdzian czy dzia³a jak powinno.

            gui_start(c, buf);
            draw();
        }
    }
}

void app()
{
    
    pthread_t t1, t2, t3;
    void *ret;
    pthread_create(&t2, NULL, input_thread, NULL);
    while(1)
    {
        if(input_read() == true)
        {
            pthread_create(&t1, NULL, hide, NULL);
            pthread_create(&t3, NULL, wait2, NULL);
            pthread_join(t1,&ret);
        }
    }
}

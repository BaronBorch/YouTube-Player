#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include "input/input.h"
#include "gui/gui.h"
#include "app.h"

static pthread_mutex_t Mutex, Mutex2;
pthread_t thread1, thread2;
int OK_button_chceck = 0, internet_access = 0;

void check_wpa_connect();

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
    int n = pthread_mutex_timedlock(&Mutex, &ts);

    if(n == ETIMEDOUT)
    {
        gui_hide();
        printf("%s\n", "HIDEEeeeeeeeeeeeeeeeeeeeeeeeeeeEE");
    }
    return 0;
}

void *wps_thread(void *vargp)
{
    printf("%s\n", "thread start here");
    int timeInMs = 2000;
    pthread_mutex_trylock(&Mutex2);
    struct timeval tv;
    struct timespec ts;

    gettimeofday(&tv, NULL);
    ts.tv_sec = time(NULL) + timeInMs / 1000;
    ts.tv_nsec = tv.tv_usec * 1000 + 1000 * 1000 * (timeInMs % 1000);
    ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
    ts.tv_nsec %= (1000 * 1000 * 1000);
    int n = pthread_mutex_timedlock(&Mutex2, &ts);

    if(n == ETIMEDOUT)
    {
        check_wpa_connect();
        printf("%s\n", "checking wps scan");
    }
    return 0;
}

void handle_volume_change()
{
    FILE *file_p;
    double read_vol, convert_vol;
    int converted_volume_val;
    char volume_to_show[5];

    file_p = popen("amixer get PCM | awk '$0~/%/{print $5}' | tr -d '[dB]'", "r" );
    fscanf(file_p, "%lf", &read_vol);
    convert_vol = (read_vol+102.4)*0.94;
    converted_volume_val = (int)convert_vol;
    pclose(file_p);

    if(converted_volume_val < 0)
    {
        converted_volume_val = 0;
    }

    snprintf(volume_to_show, 5, "%d", converted_volume_val);
    printf( "%s\n", volume_to_show); // tymczasowy sprawdzian czy dzia³a jak powinno.

    pthread_mutex_unlock(&Mutex);
    pthread_join(thread1, NULL);
    gui_show_volumebar(converted_volume_val, volume_to_show);
    pthread_create(&thread1, NULL, wait_thread, NULL);
}

void set_HDMI()
{
    FILE *file_d;
    int display_state;
    file_d = popen("sudo vcgencmd display_power | tr -cd [:digit:]", "r");
    fscanf(file_d, "%i", &display_state);

    if(display_state == 1)
    {
        system("vcgencmd display_power 0");
    }
    else
    {
        system("vcgencmd display_power 1");
    }
    pclose(file_d);
}

void check_wpa_connect()
{
    if(internet_access == 0)
    {
        FILE *file_e;
        char wpa_state[10];

        file_e = popen("wpa_cli status | grep 'wpa_state=' | tr -d '[wpa_state=]'", "r" );
        fscanf(file_e, "%s", wpa_state);
        printf("%s\n", wpa_state);
        pclose(file_e);

        if(strcmp("COMPLETED", wpa_state) == 0)
        {
            OK_button_chceck = 1;
            printf("%s\n", "WiFi conected");
            draw_statement("WiFi conected");
            internet_access = 1;
        }

        else if(strcmp("DISCONNECTED", wpa_state) == 0)
        {
            OK_button_chceck = 1;
            printf("%s\n", "WiFi disconnected");
            draw_st_disconnect("WiFi not connected");
        }

        else if(strcmp("ASSOCIATED", wpa_state) == 0)
        {
            OK_button_chceck = 1;
            printf("%s\n", "WiFi conected");
            draw_statement("WiFi conected");
            internet_access = 1;
        }

        else if(strcmp("SCANNING", wpa_state) == 0)
        {
            OK_button_chceck = 0;
            printf("%s\n", "scanning");
            pthread_create(&thread2, NULL, wps_thread, NULL);
            wait_screen_start();
        }

        else if(strcmp("INACTIVE", wpa_state) == 0)
        {
         printf("%s\n", "WiFi inactive");
            switch(system("ping -c1 www.google.com"))
            {
                case 0:
                {
                    OK_button_chceck = 1;
                    printf("%s\n", "internet connection OK");
                    draw_statement("Internet connected");
                    internet_access = 1;
                }
                break;
                case 512:
                {
                    OK_button_chceck = 1;
                    draw_st_disconnect("WiFi not connected");
                    printf("%s\n", "no internet");
                }
                break;
                default:
                {
                    OK_button_chceck = 0;
                    draw_statement("Something went wrong :(");
                    printf("%s\n", "inactive Wystąpił błąd :(");
                }
                break;
            }
        }

        else
        {
            OK_button_chceck = 0;
            draw_statement("Something went wrong :(");
            printf("%s\n", "else Wystąpił błąd :(");
        }
    }
}

void wps_connect()
{
    draw_st_destroy();
    if(internet_access == 0)
    {
        system("wpa_cli wps_pbc");
        printf("%s\n", "wps connect here, trying to connect");
        check_wpa_connect();
    }
}

void app()
{
    gui_init();
    check_wpa_connect();
    input_read_start();

    if(OK_button_chceck == 1)
    {
    register_enter_callback(wps_connect);
    }
    register_volume_up_callback(handle_volume_change);
    register_volume_down_callback(handle_volume_change);
    register_volume_mute_callback(handle_volume_change);
    register_power_callback(set_HDMI);
}
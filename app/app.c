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
#include "gui/gui_draw_state.h"
#include "gui/gui_volumebar.h"
#include "app.h"

pthread_mutex_t Mutex, Mutex2;
pthread_t thread1, thread2, thread3;
int pass_connect_check = 0, scan_counter = 0;
event_cb internet_connection;

void check_internet_connect();

void register_internet_conection(event_cb a)
{
    internet_connection = a;
}

void call_cb(event_cb a)
{
    if(a != NULL)
    {
        a();
    }
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
    int timeInMs = 5000;
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
        scan_counter++;
        printf("\nscan_counter = %d\n", scan_counter);
        printf("%s\n", "checking wps scan");
        check_internet_connect();
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
        converted_volume_val = 0;

    snprintf(volume_to_show, 5, "%d", converted_volume_val);
    printf( "%s\n", volume_to_show);

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
        system("vcgencmd display_power 0");
    else
        system("vcgencmd display_power 1");
    pclose(file_d);
}

void clear_last_network()
{
    printf("++ %s\n", __func__);
    int number_of_lines;
    FILE *lines = popen("wc -l < /etc/wpa_supplicant/wpa_supplicant.conf", "r");
    fscanf(lines, "%i", &number_of_lines);
    printf("number_of_lines = %i\n", number_of_lines);

    if(number_of_lines >= 7)
    {
        int a;
        for(a = 0; a<4; a++)
        {
            system("sudo sed -i '$d' /etc/wpa_supplicant/wpa_supplicant.conf");
            printf("clear line, a = %d\n", a);
        }
    }
    fclose(lines);
    printf("-- %s\n", __func__);
}

void check_internet_connect()
{
    switch(system("ping -c1 www.google.com"))
    {
        case 0:
        {
            printf("%s\n", "internet connection OK");
            draw_statement("Internet connected", 0);
            if(pass_connect_check == 1)
                pass_connect_check = 0;
        }
        break;
        case 512:
        {
            printf("%s\n", "no internet");
            FILE *file_e;
            char wpa_state[10];

            file_e = popen("wpa_cli status | grep 'wpa_state=' | tr -d '[wpa_state=]'", "r" );
            fscanf(file_e, "%s", wpa_state);
            printf("%s\n", wpa_state);

            if((strcmp("DISCONNECTED", wpa_state) == 0) || (strcmp("INACTIVE", wpa_state) == 0))
            {
                printf("%s\n", "WiFi disconnected");
                if(pass_connect_check == 1)
                {
                    clear_last_network();
                    pass_connect_check = 0;
                    draw_statement("Wrong password", 1);
                }
                else
                    draw_statement("No internet connection", 1);
            }

            else if((strcmp("COMPLETED", wpa_state) == 0) || (strcmp("ASSOCIATED", wpa_state) == 0))
            {
                printf("%s\n", "WiFi conected");
                if(pass_connect_check == 1)
                    pass_connect_check = 0;
                draw_statement("WiFi conected", 0);

            }

            else if(strcmp("SCANNING", wpa_state) == 0)
            {
                printf("%s\n", "scanning");
                pthread_create(&thread2, NULL, wps_thread, NULL);
                if(scan_counter == 6)
                {
                    system("wpa_cli disconnect");
                    scan_counter = 0;
                    sleep(1);
                    check_internet_connect();
                }
            }

            else if(strcmp("4WAYHANDSHAKE", wpa_state) == 0)
            {
                printf("%s\n", "4WAYHANDSHAKE");
                int t = pthread_create(&thread3, NULL, wps_thread, NULL);
                printf("pthread return = %d\n", t);
            }

            else
            {
                printf("%s\n", "else Nie można pobrać statusu wpa_supplicant :(");
                draw_statement("Something went wrong :(", 1);
            }
            pclose(file_e);
        }
        break;
        default:
        {
            printf("%s\n", "inactive Wystąpił błąd :(");
            draw_statement("Something went wrong :(", 1);
        }
        break;
    }
}

void wps_connect()
{
    system("wpa_cli wps_pbc");
    printf("%s\n", "wps connect here, trying to connect");
    check_internet_connect();
}

void run_yt()
{
    call_cb(internet_connection);
}

void connect_with_pass()
{
    printf("++ %s\n", __func__);
    system("wpa_cli reconfigure");
    system("wpa_cli reconnect");
    pass_connect_check = 1;
    sleep(7);
    check_internet_connect();
    printf("-- %s\n", __func__);
}

void app()
{
    printf("++ %s\n", __func__);
    system("sudo rm /var/run/wpa_supplicant/p2p-dev-wlan0");
    system("sudo chmod 777 /etc/wpa_supplicant/wpa_supplicant.conf");
    register_button_OK_connected_callback(run_yt);
    register_button_OK_connect_with_wps_callback(wps_connect);
    register_connect_with_password(connect_with_pass);
    register_volume_up_callback(handle_volume_change);
    register_volume_down_callback(handle_volume_change);
    register_volume_mute_callback(handle_volume_change);
    register_power_callback(set_HDMI);

    gui_init();
    input_read_start();
    check_internet_connect();

    printf("-- %s\n", __func__);
}
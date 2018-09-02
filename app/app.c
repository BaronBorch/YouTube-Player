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

pthread_mutex_t Mutex, Mutex2;
pthread_t thread1, thread2;
int pass_connect_check = 0;
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
        check_internet_connect();
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

void remember_password()
{
    printf("++ %s\n", __func__);
    system("sudo chmod 777 /etc/wpa_supplicant/wpa_supplicant.conf");
    char network_name[30], search_network_command[100] = "head /etc/wpa_supplicant/wpa_supplicant.conf |grep ";
    FILE *network_conf = popen("head /home/pi/wpa.conf |sed -n 2p |sed 's/ssid=//g'", "r");
    fscanf(network_conf, "%s", network_name);
    printf("%s\n", network_name);
    strcat(search_network_command, network_name);
    printf("%s\n", search_network_command);
    pclose(network_conf);

    if(system(search_network_command) == 256)
    {
        printf("%i\n", system("cat /home/pi/wpa.conf >> /etc/wpa_supplicant/wpa_supplicant.conf"));
        printf("added new network to wpa_supplicant.conf\n");
    }
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
            {
                remember_password();
                pass_connect_check = 0;
            }
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
                    draw_statement("Wrong password", 1);
                    pass_connect_check = 0;
                }
                else
                    draw_statement("No internet connection", 1);
            }

            else if((strcmp("COMPLETED", wpa_state) == 0) || (strcmp("ASSOCIATED", wpa_state) == 0))
            {
                printf("%s\n", "WiFi conected");
                draw_statement("WiFi conected", 0);
            }

            else if(strcmp("SCANNING", wpa_state) == 0)
            {
                printf("%s\n", "scanning");
                pthread_create(&thread2, NULL, wps_thread, NULL);
            }

            else
            {
                printf("%s\n", "else Nie można pobrać statusu wpa_supplicant :(");
                if(pass_connect_check == 1)
                {
                    system("sudo killall wpa_supplicant");
                    sleep(2);
                    system("sudo wpa_supplicant -Dwext -iwlan0 -c/etc/wpa_supplicant/wpa_supplicant.conf -B");
                    check_internet_connect();
                }
            }
            pclose(file_e);
        }
        break;
        default:
        {
            draw_statement("Something went wrong :(", 1);
            printf("%s\n", "inactive Wystąpił błąd :(");
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
    printf("%i\n", system("sudo killall wpa_supplicant"));
    sleep(2);
    printf("%i\n", system("sudo wpa_supplicant -Dwext -iwlan0 -c/home/pi/wpa.conf -B"));
    sleep(10);
    pass_connect_check = 1;
    check_internet_connect();
    printf("-- %s\n", __func__);
}

void app()
{
    printf("++ %s\n", __func__);
    system("sudo rm /var/run/wpa_supplicant/p2p-dev-wlan0");
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
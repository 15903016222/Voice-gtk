/**
 * @file core.c
 * @brief core
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-03-16
 */

#include "core.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static time_t phascanTime = 0;
static pthread_mutex_t phascanTimeMutex = PTHREAD_MUTEX_INITIALIZER;

#define PHASCAN_TIME_FILE "/home/tt/.time"

static void time_init();

int core_init()
{
    time_init();
	gpio_init();
}

void core_uninit()
{
	gpio_uninit();
}

void time_init()
{
    FILE *fp = fopen(PHASCAN_TIME_FILE, "r");
    if (NULL == fp) {
        return;
    }
    pthread_mutex_lock(&phascanTimeMutex);
    fscanf(fp, "%ld", &phascanTime);
    pthread_mutex_unlock(&phascanTimeMutex);
    fclose(fp);
}

time_t core_time()
{
    time_t sysTime = time(NULL);
    pthread_mutex_lock(&phascanTimeMutex);
    sysTime += phascanTime;
    pthread_mutex_unlock(&phascanTimeMutex);
    return sysTime;
}

void core_set_time(time_t t)
{
    time_t sysTime = time(NULL);
    gchar *cmd = NULL;
    pthread_mutex_lock(&phascanTimeMutex);
    phascanTime = t - sysTime;
    cmd = g_strdup_printf("echo %ld > %s && sync", phascanTime, PHASCAN_TIME_FILE);
    pthread_mutex_unlock(&phascanTimeMutex);
    system(cmd);
    g_free(cmd);
}

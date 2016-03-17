/**
 * @file gpio.c
 * @brief gpio
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-03-16
 */

#include "gpio.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include <pthread.h>

#define GPIO_DEV    "/dev/tt"

static int fd = -1;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

inline int gpio_init()
{
    pthread_mutex_lock(&mutex);

    if (fd < 0) {
        fd = open(GPIO_DEV, O_RDWR);
    }

    pthread_mutex_unlock(&mutex);

    return fd > 0;
}


inline void gpio_uninit()
{
    pthread_mutex_lock(&mutex);
    close(fd);
    fd = -1;
    pthread_mutex_unlock(&mutex);
}

int gpio_set_pin(GpioPin pin, PinState state)
{
    int ret = -1;

    pthread_mutex_lock(&mutex);
    ret = ioctl(fd, pin+state, NULL);
    pthread_mutex_unlock(&mutex);
    return ret;
}

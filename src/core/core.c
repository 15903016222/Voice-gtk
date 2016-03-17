/**
 * @file core.c
 * @brief core
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-03-16
 */

#include "core.h"

int core_init()
{
	gpio_init();
}

void core_uninit()
{
	gpio_uninit();
}

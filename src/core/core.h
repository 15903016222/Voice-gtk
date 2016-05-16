/**
 * @file core.h
 * @brief core
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-03-16
 */

#ifndef __CORE_H__
#define __CORE_H__

#include <glib.h>

G_BEGIN_DECLS

#include "gpio.h"

extern int core_init();
extern void core_uninit();

extern time_t core_time();
extern void core_set_time(time_t t);


G_END_DECLS

#endif /* end of __CORE_H__ */

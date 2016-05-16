/**
 * @file auth.h
 * @brief authorization
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-05-13
 */

#ifndef __AUTH_H__
#define __AUTH_H__

#include "glib.h"

G_BEGIN_DECLS

typedef enum {
    AUTH_MODE_INVALID,  /*非法模式*/
    AUTH_MODE_NONE,     /*无认证模式*/
    AUTH_MODE_CNT,      /*计数模式*/
    AUTH_MODE_RUN,      /*运行时间模式*/
    AUTH_MODE_DATE,     /*有效期模式*/
} AuthMode;

extern void auth_init();
extern AuthMode auth_get_mode();
extern time_t auth_get_data();
extern gboolean auth_is_valid();
extern void auth_uninit();

G_END_DECLS

#endif /* end of __AUTH_H__ */

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

enum AuthMode {
    AUTH_MODE_NONE,
    AUTH_MODE_CNT,
    AUTH_MODE_RUN,
    AUTH_MODE_VALID
};

struct _Auth {
    gint mode;
    gint data;
};

extern int auth();

G_END_DECLS

#endif /* end of __AUTH_H__ */

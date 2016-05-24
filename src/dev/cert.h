/**
 * @file cert.h
 * @brief certification
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-05-24
 */

#ifndef __CERT_H__
#define __CERT_H__

#include <glib.h>

G_BEGIN_DECLS

typedef enum {
    CERT_MODE_INVALID,  /*非法模式*/
    CERT_MODE_NONE,     /*无认证模式*/
    CERT_MODE_RUNCOUNT,      /*计数模式*/
    CERT_MODE_RUNTIME,      /*运行时间模式*/
    CERT_MODE_DATE,     /*有效期模式*/
} CertMode;

typedef struct _Cert Cert;
struct _Cert {
    gchar *id;
    CertMode mode;
    time_t data;
};

extern Cert *cert_load(const gchar *certFile, const gchar *pubPem);
extern void cert_free(Cert *c);
static inline gchar *cert_get_id(Cert *c)
{
    g_return_val_if_fail( NULL != c , NULL );
    return c->id;
}

static inline CertMode cert_get_mode(Cert *c)
{
    g_return_val_if_fail( NULL != c, CERT_MODE_INVALID);
    return c->mode;
}

static inline time_t cert_get_data(Cert *c)
{
    g_return_val_if_fail( NULL != c, 0 );
    return c->data;
}

G_END_DECLS

#endif /* end of __CERT_H__ */

/**
 * @file report_base.h
 * @brief report base
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-04-27
 */

#ifndef __REPORT_BASE_H__
#define __REPORT_BASE_H__

#include <glib.h>

G_BEGIN_DECLS

static inline void _report_set_str(gchar **var, const gchar *val)
{
    g_return_if_fail( var != NULL && val != NULL );
    g_free(*var);
    *var = g_strdup(val);
}

static inline void _report_set_int(gchar **var, gint val)
{
    g_return_if_fail( var != NULL );
    g_free(*var);
    *var = g_strdup_printf("%d", val);
}

static inline void _report_set_double(gchar **var, gchar *format, gdouble val)
{
    g_return_if_fail( var != NULL && format != NULL );
    g_free(*var);
    *var = g_strdup_printf(format, val);
}

static inline void _report_set_member(gpointer *p, gpointer v, GDestroyNotify f)
{
    if (*p) {
        f(*p);
    }
    *p = v;
}

G_END_DECLS

#endif /* end of include guard */

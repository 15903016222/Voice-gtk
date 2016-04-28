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

static inline _report_set_str(gchar **var, const gchar *val)
{
    g_return_if_fail( var != NULL && val != NULL );
    if ( *var ) {
        g_free(*var);
    }
    *var = g_strdup(val);
}

G_END_DECLS

#endif /* end of include guard */

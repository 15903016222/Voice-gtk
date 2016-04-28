/**
 * @file report.h
 * @brief report 
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-04-25
 */

#ifndef __REPORT_H__
#define __REPORT_H__

#include "report_header.h"
#include "report_users.h"
#include "report_defects.h"

G_BEGIN_DECLS


typedef struct _Report Report;

struct _Report {
    gchar *tmpl;          /* the name of template file */
    ReportHeader *header;
    ReportUsers *users;
    ReportDefects *defects;
};

static inline Report *report_new()
{
    return g_malloc0(sizeof(Report));
}

extern void *report_free(Report *r);

extern void report_save(const Report *report);

static inline void report_set_header(Report *r, ReportHeader *hdr)
{
    g_return_if_fail( r != NULL );
    _report_set_member((gpointer *)&r->header, hdr, (GDestroyNotify)report_header_free);
}

static inline void report_set_users(Report *r, ReportUsers *users)
{
    g_return_if_fail( r != NULL );
    _report_set_member((gpointer *)&r->users, users, (GDestroyNotify)report_users_free);
}

static inline void report_set_defects(Report *r, ReportDefects *ds)
{
    g_return_if_fail( r != NULL );
    _report_set_member((gpointer *)&r->defects, ds, (GDestroyNotify)report_defects_free);
}


G_END_DECLS

#endif /* end of include guard */

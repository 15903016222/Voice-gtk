/**
 * @file report.h
 * @brief report 
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-04-25
 */

#ifndef __REPORT_H__
#define __REPORT_H__

#include "report_defects.h"

G_BEGIN_DECLS

#define REPORT_VERSION     ("1.0")
#define INSPECTION_VERSION  ("1.0")

typedef struct _ReportHeader ReportHeader;      /* header of report */
typedef struct _ReportUser ReportUser;
typedef GSList ReportUsers;                     /* users data of report */
typedef struct _Report Report;

struct _ReportHeader {
    const gchar *setupFile;     /* setup file name */
    const gchar *saveMode;      /* save mode */
    const gchar *deviceType;    /* device type */
    gchar *reportFile;          /* the name of report file */
};

struct _ReportUser{
    const gchar *name;
    const gchar *content;
};


struct _Report {
    gchar *tmpl;          /* the name of template file */
    ReportHeader header;
    ReportUsers *users;
    ReportDefects *defects;
};

static inline Report *report_new()
{
    return g_malloc0(sizeof(Report));
}

extern void *report_free(Report *r, GDestroyNotify free_user);

extern void report_save(const Report *report);

static inline void report_insert_user(Report *report, ReportUser *user)
{
    g_return_if_fail( report != NULL && user != NULL );
    report->users = g_slist_append(report->users, user);
}

G_END_DECLS

#endif /* end of include guard */

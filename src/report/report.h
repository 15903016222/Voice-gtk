/**
 * @file report.h
 * @brief report 
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-04-25
 */

#ifndef __REPORT_H__
#define __REPORT_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <glib.h>

#define REPORT_VERSION     ("1.0")
#define INSPECTION_VERSION  ("1.0")

typedef struct _ReportHeader ReportHeader;
typedef struct _ReportUsers ReportUsers;
typedef struct _Report Report;

struct _ReportHeader {
    const gchar *setupFile;     /* setup file name */
    const gchar *saveMode;      /* save mode */
    const gchar *deviceType;    /* device type */
    gchar *reportFile;          /* the name of report file */
};

#define MAX_USERS 10
struct _ReportUser{
    const gchar *name;
    const gchar *content;
};
struct _ReportUsers {
    gint count;
    struct _ReportUser user[MAX_USERS];
};

struct _Report {
    gchar *tmpl;          /* the name of template file */
    ReportHeader header;
    ReportUsers users;
};



extern void report_save(const Report *report);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard */

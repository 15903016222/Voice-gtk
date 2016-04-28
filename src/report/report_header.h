/**
 * @file report_header.h
 * @brief report header
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-04-27
 */

#ifndef __REPORT_HEADER
#define __REPORT_HEADER

#include "report_base.h"

G_BEGIN_DECLS

#define REPORT_VERSION     ("1.0")
#define INSPECTION_VERSION  ("1.0")

typedef struct _ReportHeader ReportHeader;      /* header of report */


struct _ReportHeader {
    gchar *setupFile;     /* setup file name */
    gchar *saveMode;      /* save mode */
    gchar *deviceType;    /* device type */
    gchar *reportFile;          /* the name of report file */
};

static inline ReportHeader *report_header_new()
{
    return g_malloc0(sizeof(ReportHeader));
}

static inline void report_header_free(ReportHeader *h)
{
    g_free(h->setupFile);
    g_free(h->saveMode);
    g_free(h->deviceType);
    g_free(h->reportFile);
    g_free(h);
}

static inline void report_header_set_setup_file(ReportHeader *h, const gchar *setupFile)
{
    g_return_if_fail(h != NULL);
    _report_set_str(&h->setupFile, setupFile);
}

static inline void report_header_set_save_mode(ReportHeader *h, const gchar *saveMode)
{
    g_return_if_fail(h != NULL);
    _report_set_str(&h->saveMode, saveMode);
}

static inline void report_header_set_device_type(ReportHeader *h, const gchar *deviceType)
{
    g_return_if_fail(h != NULL);
    _report_set_str(&h->deviceType, deviceType);
}

static inline void report_header_set_report_file(ReportHeader *h, const gchar *reportFile)
{
    g_return_if_fail(h != NULL);
    _report_set_str(&h->reportFile, reportFile);
}

G_END_DECLS

#endif /* end of include guard */

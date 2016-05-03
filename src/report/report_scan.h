/**
 * @file report_scan.h
 * @brief scan information
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-04-28
 */

#ifndef __REPORT_SCAN_H__
#define __REPORT_SCAN_H__

#include "report_encoder.h"

G_BEGIN_DECLS

typedef struct _ReportScan ReportScan;

struct _ReportScan {
    gchar *scanStart;
    gchar *scanStop;
    gchar *scanResolution;
    gchar *indexStart;
    gchar *indexStop;
    gchar *indexResolution;
    gchar *scanSynchro;
    gchar *scanSpeed;
    ReportEncoder *scanEncoder;
    ReportEncoder *indexEncoder;
};

static  inline ReportScan *report_scan_new()
{
    return g_malloc0(sizeof(ReportScan));
}

static inline void report_scan_free(ReportScan *scan)
{
    g_free(scan->scanStart);
    g_free(scan->scanStop);
    g_free(scan->scanResolution);
    g_free(scan->indexStart);
    g_free(scan->indexStop);
    g_free(scan->indexResolution);
    g_free(scan->scanSynchro);
    g_free(scan->scanSpeed);
    report_encoder_free(scan->scanEncoder);
    report_encoder_free(scan->indexEncoder);
    g_free(scan);
}

static inline void report_scan_set_scan_start(ReportScan *scan, gdouble val)
{
    g_return_if_fail(scan != NULL);
    _report_set_double(&scan->scanStart, "%.2f", val);
}

static inline void report_scan_set_scan_stop(ReportScan *scan, gdouble val)
{
    g_return_if_fail(scan != NULL);
    _report_set_double(&scan->scanStop, "%.2f", val);
}

static inline void report_scan_set_scan_resolution(ReportScan *scan, gdouble val)
{
    g_return_if_fail(scan != NULL);
    _report_set_double(&scan->scanResolution, "%.2f", val);
}

static inline void report_scan_set_index_start(ReportScan *scan, gdouble val)
{
    g_return_if_fail(scan != NULL);
    _report_set_double(&scan->indexStart, "%.2f", val);
}

static inline void report_scan_set_index_stop(ReportScan *scan, gdouble val)
{
    g_return_if_fail(scan != NULL);
    _report_set_double(&scan->indexStop, "%.2f", val);
}

static inline void report_scan_set_index_resolution(ReportScan *scan, gdouble val)
{
    g_return_if_fail(scan != NULL);
    _report_set_double(&scan->indexResolution, "%.2f", val);
}

static inline void report_scan_set_scan_synchro(ReportScan *scan, const gchar *val)
{
    g_return_if_fail(scan != NULL);
    _report_set_str(&scan->scanSynchro, val);
}

static inline void report_scan_set_scan_speed(ReportScan *scan, gdouble val)
{
    g_return_if_fail(scan != NULL);
    _report_set_double(&scan->scanSpeed, "%.2f", val);
}

static inline void report_scan_set_scan_encoder(ReportScan *scan, ReportEncoder *enc)
{
    g_return_if_fail(scan != NULL);
    _report_set_member((gpointer *)&scan->scanEncoder, enc, (GDestroyNotify)report_encoder_free);
}

static inline void report_scan_set_index_encoder(ReportScan *scan, ReportEncoder *enc)
{
    g_return_if_fail(scan != NULL);
    _report_set_member((gpointer *)&scan->indexEncoder, enc, (GDestroyNotify)report_encoder_free);
}

G_END_DECLS

#endif /* end of __REPORT_SCAN_H__ */

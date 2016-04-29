/**
 * @file report_gate.h
 * @brief Gate Information
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-04-29
 */

#ifndef __REPORT_GATE_H__
#define __REPORT_GATE_H__

#include "report_base.h"

G_BEGIN_DECLS

typedef struct _ReportGate ReportGate;

struct _ReportGate {
    gchar *start;
    gchar *width;
    gchar *threshold;
    gchar *synchro;
};

static inline ReportGate *report_gate_new()
{
    return g_malloc0(sizeof(ReportGate));
}

static inline void *report_gate_free(ReportGate *gate)
{
    g_return_if_fail( gate != NULL );
    g_free(gate->start);
    g_free(gate->width);
    g_free(gate->threshold);
    g_free(gate->synchro);
    g_free(gate);
}

static inline void *report_gate_set_start(ReportGate *gate, gdouble val)
{
    g_return_if_fail( gate != NULL );
    _report_set_double(&gate->start, "%.2f", val);
}

static inline void *report_gate_set_width(ReportGate *gate, gdouble val)
{
    g_return_if_fail( gate != NULL );
    _report_set_double(&gate->width, "%.2f", val);
}

static inline void *report_gate_set_threshold(ReportGate *gate, gint val)
{
    g_return_if_fail( gate != NULL );
    _report_set_int(&gate->threshold, val);
}

static inline void *report_gate_set_synchro(ReportGate *gate, const gchar *val)
{
    g_return_if_fail( gate != NULL );
    _report_set_str(&gate->synchro, val);
}

G_END_DECLS

#endif /* end of __REPORT_GATE_H__ */

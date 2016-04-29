/**
 * @file report_probe.h
 * @brief report probe information
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-04-28
 */

#ifndef __REPORT_PROBE_H__
#define __REPORT_PROBE_H__

#include "report_base.h"

G_BEGIN_DECLS

typedef struct _ReportProbe ReportProbe;

struct _ReportProbe {
    gchar *model;
    gchar *serial;
    gchar *freq;
    gchar *aperture;
};

static inline ReportProbe *report_probe_new()
{
    return g_malloc0(sizeof(ReportProbe));
}

extern void report_probe_free(ReportProbe *p);

static inline void report_probe_set_model(ReportProbe *p, const gchar *model)
{
    g_return_if_fail( p != NULL );
    _report_set_str(&p->model, model);
}

static inline void report_probe_set_serial(ReportProbe *p, const gchar *serial)
{
    g_return_if_fail( p != NULL );
    _report_set_str(&p->serial, serial);
}

/**
 * @brief report_probe_set_freq
 * @param p     pointer to ReportProbe
 * @param freq  Units: MHz
 */
static inline void report_probe_set_freq(ReportProbe *p, gdouble freq)
{
    g_return_if_fail( p != NULL );
    g_free(p->freq);
    p->freq = g_strdup_printf("%.1f", freq);
}

static inline void report_probe_set_aperture(ReportProbe *p, gint aperture)
{
    g_return_if_fail( p != NULL );
    g_free(p->aperture);
    p->aperture = g_strdup_printf("%d", aperture);
}


G_END_DECLS

#endif /* end of __REPORT_PROBE_H__ */

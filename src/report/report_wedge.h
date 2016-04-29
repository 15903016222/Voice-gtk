/**
 * @file report_wedge.h
 * @brief wedget information
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-04-28
 */

#ifndef __REPORT_WEDGE_H__
#define __REPORT_WEDGE_H__

#include "report_base.h"

G_BEGIN_DECLS

typedef struct _ReportWedge ReportWedge;

struct _ReportWedge {
    gchar *model;
    gchar *angle;
};

static inline ReportWedge *report_wedge_new()
{
    return g_malloc0(sizeof(ReportWedge));
}

static inline void report_wedge_free(ReportWedge *w)
{
    g_free(w->angle);
    g_free(w->model);
    g_free(w);
}

static inline void report_wedge_set_model(ReportWedge *w, const gchar *model)
{
    g_return_if_fail( w != NULL );
    _report_set_str(&w->model, model);
}

/**
 * @brief report_wedge_set_angle
 * @param w     Pointer to ReportWedge
 * @param angle Units: Degree(°)
 */
static inline void report_wedge_set_angle(ReportWedge *w, gdouble angle)
{
    g_return_if_fail( w != NULL );
    g_free(w->angle);
    w->angle = g_strdup_printf("%0.1f", angle);
}

G_END_DECLS

#endif /* end of __REPORT_WEDGE_H__ */

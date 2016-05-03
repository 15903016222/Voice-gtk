/**
 * @file report_part.h
 * @brief part information
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-04-28
 */

#ifndef __REPORT_PART_H__
#define __REPORT_PART_H__

#include "report_base.h"

G_BEGIN_DECLS

typedef struct _ReportPart ReportPart;

struct _ReportPart {
    gchar *material;
    gchar *geometry;
    gchar *thickness;
};

static inline ReportPart *report_part_new()
{
    return g_malloc0(sizeof(ReportPart));
}

static inline void report_part_free(ReportPart *p)
{
    g_free(p->material);
    g_free(p->geometry);
    g_free(p->thickness);
    g_free(p);
}

static inline void report_part_set_material(ReportPart *p, const gchar *val)
{
    g_return_if_fail( p != NULL );
    _report_set_str(&p->material, val);
}

static inline void report_part_set_geometry(ReportPart *p, const gchar *val)
{
    g_return_if_fail( p != NULL );
    _report_set_str(&p->geometry, val);
}

static inline void report_part_set_thinckness(ReportPart *p, gdouble val)
{
    g_return_if_fail( p != NULL );
    _report_set_double(&p->thickness, "%.2f", val);
}

G_END_DECLS

#endif /* end of __REPORT_PART_H__ */

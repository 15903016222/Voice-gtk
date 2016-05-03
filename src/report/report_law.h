/**
 * @file report_law.h
 * @brief focal law information
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-04-28
 */

#ifndef __REPORT_LAW_H__
#define __REPORT_LAW_H__

#include "report_base.h"

G_BEGIN_DECLS

#define FOCAL_FIELDS_MAX    (4)

typedef struct _ReportLaw ReportLaw;

struct _ReportLaw {
    gchar *elementQty;
    gchar *firstTxElement;
    gchar *lastTxElement;
    gchar *firstRxElement;
    gchar *lastRxElement;
    gchar *elementResolution;
    gchar *startAngle;
    gchar *stopAngle;
    gchar *angleResolution;
    gchar *type;
    gchar *focalType;
    gchar *focalFieldNames[FOCAL_FIELDS_MAX];
    gchar *focalFieldValues[FOCAL_FIELDS_MAX];
};

static inline ReportLaw *report_law_new()
{
    return g_malloc0(sizeof(ReportLaw));
}

extern void report_law_free(ReportLaw *law);


static inline void report_law_set_element_qty(ReportLaw *l, gint val)
{
    g_return_if_fail( l != NULL );
    _report_set_int(&l->elementQty, val);
}

static inline void report_law_set_first_tx_element(ReportLaw *l, gint val)
{
    g_return_if_fail( l != NULL );
    _report_set_int(&l->firstTxElement, val);
}

static inline void report_law_set_last_tx_element(ReportLaw *l, gint val)
{
    g_return_if_fail( l != NULL );
    _report_set_int(&l->lastTxElement, val);
}

static inline void report_law_set_first_rx_element(ReportLaw *l, gint val)
{
    g_return_if_fail( l != NULL );
    _report_set_int(&l->firstRxElement, val);
}

static inline void report_law_set_last_rx_element(ReportLaw *l, gint val)
{
    g_return_if_fail( l != NULL );
    _report_set_int(&l->lastRxElement, val);
}

static inline void report_law_set_element_resolution(ReportLaw *l, gint val)
{
    g_return_if_fail( l != NULL );
    _report_set_int(&l->elementResolution, val);
}

static inline void report_law_set_start_angle(ReportLaw *l, gdouble val)
{
    g_return_if_fail( l != NULL );
    _report_set_double(&l->startAngle, "%.1f", val);
}

static inline void report_law_set_stop_angle(ReportLaw *l, gdouble val)
{
    g_return_if_fail( l != NULL );
    _report_set_double(&l->stopAngle, "%.1f", val);
}

static inline void report_law_set_angle_resolution(ReportLaw *l, gdouble val)
{
    g_return_if_fail( l != NULL );
    _report_set_double(&l->angleResolution, "%.1f", val);
}

static inline void report_law_set_type(ReportLaw *l, const gchar *val)
{
    g_return_if_fail( l != NULL );
    _report_set_str(&l->type, val);
}

static inline void report_law_set_focal_type(ReportLaw *l, const gchar *val)
{
    g_return_if_fail( l != NULL );
    _report_set_str(&l->focalType, val);
}

static inline void report_law_set_focal_field_name(ReportLaw *l, guint field, const gchar *val)
{
    g_return_if_fail( l != NULL && field < FOCAL_FIELDS_MAX);
    _report_set_str(&l->focalFieldNames[field], val);
}

static inline void report_law_set_focal_filed_value(ReportLaw *l, guint field, gdouble val)
{
    g_return_if_fail( l != NULL && field < FOCAL_FIELDS_MAX );
    _report_set_double(&l->focalFieldValues[field], "%.2f", val);
}



G_END_DECLS

#endif /* end of __REPORT_LAW_H__ */

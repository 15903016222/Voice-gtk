/**
 * @file report_encoder.h
 * @brief encoder information
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-05-03
 */

#ifndef __REPORT_ENCODER_H__
#define __REPORT_ENCODER_H__

#include "report_base.h"

G_BEGIN_DECLS

typedef struct _ReportEncoder ReportEncoder;
struct _ReportEncoder {
    gchar *name;
    gchar *type;
    gchar *resolution;
    gchar *polarity;
};

static inline ReportEncoder *report_encoder_new()
{
    return g_malloc0(sizeof(ReportEncoder));
}

static inline void report_encoder_free(ReportEncoder *enc)
{
    g_free(enc->name);
    g_free(enc->type);
    g_free(enc->resolution);
    g_free(enc->polarity);
    g_free(enc);
}

static inline void report_encoder_set_name(ReportEncoder *enc, const gchar *val)
{
    g_return_if_fail( enc != NULL );
    _report_set_str(&enc->name, val);
}

static inline void report_encoder_set_type(ReportEncoder *enc, const gchar *val)
{
    g_return_if_fail( enc != NULL );
    _report_set_str(&enc->type, val);
}

static inline void report_encoder_set_resolution(ReportEncoder *enc, gdouble val)
{
    g_return_if_fail( enc != NULL );
    _report_set_double(&enc->resolution, "%.3f", val);
}

static inline void report_encoder_set_polarity(ReportEncoder *enc, const gchar *val)
{
    g_return_if_fail( enc != NULL );
    _report_set_str(&enc->polarity, val);
}

G_END_DECLS

#endif /* end of __REPORT_ENCODER_H__ */

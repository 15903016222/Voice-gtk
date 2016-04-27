/**
 * @file report_defects.h
 * @brief report defects information
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-04-27
 */

#ifndef __REPORT_DEFECTS_H__
#define __REPORT_DEFECTS_H__

#include <glib.h>

G_BEGIN_DECLS

#define MAX_FIELDS  (8)

typedef struct _ReportDefect ReportDefect;
typedef struct _ReportDefects ReportDefects;    /* defects data of report */

/**
 * @brief The _ReportDefect struct
 */
struct _ReportDefect{
    gchar *scan;      /* scan-axis position */
    gchar *index;     /* index-axis position */
    gchar *group;
    gchar *channel;
    gchar *fieldValues[MAX_FIELDS];
    gchar *comments;  /* comments about defect */
};

/**
 * @brief The _ReportDefects struct
 */
struct _ReportDefects {
    gchar *fieldNames[MAX_FIELDS];
    GSList *defects;
};

static inline ReportDefect *report_defect_new()
{
    return g_malloc0(sizeof(ReportDefect));
}

static inline ReportDefects *report_defects_new()
{
    return g_malloc0(sizeof(ReportDefects));
}

void report_defect_free(ReportDefect *d);
void report_defects_free(ReportDefects *ds);

static inline void _set_str(gchar **d, const gchar *str)
{
    g_return_if_fail( d != NULL && str != NULL );
    if ( NULL != *d ) {
        g_free(*d);
    }
    *d = g_strdup(str);
}


static inline void report_defects_add_defect(ReportDefects *ds, ReportDefect *d)
{
    g_return_if_fail( ds != NULL && d != NULL );
    ds->defects = g_slist_append(ds->defects, d);
}

static inline void report_defects_set_field_name(ReportDefects *ds, guint field, const gchar *name)
{
    g_return_if_fail( ds != NULL && field < MAX_FIELDS);
    _set_str(&ds->fieldNames[field], name);
}

static inline void report_defect_set_scan(ReportDefect *d, const gchar *scan)
{
    g_return_if_fail( d != NULL );
    _set_str(&d->scan, scan);
}

static inline void report_defect_set_index(ReportDefect *d, const gchar *index)
{
    g_return_if_fail( d != NULL );
    _set_str(&d->index, index);
}

static inline void report_defect_set_group(ReportDefect *d, gint group)
{
    g_return_if_fail( d != NULL );
    if (NULL != d->group) {
        g_free(d->group);
    }
    d->group = g_strdup_printf("%d", group);
}

static inline void report_defect_set_channel(ReportDefect *d, gint channel)
{
    g_return_if_fail( d != NULL );
    if (NULL != d->channel) {
        g_free(d->channel);
    }
    d->channel = g_strdup_printf("%d", channel);
}

static inline void report_defect_set_field_value(ReportDefect *d, guint field, const gchar *val)
{
    g_return_if_fail( d != NULL && field < MAX_FIELDS );
    _set_str(&d->fieldValues[field], val);
}

static inline void report_defect_comment(ReportDefect *d, const gchar *comment)
{
    g_return_if_fail(NULL != d);
    _set_str(&d->comments, comment);
}

G_END_DECLS

#endif /* end of include guard */

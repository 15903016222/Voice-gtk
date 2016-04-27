/**
 * @file report_defects.c
 * @brief report defects information
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-04-27
 */

#include "report_defects.h"

void report_defect_free(ReportDefect *d)
{
    gint i = 0;
    g_free(d->channel);
    g_free(d->comments);
    g_free(d->group);
    g_free(d->index);
    g_free(d->scan);
    for (; i < MAX_FIELDS; ++i) {
        g_free(d->fieldValues[i]);
    }
    g_free(d);
}

void report_defects_free(ReportDefects *ds)
{
    gint i = 0;
    for (; i < MAX_FIELDS; ++i) {
        g_free(ds->fieldNames[i]);
    }
    g_slist_free_full(ds->defects, report_defect_free);
}

/**
 * @file report_defects.c
 * @brief report defects information
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-04-27
 */

#include "report_defects.h"

#include <stdio.h>
#include <string.h>

void report_defect_free(ReportDefect *d)
{
    gint i = 0;
    g_free(d->channel);
    g_free(d->comments);
    g_free(d->group);
    g_free(d->index);
    g_free(d->scan);
    g_free(d->image);
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
    g_slist_free_full(ds->defects, (GDestroyNotify)report_defect_free);
    g_free(ds);
}

void report_defect_set_image(ReportDefect *d, const gchar *imageName)
{
    gchar *cmd = g_strdup_printf("base64 %s", imageName);
    GString *data = g_string_sized_new(1024*2);
    gchar buf[1024] = {0};
    FILE *f = NULL;

    f = popen(cmd, "r");
    if (NULL == f) {
        return;
    }

    while( !feof(f) ) {
        if ( NULL == fgets(buf, 1024, f) ) {
            break;
        }

        data = g_string_append(data, buf);
        memset(buf, 0, 1024);
    }
    d->image = g_string_free(data, FALSE);
report_defect_set_image_end:
    pclose(f);
    g_free(cmd);
}

/**
 * @file report_probe.c
 * @brief probe information
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-04-28
 */

#include "report_probe.h"


void report_probe_free(ReportProbe *p)
{
    g_free(p->model);
    g_free(p->serial);
    g_free(p->freq);
    g_free(p->aperture);
    g_free(p);
}

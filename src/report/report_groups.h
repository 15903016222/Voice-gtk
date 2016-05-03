/**
 * @file report_group.h
 * @brief report group
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-04-28
 */

#ifndef __REPORT_GROUP_H__
#define __REPORT_GROUP_H__

#include "report_probe.h"
#include "report_wedge.h"
#include "report_fft.h"
#include "report_setup.h"
#include "report_law.h"
#include "report_part.h"

G_BEGIN_DECLS

typedef struct _ReportGroups ReportGroups;
typedef struct _ReportGroup ReportGroup;

typedef struct _ReportGroupScan ReportGroupScan;

struct _ReportGroup {
    ReportProbe *probe;
    ReportWedge *wedge;
    ReportFFT *fft;
    ReportSetup *setup;
    ReportLaw *law;
    ReportPart *part;

    ReportGroupScan *scan;
};

struct _ReportGroups {
    GSList *groups;
};

static inline ReportGroups *report_groups_new()
{
    return g_malloc0(sizeof(ReportGroups));
}

static inline ReportGroup *report_group_new()
{
    return g_malloc0(sizeof(ReportGroup));
}

static inline void report_group_free(ReportGroup *grp)
{
    g_return_if_fail(grp != NULL);

    report_probe_free(grp->probe);
    report_wedge_free(grp->wedge);
    report_fft_free(grp->fft);
    report_setup_free(grp->setup);
    report_law_free(grp->law);

    g_free(grp);
}

static inline void report_groups_free(ReportGroups *grps)
{
    g_return_if_fail(grps != NULL);
    g_slist_free_full(grps->groups, (GDestroyNotify)report_group_free);
    g_free(grps);
}

static inline void report_groups_add_group(ReportGroups *grps, ReportGroup *grp)
{
    g_return_if_fail(grps != NULL && grp != NULL);
    grps->groups = g_slist_append(grps->groups, grp);
}

static inline void report_group_set_probe(ReportGroup *grp, ReportProbe *probe)
{
    g_return_if_fail( grp != NULL );
    _report_set_member((gpointer *)&grp->probe, probe, (GDestroyNotify)report_probe_free);
}

static inline void report_group_set_wedge(ReportGroup *grp, ReportWedge *wedge)
{
    g_return_if_fail( grp != NULL );
    _report_set_member((gpointer *)&grp->wedge, wedge, (GDestroyNotify)report_wedge_free);
}

static inline void report_group_set_fft(ReportGroup *grp, ReportFFT *fft)
{
    g_return_if_fail( grp != NULL );
    _report_set_member((gpointer *)&grp->fft, fft, (GDestroyNotify)report_fft_free);
}

static inline void report_group_set_setup(ReportGroup *grp, ReportSetup *setup)
{
    g_return_if_fail( grp != NULL );
    _report_set_member((gpointer *)&grp->setup, setup, (GDestroyNotify)report_setup_free);
}

static inline void report_group_set_law(ReportGroup *grp, ReportLaw *law)
{
    g_return_if_fail( grp != NULL );
    _report_set_member((gpointer *)&grp->law, law, (GDestroyNotify)report_law_free);
}

static inline void report_group_set_part(ReportGroup *grp, ReportPart *part)
{
    g_return_if_fail( grp != NULL );
    _report_set_member((gpointer *)&grp->part, part, (GDestroyNotify)report_part_free);
}

G_END_DECLS

#endif /* end of __REPORT_GROUP_H__ */

/**
 * @file report.c
 * @brief 0.1
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-04-25
 */

#include "report.h"

#include <libgen.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

static inline void set_kv(lua_State *L, const gchar *key, const gchar *val)
{
    lua_pushstring(L, key);
    if (NULL == val) {
        lua_pushstring(L, "-");
    } else {
        lua_pushstring(L, val);
    }
    lua_settable(L, -3);
}

static inline void set_av(lua_State *L, gint index, const gchar *val)
{
    lua_pushinteger(L, index);
    if (NULL == val) {
        lua_pushstring(L, "-");
    } else {
        lua_pushstring(L, val);
    }
    lua_settable(L, -3);
}

static void report_header(lua_State *L, const ReportHeader *hdr)
{
    gchar *msg = NULL;
    time_t t;
    struct tm *p;


    /* report and inspection date */
    time(&t);
    p = localtime(&t);
    msg = g_strdup_printf("%d/%d/%d", 1900+p->tm_year, 1+p->tm_mon, p->tm_mday);
    set_kv(L, "ReportDate", msg);
    set_kv(L, "InspectionDate", msg);
    g_free(msg);

    set_kv(L, "ReportVersion", REPORT_VERSION);
    set_kv(L, "SetupFile", hdr->setupFile);
    set_kv(L, "InspectionVersion", INSPECTION_VERSION);
    set_kv(L, "SaveMode", hdr->saveMode);
    set_kv(L, "DeviceType", hdr->deviceType);
    set_kv(L, "ReportFile", basename(hdr->reportFile));
}

static void report_users(lua_State *L, const ReportUsers *users)
{
    const GSList *it = users->users;
    ReportUser *user = NULL;
    gint i = 0;

    lua_pushstring(L, "Users");
    lua_createtable(L, 0, 0);

    for (; it; it = it->next) {
        user = it->data;
        lua_pushinteger(L, ++i);

        lua_createtable(L, 0, 0);
        set_kv(L, "Name", user->name);
        set_kv(L, "Content", user->content);

        lua_settable(L, -3);
    }

    lua_settable(L, -3);
}

static void report_group_probe(lua_State *L, ReportProbe *p)
{
    set_kv(L, "ProbeModel", p->model);
    set_kv(L, "ProbeSerial", p->serial);
    set_kv(L, "ProbeFreq", p->freq);
    set_kv(L, "ProbeAperture", p->aperture);
}

static void report_group_wedge(lua_State *L, ReportWedge *w)
{
    set_kv(L, "WedgeModel", w->model);
    set_kv(L, "WedgeAngle", w->angle);
}

static void report_group_fft(lua_State *L, ReportFFT *t)
{
    set_kv(L, "PeakFreq", t->peakFreq);
    set_kv(L, "LowerFreq_6dB", t->lowerFreq_6dB);
    set_kv(L, "HigherFreq_6dB", t->higherFreq_6dB);
    set_kv(L, "CenterFreq_6dB", t->centerFreq_6dB);
    set_kv(L, "Bandwidth_6dB", t->bandwidth_6dB);
    set_kv(L, "BandwidthPercent_6dB", t->bandwidthPercent_6dB);
    set_kv(L, "LowerFreq_20dB", t->lowerFreq_20dB);
    set_kv(L, "HigherFreq_20dB", t->higherFreq_20dB);
    set_kv(L, "CenterFreq_20dB", t->centerFreq_20dB);
    set_kv(L, "Bandwidth_20dB", t->bandwidth_20dB);
    set_kv(L, "BandwidthPercent_20dB", t->bandwidthPercent_20dB);
}

static void report_group_setup(lua_State *L, ReportSetup *s)
{
    set_kv(L, "BeamDelay", s->beamDelay);
    set_kv(L, "HalfPathStart", s->halfPathStart);
    set_kv(L, "HalfPathRange", s->halfPathRange);
    set_kv(L, "PRF", s->prf);
    set_kv(L, "InspectionType",  s->inspectionType);
    set_kv(L, "AveragingFactor", s->averagingFactor);
    set_kv(L, "ScaleFactor", s->scaleFactor);
    set_kv(L, "VideoFilter", s->videoFilter);
    set_kv(L, "Rectification", s->rectification);
    set_kv(L, "BandPassFilter", s->bandPassFilter);
    set_kv(L, "Voltage", s->voltage);
    set_kv(L, "Gain", s->gain);
    set_kv(L, "RxTxMode", s->rxTxMode);
    set_kv(L, "WaveType", s->waveType);
    set_kv(L, "SoundVelocity", s->soundVelocity);
    set_kv(L, "PulseWidth", s->pulseWidth);
    set_kv(L, "ScanOffset", s->scanOffset);
    set_kv(L, "IndexOffset", s->indexOffset);
    set_kv(L, "Skew", s->skew);
    if (s->gateA) {
        set_kv(L, "GateAStart", s->gateA->start);
        set_kv(L, "GateAWidth", s->gateA->width);
        set_kv(L, "GateAThreshold", s->gateA->threshold);
        set_kv(L, "GateASynchro", s->gateA->synchro);
    }
    if (s->gateB) {
        set_kv(L, "GateBStart", s->gateB->start);
        set_kv(L, "GateBWidth", s->gateB->width);
        set_kv(L, "GateBThreshold", s->gateB->threshold);
        set_kv(L, "GateBSynchro", s->gateB->synchro);
    }
    if (s->gateI) {
        set_kv(L, "GateIStart", s->gateI->start);
        set_kv(L, "GateIWidth", s->gateI->width);
        set_kv(L, "GateIThreshold", s->gateI->threshold);
        set_kv(L, "GateISynchro", s->gateI->synchro);
    }
}

static void report_group_focallaw(lua_State *L, ReportLaw *l)
{
    if (NULL == l) {
        lua_pushstring(L, "FocalFieldNames");
        lua_createtable(L, 0, 0);
        lua_settable(L, -3);

        lua_pushstring(L, "FocalFieldValues");
        lua_createtable(L, 0, 0);
        lua_settable(L, -3);
        return;
    }
    set_kv(L, "ElementQty", l->elementQty);
    set_kv(L, "FirstTxElement", l->firstTxElement);
    set_kv(L, "LastTxElement", l->lastTxElement);
    set_kv(L, "FirstRxElement", l->firstRxElement);
    set_kv(L, "LastRxElement", l->lastRxElement);
    set_kv(L, "ElementResolution", l->elementResolution);
    set_kv(L, "StartAngle", l->startAngle);
    set_kv(L, "StopAngle", l->stopAngle);
    set_kv(L, "AngleResolution", l->angleResolution);
    set_kv(L, "LawType", l->type);
    set_kv(L, "FocalType", l->focalType);

    gint i = 0;
    lua_pushstring(L, "FocalFieldNames");
    lua_createtable(L, 0, 0);
    for (i=0; i<4; ++i) {
        if (NULL == l->focalFieldNames[i]) {
            set_av(L, i+1, "");
        } else {
            set_av(L, i+1, l->focalFieldNames[i]);
        }
    }
    lua_settable(L, -3);

    lua_pushstring(L, "FocalFieldValues");
    lua_createtable(L, 0, 0);
    for (i=0; i<4; ++i) {
        if (NULL == l->focalFieldNames[i]) {
            set_av(L, i+1, "");
        } else {
            set_av(L, i+1, l->focalFieldValues[i]);
        }
    }
    lua_settable(L, -3);
}

static void report_part(lua_State *L)
{
    set_kv(L, "PartMaterial", "Steel common");
    set_kv(L, "PartGeometry", "Plate");
    set_kv(L, "PartThickness", "50");
}

static void report_scan(lua_State *L)
{
    set_kv(L, "ScanStart", "40.00");
    set_kv(L, "ScanStop", "750.00");
    set_kv(L, "ScanResolution", "1.00");
    set_kv(L, "IndexStart", "-");
    set_kv(L, "IndexStop", "-");
    set_kv(L, "IndexResolution", "-");
    set_kv(L, "ScanSynchro", "Time");
    set_kv(L, "ScanSpeed", "20.00");
    set_kv(L, "ScanEncoder", "Off");
    set_kv(L, "ScanEncoderType", "Off");
    set_kv(L, "ScanEncoderResolution", "Off");
    set_kv(L, "ScanPolarity", "Off");
    set_kv(L, "IndexEncoder", "Off");
    set_kv(L, "IndexEncoderType", "Off");
    set_kv(L, "IndexEncoderResolution", "Off");
    set_kv(L, "IndexPolarity", "Off");
}


static void report_groups(lua_State *L, ReportGroups *groups)
{
    const GSList *item = groups->groups;
    ReportGroup *group = NULL;
    gint i = 0;

    lua_pushstring(L, "Groups");
    lua_createtable(L, 0, 0);

    for ( ; item; item = item->next ) {
        group = (ReportGroup *)item->data;
        lua_pushinteger(L, ++i);
        lua_createtable(L, 0, 0);

        report_group_probe(L, group->probe);
        report_group_wedge(L, group->wedge);
        report_group_fft(L, group->fft);
        report_group_setup(L, group->setup);
        report_group_focallaw(L, group->law);

        lua_settable(L, -3);
    }

    lua_settable(L, -3);
}

static void report_field_names(lua_State *L, gchar * const *fieldNames)
{
    g_return_if_fail(fieldNames != NULL);
    gint i = 0;
    lua_pushstring(L, "FieldNames");
    lua_createtable(L, 0, 0);
    for (; i < MAX_FIELDS; ++i) {
        set_av(L, i+1, fieldNames[i]);
    }
    lua_settable(L, -3);
}

static void report_defect_field_values(lua_State *L, gchar * const *values)
{
    gint i = 0;
    lua_pushstring(L, "FieldValues");
    lua_createtable(L, 0, 0);

    for (; i<MAX_FIELDS; ++i) {
        set_av(L, i+1, values[i]);
    }

    lua_settable(L, -3);
}

static void _report_defects(lua_State *L, const ReportDefects *defects)
{
    const GSList *it = defects->defects;
    ReportDefect *d = NULL;
    gint i = 0;

    lua_pushstring(L, "Defects");
    lua_createtable(L, 0, 0);

    for (; it; it = it->next) {
        d = it->data;

        lua_pushinteger(L, ++i);
        lua_createtable(L, 0, 0);

        set_kv(L, "Scan", d->scan);
        set_kv(L, "Index", d->index);
        set_kv(L, "Group", d->group);
        set_kv(L, "Channel", d->channel);
        report_defect_field_values(L, d->fieldValues);

        lua_settable(L, -3);
    }

    lua_settable(L, -3);
}

static inline void report_defects(lua_State *L, const ReportDefects *defects)
{
    /* field names */
    report_field_names(L, defects->fieldNames);

    _report_defects(L, defects);
}


void report_save(const Report *report)
{
    g_return_if_fail(report != NULL
            && report->tmpl != NULL
            && report->header != NULL
            && report->header->reportFile != NULL);

    lua_State *L = lua_open();
    if (NULL == L) {
        g_warning("loading report library failed");
        return;
    }
    luaL_openlibs(L);

    if ( luaL_dofile(L, "/home/tt/TT/report.lua") != 0
         && luaL_dofile(L, "/usr/share/phascan/report.lua") != 0 ) {
        g_warning("loading report library failed[2]");
        lua_close(L);
        return;
    }

    lua_getglobal(L, "output");
    lua_pushstring(L, report->tmpl);
    lua_pushstring(L, report->header->reportFile);
    lua_createtable(L, 0, 0);   /* 1 */

    report_header(L, report->header);
    report_users(L, report->users);
    report_groups(L, report->groups);
    report_defects(L, report->defects);

    lua_pcall(L, 3, 0, 0);

    lua_close(L);
}

void *report_free(Report *r)
{
    g_free(r->tmpl);

    report_header_free(r->header);
    report_users_free(r->users);
    report_groups_free(r->groups);
    report_defects_free(r->defects);

    g_free(r);
}

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
    lua_pushstring(L, val);
    lua_settable(L, -3);
}

static inline void set_av(lua_State *L, gint index, const gchar *val)
{
    lua_pushinteger(L, index);
    lua_pushstring(L, val);
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
    const ReportUsers *it = users;
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

static void report_probe(lua_State *L)
{
    set_kv(L, "ProbeModel", "5L16-0.6-10-D1");
    set_kv(L, "ProbeSerial", "D1");
    set_kv(L, "ProbeFreq", "5.0");
    set_kv(L, "PeakFreq", "-");
    set_kv(L, "WedgeModel", "SD1-N55S");
    set_kv(L, "WedgeAngle", "36.0");
    set_kv(L, "ProbeAperture", "16");
    set_kv(L, "LowerFreq_6dB", "-");
    set_kv(L, "HigherFreq_6dB", "-" );
    set_kv(L, "CenterFreq_6dB", "-");
    set_kv(L, "Bandwidth_6dB", "-");
    set_kv(L, "BandwidthPercent_6dB", "-");
    set_kv(L, "LowerFreq_20dB", "-");
    set_kv(L, "HigherFreq_20dB", "-");
    set_kv(L, "CenterFreq_20dB", "-");
    set_kv(L, "Bandwidth_20dB", "-");
    set_kv(L, "BandwidthPercent_20dB", "-");
}

static void report_setup(lua_State *L)
{
    set_kv(L, "BeamDelay", "11.08");
    set_kv(L, "HalfPathStart", "0.00");
    set_kv(L, "HalfPathRange", "92.38");
    set_kv(L, "PRF", "20");
    set_kv(L, "InspectionType", "PA");
    set_kv(L, "AveragingFactor", "1");
    set_kv(L, "SCALE_FACTOR", "9");
    set_kv(L, "VideoFilter", "On");
    set_kv(L, "Rectification", "FW");
    set_kv(L, "BandPassFilter", "None");
    set_kv(L, "Voltage", "50");
    set_kv(L, "Gain", "20.0");
    set_kv(L, "RxTxMode", "PE Pulse-Echo");
    set_kv(L, "WaveType", "SW");
    set_kv(L, "SoundVelocity", "3240");
    set_kv(L, "PulseWidth", "100.0");
    set_kv(L, "ScanOffset", "0.0");
    set_kv(L, "IndexOffset", "0.0");
    set_kv(L, "Skew", "90.0");
    set_kv(L, "GateIStart", "0.00");
    set_kv(L, "GateIWidth", "0.00");
    set_kv(L, "GateIThreshold", "0");
    set_kv(L, "GateISynchro", "Pulse");
    set_kv(L, "GateAStart", "0.00");
    set_kv(L, "GateAWidth", "8.10");
    set_kv(L, "GateAThreshold", "25");
    set_kv(L, "GateASynchro", "Pulse");
    set_kv(L, "GateBStart", "4.05");
    set_kv(L, "GateBWidth", "4.05");
    set_kv(L, "GateBThreshold", "20");
    set_kv(L, "GateBSynchro", "Pulse");
}

static void report_focallaw(lua_State *L)
{
    set_kv(L, "ElementQty", "16");
    set_kv(L, "FirstTxElement", "1");
    set_kv(L, "LastTxElement", "16");
    set_kv(L, "FirstRxElement", "-");
    set_kv(L, "LastRxElement", "-");
    set_kv(L, "ElementResolution", "-");
    set_kv(L, "WaveType", "SW");
    set_kv(L, "StartAngle", "30.0");
    set_kv(L, "StopAngle", "60.0");
    set_kv(L, "AngleResolution", "1.0");
    set_kv(L, "LawType", "Azimuthal");
    set_kv(L, "FocalType", "True Depth");
    set_kv(L, "FocalDepth", "30.00");
    set_kv(L, "PositionEnd", "-");
    set_kv(L, "PositionStep", "-");
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

static void report_groups(lua_State *L)
{
    lua_pushstring(L, "Groups");
    lua_createtable(L, 0, 0);

//    lua_pushinteger(L, 1);
//    lua_createtable(L, 0, 0);
//    report_probe(L);
//    report_setup(L);
//    report_focallaw(L);
//    report_part(L);
//    report_scan(L);
//    lua_settable(L, -3);

//    lua_pushinteger(L, 2);
//    lua_createtable(L, 0, 0);
//    report_probe(L);
//    report_setup(L);
//    report_focallaw(L);
//    report_part(L);
//    report_scan(L);
//    lua_settable(L, -3);

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

    if ( luaL_dofile(L, "./report.lua") != 0 ) {
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
    report_groups(L);
    report_defects(L, report->defects);

    lua_pcall(L, 3, 0, 0);

    lua_close(L);
}

void *report_free(Report *r, GDestroyNotify free_user)
{
    if (free_user) {
        g_slist_free_full(r->users, free_user);
    } else {
        g_slist_free(r->users);
    }

    report_defects_free(r->defects);

    g_free(r);
}

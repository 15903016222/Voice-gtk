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

void report_save(const Report *report)
{
    g_return_if_fail(report != NULL
            && report->tmpl != NULL
            && report->header.reportFile != NULL);

    lua_State *L = lua_open();
    if (NULL == L) {
        g_warning("loading report library failed");
        return;
    }
    luaL_openlibs(L);

    if ( luaL_dofile(L, "./report.lua") != 0 ) {
        g_warning("loading report library failed[2]");
        lua_close(L);
    }

    lua_getglobal(L, "output");
    lua_pushstring(L, report->tmpl);
    lua_pushstring(L, report->header.reportFile);
    lua_createtable(L, 0, 0);

    report_header(L, &report->header);


    lua_pcall(L, 3, 0, 0);

    lua_close(L);
}

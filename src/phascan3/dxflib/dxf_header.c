/**
 * @file dxf_header.c
 * @brief dxf header层
 * @author JakeYang <add358@gmail.com>
 * @version 0.2
 * @date 2015-04-14
 */

#include "dxf_header.h"

static inline DxfHeader *dxf_header_new_item()
{
    DxfHeader *h = g_malloc0(sizeof(DxfHeader));
    h->angdir = DXF_ANGDIR_COUNTERCLOCKWISE;
    h->insunits = DXF_UNIT_MM;
    h->ucsxdir.x = DXF_AXIS_X_RIGHT;
    h->ucsydir.y = DXF_AXIS_Y_UPWARD;
    return h;
}

static void header_get_point(Dxfile *f, DxfPointData *p);
static inline void header_get_line(Dxfile *f, gchar **str);

static inline void header_acadver(Dxfile *f, DxfHeader *h);
static inline void header_angdir(Dxfile *f, DxfHeader *h);
static inline void header_insunits(Dxfile *f, DxfHeader *h);

static inline void header_pucsname(Dxfile *f, DxfHeader *h);
static inline void header_pucsorg(Dxfile *f, DxfHeader *h);
static inline void header_pucsxdir(Dxfile *f, DxfHeader *h);
static inline void header_pucsydir(Dxfile *f, DxfHeader *h);

static inline void header_ucsorg(Dxfile *f, DxfHeader *h);
static inline void header_ucsname(Dxfile *f, DxfHeader *h);
static inline void header_ucsxdir(Dxfile *f, DxfHeader *h);
static inline void header_ucsydir(Dxfile *f, DxfHeader *h);

static inline void header_ltscale(Dxfile *f, DxfHeader *h);

typedef void (*VarFun)(const Dxfile *f, DxfHeader *h);
static GHashTable *headerMap = NULL;

void dxf_header_init()
{
    g_return_if_fail( headerMap == NULL );
    headerMap = g_hash_table_new(g_str_hash, g_str_equal);

    g_hash_table_insert(headerMap, HEADER_ACADVER, header_acadver);
    g_hash_table_insert(headerMap, HEADER_ANGDIR, header_angdir);
    g_hash_table_insert(headerMap, HEADER_INSUNITS, header_insunits);

    g_hash_table_insert(headerMap, HEADER_PUCSNAME, header_pucsname);
    g_hash_table_insert(headerMap, HEADER_PUCSORG, header_pucsorg);
    g_hash_table_insert(headerMap, HEADER_PUCSXDIR, header_pucsxdir);
    g_hash_table_insert(headerMap, HEADER_PUCSYDIR, header_pucsydir);

    g_hash_table_insert(headerMap, HEADER_UCSNAME, header_ucsname);
    g_hash_table_insert(headerMap, HEADER_UCSORG, header_ucsorg);
    g_hash_table_insert(headerMap, HEADER_UCSXDIR, header_ucsxdir);
    g_hash_table_insert(headerMap, HEADER_UCSYDIR, header_ucsydir);

    g_hash_table_insert(headerMap, HEADER_LTSCALE, header_ltscale);
}

void dxf_header_uninit()
{
    g_return_if_fail( headerMap != NULL );
    g_hash_table_destroy(headerMap);
}

DxfHeader *dxf_header_parse(Dxfile *f)
{
    DxfHeader *h = NULL;
    gchar *varName = NULL;
    VarFun var_f = NULL;

    g_return_val_if_fail( f != NULL, NULL );

    if ( !dxfile_lseek_section(f, DXF_SECTION_HEADER) ) {
        return NULL;
    }

    h = dxf_header_new_item();


    SWITCH_CODE_BEGIN(f);
    SWITCH_CODE_DO( 9 ) {
        /*获取变量名称,并处理*/
        dxfile_get_line(f, &varName, NULL);
        var_f = g_hash_table_lookup(headerMap, varName);
        g_free(varName);
        varName = NULL;
        if (var_f != NULL) {
            var_f(f, h);
        }
    }
    SWITCH_CODE_END(f);

    return h;
}


void dxf_header_delete(DxfHeader *h)
{
    g_return_if_fail( h != NULL );

    g_free(h->acadver);
    g_free(h->pucsname);
    g_free(h->ucsname);

    g_free(h);
}

gchar *dxf_header_print(DxfHeader *h)
{
    g_return_val_if_fail( h != NULL, NULL );
    return g_strdup_printf("----- Header -----\n"
                           "$ACADVER    : %s\n"
                           "$ANGDIR     : %d\n"
                           "$INSUNITS   : %d\n"
                           "$PUCSNAME   : %s\n"
                           "$PUCSORG    : (%g, %g, %g)\n"
                           "$PUCSXDIR   : (%g, %g, %g)\n"
                           "$PUCSYDIR   : (%g, %g, %g)\n"
                           "$UCSNAME    : %s\n"
                           "$UCSORG     : (%g, %g, %g)\n"
                           "$UCSXDIR    : (%g, %g, %g)\n"
                           "$UCSYDIR    : (%g, %g, %g)\n"
                           "$LTSCALE    : %g\n",
                           h->acadver,
                           h->angdir,
                           h->insunits,
                           h->pucsname,
                           h->pucsorg.x, h->pucsorg.y, h->pucsorg.z,
                           h->pucsxdir.x, h->pucsxdir.y, h->pucsxdir.z,
                           h->pucsydir.x, h->pucsydir.y, h->pucsydir.z,
                           h->ucsname,
                           h->ucsorg.x, h->ucsorg.y, h->ucsorg.z,
                           h->ucsxdir.x, h->ucsxdir.y, h->ucsxdir.z,
                           h->ucsydir.x, h->ucsydir.y, h->ucsydir.z,
                           h->ltscale);
}

static void header_get_point(Dxfile *f, DxfPointData *p)
{
    gint code = 0;
    glong pos = 0;
    gboolean flag = TRUE;
    while( flag ) {
        pos = dxfile_get_pos(f);
        dxfile_get_code(f, &code);
        switch( code ) {
        case 10:
            dxfile_get_double(f, &p->x);
            break;
        case 20:
            dxfile_get_double(f, &p->y);
            break;
        case 30:
            dxfile_get_double(f, &p->z);
            break;
        default:
            flag = FALSE;
            break;
        }
    }
    dxfile_set_pos(f, pos);
}

static inline void header_get_line(Dxfile *f, gchar **str)
{
    dxfile_goto_nextline(f);
    dxfile_get_line(f, str, NULL);
}

static inline void header_acadver(Dxfile *f, DxfHeader *h)
{
    header_get_line(f, &h->acadver);
}

static inline void header_angdir(Dxfile *f, DxfHeader *h)
{
    gint16 angdir = 0;
    dxfile_goto_nextline(f);
    dxfile_get_int16(f, &angdir);
    if (angdir < DXF_ANGDIR_MAX && angdir >= 0) {
        h->angdir = angdir;
    }
}

static inline void header_insunits(Dxfile *f, DxfHeader *h)
{
    gint16 unit = 0;
    dxfile_goto_nextline(f);
    dxfile_get_int16(f, &unit);
    if (unit > DXF_UNIT_PS || unit < 0) {
        return;
    }
    h->insunits = unit;
}

static inline void header_pucsname(Dxfile *f, DxfHeader *h)
{
    header_get_line(f, &h->pucsname);
}

static inline void header_pucsorg(Dxfile *f, DxfHeader *h)
{
    header_get_point(f, &h->pucsorg);
}

static inline void header_pucsxdir(Dxfile *f, DxfHeader *h)
{
    header_get_point(f, &h->pucsxdir);
}

static inline void header_pucsydir(Dxfile *f, DxfHeader *h)
{
    header_get_point(f, &h->pucsydir);
}

static inline void header_ucsorg(Dxfile *f, DxfHeader *h)
{
    header_get_point(f, &h->ucsorg);
}

static inline void header_ucsname(Dxfile *f, DxfHeader *h)
{
    header_get_line(f, &h->ucsname);
}

static inline void header_ucsxdir(Dxfile *f, DxfHeader *h)
{
    header_get_point(f, &h->ucsxdir);
}

static inline void header_ucsydir(Dxfile *f, DxfHeader *h)
{
    header_get_point(f, &h->ucsydir);
}

static inline void header_ltscale(Dxfile *f, DxfHeader *h)
{
    dxfile_goto_nextline(f);
    dxfile_get_double(f, &h->ltscale);
}

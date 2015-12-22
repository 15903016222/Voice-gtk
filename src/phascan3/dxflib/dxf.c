#include "dxf.h"
#include "dxfile.h"
#include <glib.h>

static gboolean initFlag = FALSE;   /*初始化标志*/

static inline Dxf *dxf_new_Item() { return g_malloc0(sizeof(Dxf)); }
static Dxf *_dxf_parse_file(Dxfile *f, const DxfSectionFlag parseFlag);

Dxf *dxf_new_for_file(const gchar *filename, const DxfSectionFlag flag)
{
    Dxfile *dxfile = NULL;
    Dxf *dxf = NULL;

    g_return_val_if_fail( initFlag == TRUE, NULL);

    dxfile = dxfile_open(filename);
    if (NULL == dxfile) {
        c_log_info("Open dxfile [%s] failed", filename);
        return NULL;
    }

    dxf = _dxf_parse_file(dxfile, flag);

    dxfile_destory(dxfile);
    return dxf;
}

static Dxf *_dxf_parse_file(Dxfile *f, const DxfSectionFlag parseFlag)
{
    Dxf *dxf = NULL;

    dxf = dxf_new_Item();

    if ((parseFlag & DXF_SECTION_HEADER)) {
        /*解析HEADER段*/
        DxfHeader *h = dxf_header_parse(f);
        if (NULL == h) {
            c_log_err("header parse failed");
            goto PARSE_FAILED;
        }
        dxf->header = h;
    }

    if ((parseFlag & DXF_SECTION_CLASSES)) {
        /*解析CLASSES段*/
    }

    if ((parseFlag & DXF_SECTION_TABLES)) {
        /*解析TABLES段*/
        DxfTables *t = dxf_tables_parse(f);
        if (NULL == t) {
            c_log_err("tables parse failed");
            goto PARSE_FAILED;
        }
        dxf->tables = t;
    }

    if ((parseFlag & DXF_SECTION_BLOCKS)) {
        /*解析BLOCKS段*/
    }

    if ((parseFlag & DXF_SECTION_ENTITIES)) {
        /*解析ENTITIES段*/
        DxfEntities *e = dxf_entities_parse(f);
        if (NULL == e) {
            c_log_err("entities parse failed");
            goto PARSE_FAILED;
        }
        dxf->entities = e;
    }

    if ((parseFlag & DXF_SECTION_OBJECTS)) {
        /*解析OBJECTS段*/
    }

    return dxf;

PARSE_FAILED:
    dxf_delete(dxf);
    return NULL;
}


void dxf_delete(Dxf *dxf)
{
    g_return_if_fail(dxf != NULL);
    g_return_if_fail( initFlag == TRUE );

    dxf_header_delete(dxf->header);
    dxf_tables_delete(dxf->tables);
    dxf_entities_delete(dxf->entities);

    g_free(dxf);
}

void dxf_init()
{
    g_return_if_fail( initFlag == FALSE );

    dxf_header_init();
    dxf_tables_init();
    dxf_entities_init();

    initFlag = TRUE;
}


void dxf_uninit()
{
    g_return_if_fail( initFlag == TRUE );

    dxf_header_uninit();
    dxf_tables_uninit();
    dxf_entities_uninit();

    initFlag = FALSE;
}

#include "dxf.h"
#include "dxfile.h"
#include <glib.h>

static gboolean initFlag = FALSE;   /*初始化标志*/

static inline Dxf *dxf_new_Item() { return g_malloc0(sizeof(Dxf)); }
static Dxf *_dxf_parse_file(Dxfile *f, const DxfParseFlag parseFlag);

Dxf *dxf_new_for_file(const gchar *filename, const DxfParseFlag flag)
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

static Dxf *_dxf_parse_file(Dxfile *f, const DxfParseFlag parseFlag)
{
    Dxf *dxf = NULL;
    Section s;
    gboolean flag = TRUE;

    dxf = dxf_new_Item();

    while( ! dxfile_is_end(f) ) {

        dxfile_get_section(f, &s);

        if (section_is_eof(&s)) {
            break;
        }else if (!section_is_begin(&s)) {
            /*不是段开始，则重新获取段内容*/
            c_log_info("[dxf:%d] invalid section(%d:%s)", __LINE__, s.code, s.value);
            continue;
        }

        dxfile_get_section(f, &s);
        if ((parseFlag & DXF_PARSE_HEADER) && section_is_header(&s)) {
            /*解析HEADER段*/
            DxfHeader *h = dxf_header_parse(f);
            if (NULL == h) {
                c_log_err("header parse failed");
                flag = FALSE;
                break;
            }
            if (dxf->header != NULL) {          /*防止恶意文件，重复包含HEADER段*/
                dxf_header_delete(dxf->header);
            }
            dxf->header = h;
        } else if ((parseFlag & DXF_PARSE_CLASSES) && section_is_classes(&s)) {
            /*解析CLASSES段*/
        } else if ((parseFlag & DXF_PARSE_TABLES) && section_is_tables(&s)) {
            /*解析TABLES段*/
            DxfTables *t = dxf_tables_parse(f);
            if (NULL == t) {
                c_log_err("tables parse failed");
                flag = FALSE;
                break;
            }
            if (dxf->tables != NULL) {          /*防止恶意文件，重复包含TABLES段*/
                dxf_tables_delete(dxf->tables);
            }
            dxf->tables = t;
        } else if ((parseFlag & DXF_PARSE_BLOCKS) && section_is_blocks(&s)) {
            /*解析BLOCKS段*/
        } else if ((parseFlag & DXF_PARSE_ENTITIES) && section_is_entities(&s)) {
            /*解析ENTITIES段*/
            DxfEntities *e = dxf_entities_parse(f);
            if (NULL == e) {
                c_log_err("entities parse failed");
                flag = FALSE;
                break;
            }
            if (dxf->entities != NULL) {        /*防止恶意文件重复包含ENTITIES段*/
                dxf_tables_delete(dxf->entities);
            }
            dxf->entities = e;
        } else if ((parseFlag & DXF_PARSE_OBJECTS) && section_is_objects(&s)) {
            /*解析OBJECTS段*/
        } else if (s.code == DXF_CODE_COMMENT) {
            /*输出注释字符串*/
            c_debug("comment : %s", s.value);
        } else {
            c_log_info("[dxf:%d] invalid section(%d:%s)", __LINE__, s.code, s.value);
        }
    }

    if (! flag) {
        dxf_delete(dxf);
        dxf = NULL;
    }

    return dxf;
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

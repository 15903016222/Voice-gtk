
#include "entities.h"
#include "dxf_map.h"

static inline DxfEntity *entity_new_item()
{
    return g_malloc0(sizeof(DxfEntity));
}

static DxfAcDbEntity *entity_submarker_parse(Dxfile *f);

/**
 * @brief entity_delete 释放DxfEntity资源
 * @param data          指向DxfEntity类型
 */
static void entity_delete(gpointer data);

/**
 * @brief entity_cmn_print    打印ENTITY通用部分信息
 * @param e                     指向DxfEntity类型
 * @return                      成功返回打印信息，失败返回NULL
 */
static inline gchar *entity_cmn_print(DxfEntity *e);

/**
 * @brief entity_print  打印ENTITY信息
 * @param data          指向DxfEntity类型
 * @param userData      保存输出的数据
 */
static void entity_print(gpointer data, gpointer userData);

/*定义entities映射表*/
static DxfMap *entitiesMap = NULL;

static inline void entities_map_insert(const gpointer name,
                                       const gpointer parse_f,
                                       const gpointer delete_f,
                                       const gpointer print_f)
{
    return dxf_map_insert(entitiesMap, name, parse_f, delete_f, print_f);
}

void dxf_entities_init()
{
    g_return_if_fail( entitiesMap == NULL );
    entitiesMap = dxf_map_new();

    entities_map_insert(ENTITIES_3DSOLID,   dxf_3dsolid_parse,  dxf_3dsolid_delete, dxf_3dsolid_print );
    entities_map_insert(ENTITIES_ARC,       dxf_arc_parse,      dxf_arc_delete,     dxf_arc_print);
    entities_map_insert(ENTITIES_CIRCLE,    dxf_circle_parse,   dxf_circle_delete,  dxf_circle_print);
    entities_map_insert(ENTITIES_ELLIPSE,   dxf_ellipse_parse,  dxf_ellipse_delete, dxf_ellipse_print);
    entities_map_insert(ENTITIES_IMAGE,     dxf_image_parse,    dxf_image_delete,   dxf_image_print);
    entities_map_insert(ENTITIES_LINE,      dxf_line_parse,     dxf_line_delete,    dxf_line_print);
    entities_map_insert(ENTITIES_LWPOLYLINE,    dxf_lwpolyline_parse,   dxf_lwpolyline_detele,  dxf_lwpolyline_print);
    entities_map_insert(ENTITIES_MTEXT,     dxf_mtext_parse,    dxf_mtext_delete,   dxf_mtext_print);
    entities_map_insert(ENTITIES_POINT,     dxf_point_parse,    dxf_point_delete,   dxf_point_print);
    entities_map_insert(ENTITIES_SPLINE,    dxf_spline_parse,   dxf_spline_delete,  dxf_spline_print);
    entities_map_insert(ENTITIES_TEXT,      dxf_text_parse,     dxf_text_delete,    dxf_text_print);
}

void dxf_entities_uninit()
{
    dxf_map_destroy(entitiesMap);
    entitiesMap = NULL;
}

static DxfEntity *entity_parse(Dxfile *f, const Section *s)
{
    DxfEntity *e = NULL;            /*图元数据*/
    DxfMapItem *item = NULL;
    gboolean flag = TRUE;

    if (s->code != 0) {
        c_log_warning("[%s] invalid code %d", __func__, s->code);
        return NULL;
    }

    item = g_hash_table_lookup(entitiesMap, s->value);
    if (NULL == item) {
        c_log_warning("[%s] cann't get item for [%d:%s]", __func__, s->code, s->value);
        return NULL;
    }

    e =  entity_new_item();

    e->name = g_strdup(s->value);

    SWITCH_CODE_BEGIN(f);
    SWITCH_CODE_DO( ENTITIES_CODE_HANDLE )
            dxfile_get_line(f, &e->handle, NULL);
    SWITCH_CODE_DO( ENTITIES_CODE_ID_TO_BLK_RECORD )
            dxfile_get_int16(f, &e->ID2BlkRecord);
    SWITCH_CODE_DO( DXF_CODE_SUBCLASS_MARKER ){
        gchar *str = NULL;
        dxfile_get_line(f, &str, NULL);
        if (!g_strcmp0(str, ENTITIES_SUBMARKER_ENTITY)) {
            e->acDbEntity = entity_submarker_parse(f);
            if (NULL == e->acDbEntity) {
                c_log_err("[%s] parse %s AcDbEntity section failed", __func__, s->value);
                flag = FALSE;
            }
            g_free(str);
        } else {
            SWITCH_CODE_SEEK(f);
            e->spData = item->parse_f(f);
            if (NULL == e->spData) {
                c_log_err("[%s] Parse %s special section failed", __func__, s->value);
                flag = FALSE;
            }
            g_free(str);
            SWITCH_CODE_OUT_OFFSET(f);
        }
    }
    SWITCH_CODE_END(f);

    if (! flag) {
        entity_delete(e);
        e = NULL;
    }

    return e;
}

DxfEntities *dxf_entities_parse(Dxfile *f)
{
    DxfEntities *entities = NULL;   /*图元链表*/
    DxfEntity *e = NULL;            /*图元数据*/
    Section s;                      /*组码*/

    g_return_val_if_fail(f != NULL, NULL);

    if ( !dxfile_lseek_section(f, DXF_SECTION_ENTITIES) ) {
        return NULL;
    }

    while (TRUE) {

        dxfile_get_section(f, &s);
        if (section_is_end(&s)
                || dxfile_is_end(f)) {
            break;
        }

        e = entity_parse(f, &s);
        if (NULL == e) {
            c_log_warning("[%s] invalid entities %d:%s",__func__, s.code, s.value);
            continue;
        }

        entities = g_list_append(entities, e);  /*追加到链表中*/
    }

    return entities;
}

static DxfAcDbEntity *entity_submarker_parse(Dxfile *f)
{
    DxfAcDbEntity *acDbEntity = NULL;

    acDbEntity = g_malloc0(sizeof(DxfAcDbEntity));

    SWITCH_CODE_BEGIN(f);
    SWITCH_CODE_DO( ENTITIES_CODE_LAYER )
            dxfile_get_line(f, &acDbEntity->layerName, NULL);
    SWITCH_CODE_DO( ENTITIES_CODE_LINETYPE )
            dxfile_get_line(f, &acDbEntity->linetype, NULL);
    SWITCH_CODE_DO( ENTITIES_CODE_COLOR )
            dxfile_get_int16(f, &acDbEntity->color);
    SWITCH_CODE_DO( ENTITIES_CODE_LINEWEIGHT )
            dxfile_get_int16(f, &acDbEntity->lineweight);
    SWITCH_CODE_DO( DXF_CODE_SUBCLASS_MARKER )
            SWITCH_CODE_OUT();
    SWITCH_CODE_END(f);

    return acDbEntity;
}

static void entities_submarker_delete(DxfAcDbEntity *acDbentity)
{
    g_return_if_fail( acDbentity != NULL );

    g_free(acDbentity->layerName);
    g_free(acDbentity->linetype);

    g_free(acDbentity);
}

static void entity_delete(gpointer data)
{
    DxfEntity *e = data;
    DxfMapItem *item = NULL;

    g_return_if_fail(data != NULL);
    g_return_if_fail(e->name != NULL);

    item = g_hash_table_lookup(entitiesMap, e->name);
    if (item) {
        item->delete_f(e->spData);
    }

    entities_submarker_delete(e->acDbEntity);

    g_free(e->name);
    g_free(e->handle);

    g_free(e); //释放该数据结点所占空间
}

void dxf_entities_delete(DxfEntities *es)
{
    g_return_if_fail(es!=NULL);
    g_list_free_full(es, entity_delete);
}

/**
 * @brief entities_cmn_print    打印图元公共头信息
 * @param e                     图元数据
 * @return                      成功返回字符串，失败返回NULL; 返回的字符串需要调用g_free来释放
 */
static inline gchar *entity_cmn_print(DxfEntity *e)
{
    return g_strdup_printf("----- Enitity %s ------\n"
                           "Handle        : %s\n"
                           "Layername     : %s\n"
                           "Linetype Name : %s\n"
                           "Color         : %hd\n"
                           "line weight   : %hd\n"
                           "ID/Handle to BLOCK_RECORD : %hd\n",
                           e->name,
                           e->handle,
                           e->acDbEntity->layerName,
                           e->acDbEntity->linetype,
                           e->acDbEntity->color,
                           e->acDbEntity->lineweight,
                           e->ID2BlkRecord);
}

static void entity_print(gpointer data, gpointer userData)
{
    DxfEntity *e = data;
    DxfMapItem *item = NULL;

    GString *str = (GString *)userData;
    gchar *tmp;

    tmp = entity_cmn_print(e);
    g_string_append(str, tmp);
    g_free(tmp);

    item = g_hash_table_lookup(entitiesMap, e->name);
    if (NULL == item) {
        c_log_info("[entities] can't get print funtion for %s", e->name);
        return;
    }

    tmp = item->print_f(e->spData);
    g_string_append(str, tmp);
    g_free(tmp);
}

gchar *dxf_entities_print(DxfEntities *es)
{
    GString *out = NULL;

    g_return_val_if_fail( es != NULL, NULL);

    out = g_string_sized_new(2048);

    g_list_foreach(es, entity_print, out);

    return g_string_free(out, FALSE);
}

#ifndef __ENTITIES_H__
#define __ENTITIES_H__

#include "entities_3dsolid.h"
#include "entities_arc.h"
#include "entities_circle.h"
#include "entities_ellipse.h"
#include "entities_image.h"
#include "entities_line.h"
#include "entities_lwpolyline.h"
#include "entities_mtext.h"
#include "entities_point.h"
#include "entities_spline.h"
#include "entities_text.h"

/* Entities Common Group Codes */
#define ENTITIES_CODE_NAME              -1  /*APP: entity name*/
#define ENTITIES_CODE_TYPE              0   /*entity type*/
#define ENTITIES_CODE_HANDLE            5   /*Entity handle*/
#define ENTITIES_CODE_LINETYPE          6   /*Linetype name*/
#define ENTITIES_CODE_LAYER             8   /*Layer name*/
#define ENTITIES_CODE_COLOR             62  /*Color number*/
#define ENTITIES_CODE_SUBMARKER         100 /*子类标记*/
#define ENTITIES_CODE_APP_GROUP         102 /*组的结束"}"*/
#define ENTITIES_CODE_ID_TO_BLK_RECORD  330 /*所有者BLOCK_RECORD对象的软指针ID/句柄*/
#define ENTITIES_CODE_LINEWEIGHT        370 /*线宽枚举值。作为 16 位整数存储和移动。*/

/* ENTITY NAME */
#define ENTITIES_3DFACE             "3DFACE"
#define ENTITIES_3DSOLID            "3DSOLID"
#define ENTITIES_ACAD_PROXY_ENTITY  "ACAD_PROXY_ENTITY"
#define ENTITIES_ARC                "ARC"
#define ENTITIES_ATTDEF             "ATTDEF"
#define ENTITIES_ATTRIB             "ATTRIB"
#define ENTITIES_BODY               "BODY"
#define ENTITIES_CIRCLE             "CIRCLE"
#define ENTITIES_DIMENSION          "DIMENSION"
#define ENTITIES_ELLIPSE            "ELLIPSE"
#define ENTITIES_HATCH              "HATCH"
#define ENTITIES_HELIX              "HELIX"
#define ENTITIES_IMAGE              "IMAGE"
#define ENTITIES_INSERT             "INSERT"
#define ENTITIES_LEADER             "LEADER"
#define ENTITIES_LIGHT              "LIGHT"
#define ENTITIES_LINE               "LINE"
#define ENTITIES_LWPOLYLINE         "LWPOLYLINE"
#define ENTITIES_MESH               "MESH"
#define ENTITIES_MLINE              "MLINE"
#define ENTITIES_MLEADERSTYLE       "MLEADERSTYLE"
#define ENTITIES_MLEADER            "MLEADER"
#define ENTITIES_MTEXT              "MTEXT"
#define ENTITIES_OLEFRAME           "OLEFRAME"
#define ENTITIES_OLE2FRAME          "OLE2FRAME"
#define ENTITIES_POINT              "POINT"
#define ENTITIES_POLYLINE           "POLYLINE"
#define ENTITIES_RAY                "RAY"
#define ENTITIES_REGION             "REGION"
#define ENTITIES_SECTION            "SECTION"
#define ENTITIES_SEQEND             "SEQEND"
#define ENTITIES_SHAPE              "SHAPE"
#define ENTITIES_SOLID              "SOLID"
#define ENTITIES_SPLINE             "SPLINE"
#define ENTITIES_SUN                "SUN"
#define ENTITIES_SURFACE            "SURFACE"
#define ENTITIES_TABLE              "TABLE"
#define ENTITIES_TEXT               "TEXT"
#define ENTITIES_TOLERANCE          "TOLERANCE"
#define ENTITIES_TRACE              "TRACE"
#define ENTITIES_UNDERLAY           "UNDERLAY"
#define ENTITIES_VERTEX             "VERTEX"
#define ENTITIES_VIEWPORT           "VIEWPORT"
#define ENTITIES_WIPEOUT            "WIPEOUT"
#define ENTITIES_XLINE              "XLINE"

/* ENTITIES Subclass Marker */
#define ENTITIES_SUBMARKER_ENTITY   "AcDbEntity"
#define ENTITIES_SUBMARKER_CIRCLE   "AcDbCircle"

/* Entity line type */
#define ENTITY_LINE_TYPE_BYLAYER    "ByLayer"
#define ENTITY_LINE_TYPE_BYBLOCK    "ByBlock"
#define ENTITY_LINE_TYPE_CONTINUOUS "Continuous"
#define ENTITY_LINE_TYPE_DOT        "DOT"
#define ENTITY_LINE_TYPE_DOT2       "DOT2"
#define ENTITY_LINE_TYPE_DOTX2      "DOTX2"
#define ENTITY_LINE_TYPE_DASHED     "DASHED"
#define ENTITY_LINE_TYPE_DASHED2    "DASHED2"
#define ENTITY_LINE_TYPE_DASHEDX2   "DASHEDX2"
#define ENTITY_LINE_TYPE_DASHDOT    "DASHDOT"
#define ENTITY_LINE_TYPE_DASHDOT2   "DASHDOT2"
#define ENTITY_LINE_TYPE_DASHDOTX2  "DASHDOTX2"
#define ENTITY_LINE_TYPE_DIVIDE     "DIVIDE"
#define ENTITY_LINE_TYPE_DIVIDE2    "DIVIDE2"
#define ENTITY_LINE_TYPE_DIVIDEX2   "DIVIDEX2"
#define ENTITY_LINE_TYPE_BORDER     "BORDER"
#define ENTITY_LINE_TYPE_BORDER2    "BORDER2"
#define ENTITY_LINE_TYPE_BORDERX2   "BORDERX2"
#define ENTITY_LINE_TYPE_CENTER     "CENTER"
#define ENTITY_LINE_TYPE_CENTER2    "CENTER2"
#define ENTITY_LINE_TYPE_CENTERX2   "CENTERX2"

typedef struct _DxfAcDbEntity DxfAcDbEntity;
typedef struct _DxfEntity DxfEntity;            /* Entity */
typedef GList DxfEntities;                      /* Entity List */

struct _DxfEntity{
    /*common*/
    gchar *name;            /*图元类型名称*/
    gchar *handle;          /*句柄*/
    gint16 ID2BlkRecord;    /*所有者BLOCK_RECORD对象的软件指针ID/句柄*/

    /* AcDbEntity */
    DxfAcDbEntity *acDbEntity;

    /*special*/
    gpointer spData;       /*各图形特有数据块，根据图元类型决定指向的图元结构*/
};

struct _DxfAcDbEntity {
    gchar *layerName;       /*图层名*/
    gchar *linetype;        /*线型名*/
    gint16 color;           /*颜色号*/
    gint16 lineweight;      /*线宽枚举值*/
};

///* 线形类型 */
//typedef enum {
//    LINE_TYPE_NONE,
//    LINE_TYPE_BYLAYER,
//    LINE_TYPE_BYBLOCK,
//    LINE_TYPE_CONTINUOUS,
//    LINE_TYPE_DOT,
//    LINE_TYPE_DOT_SMALL,
//    LINE_TYPE_DOT_LARGE,
//    LINE_TYPE_DASH,
//    LINE_TYPE_DASH_SMALL,
//    LINE_TYPE_DASH_LARGE,
//    LINE_TYPE_DASH_DOT,
//    LINE_TYPE_DASH_DOT_SMALL,
//    LINE_TYPE_DASH_DOT_LARGE,
//    LINE_TYPE_DIVIDE,
//    LINE_TYPE_DIVIDE_SMALL,
//    LINE_TYPE_DIVIDE_LARGE,
//    LINE_TYPE_CENTER,
//    LINE_TYPE_CENTER_SMALL,
//    LINE_TYPE_CENTER_LARGE,
//    LINE_TYPE_BORDER,
//    LINE_TYPE_BORDER_SMALL,
//    LINE_TYPE_BORDER_LARGE
//} EntityLineType;

/**
 * @brief dxf_entities_init 初始化entities模块，调用该模块的其他接口之前必须初始化
 */
extern void dxf_entities_init(void);

/**
 * @brief dxf_entities_uninit   卸载entities模块
 */
extern void dxf_entities_uninit(void);

/**
 * @brief dxf_entities_parse    图元解析
 * @param f                     Dxfile类型
 * @return                      成功返回DxfEntities类型数据，失败返回NULL
 */
extern DxfEntities *dxf_entities_parse(Dxfile *f);

/**
 * @brief dxf_entities_delete   图元删除
 * @param es                    DxfEntities类型
 */
extern void dxf_entities_delete(DxfEntities *es);

/**
 * @brief dxf_entities_print    图元打印
 * @param es                    DxfEntities类型
 * @return                      返回打印内容
 */
extern gchar *dxf_entities_print(DxfEntities *es);

/*图元类型判断*/
static inline gboolean entity_is_line(const DxfEntity *e){ return ! g_strcmp0(e->name, ENTITIES_LINE); }
static inline gboolean entity_is_arc(const DxfEntity *e) { return ! g_strcmp0(e->name, ENTITIES_ARC); }
static inline gboolean entity_is_circle(const DxfEntity *e) { return ! g_strcmp0(e->name, ENTITIES_CIRCLE); }
static inline gboolean entity_is_ellipse(const DxfEntity *e) { return ! g_strcmp0(e->name, ENTITIES_ELLIPSE); }
static inline gboolean entity_is_lwpolyline(const DxfEntity *e) { return ! g_strcmp0(e->name, ENTITIES_LWPOLYLINE); }
static inline gboolean entity_is_point(const DxfEntity *e) { return ! g_strcmp0(e->name, ENTITIES_POINT); }
static inline gboolean entity_is_mtext(const DxfEntity *e) { return ! g_strcmp0(e->name, ENTITIES_MTEXT); }
static inline gboolean entity_is_spline(const DxfEntity *e) { return ! g_strcmp0(e->name, ENTITIES_SPLINE); }

/*图元线型判断*/
static inline gboolean entity_linetype_is_bylayer(const DxfEntity *e) { return ! g_strcmp0(e->acDbEntity->linetype, ENTITY_LINE_TYPE_BYLAYER); }
static inline gboolean entity_linetype_is_byblock(const DxfEntity *e) { return ! g_strcmp0(e->acDbEntity->linetype, ENTITY_LINE_TYPE_BYBLOCK); }
static inline gboolean entity_linetype_is_continuous(const DxfEntity *e) { return ! g_strcmp0(e->acDbEntity->linetype, ENTITY_LINE_TYPE_CONTINUOUS); }
static inline gboolean entity_linetype_is_dot(const DxfEntity *e) { return ! g_strcmp0(e->acDbEntity->linetype, ENTITY_LINE_TYPE_DOT); }
static inline gboolean entity_linetype_is_dot2(const DxfEntity *e) { return ! g_strcmp0(e->acDbEntity->linetype, ENTITY_LINE_TYPE_DOT2); }
static inline gboolean entity_linetype_is_dotx2(const DxfEntity *e) { return ! g_strcmp0(e->acDbEntity->linetype, ENTITY_LINE_TYPE_DOTX2); }
static inline gboolean entity_linetype_is_dashed(const DxfEntity *e) { return ! g_strcmp0(e->acDbEntity->linetype, ENTITY_LINE_TYPE_DASHED); }
static inline gboolean entity_linetype_is_dashed2(const DxfEntity *e) { return ! g_strcmp0(e->acDbEntity->linetype, ENTITY_LINE_TYPE_DASHED2); }
static inline gboolean entity_linetype_is_dashedx2(const DxfEntity *e) { return ! g_strcmp0(e->acDbEntity->linetype, ENTITY_LINE_TYPE_DASHEDX2); }
static inline gboolean entity_linetype_is_dashdot(const DxfEntity *e) { return ! g_strcmp0(e->acDbEntity->linetype, ENTITY_LINE_TYPE_DASHDOT); }
static inline gboolean entity_linetype_is_dashdot2(const DxfEntity *e) { return ! g_strcmp0(e->acDbEntity->linetype, ENTITY_LINE_TYPE_DASHDOT2); }
static inline gboolean entity_linetype_is_dashdotx2(const DxfEntity *e) { return ! g_strcmp0(e->acDbEntity->linetype, ENTITY_LINE_TYPE_DASHDOTX2); }
static inline gboolean entity_linetype_is_divide(const DxfEntity *e) { return ! g_strcmp0(e->acDbEntity->linetype, ENTITY_LINE_TYPE_DIVIDE); }
static inline gboolean entity_linetype_is_divide2(const DxfEntity *e) { return ! g_strcmp0(e->acDbEntity->linetype, ENTITY_LINE_TYPE_DIVIDE2); }
static inline gboolean entity_linetype_is_dividex2(const DxfEntity *e) { return ! g_strcmp0(e->acDbEntity->linetype, ENTITY_LINE_TYPE_DIVIDEX2); }
static inline gboolean entity_linetype_is_border(const DxfEntity *e) { return ! g_strcmp0(e->acDbEntity->linetype, ENTITY_LINE_TYPE_BORDER); }
static inline gboolean entity_linetype_is_border2(const DxfEntity *e) { return ! g_strcmp0(e->acDbEntity->linetype, ENTITY_LINE_TYPE_BORDER2); }
static inline gboolean entity_linetype_is_borderx2(const DxfEntity *e) { return ! g_strcmp0(e->acDbEntity->linetype, ENTITY_LINE_TYPE_BORDERX2); }
static inline gboolean entity_linetype_is_center(const DxfEntity *e) { return ! g_strcmp0(e->acDbEntity->linetype, ENTITY_LINE_TYPE_CENTER); }
static inline gboolean entity_linetype_is_center2(const DxfEntity *e) { return ! g_strcmp0(e->acDbEntity->linetype, ENTITY_LINE_TYPE_CENTER2); }
static inline gboolean entity_linetype_is_centerx2(const DxfEntity *e) { return ! g_strcmp0(e->acDbEntity->linetype, ENTITY_LINE_TYPE_CENTERX2); }

#define dxf_entities_foreach(entities, item) \
    for( ; entities && (item = entities->data) ; entities = g_list_next(entities) )

#endif /* __ENTITIES_H__ */

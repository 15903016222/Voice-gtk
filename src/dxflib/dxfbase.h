/**
 * @file dxfbase.h
 * @brief dxf基本接口
 * @author JakeYang <add358@gmail.com>
 * @version 0.2
 * @date 2015-04-14
 */

#ifndef __DXFBASE_H__
#define __DXFBASE_H__

#include <glib.h>
#include <stdio.h>
#include <string.h>

#include "c_message.h"

#define STR_MAX     256

#define SECTION_START                       "SECTION"
#define SECTION_END                         "ENDSEC"
#define SECTION_EOF                         "EOF"

/*sections*/
#define SECTION_HEADER                      "HEADER"
#define SECTION_CLASSES                     "CLASSES"
#define SECTION_TABLES                      "TABLES"
#define SECTION_BLOCKS                      "BLOCKS"
#define SECTION_ENTITIES                    "ENTITIES"
#define SECTION_OBJECTS                     "OBJECTS"


/**
 *                      dxf common group codes
 * “fixed” indicates that the group code always has the same purpose
 */
#define DXF_CODE_INVALID                (-10)
#define DXF_CODE_SUBCLASS_MARKER        100 /*子类标志 */
#define DXF_CODE_COMMENT                999 /*指示后面的行是注释字符串*/

typedef struct _Section Section;
struct _Section {
    gint code;              /*group code*/
    gchar value[STR_MAX];   /*group value*/
};

static inline gboolean section_check(const Section *s, gint code,
                                     const gchar *codestr)
{
    return ((s->code == code) && (! g_strcmp0(s->value, codestr)));
}
#define section_is_begin(s) \
    section_check(s, 0, SECTION_START)      /*判断为段开始*/
#define section_is_end(s) \
    section_check(s, 0, SECTION_END)        /*判断为段结束*/
#define section_is_eof(s) \
    section_check(s, 0, SECTION_EOF)        /*判断为文件结束*/
#define section_clean(s) \
    memset(s, 0, sizeof(Section))

/* Point */
typedef struct _DxfPointData DxfPointData;
struct _DxfPointData {
    gdouble x;
    gdouble y;
    gdouble z;
};

#endif /* __DXFBASE_H__ */

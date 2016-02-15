/**
 * @file dxf_tables_entries.h
 * @brief dxf tables层, entries封装
 * @author JakeYang <add358@gmail.com>
 * @version 0.2
 * @date 2015-04-14
 */


#ifndef __DXF_TABLES_ENTRIES_H__
#define __DXF_TABLES_ENTRIES_H__

#include "dxfile.h"
#include "dxf_tables_ltype.h"
#include "dxf_tables_vport.h"

/*Entries Subclass Marker*/
#define ENTRIES_AcDbSymbolTableRecord    "AcDbSymbolTableRecord"
#define ENTRIES_AcDbLinetypeTableRecord  "AcDbLinetypeTableRecord"
#define ENTRIES_AcDbViewportTableRecord  "AcDbViewportTableRecord"

typedef GList DxfTablesEntries;                 /*条目链表，包含多个条目*/
typedef struct _DxfTablesEntry DxfTablesEntry;  /*单个条目*/

struct _DxfTablesEntry {
    /*通用属性*/
    gchar *type;
    gchar *handle;                  /*5/105:句柄*/
    gchar *softPointerHandler;      /*330:所有者对象的软指针 ID/句柄*/
    gchar *submarker;               /*100:条目的子类标记*/
    /*特定条目类型数据*/
    gpointer data;
};

/**
 * @brief dxf_tables_entries_init   初始化entries模块，调用该模块的其他接口之前必须初始化
 */
extern void dxf_tables_entries_init();

/**
 * @brief dxf_tables_entries_uninit 卸载entries模块
 */
extern void dxf_tables_entries_uninit();

/**
 * @brief dxf_tables_entries_parse   表的条目解析
 * @param f                         dxfile类型
 * @return                          成功返回DxfTableEntries
 */
extern DxfTablesEntries *dxf_tables_entries_parse(Dxfile *f);

/**
 * @brief dxf_tables_entries_delete 表的条目删除
 * @param es                        DxfTablesEntries类型
 */
extern void dxf_tables_entries_delete(DxfTablesEntries *es);

/**
 * @brief dxf_tables_entries_print  表的条目打印
 * @param es                        DxfTablesEntries类型
 * @return                          返回打印内容
 */
extern gchar *dxf_tables_entries_print(DxfTablesEntries *es);

#endif /* End of __DXF_TABLES_ENTRIES_H__ */

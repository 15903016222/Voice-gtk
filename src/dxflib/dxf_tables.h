/**
 * @file dxf_tables.h
 * @brief dxf tables层
 * @author JakeYang <add358@gmail.com>
 * @version 0.2
 * @date 2015-04-14
 */

#ifndef __DXF_TABLES_H__
#define __DXF_TABLES_H__

#include "dxf_tables_entries.h"

typedef GList DxfTables;                    /*表链表，包含多个表*/
typedef struct _DxfTable DxfTable;          /*单个表，包含多个条目*/

struct _DxfTable {
    /*通用属性*/
    gchar *name;                /*2:表名*/
    gchar *handle;              /*5:句柄*/
    gchar *softPointerHandler;  /*330:所有者对象的软指针 ID/句柄 */

    /*该表的所有条目*/
    DxfTablesEntries *entries;
};

/**
 * @brief dxf_tables_init   初始化tables模块，调用该模块其他接口前必须初始化
 */
extern void dxf_tables_init();

/**
 * @brief dxf_tables_uninit 卸载tables模块
 */
extern void dxf_tables_uninit();

/**
 * @brief dxf_tables_parse  tables解析
 * @param f                 DxfTables类型
 * @return                  成功返回DxfTables类型数据，失败返回NULL
 */
extern DxfTables *dxf_tables_parse(Dxfile *f);

/**
 * @brief dxf_tables_delete tables删除
 * @param t                 Dxftables类型
 */
extern void dxf_tables_delete(DxfTables *t);

/**
 * @brief dxf_tables_print  tables打印
 * @param t                 Dxftables类型
 * @return                  打印内容
 */
extern gchar *dxf_tables_print(DxfTables *t);

#endif /* End of __DXF_TABLES_H__ */

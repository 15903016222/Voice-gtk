/**
 * File: dxf_map.h
 * Desc: 映射表，保存解析、删除和打印函数
 * Author: JakeYang
 * Log: 2015-09-25
 */

#ifndef __DXF_MAP_H__
#define __DXF_MAP_H__

#include "dxfile.h"

/* 定义解析、删除、打印函数的类型 */
typedef gpointer (*parse_fun_t)(Dxfile *f);
typedef void (*delete_fun_t)(gpointer e);
typedef gchar *(*print_fun_t)(gpointer e);

/* 定义映射表和映射表项目 类型 */
typedef GHashTable DxfMap;
typedef struct _DxfMapItem DxfMapItem;
struct _DxfMapItem {
    parse_fun_t parse_f;
    delete_fun_t delete_f;
    print_fun_t print_f;
};

extern DxfMap *dxf_map_new();
extern void dxf_map_insert(DxfMap *map,
                           const gpointer key,
                           const gpointer parse_f,
                           const gpointer delete_f,
                           const gpointer print_f);
extern void dxf_map_destroy(DxfMap *map);

#endif /* End of __DXF_MAP_H__ */

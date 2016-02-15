/**
 * @file dxf_map.c
 * @brief dxf映射表封装
 * @author JakeYang <add358@gmail.com>
 * @version 0.2
 * @date 2015-04-14
 */

#include "dxf_map.h"

static DxfMapItem *dxf_map_item_new(const gpointer parse_f,
                                    const gpointer delete_f,
                                    const gpointer print_f)
{
    DxfMapItem *item = g_malloc0(sizeof(DxfMapItem));
    item->delete_f = delete_f;
    item->parse_f = parse_f;
    item->print_f = print_f;
    return item;
}

DxfMap *dxf_map_new()
{
    return g_hash_table_new_full(g_str_hash, g_str_equal, NULL, g_free);
}

void dxf_map_insert(DxfMap *map,
                    const gpointer key,
                    const gpointer parse_f,
                    const gpointer delete_f,
                    const gpointer print_f)
{
    DxfMapItem *item;

    g_return_if_fail( parse_f != NULL );
    g_return_if_fail( delete_f != NULL );

    item = dxf_map_item_new(parse_f, delete_f, print_f);
    g_hash_table_insert(map, key, item);
}

void dxf_map_destroy(DxfMap *map)
{
    g_return_if_fail( map != NULL );
    g_hash_table_destroy(map);
}

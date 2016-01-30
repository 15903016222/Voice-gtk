/**
 * @file dxf_tables_entries.c
 * @brief dxf tables层, entries封装
 * @author JakeYang <add358@gmail.com>
 * @version 0.2
 * @date 2015-04-14
 */

#include "dxf_tables_entries.h"
#include "dxf_map.h"

/**
 * @brief entries_map_insert    向entries映射表插入一条entry记录
 * @param name                  entry子类标志
 * @param parse_f               entry解析函数
 * @param delete_f              entry删除函数
 * @param print_f               entry打印函数
 */
static inline void entries_map_insert(const gpointer name,
                                      const gpointer parse_f,
                                      const gpointer delete_f,
                                      const gpointer print_f);
/**
 * @brief entry_parse   单个条目解析
 * @param f             Dxfile类型
 * @return              成功返回DxfTablesEnry数据，失败返回NULL
 */
static DxfTablesEntry *entry_parse(Dxfile *f);

/**
 * @brief entry_delete  单个条目删除
 * @param data          指向DxfTablesEntry数据
 */
static void entry_delete(gpointer data);

/**
 * @brief entry_print   单个条目打印
 * @param data          指向DxfTableEntry数据
 * @param userData      保存输出数据
 */
static void entry_print(gpointer data, gpointer userData);


static DxfMap *entriesMap = NULL;

void dxf_tables_entries_init()
{
    g_return_if_fail( entriesMap == NULL );

    entriesMap = dxf_map_new();

    entries_map_insert(ENTRIES_AcDbLinetypeTableRecord,
                       dxf_tables_ltype_parse,
                       dxf_tables_ltype_delete,
                       dxf_tables_ltype_print);
    entries_map_insert(ENTRIES_AcDbViewportTableRecord,
                       dxf_tables_vport_parse,
                       dxf_tables_vport_delete,
                       dxf_tables_vport_print);
}

void dxf_tables_entries_uninit()
{
    g_return_if_fail( entriesMap != NULL );
    dxf_map_destroy(entriesMap);
    entriesMap = NULL;
}

static inline void entries_map_insert(const gpointer name,
                                      const gpointer parse_f,
                                      const gpointer delete_f,
                                      const gpointer print_f)
{
    return dxf_map_insert(entriesMap, name, parse_f, delete_f, print_f);
}

DxfTablesEntries *dxf_tables_entries_parse(Dxfile *f)
{
    DxfTablesEntries *entries = NULL;
    DxfTablesEntry *entry = NULL;
    Section s;

    while(!dxfile_is_end(f)) {
        dxfile_get_section(f, &s);
        if (section_check(&s, 0, "ENDTAB")) {
            break;
        }
        entry = entry_parse(f);
        if (NULL == entry) {
            c_log_warning("[%s] invalid section(%d:%s)", __func__, s.code, s.value);
            break;
        }
        entries = g_list_append(entries, entry);
    }
    return entries;
}

void dxf_tables_entries_delete(DxfTablesEntries *es)
{
    g_return_if_fail(es != NULL);
    g_list_free_full(es, entry_delete);
}

gchar *dxf_tables_entries_print(DxfTablesEntries *es)
{
    GString *out = NULL;
    g_return_val_if_fail( es != NULL, NULL );
    out = g_string_sized_new(1024);
    g_list_foreach(es, entry_print, out);

    return g_string_free(out, FALSE);
}

static DxfTablesEntry *entry_parse(Dxfile *f)
{
    DxfTablesEntry *e = NULL;
    DxfMapItem *item = NULL;

    e = g_malloc0(sizeof(DxfTablesEntry));

    SWITCH_CODE_BEGIN(f);
    SWITCH_CODE_DO( 5 ) {}
    SWITCH_CODE_DO_FOLLOW( 105 )
            /*句柄*/
            dxfile_get_line(f, &e->handle, NULL);
//    SWITCH_CODE_DO( 330 )
//            /*所有者对象的软指针ID/句柄*/
//            dxfile_get_line(f, &e->softPointerHandler, NULL);
    SWITCH_CODE_DO( 100 ) {
        /*子类标记*/
        gchar *str = NULL;
        dxfile_get_line(f, &str, NULL);
        item = g_hash_table_lookup(entriesMap, str);
        if (NULL != item) {
            e->submarker = str;
            e->data = item->parse_f(f);
            SWITCH_CODE_OUT_OFFSET(f);
        } else {
            g_free(str);
            str = NULL;
        }
    }
    SWITCH_CODE_END(f);

    return e;
}

static void entry_delete(gpointer data)
{
    DxfTablesEntry *entry = data;
    DxfMapItem *item = NULL;
    g_return_if_fail(data != NULL);
    if (entry->submarker != NULL) {
        item = g_hash_table_lookup(entriesMap, entry->submarker);
    }
    if (item) {
        item->delete_f(entry->data);
    }
    g_free(entry->handle);
    g_free(entry->softPointerHandler);
    g_free(entry->submarker);
    g_free(entry->type);
    g_free(entry);
}


static void entry_print(gpointer data, gpointer userData)
{
    DxfTablesEntry *e = (DxfTablesEntry *)data;
    GString *str = (GString *)userData;
    gchar *tmp = NULL;
    DxfMapItem *item = NULL;

    tmp = g_strdup_printf("-- Entry %s --\n"
                          "handle : %s\n",
                          e->submarker,
                          e->handle);
    g_string_append(str, tmp);
    g_free(tmp);
    if (e->submarker != NULL) {
        item = g_hash_table_lookup(entriesMap, e->submarker);
    }
    if (item) {
        tmp = item->print_f(e->data);
        g_string_append(str, tmp);
        g_free(tmp);
    }
}

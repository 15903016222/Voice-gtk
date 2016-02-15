/**
 * @file dxf_tables_ltype.c
 * @brief dxf tables层, ltype封装
 * @author JakeYang <add358@gmail.com>
 * @version 0.2
 * @date 2015-04-14
 */

#include "dxf_tables_ltype.h"

DxfTablesLtype *dxf_tables_ltype_parse(Dxfile *f)
{
    DxfTablesLtype *ltype = g_malloc0(sizeof(DxfTablesLtype));

    SWITCH_CODE_BEGIN(f);
    SWITCH_CODE_DO( 2 )
            /*线型名*/
            dxfile_get_line(f, &ltype->name, NULL);
    SWITCH_CODE_DO( 70 )
             /* 标准标记值（按位编码值）：
             * 16 = 如果设定了此位，则表条目外部依赖于外部参照
             * 32 = 如果同时设定了此位和位 16，则表明已成功融入了外部依赖的外部参照
             * 64 = 如果设定了此位，则表明在上次编辑图形时，图形中至少有一个图元参照
             *      了表条目。（此标志对 AutoCAD 命令很有用。大多数读取 DXF 文件的
             *      程序都可以忽略它，并且无需由写入 DXF 文件的程序对其进行设定） */
            dxfile_get_int16(f, &ltype->flag);
    SWITCH_CODE_DO( 3 )
            /*线型的说明文字*/
            dxfile_get_line(f, &ltype->description, NULL);
    SWITCH_CODE_DO( 72 )
            /*对齐代码；值通常为 65，A 的 ASCII 代码*/
            dxfile_get_int16(f, &ltype->alignment);
    SWITCH_CODE_DO( 73 )
            /*线型元素的数目*/
            dxfile_get_int16(f, &ltype->elementsNum);
    SWITCH_CODE_DO( 40 )
            /*图案总长度*/
            dxfile_get_double(f, &ltype->totalPattenLength);
    SWITCH_CODE_DO( 49 )
            /*虚线、点或空间长度（每个元素一个条目）*/
            dxfile_get_double(f, &ltype->dashLength);
    SWITCH_CODE_DO( 74 )
            /* 复杂线型元素类型（每个元素一种类型）。默认值为 0（无嵌入的形/文字）
             * 以下代码是位值：
             * 1 = 如果设定了此位，则代码 50 指定绝对旋转；如果未设置此位，则代码 50 指定相对旋转
             * 2 = 嵌入元素是字符串
             * 4 = 嵌入元素是形 */
            dxfile_get_int16(f, &ltype->complexElement);
    SWITCH_CODE_DO( 75 )
            /* 如果代码 74 指定嵌入的形，则表示形编号（每个元素一个编号）
             * 如果代码 74 指定嵌入的字符串，此值将设定为 0
             * 如果代码 74 设定为 0，则省略代码 75 */
            dxfile_get_int16(f, &ltype->shapeNum);
    SWITCH_CODE_DO( 340 )
            /*指向 STYLE 对象的指针（如果代码 74 > 0，则每个元素一个指针）*/
            dxfile_get_line(f, &ltype->stylePointer, NULL);
    SWITCH_CODE_DO( 46 )
            /*S = 比例值（可选）；可存在多个条目*/
            dxfile_get_double(f, &ltype->scale);
    SWITCH_CODE_DO( 50 )
            /*R = 嵌入的形或文字的相对旋转值（以弧度为单位）
             *A = 嵌入的形或文字的绝对旋转值（以弧度为单位）
             *如果代码 74 指定了嵌入的形或字符串，则每个元素一个*/
            dxfile_get_double(f, &ltype->rotation);
    SWITCH_CODE_DO( 44 )
            /*X = X 偏移值（可选）；可存在多个条目*/
            dxfile_get_double(f, &ltype->xOffset);
    SWITCH_CODE_DO( 45 )
            /*Y = Y 偏移值（可选）；可存在多个条目*/
            dxfile_get_double(f, &ltype->yOffset);
    SWITCH_CODE_DO( 9 )
            /*字符串（如果代码 74 = 2，则每个元素一个字符串）*/
            dxfile_get_line(f, &ltype->text, NULL);
    SWITCH_CODE_END(f);

    return ltype;
}


void dxf_tables_ltype_delete(DxfTablesLtype *l)
{
    g_return_if_fail( l != NULL );

    g_free(l->name);
    g_free(l->description);
    g_free(l->text);
    g_free(l->stylePointer);
    g_free(l);
}


gchar *dxf_tables_ltype_print(DxfTablesLtype *l)
{
    return g_strdup_printf("name       : %s\n"
                           "description: %s\n"
                           "text       : %s\n"
                           "total patten length : %f\n"
                           "x offset   : %f\n"
                           "y offset   : %f\n"
                           "dash length: %f\n"
                           "scale      : %f\n"
                           "rotation   : %f\n"
                           "flag       : %d\n"
                           "alignment  : %d\n"
                           "elemnts num: %d\n"
                           "comple element: %d\n"
                           "shape num  : %d\n"
                           "style pointer : %s\n",
                           l->name,
                           l->description,
                           l->text,
                           l->totalPattenLength,
                           l->xOffset,
                           l->yOffset,
                           l->dashLength,
                           l->scale,
                           l->rotation,
                           l->flag,
                           l->alignment,
                           l->elementsNum,
                           l->complexElement,
                           l->shapeNum,
                           l->stylePointer);
}

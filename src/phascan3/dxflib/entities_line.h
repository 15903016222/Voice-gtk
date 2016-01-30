/**
 * @file entities_line.h
 * @brief dxf entities层, line对象
 * @author JakeYang <add358@gmail.com>
 * @version 0.2
 * @date 2015-04-14
 */

#ifndef __ENTITIES_LINE_H__
#define __ENTITIES_LINE_H__

#include "dxfile.h"

typedef struct _DxfLine DxfLine;
/* line */
struct _DxfLine {
    gdouble thickness;          /*39 Thickness (optional; default = 0)*/
    DxfPointData startPoint;    /*10,20,30 Start point (in WCS)*/
    DxfPointData endPoint;      /*11,21,31 Endpoint (in WCS)*/
    DxfPointData extrDir;       /*210,220,230 Extrusion direction (optional; default = 0, 0, 1) */
};

extern DxfLine *dxf_line_parse(Dxfile *f);
extern void dxf_line_delete(DxfLine *l);

/**
 * @brief line_print    输出线图元信息
 * @param l             线图元
 * @return              成功返回字符串，失败返回NULL; 返回的字符串需要调用g_free来释放
 */
extern gchar *dxf_line_print(const DxfLine *l);

#endif /* end of __ENTITIES_LINE_H__ */

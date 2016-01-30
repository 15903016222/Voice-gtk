/**
 * @file entities_point.h
 * @brief dxf entities层, point对象
 * @author JakeYang <add358@gmail.com>
 * @version 0.2
 * @date 2015-04-14
 */

#ifndef __ENTITIES_POINT_H__
#define __ENTITIES_POINT_H__

#include "dxfile.h"

typedef struct _DxfPoint DxfPoint;

struct _DxfPoint {
    DxfPointData data;          /*10,20,30 Point location (in WCS)*/
    DxfPointData extrDir;       /*210,220,230 Extrusion direction (optional; default = 0, 0, 1) */
    gdouble thickness;          /*39 Thickness (optional; default = 0)*/
};

extern DxfPoint *dxf_point_parse(Dxfile *f);
extern void dxf_point_delete(DxfPoint *p);
extern gchar *dxf_point_print(const DxfPoint *p);

#endif /* End of __ENTITIES_POINT_H__ */

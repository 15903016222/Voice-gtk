/**
 * @file entities_circle.h
 * @brief dxf entities层, circle对象
 * @author JakeYang <add358@gmail.com>
 * @version 0.2
 * @date 2015-04-14
 */

#ifndef __ENTITIES_CIRCLE_H__
#define __ENTITIES_CIRCLE_H__

#include "dxfile.h"

typedef struct _DxfCircle DxfCircle;
struct _DxfCircle {
    gdouble thickness;              /*39 Thickness (optional; default = 0)*/
    DxfPointData centerPoint;       /*10,20,30 Center point (in OCS)*/
    gdouble radius;                 /*40 Radius*/
    DxfPointData extrDir;           /*210,220,230 Extrusion direction (optional; default = 0, 0, 1) */
};

extern DxfCircle *dxf_circle_parse(Dxfile *f);
extern void dxf_circle_delete(DxfCircle *c);
extern gchar *dxf_circle_print(const DxfCircle *c);

#endif /* end of __ENTITIES_CIRCLE_H__ */

/**
 * @file entities_arc.h
 * @brief dxf entities层, arc对象
 * @author JakeYang <add358@gmail.com>
 * @version 0.2
 * @date 2015-04-14
 */

#ifndef __ENTITIES_ARC_H__
#define __ENTITIES_ARC_H__

#include "dxfile.h"
#include "entities_circle.h"

typedef struct _DxfArc DxfArc;
struct _DxfArc {
    gdouble thickness;          /*39 Thickness (optional; default = 0)*/
    DxfPointData centerPoint;   /*10,20,30 Center point (in OCS)*/
    gdouble radius;             /*40 Radius*/
    gdouble startAngle;         /*50 Start angle*/
    gdouble endAngle;           /*51 End angle*/
    DxfPointData extrDir;       /*210,220,230 Extrusion direction (optional; default = 0, 0, 1) */
};

extern DxfArc *dxf_arc_parse(Dxfile *f);
extern void dxf_arc_delete(DxfArc *e);
extern gchar *dxf_arc_print(DxfArc *e);

#endif /* End of __ENTITIES_ARC_H__ */

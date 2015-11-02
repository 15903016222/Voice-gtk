#ifndef __ENTITIES_ELLIPSE_H__
#define __ENTITIES_ELLIPSE_H__

#include "dxfile.h"

typedef struct _DxfEllipse DxfEllipse;
struct _DxfEllipse {
    DxfPointData centerPoint;           /*10,20,30 Center point (in WCS)*/
    DxfPointData endPointMajorAxis;     /*11,21,31 Endpoint of major axis, relative to the center (in WCS)*/
    DxfPointData extrDir;               /*210,220,230 Extrusion direction (optional; default = 0, 0, 1) */
    gdouble ration;                     /*40 Ratio of minor axis to major axis*/
    gdouble startAngle;                 /*41 Start parameter (this value is 0.0 for a full ellipse)*/
    gdouble endAngle;                   /*42 End parameter (this value is 2pi for a full ellipse)*/
};

extern DxfEllipse *dxf_ellipse_parse(Dxfile *f);
extern void dxf_ellipse_delete(DxfEllipse *e);
extern gchar *dxf_ellipse_print(const DxfEllipse *e);
extern gdouble dxf_ellipse_calc_endpoint_pi(const DxfEllipse *e);


#endif /* End of __ENTITIES_ELLIPSE_H__ */

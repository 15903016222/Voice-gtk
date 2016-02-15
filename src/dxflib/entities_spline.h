/**
 * @file entities_spline.h
 * @brief dxf entities层, spline对象
 * @author JakeYang <add358@gmail.com>
 * @version 0.2
 * @date 2015-04-14
 */

#ifndef __ENTITIES_SPLINE_H__
#define __ENTITIES_SPLINE_H__

#include "dxfile.h"

typedef struct _DxfSpline DxfSpline;
struct _DxfSpline {
    DxfPointData vector;            /*210,220,230 Normal vector (omitted if the spline is nonplanar)*/
    gint16 flag;                    /*70 Spline flag*/
    gint16 degree;                  /*71 Degree of the spline curve*/
    gint16 knotsNum;                /*72 Number of knots*/
    gint16 ctrlPointsNum;           /*73 Number of control points*/
    gint16 fitPointsNum;            /*74 Number of fit points (if any)*/
    gdouble knotTolerance;          /*42 Knot tolerance (default = 0.0000001)*/
    gdouble ctrlPointTolerance;     /*43 Control-point tolerance (default = 0.0000001)*/
    gdouble fitTolerance;           /*44 Fit tolerance (default = 0.0000000001)*/
    gdouble *knotValues;            /*40 Knot value (one entry per knot)*/
    DxfPointData *ctrlPoints;       /*10,20,30 Control points (in WCS); one entry per control point*/
};

extern DxfSpline *dxf_spline_parse(Dxfile *f);
extern void dxf_spline_delete(DxfSpline *e);
extern gchar *dxf_spline_print(DxfSpline *e);

#endif /* End of __ENTITIES_SPLINE_H__ */

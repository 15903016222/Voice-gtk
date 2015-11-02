#ifndef __ENTITIES_LWPOLYLINE_H__
#define __ENTITIES_LWPOLYLINE_H__

#include "dxfile.h"

/* 注意： 由于polyline的实现方式与lwpolyline一样，故此不在实现，只做标记 */

typedef enum {
    LWPOLYLINE_NONE,
    LWPOLYLINE_CLOSE = 1,
    LWPOLYLINE_CURVE_FIT = 1<<1,
    LWPOLYLINE_SPLINE_FIT = 1<<2,
    LWPOLYLINE_3D = 1<<3,
    LWPOLYLINE_3D_POLYGON_MESG = 1<<4,
} LwPolyLineFlag;

typedef struct _DxfLWPolyline DxfLWPolyline;
struct _DxfLWPolyline {
    gint    vertexNum;      /*90 Number of vertices*/
    gint16  flag;           /*70 Polyline flag (bit-coded); default is 0*/
    gdouble width;          /*43 Constant width (optional; default = 0)*/
    gdouble elevation;      /*38 Elevation (optional; default = 0)*/
    gdouble thickness;      /*39 Thickness (optional; default = 0)*/
    DxfPointData *vertexes; /*10,20 Vertex coordinates (in OCS), multiple entries*/
    gint    vertexId;       /*91 Vertex identifier*/
    gdouble startingWidth;  /*40 Starting width (multiple entries; one entry for each vertex) (optional; default = 0; multiple entries). Not used if constant width (code 43) is set */
    gdouble endWidth;       /*41 End width (multiple entries; one entry for each vertex) (optional; default = 0; multiple entries). Not used if constant width (code 43) is set*/
    gdouble bulge;          /*42 Bulge (multiple entries; one entry for each vertex) (optional; default = 0) */
    DxfPointData extrDir;   /*210,220,230 Extrusion direction (optional; default = 0, 0, 1)*/
};

extern DxfLWPolyline *dxf_lwpolyline_parse(Dxfile *f);
extern void dxf_lwpolyline_detele(DxfLWPolyline *l);
extern gchar *dxf_lwpolyline_print(const DxfLWPolyline *l);

#endif /* End of __ENTITIES_LWPOLYLINE_H__ */

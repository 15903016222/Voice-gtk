#ifndef __ENTITIES_IMAGE_H__
#define __ENTITIES_IMAGE_H__

#include "dxfile.h"

typedef struct _DxfImage DxfImage;
struct _DxfImage {
    gint32 classVersion;            /*90 Class version*/
    DxfPointData insertionPoint;    /*10,20,30 Insertion point (in WCS)*/
    DxfPointData uVectorPoint;      /*11,21,31 U-vector (in WCS)*/
    DxfPointData vVectorPoint;      /*12,22,32 V-vector (in WCS)*/
    gdouble usize;                  /*13 U value of Image size in pixels*/
    gdouble vSize;                  /*23 V value of image size in pixels*/
    gchar *hdRef2Imgdef;            /*340 Hard reference to imagedef object*/
    gint16 displayProperties;       /*70 Image display properties*/
    gint16 clippingState;           /*280 Clipping state: 0 = Off; 1 = On*/
    gint16 brightness;              /*281 Brightness value (0-100; default = 50)*/
    gint16 contrast;                /*282 Contrast value (0-100; default = 50)*/
    gint16 fade;                    /*283 Fade value (0-100; default = 0)*/
    gchar *hdRef2ImgdefReactor;     /*360 Hard reference to imagedef_reactor object*/
    gint16 clipBoundaryType;        /*71 Clipping boundary type. 1 = Rectangular; 2 = Polygonal*/
    gint32 vertexNum;               /*91 Number of clip boundary vertices that follow*/
    DxfPointData *vertexes;         /*14,24 Clip boundary vertex (in OCS)*/
    gint16 clipMode;                /*290 Clip Mode: 0 = Outside Mode; 1 = Inside Mode */
};

extern DxfImage *dxf_image_parse(Dxfile *f);
extern void dxf_image_delete(DxfImage *e);
extern gchar *dxf_image_print(DxfImage *e);

#endif /* End of __ENTITIES_IMAGE_H__ */

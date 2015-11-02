#ifndef __DXF_TABLES_VPORT_H__
#define __DXF_TABLES_VPORT_H__

#include "dxfile.h"

typedef struct _DxfTablesVport DxfTablesVport;
struct _DxfTablesVport {
        gchar *name;                    /*2:Viewport name*/
        gint16 standardFlag;            /*70:Standard flag values*/
        DxfPointData lowerLeftCorner;   /*10,20:Lower-left corner of viewport*/
        DxfPointData upperRightCorner;  /*11,21:Upper-right corner of viewport*/
        DxfPointData viewCenter;        /*12,22:View Center Point*/
        DxfPointData snapBase;          /*13,23:snap base point*/
        DxfPointData snapSpacing;       /*14,24:Snap spacing X and Y*/
        DxfPointData gridSpacing;       /*15,25:Grid spacing X and Y*/
        DxfPointData direction;         /*16,26,36:View direction from target
                                          point*/
        DxfPointData target;            /*17,27,37:View target point*/
        gdouble aspectRatio;            /*41:Viewport aspect ratio*/
        gdouble lensLenght;             /*42:Lens length*/
        gdouble frontPlaneOffset;       /*43:Front clipping plane (offset from
                                          target point) */
        gdouble backPlaneOffset;        /*44:Back clipping plane (offset from
                                          target point) */
        gdouble viewHeight;             /*40or45:View height*/
        gdouble snapRotationAngle;      /*50:Snap rotation angle*/
        gdouble viewTwistAngle;         /*51:View twist angle*/
        gint16 viewMode;                /*71:View Mode*/
        gint16 circleSides;             /*72:Circle sides*/
        gint16 fastZoomSetting;         /*73:Fast zoom setting*/
//        gint16 ucsiconSetting;          /*74:UCSICON setting*/
//        gint16 snapOn;                  /*75:Snap on/off*/
//        gint16 gridOn;                  /*76:Grid on/off*/
//        gint16 snap_style;              /*77:Snap style*/
//        gint16 snapIsopair;             /*78:Snap isopair*/

        gchar *frozenLayerSoftPointer;  /*331:Soft-pointer ID/handle to frozen
                                          layer objects*/
        gchar *frozenLayerHardPointer;  /*441:hard-pointer ID/handle to frozen
                                          layer objects*/
};

extern DxfTablesVport *dxf_tables_vport_parse(Dxfile *f);
extern void dxf_tables_vport_delete(DxfTablesVport *e);
extern gchar *dxf_tables_vport_print(DxfTablesVport *e);

#endif /* End of __DXF_TABLES_VPORT_H__ */

/**
 * @file dxf_tables_ltype.h
 * @brief dxf tables层, ltype封装
 * @author JakeYang <add358@gmail.com>
 * @version 0.2
 * @date 2015-04-14
 */

#ifndef __DXF_TABLES_LTYPE_H__
#define __DXF_TABLES_LTYPE_H__

#include "dxfile.h"

/* Group Code ID */

typedef struct _DxfTablesLtype DxfTablesLtype;
struct _DxfTablesLtype {
    gchar *name;            /*2:Linetype name*/
    gchar *description;     /*3:Descriptive text for linetype*/
    gchar *text;            /*9:Text string (one per element if code 74 = 2)*/
    gdouble totalPattenLength;/*40:total_pattern_length*/
    gdouble xOffset;        /*44:X offset value*/
    gdouble yOffset;        /*45:Y offset value*/
    gdouble dashLength;     /*49:Dash, dot or space length*/
    gdouble scale;          /*46:Scale value (optional);
                                multiple entries can exist */
    gdouble rotation;       /*50:R = (relative) or A = (absolute) rotation value
                                in radians of embedded shape or text*/
    gint16 flag;            /*70:Standard flag values*/
    gint16 alignment;       /*72:Alignment code*/
    gint16 elementsNum;     /*73:The number of linetype elements*/
    gint16 complexElement;  /*74:Complex linetype element type (one per element)*/
    gint16 shapeNum;        /*75:Shape number (one per element)*/
    gchar *stylePointer;    /*340:Pointer to STYLE object*/
};

extern DxfTablesLtype *dxf_tables_ltype_parse(Dxfile *f);
extern void dxf_tables_ltype_delete(DxfTablesLtype *l);
extern gchar *dxf_tables_ltype_print(DxfTablesLtype *l);

#endif /* End of __DXF_TABLES_LTYPE_H__ */

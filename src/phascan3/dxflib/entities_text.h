#ifndef __ENTITIES_TEXT_H__
#define __ENTITIES_TEXT_H__

#include "dxfile.h"

typedef struct _DxfText DxfText;
struct _DxfText {
    gdouble thinckness;             /*39 Thickness (optional; default = 0)*/
    DxfPointData fstAlignPoint;     /*10,20,30 First alignment point (in OCS)*/
    gdouble height;                 /*40 Text height*/
    gchar *defaultValue;            /*1 default value(The string itself)*/
    gdouble rotation;               /*50 Text rotation(optional: default = 0)*/
    gdouble width;                  /*41 Relative X scale factor—width (optional; default = 1)*/
    gdouble obliqueAngle;           /*51 Oblique angle (optional; default = 0)*/
    gchar *style;                   /*7 Text style name (optional, default = STANDARD)*/
    gint16 flag;                    /*71 Text generation flags (optional, default = 0)*/
    gint16 htype;                   /*72 Horizontal text justification type (optional, default = 0) integer codes (not bit-coded)*/
    DxfPointData sndAlignPoint;     /*11,21,31 Second alignment point (in OCS) (optional)*/
    DxfPointData extrDir;           /*210,220,230 Extrusion direction (optional; default = 0, 0, 1)*/
    gint16 vtype;                   /*73 Vertical text justification type (optional, default = 0): integer codes (not bit-coded)*/
};

extern DxfText *dxf_text_parse(Dxfile *f);
extern void dxf_text_delete(DxfText *t);
extern gchar *dxf_text_print(DxfText *t);

#endif /* End of __ENTITIES_TEXT_H__ */

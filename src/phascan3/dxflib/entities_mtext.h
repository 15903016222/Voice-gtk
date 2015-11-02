#ifndef __ENTITIES_MTEXT_H__
#define __ENTITIES_MTEXT_H__

#include "dxfile.h"

/* Group Code ID */
#define MTEXT_CODE_H_WIDTH          42  /*Horizontal width of the characters
                                            that make up the mtext entity. This
                                            value will always be equal to or
                                            less than the value of group code 41
                                            (read-only, ignored if supplied)*/
#define MTEXT_CODE_V_HEIGHT         43  /*Vertical height of the mtext entity
                                            (read-only, ignored if supplied)*/
#define MTEXT_CODE_ROTATION         50  /*Rotation angle in radians*/
#define MTEXT_CODE_SPACING_STYLE    73  /*Mtext line spacing style (optional):
                                            1 = At least (taller characters will
                                                override)
                                            2 = Exact (taller characters will
                                                not override)*/
#define MTEXT_CODE_SPACING_FACTOR   44  /*Mtext line spacing factor (optional):
                                            Percentage of default (3-on-5) line
                                            spacing to be applied. Valid values
                                            range from 0.25 to 4.00*/
#define MTEXT_CODE_BG_FILL_SETTING  90  /*Background fill setting:
                                            0 = Background fill off
                                            1 = Use background fill color
                                            2 = Use drawing window color as
                                                background fill color*/
#define MTEXT_CODE_BG_COLOR_RGB0    420 /*Background color (if RGB color)*/
#define MTEXT_CODE_BG_COLOR_RGB1    421
#define MTEXT_CODE_BG_COLOR_RGB2    422
#define MTEXT_CODE_BG_COLOR_RGB3    423
#define MTEXT_CODE_BG_COLOR_RGB4    424
#define MTEXT_CODE_BG_COLOR_RGB5    425
#define MTEXT_CODE_BG_COLOR_RGB6    426
#define MTEXT_CODE_BG_COLOR_RGB7    427
#define MTEXT_CODE_BG_COLOR_RGB8    428
#define MTEXT_CODE_BG_COLOR_RGB9    249
#define MTEXT_CODE_BG_COLOR_NAME0   430 /*Background color (if color name)*/
#define MTEXT_CODE_BG_COLOR_NAME1   431
#define MTEXT_CODE_BG_COLOR_NAME2   432
#define MTEXT_CODE_BG_COLOR_NAME3   433
#define MTEXT_CODE_BG_COLOR_NAME4   434
#define MTEXT_CODE_BG_COLOR_NAME5   435
#define MTEXT_CODE_BG_COLOR_NAME6   436
#define MTEXT_CODE_BG_COLOR_NAME7   437
#define MTEXT_CODE_BG_COLOR_NAME8   438
#define MTEXT_CODE_BG_COLOR_NAME9   439
#define MTEXT_CODE_FILL_BOX_SCALE   45  /*Fill box scale (optional):
                                            Determines how much border there is
                                            around the text.*/
#define MTEXT_CODE_BG_FILL_COLOR    63  /*Background fill color (optional):
                                            Color to use for background fill
                                            when group code 90 is 1.*/
#define MTEXT_CODE_TRANSPARENCY     441 /*Transparency of background fill color
                                            (not implemented)*/
#define MTEXT_CODE_COLUMN_TYPE      75  /*Column type*/
#define MTEXT_CODE_COLUMN_COUNT     76  /*Column count*/
#define MTEXT_CODE_COLUMN_FLOW      78  /*Column Flow Reversed*/
#define MTEXT_CODE_COLUMN_AUTOHEIGHT    79  /*Column Autoheight*/
#define MTEXT_CODE_COLUMN_WIDTH     48  /*Column width*/
#define MTEXT_CODE_COLUMN_GUTTER    49  /*Column gutter*/
#define MTEXT_CODE_COLUMN_HEIGHTS   50  /*Column heights; this code is followed
                                            by a column count (Int16), and then
                                            the number of column heights*/

typedef struct _DxfMtext DxfMtext;

struct _DxfMtext {
    DxfPointData insertionPoint;    /*10,20,30 Insertion point*/
    gdouble nominalHeight;          /*40 Nominal (initial) text height*/
    gdouble rectangleWidth;         /*41 Reference rectangle width*/
    gint16 attachment;              /*71 Attachment point*/
    gint16 direction;               /*72 Drawing direction*/
    GString *str;                   /*1,3 Text string*/
    gchar *style;                   /*7 Text style name*/
    DxfPointData extrDir;           /*210,220,230 Extrusion direction (optional; default = 0, 0, 1) */
    DxfPointData xAxisDir;          /*11,21,31 X-axis direction vector (in WCS)*/
    gdouble rotation;
    gdouble spacingFactor;
    gint16 spacingStyle;
};

extern DxfMtext *dxf_mtext_parse(Dxfile *f);
extern void dxf_mtext_delete(DxfMtext *e);
extern gchar *dxf_mtext_print(DxfMtext *e);

#endif /* End of __ENTITIES_MTEXT_H__ */

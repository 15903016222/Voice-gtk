/**
 * @file entities_mtext.c
 * @brief dxf entities层, mtext对象
 * @author JakeYang <add358@gmail.com>
 * @version 0.2
 * @date 2015-04-14
 */

#include "entities_mtext.h"

static inline DxfMtext *mtext_new_item()
{
    DxfMtext *item = g_malloc0(sizeof(DxfMtext));
    item->str = g_string_new(NULL);
    item->extrDir.z = 1;
    return item;
}

DxfMtext *dxf_mtext_parse(Dxfile *f)
{
    DxfMtext *e = NULL;
    gchar *lineData = NULL;

    e = mtext_new_item();

    SWITCH_CODE_BEGIN(f);
    SWITCH_CODE_DO( 10 )
            dxfile_get_double(f, &e->insertionPoint.x);
    SWITCH_CODE_DO( 20 )
            dxfile_get_double(f, &e->insertionPoint.y);
    SWITCH_CODE_DO( 30 )
            dxfile_get_double(f, &e->insertionPoint.z);
    SWITCH_CODE_DO( 40 )
            dxfile_get_double(f, &e->nominalHeight);
    SWITCH_CODE_DO( 41 )
            dxfile_get_double(f, &e->rectangleWidth);
    SWITCH_CODE_DO( 71 )
            dxfile_get_int16(f, &e->attachment);
    SWITCH_CODE_DO( 72 )
            dxfile_get_int16(f, &e->direction);
    SWITCH_CODE_DO( 1 )
    SWITCH_CODE_DO_FOLLOW( 3) {
        lineData = NULL;
        dxfile_get_line(f, &lineData, NULL);
        g_string_append(e->str, lineData);
        g_free(lineData);
    }
    SWITCH_CODE_DO( 7)
            dxfile_get_line(f, &e->style, NULL);
    SWITCH_CODE_DO( 210)
            dxfile_get_double(f, &e->extrDir.x);
    SWITCH_CODE_DO( 220)
            dxfile_get_double(f, &e->extrDir.y);
    SWITCH_CODE_DO( 230)
            dxfile_get_double(f, &e->extrDir.z);
    SWITCH_CODE_DO( 50)
            dxfile_get_double(f, &e->rotation);
    SWITCH_CODE_DO( 73)
            dxfile_get_int16(f, &e->spacingStyle);
    SWITCH_CODE_DO( 44)
            dxfile_get_double(f, &e->spacingFactor);
    SWITCH_CODE_END(f);
    return e;
}

void dxf_mtext_delete(DxfMtext *e)
{
    g_return_if_fail( e != NULL );

    g_string_free(e->str, TRUE);
    g_free(e->style);
    g_free(e);
}

gchar *dxf_mtext_print(DxfMtext *e)
{
    g_return_val_if_fail( e != NULL, NULL );

    return g_strdup_printf("Insert Point        : (%g, %g, %g)\n"
                           "Extrusion Direction : (%g, %g, %g)\n"
                           "Text Height         : %g\n"
                           "Rectangle Width     : %g\n"
                           "Attachment Point    : %d\n"
                           "Drawing Direction   : %d\n"
                           "Text Style Name     : %s\n"
                           "Rotation Angle      : %g\n"
                           "Spacing Style       : %d\n"
                           "Spacing Factor      : %g\n"
                           "TEXT                : %s\n",
                           e->insertionPoint.x, e->insertionPoint.y, e->insertionPoint.z,
                           e->extrDir.x, e->extrDir.y, e->extrDir.z,
                           e->nominalHeight,
                           e->rectangleWidth,
                           e->attachment,
                           e->direction,
                           e->style,
                           e->rotation,
                           e->spacingStyle, e->spacingFactor,
                           e->str->str);
}

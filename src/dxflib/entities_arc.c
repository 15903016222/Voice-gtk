/**
 * @file entities_arc.c
 * @brief dxf entities层, arc对象
 * @author JakeYang <add358@gmail.com>
 * @version 0.2
 * @date 2015-04-14
 */

#include "entities_arc.h"

static inline DxfArc *arc_new_item()
{
    DxfArc *e = g_malloc0(sizeof(DxfArc));
    e->extrDir.z = 1;
    return e;
}

DxfArc *dxf_arc_parse(Dxfile *f)
{
    DxfArc *e = NULL;

    e = arc_new_item();

    SWITCH_CODE_BEGIN(f);
    SWITCH_CODE_DO( 10 )
            dxfile_get_double(f, &e->centerPoint.x);
    SWITCH_CODE_DO( 20 )
            dxfile_get_double(f, &e->centerPoint.y);
    SWITCH_CODE_DO( 30 )
            dxfile_get_double(f, &e->centerPoint.z);
    SWITCH_CODE_DO( 39 )
            dxfile_get_double(f, &e->thickness);
    SWITCH_CODE_DO( 40 )
            dxfile_get_double(f, &e->radius);
    SWITCH_CODE_DO( 50 )
            dxfile_get_double(f, &e->startAngle);
    SWITCH_CODE_DO( 51 )
            dxfile_get_double(f, &e->endAngle);
    SWITCH_CODE_DO( 210 )
            dxfile_get_double(f, &e->extrDir.x);
    SWITCH_CODE_DO( 220 )
            dxfile_get_double(f, &e->extrDir.y);
    SWITCH_CODE_DO( 230 )
            dxfile_get_double(f, &e->extrDir.z);
    SWITCH_CODE_END(f);

    return e;
}

void dxf_arc_delete(DxfArc *e)
{
    g_return_if_fail( e != NULL );
    g_free(e);
}

gchar *dxf_arc_print(DxfArc *e)
{
    g_return_val_if_fail( e != NULL, NULL );
    return g_strdup_printf("Center Point : (%g, %g, %g)\n"
                           "Extr Point   : (%g, %g, %g)\n"
                           "Thickness    : %g\n"
                           "Radius       : %g\n"
                           "Start Angle  : %g\n"
                           "End Angle    : %g\n",
                           e->centerPoint.x, e->centerPoint.y, e->centerPoint.z,
                           e->extrDir.x, e->extrDir.y, e->extrDir.z,
                           e->thickness,
                           e->radius,
                           e->startAngle,
                           e->endAngle);
}

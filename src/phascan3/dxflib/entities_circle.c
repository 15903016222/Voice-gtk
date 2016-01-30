/**
 * @file entities_circle.c
 * @brief dxf entities层, circle对象
 * @author JakeYang <add358@gmail.com>
 * @version 0.2
 * @date 2015-04-14
 */

#include "entities_circle.h"

static inline DxfCircle *circle_new_item()
{
    DxfCircle *c = g_malloc0(sizeof(DxfCircle));
    c->extrDir.z = 1;
    return c;
}

DxfCircle *dxf_circle_parse(Dxfile *f)
{
    DxfCircle *c = NULL;

    c = circle_new_item();

    SWITCH_CODE_BEGIN(f);
    SWITCH_CODE_DO( 10 )
            dxfile_get_double(f, &c->centerPoint.x);
    SWITCH_CODE_DO( 20 )
            dxfile_get_double(f, &c->centerPoint.y);
    SWITCH_CODE_DO( 30 )
            dxfile_get_double(f, &c->centerPoint.z);
    SWITCH_CODE_DO( 39 )
            dxfile_get_double(f, &c->thickness);
    SWITCH_CODE_DO( 40 )
            dxfile_get_double(f, &c->radius);
    SWITCH_CODE_DO( 210 )
            dxfile_get_double(f, &c->extrDir.x);
    SWITCH_CODE_DO( 220 )
            dxfile_get_double(f, &c->extrDir.y);
    SWITCH_CODE_DO( 230 )
            dxfile_get_double(f, &c->extrDir.z);
    SWITCH_CODE_END(f);

    return c;
}


void dxf_circle_delete(DxfCircle *c)
{
    g_free(c);
}

gchar *dxf_circle_print(const DxfCircle *c)
{
    return g_strdup_printf("Center Point       : (%g, %g, %g)\n"
                           "Extrusion Direction: (%g, %g, %g)\n"
                           "radius             : %g\n",
                           c->centerPoint.x, c->centerPoint.y, c->centerPoint.z,
                           c->extrDir.x, c->extrDir.y, c->extrDir.z,
                           c->radius);
}

#include "entities_point.h"

static inline DxfPoint *point_new_item()
{
    DxfPoint *p = g_malloc0(sizeof(DxfPoint));
    p->extrDir.z = 1;
    return p;
}

DxfPoint *dxf_point_parse(Dxfile *f)
{
    DxfPoint *p;

    p = point_new_item();

    SWITCH_CODE_BEGIN(f);
    SWITCH_CODE_DO( 10 )
            dxfile_get_double(f, &p->data.x);
    SWITCH_CODE_DO( 20 )
            dxfile_get_double(f, &p->data.y);
    SWITCH_CODE_DO( 30 )
            dxfile_get_double(f, &p->data.z);
    SWITCH_CODE_DO( 39 )
            dxfile_get_double(f, &p->thickness);
    SWITCH_CODE_DO( 210 )
            dxfile_get_double(f, &p->extrDir.x);
    SWITCH_CODE_DO( 220 )
            dxfile_get_double(f, &p->extrDir.y);
    SWITCH_CODE_DO( 230 )
            dxfile_get_double(f, &p->extrDir.z);
    SWITCH_CODE_END(f);

    return p;
}


void dxf_point_delete(DxfPoint *p)
{
    g_return_if_fail(p != NULL);
    g_free(p);
}


gchar *dxf_point_print(const DxfPoint *p)
{
    return g_strdup_printf("Point : (%g, %g)\n",p->data.x,p->data.y);
}

#include "entities_line.h"

static inline DxfLine *line_new_item()
{
    DxfLine *l = g_malloc0(sizeof(DxfLine));
    l->extrDir.z = 1;
    return l;
}

DxfLine *dxf_line_parse(Dxfile *f)
{
    DxfLine *l;

    l = line_new_item();

    SWITCH_CODE_BEGIN(f);
    SWITCH_CODE_DO( 39 )
            dxfile_get_double(f, &l->thickness);
    SWITCH_CODE_DO( 10 )
            dxfile_get_double(f, &l->startPoint.x);
    SWITCH_CODE_DO( 20 )
            dxfile_get_double(f, &l->startPoint.y);
    SWITCH_CODE_DO( 30 )
            dxfile_get_double(f, &l->startPoint.z);
    SWITCH_CODE_DO( 11 )
            dxfile_get_double(f, &l->endPoint.x);
    SWITCH_CODE_DO( 21 )
            dxfile_get_double(f, &l->endPoint.y);
    SWITCH_CODE_DO( 31 )
            dxfile_get_double(f, &l->endPoint.z);
    SWITCH_CODE_DO( 210 )
            dxfile_get_double(f, &l->extrDir.x);
    SWITCH_CODE_DO( 220 )
            dxfile_get_double(f, &l->extrDir.y);
    SWITCH_CODE_DO( 230 )
            dxfile_get_double(f, &l->extrDir.z);
    SWITCH_CODE_END(f);

    return l;
}

gchar *dxf_line_print(const DxfLine *l)
{
    return g_strdup_printf("thickness     : %g\n"
                           "Start Point   : (%g, %g, %g)\n"
                           "End Point     : (%g, %g, %g)\n",
                           l->thickness,
                           l->startPoint.x, l->startPoint.y, l->startPoint.z,
                           l->endPoint.x, l->endPoint.y, l->endPoint.z);
}


void dxf_line_delete(DxfLine *l)
{
     g_free(l);
}

#include "entities_polyline.h"

static inline DxfPolyline *polyline_new_item()
{
    return g_malloc0(sizeof(DxfPolyline));
}

DxfPolyline *dxf_polyline_parse(Dxfile *f)
{
    DxfPolyline *e = NULL;

    e = polyline_new_item();
    f = f;
//    DXFILE_SWITCH_BEGIN(f);
//    DXFILE_SWITCH_DO(  )
//            ;
//    DXFILE_SWITCH_END(f);

    return e;
}

void dxf_polyline_delete(DxfPolyline *e)
{
    g_return_if_fail( e != NULL );
    g_free(e);
}

gchar *dxf_polyline_print(DxfPolyline *e)
{
    e = e;
    return g_strdup_printf("Polyline\n");
}

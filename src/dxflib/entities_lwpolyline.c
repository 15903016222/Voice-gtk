/**
 * @file entities_lwpolyline.c
 * @brief dxf entities层, lwpolyline对象
 * @author JakeYang <add358@gmail.com>
 * @version 0.2
 * @date 2015-04-14
 */

#include "entities_lwpolyline.h"

static inline DxfLWPolyline *lwpolyline_new_item()
{
    DxfLWPolyline *l = g_malloc0(sizeof(DxfLWPolyline));
    l->extrDir.z = 1;
    return l;
}

DxfLWPolyline *dxf_lwpolyline_parse(Dxfile *f)
{
    DxfLWPolyline *l = NULL;
    DxfPointData *vertex = NULL;

    l = lwpolyline_new_item();

    SWITCH_CODE_BEGIN(f);
    SWITCH_CODE_DO( 90 )
            dxfile_get_int(f, &l->vertexNum);
    SWITCH_CODE_DO( 70 )
            dxfile_get_int16(f, &l->flag);
    SWITCH_CODE_DO( 43 )
            dxfile_get_double(f, &l->width);
    SWITCH_CODE_DO( 38 )
            dxfile_get_double(f, &l->elevation);
    SWITCH_CODE_DO( 39 )
            dxfile_get_double(f, &l->thickness);
    SWITCH_CODE_DO( 10 ) {
        if ( NULL == vertex) {
            if (l->vertexNum > 0) {
                vertex = g_malloc0(sizeof(DxfPointData)*l->vertexNum);
                l->vertexes = vertex;
                dxfile_get_double(f, &vertex->x);
            } else {
                c_log_warning("Haven't got the number of vertexes");
            }
        } else if ((gint16)((vertex-l->vertexes)/sizeof(DxfPointData)) < l->vertexNum){
            ++vertex;
            dxfile_get_double(f, &vertex->x);
        } else {
            c_log_warning("The array of vertexes is overflow");
            dxfile_goto_nextline(f);
        }
    }
    SWITCH_CODE_DO( 20 )
            dxfile_get_double(f, &vertex->y);
    SWITCH_CODE_DO( 91 )
            dxfile_get_int(f, &l->vertexId);
    SWITCH_CODE_DO( 40 )
            dxfile_get_double(f, &l->startingWidth);
    SWITCH_CODE_DO( 41 )
            dxfile_get_double(f, &l->endWidth);
    SWITCH_CODE_DO( 42 )
            dxfile_get_double(f, &l->bulge);
    SWITCH_CODE_DO( 210 )
            dxfile_get_double(f, &l->extrDir.x);
    SWITCH_CODE_DO( 220 )
            dxfile_get_double(f, &l->extrDir.y);
    SWITCH_CODE_DO( 230 )
            dxfile_get_double(f, &l->extrDir.z);
    SWITCH_CODE_END(f);

    return l;
}


void dxf_lwpolyline_detele(DxfLWPolyline *l)
{
    g_return_if_fail( l != NULL);

    g_free(l->vertexes);
    g_free(l);
}


gchar *dxf_lwpolyline_print(const DxfLWPolyline *l)
{
    GString *msg = NULL;
    gint i=0;

    g_return_val_if_fail( l != NULL, NULL );
    g_return_val_if_fail( l->vertexes != NULL, NULL);

    msg = g_string_sized_new(1024);

    g_string_append_printf(msg,
                           "Number of vertices : %d\n"
                           "Polyline flag      : %d\n"
                           "Constant Width     : %g\n"
                           "Vertexes :\n",
                           l->vertexNum,
                           l->flag,
                           l->width);

    for (i = 0; i < l->vertexNum; ++i) {
        g_string_append_printf(msg, " [%03d] : (%g, %g)\n", i+1, l->vertexes[i].x, l->vertexes[i].y);
    }

    return g_string_free(msg, FALSE);
}

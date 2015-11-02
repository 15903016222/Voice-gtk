#include "entities_image.h"

static inline DxfImage *image_new_item()
{
    DxfImage *image = g_malloc0(sizeof(DxfImage));
    image->brightness = 50;
    image->contrast = 50;
    return image;
}

DxfImage *dxf_image_parse(Dxfile *f)
{
    DxfImage *e = NULL;
    DxfPointData *vertex = NULL;

    e = image_new_item();

    SWITCH_CODE_BEGIN(f);
    SWITCH_CODE_DO( 10 )
            dxfile_get_double(f, &e->insertionPoint.x);
    SWITCH_CODE_DO( 20 )
            dxfile_get_double(f, &e->insertionPoint.y);
    SWITCH_CODE_DO( 30 )
            dxfile_get_double(f, &e->insertionPoint.z);
    SWITCH_CODE_DO( 12 )
            dxfile_get_double(f, &e->vVectorPoint.x);
    SWITCH_CODE_DO( 22 )
            dxfile_get_double(f, &e->vVectorPoint.y);
    SWITCH_CODE_DO( 32 )
            dxfile_get_double(f, &e->vVectorPoint.z);
    SWITCH_CODE_DO( 11 )
            dxfile_get_double(f, &e->uVectorPoint.x);
    SWITCH_CODE_DO( 21 )
            dxfile_get_double(f, &e->uVectorPoint.y);
    SWITCH_CODE_DO( 31 )
            dxfile_get_double(f, &e->uVectorPoint.z);
    SWITCH_CODE_DO( 13 )
            dxfile_get_double(f, &e->usize);
    SWITCH_CODE_DO( 23 )
            dxfile_get_double(f, &e->vSize);
    SWITCH_CODE_DO( 340 )
            dxfile_get_line(f, &e->hdRef2Imgdef, NULL);
    SWITCH_CODE_DO( 70 )
            dxfile_get_int16(f, &e->displayProperties);
    SWITCH_CODE_DO( 280 )
            dxfile_get_int16(f, &e->clippingState);
    SWITCH_CODE_DO( 281 )
            dxfile_get_int16(f, &e->brightness);
    SWITCH_CODE_DO( 282 )
            dxfile_get_int16(f, &e->contrast);
    SWITCH_CODE_DO( 283 )
            dxfile_get_int16(f, &e->fade);
    SWITCH_CODE_DO( 360 )
            dxfile_get_line(f, &e->hdRef2ImgdefReactor, NULL);
    SWITCH_CODE_DO( 71 )
            dxfile_get_int16(f, &e->clipBoundaryType);
    SWITCH_CODE_DO( 91 )
            dxfile_get_int(f, &e->vertexNum);
    SWITCH_CODE_DO( 14 ) {
        if (NULL == vertex) {
            if (e->vertexNum > 0) {
                vertex = g_malloc0(sizeof(DxfPointData)*e->vertexNum);
                e->vertexes = vertex;
                dxfile_get_double(f, &vertex->x);
            } else {
                c_log_warning("Haven't got the number of vertexes");
            }
        } else if ( (gint16)((vertex-e->vertexes)/sizeof(DxfPointData)) < e->vertexNum ) {
            ++vertex;
            dxfile_get_double(f, &vertex->x);
        } else {
            c_log_warning("The array of vertexes is overflow");
            dxfile_goto_nextline(f);
        }
    }
    SWITCH_CODE_DO( 24 )
            dxfile_get_double(f, &vertex->y);
    SWITCH_CODE_DO( 290 )
            dxfile_get_int16(f, &e->clipMode);
    SWITCH_CODE_END(f);

    return e;
}

void dxf_image_delete(DxfImage *e)
{
    g_return_if_fail( e != NULL );
    g_free(e->hdRef2Imgdef);
    g_free(e->hdRef2ImgdefReactor);
    g_free(e);
}

gchar *dxf_image_print(DxfImage *e)
{
    return g_strdup_printf("Insert Point   : (%g, %g, %g)\n"
                           "U Vector Point : (%g, %g, %g)\n"
                           "V Vector Point : (%g, %g, %g)\n"
                           "Image Size     : %g\n"
                           "Image V Size   : %g\n"
                           "Display Prop   : %hd\n"
                           "Brightness     : %hd\n"
                           "Contrast       : %hd\n"
                           "Fade           : %hd\n"
                           "Hard Ref to Imagedef         : %s\n"
                           "Hard Ref to Imagedef Reactor : %s\n",
                           e->insertionPoint.x, e->insertionPoint.y, e->insertionPoint.z,
                           e->uVectorPoint.x, e->uVectorPoint.y, e->uVectorPoint.z,
                           e->vVectorPoint.x, e->vVectorPoint.y, e->vVectorPoint.z,
                           e->usize,
                           e->vSize,
                           e->displayProperties,
                           e->brightness,
                           e->contrast,
                           e->fade,
                           e->hdRef2Imgdef,
                           e->hdRef2ImgdefReactor);
}

#include "entities_text.h"

static inline DxfText *text_new_item()
{
    DxfText *t = g_malloc0(sizeof(DxfText));
    t->width = 1;
    t->extrDir.z = 1;
    return t;
}

DxfText *dxf_text_parse(Dxfile *f)
{
    DxfText *e = NULL;

    e = text_new_item();

    SWITCH_CODE_BEGIN(f);
    SWITCH_CODE_DO( 39 )
            dxfile_get_double(f, &e->thinckness);
    SWITCH_CODE_DO( 10 )
            dxfile_get_double(f, &e->fstAlignPoint.x);
    SWITCH_CODE_DO( 20 )
            dxfile_get_double(f, &e->fstAlignPoint.y);
    SWITCH_CODE_DO( 30 )
            dxfile_get_double(f, &e->fstAlignPoint.z);
    SWITCH_CODE_DO( 40 )
            dxfile_get_double(f, &e->height);
    SWITCH_CODE_DO( 1 )
            dxfile_get_line(f, &e->defaultValue, NULL);
    SWITCH_CODE_DO( 50 )
            dxfile_get_double(f, &e->rotation);
    SWITCH_CODE_DO( 51 )
            dxfile_get_double(f, &e->obliqueAngle);
    SWITCH_CODE_DO( 7 )
            dxfile_get_line(f, &e->style, NULL);
    SWITCH_CODE_DO( 71 )
            dxfile_get_int16(f, &e->flag);
    SWITCH_CODE_DO( 72 )
            dxfile_get_int16(f, &e->htype);
    SWITCH_CODE_DO( 11 )
            dxfile_get_double(f, &e->sndAlignPoint.x);
    SWITCH_CODE_DO( 21 )
            dxfile_get_double(f, &e->sndAlignPoint.y);
    SWITCH_CODE_DO( 31 )
            dxfile_get_double(f, &e->sndAlignPoint.z);
    SWITCH_CODE_DO( 210 )
            dxfile_get_double(f, &e->extrDir.x);
    SWITCH_CODE_DO( 220 )
            dxfile_get_double(f,&e->extrDir.y);
    SWITCH_CODE_DO( 230 )
            dxfile_get_double(f, &e->extrDir.z);
    SWITCH_CODE_DO( 73 )
            dxfile_get_int16(f, &e->vtype);
    SWITCH_CODE_END(f);

    return e;
}

void dxf_text_delete(DxfText *e)
{
    g_return_if_fail( e != NULL );
    g_free(e->defaultValue);
    g_free(e->style);
    g_free(e);
}

gchar *dxf_text_print(DxfText *e)
{
    return g_strdup_printf("Thickness               : %g\n"
                           "Text Height             : %g\n"
                           "Default Value           : %s\n"
                           "Text rotation           : %g\n"
                           "Relative X scale factor : %g\n"
                           "Oblique angle           : %g\n"
                           "Text style name         : %s\n"
                           "Text generation flags   : %d\n"
                           "H-T justification type  : %d\n"
                           "V-T justification type  : %d\n"
                           "First Alignment Point   : (%g, %g, %g)\n"
                           "Second Alignment Point  : (%g, %g, %g)\n"
                           "Extrusion direction     : (%g, %g, %g)\n",
                           e->thinckness,
                           e->height,
                           e->defaultValue,
                           e->rotation,
                           e->width,
                           e->obliqueAngle,
                           e->style,
                           e->flag,
                           e->htype,
                           e->vtype,
                           e->fstAlignPoint.x, e->fstAlignPoint.y, e->fstAlignPoint.z,
                           e->sndAlignPoint.x, e->sndAlignPoint.y, e->sndAlignPoint.z,
                           e->extrDir.x, e->extrDir.y, e->extrDir.z);
}

#include "entities_3dsolid.h"

static inline Dxf3Dsolid *dxf3dsolid_new_item()
{
    return g_malloc0(sizeof(Dxf3Dsolid));
}

Dxf3Dsolid *dxf_3dsolid_parse(Dxfile *f)
{
    Dxf3Dsolid *e = NULL;
    gchar *data = NULL;
    DxfPropData *propData = NULL;

    e = dxf3dsolid_new_item();

    SWITCH_CODE_BEGIN(f);
    SWITCH_CODE_DO( DXF_3DSOLID_CODE_VERSION )
            dxfile_get_int16(f, &e->version);
    SWITCH_CODE_DO( DXF_3DSOLID_CODE_PROPRIETARY_DATA )
    SWITCH_CODE_DO_FOLLOW( DXF_3DSOLID_CODE_ADDITIONAL_PROPRIETARY_DATA ) {
        data = NULL;
        dxfile_get_line(f, &data, NULL);
        if (NULL == propData) {
            propData = g_malloc0(sizeof(DxfPropData));
            e->proprietaryData = propData;
        } else {
            propData->next = g_malloc0(sizeof(DxfPointData));
            propData = propData->next;
        }
        propData->data = data;
    }
    SWITCH_CODE_END(f);

    return e;
}

void dxf_3dsolid_delete(Dxf3Dsolid *e)
{
    DxfPropData *propData = NULL;

    g_return_if_fail( e != NULL );

    while(e->proprietaryData) {
        propData = e->proprietaryData->next;
        g_free(e->proprietaryData->data);
        g_free(e->proprietaryData);
        e->proprietaryData = propData;
    }

    g_free(e);
}

gchar *dxf_3dsolid_print(Dxf3Dsolid *e)
{
    DxfPropData *d;
    GString *msg = NULL;

    g_return_val_if_fail( e != NULL, NULL );
    g_return_val_if_fail( e->proprietaryData != NULL, NULL);

    msg = g_string_sized_new(1024);
    g_string_append_printf(msg,
                           "Version       : %d\n"
                           "Data          :\n",
                           e->version);

    for (d = e->proprietaryData; d; d = d->next) {
        g_string_append_printf(msg, "%s\n", d->data);
    }

    return g_string_free(msg, FALSE);
}

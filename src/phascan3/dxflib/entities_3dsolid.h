/**
 * @file entities_3dsolid.h
 * @brief dxf entities层, 3dsolid对象
 * @author JakeYang <add358@gmail.com>
 * @version 0.2
 * @date 2015-04-14
 */

#ifndef __ENTITIES_3DSOLID_H__
#define __ENTITIES_3DSOLID_H__

#include "dxfile.h"

/* Group Code ID */
#define DXF_3DSOLID_CODE_VERSION            70  /*Modeler format version number
                                                    (currently = 1)*/
#define DXF_3DSOLID_CODE_PROPRIETARY_DATA   1   /*Proprietary data
                                        (multiple lines < 255 characters each)*/
#define DXF_3DSOLID_CODE_ADDITIONAL_PROPRIETARY_DATA    3 /*Additional lines of
                                                            proprietary data*/
#define DXF_3DSOLID_CODE_HISTORY_OBJECT     350 /*Soft-owner ID/handle to
                                                  history object*/

typedef struct _Dxf3Dsolid Dxf3Dsolid;
typedef struct _DxfProprietaryData DxfPropData;

struct _Dxf3Dsolid {
    gint16  version;
    DxfPropData *proprietaryData;
    DxfPropData *additionalProprietaryData;
};

struct _DxfProprietaryData {
    gchar *data;
    struct _DxfProprietaryData *next;
};

extern Dxf3Dsolid *dxf_3dsolid_parse(Dxfile *f);
extern void dxf_3dsolid_delete(Dxf3Dsolid *e);
extern gchar *dxf_3dsolid_print(Dxf3Dsolid *e);

#endif /* End of __ENTITIES_3DSOLID_H__ */

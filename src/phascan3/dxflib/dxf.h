#ifndef __DXF_H__
#define __DXF_H__

#include "dxf_header.h"
#include "dxf_tables.h"
#include "entities.h"
#include "version.h"

typedef struct _Dxf Dxf;

struct _Dxf {
    DxfHeader *header;
    DxfTables *tables;
    DxfEntities *entities;
};

extern void dxf_init(void);
extern void dxf_uninit(void);
extern Dxf *dxf_new_for_file(const gchar *filename, const DxfSectionFlag flag);
extern void dxf_delete(Dxf *dxf);

#endif /* __DXF_H__ */

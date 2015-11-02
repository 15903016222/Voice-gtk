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

typedef enum {
    DXF_PARSE_NONE      = 0,
    DXF_PARSE_HEADER    = 1<<0,
    DXF_PARSE_CLASSES   = 1<<1,
    DXF_PARSE_TABLES    = 1<<2,
    DXF_PARSE_BLOCKS    = 1<<3,
    DXF_PARSE_ENTITIES  = 1<<4,
    DXF_PARSE_OBJECTS   = 1<<5,
    DXF_PARSE_ALL       = DXF_PARSE_HEADER   | DXF_PARSE_CLASSES
                         |DXF_PARSE_TABLES   | DXF_PARSE_BLOCKS
                         |DXF_PARSE_ENTITIES | DXF_PARSE_OBJECTS
}DxfParseFlag;

extern void dxf_init(void);
extern void dxf_uninit(void);
extern Dxf *dxf_new_for_file(const gchar *filename, const DxfParseFlag flag);
extern void dxf_delete(Dxf *dxf);

#endif /* __DXF_H__ */

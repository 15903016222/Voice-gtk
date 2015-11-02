#ifndef __DXFILE_H__
#define __DXFILE_H__

#include "dxfbase.h"

typedef struct _Dxfile Dxfile;
struct _Dxfile {
    FILE *fp;
};

extern Dxfile *dxfile_open(const gchar *filename);
extern void dxfile_destory(Dxfile *f);

static inline gboolean dxfile_is_end(Dxfile *f) { return !! feof(f->fp); }

static inline void dxfile_goto_nextline(Dxfile *f)
{
    gchar c = -1;
    for (c = fgetc(f->fp); (c != '\n') && (c != -1); c = fgetc(f->fp)) {}
}

static inline gint dxfile_seek(Dxfile *f, glong offset, gint whence) { return fseek(f->fp, offset, whence); }
static inline glong dxfile_get_offset(Dxfile *f) { return ftell(f->fp); }

static inline void dxfile_get_int(Dxfile *f, gint *i) { fscanf(f->fp, "%d", i); dxfile_goto_nextline(f);}
static inline void dxfile_get_int16(Dxfile *f, gint16 *i) { fscanf(f->fp, "%hd", i); dxfile_goto_nextline(f); }
static inline gint dxfile_get_double(Dxfile *f, gdouble *d) { return fscanf(f->fp, "%lf", d); dxfile_goto_nextline(f);}

extern void dxfile_get_line(Dxfile *f, gchar **str, gsize *len);

static inline void dxfile_get_code(Dxfile *f, gint *i) { *i=DXF_CODE_INVALID; return dxfile_get_int(f, i); }
extern void dxfile_get_section(Dxfile *f, Section *s);


/*选择code模板*/
#define SWITCH_CODE_BEGIN(f)   \
{\
    gint code = 0; \
    glong offset = 0; \
    offset = dxfile_get_offset(f); \
    for ( dxfile_get_code(f, &code); \
          code && (code != DXF_CODE_INVALID); ) { \
            switch(code) {
#define SWITCH_CODE_DO(cond)  break; case (cond):
#define SWITCH_CODE_DO_FOLLOW(cond) case (cond):
#define SWITCH_CODE_OUT()  code=0; continue;
#define SWITCH_CODE_OUT_OFFSET(f) offset = dxfile_get_offset(f); SWITCH_CODE_OUT()
#define SWITCH_CODE_SEEK(f)  dxfile_seek(f, offset, SEEK_SET)
#define SWITCH_CODE_END(f)  break;\
        default: \
            c_debug("[%s] invalid code %d", __func__, code); \
            dxfile_goto_nextline(f);\
            break; \
        } \
        offset = dxfile_get_offset(f); \
        dxfile_get_code(f, &code); \
    } \
    if ( code == DXF_CODE_INVALID ) { c_debug("[%s] Can't get Code", __func__); }\
    dxfile_seek(f, offset, SEEK_SET);\
}

#endif /* __DXFILE_H__ */

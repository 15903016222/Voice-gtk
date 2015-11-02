#include "dxfile.h"

static inline Dxfile *dxfile_new_item() { return g_malloc0(sizeof(Dxfile)); }

Dxfile *dxfile_open(const gchar *filename)
{
    Dxfile *dxfile = NULL;
    FILE *fp = NULL;
    g_return_val_if_fail( filename != NULL, NULL);
    fp = fopen(filename, "r");
    if ( NULL == fp) {
        c_log_err("Can't open %s file", filename);
        return NULL;
    }

    dxfile = dxfile_new_item();
    dxfile->fp = fp;

    return dxfile;
}

void dxfile_get_line(Dxfile *f, gchar **str, gsize *len)
{
    g_return_if_fail( f != NULL );
    g_return_if_fail( str != NULL );
    g_return_if_fail( *str == NULL );

    gchar s[STR_MAX] = {0};
    gchar *tmp = s;
    gchar c = -1;

    while( TRUE ) {
        c = fgetc(f->fp);
        if ( (c == '\n') || ( c == -1) ) {
            break;
        } else if ( c == '\r' ) {
            continue;
        }
        *tmp++ = c;
    }
    if (tmp == s) {
        return;
    }
    if (len != NULL) {
        *len = tmp - s;
    }
    *str = g_strdup(s);
}

static gint dxfile_get_line_str(Dxfile *f, gchar *s, gint size)
{
    gchar c = '\0';
    gchar *tmp = s;

    g_return_val_if_fail( f != NULL, -1 );
    g_return_val_if_fail( f->fp != NULL, -1 );
    g_return_val_if_fail( s != NULL, -1);
    g_return_val_if_fail( size >= 0, -1);

    while (size--) {
        c = fgetc(f->fp);
        if ( (c == '\n') || (c == -1) ) {
            break;
        } else if (c == '\r') {
            continue;
        }

        *tmp++ = c;
    }
    *tmp = '\0';
    return tmp-s;
}

void dxfile_get_section(Dxfile *f, Section *s)
{
    dxfile_get_code(f, &s->code);
    if (s->code == DXF_CODE_INVALID) {
        return;
    }
    dxfile_get_line_str(f, s->value, STR_MAX);
}

void dxfile_destory(Dxfile *f)
{
    g_return_if_fail( f != NULL );
    fclose(f->fp);
    g_free(f);
}

#include "dxfile.h"

static inline Dxfile *dxfile_new_item() { return g_malloc0(sizeof(Dxfile)); }
static gint dxfile_encoding(const gchar *filename);

Dxfile *dxfile_open(const gchar *filename)
{
    g_return_val_if_fail( filename != NULL, NULL);

    Dxfile *dxfile = NULL;
    GMappedFile *mappedFile = NULL;
    GError *err = NULL;

    mappedFile = g_mapped_file_new(filename, FALSE, &err);
    if (err) {
        g_error("Unable to read file: %s", err->message);
        g_error_free(err);
        return NULL;
    }

    dxfile = dxfile_new_item();
    dxfile->mmpedFile = mappedFile;
    dxfile->contents = g_mapped_file_get_contents(mappedFile);
    dxfile->cur = dxfile->contents;
    dxfile->len = g_mapped_file_get_length(mappedFile);
    dxfile->enc = dxfile_encoding(filename);

    return dxfile;
}

void dxfile_get_line(Dxfile *f, gchar **str, gsize *len)
{
    g_return_if_fail( f != NULL );
    g_return_if_fail( str != NULL );
    g_return_if_fail( *str == NULL );

    gchar *end = f->cur;

    for (; *end
         && *end != '\n'
         && *end != '\r';
         ++end) {}

    if (len != NULL) {
        *len = end - f->cur;
    }

    if (f->cur == end) {
        *str = g_strdup("");
    } else {
        *str = g_strndup(f->cur, end-f->cur);
    }

    if (*end == '\r') {
        ++end;
    }
    if (*end == '\n') {
        ++end;
    }

    f->cur = end;
}

gint dxfile_get_line_str(Dxfile *f, gchar *s, gint size)
{
    g_return_val_if_fail( f != NULL, -1 );
    g_return_val_if_fail( s != NULL, -1);
    g_return_val_if_fail( size > 0, -1);

    gchar *tmp = s;

    for (;
         size--
         && *f->cur != '\n'
         && *f->cur != '\r';
         ++f->cur ) {
        *tmp++ = *f->cur;
    }
    *tmp = 0;

    if (*f->cur == '\r') {
        ++f->cur;
    }
    if (*f->cur == '\n') {
        ++f->cur;
    }

    return tmp - s;
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
    g_mapped_file_unref(f->mmpedFile);
    g_free(f);
}

gboolean dxfile_set_pos(Dxfile *f, glong pos)
{
    g_return_val_if_fail( f != NULL, FALSE );
    if ( pos < 0 || pos > f->len ) {
        return FALSE;
    }
    f->cur = f->contents + pos;
    return TRUE;
}

gboolean dxfile_lseek_section(Dxfile *f, const DxfSectionFlag flag)
{
    gchar *str = NULL;
    gchar *needle = NULL;
    gchar tmp[3] = {0};

    if (f->enc) {
        tmp[0] = '\r';
        tmp[1] = '\n';
    } else {
        tmp[0] = '\n';
    }

    switch (flag) {
    case DXF_SECTION_HEADER:
        needle = g_strdup_printf("2%s%s%s",tmp, SECTION_HEADER, tmp);
        break;
    case DXF_SECTION_CLASSES:
        needle = g_strdup_printf("2%s%s%s",tmp, SECTION_CLASSES, tmp);
        break;
    case DXF_SECTION_TABLES:
        needle = g_strdup_printf("2%s%s%s",tmp, SECTION_TABLES, tmp);
        break;
    case DXF_SECTION_BLOCKS:
        needle = g_strdup_printf("2%s%s%s",tmp, SECTION_BLOCKS, tmp);
        break;
    case DXF_SECTION_ENTITIES:
        needle = g_strdup_printf("2%s%s%s",tmp, SECTION_ENTITIES, tmp);
        break;
    case DXF_SECTION_OBJECTS:
        needle = g_strdup_printf("2%s%s%s",tmp, SECTION_OBJECTS, tmp);
        break;
    default:
        break;
    }

    str = g_strstr_len(f->contents, -1, needle);
    if (NULL == str) {
        g_free(needle);
        return FALSE;
    }

    f->cur = str + strlen(needle);
    g_free(needle);
    return TRUE;
}

gint dxfile_encoding(const gchar *filename)
{
    gchar *str = g_strdup_printf("file %s | grep CRLF", filename);
    gint c = 0;
    FILE *fp = NULL;

    fp = popen(str, "r");
    g_free(str);
    if (fp == NULL) {
        return 0;
    }
    c = fgetc(fp);
    pclose(fp);
    if (c <= 0) {
        return 0;
    }
    return 1;
}

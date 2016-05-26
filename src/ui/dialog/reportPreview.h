/**
 * @file reportPreview.h
 * @brief report preview
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-05-26
 */

#ifndef __REPORT_PREVIEW_H__
#define __REPORT_PREVIEW_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define DEFAULT_REPORT_TEMPLATE "/home/tt/TT/source/system/Template/Report/Complete.html"

extern void report_preview_set_tmpl(const gchar *tmpl);
extern gchar *report_preview_get_tmpl();

extern GtkWidget* report_preview_new(GtkWidget* fatherWidget);

G_END_DECLS

#endif /* end of __REPORT_PREVIEW_H__ */

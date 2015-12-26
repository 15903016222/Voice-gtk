#include "gtk_dxf.h"
#include "dxf_paint.h"

#define cairo_set_source_rgb_color(cr, color) cairo_set_source_rgb(cr, color.red, color.green, color.blue);

static void gtk_dxf_class_init(GtkDxfClass *klass);
static void gtk_dxf_init(GtkDxf *dxf);
static void gtk_dxf_destroy(GtkObject *object);
static void gtk_dxf_size_request(GtkWidget *widget, GtkRequisition *requisition);
static void gtk_dxf_size_allocate(GtkWidget *widget, GtkAllocation *allocation);
static void gtk_dxf_realize(GtkWidget *widget);
static gboolean gtk_dxf_expose(GtkWidget *widget, GdkEventExpose *event);

static void gtk_dxf_paint(GtkWidget *widget);
static inline void _paint_backgroup(cairo_t *cr, GtkDxf *dxf);  /*设置背景颜色*/
static inline void _paint_base_point(cairo_t *cr, GtkDxf *dxf); /*画原点 */
static void _paint_wcs_axis(cairo_t *cr, GtkDxf *dxf);          /*画wcs X-Y轴*/
static void _paint_ucs_axis(cairo_t *cr, GtkDxf *dxf);          /*画ucs X-Y轴*/

#define DXF_DEFAULT_SIZE    800

GtkType gtk_dxf_get_type()
{
    static GtkType gtk_dxf_type = 0;
    if (!gtk_dxf_type) {
        static const GtkTypeInfo gtk_dxf_info = {
            "GtkDxf",
            sizeof(GtkDxf),
            sizeof(GtkDxfClass),
            (GtkClassInitFunc)gtk_dxf_class_init,
            (GtkObjectInitFunc)gtk_dxf_init,
            NULL,
            NULL,
            (GtkClassInitFunc)NULL
        };
        gtk_dxf_type = gtk_type_unique(GTK_TYPE_WIDGET, &gtk_dxf_info);
    }
    return gtk_dxf_type;
}


GtkWidget *gtk_dxf_new()
{
    return GTK_WIDGET(gtk_type_new(gtk_dxf_get_type()));
}

GtkWidget *gtk_dxf_new_with_file(const gchar *file, const DxfSectionFlag flag)
{
    GtkWidget *widget;

    g_return_val_if_fail( file != NULL, NULL );

    widget = gtk_dxf_new();
    gtk_dxf_load_file(GTK_DXF(widget), file, flag);

    return widget;
}

static void gtk_dxf_class_init(GtkDxfClass *klass)
{
    GtkWidgetClass *widgetClass = (GtkWidgetClass *)klass;
    GtkObjectClass *objectClass = (GtkObjectClass *)klass;

    widgetClass->realize = gtk_dxf_realize;
    widgetClass->size_request = gtk_dxf_size_request;
    widgetClass->size_allocate = gtk_dxf_size_allocate;
    widgetClass->expose_event = gtk_dxf_expose;

    objectClass->destroy = gtk_dxf_destroy;
}

void gtk_dxf_init(GtkDxf *dxf)
{
    dxf->bgColor.blue = 0;
    dxf->bgColor.green = 0;
    dxf->bgColor.red = 0;

    dxf->cairoColor.blue = 255;
    dxf->cairoColor.green = 255;
    dxf->cairoColor.red = 255;

    dxf->d = NULL;

    dxf->sx = 1;
    dxf->sy = 1;

    gtk_widget_add_events(GTK_WIDGET(dxf),
                          GDK_BUTTON_MOTION_MASK
                          | GDK_BUTTON_PRESS_MASK
                          | GDK_BUTTON_RELEASE_MASK
                          | GDK_SCROLL_MASK);
}

static void gtk_dxf_destroy(GtkObject *object){
    GtkDxf *dxf;
    GtkDxfClass *klass;

    g_return_if_fail(object != NULL);
    g_return_if_fail(GTK_IS_DXF(object));
    dxf = GTK_DXF(object);

    dxf_delete(dxf->d);
    dxf->d = NULL;

    klass = gtk_type_class(gtk_widget_get_type());
    if (GTK_OBJECT_CLASS(klass)->destroy) {
        (* GTK_OBJECT_CLASS(klass)->destroy) (object);
    }
}

static void gtk_dxf_size_request(GtkWidget *widget, GtkRequisition *requisition)
{
    g_return_if_fail(widget != NULL);
    g_return_if_fail(GTK_IS_DXF(widget));
    g_return_if_fail(requisition != NULL);

    if (widget->window) {
        requisition->height = widget->allocation.height;
        requisition->width = widget->allocation.width;
    } else {
        requisition->height = DXF_DEFAULT_SIZE;
        requisition->width = DXF_DEFAULT_SIZE;
    }
}

static void gtk_dxf_realize(GtkWidget *widget)
{
    GdkWindowAttr attributes;
    guint attributes_mask;

    g_return_if_fail(widget != NULL);
    g_return_if_fail(GTK_IS_DXF(widget));

    GTK_WIDGET_SET_FLAGS(widget, GTK_REALIZED);

    attributes.window_type = GDK_WINDOW_CHILD;
    attributes.x = widget->allocation.x;
    attributes.y = widget->allocation.y;
    attributes.width = widget->allocation.width;
    attributes.height = widget->allocation.height;
    attributes.wclass = GDK_INPUT_OUTPUT;
    attributes.event_mask = gtk_widget_get_events(widget) | GDK_EXPOSURE_MASK;
    attributes_mask = GDK_WA_X | GDK_WA_Y;

    widget->window = gdk_window_new(gtk_widget_get_parent_window(widget), &attributes, attributes_mask);
    gdk_window_set_user_data(widget->window, widget);

    widget->style = gtk_style_attach(widget->style, widget->window);

    gtk_style_set_background(widget->style, widget->window, GTK_STATE_NORMAL);
}

static void gtk_dxf_size_allocate(GtkWidget *widget, GtkAllocation *allocation)
{
    g_return_if_fail (widget != NULL);
    g_return_if_fail (GTK_IS_DXF(widget));
    g_return_if_fail (allocation != NULL);

    widget->allocation = *allocation;
    if (gtk_widget_get_realized (widget)) {
        gdk_window_move_resize (widget->window,
                    allocation->x, allocation->y,
                    allocation->width, allocation->height);
      }
}

static gboolean gtk_dxf_expose(GtkWidget *widget, GdkEventExpose *event)
{
    g_return_val_if_fail(widget != NULL, FALSE);
    g_return_val_if_fail(widget->window != NULL, FALSE);
    g_return_val_if_fail(GTK_IS_DXF(widget), FALSE);
    g_return_val_if_fail(event != NULL, FALSE);

    gtk_dxf_paint(widget);

    return FALSE;
}

/**
 * @brief gtk_dxf_paint     绘制图形
 * @param widget            指向部件本身
 */
static void gtk_dxf_paint(GtkWidget *widget)
{
    cairo_t *cr;

    cr = gdk_cairo_create(widget->window);

    /* 通用设置 */
    _paint_backgroup(cr, GTK_DXF(widget));
    _paint_base_point(cr, GTK_DXF(widget));     /* 原点 */
    _paint_wcs_axis(cr, GTK_DXF(widget));       /* wcs X-Y 轴 */
    _paint_ucs_axis(cr, GTK_DXF(widget));       /* ucs X-Y轴 */

    cairo_set_dash(cr, NULL, 0, 0);
    cairo_scale(cr, GTK_DXF(widget)->sx, GTK_DXF(widget)->sy);
    cairo_set_line_width (cr, 1/GTK_DXF(widget)->sx);
    cairo_set_source_rgb_color(cr, GTK_DXF(widget)->cairoColor);

    dxf_paint(GTK_DXF(widget)->d, cr, GTK_DXF(widget)->sx, GTK_DXF(widget)->sy);

    cairo_destroy(cr);
}

static void _paint_backgroup(cairo_t *cr, GtkDxf *dxf)
{
    cairo_set_source_rgb_color(cr, dxf->bgColor);
    cairo_paint(cr);
}

static inline void _paint_base_point(cairo_t *cr, GtkDxf *dxf)
{
    cairo_set_source_rgb(cr, 255, 0, 0);    /*set red*/
    cairo_translate(cr,
                    dxf->basePointX + dxf->ucsx,
                    dxf->basePointY + dxf->ucsy);
    cairo_arc(cr, 0, 0, 3, 0, 10);

    cairo_fill(cr);
}

static void _paint_wcs_axis(cairo_t *cr, GtkDxf *dxf)
{
    gdouble dashed[2] = {0.5, 7.5};

    cairo_save(cr);
    cairo_set_source_rgb(cr, 0xffff, 0xffff, 0xffff);
    cairo_set_line_width (cr, 1);

    /* wcs X-Y Direction */
    cairo_move_to(cr,
                  GTK_WIDGET(dxf)->allocation.width - (dxf->basePointX+dxf->ucsx) - 10,
                  10);
    cairo_line_to(cr,
                  GTK_WIDGET(dxf)->allocation.width - (dxf->basePointX+dxf->ucsx),
                  0);
    cairo_move_to(cr,
                  GTK_WIDGET(dxf)->allocation.width - (dxf->basePointX+dxf->ucsx) - 10,
                  -10);
    cairo_line_to(cr,
                  GTK_WIDGET(dxf)->allocation.width - (dxf->basePointX+dxf->ucsx),
                  0);
    cairo_move_to(cr,
                  10,
                  -(dxf->basePointY + dxf->ucsy) + 10);
    cairo_line_to(cr,
                  0,
                  -(dxf->basePointY + dxf->ucsy));
    cairo_move_to(cr,
                  -10,
                  -(dxf->basePointY + dxf->ucsy) + 10);
    cairo_line_to(cr,
                  0,
                  -(dxf->basePointY + dxf->ucsy));

    cairo_stroke(cr);

    cairo_set_dash (cr, dashed, 2, 0);

    /* X-Y axis */
    cairo_move_to (cr,
                   -(dxf->basePointX + dxf->ucsx),
                   0);
    cairo_line_to (cr,
                   GTK_WIDGET(dxf)->allocation.width - (dxf->basePointX + dxf->ucsx),
                   0);
    cairo_move_to (cr,
                   0,
                   -(dxf->basePointY + dxf->ucsy));
    cairo_line_to (cr, 0, GTK_WIDGET(dxf)->allocation.height - (dxf->basePointY + dxf->ucsy));
    cairo_stroke (cr);
    cairo_restore(cr);
}

static void _paint_ucs_axis(cairo_t *cr, GtkDxf *dxf)
{
    if (dxf->d == NULL) {
        return;
    }
    DxfHeader *h = dxf->d->header;
    gdouble x = 55 * h->ucsxdir.x;
    gdouble y = -55 * h->ucsydir.y;
    cairo_save(cr);

    cairo_translate(cr, h->ucsorg.x * dxf->sx, -h->ucsorg.y * dxf->sy);

    /* X axis */
    cairo_move_to(cr, 0, 0);
    cairo_line_to(cr, x, 0);
    /* Y axis */
    cairo_move_to(cr, 0, 0);
    cairo_line_to(cr, 0, y);

    /* X axis direction */
    cairo_move_to(cr, x, 0);
    cairo_line_to(cr, x-5*h->ucsxdir.x, 5);
    cairo_move_to(cr, x, 0);
    cairo_line_to(cr, x-5*h->ucsxdir.x, -5);
    /* Y axis direction */
    cairo_move_to(cr, 0, y);
    cairo_line_to(cr, 5, y+5*h->ucsydir.y);
    cairo_move_to(cr, 0, y);
    cairo_line_to(cr, -5, y+5*h->ucsydir.y);
    cairo_stroke(cr);

    /* Text */
    cairo_move_to(cr, x, -10*h->ucsydir.y);
    cairo_show_text(cr, "X");
    cairo_move_to(cr, 10, y + 5*h->ucsydir.y);
    cairo_show_text(cr, "Y");
    cairo_move_to(cr, 5*h->ucsxdir.x, -10*h->ucsydir.y);
    cairo_show_text(cr, "(ucs)");
    cairo_stroke(cr);

    cairo_restore(cr);
}

void gtk_dxf_load_file(GtkDxf *dxf, const gchar *file, const DxfSectionFlag flag)
{
    g_return_if_fail(dxf != NULL);
    g_return_if_fail(file != NULL);

    if (dxf->d) {
        dxf_delete(dxf->d);
        dxf->d = NULL;
    }

    dxf->d = dxf_new_for_file(file, flag);
    if (NULL == dxf->d) {
        c_log_err("Invalid file");
        return;
    }

    /* scale */
    dxf->sx=unit_conversion(dxf->d->header->insunits, DXF_UNIT_MM);
    dxf->sy=unit_conversion(dxf->d->header->insunits, DXF_UNIT_MM);

    /*basepoint*/
    dxf->basePointX = 0;
    dxf->basePointY = 0;

    /* ucs */
    if (dxf->d->header && dxf->d->header) {
        DxfHeader *h = dxf->d->header;
        if (h->ucsxdir.x == 1) {
            dxf->ucsx = - h->ucsorg.x;
        } else if (h->ucsxdir.x == -1) {
            dxf->ucsx = GTK_WIDGET(dxf)->allocation.width - h->ucsorg.x;
        } else {
            dxf->ucsx = 0;
        }

        if (h->ucsydir.y == 1) {
            dxf->ucsy = GTK_WIDGET(dxf)->allocation.height + h->ucsorg.y;
        } else if (h->ucsydir.y == -1){
            dxf->ucsy = h->ucsorg.y;
        } else {
            dxf->ucsy = GTK_WIDGET(dxf)->allocation.height;
        }
    }

#ifdef C_DEBUG
    {
        gchar *msg = NULL;
        msg = dxf_header_print(dxf->d->header);
        g_message("\n%s", msg);
        g_free(msg);
        msg = dxf_entities_print(dxf->d->entities);
        g_message("\n%s", msg);
        g_free(msg);
    }
#endif

    gtk_widget_queue_draw(GTK_WIDGET(dxf));
}

void gtk_dxf_set_basepoint(GtkDxf *dxf, const gdouble x, const gdouble y)
{
    g_return_if_fail(dxf != NULL);

    dxf->basePointX = x;
    dxf->basePointY = y;

    gtk_widget_queue_draw(GTK_WIDGET(dxf));
}


void gtk_dxf_set_scale(GtkDxf *dxf, gdouble sx, gdouble sy)
{
    g_return_if_fail(dxf != NULL);

    dxf->sx = sx;
    dxf->sy = sy;

    gtk_widget_queue_draw(GTK_WIDGET(dxf));
}

void gtk_dxf_set_fg(GtkDxf *dxf, const GdkColor *color)
{
    g_return_if_fail(dxf != NULL);
    g_return_if_fail(color != NULL);

    dxf->cairoColor.blue = color->blue;
    dxf->cairoColor.green = color->green;
    dxf->cairoColor.pixel = color->pixel;
    dxf->cairoColor.red = color->red;
}

void gtk_dxf_set_bg(GtkDxf *dxf, const GdkColor *color)
{
    g_return_if_fail(dxf != NULL);
    g_return_if_fail(color != NULL);

    dxf->bgColor.blue = color->blue;
    dxf->bgColor.green = color->green;
    dxf->bgColor.pixel = color->pixel;
    dxf->bgColor.red = color->red;
}

#ifndef __GDXF_H__
#define __GDXF_H__

#include <gtk/gtk.h>
#include <cairo/cairo.h>
#include <dxf.h>

G_BEGIN_DECLS

/*定义类型检测*/
#define GTK_DXF(obj) GTK_CHECK_CAST(obj, gtk_dxf_get_type(), GtkDxf)

#define GTK_DXF_CLASS(klass) GTK_CHECK_CLASS_CAST(klass, gtk_dxf_get_type(), GtkDxfClass)

#define GTK_IS_DXF(obj) GTK_CHECK_TYPE(obj, gtk_dxf_get_type())

typedef struct _GtkDxf GtkDxf;
typedef struct _GtkDxfClass GtkDxfClass;

#define Y_AXIS_UPWARD   (-1)
#define Y_AXIS_DOWN     (1)
#define X_AXIS_RIGHT    (1)
#define X_AXIS_LEFT     (-1)

struct _GtkDxf {
    GtkWidget parentWidget; /*父控件*/
    gdouble basePointX;     /*原点X-Y坐标*/
    gdouble basePointY;
    gdouble ucsx;           /*ucs*/
    gdouble ucsy;
    GdkColor bgColor;       /*背景颜色*/
    GdkColor cairoColor;    /*画笔颜色*/
    Dxf *d;                 /*dxf数据*/
    gdouble sx;             /*X轴缩放倍数*/
    gdouble sy;             /*Y轴缩放倍数*/
};

struct _GtkDxfClass {
    GtkWidgetClass parentWidgetClass;   /*父对象类*/
};

GtkType gtk_dxf_get_type();

/**
 * @brief gtk_dxf_new   创建GtkDxf对象
 * @return              成功返回GtkDxf对象，失败返回NULL
 */
extern GtkWidget *gtk_dxf_new();

/**
 * @brief gtk_dxf_new_with_file 创建GtkDxf对象
 * @param dxfile                dxf文件
 * @return                      成功返回GtkDxf对象，失败返回NULL
 */
extern GtkWidget *gtk_dxf_new_with_file(const gchar *dxfile, const DxfSectionFlag flag);

/**
 * @brief gtk_dxf_load_file 加载dxf文件
 * @param dxf               dxf对象
 * @param file              dxf文件
 */
extern void gtk_dxf_load_file(GtkDxf *dxf, const gchar *file, const DxfSectionFlag flag);

/**
 * @brief gtk_dxf_set_basepoint 设置原点坐标
 * @param dxf                   GtkDxf对象
 * @param x                     x坐标
 * @param y                     y坐标
 */
extern void gtk_dxf_set_basepoint(GtkDxf *dxf, const gdouble x, const gdouble y);

/**
 * @brief gtk_dxf_get_sx    获取X轴缩放系数
 * @param dxf               GtkDxf对象
 * @return                  返回缩放系数
 */
static inline gdouble gtk_dxf_get_sx(const GtkDxf *dxf) { g_return_val_if_fail(dxf != NULL, 0); return dxf->sx; }

/**
 * @brief gtk_dxf_get_sy    获取Y轴缩放系数
 * @param dxf               GtkDxf对象
 * @return                  返回缩放系数
 */
static inline gdouble gtk_dxf_get_sy(const GtkDxf *dxf) { g_return_val_if_fail(dxf != NULL, 0); return dxf->sy; }

/**
 * @brief gtk_dxf_set_scale 设置缩放系数
 * @param dxf               GtkDxf对象
 * @param sx                X轴缩放系数
 * @param sy                Y轴缩放系数
 */
extern void gtk_dxf_set_scale(GtkDxf *dxf, gdouble sx, gdouble sy);

/**
 * @brief gtk_dxf_set_fg    设置图形的颜色
 * @param dxf               GtkDxf对象
 * @param color             指定颜色
 */
extern void gtk_dxf_set_fg(GtkDxf *dxf, const GdkColor *color);

/**
 * @brief gtk_dxf_set_bg    设置背景颜色
 * @param dxf               GtkDxf对象
 * @param color             指定颜色
 */
extern void gtk_dxf_set_bg(GtkDxf *dxf, const GdkColor *color);

G_END_DECLS



#endif /* End of __GDXF_H__ */

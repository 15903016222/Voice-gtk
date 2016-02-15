/**
 * File: workpiece.h
 * Desc: load and paint workpiece
 * Author: JakeYang
 * Log: 2015-09-21
 */

#ifndef __WORKPIECE_H__
#define __WORKPIECE_H__

#include "../base.h"

#include <gtk/gtk.h>
#include <cairo/cairo.h>

#define WORKPIECE_NUM    8           /* 最大加载工件数 */
#define Y_AXIS_UPWARD   (-1)    /*Y轴朝上*/
#define Y_AXIS_DOWN     (1)     /*Y轴朝下*/

/**
 * @brief workpiece_init 工件模块初始化
 */
extern void workpiece_init();

/**
 * @brief workpiece_uninit 工件卸载
 */
extern void workpiece_uninit();

/**
 * @brief workpiece_load_file   加载dxf文件
 * @param filename              dxf文件名
 * @param grp                   配置组
 * @return                      成功返回TRUE， 失败返回FALSE
 */
extern gboolean workpiece_load_file(const gchar *filename, guint grp);

/**
 * @brief workpiece_paint   画工件
 * @param widget            指向画板
 * @param data
 * @param cr                画笔
 */
extern void workpiece_paint(GtkWidget* widget , const DRAW_AREA_P prule , cairo_t* cr);

/**
 * @brief workpiece_clean   清除配置组的工件信息
 */
extern void workpiece_clean();

/**
 * @brief workpiece_clean_grp   清除指定组信息
 * @param grp                   指定组
 */
extern void workpiece_clean_grp(guint grp);

/**
 * @brief workpiece_basepoint   获取原点坐标
 * @param grp                   指定组
 * @param x                     原点X坐标
 * @param y                     原点Y坐标
 */
extern void workpiece_basepoint(guint grp, gdouble *x, gdouble *y);

/**
 * @brief workpiece_set_basepoint   设置原点坐标
 * @param grp                       指定组
 * @param x                         原点X坐标
 * @param y                         原点Y坐标
 */
extern void workpiece_set_basepoint(guint grp, gdouble x, gdouble y);

#endif /* End of __WORKPIECE_H__ */

#if 0
#ifndef DRAW_PART_H_
#define DRAW_PART_H_
#include <gtk/gtk.h>

enum
{
	NCC_CMD_NULL = 0,
	NCC_CMD_MOVE_TO ,
	NCC_CMD_LINE_TO ,
	NCC_CMD_CURVE_TO,
	NCC_CMD_ARC_POSITIVE,
	NCC_CMD_ARC_NEGATIVE,
	NCC_CMD_LINE_WIDTH,
	NCC_CMD_LINE_MODE,
	NCC_CMD_COLOR_CHANGE,
};

typedef struct weld_file
{
	char strName[3] ;
	int  nDataOffset;
	int  nFileSize  ;
	int  nDataSize  ;
}WeldFile ;

typedef struct ncc_cmd
{
	int nCmd  ;
	double p1 ;
	double p2 ;
	double p3 ;
	double p4 ;
}NCC_CMD , *NCC_CMD_P;


typedef struct part_info
{
	NCC_CMD_P pNccCmd ;
	int nCmdQty  ;
	double nPosx ;
	double nPosy ;
	double nScaleRate ;
}PART_INFO , *PART_INFO_P  ;

typedef struct display_zoom
{
	double nStartX ;
	double nStartY ;
	double nRangeX ;
	double nRangeY ;
	double nDisStartX ;
	double nDisStartY ;
	double nDisRangeX ;
	double nDisRangeY ;
}DISPLAY_ZOOM , *DISPLAY_ZOOM_P  ;

typedef struct f_point
{
	double x ;
	double y;
}FPoint , *FPoint_P;

extern void DrawPart(GtkWidget* widget , gpointer data , cairo_t* cr);
extern int  LoadWeldFile(const char* strPathName_ , int nGroupId_);
extern void ClearPartInfor(int nModel) ;
extern double GetScaleRate(int grp) ;
extern void SetScaleRage(int grp , double scale) ;
#endif /* DRAW_PART_H_ */
#endif

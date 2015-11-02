/**
 * File: dxf_paint.h
 * Desc: 根据Dxf描绘图形
 * Author: JakeYang
 * Log: 2015-10-16
 */

#ifndef __DXF_PAINT_H__
#define __DXF_PAINT_H__

#include "dxf.h"
#include <cairo/cairo.h>

/**
 * @brief dxf_paint     根据Dxf数据绘制图形
 * @param d             Dxf数据
 * @param cr            画笔
 * @param scalex        X坐标缩放系数
 * @param scaley        Y坐标缩放系数
 */
extern void dxf_paint(const Dxf *d, cairo_t *cr, gdouble scalex, gdouble scaley);


/**
 * @brief unit_conversion   计算单位转换率 单位1/单位2
 * @param s                 单位1
 * @param t                 单位2
 * @return                  返回比值
 */
extern gdouble unit_conversion(DxfUnits s, DxfUnits t);

#endif /* End of __DXF_PAINT_H__ */

#ifndef __ENTITIES_POLYLINE_H__
#define __ENTITIES_POLYLINE_H__

/**
 * 注意： 由于polyline的实现方式与lwpolyline一样，故此不在实现，只做标记
 */

#include "dxfile.h"

/* Group Code ID */
#define POLYLINE_CODE_FOLLOW_FLAG   66  /*已废弃；以前是“后跟图元标志”
                                            （可选；如果存在则忽略）*/
#define POLYLINE_C_VERTEX_X     10  /*DXF：始终为 0;
                                        APP：“虚拟”点；X 值和 Y 值始终为 0;
                                        Z 值是多段线的标高（二维时在 OCS 中，
                                        三维时在 WCS 中）*/
#define POLYLINE_C_VERTEX_Y     20  /*DXF：始终为 0*/
#define POLYLINE_C_VERTEX_Z     30  /*DXF：多段线的标高
                                        （二维时在 OCS 中，三维时在 WCS 中）*/
#define POLYLINE_C_THICKNESS    39  /*厚度（可选；默认值 = 0）*/
#define POLYLINE_C_FLAG         70  /*多段线标志（按位编码；默认值 = 0）：
                                        1 = 这是一个闭合多段线
                                            （或按 M 方向闭合的多边形网格）
                                        2 = 已添加曲线拟合顶点
                                        4 = 已添加样条曲线拟合顶点
                                        8 = 这是一条三维多段线
                                        16 = 这是一个三维多边形网格
                                        32 = 多边形网格以 N 方向闭合
                                        64 = 多段线是一个多面网格
                                        128 = 线型图案在该多段线顶点的周围连续生成*/
#define POLYLINE_C_START_WIDTH  40  /*默认起点宽度（可选；默认值 = 0）*/
#define POLYLINE_C_END_WIDTH    41  /*默认端点宽度（可选；默认值 = 0）*/
#define POLYLINE_C_POLYGON_MESH_N_VERTEX_COUNT  71  /*多边形网格 M 顶点计数
                                                        （可选；默认值 = 0）*/
#define POLYLINE_C_POLYGON_MESH_N_VERTEX_CNT    72  /*多边形网格 N 顶点计数
                                                        （可选；默认值 = 0）*/
#define POLYLINE_C_SMOOTH_M     73  /*平滑曲面 M 密度（可选；默认值 = 0）*/
#define POLYLINE_C_SMOOTH_N     74  /*平滑曲面 N 密度（可选；默认值 = 0）*/
#define POLYLINE_C_SMOOTH_TYPE  75  /*曲线和平滑曲面类型
                                        （可选；默认值 = 0）；整数代码，非按位编码：
                                        0 = 不拟合平滑曲面
                                        5 = 二次 B 样条曲面
                                        6 = 三次 B 样条曲面
                                        8 = Bezier 曲面*/

#define POLYLINE_C_EXTR_X       210 /*拉伸方向（可选；默认值 = 0, 0, 1）
                                            DXF：X 值；APP：三维矢量*/
#define POLYLINE_C_EXTR_Y       220 /*DXF：拉伸方向的 Y 值（可选）*/
#define POLYLINE_C_EXTR_Z       230 /*DXF：拉伸方向的 Z 值（可选）*/

typedef struct _DxfPolyline DxfPolyline;
struct _DxfPolyline {
};

extern DxfPolyline *dxf_polyline_parse(Dxfile *f);
extern void dxf_polyline_delete(DxfPolyline *e);
extern gchar *dxf_polyline_print(DxfPolyline *e);

#endif /* End of __ENTITIES_POLYLINE_H__ */

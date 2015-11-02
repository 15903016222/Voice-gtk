#include "dxf_tables_vport.h"


DxfTablesVport *dxf_tables_vport_parse(Dxfile *f)
{
    DxfTablesVport *v = NULL;

    v = g_malloc0(sizeof(DxfTablesVport));

    SWITCH_CODE_BEGIN(f);
    SWITCH_CODE_DO( 2 )
            /* 视口名 */
            dxfile_get_line(f, &v->name, NULL);
    SWITCH_CODE_DO( 70 )
            /* 标准标记值（按位编码值）：
             * 16 = 如果设定了此位，则表条目外部依赖于外部参照
             * 32 = 如果同时设定了此位和位 16，则表明已成功融入了
             *      外部依赖的外部参照
             * 64 = 如果设定了此位，则表明在上次编辑图形时，图形中至少
             *      有一个图元参照了表条目。
             * （此标志对 AutoCAD 命令很有用。大多数读取DXF
             * 文件的程序都可以忽略它，并且无需由写入 DXF 文件
             * 的程序对其进行设定）*/
            dxfile_get_int16(f, &v->standardFlag);
    SWITCH_CODE_DO( 10 )
            /*视口的左下角, DXF：X 值；APP：二维点*/
            dxfile_get_double(f, &v->lowerLeftCorner.x);
    SWITCH_CODE_DO( 20 )
            /*DXF：视口左下角的 Y 值*/
            dxfile_get_double(f, &v->lowerLeftCorner.y);
    SWITCH_CODE_DO( 11 )
            /*视口右上角, DXF：X 值；APP：二维点*/
            dxfile_get_double(f, &v->upperRightCorner.x);
    SWITCH_CODE_DO( 21 )
            /* DXF：视口右上角的 Y 值 */
            dxfile_get_double(f, &v->upperRightCorner.y);
    SWITCH_CODE_DO( 12 )
            /*视口中心点（在 DCS 中）,DXF：X 值；APP：二维点*/
            dxfile_get_double(f, &v->viewCenter.x);
    SWITCH_CODE_DO( 22 )
            /*DXF：视图中心点的 Y 值（在 DCS 中）*/
            dxfile_get_double(f, &v->viewCenter.y);
    SWITCH_CODE_DO( 13 )
            /*捕捉基点（在 DCS 中）,DXF：X 值；APP：二维点*/
            dxfile_get_double(f, &v->snapBase.x);
    SWITCH_CODE_DO( 23 )
            /*DXF：捕捉基点的 Y 值（在 DCS 中）*/
            dxfile_get_double(f, &v->snapBase.y);
    SWITCH_CODE_DO( 14 )
            /*捕捉间距 X 和 Y, DXF：X 值；APP：二维点*/
            dxfile_get_double(f, &v->snapSpacing.x);
    SWITCH_CODE_DO( 24 )
            /*DXF：捕捉间距 X 和 Y 的 Y 值*/
            dxfile_get_double(f, &v->snapSpacing.y);
    SWITCH_CODE_DO( 15 )
            /*栅格间距 X 和 Y, DXF：X 值；APP：二维点*/
            dxfile_get_double(f, &v->gridSpacing.x);
    SWITCH_CODE_DO( 25 )
            /*DXF：栅格间距 X 和 Y 的 Y 值*/
            dxfile_get_double(f, &v->gridSpacing.y);
    SWITCH_CODE_DO( 16 )
            /*相对于目标点的观察方向（在 WCS 中）,DXF：X 值；APP：三维点*/
            dxfile_get_double(f, &v->viewCenter.x);
    SWITCH_CODE_DO( 26 )
            /*DXF：相对于目标点的观察方向的 Y 值,（在 WCS 中）*/
            dxfile_get_double(f, &v->viewCenter.y);
    SWITCH_CODE_DO( 36 )
            /*DXF：相对于目标点的观察方向的 Z 值,（在 WCS 中）*/
            dxfile_get_double(f, &v->viewCenter.z);
    SWITCH_CODE_DO( 17 )
            /*观察目标点（在 WCS 中）, DXF：X 值；APP：三维点*/
            dxfile_get_double(f, &v->target.x);
    SWITCH_CODE_DO( 27 )
            /*DXF：观察目标点的 Y 值和 Z 值（在 WCS 中）*/
            dxfile_get_double(f, &v->target.y);
    SWITCH_CODE_DO( 37 )
            /*DXF：观察目标点的 Y 值和 Z 值（在 WCS 中）*/
            dxfile_get_double(f, &v->target.z);
    SWITCH_CODE_DO( 40 ) {}
    SWITCH_CODE_DO_FOLLOW( 45 )
            /*视图高度*/
            dxfile_get_double(f, &v->viewHeight);
    SWITCH_CODE_DO( 41 )
            /*视图纵横比*/
            dxfile_get_double(f, &v->aspectRatio);
    SWITCH_CODE_DO( 42 )
            /*焦距*/
            dxfile_get_double(f, &v->lensLenght);
    SWITCH_CODE_DO( 43 )
            /*前向剪裁平面（距目标点的偏移）*/
            dxfile_get_double(f, &v->frontPlaneOffset);
    SWITCH_CODE_DO( 44 )
            /*后向剪裁平面（距目标点的偏移）*/
            dxfile_get_double(f, &v->backPlaneOffset);
    SWITCH_CODE_DO( 50 )
            /*捕捉旋转角度*/
            dxfile_get_double(f, &v->snapRotationAngle);
    SWITCH_CODE_DO( 51 )
            /*视图扭转角度*/
            dxfile_get_double(f, &v->viewTwistAngle);
    SWITCH_CODE_DO( 71 )
            /*视图模式（参见 VIEWMODE 系统变量）*/
            dxfile_get_int16(f, &v->viewMode);
    SWITCH_CODE_DO( 72 )
            /*圆形边*/
            dxfile_get_int16(f, &v->circleSides);
    SWITCH_CODE_DO( 73 )
            /*快速变焦设定*/
            dxfile_get_int16(f, &v->fastZoomSetting);
    SWITCH_CODE_DO( 331 )
            /*冻结图层对象的软指针 ID/句柄；为每个冻结图层重复*/
            dxfile_get_line(f, &v->frozenLayerSoftPointer, NULL);
    SWITCH_CODE_DO( 441 )
            /*冻结图层对象的硬指针 ID/句柄；为每个冻结图层重复*/
            dxfile_get_line(f, &v->frozenLayerHardPointer, NULL);
    SWITCH_CODE_END(f);

    return v;
}


void dxf_tables_vport_delete(DxfTablesVport *e)
{
    g_return_if_fail(e != NULL );
    g_free(e->name);
    g_free(e->frozenLayerHardPointer);
    g_free(e->frozenLayerSoftPointer);
    g_free(e);
}


gchar *dxf_tables_vport_print(DxfTablesVport *e)
{
    e = e;
    return g_strdup("vport test message\n");
}

/**
 * @file entities_ellipse.c
 * @brief dxf entities层, ellipse对象
 * @author JakeYang <add358@gmail.com>
 * @version 0.2
 * @date 2015-04-14
 */

#include "entities_ellipse.h"
#include <math.h>

static inline DxfEllipse *ellipse_new_item()
{
    DxfEllipse *e = g_malloc0(sizeof(DxfEllipse));

    e->ration = 1;
    e->startAngle = 0;
    e->endAngle = 2*M_PI;
    e->extrDir.z = 1;

    return e;
}

DxfEllipse *dxf_ellipse_parse(Dxfile *f)
{
    DxfEllipse *e;

    e = ellipse_new_item();

    SWITCH_CODE_BEGIN(f);
    SWITCH_CODE_DO( 10 )
            dxfile_get_double(f, &e->centerPoint.x);
    SWITCH_CODE_DO( 20 )
            dxfile_get_double(f, &e->centerPoint.y);
    SWITCH_CODE_DO( 30 )
            dxfile_get_double(f, &e->centerPoint.z);
    SWITCH_CODE_DO( 11 )
            dxfile_get_double(f, &e->endPointMajorAxis.x);
    SWITCH_CODE_DO( 21 )
            dxfile_get_double(f, &e->endPointMajorAxis.y);
    SWITCH_CODE_DO( 31 )
            dxfile_get_double(f, &e->endPointMajorAxis.z);
    SWITCH_CODE_DO( 40 )
            dxfile_get_double(f, &e->ration);
    SWITCH_CODE_DO( 41 )
            dxfile_get_double(f, &e->startAngle);
    SWITCH_CODE_DO( 42 )
            dxfile_get_double(f, &e->endAngle);
    SWITCH_CODE_DO( 210 )
            dxfile_get_double(f, &e->extrDir.x);
    SWITCH_CODE_DO( 220 )
            dxfile_get_double(f, &e->extrDir.y);
    SWITCH_CODE_DO( 230 )
            dxfile_get_double(f, &e->extrDir.z);
    SWITCH_CODE_END(f);

    return e;
}


void dxf_ellipse_delete(DxfEllipse *e)
{
    g_return_if_fail(e!=NULL);
    g_free(e);
}


gchar *dxf_ellipse_print(const DxfEllipse *e)
{
    return g_strdup_printf("Center Point          : (%g, %g, %g)\n"
                           "Endpoint of Major Axis: (%g, %g, %g)\n"
                           "Ratio of minor axis to major axis: %g\n"
                           "Start Parameter       : %g\n"
                           "End Parameter         : %g\n",
                           e->centerPoint.x, e->centerPoint.y, e->centerPoint.z,
                           e->endPointMajorAxis.x, e->endPointMajorAxis.y, e->endPointMajorAxis.z,
                           e->ration,
                           e->startAngle,
                           e->endAngle);
}

/**
 * @brief dxf_ellipse_calc_endpoint_pi 以dxf坐标轴计算长轴端点相对于x轴的角度，角度方向为逆时针,比X轴开始算起
 * @param e     指向椭圆
 * @return      成功返回相对于x轴的角度
 */
gdouble dxf_ellipse_calc_endpoint_pi(const DxfEllipse *e)
{
    g_return_val_if_fail( e != NULL, 0);

    const DxfPointData *p = &e->endPointMajorAxis;
    gdouble pi = 0;

    if ( fabs(p->x) < 1e-15 ) {
        if ( p->y < -(1e-15) ) {
            pi = 3*M_PI/2;
        } else if ( p->y > 1e-15 ) {
            pi = M_PI/2;
        }
        return pi;
    }

    pi = atan(fabs(p->y/p->x));

    if ( p->x > 1e-15 ) {
        if (p->y > 1e-15) {
            /*第一象区*/
            pi = pi;
        } else if ( p->y < -1e-15 ) {
            /*第四象区*/
            pi = 2*M_PI - pi;
        }
    } else if (p->x < -1e-15) {
        if (p->y > 1e-15) {
            /*第二象区*/
            pi = M_PI - pi;
        } else if (p->y < -1e-15) {
            /*第三象区*/
            pi = M_PI + pi;
        }
    }

    return pi;
}

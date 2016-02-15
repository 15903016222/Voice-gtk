#include "entities_spline.h"

static inline DxfSpline *spline_new_item()
{
    DxfSpline *spline;
    spline = g_malloc0(sizeof(DxfSpline));

    spline->knotTolerance = 0.0000001;
    spline->ctrlPointTolerance = 0.0000001;
    spline->fitTolerance = 0.0000000001;

    return spline;
}

DxfSpline *dxf_spline_parse(Dxfile *f)
{
    DxfSpline *e = NULL;
    gdouble *knotValue = NULL;
    DxfPointData *ctrlPoint = NULL;

    e = spline_new_item();

    SWITCH_CODE_BEGIN(f);
    SWITCH_CODE_DO( 210 )
            dxfile_get_double(f, &e->vector.x);
    SWITCH_CODE_DO( 220 )
            dxfile_get_double(f, &e->vector.y);
    SWITCH_CODE_DO( 230 )
            dxfile_get_double(f, &e->vector.z);
    SWITCH_CODE_DO( 70 )
            dxfile_get_int16(f, &e->flag);
    SWITCH_CODE_DO( 71 )
            dxfile_get_int16(f, &e->degree);
    SWITCH_CODE_DO( 72 )
        dxfile_get_int16(f, &e->knotsNum);
    SWITCH_CODE_DO( 73 )
            dxfile_get_int16(f, &e->ctrlPointsNum);
    SWITCH_CODE_DO( 74 )
            dxfile_get_int16(f, &e->fitPointsNum);
    SWITCH_CODE_DO( 42 )
            dxfile_get_double(f, &e->knotTolerance);
    SWITCH_CODE_DO( 43 )
            dxfile_get_double(f, &e->ctrlPointTolerance);
    SWITCH_CODE_DO( 44 )
            dxfile_get_double(f, &e->fitTolerance);
    SWITCH_CODE_DO( 40 ) {
        if ( NULL == knotValue ) {
            if (e->knotsNum > 0) {
                knotValue = g_malloc0(sizeof(gdouble) * e->knotsNum);
                e->knotValues = knotValue;
                dxfile_get_double(f, knotValue);
            } else {
                c_log_warning("Havent' got the number of knot");
            }
        } else if ( (gint16)((knotValue - e->knotValues)/sizeof(gdouble)) < e->knotsNum ) {
            ++knotValue;
            dxfile_get_double(f, knotValue);
        } else {
            c_log_warning("The array of knot is overflow");
        }
    }
    SWITCH_CODE_DO( 10 ) {
        if ( NULL == ctrlPoint) {
            if (e->ctrlPointsNum > 0) {
                ctrlPoint = g_malloc0(sizeof(DxfPointData)*e->ctrlPointsNum);
                e->ctrlPoints = ctrlPoint;
                dxfile_get_double(f, &ctrlPoint->x);
            } else {
                c_log_warning("Haven't got the number of control point");
            }
        } else if ((gint16)((ctrlPoint - e->ctrlPoints)/sizeof(DxfPointData)) < e->ctrlPointsNum){
            ++ctrlPoint;
            dxfile_get_double(f, &ctrlPoint->x);
        } else {
            c_log_warning("The array of control point is overflow");
            dxfile_goto_nextline(f);
        }
    }
    SWITCH_CODE_DO( 20 )
            dxfile_get_double(f, &ctrlPoint->y);
    SWITCH_CODE_DO( 30 )
            dxfile_get_double(f, &ctrlPoint->z);
    SWITCH_CODE_END(f);

    return e;
}

void dxf_spline_delete(DxfSpline *e)
{
    g_return_if_fail( e != NULL );

    g_free(e->knotValues);
    g_free(e->ctrlPoints);
    g_free(e);
}

gchar *dxf_spline_print(DxfSpline *e)
{
    GString *msg = NULL;
    int i = 0;
    msg = g_string_sized_new(1024);
    g_string_append_printf(msg,
                           "Normal Vector              : (%g, %g, %g)\n"
                           "Spline Flag                : %hd\n"
                           "Degree of the spline curve : %hd\n"
                           "Number of Knots            : %hd\n"
                           "Number of Control Points   : %hd\n"
                           "Number of Fit Points       : %hd\n"
                           "Knot Tolerance             : %g\n"
                           "Control-point Tolerance    : %g\n"
                           "Fit Tolerance              : %g\n",
                           e->vector.x, e->vector.y, e->vector.z,
                           e->flag,
                           e->degree,
                           e->knotsNum,
                           e->ctrlPointsNum,
                           e->fitPointsNum,
                           e->knotTolerance,
                           e->ctrlPointTolerance,
                           e->fitTolerance);

    g_string_append(msg, "Knots Value :\n");
    for (i = 0; i < e->knotsNum; ++i) {
        g_string_append_printf(msg, " [%02d] %g\n", i+1, e->knotValues[i]);
    }

    g_string_append(msg, "Control Points Value :\n");
    for (i = 0; i < e->ctrlPointsNum; ++i) {
        g_string_append_printf(msg, " [%02d] : (%g, %g, %g)\n",
                               i+1,
                               e->ctrlPoints[i].x,
                               e->ctrlPoints[i].y,
                               e->ctrlPoints[i].z);
    }

    return g_string_free(msg, FALSE);
}

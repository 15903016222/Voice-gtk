/**
 * File: dxf_paint.c
 * Desc: 根据Dxf描绘图形
 * Author: JakeYang
 * Log: 2015-10-16
 */

#include "dxf_paint.h"
#include <math.h>

static void _paint_set_dash(cairo_t *cr, gdouble scale, const DxfEntity *e);    /*设置线型*/
static inline void _paint_line(cairo_t *cr, const DxfLine *l);                  /*画直线*/
static void _paint_arc(cairo_t *cr, const DxfArc *arc);                         /*画圆弧*/
static inline void _paint_circle(cairo_t *cr, const DxfCircle *c);              /*画圆*/
static void _paint_ellipse(cairo_t *cr, const DxfEllipse *e);                   /*画椭圆*/
static void _paint_lwpolyline(cairo_t *cr, const DxfLWPolyline *l);             /*折线*/
static inline void _paint_point(cairo_t *cr, gdouble scale , const DxfPoint *p);/*画点*/
static void _paint_mtext(cairo_t *cr, const DxfMtext *t);     /*画文字*/

void dxf_paint(const Dxf *d, cairo_t *cr, gdouble scalex, gdouble scaley)
{
    g_return_if_fail( d != NULL );
    g_return_if_fail( d->entities != NULL );
    g_return_if_fail( cr != NULL );

    DxfEntities *entities = d->entities;
    DxfEntity *entity = NULL;

    entities = d->entities;

    dxf_entities_foreach(entities, entity) {
        cairo_save(cr);
        _paint_set_dash(cr, scalex, entity);
        if ( entity_is_line(entity) ) {
            _paint_line(cr, entity->spData);
        } else if ( entity_is_arc(entity) ) {
            _paint_arc(cr, entity->spData);
        } else if ( entity_is_circle(entity) ) {
            _paint_circle(cr, entity->spData);
        } else if ( entity_is_ellipse(entity) ) {
            _paint_ellipse(cr, entity->spData);
        } else if ( entity_is_lwpolyline(entity) ) {
            _paint_lwpolyline(cr, entity->spData);
        } else if ( entity_is_point(entity) ) {
            _paint_point(cr, scalex, entity->spData);
        } else if ( entity_is_mtext(entity) ) {
            _paint_mtext(cr, entity->spData);
        }
        cairo_restore(cr);
    }

}

static void _paint_set_dash(cairo_t *cr, gdouble scale, const DxfEntity *e)
{
    gdouble dashed[4] = {0};
    gint num = 4;
    gint multi = 4;

    if ( entity_linetype_is_center(e)) {
        dashed[1] = 20 / scale;
    } else if ( entity_linetype_is_center2(e) ) {
        dashed[1] = 20 / scale;
        multi = 3;
    } else if ( entity_linetype_is_centerx2(e) ) {
        dashed[1] = 40 / scale;
    }else {
        return;
    }

    dashed[0] = dashed[1] * multi ;
    dashed[2] = dashed[1];
    dashed[3] = dashed[1];

    cairo_set_dash(cr, dashed, num, 0);
}

static inline void _paint_line(cairo_t *cr, const DxfLine *l)
{
    g_return_if_fail( l != NULL );
    cairo_move_to(cr, l->startPoint.x, -l->startPoint.y);
    cairo_line_to(cr, l->endPoint.x,   -l->endPoint.y);
    cairo_stroke(cr);
}

static void _paint_arc(cairo_t *cr, const DxfArc *arc)
{
    g_return_if_fail( arc != NULL );

    /* dxf 与 gtk 计算角度的方向是相反的，dxf是逆时针， gtk是顺时针, 且 dxf保存是角度值，gtk需要ＰＩ值*/
    gdouble startAngle = (360 - arc->startAngle) * 2*M_PI / 360;
    gdouble endAngle = (360 - arc->endAngle) * 2*M_PI / 360;

    if ( endAngle < startAngle ) {
        endAngle += 2*M_PI;
    }

    cairo_arc_negative(cr,
                       arc->centerPoint.x * arc->extrDir.z,
                       -arc->centerPoint.y,
                       arc->radius,
                       startAngle,
                       endAngle);

    cairo_stroke(cr);
}

static inline void _paint_circle(cairo_t *cr, const DxfCircle *c)
{
    g_return_if_fail( c != NULL );
    cairo_arc(cr,
              c->centerPoint.x * c->extrDir.z,
              -c->centerPoint.y,
              c->radius,
              0, 2*M_PI);
    cairo_stroke(cr);
}

static void _paint_ellipse(cairo_t *cr, const DxfEllipse *e)
{
    g_return_if_fail( e != NULL );
    gdouble radius;
    gdouble pi = 0;
    gdouble startAngle = 0;
    gdouble endAngle = 0;

    /*计算长半轴半径*/
    radius = sqrt(
                pow((e->endPointMajorAxis.x),2)
                + pow((e->endPointMajorAxis.y), 2)
                );

    cairo_translate(cr, e->centerPoint.x, -e->centerPoint.y);
    pi = dxf_ellipse_calc_endpoint_pi(e);
    /* dxf 与 gtk 计算角度的方向是相反的，dxf是逆时针， gtk是顺时针*/
    /* 转换dxf的起始和终止角度为gtk格式, dxf起始和终止角度是相对于长轴端点开始计算的 */

    startAngle = M_PI/2-(e->endAngle-M_PI);
    endAngle = M_PI/2-(e->startAngle-M_PI);
    cairo_rotate(cr, M_PI/2 - pi);                   //该函数要在cairo_scale函数前调用

    if (endAngle < startAngle) {
        endAngle += 2*M_PI;
    }

    cairo_scale(cr, e->ration, 1);

    c_debug("转换成GTK数据 **\n"
              "相对X轴弧/角度\t: %g\t%g\n"
              "起始弧/角度\t: %g\t%g\n"
              "结束弧/角度\t: %g\t%g\n",
              M_PI/2-pi,
              180*(M_PI/2-pi)/M_PI,
              startAngle,
              180*startAngle/M_PI,
              endAngle,
              180*endAngle/M_PI);

    cairo_arc(cr, 0, 0,
              radius,
              startAngle,
              endAngle);

    cairo_stroke(cr);
}

static void _paint_lwpolyline(cairo_t *cr, const DxfLWPolyline *l)
{
    g_return_if_fail( l != NULL );
    int i;

    for (i = 0; i < l->vertexNum; ++i) {
        cairo_line_to(cr, l->vertexes[i].x, -l->vertexes[i].y);
    }

    if (l->flag == LWPOLYLINE_CLOSE && l->vertexNum > 0) {
        cairo_move_to(cr,
                      l->vertexes[0].x * l->extrDir.z,
                      -l->vertexes[0].y);
        cairo_line_to(cr,
                      l->vertexes[l->vertexNum-1].x,
                      -l->vertexes[l->vertexNum-1].y);
    }

    cairo_stroke(cr);
}

static inline void _paint_point(cairo_t *cr, gdouble scale , const DxfPoint *p)
{
    g_return_if_fail( p != NULL );
    cairo_arc(cr, p->data.x, -p->data.y, 1/scale, 0, 2*M_PI);
    cairo_fill(cr);
}

static inline void _paint_mtext(cairo_t *cr, const DxfMtext *t)
{
    g_return_if_fail( t != NULL );
    cairo_move_to(cr,
                  t->insertionPoint.x,
                  -t->insertionPoint.y);
    cairo_show_text(cr, t->str->str);
}

static gdouble units[] = {
    1,              /*0 = 无单位*/
    25.4,           /*1 = 英寸*/
    25.4*12,        /*2 = 英尺*/
    25.4*12*5280,   /*3 = 英里*/
    1,              /*4 = 毫米*/
    1e1,            /*5 = 厘米*/
    1e3,            /*6 = 米*/
    1e6,            /*7 = 千米*/
    25.4e-06,       /*8 = 微英寸*/
    25.4e-03,       /*9 = 密耳*/
    25.4*36,        /*10 = 码*/
    1e-7,           /*11 = 埃*/
    1e-6,           /*12 = 纳米*/
    1e-3,           /*13 = 微米*/
    1e2,            /*14 = 分米*/
    1e4,            /*15 = 十米*/
    1e5,            /*16 = 百米*/
    1,              /*17 = 百万公里, 单位过大，不作计算*/
    1,              /*18 = 天文单位， 单位过大， 不作计算*/
    1,              /*19 = 光年， 单位过大，不作计算*/
    1               /*20 = 秒差距， 单位过大， 不作计算*/
};

gdouble unit_conversion(DxfUnits s, DxfUnits t)
{
    return units[s] / units[t];
}

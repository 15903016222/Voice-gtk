/*
 * draw_overlay.c
 *
 *  Created on: 2012-12-6
 *      Author: wolflord
 */

#include <cairo.h>
#include "../drawui.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "../lzk/tofd.h"

void DrawCursorASCAN(GtkWidget *widget , gpointer data , cairo_t *cr)
{
	int w,h;
	char *str = NULL;
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	unsigned char grp = prule->group ;
    double _nTmpX  ;
    double _nTmpY  ;
	gtk_widget_get_size_request (widget, &w, &h);
	int _nMark = prule->mask ;
	if(_nMark & 0x1)  h += 20 ;
	if(_nMark & 0x2)  w += 20 ;
	if(_nMark & 0x4)  w += 10 ;
	if(_nMark & 0x8)  w += 20 ;
	if(_nMark & 0x10) h += 15 ;

	double per_reference = GROUP_VAL_POS(grp , per_reference)/10000.0 ;
	double per_measure = GROUP_VAL_POS(grp , per_measure)/10000.0 ;
	double u_reference = GROUP_VAL_POS(grp , u_reference)/1000.0 ;
	double u_measure = GROUP_VAL_POS(grp , u_measure)/1000.0 ;
	double sample_start = get_group_val (get_group_by_id (pp->p_config, grp), GROUP_START) / 1000.0 ;
	double sample_range = get_group_val (get_group_by_id (pp->p_config, grp), GROUP_RANGE) / 1000.0;
	double velocity = get_group_val (get_group_by_id (pp->p_config, grp), GROUP_VELOCITY) / 200000.0;

	double current_angle = TMP(current_angle[grp]);
	double cos_current_angle = cos(current_angle) ;
	cairo_set_line_width (cr, 0.5);
	//*************************************************************************************
	//*************************************************************************************
	/* 参考光标高度 ％(r) */
	if(GROUP_VAL_POS(grp , ut_unit)==UT_UNIT_TRUE_DEPTH)
	{
		sample_start *= cos_current_angle ;
		sample_range *= cos_current_angle ;
	}
	cairo_set_source_rgba(cr,1.0,0.5,0.5,1.0);/*红色cursor*/
	_nTmpY = (int)((h-20)*(1.0-per_reference))  + 0.5 ;
	cairo_move_to (cr, 20, _nTmpY);
	cairo_line_to (cr, w-30, _nTmpY);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.93,0.945,0.412,1.0);/*显示当前值的黄色框*/
	cairo_rectangle(cr,21, _nTmpY - 14, 25 , 13);
	cairo_fill (cr);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
	cairo_move_to(cr,23, _nTmpY - 4);
	str=g_strdup_printf("%.1f", per_reference * 100.0);
	cairo_show_text(cr,str);
	cairo_stroke(cr);
	g_free (str);
	/* 测量光标高度 %(m) */
	cairo_set_source_rgba(cr,0.5,1.0,0.5,1.0);/*绿色cursor*/
	_nTmpY = (int)((h-20)*(1.0-per_measure))  + 0.5 ;
	cairo_move_to (cr, 20, _nTmpY);
	cairo_line_to (cr, w-30, _nTmpY);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.93,0.945,0.412,1.0);/*显示当前值的黄色框*/
	cairo_rectangle(cr , 21 , _nTmpY - 14 , 25 , 13);
	cairo_fill (cr);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
	cairo_move_to(cr , 23 , _nTmpY - 4);
	str=g_strdup_printf("%.1f", per_measure * 100.0);
	cairo_show_text(cr,str);
	cairo_stroke(cr);
	g_free (str);
	cairo_set_source_rgba(cr,1.0,0.5,0.5,1.0);/*红色cursor*/
	/* 当前值除以最大值再乘以窗口宽度*/
	_nTmpX = 20.5 + (int)((w-50) * (u_reference - sample_start) / (sample_range))  ;
	cairo_move_to (cr, _nTmpX, 0);
	cairo_line_to (cr, _nTmpX, h-20);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.84,0.67,0.945,1.0);/*显示当前值的紫色框*/
	cairo_rectangle(cr, _nTmpX + 1, 0, 30, 13);
	cairo_fill (cr);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
	cairo_move_to(cr, _nTmpX + 3,10);

	if(GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_TIME)
		str=g_strdup_printf("%.2f",u_reference);
	else
	{
		if(get_unit(pp->p_config)==UNIT_MM)
			str=g_strdup_printf("%.2f",u_reference * velocity);
		else
			str=g_strdup_printf("%.2f",u_reference * 0.03937 * velocity);
	}
	cairo_show_text(cr,str);
	cairo_stroke(cr);
	g_free (str);
	cairo_set_source_rgba(cr,0.5,1.0,0.5,1.0);/*绿色cursor*/
	/* 当前值除以最大值再乘以窗口宽度 */
	_nTmpX = 20.5 + (int)((w-50)*(u_measure-sample_start)/(sample_range))  ;
	cairo_move_to (cr, _nTmpX, 0);
	cairo_line_to (cr, _nTmpX, h-20);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.84,0.67,0.945,1.0);/*显示当前值的紫色框*/
	cairo_rectangle(cr,_nTmpX + 1, 0, 30, 13);
	cairo_fill (cr);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
	cairo_move_to(cr, _nTmpX + 3,10);

	if(GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_TIME)
		str=g_strdup_printf("%.2f",u_measure);
	else
	{
		if(get_unit(pp->p_config)==UNIT_MM)
			str=g_strdup_printf("%.2f",u_measure * velocity);
		else
			str=g_strdup_printf("%.2f",u_measure * 0.03937 * velocity);
	}
	cairo_show_text(cr,str);
	cairo_stroke(cr);
	g_free (str);

}

void DrawCursorASCANR(GtkWidget *widget , gpointer data , cairo_t *cr)
{
	int w,h;
	char *str = NULL;
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	unsigned char grp = prule->group ;
    double _nTmpX  ;
    double _nTmpY  ;
	gtk_widget_get_size_request (widget, &w, &h);
	int _nMark = prule->mask ;
	if(_nMark & 0x1)  h += 20 ;
	if(_nMark & 0x2)  w += 20 ;
	if(_nMark & 0x4)  w += 10 ;
	if(_nMark & 0x8)  w += 20 ;
	if(_nMark & 0x10) h += 15 ;
	double min_angle ;
	double max_angle ;
	double per_reference = GROUP_VAL_POS(grp , per_reference)/10000.0 ;
	double per_measure = GROUP_VAL_POS(grp , per_measure)/10000.0 ;
	double u_reference = GROUP_VAL_POS(grp , u_reference)/1000.0 ;
	double u_measure = GROUP_VAL_POS(grp , u_measure)/1000.0 ;
	double sample_start = get_group_val (get_group_by_id (pp->p_config, grp), GROUP_START) / 1000.0 ;
	double sample_range = get_group_val (get_group_by_id (pp->p_config, grp), GROUP_RANGE) / 1000.0;
	double velocity = get_group_val (get_group_by_id (pp->p_config, grp), GROUP_VELOCITY) / 200000.0;
	cairo_set_line_width (cr, 0.5);
	//*************************************************************************************
	_nTmpX = 20.5 + (int)((w-50) * per_reference) ;
	cairo_set_source_rgba(cr,1.0,0.5,0.5,1.0);/*红色cursor*/
	cairo_move_to (cr, _nTmpX , 0);
	cairo_line_to (cr, _nTmpX , h -20 );
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.93,0.945,0.412,1.0);/*显示当前值的黄色框*/
	cairo_rectangle(cr, _nTmpX + 1 ,1 , 25 , 13);
	cairo_fill (cr);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
	cairo_move_to(cr , _nTmpX + 3 , 11);
	str=g_strdup_printf("%.1f", per_reference * 100.0);
	cairo_show_text(cr,str);
	cairo_stroke(cr);
	g_free (str);
	/* 测量光标高度 %(m) */
	_nTmpX = 20.5 + (int)((w-50) * per_measure);
	cairo_set_source_rgba(cr,0.5,1.0,0.5,1.0);/*绿色cursor*/
	cairo_move_to (cr, _nTmpX , 0);
	cairo_line_to (cr, _nTmpX , h -20 );
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.93,0.945,0.412,1.0);/*显示当前值的黄色框*/
	cairo_rectangle(cr, _nTmpX + 1  ,1 , 25 , 13);
	cairo_fill (cr);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
	cairo_move_to(cr, _nTmpX + 3 , 11 );
	str=g_strdup_printf("%.1f", per_measure * 100.0);
	cairo_show_text(cr, str);
	cairo_stroke(cr);
	g_free (str);
	if(GROUP_VAL_POS(grp , ut_unit)==UT_UNIT_TRUE_DEPTH)
	{
		if(LAW_VAL_POS(grp, Focal_type) == 0)
		{
			if(LAW_VAL_POS(grp, Angle_min) * LAW_VAL_POS(grp, Angle_max)<0)
				min_angle = 0.0;
			else
				min_angle = MIN(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
			max_angle = MAX(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
		}
		else
		{
			max_angle = min_angle = LAW_VAL_POS(grp, Angle_min) * G_PI / 18000.0 ;
		}
		sample_range  = (sample_start + sample_range) * cos(min_angle);
		sample_start  *= cos(max_angle);
		sample_range -= sample_start ;
	}
	cairo_set_source_rgba(cr,1.0,0.5,0.5,1.0);/*红色cursor*/
	/* 当前值除以最大值再乘以窗口宽度*/
	_nTmpY = (int)((h-20)*(u_reference-sample_start)/(sample_range)) + 0.5 ;
	cairo_move_to (cr, 20,  _nTmpY);
	cairo_line_to (cr, w-30, _nTmpY);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.84,0.67,0.945,1.0);/*显示当前值的紫色框*/
	cairo_rectangle(cr,21, _nTmpY - 14,30,13);
	cairo_fill (cr);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
	cairo_move_to(cr,23, _nTmpY - 4 );
	if(get_unit(pp->p_config)==UNIT_MM)
		str=g_strdup_printf("%.2f",u_reference * velocity);
	else
		str=g_strdup_printf("%.2f",u_reference * 0.03937 * velocity);
	cairo_show_text(cr,str);
	cairo_stroke(cr);
	g_free (str);
	cairo_set_source_rgba(cr,0.5,1.0,0.5,1.0);/*绿色cursor*/
	/* 当前值除以最大值再乘以窗口宽度*/
	cairo_move_to (cr, 20, (h-20)*(u_measure-sample_start)/(sample_range) );
	cairo_line_to (cr, w-30, (h-20)*(u_measure-sample_start)/(sample_range) );
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.84,0.67,0.945,1.0);/*显示当前值的紫色框*/
	cairo_rectangle(cr,21, (h-20)*(u_measure-sample_start)/(sample_range)-14,30,13);
	cairo_fill (cr);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
	cairo_move_to(cr,23, (h-20)*(u_measure-sample_start)/(sample_range)-4 );
	if(get_unit(pp->p_config)==UNIT_MM)
		str=g_strdup_printf("%.2f",u_measure * velocity);
	else
		str=g_strdup_printf("%.2f",u_measure * 0.03937 * velocity);
	cairo_show_text(cr,str);
	cairo_stroke(cr);
	g_free (str);
}

void DrawCursorBSCAN(GtkWidget *widget , gpointer data , cairo_t *cr)
{
	int w,h;
	char *str = NULL;
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	unsigned char grp = prule->group ;
    double _nTmpX  , _nTmpY ;

	gtk_widget_get_size_request (widget, &w, &h);

	int _nMark = prule->mask ;
	if(_nMark & 0x1)  h += 20 ;
	if(_nMark & 0x2)  w += 20 ;
	if(_nMark & 0x4)  w += 10 ;
	if(_nMark & 0x8)  w += 20 ;
	if(_nMark & 0x10) h += 15 ;

	double _nVStop      = prule->hmin1  ;
	double _nVStart     = prule->hmax1  ;

	double u_reference = GROUP_VAL_POS(grp , u_reference)/1000.0 ;
	double u_measure   = GROUP_VAL_POS(grp , u_measure)/1000.0 ;
	double s_reference = GROUP_VAL_POS(grp , s_reference)/100.0 ;
	double s_measure   = GROUP_VAL_POS(grp , s_measure)/100.0 ;
	double rate = 1.0;
	if(prule->h1_unit == UNIT_S)
	{
		rate =  GROUP_VAL_POS(0 , prf1) * get_area_scanresolution (pp->p_config) / 10000.0;
		s_reference = s_reference /rate ;
		s_measure   = s_measure / rate ;
	}
	GROUP* pGroup = get_group_by_id (pp->p_config, grp);
	double sample_start = get_group_val (pGroup, GROUP_START) / 1000.0 ;
	double sample_range = get_group_val (pGroup, GROUP_RANGE) / 1000.0;
	double velocity = get_group_val (pGroup, GROUP_VELOCITY) / 200000.0;
	double current_angle = TMP(current_angle[grp]);
	double cos_current_angle = cos(current_angle) ;
	cairo_set_line_width (cr, 0.5);
	//*************************************************************************************
	if(GROUP_VAL_POS(grp , ut_unit)==UT_UNIT_TRUE_DEPTH)
	{
		sample_start *= cos_current_angle ;
		sample_range *= cos_current_angle ;
	}

    double tofdRefLine = gTofdS[grp].refLine / rate;
	struct TofdStruct tofd;
	int i;
	double xx[NUMOFPOINT];
	struct DefectStruct defect;

	if(get_group_val (pGroup, GROUP_TX_RX_MODE) == TOFD)//tofd
	{
		tofd.pcs = GROUP_VAL_POS(grp , field_distance[254]);//mm
		tofd.t0 = pGroup->wedge_delay / 1000.0;//pGroup->wedge.Probe_delay / 1000.0;//楔块延时 us
		tofd.c = get_group_val (pGroup, GROUP_VELOCITY) /100000.0 ;//mm/us
		tofd.mmPerPixel = (_nVStop - _nVStart) / (h - 20);//mm
		if(prule->h1_unit == UNIT_S)
		{
			tofd.mmPerPixel *= rate;
		}
	}

//参考线
	//ur线
	cairo_set_source_rgba(cr,1.0,0.5,0.5,1.0);/*红色cursor*/
	_nTmpX = (u_reference-sample_start)/sample_range  ;/* 当前值除以最大值再乘以窗口宽度*/
    gTofdS[grp].ur = _nTmpX ;
	_nTmpX = 20+(w-50)*_nTmpX;
	_nTmpX = (int)_nTmpX + 0.5  ;
	cairo_move_to (cr, _nTmpX, 0);
	cairo_line_to (cr, _nTmpX, h-20);
	cairo_stroke (cr);

	//ur线指示框
	cairo_set_source_rgba(cr,0.84,0.67,0.945,1.0);/*显示当前值的紫色框*/
	cairo_rectangle(cr, _nTmpX + 1 , 0 , 30 , 13);
	cairo_fill (cr);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
	cairo_move_to(cr , _nTmpX + 3 , 10);

	if(GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_TIME)
		str=g_strdup_printf("%.2f",u_reference);
	else
	{
		if(get_unit(pp->p_config)==UNIT_MM)
			str=g_strdup_printf("%.2f",u_reference * velocity);
		else
			str=g_strdup_printf("%.2f",u_reference * 0.03937 * velocity);
	}
	cairo_show_text(cr,str);
	cairo_stroke(cr);
	g_free (str);

	//sr线
	_nTmpY = (s_reference-_nVStart) / (_nVStop - _nVStart);
    gTofdS[grp].sr = _nTmpY;
	_nTmpY = (1 - _nTmpY) * (h - 20) ;
	_nTmpY = (int)_nTmpY + 0.5 ;
	cairo_set_source_rgba(cr,1.0,0.5,0.5,1.0);/*红色cursor*/
	cairo_set_line_width(cr, 0.5);
	cairo_move_to (cr, 20 , _nTmpY  );
	cairo_line_to (cr, 20 + w , _nTmpY  );
	cairo_stroke(cr);

	//sr线指示框
	cairo_set_source_rgba(cr,0.678,0.984,0.89,1.0);/*显示当前值的绿色框*/
	cairo_rectangle(cr,21, _nTmpY - 14,30,13);
	cairo_fill (cr);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
	cairo_move_to(cr,23, _nTmpY - 4 );
	str=g_strdup_printf("%.1f", s_reference );
	cairo_show_text(cr,str);
	cairo_stroke(cr);
	g_free (str);

	//tofd抛物线
	if(TOFD == get_group_val (pGroup, GROUP_TX_RX_MODE))//tofd
	{
		defect.t = GROUP_VAL_POS(grp , u_reference) / 1000.0;//us

		//depth指示框
		cairo_set_source_rgba(cr,0.84,0.67,0.945,1.0);/*显示当前值的紫色框*/
		cairo_rectangle(cr, _nTmpX + 1 , h-20-15 , 40 , 13);
		cairo_fill (cr);
		cairo_stroke (cr);
		cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
		cairo_move_to(cr , _nTmpX + 3 , h-20-5);

		double depth = GetDepthCal(grp ,defect.t, 0);
		if(MEASURE_DATA_ND - depth < 0.001)
		{
			str=g_strdup_printf("ND");
		}
		else
		{

			if(get_unit(pp->p_config)==UNIT_MM)
				str=g_strdup_printf("%.2f", depth );
			else
				str=g_strdup_printf("%.2f", depth  * 0.03937);
		}
		cairo_show_text(cr,str);
		cairo_stroke(cr);
		g_free (str);


		GetPointer(&tofd ,&defect ,xx ,NUMOFPOINT);
		int x,y;
		x = _nTmpX;
		y = _nTmpY;
		cairo_set_source_rgba(cr,1.0,0.5,0.5,1.0);/*红色cursor*/

		cairo_move_to (cr, x ,y);
		for(i = 0 ;i < NUMOFPOINT - 1 ;i++)
		{
			xx[i] = 20+(w-50)*(xx[i] - sample_start) / sample_range;
			cairo_line_to (cr, xx[i], y + i + 1);
			cairo_move_to (cr, xx[i], y + i + 1);
		}
		xx[i] = 20+(w-50)*(xx[i] - sample_start) / sample_range;
		cairo_line_to (cr, xx[i], y + i + 1);

		cairo_move_to (cr, x, y);
		for(i = 0 ;i < NUMOFPOINT - 1 ;i++)
		{
			cairo_line_to (cr, xx[i], y - i - 1);
			cairo_move_to (cr, xx[i], y - i - 1);
		}
		cairo_line_to (cr, xx[i], y - i - 1);
		cairo_stroke (cr);

		//ref line线
		_nTmpY = (tofdRefLine -_nVStart) / (_nVStop - _nVStart);
        gTofdS[grp].averagePos = _nTmpY;//从开始算的第几个像素
		_nTmpY = (1 - _nTmpY) * (h - 20) ;
		_nTmpY = (int)_nTmpY + 0.5 ;
		cairo_set_source_rgba(cr,1.0,1.0,0.5,1.0);/*黄色cursor*/
		cairo_set_line_width(cr, 0.5);
		cairo_move_to (cr, 20 , _nTmpY  );
		cairo_line_to (cr, 20 + w , _nTmpY  );
		cairo_stroke(cr);

		//ref line指示框
		cairo_set_source_rgba(cr,0.678,0.984,0.89,1.0);/*显示当前值的绿色框*/
		cairo_rectangle(cr,21, _nTmpY - 14,30,13);
		cairo_fill (cr);
		cairo_stroke (cr);
		cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
		cairo_move_to(cr,23, _nTmpY - 4 );
		str=g_strdup_printf("%.1f", tofdRefLine );
		cairo_show_text(cr,str);
		cairo_stroke(cr);
		g_free (str);
	}

//测量线
	//um线
	cairo_set_source_rgba(cr,0,0,1,1.0);/*蓝色cursor*/
	/* 当前值除以最大值再乘以窗口宽度 */
	_nTmpX = (u_measure-sample_start)/(sample_range)  ;
    gTofdS[grp].um = _nTmpX;
	_nTmpX = 20+(w-50)*_nTmpX;
	_nTmpX = (int)_nTmpX + 0.5  ;
	cairo_move_to (cr, _nTmpX, 0);
	cairo_line_to (cr, _nTmpX, h-20);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.84,0.67,0.945,1.0);/*显示当前值的紫色框*/
	cairo_rectangle(cr, _nTmpX + 1 , 0 , 30 , 13);
	cairo_fill (cr);
	cairo_stroke (cr);

	//um线指示框
	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
	cairo_move_to(cr , _nTmpX + 3 , 10);

	if(GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_TIME)
		str=g_strdup_printf("%.2f",u_measure);
	else
	{
		if(get_unit(pp->p_config)==UNIT_MM)
			str=g_strdup_printf("%.2f",u_measure * velocity);
		else
			str=g_strdup_printf("%.2f",u_measure * 0.03937 * velocity);
	}
	cairo_show_text(cr,str);
	cairo_stroke(cr);
	g_free (str);

	//sm线
	_nTmpY = (s_measure - _nVStart) / (_nVStop - _nVStart);
    gTofdS[grp].sm = _nTmpY;
	_nTmpY = (1 - _nTmpY) * (h - 20) ;
	_nTmpY = (int)_nTmpY + 0.5 ;
	cairo_set_source_rgba(cr,0,0,1,1.0);/*蓝色cursor*/
	cairo_set_line_width(cr, 0.5);
	cairo_move_to (cr, 20 , _nTmpY  );
	cairo_line_to (cr, 20 + w , _nTmpY  );
	cairo_stroke(cr);

	//sm线指示框
	cairo_set_source_rgba(cr,0.678,0.984,0.89,1.0);/*显示当前值的绿色框*/
	cairo_rectangle(cr,21, _nTmpY - 14,30,13);
	cairo_fill (cr);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
	cairo_move_to(cr,23, _nTmpY - 4 );
	str=g_strdup_printf("%.1f", s_measure );
	cairo_show_text(cr,str);
	cairo_stroke(cr);
	g_free (str);

	//tofd抛物线
	if(TOFD == get_group_val (pGroup, GROUP_TX_RX_MODE))//tofd
	{
		defect.t = GROUP_VAL_POS(grp , u_measure) / 1000.0;//us

		//depth指示框
		cairo_set_source_rgba(cr,0.84,0.67,0.945,1.0);/*显示当前值的紫色框*/
		cairo_rectangle(cr, _nTmpX + 1 , h-20-15 , 40 , 13);
		cairo_fill (cr);
		cairo_stroke (cr);
		cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
		cairo_move_to(cr , _nTmpX + 3 , h-20-5);

		double depth = GetDepthCal(grp ,defect.t, 1);
		if(MEASURE_DATA_ND - depth < 0.001)
		{
			str=g_strdup_printf("ND");
		}
		else
		{

			if(get_unit(pp->p_config)==UNIT_MM)
				str=g_strdup_printf("%.2f", depth );
			else
				str=g_strdup_printf("%.2f", depth  * 0.03937);
		}

		cairo_show_text(cr,str);
		cairo_stroke(cr);
		g_free (str);

		GetPointer(&tofd ,&defect ,xx ,NUMOFPOINT);
		int x,y;
		x = _nTmpX;
		y = _nTmpY;
		cairo_set_source_rgba(cr,0,0,1,1.0);/*蓝色cursor*/
			//cairo_set_source_rgba(cr,1.0,1.0,0.1,1.0);/*黄色cursor*/
	//cairo_set_source_rgba(cr,0.5,1.0,0.5,1.0);/*绿色cursor*/

		cairo_move_to (cr, x ,y);
		for(i = 0 ;i < NUMOFPOINT - 1 ;i++)
		{
			xx[i] = 20+(w-50)*(xx[i] - sample_start) / sample_range;
			cairo_line_to (cr, xx[i], y + i + 1);
			cairo_move_to (cr, xx[i], y + i + 1);
		}
		xx[i] = 20+(w-50)*(xx[i] - sample_start) / sample_range;
		cairo_line_to (cr, xx[i], y + i + 1);

		cairo_move_to (cr, x, y);
		for(i = 0 ;i < NUMOFPOINT - 1 ;i++)
		{
			cairo_line_to (cr, xx[i], y - i - 1);
			cairo_move_to (cr, xx[i], y - i - 1);
		}
		cairo_line_to (cr, xx[i], y - i - 1);

		cairo_stroke (cr);
	}
}

void DrawCursorCSCAN(GtkWidget *widget , gpointer data , cairo_t *cr)
{
	int w,h;
	char *str = NULL;
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	unsigned char grp = prule->group ;
    double _nTmpX  ;
	gtk_widget_get_size_request (widget, &w, &h);

	int _nMark = prule->mask ;
	if(_nMark & 0x1)  h += 20 ;
	if(_nMark & 0x2)  w += 20 ;
	if(_nMark & 0x4)  w += 10 ;
	if(_nMark & 0x8)  w += 20 ;
	if(_nMark & 0x10) h += 15 ;
	double s_reference = GROUP_VAL_POS(grp , s_reference)/100.0 ;
	double s_measure = GROUP_VAL_POS(grp , s_measure)/100.0 ;
	double rate =  GROUP_VAL_POS(0 , prf1) * get_area_scanresolution (pp->p_config) / 10000.0;
	double scan_start ; // = get_area_scanstart (pp->p_config)/1000.0 ;
	double scan_end   ; // = get_area_scanend (pp->p_config)/1000.0 ;
	cairo_set_line_width (cr, 0.5);
	//*************************************************************************************
	scan_start  = get_area_scanstart (pp->p_config)/1000.0 ;
	scan_end    = get_area_scanend (pp->p_config)/1000.0 ;
	if(get_inspec_source(pp->p_config))
	{
			scan_start = prule->wmin1 ;
			scan_end   = prule->wmax1 ;
		if(get_unit(pp->p_config)==UNIT_INCH)
		{
			s_reference *= 0.03937   ;
			s_measure   *= 0.03937   ;
		}
	}
	else
	{
			scan_start = 0 ; //prule->wmin1 * GROUP_VAL_POS(0 , prf1) * get_area_scanresolution (pp->p_config)/1000.0 ;
			scan_end   = prule->wmax1 * GROUP_VAL_POS(0 , prf1) * get_area_scanresolution (pp->p_config)/10000.0;
		if(get_unit(pp->p_config)==UNIT_INCH)
		{
			scan_start  *= 0.03937   ;
			scan_end    *= 0.03937   ;
			s_reference *= 0.03937   ;
			s_measure   *= 0.03937   ;
		}
	}
	cairo_set_source_rgba(cr,1.0,0.5,0.5,1.0);/*红色cursor*/
	_nTmpX = 20.5 + (int)((w-50)*(s_reference - scan_start)/(scan_end -scan_start) ) ;
	cairo_move_to (cr, _nTmpX , 0);
	cairo_line_to (cr, _nTmpX , h-20);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.678,0.984,0.89,1.0);/*显示当前值的绿色框*/
	cairo_rectangle(cr,  _nTmpX + 1 , 0 , 30 , 13);
	cairo_fill (cr);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
	cairo_move_to(cr,_nTmpX + 3, 10);

	if(prule->w_unit == UNIT_S )// h1_unit == UNIT_S)
	{
		s_reference = s_reference /rate ;
	}
	else
	{
		if(get_unit(pp->p_config)==UNIT_INCH)
			s_reference *= 0.03937  ;
	}
	str=g_strdup_printf("%.1f",s_reference) ;

	cairo_show_text(cr,str);
	cairo_stroke(cr);
	g_free (str);

	_nTmpX = 20.5 + (int)((w-50)*(s_measure - scan_start)/(scan_end - scan_start) ) ;
	cairo_set_source_rgba(cr,0.5,1.0,0.5,1.0);/*绿色cursor*/
	/* 当前值除以最大值再乘以窗口宽度 */
	cairo_move_to (cr, _nTmpX , 0);
	cairo_line_to (cr, _nTmpX , h-20);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.678,0.984,0.89,1.0);/*显示当前值的绿色框*/
	cairo_rectangle(cr,  _nTmpX + 1 , 0 , 30 , 13);
	cairo_fill (cr);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
	cairo_move_to(cr,_nTmpX + 3, 10);

	if(prule->w_unit == UNIT_S )// h1_unit == UNIT_S)
	{
		s_measure = s_measure /rate ;
	}
	else
	{
		if(get_unit(pp->p_config)==UNIT_INCH)
			s_measure *= 0.03937  ;
	}
	str=g_strdup_printf("%.1f",s_measure) ;

	cairo_show_text(cr,str);
	cairo_stroke(cr);
	g_free (str);

}

void DrawCursorCSCANVert(GtkWidget *widget , gpointer data , cairo_t *cr)
{
	int w,h;
	char *str = NULL;
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	unsigned char grp = prule->group ;
    double _nTmpY;
	gtk_widget_get_size_request (widget, &w, &h);

	int _nMark = prule->mask ;
	if(_nMark & 0x1)  h += 20 ;
	if(_nMark & 0x2)  w += 20 ;
	if(_nMark & 0x4)  w += 10 ;
	if(_nMark & 0x8)  w += 20 ;
	if(_nMark & 0x10) h += 15 ;
	double s_reference = GROUP_VAL_POS(grp , s_reference)/100.0 ;
	double s_measure = GROUP_VAL_POS(grp , s_measure)/100.0 ;
	double rate =  GROUP_VAL_POS(0 , prf1) * get_area_scanresolution (pp->p_config) / 10000.0;
	double scan_start ; // = get_area_scanstart (pp->p_config)/1000.0 ;
	double scan_end   ; // = get_area_scanend (pp->p_config)/1000.0 ;
	cairo_set_line_width (cr, 0.5);
	//*************************************************************************************
	scan_start  = get_area_scanstart (pp->p_config)/1000.0 ;
	scan_end    = get_area_scanend (pp->p_config)/1000.0 ;
	if(get_inspec_source(pp->p_config))
	{
        scan_end = prule->hmin1 ;
        scan_start = prule->hmax1 ;
		if(get_unit(pp->p_config)==UNIT_INCH)
		{
			s_reference *= 0.03937   ;
			s_measure   *= 0.03937   ;
		}
	}
	else
	{
        scan_start = 0 ;
        scan_end   = prule->hmin1 * rate;
		if(get_unit(pp->p_config)==UNIT_INCH)
		{
			scan_start  *= 0.03937   ;
			scan_end    *= 0.03937   ;
			s_reference *= 0.03937   ;
			s_measure   *= 0.03937   ;
		}
	}
	cairo_set_source_rgba(cr,1.0,0.5,0.5,1.0);/*红色cursor*/
	_nTmpY = 0.5 + (h-20)*(1-(s_reference - scan_start)/(scan_end -scan_start));

	cairo_move_to (cr, 20, _nTmpY);
	cairo_line_to (cr, w+20, _nTmpY);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.678,0.984,0.89,1.0);/*显示当前值的绿色框*/
	cairo_rectangle(cr, 21, _nTmpY - 14, 30, 13);
	cairo_fill (cr);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
	cairo_move_to(cr, 23, _nTmpY - 4 );

	if(prule->h1_unit == UNIT_S )// h1_unit == UNIT_S)
	{
		s_reference = s_reference /rate ;
	}
	else
	{
		if(get_unit(pp->p_config)==UNIT_INCH)
			s_reference *= 0.03937  ;
	}
	str=g_strdup_printf("%.1f",s_reference) ;

	cairo_show_text(cr,str);
	cairo_stroke(cr);
	g_free (str);

	_nTmpY = 0.5 + (h-20)*(1-(s_measure - scan_start)/(scan_end -scan_start));
	cairo_set_source_rgba(cr,0.5,1.0,0.5,1.0);/*绿色cursor*/
	/* 当前值除以最大值再乘以窗口宽度 */
	cairo_move_to (cr, 20, _nTmpY);
	cairo_line_to (cr, 20+w, _nTmpY);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.678,0.984,0.89,1.0);/*显示当前值的绿色框*/
	cairo_rectangle(cr,  21, _nTmpY - 14, 30 , 13);
	cairo_fill (cr);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
	cairo_move_to(cr, 23, _nTmpY - 4);

	if(prule->h1_unit == UNIT_S )// h1_unit == UNIT_S)
	{
		s_measure = s_measure /rate ;
	}
	else
	{
		if(get_unit(pp->p_config)==UNIT_INCH)
			s_measure *= 0.03937  ;
	}
	str=g_strdup_printf("%.1f",s_measure) ;

	cairo_show_text(cr,str);
	cairo_stroke(cr);
	g_free (str);
}


void DrawCursorSSCAN(GtkWidget *widget , gpointer data , cairo_t *cr)
{
	int w,h;
	char *str = NULL;
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	unsigned char grp = prule->group ;
    double _nTmpX  ;
    double _nTmpY  ;
	gtk_widget_get_size_request (widget, &w, &h);
	int _nMark = prule->mask ;
	if(_nMark & 0x1)  h += 20 ;
	if(_nMark & 0x2)  w += 20 ;
	if(_nMark & 0x4)  w += 10 ;
	if(_nMark & 0x8)  w += 20 ;
	if(_nMark & 0x10) h += 15 ;
	double min_angle ;
	double max_angle ;
	double u_reference = GROUP_VAL_POS(grp , u_reference)/1000.0 ;
	double u_measure = GROUP_VAL_POS(grp , u_measure)/1000.0 ;
	double s_reference = GROUP_VAL_POS(grp , s_reference)/100.0 ;
	double s_measure = GROUP_VAL_POS(grp , s_measure)/100.0 ;
	double i_reference = GROUP_VAL_POS(grp , i_reference)/100.0 ;
	double i_measure =GROUP_VAL_POS(grp , i_measure)/100.0 ;
	double sample_start = get_group_val (get_group_by_id (pp->p_config, grp), GROUP_START) / 1000.0 ;
	double sample_range = get_group_val (get_group_by_id (pp->p_config, grp), GROUP_RANGE) / 1000.0;
	double velocity = get_group_val (get_group_by_id (pp->p_config, grp), GROUP_VELOCITY) / 200000.0;
	cairo_set_line_width (cr, 0.5);
	//*************************************************************************************
	if((GROUP_VAL_POS(grp , ut_unit)==UT_UNIT_SOUNDPATH)||(GROUP_VAL_POS(grp , ut_unit)==UT_UNIT_TIME))
	{
		cairo_set_source_rgba(cr,1.0,0.5,0.5,1.0);/*红色cursor*/
		/* 当前值除以最大值再乘以窗口宽度*/
		_nTmpX = 20.5 + (int)((w-50)*(u_reference-sample_start)/(sample_range)) ;
		cairo_move_to (cr, _nTmpX ,  0);
		cairo_line_to (cr, _nTmpX ,  h-20);
		cairo_stroke (cr);
		cairo_set_source_rgba(cr,0.84,0.67,0.945,1.0);/*显示当前值的紫色框*/
		cairo_rectangle(cr, _nTmpX + 1 , 0 , 30 , 13);
		cairo_fill (cr);
		cairo_stroke (cr);
		cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
		cairo_move_to(cr , _nTmpX + 3, 10);
		if(GROUP_VAL_POS(grp , ut_unit)==UT_UNIT_TIME)
			str=g_strdup_printf("%.2f",u_reference);
		else if(get_unit(pp->p_config)==UNIT_MM)
			str=g_strdup_printf("%.2f",u_reference * velocity);
		else
			str=g_strdup_printf("%.2f",u_reference * velocity * 0.03937);

		cairo_show_text(cr,str);
		cairo_stroke(cr);
		g_free (str);

		cairo_set_source_rgba(cr,0.5,1.0,0.5,1.0);/*绿色cursor*/
		/* 当前值除以最大值再乘以窗口宽度 */
		_nTmpX = 20.5 + (int)((w-50)*(u_measure-sample_start)/(sample_range)) ;
		cairo_move_to (cr, _nTmpX, 0);
		cairo_line_to (cr, _nTmpX, h-20);
		cairo_stroke (cr);
		cairo_set_source_rgba(cr,0.84,0.67,0.945,1.0);/*显示当前值的紫色框*/
		cairo_rectangle(cr, _nTmpX + 1, 0, 30, 13);
		cairo_fill (cr);
		cairo_stroke (cr);
		cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
		cairo_move_to(cr, _nTmpX + 3, 10);
		if(GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_TIME)
			str=g_strdup_printf("%.2f",u_measure);
		else if(get_unit(pp->p_config)==UNIT_MM)
			str=g_strdup_printf("%.2f",u_measure * velocity);
		else
			str=g_strdup_printf("%.2f",u_measure * velocity * 0.03937);
		cairo_show_text(cr,str);
		cairo_stroke(cr);
		g_free (str);
	}
	else
	{
		if(LAW_VAL_POS(grp, Focal_type) == 0)
		{
			if(LAW_VAL_POS(grp, Angle_min) * LAW_VAL_POS(grp, Angle_max)<0)
				min_angle = 0.0;
			else
				min_angle = MIN(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
			max_angle = MAX(abs(LAW_VAL_POS(grp, Angle_min)), abs(LAW_VAL_POS(grp, Angle_max))) * G_PI / 18000.0 ;
		}
		else
		{
			max_angle = min_angle = LAW_VAL_POS(grp, Angle_min) * G_PI / 18000.0 ;
		}
		sample_range  = (sample_start + sample_range) * cos(min_angle);
		sample_start  *= cos(max_angle);
		sample_range -= sample_start ;

		cairo_set_source_rgba(cr,1.0,0.5,0.5,1.0);/*红色cursor*/
		/* 当前值除以最大值再乘以窗口宽度*/
		_nTmpY = (h-20)*(u_reference-sample_start)/(sample_range)  ;
		_nTmpY = 0.5 + (int)_nTmpY ;
		cairo_move_to (cr, 20, _nTmpY );
		cairo_line_to (cr, w-30, _nTmpY );
		cairo_stroke (cr);
		cairo_set_source_rgba(cr,0.84,0.67,0.945,1.0);/*显示当前值的紫色框*/
		cairo_rectangle(cr, 21 , _nTmpY - 14, 30 , 13);
		cairo_fill (cr);
		cairo_stroke (cr);
		cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
		cairo_move_to(cr,23, _nTmpY - 4 );
		if(get_unit(pp->p_config)==UNIT_MM)
			str=g_strdup_printf("%.2f",u_reference * velocity);
		else
			str=g_strdup_printf("%.2f",u_reference * 0.03937 * velocity);
		cairo_show_text(cr,str);
		cairo_stroke(cr);
		g_free (str);
		cairo_set_source_rgba(cr,0.5,1.0,0.5,1.0);/*绿色cursor*/
		/* 当前值除以最大值再乘以窗口宽度*/
		_nTmpY = (h-20)*(u_measure-sample_start)/(sample_range)  ;
		_nTmpY = 0.5 + (int)_nTmpY ;
		cairo_move_to (cr, 20, _nTmpY );
		cairo_line_to (cr, w-30, _nTmpY );
		cairo_stroke (cr);
		cairo_set_source_rgba(cr,0.84,0.67,0.945,1.0);/*显示当前值的紫色框*/
		cairo_rectangle(cr, 21 , _nTmpY - 14, 30 , 13);
		cairo_fill (cr);
		cairo_stroke (cr);
		cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
		cairo_move_to(cr,23, _nTmpY - 4 );
		if(get_unit(pp->p_config) == UNIT_MM)
			str=g_strdup_printf("%.2f",u_measure * velocity);
		else
			str=g_strdup_printf("%.2f",u_measure * 0.03937 * velocity);
		cairo_show_text(cr,str);
		cairo_stroke(cr);
		g_free (str);
		cairo_set_source_rgba(cr,1.0,0.5,0.5,1.0);/*红色cursor*/
		/* 当前值除以最大值再乘以窗口宽度*/
		if(GROUP_VAL_POS(grp , skew_pos) == 1 || GROUP_VAL_POS(grp , skew_pos) == 3)
		{
			//_nTmpX =  20.5 + (int)((w-50)*(i_reference-pp->swmin)/(pp->swmax - pp->swmin));
			_nTmpX = 20.5 + (int)((w-50) * (i_reference-prule->wmin1) / (prule->wmax1 - prule->wmin1));
			cairo_move_to (cr, _nTmpX, 0);
			cairo_line_to (cr, _nTmpX, h-20);
			cairo_stroke (cr);
			cairo_set_source_rgba(cr,0.678,0.984,0.89,1.0);/*显示当前值的绿色框*/
			cairo_rectangle(cr, _nTmpX + 1, 0, 30 , 13);
			cairo_fill (cr);
			cairo_stroke (cr);
			cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
			cairo_move_to(cr, _nTmpX + 3,10);
			str=g_strdup_printf("%.2f",i_reference);
			cairo_show_text(cr,str);
			cairo_stroke(cr);
			g_free (str);
			cairo_set_source_rgba(cr,0.5,1.0,0.5,1.0);/*绿色cursor*/
			/* 当前值除以最大值再乘以窗口宽度 */
			//_nTmpX =  20.5 + (int)((w-50)*(i_measure-pp->swmin)/(pp->swmax - pp->swmin));
			_nTmpX = 20.5 + (int)((w-50) * (i_measure-prule->wmin1) / (prule->wmax1 - prule->wmin1));
			cairo_move_to (cr, _nTmpX , 0);
			cairo_line_to (cr, _nTmpX , h-20);
			cairo_stroke (cr);
			cairo_set_source_rgba(cr,0.678,0.984,0.89,1.0);/*显示当前值的绿色框*/
			cairo_rectangle(cr, _nTmpX + 1 , 0 , 30 , 13);
			cairo_fill (cr);
			cairo_stroke (cr);
			cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
			cairo_move_to(cr , _nTmpX + 3,10);
			str=g_strdup_printf("%.2f",i_measure);
			cairo_show_text(cr,str);
			cairo_stroke(cr);
			g_free (str);
		}
		else
		{
			_nTmpX = 20.5 + (int)((w-50) * (s_reference-prule->wmin1) / (prule->wmax1 - prule->wmin1));
			cairo_move_to (cr, _nTmpX, 0);
			cairo_line_to (cr, _nTmpX, h-20);
			cairo_stroke (cr);
			cairo_set_source_rgba(cr,0.039,0.835,0.827,1.0);/*显示当前值的lan色框*/
			cairo_rectangle(cr, _nTmpX + 1, 0, 30 , 13);
			cairo_fill (cr);
			cairo_stroke (cr);
			cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
			cairo_move_to(cr, _nTmpX + 3,10);
			str=g_strdup_printf("%.2f",s_reference);
			cairo_show_text(cr,str);
			cairo_stroke(cr);
			g_free (str);
			cairo_set_source_rgba(cr,0.5,1.0,0.5,1.0);/*绿色cursor*/
			/* 当前值除以最大值再乘以窗口宽度 */
			//_nTmpX =  20.5 + (int)((w-50)*(s_measure-pp->swmin)/(pp->swmax - pp->swmin));
			_nTmpX = 20.5 + (int)((w-50) * (s_measure-prule->wmin1) / (prule->wmax1 - prule->wmin1));
			cairo_move_to (cr, _nTmpX , 0);
			cairo_line_to (cr, _nTmpX , h-20);
			cairo_stroke (cr);
			cairo_set_source_rgba(cr,0.039,0.835,0.827,1.0);/*显示当前值的绿色框*/
			cairo_rectangle(cr, _nTmpX + 1 , 0 , 30 , 13);
			cairo_fill (cr);
			cairo_stroke (cr);
			cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
			cairo_move_to(cr , _nTmpX + 3,10);
			str=g_strdup_printf("%.2f", s_measure);
			cairo_show_text(cr,str);
			cairo_stroke(cr);
			g_free (str);
		}
	}
}

void DrawCursorTOFDB(GtkWidget *widget , gpointer data , cairo_t *cr)
{
	int w,h;
	char *str = NULL;
	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	unsigned char grp = prule->group ;
    double _nTmpX  , _nTmpY ;

	gtk_widget_get_size_request (widget, &w, &h);

	int _nMark = prule->mask ;
	if(_nMark & 0x1)  h += 20 ;
	if(_nMark & 0x2)  w += 20 ;
	if(_nMark & 0x4)  w += 10 ;
	if(_nMark & 0x8)  w += 20 ;
	if(_nMark & 0x10) h += 15 ;

	double _nVStart     = prule->wmin1  ;
	double _nVStop      = prule->wmax1  ;

	double u_reference = GROUP_VAL_POS(grp , u_reference)/1000.0 ;
	double u_measure   = GROUP_VAL_POS(grp , u_measure)/1000.0 ;
	double s_reference = GROUP_VAL_POS(grp , s_reference)/100.0 ;
	double s_measure   = GROUP_VAL_POS(grp , s_measure)/100.0 ;
	double rate = 1.0;
	if(prule->w_unit == UNIT_S)
	{
		rate =  GROUP_VAL_POS(0 , prf1) * get_area_scanresolution (pp->p_config) / 10000.0;
		s_reference = s_reference /rate ;
		s_measure   = s_measure / rate ;
	}
	GROUP* pGroup = get_group_by_id (pp->p_config, grp);
	double sample_start = get_group_val (pGroup, GROUP_START) / 1000.0 ;
	double sample_range = get_group_val (pGroup, GROUP_RANGE) / 1000.0;
	double velocity = get_group_val (pGroup, GROUP_VELOCITY) / 200000.0;
	double current_angle = TMP(current_angle[grp]);
	double cos_current_angle = cos(current_angle) ;
	cairo_set_line_width (cr, 0.5);
	//*************************************************************************************
	if(GROUP_VAL_POS(grp , ut_unit)==UT_UNIT_TRUE_DEPTH)
	{
		sample_start *= cos_current_angle ;
		sample_range *= cos_current_angle ;
	}

    double tofdRefLine = gTofdS[grp].refLine / rate;
	struct TofdStruct tofd;
	int i;
	double yy[NUMOFPOINT];
	struct DefectStruct defect;

	if(get_group_val (pGroup, GROUP_TX_RX_MODE) == TOFD)//tofd
	{
		tofd.pcs = GROUP_VAL_POS(grp , field_distance[254]);//mm
		tofd.t0 = pGroup->wedge_delay / 1000.0;//pGroup->wedge.Probe_delay / 1000.0;//楔块延时 us
		tofd.c = get_group_val (pGroup, GROUP_VELOCITY) /100000.0 ;//mm/us
		tofd.mmPerPixel = (_nVStop - _nVStart) / (w - 50);//mm
		if(prule->w_unit == UNIT_S)
		{
			tofd.mmPerPixel *= rate;
		}
	//	g_debug("start:%f stop:%f width :%d mm per pixel :%f" ,_nVStart ,_nVStop ,w - 50 ,tofd.mmPerPixel);

	}

//参考线
	//ur线
	cairo_set_source_rgba(cr,1.0,0.5,0.5,1.0);/*红色cursor*/
	_nTmpX = (u_reference - sample_start) / sample_range  ;/* 当前值除以最大值再乘以窗口宽度*/
    gTofdS[grp].ur = _nTmpX ;
	_nTmpX *= (h - 20);
	_nTmpX = (int)_nTmpX + 0.5  ;
	cairo_move_to (cr, 20 , _nTmpX);
	cairo_line_to (cr, w-30  , _nTmpX );
	cairo_stroke (cr);

	//ur线指示框
	cairo_set_source_rgba(cr,0.84,0.67,0.945,1.0);/*显示当前值的紫色框*/
	//********
	//cairo_rectangle(cr, _nTmpX + 1 , 0 , 30 , 13);
	cairo_rectangle(cr, 21, _nTmpX - 14 , 30 , 13);
	cairo_fill (cr);
	cairo_stroke (cr);

	//cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
	//********
	//cairo_fill (cr);
	//cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
	//cairo_move_to(cr , _nTmpX + 3 , 10);
	cairo_move_to(cr,23, _nTmpX - 4 );
	if(GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_TIME)
		str=g_strdup_printf("%.2f",u_reference);
	else
	{
		if(get_unit(pp->p_config)==UNIT_MM)
			str=g_strdup_printf("%.2f",u_reference * velocity);
		else
			str=g_strdup_printf("%.2f",u_reference * 0.03937 * velocity);
	}
	cairo_show_text(cr,str);
	cairo_stroke(cr);
	g_free (str);

	//sr线
	_nTmpY = (s_reference-_nVStart) / (_nVStop - _nVStart);
    gTofdS[grp].sr = _nTmpY;
	_nTmpY = _nTmpY * (w - 50) + 20 ;
	_nTmpY = (int)_nTmpY + 0.5 ;
	cairo_set_source_rgba(cr,1.0,0.5,0.5,1.0);/*红色cursor*/
	cairo_set_line_width(cr, 0.5);
	cairo_move_to (cr,  _nTmpY , 0 );
	cairo_line_to (cr,  _nTmpY , h - 20);
	cairo_stroke(cr);

	//sr线指示框
	cairo_set_source_rgba(cr,0.678,0.984,0.89,1.0);/*显示当前值的绿色框*/
	cairo_rectangle(cr, _nTmpY + 1 , 0 , 30 , 13);
	cairo_fill (cr);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
	cairo_move_to(cr , _nTmpY + 3 , 10);
	str=g_strdup_printf("%.1f", s_reference );
	cairo_show_text(cr,str);
	cairo_stroke(cr);
	g_free (str);

	//tofd抛物线
	if(TOFD == get_group_val (pGroup, GROUP_TX_RX_MODE))//tofd
	{
		defect.t = GROUP_VAL_POS(grp , u_reference) / 1000.0;//us
		cairo_set_source_rgba(cr,0.84,0.67,0.945,1.0);/*显示当前值的紫色框*/
		//********
		//cairo_rectangle(cr, _nTmpX + 1 , 0 , 30 , 13);
		cairo_rectangle(cr, w - 30 - 41, _nTmpX - 14 , 40 , 13);
		cairo_fill (cr);
		cairo_stroke (cr);

		//cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
		//********
		//cairo_fill (cr);
		//cairo_stroke (cr);
		cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
		//cairo_move_to(cr , _nTmpX + 3 , 10);
		cairo_move_to(cr,w - 30 - 40, _nTmpX - 4 );
		double depth = GetDepthCal(grp ,defect.t, 0);
		if(MEASURE_DATA_ND - depth < 0.001)
		{
			str=g_strdup_printf("ND");
		}
		else
		{

			if(get_unit(pp->p_config)==UNIT_MM)
				str=g_strdup_printf("%.2f", depth );
			else
				str=g_strdup_printf("%.2f", depth  * 0.03937);
		}
		cairo_show_text(cr,str);
		cairo_stroke(cr);
		g_free (str);

		GetPointer(&tofd ,&defect ,yy ,NUMOFPOINT);

		int x,y;
		x = _nTmpY;
		y = _nTmpX;
		cairo_set_source_rgba(cr,1.0,0.5,0.5,1.0);/*红色cursor*/

		cairo_move_to (cr, x ,y);
		for(i = 0 ;i < NUMOFPOINT - 1 ;i++)
		{
			yy[i] = (h - 20) * (yy[i] - sample_start) / sample_range;
			cairo_line_to (cr, x + i + 1, yy[i]);
			cairo_move_to (cr, x + i + 1, yy[i]);
		}
		yy[i] = (h - 20) * (yy[i] - sample_start) / sample_range;
		cairo_line_to (cr, x + i + 1, yy[i]);

		cairo_move_to (cr, x, y);
		for(i = 0 ;i < NUMOFPOINT - 1 ;i++)
		{
			cairo_line_to (cr, x - i - 1, yy[i]);
			cairo_move_to (cr, x - i - 1, yy[i]);
		}
		cairo_line_to (cr, x - i - 1, yy[i]);
		cairo_stroke (cr);

		//ref line线
		_nTmpY = (tofdRefLine -_nVStart) / (_nVStop - _nVStart);
        gTofdS[grp].averagePos = _nTmpY ;//从开始算的第几个像素
		_nTmpY = _nTmpY * (w - 50) + 20 ;
		_nTmpY = (int)_nTmpY + 0.5 ;
		cairo_set_source_rgba(cr,1.0,1.0,0.5,1.0);/*黄色cursor*/
		cairo_set_line_width(cr, 0.5);
		cairo_move_to (cr,  _nTmpY , 0 );
		cairo_line_to (cr,  _nTmpY , h - 20);
		cairo_stroke(cr);

		//ref line指示框
		cairo_set_source_rgba(cr,0.678,0.984,0.89,1.0);/*显示当前值的绿色框*/
		cairo_rectangle(cr, _nTmpY + 1 , 0 , 30 , 13);
		cairo_fill (cr);
		cairo_stroke (cr);
		cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
		cairo_move_to(cr , _nTmpY + 3 , 10);
		str=g_strdup_printf("%.1f", tofdRefLine );
		cairo_show_text(cr,str);
		cairo_stroke(cr);
		g_free (str);
	}

//测量线
	//um线
	cairo_set_source_rgba(cr,0,0,1,1.0);/*蓝色cursor*/
//	cairo_set_source_rgba(cr,0.5,1.0,0.5,1.0);/*绿色cursor*/
	_nTmpX = (u_measure-sample_start)/(sample_range)  ;	/* 当前值除以最大值再乘以窗口宽度 */
    gTofdS[grp].um = _nTmpX;
	_nTmpX *= (h-20);
	_nTmpX = (int)_nTmpX + 0.5  ;
	cairo_move_to (cr, 20 , _nTmpX);
	cairo_line_to (cr, w-30  , _nTmpX );
	cairo_stroke (cr);

	//um线指示框
	cairo_set_source_rgba(cr,0.84,0.67,0.945,1.0);/*显示当前值的紫色框*/
	//cairo_rectangle(cr, _nTmpX + 1 , 0 , 30 , 13);
	//cairo_fill (cr);
	//cairo_stroke (cr);
	//cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
	//cairo_move_to(cr , _nTmpX + 3 , 10);
	cairo_rectangle(cr, 21, _nTmpX - 14 , 30 , 13);
	cairo_fill (cr);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
	//cairo_move_to(cr , _nTmpX + 3 , 10);
	cairo_move_to(cr,23, _nTmpX - 4 );

	if(GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_TIME)
		str=g_strdup_printf("%.2f",u_measure);
	else
	{
		if(get_unit(pp->p_config)==UNIT_MM)
			str=g_strdup_printf("%.2f",u_measure * velocity);
		else
			str=g_strdup_printf("%.2f",u_measure * 0.03937 * velocity);
	}
	cairo_show_text(cr,str);
	cairo_stroke(cr);
	g_free (str);

	//sm线
	_nTmpY = (s_measure - _nVStart) / (_nVStop - _nVStart);
    gTofdS[grp].sm = _nTmpY;
	_nTmpY = _nTmpY * (w - 50) + 20 ;
	_nTmpY = (int)_nTmpY + 0.5 ;
	cairo_set_source_rgba(cr,0,0,1,1.0);/*蓝色cursor*/
	cairo_set_line_width(cr, 0.5);
	cairo_move_to (cr,  _nTmpY , 0 );
	cairo_line_to (cr,  _nTmpY , h - 20);
	cairo_stroke(cr);

	//sm线指示框
	cairo_set_source_rgba(cr,0.678,0.984,0.89,1.0);/*显示当前值的绿色框*/
	cairo_rectangle(cr, _nTmpY + 1 , 0 , 30 , 13);
	cairo_fill (cr);
	cairo_stroke (cr);
	cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
	cairo_move_to(cr , _nTmpY + 3 , 10);
	str=g_strdup_printf("%.1f", s_measure );
	cairo_show_text(cr,str);
	cairo_stroke(cr);
	g_free (str);

	//tofd抛物线
	if(TOFD == get_group_val (pGroup, GROUP_TX_RX_MODE))//tofd
	{
		defect.t = GROUP_VAL_POS(grp , u_measure) / 1000.0;//us
		cairo_set_source_rgba(cr,0.84,0.67,0.945,1.0);/*显示当前值的紫色框*/
		//********
		//cairo_rectangle(cr, _nTmpX + 1 , 0 , 30 , 13);
		cairo_rectangle(cr, w - 30 - 41, _nTmpX - 14 , 40 , 13);
		cairo_fill (cr);
		cairo_stroke (cr);

		//cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
		//********
		//cairo_fill (cr);
		//cairo_stroke (cr);
		cairo_set_source_rgba(cr,0.0,0.0,0.0,1.0);/*当前值*/
		//cairo_move_to(cr , _nTmpX + 3 , 10);
		cairo_move_to(cr,w - 30 - 40, _nTmpX - 4 );
		double depth = GetDepthCal(grp ,defect.t, 1);
		if(MEASURE_DATA_ND - depth < 0.001)
		{
			str=g_strdup_printf("ND");
		}
		else
		{

			if(get_unit(pp->p_config)==UNIT_MM)
				str=g_strdup_printf("%.2f", depth );
			else
				str=g_strdup_printf("%.2f", depth  * 0.03937);
		}
		cairo_show_text(cr,str);
		cairo_stroke(cr);
		g_free (str);

		GetPointer(&tofd ,&defect ,yy ,NUMOFPOINT);
		int x,y;
		x = _nTmpY;
		y = _nTmpX;
		cairo_set_source_rgba(cr,0,0,1,1.0);/*蓝色cursor*/
	//cairo_set_source_rgba(cr,0.5,1.0,0.5,1.0);/*绿色cursor*/

		cairo_move_to (cr, x ,y);
		for(i = 0 ;i < NUMOFPOINT - 1 ;i++)
		{
			yy[i] = (h - 20) * (yy[i] - sample_start) / sample_range;
			cairo_line_to (cr, x + i + 1, yy[i]);
			cairo_move_to (cr, x + i + 1, yy[i]);
		}
		yy[i] = (h - 20) * (yy[i] - sample_start) / sample_range;
		cairo_line_to (cr, x + i + 1, yy[i]);

		cairo_move_to (cr, x, y);
		for(i = 0 ;i < NUMOFPOINT - 1 ;i++)
		{
			cairo_line_to (cr, x - i - 1, yy[i]);
			cairo_move_to (cr, x - i - 1, yy[i]);
		}
		cairo_line_to (cr, x - i - 1, yy[i]);

		cairo_stroke (cr);
	}
}



void DrawCursor(GtkWidget *widget , gpointer data , cairo_t *cr)
{

	DRAW_AREA_P prule = (DRAW_AREA_P)(data) ;
	switch (prule->scan_type)
	{
		case A_SCAN:
			DrawCursorASCAN(widget , data , cr);
			break;
		case A_SCAN_R:
			DrawCursorASCANR(widget , data , cr);
			break ;
		case B_SCAN:
		case B_SCAN_VERT:
			DrawCursorBSCAN(widget , data , cr);
			break;
		case C_SCAN:
		case CC_SCAN:
		case CCC_SCAN:
			DrawCursorCSCAN(widget , data , cr);
			break;
		case C_SCAN_VE:
			DrawCursorCSCANVert(widget , data , cr);
			break;
		case S_SCAN:
		case S_SCAN_A:
		case S_SCAN_L:
			DrawCursorSSCAN(widget , data , cr);
			break;
		case TOFD_B_SCAN:
			DrawCursorTOFDB(widget , data , cr);
			break;
		default:
			break ;
	}
}



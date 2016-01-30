/*
 * calibration.c
 *
 *  Created on: 2012-6-8
 *      Author: shensheng
 */
#include "drawui.h"
#include "drawfb.h"
#include "focallaw.h"		/* 计算聚焦法则的头文件 */
#include "base_config.h"
#include "base.h"
#include "spi_d.h"
#include "file_op.h"
#include "main.h"
#include "draw_dialog.h"
#include "calibration.h"
#include "callback.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <memory.h>
#include "ui/ui.h"
#include "string/_string.h"
#include "calculation/sizingcurve.h"
#include "calculation/limit.h"
pthread_cond_t qready = PTHREAD_COND_INITIALIZER;
pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;

void GetCalibrationStartAndStepLaw(int grp , int* nStart_ , int* nStep_)
{
	int _nStart , _nStep ;
	if(LAW_VAL_POS(grp,Focal_type) == AZIMUTHAL_SCAN)
	{
		_nStart  = (TMP_CBA(clb_start) - LAW_VAL_POS(grp , Angle_min))/ LAW_VAL_POS(grp , Angle_step)  ;
		_nStep   = (TMP_CBA(clb_end)  -  TMP_CBA(clb_start)) / LAW_VAL_POS(grp , Angle_step) + 1;
	}
	else if (LAW_VAL_POS(grp , Focal_type) == LINEAR_SCAN)
	{
		_nStart = TMP_CBA(clb_start) / 100 - 1  ;
		_nStep  =  (TMP_CBA(clb_end)  -  TMP_CBA(clb_start))/ 100 + 1 ;
	}
	*nStart_ = _nStart ;
	*nStep_  = _nStep  ;
}


void update_spi_after_wedge_delay_calibration(unsigned char grp)
{
	int i;
	int offset ;
	int _nMaxBeamDelay ;

	for(offset = 0 , i = 0 ; i < grp ; i++)
		offset += TMP(beam_qty[i]);

	UpdateGateForSpiSending(grp);

    TMP(group_spi[grp]).sample_start	= (group_get_start(grp) + GROUP_VAL_POS(grp , wedge_delay)) / 10;
	TMP(group_spi[grp]).sample_range	= TMP(group_spi[grp]).sample_start + GROUP_VAL_POS(grp , range) / 10;

	_nMaxBeamDelay = GetGroupMaxBeamDelay(grp) ;
	TMP(group_spi[grp]).rx_time	= TMP(group_spi[grp]).sample_range  + _nMaxBeamDelay + 5;

	MultiGroupSendGroupSpi();
	send_focal_spi (grp , SPI_RESET_NO);
}

//最小二乘法拟合直线
void MinSqureFittingLine(int nPointQty_ , int* nArray_)
{
     double _nSum  = 0.0;
     double _nISum = 0.0;
     double a , b ;
     int i ;
     // if point quantity is only one , return
     if((nPointQty_ < 2) || (!nArray_))
     {
    	 return ;
     }

     for(i = 0 ; i< nPointQty_ ; i++)
     {
          _nSum += nArray_[i] ;
	      _nISum+= i * nArray_[i] ;
     }

     a = 6 * ((1-nPointQty_) * _nSum + 2 * _nISum) /(nPointQty_*(nPointQty_*nPointQty_ -1)) ;
     b = -2 * ((1-2 * nPointQty_) * _nSum + 3 * _nISum) / (nPointQty_ * (nPointQty_ + 1)) ;
     for(i = 0 ; i< nPointQty_ ; i++)
     {
         nArray_[i] = (int)(i * a + b) ;
     }
}

//****************************************
//  编码器校准：2011.7.1 何凡
//****************************************
double cba_encoder()
{
	TMP_CBA(delt_distance) = pp->distance / 1000;//- get_enc_origin (pp->p_config, get_cur_encoder (pp->p_config));
	TMP_CBA(delt_measure) = TMP_CBA(measure_end) - TMP_CBA(measure_start);
	if(fabs(TMP_CBA(delt_measure)) < 0.01) TMP_CBA(delt_measure) = 0.01 ;
	TMP_CBA(resolution) =  fabs(TMP_CBA(delt_measure)/TMP_CBA(delt_distance));
	//printf("%f %f %f %f\n" , TMP_CBA(delt_distance) , TMP_CBA(measure_end) , TMP_CBA(measure_start) , TMP_CBA(resolution));
    if(TMP_CBA(resolution) < 1) TMP_CBA(resolution) = 1  ;
    if(TMP_CBA(resolution) > 2000) TMP_CBA(resolution) = 2000 ;
	return TMP_CBA(resolution);
}


//****************************************
//  声速校准：2011.7.1 何凡
//****************************************
double cba_ultrasound_velocity()
{
	int grp = get_current_group(pp->p_config);
	double cos_current_angle = cos(TMP(current_angle[grp]));

	if(TMP_CBA(time_start) != TMP_CBA(time_end))
	{
		switch(pp->echotype_pos)
		{
			case 0://Radius
				TMP_CBA(radius1) = pp->radius1 / 1000.0;
				TMP_CBA(radius2) = pp->radius2 / 1000.0;
				TMP_CBA(velocity_last) = 200000*fabs(TMP_CBA(radius2)-TMP_CBA(radius1)) / fabs(TMP_CBA(time_end)-TMP_CBA(time_start)) ;
				break;
			case 1://Depth
				TMP_CBA(depth1) = pp->depth1 / 1000.0;
				TMP_CBA(depth2) = pp->depth2 / 1000.0;
				TMP_CBA(velocity_last) = 200000*fabs(TMP_CBA(depth2)-TMP_CBA(depth1)) / fabs(TMP_CBA(time_end)-TMP_CBA(time_start)) ;
				TMP_CBA(velocity_last) /= cos_current_angle ;
				break;
			case 2://Thickness
				TMP_CBA(thickness1) = pp->thickness1 / 1000.0;
				TMP_CBA(thickness2) = pp->thickness2 / 1000.0;
				TMP_CBA(velocity_last) = 200000*fabs(TMP_CBA(thickness2)-TMP_CBA(thickness1)) / fabs(TMP_CBA(time_end)-TMP_CBA(time_start)) ;
				TMP_CBA(velocity_last) /= cos_current_angle ;
				break;
		}
		return TMP_CBA(velocity_last);//单位m/s
	}
	else
		return 0;
}

//****************************************
//  延时校准：2011.7.1 何凡
//****************************************
int cba_ultrasound_wedgedelay()
{
	int i;
	double vel, t1 ;
	int _nStart  ,  _nStep  ;
	unsigned int wedge_delay  , pulse_width ;
	int grp = get_current_group(pp->p_config);
	int    temp_beam_delay[256] ;
	double tolerance ;
	double temp_angle ;

	vel = GROUP_VAL_POS(grp , velocity) / 100.0;// m/s
	t1  = 2000000 * TMP_CBA(distance) / vel    ;//ns

	wedge_delay = GROUP_VAL_POS(grp , wedge_delay);
	pulse_width = GROUP_VAL_POS(grp , pulser_width1) / PW_DIV ;
	tolerance = pp->tolerance / 100.0 ;
	tolerance = 2000000 * tolerance / vel ;
	tolerance *= 1.5  ;
	GetCalibrationStartAndStepLaw(grp , &_nStart , &_nStep ) ;

	double temp_value ;
	int _nGateHeight = GROUP_GATE_VAL_POS(grp , GATE_A , height) ;
	for(i = _nStart ; i < _nStart + _nStep ; i++)
	{
		//*****************************************
		temp_value = TMP(clb_wedge_max_DB[i]) / 20.47    ;
		if(fabs(temp_value) < _nGateHeight)  return TRUE ;
		//*****************************************
	}

	if(GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_TRUE_DEPTH)
	{
		if(LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
		{
			for(i = _nStart ; i < _nStart + _nStep ; i++)
			{
				temp_angle = LAW_VAL_POS(grp , Angle_min) + i * LAW_VAL_POS(grp , Angle_step) ;
				temp_angle = cos( temp_angle * G_PI / 18000.0 );
				temp_beam_delay[i] = (int)(TMP(clb_wedge_position[i]) * 10 - wedge_delay - pulse_width - t1 / temp_angle) ;
			}
		}
		else
		{
			temp_angle = (double)(LAW_VAL_POS(grp , Angle_min)) ;
			temp_angle = cos( temp_angle * G_PI / 18000.0 );
			for(i = _nStart ; i < _nStart + _nStep ; i++)
			{
				temp_beam_delay[i] = (double)(TMP(clb_wedge_position[i]) * 10 - wedge_delay - pulse_width -t1 / temp_angle) ;
			}
		}
	}
	else
	{
		for(i = _nStart ; i < _nStart + _nStep ; i++)
		{
			temp_beam_delay[i] = TMP(clb_wedge_position[i]) * 10 - pulse_width - wedge_delay - t1 ;
		}
	}
	for(i = _nStart ; i < _nStart + _nStep ; i++)
	{
		temp_beam_delay[i] = temp_beam_delay[i] - GROUP_VAL_POS(grp , beam_delay[i]) ;
	}

	MinSqureFittingLine(_nStep , &(temp_beam_delay[_nStart]));

	for(i = _nStart ; i < _nStart + _nStep ; i++)
	{
		temp_beam_delay[i] = temp_beam_delay[i] + GROUP_VAL_POS(grp , beam_delay[i]) ;
	}

	for(i = _nStart ; i < _nStart + _nStep ; i++)
	{
		if(temp_beam_delay[i] < 0) temp_beam_delay[i]  = 0 ;
		GROUP_VAL_POS(grp , beam_delay[i]) = temp_beam_delay[i] ;
		TMP(focal_law_all_beam[grp][i]).G_delay = temp_beam_delay[i] ;
	}
	return FALSE ;
}


//****************************************
//  灵敏度校准：2011.7.1 何凡
//****************************************
int cba_ultrasound_sensitivity()
{
	int i;
	double temp_value[256] ;
	double max_temp_value = 0;
	double amplitude = pp->ref_amplitude / 100.0 ;
	int grp = get_current_group(pp->p_config);
	int _nGateHeight = GROUP_GATE_VAL_POS(grp , GATE_A , height) ;

	int _nStart  ,  _nStep  ;
	GetCalibrationStartAndStepLaw(grp , &_nStart , &_nStep ) ;

	for(i = _nStart ; i < _nStart + _nStep ; i++)
	{
		//*****************************************
		temp_value[i] = TMP(clb_sensation_max_DB[i]) / 20.47      ;
		if(fabs(temp_value[i]) < _nGateHeight)  return -1 ;
		if(temp_value[i] > max_temp_value)  max_temp_value = temp_value[i] ;
		//*****************************************
	}
	if(max_temp_value > 100)  return -2 ;
	if(max_temp_value < amplitude)  max_temp_value = amplitude  ;
	for(i = _nStart ; i < _nStart + _nStep ; i++)
	{
		temp_value[i] = max_temp_value / temp_value[i]  ;
		pp->tmp_gain_off[i] = (int)(10 * fabs( 20 * log10(temp_value[i]) ));
		TMP(clb_sensation_max_DB[i]) = max_temp_value * 20.47 ;
	}
	return FALSE ;
}

//****************************************
//  TCG校准：2012.10.15
//****************************************

void RefreshTCGCalibration(int grp , int nPointPos_)
{
	int _nPointQty = GROUP_VAL_POS(grp , SizingCurves.dac_point_qty) ;
	int _nBeamQty  = TMP(beam_qty[grp]) ;
	int i  , j ;

	//确保当前点的TCG增益 不小于O(要比第一点的幅度值 小)
	for(j = 0 ; j < _nBeamQty ; j++)
	{
		if(GROUP_VAL_POS(grp , SizingCurves.amplitude[j][0]) < GROUP_VAL_POS(grp , SizingCurves.amplitude[j][nPointPos_]))
		{
			GROUP_VAL_POS(grp , SizingCurves.amplitude[j][nPointPos_]) = GROUP_VAL_POS(grp , SizingCurves.amplitude[j][0]) ;
		}
	}

	for(i = nPointPos_ ; i < _nPointQty - 1; i++)
	{
		for(j = 0 ; j < _nBeamQty ; j++)
		{
			//if(GROUP_VAL_POS(grp , SizingCurves.amplitude[j][0]) < GROUP_VAL_POS(grp , SizingCurves.amplitude[j][i + 1]))
			//{
			//	GROUP_VAL_POS(grp , SizingCurves.amplitude[j][i + 1]) = GROUP_VAL_POS(grp , SizingCurves.amplitude[j][0]) ;
			//}
			if(GROUP_VAL_POS(grp , SizingCurves.position[j][i]) >= GROUP_VAL_POS(grp , SizingCurves.position[j][i + 1]))
			{
				GROUP_VAL_POS(grp , SizingCurves.position[j][i + 1]) = GROUP_VAL_POS(grp , SizingCurves.position[j][i]) + 10 ;
			}
		}
	}
}

#define MAX_CALIBRATE_AMP  150
int cba_ultrasound_TCG()
{
	int i;
	double temp_value[256] , scale ;
	double amplitude = pp->ref_amplitude / 100.0 ;

	int grp = get_current_group(pp->p_config);
	int _nPointPos   = GROUP_VAL_POS(grp , SizingCurves.point_pos) ;
	int wedge_delay  = GROUP_VAL_POS(grp , wedge_delay) ;
	int pulse_width  = GROUP_VAL_POS(grp , pulser_width1) / PW_DIV ;
	//double _nTolerance = pp->tolerance_t / 100.0 ;
	int _nStart  ,  _nStep  ;
	GetCalibrationStartAndStepLaw(grp , &_nStart , &_nStep ) ;
	//dac_ref_ampl
	for(i = _nStart ; i < _nStart + _nStep ; i++)
	{
		//*****************************************
		temp_value[i] = TMP(clb_tcg_max_DB[i]) / 20.47    ;
		if(fabs(temp_value[i]) > MAX_CALIBRATE_AMP)  return -1 ;

	}
	double _nRefGain , _nTmpGain ;
	for(i = _nStart ; i < _nStart + _nStep ; i++)
	{
		_nRefGain  = GROUP_VAL_POS(grp , SizingCurves.dac_ref_ampl[i]) / 10.0  ;
		scale = GROUP_VAL_POS(grp , SizingCurves.amplitude[i][_nPointPos]) / (1000 * _nRefGain) ;
		temp_value[i] *= scale ;
		// confirm the tcg gain is small than 40 db
		_nTmpGain = _nRefGain / temp_value[i]  ;
		_nTmpGain = log10 ( scale ) * 20 ;
		if(_nTmpGain > 40)
		{
			temp_value[i] = _nRefGain / 100 ;
		}
		temp_value[i] *= 1000 ;
	}
	for(i = _nStart ; i < _nStart + _nStep ; i++)
	{
		GROUP_VAL_POS(grp , SizingCurves.amplitude[i][_nPointPos]) = MAX( temp_value[i] , 1000 );
		GROUP_VAL_POS(grp , SizingCurves.position[i][_nPointPos])  = TMP(clb_tcg_position[i]) * 10 - pulse_width - wedge_delay -  GROUP_VAL_POS(grp , beam_delay[i])  ;
		TMP(clb_tcg_max_DB[i]) = amplitude * 20.47 ;
	}
	// 保证当前校准点之后的点增益 和 位置都大于当前点
	RefreshTCGCalibration(grp , _nPointPos) ;
	return 0 ;
}


void ClearCalibrationWedgeDelay()
{
	int nGroupId = get_current_group(pp->p_config);
	int _nStart  ,  _nStep , i ;
	GetCalibrationStartAndStepLaw(nGroupId , &_nStart , &_nStep ) ;

	for(i = _nStart ; i < _nStart + _nStep ; i++ )
	{
		TMP(clb_wedge_max_DB[i])   = 0 ;
		TMP(clb_wedge_position[i]) = 0 ;
		GROUP_VAL_POS(nGroupId , beam_delay[i]) = TMP_CBA(BeamDelay[i]);
	}
	GROUP_VAL_POS(nGroupId , WedgeDelayCalibrated) = 0 ;
}

void ClearCalibrationWedgeDelayAll()
{
	int nGroupId = get_current_group(pp->p_config);
	int i ;

	for(i = 0 ; i < TMP(beam_qty[nGroupId]) ; i++ )
	{
		TMP(clb_wedge_max_DB[i])   = 0 ;
		TMP(clb_wedge_position[i]) = 0 ;
		GROUP_VAL_POS(nGroupId , beam_delay[i]) = TMP_CBA(BeamDelay[i]);
	}
	GROUP_VAL_POS(nGroupId , WedgeDelayCalibrated) = 0 ;

}

void ClearCalibrationSensation()
{
	int nGroupId = get_current_group(pp->p_config);
	int _nStart  ,  _nStep ,  i ;
	GetCalibrationStartAndStepLaw(nGroupId , &_nStart , &_nStep ) ;

	for(i = _nStart ; i < _nStart + _nStep ; i++ )
	{
		TMP(clb_sensation_max_DB[i]) = 0 ;
		GROUP_VAL_POS(nGroupId, gain_offset[i]) = 0;
	}
	GROUP_VAL_POS(nGroupId , SensationCalibrated) = 0 ;
}


// return value is ns
int GetUtWedgeDelayCalicbration(int nGroupId_ , double nVelocity_)
{
	//获取闸门1信息
	int _nResult ;
	double _nDistance ;
	double _nTmpDistance ;
	int _nDistance_ns  ;
	// not reasonable!
	if(nVelocity_ < 600 || nVelocity_ > 10000)  return 0;

	int offset , k;
	for (offset = 0, k = 0 ; k < nGroupId_; k++)
		offset += TMP(beam_qty[k]);

	_nDistance = TMP_CBA(time_start) ;
	switch(pp->echotype_pos)
	{
		case 0://Radius
			_nTmpDistance = pp->radius1 / 1000.0;
			_nDistance_ns = (int) ( _nTmpDistance * 2000000.0 / nVelocity_ ) ;
			break;
		case 1://Depth
			_nTmpDistance =  pp->depth1 / 1000.0;
			_nDistance_ns = (int) ( _nTmpDistance * 2000000.0 / (nVelocity_ * cos(TMP(current_angle[nGroupId_])))) ;
			break;
		case 2://Thickness
			_nTmpDistance = pp->thickness1 / 1000.0;
			_nDistance_ns = (int) ( _nTmpDistance * 2000000.0 /(nVelocity_ * cos(TMP(current_angle[nGroupId_])))) ;
			break;
	}
	_nResult = _nDistance * 10 - _nDistance_ns  ;
	return _nResult ;
}


// TOFD calibration
void UtTOFDCalicbration(int nGroupId_)
{
	double _fThickness  = GROUP_VAL_POS(nGroupId_ , part.Thickness) / 1000.0  ;
	double _fWedgeSep   = GROUP_VAL_POS(nGroupId_ , field_distance[255])     ;
	double _fVelocity = GROUP_VAL_POS(nGroupId_ , velocity) / 100.0 ;
	double _fCursor1  = GROUP_VAL_POS(nGroupId_ , u_reference)/1000.0 ;
	double _fCursor2  = GROUP_VAL_POS(nGroupId_ , u_measure) / 1000.0 ;
	_fCursor1 = _fCursor1 * _fVelocity / 2000 ;
	_fCursor2 = _fCursor2 * _fVelocity / 2000 ;

	double _fTmp = fabs(_fCursor1 - _fCursor2) ;
	_fTmp  = (_fThickness * _fThickness - _fTmp * _fTmp) / (2 * _fTmp)   ;
	double _fTmpRefPoint  = _fTmp - _fWedgeSep / 2  ;
	if(_fTmpRefPoint < 0)
		return ;

	if(_fCursor1 > _fCursor2) _fCursor1 = _fCursor2 ;
	double _fTmpPCS = _fTmpRefPoint * 2 + _fWedgeSep  ;
	double _fDelay  = (_fTmpPCS - _fCursor1 * 2) * 1000000 /  _fVelocity;
	int _nWedgeDelay = GROUP_VAL_POS(nGroupId_ , wedge_delay) ;
	_nWedgeDelay = _nWedgeDelay - _fDelay ;
	TMP_CBA(CalibrationWedgeDelay)  = _nWedgeDelay ;
	GROUP_VAL_POS(nGroupId_ , field_distance[254])   = _fTmpPCS        ;
	GROUP_VAL_POS(nGroupId_ , field_distance[253])   = _fTmpRefPoint   ;

}

void esc_calibration()
{
	int	grp = get_current_group (pp->p_config);
	if(!pp->ctype_pos)//当位Encoder时无需更新扫描
	{
		pp->clb_encoder = 0;
	}

	pp->clb_flag = 0;
	pp->pos1[pp->pos] = 2;
	if(pp->cstart_qty != 1)
		pp->cstart_qty = 1;
	gtk_widget_set_sensitive(pp->eventbox2[0],TRUE);
	gtk_widget_set_sensitive(pp->eventbox2[3],TRUE);
	gtk_widget_set_sensitive(pp->eventbox2[1],TRUE);

	gtk_widget_set_sensitive(pp->menubar,TRUE);
	gtk_widget_queue_draw (pp->status_area);
	GROUP_VAL_POS(get_current_group(pp->p_config) , ascan_envelope)  = FALSE;
	if(pp->echotype_pos == 0)
	{
		GROUP_VAL_POS(grp , ut_unit) = UT_UNIT_TRUE_DEPTH;
		UpdateGateForSpiSending(grp) ;
		send_focal_spi (grp , SPI_RESET_NO);		
	}
	request_refresh(REFRESH_DRAW_AREA_ALL);
	//draw_area_all();
	//pp->bRefreshDraw = TRUE ;
}

void EncoderCalibrationCallback021()
{
	((pp->cstart_qty) < 4) ? (pp->cstart_qty) ++ : ((pp->cstart_qty) = 1);
	if((pp->cstart_qty) == 2)
	{
		pp->clb_flag = 1 ; // draw calibration area need it to be set
		dialogMessage(pp->window , getDictString(_STRING_CLB_ENCODER_BEGIN) , &color_blue);
	}
	if((pp->cstart_qty) == 3)
	{
		//先让编码器的起点值与origin一致
		TMP_CBA(measure_start) = TMP(measure_data[0][4]);
		dialogMessage(pp->window , getDictString(_STRING_CLB_ENCODER_END) , &color_blue);
	}
	else if((pp->cstart_qty) == 4)
	{
		TMP_CBA(measure_end) = TMP(measure_data[0][4]);
		//调用校准函数cba_encoder()
		set_enc_resolution (pp->p_config, cba_encoder()*1000,
				get_cur_encoder (pp->p_config));
	}
	else if((pp->cstart_qty) == 1)//Accpet
	{
		set_enc_resolution (pp->p_config, TMP_CBA(resolution)*1000, get_cur_encoder (pp->p_config));
		esc_calibration();
		draw_area_all();
	}
}


void UltraSoundCalibrationVelocityCallback021()
{
	int offset,k;
	int grp = get_current_group(pp->p_config);
    int _nIndex ;

	((pp->cstart_qty) < 6) ? (pp->cstart_qty) ++ : ((pp->cstart_qty) = 1);
	if((pp->cstart_qty) == 2)
	{
		pp->clb_flag = 1 ; // draw calibration area need it to be setted
		//根据当前的显示模式，确定校准默认模式是深度还是圆弧
		if(GROUP_VAL_POS(grp , ut_unit) == 0)
		{
			pp->echotype_pos  = 0;
		}
		else
		{
			pp->echotype_pos = 1 ;
		}
		request_refresh(REFRESH_DRAW_AREA_ALL);
		//draw_area_all();
		//pp->bRefreshDraw = TRUE ;
	}
	else if((pp->cstart_qty) == 5)
	{
		for (offset = 0, k = 0 ; k < grp; k++)
			offset += TMP(beam_qty[k]);
		_nIndex = offset + TMP(beam_num[grp]);
		TMP_CBA(time_start) = ((TMP(measure_data[_nIndex][1])) & 0xfffff);
	}
	else if((pp->cstart_qty) == 6)
	{
		for (offset = 0, k = 0 ; k < grp; k++)
			offset += TMP(beam_qty[k]);
		_nIndex = offset + TMP(beam_num[grp]);
		TMP_CBA(time_end)   = ((TMP(measure_data[_nIndex][1])) & 0xfffff);
		pp->vel = cba_ultrasound_velocity();
	}
	else if((pp->cstart_qty) == 1)
	{
		if((pp->vel > 635) && (pp->vel<10000))
		{
			//在此调用声速校准函数->此处校准之后的声速用于Wedge Delay校准
			set_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)),
					GROUP_VELOCITY, (int)(100*pp->vel))  ;
			GROUP_VAL_POS(grp , VelocityCalibrated) = 1  ;
			GROUP_VAL_POS(grp , WedgeDelayCalibrated) = 0;
			gtk_widget_queue_draw (pp->status_area);
			esc_calibration();
			generate_focallaw (grp);
			RefreshBeamDelay(grp);
			MultiGroupSendGroupSpi();
			MultiGroupSendFocalSpi();
			RefreshScanInfor() ;
			pp->bRefreshDraw = TRUE ;
		}
		else
		{
			dialogError(pp->window , getDictString(_STRING_CLB_VELOCITY_NOGO));
			pp->cstart_qty = 6 ;
		}
	}
}
int CheckPACalibrationWedgeDelayResult()
{
	int ret = 0;
	int i;
	int offset,k;
	double s, vel, wedge_delay;
	int grp = get_current_group(pp->p_config);

	for (offset = 0, k = 0 ; k < grp; k++)
		offset += TMP(beam_qty[k]);

	int pulse_width ;
	double tolerance , t1;
	double temp_angle ;

	int _nStart  ,  _nStep  ;
	GetCalibrationStartAndStepLaw(grp , &_nStart , &_nStep ) ;

	vel = GROUP_VAL_POS(grp , velocity) / 100.0 ;// m/s
	t1  = 2000000 * TMP_CBA(distance) / vel ;//ns
	wedge_delay = GROUP_VAL_POS(grp , wedge_delay);
	pulse_width = GROUP_VAL_POS(grp , pulser_width1) /  PW_DIV;
	tolerance = pp->tolerance / 100.0 ;
	tolerance = 2000000 * tolerance / vel ;

	if(GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_TRUE_DEPTH)
	{
		if(LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
		{
			for(i = _nStart ; i < _nStart + _nStep ; i++)
			{

				temp_angle = LAW_VAL_POS(grp , Angle_min) + i * LAW_VAL_POS(grp , Angle_step) ;
				temp_angle = cos( temp_angle * G_PI / 18000.0 );
				s = (double)( TMP(clb_wedge_position[i]) * 10 - pulse_width - GROUP_VAL_POS(grp , beam_delay[i]) - wedge_delay );
				s = s * temp_angle ;
				s = s - t1;
				if( fabs(s) > tolerance )  { ret = 1 ; break ;}
			}
		}
		else
		{
			temp_angle = (double)(LAW_VAL_POS(grp , Angle_min)) ;
			temp_angle = cos( temp_angle * G_PI / 18000.0 );
			for(i = _nStart ; i < _nStart + _nStep ; i++)
			{
				s = (double) ( TMP(clb_wedge_position[i]) * 10 - pulse_width - GROUP_VAL_POS(grp , beam_delay[i]) - wedge_delay );
				s = s * temp_angle - t1;
				if( fabs(s) > tolerance )  { ret = 1 ; break ;}
			}
		}
	}
	else
	{
		for(i = _nStart ; i < _nStart + _nStep ; i++)
		{
			s = (double) ( TMP(clb_wedge_position[i]) * 10 - pulse_width - GROUP_VAL_POS(grp , beam_delay[i]) - wedge_delay ) ;
			s = s - t1;
			if( fabs(s) > tolerance )  { ret = 1 ; break ;}
		}
	}
	return ret ;
}

int IfAllLawCalibrationIsFinished()
{
	int ret  = FALSE ;
	int grp  = get_current_group(pp->p_config);

	if(LAW_VAL_POS(grp,Focal_type) == AZIMUTHAL_SCAN)
	{
		if(TMP_CBA(clb_end) >= LAW_VAL_POS(grp , Angle_max))
		{
			ret = FALSE ;
		}
		else
		{
			TMP_CBA(clb_start) = TMP_CBA(clb_end) + LAW_VAL_POS(grp , Angle_step) ;
			TMP_CBA(clb_end)   = LAW_VAL_POS(grp , Angle_max) ;
			ret = TRUE ;
		}
	}
	else if (LAW_VAL_POS(grp,Focal_type) == LINEAR_SCAN)
	{
		if((TMP_CBA(clb_end)/100 )>= TMP(beam_qty[grp]))
		{
			ret = FALSE ;
		}
		else
		{
			TMP_CBA(clb_start) = TMP_CBA(clb_end) + 100 ;
			TMP_CBA(clb_end)   = TMP(beam_qty[grp]) * 100 ;
			ret = TRUE ;
		}
	}

	return ret ;
}
void PACalibrationWedgeDelayFinish()
{
	int _bContinue  = IfAllLawCalibrationIsFinished();
	int grp = get_current_group(pp->p_config);
	if(_bContinue)
	{
		pp->cstart_qty = 3 ;
		draw_area_all();
	}
	else
	{
		GROUP_VAL_POS(grp , WedgeDelayCalibrated) =  1;
		gtk_widget_queue_draw (pp->status_area);
		esc_calibration();
	}
}
void SetGainOffsetCalibrated()
{
	int _nStart  ,  _nStep , i  ;
	int grp = get_current_group(pp->p_config);
	GetCalibrationStartAndStepLaw(grp , &_nStart , &_nStep ) ;

	for(i = _nStart ; i < _nStart + _nStep ; i++)
	{
	   GROUP_VAL_POS(grp , gain_offset[i]) =  pp->tmp_gain_off[i];
	}
	ConfirmGainOffsetOfAllBeamInLimit(grp) ;
	send_focal_spi(grp , SPI_RESET_NO);
}
int isResultAcceptabel()
{
	int _nStart  ,  _nStep , i  ;
	double temp_value ;
	double tolerance = pp->tolerance_t / 100.0;
	double amplitude = pp->ref_amplitude / 100.0 ;
	tolerance *= 2 ;

	int grp = get_current_group(pp->p_config);
	if(LAW_VAL_POS(grp,Focal_type) == AZIMUTHAL_SCAN)
	{
		_nStart  = (TMP_CBA(clb_start) - LAW_VAL_POS(grp , Angle_min))/ LAW_VAL_POS(grp , Angle_step)  ;
		_nStep   = (TMP_CBA(clb_end)  -  TMP_CBA(clb_start)) / LAW_VAL_POS(grp , Angle_step) + 1;
	}
	else if (LAW_VAL_POS(grp , Focal_type) == LINEAR_SCAN)
	{
		_nStart = TMP_CBA(clb_start) / 100 - 1  ;
		_nStep  =  (TMP_CBA(clb_end)  -  TMP_CBA(clb_start))/ 100 + 1 ;
	}

	for(i = _nStart ; i < _nStart + _nStep ; i++)
	{
		//*****************************************
		temp_value = TMP(clb_sensation_max_DB[i]) / 20.47      ;
		if(fabs(temp_value - amplitude) > tolerance)  return FALSE ;
	}
	return TRUE ;
}
void PACalibrationSensationFinish()
{
	int _bContinue  = IfAllLawCalibrationIsFinished();
	int grp = get_current_group(pp->p_config);
	if(_bContinue)
	{
		pp->cstart_qty = 3 ;
		pp->bRefreshDraw = TRUE ;
		draw_area_all();
	}
	else
	{
		GROUP_VAL_POS(grp , SensationCalibrated) = 1 ;
		gtk_widget_queue_draw (pp->status_area);
		esc_calibration();
	}
}

void UltraSoundCalibrationCallbakWedgeDelay021()
{
	int offset,k;
	int grp = get_current_group(pp->p_config);
	for (offset = 0, k = 0 ; k < grp; k++)
		offset += TMP(beam_qty[k]);
	int _nIndex = offset + TMP(beam_num[grp]);

	int response ;
	((pp->cstart_qty) < 5) ? (pp->cstart_qty) ++ : ((pp->cstart_qty) = 1);
	if(PA_SCAN != get_group_by_id (pp->p_config, get_current_group (pp->p_config))->group_mode)//UT
	{
		if(3 == pp->cstart_qty)//没有角度设置
		{
			++(pp->cstart_qty);
		}
	}
	if(((pp->cstart_qty)) == 5 && (GROUP_VAL_POS(grp , group_mode) != PA_SCAN))
	{
		TMP_CBA(time_start) = ((TMP(measure_data[_nIndex][1])) & 0xfffff);
		TMP_CBA(CalibrationWedgeDelay)  = GetUtWedgeDelayCalicbration(grp , GROUP_VAL_POS(grp , velocity)/100.0);
	}
	if(pp->cstart_qty == 2)
	{
		if(!GROUP_VAL_POS(grp , VelocityCalibrated) )
		{
			response = dialogWarning(pp->window , getDictString(_STRING_CLBWEDGEDELAY_QUEST));
			if(!response)
			{
				esc_calibration();
	            return ;
			}
		}
		// *************************************
		// refer to the the current UT model
		// prepose the echotype
		if(GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_SOUNDPATH )
		{pp->echotype_pos =  0 ;TMP_CBA(distance) = pp->radius1 /1000.0 ;}
		else
		{pp->echotype_pos =  1 ;TMP_CBA(distance) = pp->depth1 / 1000.0 ;}
		//***************************************
		// clear calibration data buffers
		memset( TMP(clb_wedge_position) , 0 , sizeof(int) * setup_MAX_GROUP_LAW_QTY) ;
		memset( TMP(clb_wedge_max_DB)   , 0 , sizeof(int) * setup_MAX_GROUP_LAW_QTY) ;
		if (LAW_VAL_POS (grp , Focal_type) == AZIMUTHAL_SCAN)
		{
			TMP_CBA(clb_start) = LAW_VAL_POS(grp , Angle_min);
			TMP_CBA(clb_end)   = LAW_VAL_POS(grp , Angle_max);  // for calibration
		}
		if(LAW_VAL_POS (grp , Focal_type) == LINEAR_SCAN)
		{
			TMP_CBA(clb_start) = 100 ;
			TMP_CBA(clb_end)   = TMP(beam_qty[grp]) * 100 ;   // for calibration
		}
		//draw_area_calibration();
		pp->clb_flag = 1 ; // draw calibration area need it to be setted
		if(GROUP_VAL_POS(grp , group_mode) != PA_SCAN)
		{
			GROUP_VAL_POS(grp , ascan_envelope)  = TRUE;
		}
		// keep the origin beam offset , for reset calibration in the process
		memcpy((void*)(TMP_CBA(BeamDelay)) , (void*)(GROUP_VAL_POS(grp , beam_delay)) , sizeof(int) * setup_MAX_GROUP_LAW_QTY);
		request_refresh(REFRESH_DRAW_AREA_ALL);
		//draw_area_all();
		//pp->bRefreshDraw = TRUE ;
	}
	else if((pp->cstart_qty) == 1)//Accept
	{
		if(GROUP_VAL_POS(grp , group_mode) == PA_SCAN)
		{
			if(CheckPACalibrationWedgeDelayResult())
			{
				dialogError(pp->window ,  getDictString(_STRING_CLB_DELAY_PA_NOGO));
				pp->cstart_qty = 5;
			}
			else
			{
				PACalibrationWedgeDelayFinish() ;
			}
		}
		else
		{
			if(TMP_CBA(CalibrationWedgeDelay) < 0 || TMP_CBA(CalibrationWedgeDelay) > 50000)
			{
				dialogError(pp->window ,  getDictString(_STRING_CLB_DELAY_UT_NOGO));
				pp->cstart_qty = 5;
			}
			else
			{
				GROUP_VAL_POS(grp , WedgeDelayCalibrated) = 1;
				GROUP_VAL_POS(grp , wedge_delay) =  TMP_CBA(CalibrationWedgeDelay);
				GROUP_VAL_POS(grp , wedge.Probe_delay)  =  TMP_CBA(CalibrationWedgeDelay) ;
				gtk_widget_queue_draw (pp->status_area);
				MultiGroupSendGroupSpi();
				esc_calibration();
			}
		}
	}
}

void ClearCalibrateEnvWedgeDlay()
{
	memset( TMP(clb_wedge_position) , 0 , sizeof(int) * setup_MAX_GROUP_LAW_QTY) ;
	memset( TMP(clb_wedge_max_DB) , 0 , sizeof(int) * setup_MAX_GROUP_LAW_QTY) ;
}

void ClearCalibrateEnvSensitivity()
{
	memset( TMP(clb_sensation_max_DB) , 0 , sizeof(int) * setup_MAX_GROUP_LAW_QTY) ;
}

void ClearCalibrateEnvTCG()
{
	memset( TMP(clb_tcg_max_DB) , 0 , sizeof(int) * setup_MAX_GROUP_LAW_QTY) ;
	memset( TMP(clb_tcg_position) , 0 , sizeof(int) * setup_MAX_GROUP_LAW_QTY) ;
}

void UltraSoundCalibrationCallbackSensation()
{
	int grp = get_current_group(pp->p_config);
	((pp->cstart_qty) < 6) ? (pp->cstart_qty) ++ : ((pp->cstart_qty) = 1);
	if(PA_SCAN != get_group_by_id (pp->p_config, get_current_group (pp->p_config))->group_mode)//UT
	{
		//声速和锲块延迟
		return ;
	}
	if((pp->cstart_qty) == 2)
	{
		if (LAW_VAL_POS (grp , Focal_type) == AZIMUTHAL_SCAN)
		{
			TMP_CBA(clb_start) = LAW_VAL_POS(grp , Angle_min);
			TMP_CBA(clb_end)   = LAW_VAL_POS(grp , Angle_max);  // for calibration
		}

		if(LAW_VAL_POS (grp , Focal_type) == LINEAR_SCAN)
		{
			TMP_CBA(clb_start) = 100 ;
			TMP_CBA(clb_end)   = TMP(beam_qty[grp]) * 100 ;   // for calibration
		}
		pp->clb_flag = 1 ; // draw calibration area need it to be setted
		// keep the origin gain offset , for reset calibration in the process
		//点击start进入A-
        memset( TMP(clb_sensation_max_DB) , 0 , sizeof(int) * setup_MAX_GROUP_LAW_QTY) ;
        request_refresh(REFRESH_DRAW_AREA_ALL);
		//draw_area_all();
		//pp->bRefreshDraw = TRUE ;

	}
	else if((pp->cstart_qty) == 1)//最后一步
	{
		if(isResultAcceptabel())
		{
			SetGainOffsetCalibrated() ;
			//Accept 所做的事情就是把校准之后值显示在控件上
			PACalibrationSensationFinish();
		}
		else
		{
			dialogError(pp->window , getDictString(_STRING_CLB_SENSATION_REJECT));
			pp->cstart_qty = 6 ;
		}
	}
}

void UltraSoundCalibrationCallbackUtWedgeAndVelocity()
{
	int offset,k;
	int grp = get_current_group(pp->p_config);
	for (offset = 0, k = 0 ; k < grp; k++)
		offset += TMP(beam_qty[k]);
	int _nIndex = offset + TMP(beam_num[grp]);

	((pp->cstart_qty) < 6) ? (pp->cstart_qty) ++ : ((pp->cstart_qty) = 1);
	if((pp->cstart_qty) == 2)
	{
		pp->clb_flag = 1 ; // draw calibration area need it to be setted
		GROUP_VAL_POS(grp , ascan_envelope)  = TRUE;
		request_refresh(REFRESH_DRAW_AREA_ALL);
		//draw_area_all();
		//pp->bRefreshDraw = TRUE ;
	}
	else if((pp->cstart_qty) == 5)
	{
		//获取闸门2信息
		TMP_CBA(time_start) = ((TMP(measure_data[_nIndex][1])) & 0xfffff);
	}
	else if((pp->cstart_qty) == 6)
	{
		TMP_CBA(time_end) = ((TMP(measure_data[_nIndex][1])) & 0xfffff);
		pp->vel = cba_ultrasound_velocity();
		TMP_CBA(CalibrationWedgeDelay) = GetUtWedgeDelayCalicbration(grp , pp->vel );
	}
	else if((pp->cstart_qty) == 1)
	{
		if(((pp->vel > 635) && (pp->vel<10000))&&
		((GROUP_VAL_POS(grp , wedge_delay) >= 0) && (GROUP_VAL_POS(grp , wedge_delay) < 50000)))
		{
			//在此调用声速校准函数->此处校准之后的声速用于Wedge Delay校准
			set_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)),
					GROUP_VELOCITY, (int)(100*pp->vel))  ;
			GROUP_VAL_POS(grp , wedge_delay) = TMP_CBA(CalibrationWedgeDelay) ;
			GROUP_VAL_POS(grp , wedge.Probe_delay)  =  TMP_CBA(CalibrationWedgeDelay) ;
			GROUP_VAL_POS(grp , VelocityCalibrated) = 1  ;
			GROUP_VAL_POS(grp , WedgeDelayCalibrated) = 1 ;
			gtk_widget_queue_draw (pp->status_area);
			esc_calibration();
			MultiGroupSendGroupSpi();
			MultiGroupSendFocalSpi();
			RefreshScanInfor() ;

			pp->bRefreshDraw = TRUE ;
		}
		else
		{
			dialogError(pp->window ,  getDictString(_STRING_CLB_UT_NOGO));
			pp->cstart_qty = 6 ;
		}
	}
}

void UltraSoundCalibrationTOFDCallback021()
{
	int grp = get_current_group(pp->p_config);
	((pp->cstart_qty) < 5) ? (pp->cstart_qty) ++ : ((pp->cstart_qty) = 1);
	if((pp->cstart_qty) == 2)
	{
		double _nThickness = GROUP_VAL_POS(grp , part.Thickness)/ 1000.0  ;
		double _nAngle     = LAW_VAL_POS(grp , Angle_min) * G_PI / 18000.0;
		double _nRef       = fabs(GROUP_VAL_POS (grp, wedge.Ref_point)/1000.0) ;
		_nThickness = _nThickness * 2 / 3 ;  // default PCS calculation use 2/3 thickness   !!!
		GROUP_VAL_POS(grp , field_distance[255]) = 2 * (_nThickness * tan(_nAngle) - _nRef) ;
		GROUP_VAL_POS(grp , field_distance[254])     = 2 *  _nThickness * tan(_nAngle)  ;

		pp->clb_flag = 1 ;
		set_overlay_cursor (pp->p_config, 1 );
		draw_area_all() ;
		pp->bRefreshDraw = TRUE ;
	}
	else if((pp->cstart_qty) == 5)
	{
		UtTOFDCalicbration(grp) ;
	}
	else if((pp->cstart_qty) == 1)
	{
		if(((TMP_CBA(CalibrationWedgeDelay) >= 0 ) &&
	    (TMP_CBA(CalibrationWedgeDelay) < 50000)) &&
		((GROUP_VAL_POS(grp , field_distance[253]) > 0)&&(GROUP_VAL_POS(grp , field_distance[253]) < 100)) &&
		(GROUP_VAL_POS(grp , field_distance[254])> 2* GROUP_VAL_POS(grp , field_distance[253])))
		{
			//在此调用声速校准函数->此处校准之后的声速用于Wedge Delay校准
			GROUP_VAL_POS(grp , wedge_delay)         =  TMP_CBA(CalibrationWedgeDelay) ;
			GROUP_VAL_POS(grp , wedge.Probe_delay)   =  TMP_CBA(CalibrationWedgeDelay) ;
			GROUP_VAL_POS(grp , wedge.Ref_point)     =  GROUP_VAL_POS(grp , field_distance[253]) * 1000 ;
			GROUP_VAL_POS(grp , field_distance[0])   =  -GROUP_VAL_POS(grp , field_distance[253])  ;
			GROUP_VAL_POS(grp , wedge.Primary_offset) =  GROUP_VAL_POS(grp , field_distance[253]) * 1000      ;
			GROUP_VAL_POS(grp , WedgeDelayCalibrated) = 1 ;
			gtk_widget_queue_draw (pp->status_area)      ;
			esc_calibration();
			MultiGroupSendGroupSpi();
			MultiGroupSendFocalSpi();
			RefreshScanInfor() ;
			pp->bRefreshDraw = TRUE ;
		}
		else
		{
			dialogError(pp->window ,  getDictString(_STRING_CLB_UT_NOGO));
			pp->cstart_qty = 5 ;
		}
	}
}

void UltraSoundCalibrationTCGCallback021Step2()
{
	int grp = get_current_group(pp->p_config);
	if(!pp->clb_flag)
	{
		GROUP_VAL_POS(grp , SizingCurves.bApplyToAllLaws) = -1 ;  // 校准时更新当前点位置，幅度都希望更改到所有聚焦法则
		GROUP_VAL_POS(grp , SizingCurves.curve_pos ) = 3 ; // SHOW TCG
		GROUP_VAL_POS(grp , SizingCurves.mode_pos )  = 0 ; // setup mode, useless ,如不这样在退出校准进入SIZINGCURVE，有可能不需要询问是否更新所有聚焦法则
		GROUP_VAL_POS(grp , SizingCurves.point_pos ) = 1 ; // setup mode, useless
		TMP_CBA(bNewPoint) = 0 ;
		if (LAW_VAL_POS (grp , Focal_type) == AZIMUTHAL_SCAN)
		{
			TMP_CBA(clb_start) = LAW_VAL_POS(grp , Angle_min);
			TMP_CBA(clb_end)   = LAW_VAL_POS(grp , Angle_max);  // for calibration
		}
		if(LAW_VAL_POS (grp , Focal_type) == LINEAR_SCAN)
		{
			TMP_CBA(clb_start) = 100 ;
			TMP_CBA(clb_end)   = TMP(beam_qty[grp]) * 100 ;   // for calibration
		}
		pp->clb_flag = 1 ; // draw calibration area need it to be setted

		if(GROUP_VAL_POS(grp , SizingCurves.dac_point_qty) == 1)
		{
			AddDacPoint(grp) ;
			enable_tcg( grp , TRUE) ;
			SendTcgSpi(grp , -1) ;
		}

		set_overlay_sizing_curves(pp->p_config , 1) ;
		request_refresh(REFRESH_DRAW_AREA_ALL);
		//draw_area_all();
		//pp->bRefreshDraw = TRUE ;
	}

	memset( (void*)TMP(clb_tcg_max_DB)   , 0 , sizeof(int) * setup_MAX_GROUP_LAW_QTY)  ;
	memset( (void*)TMP(clb_tcg_position) , 0 , sizeof(int) * setup_MAX_GROUP_LAW_QTY)  ;
}

void UltraSoundCalibrationTCGCallback021Step5()
{
	int grp = get_current_group(pp->p_config);
	int ret = cba_ultrasound_TCG()  ;
	if(ret)
	{
		if(ret == -1)
		{
			dialogError(pp->window ,getDictString(_STRING_TCG_CLB_ERROR1) ) ;
			pp->cstart_qty = 5 ;
			draw_menu3(0, NULL);
		}
		else if(ret == -2)
		{
			dialogError(pp->window ,getDictString(_STRING_TCG_CLB_ERROR2) ) ;
			pp->cstart_qty = 5 ;
			draw_menu3(0, NULL);
		}
	}
	else
	{
		DrawDisplayWindowFrameCalibration() ;
		SendTcgSpi( grp , -1);
	}


}

void UltraSoundCalibraionCallbackTCG()
{
	((pp->cstart_qty) < 6) ? (pp->cstart_qty) ++ : ((pp->cstart_qty) = 1);
	int grp = get_current_group(pp->p_config);
	switch(pp->cstart_qty)
	{
	case 1:
		break;
	case 2:
		UltraSoundCalibrationTCGCallback021Step2();
		break;
	case 3:
		break;
	case 4:
		ClearCalibrateEnvTCG();
		break;
	case 5:
		UltraSoundCalibrationTCGCallback021Step5();
		break;
	case 6:
        if(GROUP_VAL_POS(grp , SizingCurves.point_pos) >= (GROUP_VAL_POS(grp , SizingCurves.dac_point_qty) - 1)) {
            TMP_CBA(bNewPoint) = 0 ;
            AddDacPoint(grp) ;
            enable_tcg( grp , TRUE) ;
            SendTcgSpi(grp , -1) ;
            gtk_widget_queue_draw (pp->vboxtable);
        } else {
            gtk_widget_queue_draw (pp->vboxtable);
            GROUP_VAL_POS(grp , SizingCurves.point_pos) += 1 ;
        }
        pp->cstart_qty = 2 ;
        pp->pos_pos = MENU3_STOP;
        draw_menu3(0, NULL);
		break;
	case 7:
		esc_calibration() ;
		break;
	default:
		break;
	}
}

void UltraSoundCalibrationCallback021()
{
	//pp->clb_flag = 1 ; // draw calibration area need it to be setted
	int grp = get_current_group(pp->p_config);
	if(GROUP_VAL_POS(grp , tx_rxmode1) == TOFD)
	{
		pp->cmode_pos = 0 ;
		UltraSoundCalibrationTOFDCallback021();
	}
	else
	{
		switch(pp->cmode_pos)
		{
			case 0://Velocity
				UltraSoundCalibrationVelocityCallback021();
				break;
			case 1://Wedge Delay
				UltraSoundCalibrationCallbakWedgeDelay021();
				break;
			case 2://Sensitivity
				if(GROUP_VAL_POS(grp , group_mode) == PA_SCAN)
				    UltraSoundCalibrationCallbackSensation();
				else
					UltraSoundCalibrationCallbackUtWedgeAndVelocity();
				break;
			case 3://TCG
				UltraSoundCalibraionCallbackTCG();
				break;
			default:break;
		}
	}

}

AWS_RANGE g_AwsClassTable[2][5][3][4] = {
		{
				{
						{        /* stress type tensile :: thickness zoom 0  :: angle  45'*/ /*class a b c d*/
								{ .up = 0 , .down = 0 } ,{.up = 0 , .down = 0 } ,{.up = 0 , .down = 0 } , {.up = 0 , .down = 0 }
						},
						{		 /* stress type tensile :: thickness zoom 0  :: angle  60'*//*class a b c d*/
								 { .up = 0 , .down = 0 } ,{.up = 0 , .down = 0 } ,{.up = 0 , .down = 0 } , {.up = 0 , .down = 0 }
						},
						{		 /* stress type tensile :: thickness zoom 0  :: angle  70'*//*class a b c d*/
								{ .up = 10 , .down = -1000 } ,{.up = 11 , .down = 10 } ,{.up = 12 , .down = 11 } , {.up = 1000 , .down = 12 }
						}
				}	,
				{
						{        /* stress type tensile :: thickness zoom 1  :: angle  45'*//*class a b c d*/
								 { .up = 0 , .down = 0 } ,{.up = 0 , .down = 0 } ,{.up = 0 , .down = 0 } , {.up = 0 , .down = 0 }
						},
						{		 /* stress type tensile :: thickness zoom 1  :: angle  60'*//*class a b c d*/
								 { .up = 0 , .down = 0 } ,{.up = 0 , .down = 0 } ,{.up = 0 , .down = 0 } , {.up = 0 , .down = 0 }
						},
						{		 /* stress type tensile :: thickness zoom 1  :: angle  70'*//*class a b c d*/
								{ .up = 8 , .down = -1000 } ,{.up = 9 , .down = 8 } ,{.up = 10 , .down = 9 } , {.up = 1000 , .down = 10 }
						}
				}   ,
				{
						{        /* stress type tensile :: thickness zoom 2  :: angle  45'*//*class a b c d*/
								 { .up = 9 , .down = -1000 } ,{.up = 11 , .down = 9 } ,{.up = 13 , .down = 11 } , {.up = 1000 , .down = 13 }
						},
						{		 /* stress type tensile :: thickness zoom 2  :: angle  60'*//*class a b c d*/
								 { .up = 7 , .down = -1000 } ,{.up =  9 , .down = 7 } ,{.up = 11 , .down = 9  } , {.up = 1000 , .down = 11 }
						},
						{		 /* stress type tensile :: thickness zoom 2  :: angle  70'*//*class a b c d*/
								 { .up = 4 , .down = -1000 } ,{.up = 6 , .down = 4 } ,{.up = 8 , .down = 6 } , {.up = 1000 , .down = 8 }
						}
				}	,
				{
						{        /* stress type tensile :: thickness zoom 3  :: angle  45'*//*class a b c d*/
								 { .up = 6 , .down = -1000 } ,{.up = 8 , .down = 6 } ,{.up = 10 , .down = 8 } , {.up = 1000 , .down = 10 }
						},
						{		 /* stress type tensile :: thickness zoom 3  :: angle  60'*//*class a b c d*/
								 { .up = 4 , .down = -1000 } ,{.up = 6 , .down = 4 } ,{.up = 8 , .down = 6 } , {.up = 1000 , .down = 8 }
						},
						{		 /* stress type tensile :: thickness zoom 3  :: angle  70'*//*class a b c d*/
								 { .up = 1 , .down = -1000 } ,{.up = 3 , .down = 1 } ,{.up = 5 , .down = 3 } , {.up = 1000 , .down = 5 }
						}
				}	,
				{
						{        /* stress type tensile :: thickness zoom 4  :: angle  45'*//*class a b c d*/
								 { .up = 3 , .down = -1000 } ,{.up = 5 , .down = 3 } ,{.up = 7 , .down = 5 } , {.up = 1000 , .down = 7 }
						},
						{		 /* stress type tensile :: thickness zoom 4  :: angle  60'*//*class a b c d*/
								 { .up = 1 , .down = -1000 } ,{.up = 3 , .down = 1 } ,{.up = 5 , .down = 3 } , {.up = 1000 , .down = 5 }
						},
						{		 /* stress type tensile :: thickness zoom 4  :: angle  70'*//*class a b c d*/
								 { .up = -2 , .down = -1000 } ,{.up = 0 , .down = -2 } ,{.up = 2 , .down = 0 } , {.up = 1000 , .down = 2 }
						}
				}
		},
		{
				{
						{        /* stress type compressive :: thickness zoom 0  :: angle  45'*/
								 { .up = 0 , .down = 0 } ,{.up = 0 , .down = 0 } ,{.up = 0 , .down = 0 } , {.up = 0 , .down = 0 }
						},
						{		 /* stress type compressive :: thickness zoom 0  :: angle  60'*/
								 { .up = 0 , .down = 0 } ,{.up = 0 , .down = 0 } ,{.up = 0 , .down = 0 } , {.up = 0 , .down = 0 }
						},
						{		 /* stress type compressive :: thickness zoom 0  :: angle  70'*/
								{ .up = 5 , .down = -1000 } ,{.up = 6 , .down = 5 } ,{.up = 7 , .down = 6 } , {.up = 1000 , .down = 7 }
						}
					}	,
					{
						{        /* stress type compressive :: thickness zoom 1  :: angle  45'*/
								 { .up = 0 , .down = 0 } ,{.up = 0 , .down = 0 } ,{.up = 0 , .down = 0 } , {.up = 0 , .down = 0 }
						},
						{		 /* stress type compressive :: thickness zoom 1  :: angle  60'*/
								 { .up = 0 , .down = 0 } ,{.up = 0 , .down = 0 } ,{.up = 0 , .down = 0 } , {.up = 0 , .down = 0 }
						},
						{		 /* stress type compressive :: thickness zoom 1  :: angle  70'*/
								{ .up = 2 , .down = -1000 } ,{.up = 3 , .down = 2 } ,{.up = 4 , .down = 3 } , {.up = 1000 , .down = 4 }
						}
					}   ,
					{
						{        /* stress type compressive :: thickness zoom 2  :: angle  45'*/
								{ .up = 3 , .down = -1000 } ,{.up = 5 , .down = 3 } ,{.up = 7 , .down = 5 } , {.up = 1000 , .down = 7 }
						},
						{		 /* stress type compressive :: thickness zoom 2  :: angle  60'*/
								{ .up = 1 , .down = -1000 } ,{.up = 3 , .down = 1 } ,{.up = 5 , .down = 3 } , {.up = 1000 , .down = 5 }
						},
						{		 /* stress type compressive :: thickness zoom 2  :: angle  70'*/
								{ .up = -2 , .down = -1000 } ,{.up = 0 , .down = -2 } ,{.up = 2 , .down = 0 } , {.up = 1000 , .down = 2 }
						}
					}	,
					{
						{        /* stress type compressive :: thickness zoom 3  :: angle  45'*/
								 { .up = 0 , .down = -1000 } ,{.up = 2 , .down = 0 } ,{.up = 4 , .down = 2 } , {.up = 1000 , .down = 4 }
						},
						{		 /* stress type compressive :: thickness zoom 3  :: angle  60'*/
								 { .up = -2 , .down = -1000 } ,{.up = 0 , .down = -2 } ,{.up = 2 , .down = 0 } , {.up = 1000 , .down = 2 }
						},
						{		 /* stress type compressive :: thickness zoom 3  :: angle  70'*/
								 { .up = -5 , .down = -1000 } ,{.up = -3 , .down = -5 } ,{.up = 2 , .down = -3 } , {.up = 1000 , .down = 2 }
						}
					}	,
					{
						{        /* stress type compressive :: thickness zoom 4  :: angle  45'*/
								 { .up = -1 , .down = -1000 } ,{.up = 1 , .down = -1 } ,{.up = 3 , .down = 1 } , {.up = 1000 , .down = 3 }
						},
						{		 /* stress type compressive :: thickness zoom 4  :: angle  60'*/
								 { .up = -4 , .down = -1000 } ,{.up = -2 , .down = -4 } ,{.up = 2 , .down = -2 } , {.up = 1000 , .down = 2 }
						},
						{		 /* stress type compressive :: thickness zoom 4  :: angle  70'*/
								 { .up = -7 , .down = -1000 } ,{.up = -5 , .down = -7 } ,{.up = 2 , .down = -5 } , {.up = 1000 , .down = 2 }
						}
					}
		}
} ;

enum
{
A = 0 ,
B ,
C ,
D ,
Invalidate
} ;



int LookupTabelAWS(int nAngleType_ , double nValue_)
{
	int ret = Invalidate;
	int _nAngle  ;

	int _nGroup = get_current_group(pp->p_config);
	int    _nPartStressType = GROUP_VAL_POS(_nGroup , part.weldtype_pos)  ;
	double _nThickness = GROUP_VAL_POS(_nGroup , part.Thickness) / 1000.0 ;

	int    _nThicknessZoom ;
	if(_nThickness < 8)                              return Invalidate   ;
	else if(_nThickness >= 8  && _nThickness < 20)   _nThicknessZoom = 0 ;
	else if(_nThickness >= 20 && _nThickness < 38)   _nThicknessZoom = 1 ;
	else if(_nThickness >= 38 && _nThickness < 60)   _nThicknessZoom = 2 ;
	else if(_nThickness >= 60 && _nThickness < 100)  _nThicknessZoom = 3 ;
	else if(_nThickness >= 100 && _nThickness< 200)  _nThicknessZoom = 4 ;
	else if(_nThickness >= 200 )                      return Invalidate  ;

	if((LAW_VAL_POS(_nGroup, Focal_type) == AZIMUTHAL_SCAN) && (GROUP_VAL_POS(_nGroup , group_mode) == PA_SCAN))
	{
		_nAngle = (LAW_VAL_POS(_nGroup , Angle_min)+ TMP(beam_num[_nGroup]) * LAW_VAL_POS(_nGroup, Angle_step));
	}
	else
	{
		_nAngle = LAW_VAL_POS(_nGroup , Angle_min)  ;
	}
	if(nAngleType_ == 3)
	{
		if     (_nAngle == 4500) nAngleType_ = 0 ;
		else if(_nAngle == 6000) nAngleType_ = 1 ;
		else if(_nAngle == 7000) nAngleType_ = 2 ;
		else                     return Invalidate ;
	}
	int i ;
	for(i = 0 ; i < 4 ; i++)
	{
		if((nValue_ >g_AwsClassTable[_nPartStressType][_nThicknessZoom][nAngleType_][i].down)&&
				(nValue_ <= g_AwsClassTable[_nPartStressType][_nThicknessZoom][nAngleType_][i].up))
		{
			return i ;
		}
	}

	return ret;
}

void CalculateCodeCalibraionAWS()
{
	double _nAmplitude , _nGainReference;
	double _nScale    ;
	double _nGain     ;
	int     offset, k ;
	char* markup ;
	int grp = get_current_group(pp->p_config);
	for (offset = 0, k = 0 ; k < grp; k++)
		offset += TMP(beam_qty[k]);
	int _nIndex = offset + TMP(beam_num[grp]);
	_nAmplitude =  (((TMP(measure_data[_nIndex][1]))>>20) & 0x00000fff)/20.47;//满屏时200% 4095
    //_nAmplitude = 40.0 ;
    _nGain  = group_get_gain(grp) ;
	_nScale =  80.0 /_nAmplitude  ;
	_nGainReference = (short)(log10(_nScale)*2000) + _nGain;
    if(_nGainReference < 0 || _nGainReference > 8000) {
        return ;
    }

    group_set_gain(grp, (gshort)(_nGainReference));
    group_set_refgain(grp, (gshort)(_nGainReference));
    TMP(group_spi[grp]).gain   = (gshort)(_nGainReference) / 10 ;
	send_group_spi (grp);

	set_group_db_ref (pp->p_config, grp , 1) ;
	markup = g_markup_printf_escaped (
				"<span foreground='white' font_desc='16'>%0.1f(%0.1f)</span>",
				0.0 , _nGainReference / 100.0);

	gtk_label_set_markup (GTK_LABEL(pp->label[GAIN_VALUE]),markup);
	g_free(markup);

	GROUP_VAL_POS(grp , AwsCalibration.nReferenceGain) = _nGainReference / 100.0;
	GROUP_VAL_POS(grp , AwsCalibration.nPosition)      = TMP(measure_data[_nIndex][1]) & 0xfffff  ;
}

void CodeCalibraionAWSCallback021()
{
	int grp = get_current_group(pp->p_config);
	((pp->cstart_qty) < 6) ? (pp->cstart_qty) ++ : ((pp->cstart_qty) = 1);

	if((pp->cstart_qty) == 2) // start
	{
		pp->clb_flag = 1 ;
		// envelope needs to be opened
		GROUP_VAL_POS(grp , ascan_envelope)  = TRUE;

		set_group_db_ref (pp->p_config, grp, 1);
		char* markup = g_markup_printf_escaped (
					"<span foreground='white' font_desc='16'>%0.1f(%0.1f)</span>",
                    (group_get_gain(grp) - group_get_refgain(grp)) / 100.0,
                    group_get_refgain(grp) / 100.0);
		gtk_label_set_markup (GTK_LABEL(pp->label[GAIN_VALUE]),markup);
		g_free(markup);
		pp->bRefreshDraw = TRUE ;
	}
	else if((pp->cstart_qty) == 5) // calibraion
	{
		CalculateCodeCalibraionAWS();
	}
	else if((pp->cstart_qty) == 6) // accept
	{
		int     offset, k ;
		for (offset = 0, k = 0 ; k < grp; k++)
			offset += TMP(beam_qty[k]);
		int _nIndex = offset + TMP(beam_num[grp]);
		double _nAmplitude ; // =  (((TMP(measure_data[_nIndex][1]))>>20) & 0xfff)/20.47;//满屏时200% 4095
		int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ;
		if(_bRectifier)
		{
			_nAmplitude = (((TMP(measure_data[_nIndex][1]))>>20) & 0x00000fff)/20.47;
		}
		else
		{
			_nAmplitude = (signed short)((TMP(measure_data[_nIndex][1]))>>16) ;
			_nAmplitude = _nAmplitude / (10.24 * 16);
		}

		if(fabs(_nAmplitude - 80) < 5)
		{
			GROUP_VAL_POS(grp , AwsCalibration.bCalibrated) = TRUE ;
			esc_calibration();
		}
		else
		{
			pp->cstart_qty = 5 ;
		}

	}
}

void CodeCalibraionCallback021()
{
	//int grp = get_current_group(pp->p_config);

	switch(pp->scode_pos)
	{
		case 0://API
			dialogError(pp->window ,"Not Support Currently!");
			break;
		case 1://AWS
			CodeCalibraionAWSCallback021();
			break;
		default:break;
	}

}

void CalibrationCallback021()
{
	switch(pp->ctype_pos)
	{
		case 0://Encoder
			EncoderCalibrationCallback021();
			break;
		case 1://Ultrasound
			UltraSoundCalibrationCallback021();
			break;
		case 2://Code
			CodeCalibraionCallback021();

			break;
		default:break;
	}
}

void VelocityCalibrationRestart()
{
	pp->cstart_qty = 2;
	pp->vel = 0;
	gtk_widget_queue_draw (pp->status_area);
	pp->pos_pos = MENU3_STOP;
	draw_menu2(0);
	draw_menu3(0, NULL);
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;
}
void WedgeDelaySensationCalibrationRestart()
{
	pp->pos_pos = MENU3_STOP;
	pp->cstart_qty = 2;
	draw_menu2(0);
	draw_menu3(0, NULL);
}


void ClearSensitivityCalibration()
{
	int k ;
	int grp = get_current_group(pp->p_config);
	for (k = 0; k < TMP(beam_qty[grp]); k++)
	{
		GROUP_VAL_POS(grp , gain_offset[k]) =  0 ;
	}
	GROUP_VAL_POS(grp , SensationCalibrated) = 0 ;
	MultiGroupSendGroupSpi();
	MultiGroupSendFocalSpi();
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;
	gtk_widget_queue_draw (pp->status_area);
	gtk_widget_queue_draw (pp->vboxtable);
}

int CalibrationCallback022()
{
	int	grp = get_current_group (pp->p_config);
	switch(pp->ctype_pos) //Code
	{
	case 2:
		switch(pp->scode_pos)
		{
		case 1:// AWS
			if((pp->cstart_qty) == 5)
			{
				AWS_D_15_P pInfo = &(GROUP_VAL_POS(grp , AwsCalibration));
                memset(pInfo , 0 , sizeof(AWS_D_15)) ;
                group_set_refgain(grp, 0);

        		set_group_db_ref (pp->p_config, grp, 1);
        		char* markup = g_markup_printf_escaped (
        					"<span foreground='white' font_desc='16'>%0.1f(%0.1f)</span>",
                            (group_get_gain(grp) - group_get_refgain(grp)) / 100.0,
                            group_get_refgain(grp) / 100.0);
        		gtk_label_set_markup (GTK_LABEL(pp->label[GAIN_VALUE]),markup);
        		g_free(markup);
    			ascanResetEnvelope(get_current_group(pp->p_config)) ;
    			ascanResetPeak(get_current_group(pp->p_config)) ;
        		pp->bRefreshDraw = TRUE ;
			}
			break;
		}
		break;
	}
	return 0;
}

int CalibrationCallback023()
{
	int	grp = get_current_group (pp->p_config);
    int ret = 0 ;
	if(pp->ctype_pos == 1) //Ultrasound
	{
		switch(pp->cmode_pos)
		{
		case 0:// velocity calibration restart
			if((pp->cstart_qty) == 6)
			{
				VelocityCalibrationRestart();
				ret = TRUE ;
			}
			break;
		case 1:
			if (pp->cstart_qty == 5)//Clear Calibrate
			{
				if(GROUP_VAL_POS(grp , group_mode) == PA_SCAN)
				{//PA wedge delay calibration  clear evn
					ClearCalibrateEnvWedgeDlay() ;
				}
				else
				{// UT wedge delay calibration restart
					ClearCalibrateEnvWedgeDlay() ;
					WedgeDelaySensationCalibrationRestart();
        			ascanResetEnvelope(get_current_group(pp->p_config)) ;
        			ascanResetPeak(get_current_group(pp->p_config)) ;
					pp->bRefreshDraw = TRUE ;
					ret = TRUE  ;
				}
			}
			break;
		case 2:
			if(pp->cstart_qty == 6)
			{
				ClearCalibrateEnvSensitivity();
			}
			break;
		case 3:// TCG
			if(pp->cstart_qty == 4)
			{
				ClearCalibrateEnvTCG();
			}
			break;
		default:
			break;
		}
	}
	else if(pp->ctype_pos == 2) //code
	{
		AWS_D_15_P pInfo = &(GROUP_VAL_POS(grp , AwsCalibration));
		switch(pp->scode_pos)
		{
			case 0://API
				//dialogError(pp->window ,"Not Support Currently!");
				break;
			case 1://AWS
				if (pp->cstart_qty == 4)//Clear Calibrate
				{
                    memset(pInfo , 0 , sizeof(AWS_D_15)) ;
                    group_set_refgain(grp, 0);
        			ascanResetEnvelope(get_current_group(pp->p_config)) ;
        			ascanResetPeak(get_current_group(pp->p_config)) ;
        			pp->bRefreshDraw = TRUE ;
                    //printf("aws clear\n");
				}
				else if(pp->cstart_qty == 5) //restart
				{
					//printf("aws restart\n");
                    memset(pInfo , 0 , sizeof(AWS_D_15)) ;
                    group_set_refgain(grp, 0);
                    pp->cstart_qty = 2 ;
        			ascanResetEnvelope(get_current_group(pp->p_config)) ;
        			ascanResetPeak(get_current_group(pp->p_config)) ;
                    pp->bRefreshDraw = TRUE ;
				}
				break;
			default:break;
		}

	}
	return ret ;
}

void CalibrationCallback024()
{
	int ret ;
	int grp = get_current_group(pp->p_config);//当前group
	switch(pp->cmode_pos)
	{
	   case 0:
		   if (pp->cstart_qty == 1 )//Clear Calibrate
		   {
			   GROUP_VAL_POS(grp , VelocityCalibrated) = 0 ;
			   gtk_widget_queue_draw (pp->status_area);
		   }
		   break;
	   case 1://wedge delay
		   if (pp->cstart_qty == 1 )//Clear Calibrate
		   {
			   ClearCalibrationWedgeDelayAll();
			   gtk_widget_queue_draw (pp->status_area);
		   }
		   else if (pp->cstart_qty == 5)//Calibrate
		   {
			   ret = cba_ultrasound_wedgedelay();
			   if (ret)
			   {
				   dialogError(pp->window, getDictString(_STRING_CLB_DELAY_PA_NOGO));
			   }
			   else
			   {
				   update_spi_after_wedge_delay_calibration(grp);
			   }
		   }
		   break;
	   case 2://sensitivity
		   if (pp->cstart_qty == 1 || pp->cstart_qty == 2)//Clear Calibrate
		   {
				ClearSensitivityCalibration();
				gtk_widget_queue_draw (pp->status_area);
		   }
		   else if (pp->cstart_qty == 6)//Calibrate
		   {
			   if(GROUP_VAL_POS(grp , group_mode) == PA_SCAN)
			   {
				   ret = cba_ultrasound_sensitivity();
				   if (ret == -1)
				   {
					   dialogError(pp->window , getDictString(_STRING_CLB_SENSATION_NOGO));
				   }
				   else if (ret == -2)
				   {
					   dialogError(pp->window , getDictString(_STRING_CLB_SENSATION_AMP_NOGO));
				   }
			   }
			   else
			   {
				   pp->cstart_qty = 2 ;
				   pp->pos_pos = MENU3_STOP;
				   ascanResetEnvelope(get_current_group(pp->p_config)) ;
				   ascanResetPeak(get_current_group(pp->p_config)) ;
				   pp->bRefreshDraw = TRUE ;
				   draw_menu2(0);
				   draw_menu3(0, NULL);
				   return ;
			   }
		   }
		   break;
	   case 3: // TCG
		   if (pp->cstart_qty == 1) {
                GROUP_VAL_POS(grp , SizingCurves.dac_point_qty) = 2;
                data_235_del_point(NULL);
		        enable_tcg(grp , FALSE) ;
                GROUP_VAL_POS(grp , SizingCurves.curve_pos) = 0;
           } else if (pp->cstart_qty == 5) {
               //UltraSoundCalibrationTCGCallback021Step5();
		       esc_calibration() ;
           }
#if 0
		   if (pp->cstart_qty == 6)//Next Point or Add Point
		   {
			   if(GROUP_VAL_POS(grp , SizingCurves.point_pos) >= (GROUP_VAL_POS(grp , SizingCurves.dac_point_qty) - 1))
			   {
				   if(TMP_CBA(bNewPoint))
				   {
					   TMP_CBA(bNewPoint) = 0 ;
					   pp->cstart_qty = 2 ;
				   }
				   else
				   {
						TMP_CBA(bNewPoint) = -1 ;
						AddDacPoint(grp) ;
						enable_tcg( grp , TRUE) ;
						SendTcgSpi(grp , -1) ;
						gtk_widget_queue_draw (pp->vboxtable);
				   }
			   }
			   else
			   {
				   gtk_widget_queue_draw (pp->vboxtable);
				   GROUP_VAL_POS(grp , SizingCurves.point_pos) += 1 ;
				   pp->cstart_qty = 2 ;
			   }
			   pp->pos_pos = MENU3_STOP;
			   draw_menu3(0, NULL);
		   }
#endif
		   break;
	   default:
		   break;
	}
}

int CalibrationCallback025()
{
	int ret = 0 ;
	int _nGroupId = get_current_group(pp->p_config) ;

	switch(pp->ctype_pos)
	{
		case 0: // encoder
			if((pp->cstart_qty) == 4)
			{
				(pp->cstart_qty) = 2;
			}
			break;
		case 1: //ultrasound
			switch(pp->cmode_pos)
			{
				case 0: // ultrasound
					if((pp->cstart_qty == 5) && (GROUP_VAL_POS(_nGroupId , tx_rxmode1) == TOFD) )//Restart
					{
						pp->cstart_qty = 2  ;
						ret = TRUE ;
					}
					break;
				case 1: // wedge
					if(pp->cstart_qty == 5)//Restart
					{
						ClearCalibrationWedgeDelay() ;
						ClearCalibrateEnvWedgeDlay();
						WedgeDelaySensationCalibrationRestart();
						ret = TRUE ;
					}

					break;
				case 2: // sensation
					//if (pp->cstart_qty == 1 || pp->cstart_qty == 2)//Clear Calibrate
					//{
					//	ClearSensitivityCalibration();
					//}
					//else if(pp->cstart_qty == 5)//Clear env
					if(pp->cstart_qty == 5)//Clear env
					{
						ClearCalibrateEnvSensitivity();
					}
					else if(pp->cstart_qty == 6)//Restart
					{
						ClearCalibrateEnvSensitivity();
						ClearSensitivityCalibration() ;
						WedgeDelaySensationCalibrationRestart();
						ret = TRUE ;
					}
					break;
				case 3: // TCG
					if(pp->cstart_qty == 5)//cancel current
					{
#if 0
						TMP_CBA(bNewPoint) = 0 ;
						ClearCalibrateEnvTCG();
						//SynocDACParametersOfCurrentPoint(_nGroupId);
						ClearTCGCalibrationCurrentPoint( _nGroupId) ;
						GROUP_VAL_POS(_nGroupId , SizingCurves.point_pos) -= 1 ;
						SendTcgSpi(_nGroupId , -1) ;
						gtk_widget_queue_draw (pp->vboxtable);
						pp->pos_pos = MENU3_STOP;
						draw_menu3(0, NULL);
#endif
                        data_235_del_point(NULL);
					}
					break;
			}
			break;
		case 2:// code
			break;
		case 3:
			break;
	}

	return ret ;
}

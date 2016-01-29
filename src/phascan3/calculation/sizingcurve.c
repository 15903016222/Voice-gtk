/*
 * sizingcurve.c
 *
 *  Created on: 2012-10-12
 *      Author: wolflord
 */

#include "../drawui.h"
#include "../callback.h"
#include "../calibration.h"
#include <stdio.h>
#include <math.h>


void InitSizingCurveParameters(int nGroupId_)
{
	int i ;
	for(i = 0 ; i < setup_MAX_GROUP_LAW_QTY ; i++)
	{
		GROUP_VAL_POS(nGroupId_ , SizingCurves.amplitude[i][0])=80000;
		GROUP_VAL_POS(nGroupId_ , SizingCurves.amplitude[i][1])=80000;
		GROUP_VAL_POS(nGroupId_ , SizingCurves.amplitude[i][2])=80000;

		GROUP_VAL_POS(nGroupId_ , SizingCurves.position[i][0]) =1000;
		GROUP_VAL_POS(nGroupId_ , SizingCurves.position[i][1]) =2000;
		GROUP_VAL_POS(nGroupId_ , SizingCurves.position[i][2]) =3000;
		GROUP_VAL_POS(nGroupId_ , SizingCurves.dac_ref_ampl[i])= 800;
	}

	GROUP_VAL_POS(nGroupId_ , SizingCurves.linear_ref_ampl)= 800;
	GROUP_VAL_POS(nGroupId_ , SizingCurves.ref_ampl_offset)=0;
	GROUP_VAL_POS(nGroupId_ , SizingCurves.curve_step)=50;
	GROUP_VAL_POS(nGroupId_ , SizingCurves.point_pos)=0;
	GROUP_VAL_POS(nGroupId_ , SizingCurves.dac_point_qty) = 1;

	GROUP_VAL_POS(nGroupId_ , SizingCurves.mat_atten)=10;
	GROUP_VAL_POS(nGroupId_ , SizingCurves.delay)=1000;
}


//**********************************************
//*******  Copy dac parameters of setted beam id to all beams
//**********************************************
void SynocDACParameters(int nGroupId_ , int nBeamNo_ )
{
	int i ;
	int _nTmpPos[16] , _nTmpAmp[16]  ;
	int _nBeamQty = TMP(beam_qty[nGroupId_]);
	int _nReference = GROUP_VAL_POS(nGroupId_ , SizingCurves.dac_ref_ampl[nBeamNo_]);
	int _nPointQty= GROUP_VAL_POS(nGroupId_ , SizingCurves.dac_point_qty);
	memcpy( (void*)_nTmpPos , (void*)GROUP_VAL_POS(nGroupId_ , SizingCurves.position[nBeamNo_]) , sizeof(int) * _nPointQty) ;
	memcpy( (void*)_nTmpAmp , (void*)GROUP_VAL_POS(nGroupId_ , SizingCurves.amplitude[nBeamNo_]) , sizeof(int) * _nPointQty) ;
	for (i = 0 ; i< _nBeamQty ; i++)
	{
		GROUP_VAL_POS(nGroupId_ , SizingCurves.dac_ref_ampl[i]) = _nReference  ;
		memcpy( (void*)GROUP_VAL_POS(nGroupId_ , SizingCurves.position[i]) , (void*)_nTmpPos , sizeof(int) * _nPointQty) ;
		memcpy( (void*)GROUP_VAL_POS(nGroupId_ , SizingCurves.amplitude[i]) , (void*)_nTmpAmp ,sizeof(int) * _nPointQty) ;
	}
}

//**********************************************
//*******  Copy dac parameters of setted beam id to all beams  (current point)
//**********************************************
void SynocDACParametersOfCurrentPoint(int nGroupId_)
{
	int i ;
	int _nTmpPos, _nTmpAmp ;
	int _nBeamQty = TMP(beam_qty[nGroupId_]);
	int _nBeamNo  = TMP(beam_num[nGroupId_]);
	int _nPointPos= GROUP_VAL_POS(nGroupId_ , SizingCurves.point_pos);
	_nTmpPos  = GROUP_VAL_POS(nGroupId_ , SizingCurves.position[_nBeamNo][_nPointPos]) ;
	_nTmpAmp  = GROUP_VAL_POS(nGroupId_ , SizingCurves.amplitude[_nBeamNo][_nPointPos]) ;

	for (i = 0 ; i< _nBeamQty ; i++)
	{
		GROUP_VAL_POS(nGroupId_ , SizingCurves.position[i][_nPointPos]) = _nTmpPos ;
		GROUP_VAL_POS(nGroupId_ , SizingCurves.amplitude[i][_nPointPos]) = _nTmpAmp;
	}
}

void ClearTCGCalibrationCurrentPoint(int nGroupId_)
{
	int i ;
	//int _nTmpPos, _nTmpAmp ;
	int _nBeamQty = TMP(beam_qty[nGroupId_]);
	//int _nBeamNo  = TMP(beam_num[nGroupId_]);
	int _nPointPos= GROUP_VAL_POS(nGroupId_ , SizingCurves.point_pos);

	if (_nPointPos <= 1)  return ;
	for (i = 0 ; i< _nBeamQty ; i++)
	{
		//GROUP_VAL_POS(nGroupId_ , SizingCurves.position[i][_nPointPos]) = _nTmpPos ;
		GROUP_VAL_POS(nGroupId_ , SizingCurves.amplitude[i][_nPointPos]) = GROUP_VAL_POS(nGroupId_ , SizingCurves.amplitude[i][_nPointPos - 1]);
	}
}

double CurrentPointTcgGainLimit(int grp)
{
    unsigned char point_pos = GROUP_VAL_POS(grp , SizingCurves.point_pos) ;
	double ret;
    ret = MIN(40 , 80 - group_get_gain(grp) / 100.0) ;

	if(point_pos == 0)		ret = 0 ;

	return ret ;
}

double GetTcgCurrentPointValue(int nGroupId_)
{
	double _nResult ;
	int point_pos = GROUP_VAL_POS(nGroupId_ , SizingCurves.point_pos) ;
	if(point_pos == 0)
	{
		return 0.0 ;
	}

	int _nBeamNo = TMP(beam_num[nGroupId_]) ;
	_nResult = GROUP_VAL_POS(nGroupId_ , SizingCurves.amplitude[_nBeamNo][0])
			/ ((double) GROUP_VAL_POS(nGroupId_ , SizingCurves.amplitude[_nBeamNo][point_pos] ) ) ;
	_nResult = log10 ( _nResult ) * 20 ;

	return _nResult ;
}


void refresh_linear_dac_pointer_info()
{
	int i ;
	double scale ;
	double material_decorate ;  // (db / 100mm)
	double delay; // 10ns

	int sample_start  ;
	int sample_range  ;
	double velocity  ;
	int interval  ;
	double interval_db ;

	int grp = get_current_group(pp->p_config);
	material_decorate = GROUP_VAL_POS(grp , SizingCurves.mat_atten ) / 1000.0;  // (db / 100mm)
	delay = GROUP_VAL_POS(grp , SizingCurves.delay ) ; // 10ns

	sample_start = get_group_val (get_group_by_id (pp->p_config, grp), GROUP_START) ;
	sample_range = get_group_val (get_group_by_id (pp->p_config, grp), GROUP_RANGE) ;
	velocity  = get_group_val (get_group_by_id (pp->p_config, grp), GROUP_VELOCITY) / 20000.0 ;
	interval = (sample_range - delay + sample_start) / 10 ;
	interval_db = interval * velocity * material_decorate / 1000.0;
	GROUP_VAL_POS(grp , SizingCurves.linearamplitude[0]) = GROUP_VAL_POS(grp , SizingCurves.linear_ref_ampl) * 100 ;
	GROUP_VAL_POS(grp , SizingCurves.linearposition[0])  = GROUP_VAL_POS(grp , SizingCurves.delay )  ;
	scale = pow( 10.0, -interval_db / 20.0);

	for (i = 1 ; i< 10 ; i++)
	{
		GROUP_VAL_POS(grp , SizingCurves.linearposition[i]) = delay + interval * i ;
		GROUP_VAL_POS(grp , SizingCurves.linearamplitude[i]) = (unsigned int)(GROUP_VAL_POS(grp , SizingCurves.linearamplitude[i - 1]) * scale ) ;
	}
}

double DacAmptitudeMinimumLimit(int grp)
{
	double _nResult ;
	double _nGainLimit;
    int _nBeamNo = TMP(beam_num[grp]) ;
    _nGainLimit = MIN(40 , 80 - group_get_gain(grp) / 100.0) ;
    _nResult = GROUP_VAL_POS(grp , SizingCurves.amplitude[_nBeamNo][0]) / 1000.0 ;
	_nResult = _nResult /(pow(10 , _nGainLimit / 20.0)) ;

	return _nResult ;
}

void RefreshDacPointInfo(int nGroupId_)
{
   double _nMinimumAmplitude = DacAmptitudeMinimumLimit(nGroupId_) * 1000 ;
   int _nBeamQty = TMP(beam_qty[nGroupId_]) ;

   int i  , j;
   for(j = 0 ; j < _nBeamQty ; j++)
   {
	   for(i = 0 ; i < GROUP_VAL_POS(nGroupId_ , SizingCurves.dac_point_qty) ; i++)
	   {
		   if(GROUP_VAL_POS(nGroupId_ , SizingCurves.amplitude[j][i]) < _nMinimumAmplitude)
		   {
			   GROUP_VAL_POS(nGroupId_ , SizingCurves.amplitude[j][i]) = _nMinimumAmplitude ;
		   }
	   }
   }

}

void AddDacPoint(int nGroupId_)
{
	int i ;
	int _nBeamQty= TMP(beam_qty[nGroupId_]);
	int _nPointPos = GROUP_VAL_POS(nGroupId_ , SizingCurves.dac_point_qty) ;
	if(_nPointPos >= 16)  return ;
	GROUP_VAL_POS(nGroupId_ , SizingCurves.dac_point_qty) = _nPointPos + 1 ;
	GROUP_VAL_POS(nGroupId_ , SizingCurves.point_pos)     = _nPointPos     ;

	for(i = 0; i < _nBeamQty ; i++)
	{
		GROUP_VAL_POS(nGroupId_ , SizingCurves.position[i][_nPointPos]) = GROUP_VAL_POS(nGroupId_ , SizingCurves.position[i][_nPointPos - 1]) +
				get_group_val (get_group_by_id (pp->p_config, nGroupId_), GROUP_RANGE) / 10 ;
		GROUP_VAL_POS(nGroupId_ , SizingCurves.amplitude[i][_nPointPos]) = GROUP_VAL_POS(nGroupId_ , SizingCurves.amplitude[i][_nPointPos - 1]) ;
	}

}

/*
 * getMeasureData.c
 *
 *  Created on: 2012-7-13
 *      Author: wolflord
 */
#include "../drawui.h"
#include "../drawfb.h"
#include "../calibration.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MM2INCH(a)     (a=a* 0.03937)
#define PI 3.1415926535897931

double getMeasureData_aHeight(int grp , int _nIndex)
{
	int _nData  ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ; //pp->p_config->group[grp].rectifier1  ;
	_nData = TMP(measure_data[_nIndex][1]) ;
	double _nMeasureData ;

	if(_bRectifier)
	{
		_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
	}
	else
	{
		_nMeasureData = (signed short)(_nData >> 16) ;
		_nMeasureData = _nMeasureData / (10.24 * 16);
	}

    return _nMeasureData ;
}

double getMeasureData_aDBa(int grp , int _nIndex)
{
	int _nData  ;
	double _nGateHeight ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ; //pp->p_config->group[grp].rectifier1  ;
	_nData = TMP(measure_data[_nIndex][1]) ;
	double _nMeasureData ;

	if(_bRectifier)
	{
		_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
	}
	else
	{
		_nMeasureData = (signed short)(_nData >> 16) ;
		_nMeasureData = _nMeasureData / (10.24 * 16);
		_nMeasureData = fabs(_nMeasureData) ;
	}

	_nGateHeight = GROUP_VAL_POS(grp , gate[0].height);
	if(_nGateHeight < 1) _nGateHeight = 1  ;
	//if(_nGateHeight > fabs(_nMeasureData))
	//{
	//	_nMeasureData = MEASURE_DATA_ND  ;
	//}
	//else
	//{
		_nMeasureData = 20 * log10(_nMeasureData / _nGateHeight);
	//}

    return _nMeasureData ;
}

double getMeasureData_aDBr(int grp , int _nIndex)
{
	int _nData , _nDataR ;
	double _nGateHeight , _nReference  ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ; //pp->p_config->group[grp].rectifier1  ;
	_nData = TMP(measure_data[_nIndex][1]) ;
	_nDataR = GROUP_VAL_POS(grp , nReferenceData[1]) ;
	double _nMeasureData ;

	if(_bRectifier)
	{
		_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
		_nReference   = ((_nDataR>>20) & 0x00000fff) /20.47;//满屏时200% 4095
		if(_nReference < 1) _nReference = 1  ;
	}
	else
	{
		_nMeasureData = (signed short)(_nData >> 16) ;
		_nMeasureData = _nMeasureData / (10.24 * 16);
		_nReference = (signed short)(_nDataR >> 16) ;
		_nReference = _nReference / (10.24 * 16);
		if(_nReference < 1) _nReference = 1  ;
		_nMeasureData = fabs(_nMeasureData) ;
	}

	_nGateHeight = GROUP_VAL_POS(grp , gate[0].height);
	//if(_nGateHeight > fabs(_nMeasureData))
	//{
	//	_nMeasureData = MEASURE_DATA_ND  ;
	//}
	//else
	//{
		if(_nReference)
		{
			_nMeasureData = 20 * log10(_nMeasureData / _nReference) ;
		}
		else
		{
			_nMeasureData =  MEASURE_DATA_ND;
		}
	//}


    return _nMeasureData ;
}

double getMeasureData_bHeight(int grp , int _nIndex)
{
	int _nData  ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ; //pp->p_config->group[grp].rectifier1  ;
	_nData = TMP(measure_data[_nIndex][2]) ;
	double _nMeasureData ;

	if(_bRectifier)
	{
		_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
	}
	else
	{
		_nMeasureData = (signed short)(_nData >> 16) ;
		_nMeasureData = _nMeasureData / (10.24 * 16);
	}

    return _nMeasureData ;
}

double getMeasureData_bDBb(int grp , int _nIndex)
{
	int _nData  ;
	double _nGateHeight ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ; //pp->p_config->group[grp].rectifier1  ;
	_nData = TMP(measure_data[_nIndex][2]) ;
	double _nMeasureData ;

	if(_bRectifier)
	{
		_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
	}
	else
	{
		_nMeasureData = (signed short)(_nData >> 16) ;
		_nMeasureData = _nMeasureData / (10.24 * 16);
		_nMeasureData = fabs(_nMeasureData) ;
	}


	_nGateHeight = GROUP_VAL_POS(grp , gate[1].height);
	if(_nGateHeight < 1) _nGateHeight = 1  ;
	//if(_nGateHeight > fabs(_nMeasureData))
	//{
	//	_nMeasureData = MEASURE_DATA_ND  ;
	//}
	//else
	//{
		_nMeasureData = 20 * log10(_nMeasureData / _nGateHeight);
	//}

    return _nMeasureData ;
}

double getMeasureData_bDBr(int grp , int _nIndex)
{
	int _nData  ;
	int _nDataR ;
	double _nGateHeight ;
	double _nReference  ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ; //pp->p_config->group[grp].rectifier1  ;
	_nData = TMP(measure_data[_nIndex][2]) ;
	_nDataR = GROUP_VAL_POS(grp , nReferenceData[2]) ;
	double _nMeasureData ;

	if(_bRectifier)
	{
		_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
		_nReference   = ((_nDataR>>20) & 0x00000fff) /20.47;//满屏时200% 4095
		if(_nReference < 1) _nReference = 1  ;
	}
	else
	{
		_nMeasureData = (signed short)(_nData >> 16) ;
		_nMeasureData = _nMeasureData / (10.24 * 16);
		_nReference = (signed short)(_nDataR >> 16) ;
		_nReference = _nReference / (10.24 * 16);
		if(_nReference < 1) _nReference = 1  ;
		_nMeasureData = fabs(_nMeasureData) ;
	}


	_nGateHeight = GROUP_VAL_POS(grp , gate[1].height);
	//if(_nGateHeight > fabs(_nMeasureData))
	//{
	//	_nMeasureData = MEASURE_DATA_ND  ;
	//}
	//else
	//{
		if(_nReference)
		{
			_nMeasureData = 20 * log10(_nMeasureData / _nReference) ;
		}
		else
		{
			_nMeasureData =  MEASURE_DATA_ND;
		}
	//}


    return _nMeasureData ;
}

double getMeasureData_aPeak(int grp , int _nIndex)
{
	int _nData  ;
	double _nGateHeight ;
	double _nVelocity;
	double _nMeasureData  ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ;
	_nData = TMP(measure_data[_nIndex][1]) ;
	_nGateHeight = GROUP_VAL_POS(grp , gate[0].height);

	if(_bRectifier)
	{
		_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
	}
	else
	{
		_nMeasureData = (signed short)(_nData >> 16) ;
		_nMeasureData = _nMeasureData / (10.24 * 16);
	}

	if(_nGateHeight > fabs(_nMeasureData))
	{
		_nMeasureData = MEASURE_DATA_ND  ;
	}
	else
	{
		_nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0     ;
		_nMeasureData = (double)(_nData & 0xfffff)             ;
		if  (UT_UNIT_TIME == GROUP_VAL_POS( grp , ut_unit))
		{
			_nMeasureData = _nMeasureData / 100  ;
		}
		else
		{
			_nMeasureData = _nMeasureData * _nVelocity / 200000    ;
			if (UNIT_INCH == get_unit(pp->p_config))
			{
				MM2INCH(_nMeasureData) ; //MM2INCH(_nMeasureData) ;
			}
		}
	}
	return _nMeasureData ;
}

double getMeasureData_bPeak(int grp , int _nIndex)
{
	int _nData  ;
	double _nGateHeight ;
	double _nVelocity;
	double _nMeasureData  ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ;
	_nData = TMP(measure_data[_nIndex][2]) ;
	_nGateHeight = GROUP_VAL_POS(grp , gate[1].height);
	if(_bRectifier)
	{
		_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
	}
	else
	{
		_nMeasureData = (signed short)(_nData >> 16) ;
		_nMeasureData = _nMeasureData / (10.24 * 16);
	}

	if(_nGateHeight > fabs(_nMeasureData))
	{
		_nMeasureData = MEASURE_DATA_ND  ;
	}
	else
	{
		_nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
		_nMeasureData = (double)(_nData & 0xfffff)       ;
		if  (UT_UNIT_TIME == GROUP_VAL_POS( grp , ut_unit))
		{
			_nMeasureData = _nMeasureData / 100  ;
		}
		else
		{
			_nMeasureData = _nMeasureData * _nVelocity / 200000    ;
			if (UNIT_INCH == get_unit(pp->p_config))
			{
				MM2INCH(_nMeasureData) ; //MM2INCH(_nMeasureData) ;
			}
		}
	}

	return _nMeasureData ;
}

double getMeasureData_iEdge(int grp , int _nIndex)
{
	int _nData  ;
	double _nGateHeight ;
	double _nVelocity;
	double _nMeasureData  ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ;
	_nData = TMP(measure_data[_nIndex][3]) ;
	_nGateHeight = GROUP_VAL_POS(grp , gate[2].height);
	if(_bRectifier)
	{
		_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
	}
	else
	{
		_nMeasureData = (signed short)(_nData >> 16) ;
		_nMeasureData = _nMeasureData / (10.24 * 16);
	}

	if(_nGateHeight > fabs(_nMeasureData))
	{
		_nMeasureData = MEASURE_DATA_ND  ;
	}
	else
	{
		_nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
		_nMeasureData = (double)(_nData & 0xfffff)         ;
		if  (UT_UNIT_TIME == GROUP_VAL_POS( grp , ut_unit))
		{
			_nMeasureData = _nMeasureData / 100  ;
		}
		else
		{
			_nMeasureData = _nMeasureData * _nVelocity / 200000    ;
			if (UNIT_INCH == get_unit(pp->p_config))
			{
				MM2INCH(_nMeasureData) ;
			}
		}
	}

	return _nMeasureData ;
}

double getMeasureData_iEdgeWater(int grp , int _nIndex)
{
	int _nData  ;
	double _nGateHeight ;
	double _nVelocity;
	double _nMeasureData  ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ;
	_nData = TMP(measure_data[_nIndex][3]) ;

	if(_bRectifier)
	{
		_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
	}
	else
	{
		_nMeasureData = (signed short)(_nData >> 16) ;
		_nMeasureData = _nMeasureData / (10.24 * 16);
	}
	_nGateHeight = GROUP_VAL_POS(grp , gate[2].height);
	if(_nGateHeight > fabs(_nMeasureData))
	{
		_nMeasureData = MEASURE_DATA_ND  ;
	}
	else
	{
		_nVelocity =  1480 ;
		_nMeasureData = (double)(_nData & 0xfffff)       ;
		if  (UT_UNIT_TIME == GROUP_VAL_POS( grp , ut_unit))
		{
			_nMeasureData = _nMeasureData / 100  ;
		}
		else
		{
			_nMeasureData = _nMeasureData * _nVelocity / 200000    ;
			if (UNIT_INCH == get_unit(pp->p_config))
			{
				MM2INCH(_nMeasureData) ;
			}
		}
	}
	return _nMeasureData ;
}

double getMeasureData_aThickness(int grp , int _nIndex)
{
	int _nData  ;
	double _nGateHeight ;
	double _nVelocity;
	int _nPulseWidth ,_nWedgeDelay  ,_nBeamDelay  ;
	//double _nThickness ;//工件厚度
	double _nAngle ;
	double _nMeasureData  ;
	int _nBeamNo = TMP(beam_num[grp]) ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ;
	_nData = TMP(measure_data[_nIndex][1]) ;
	_nGateHeight = GROUP_VAL_POS(grp , gate[0].height);
	if(_bRectifier)
	{
		_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
	}
	else
	{
		_nMeasureData = (signed short)(_nData >> 16) ;
		_nMeasureData = _nMeasureData / (10.24 * 16);
	}

	if(_nGateHeight > fabs(_nMeasureData))
	{
		_nMeasureData = MEASURE_DATA_ND  ;
	}
	else
	{
		_nAngle = TMP(current_angle[grp]);
		_nPulseWidth  = GROUP_VAL_POS(grp , pulser_width1) / (10 * PW_DIV);
		_nWedgeDelay  = GROUP_VAL_POS(grp , wedge_delay) / 10;
		_nBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) / 10;    // 10ns
		_nWedgeDelay += _nBeamDelay + _nPulseWidth  ;

		_nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
		_nMeasureData = (double)(_nData & 0xfffff)   - _nWedgeDelay   ;
		_nMeasureData = _nMeasureData * _nVelocity * cos(_nAngle) / 200000    ;
		if (UNIT_INCH == get_unit(pp->p_config))
		{
			MM2INCH(_nMeasureData) ;
		}
	}

	return _nMeasureData ;
}

double getMeasureData_ML(int grp , int _nIndex)
{
	int _nData  ;
	double _nGateHeight ;
	double _nVelocity;
	int _nPulseWidth ,_nWedgeDelay  ,_nBeamDelay  ;
	double _nThickness ;//工件厚度
	double _nAngle ;
	double _nMeasureData  ;
	int _nBeamNo = TMP(beam_num[grp]) ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ;
	_nData = TMP(measure_data[_nIndex][1]) ;

	if(_bRectifier)
	{
		_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
	}
	else
	{
		_nMeasureData = (signed short)(_nData >> 16) ;
		_nMeasureData = _nMeasureData / (10.24 * 16);
	}
	_nGateHeight = GROUP_VAL_POS(grp , gate[0].height);
	if(_nGateHeight > fabs(_nMeasureData))
	{
		_nMeasureData = MEASURE_DATA_ND  ;
	}
	else
	{
		_nAngle = TMP(current_angle[grp]);
		_nThickness = (double)(GROUP_VAL_POS(grp , part.Thickness)/1000.0);//工件厚度
		if(_nThickness < 0.1) _nThickness = 0.1  ;
		_nPulseWidth  = GROUP_VAL_POS(grp , pulser_width1) / (10 * PW_DIV);
		_nWedgeDelay  = GROUP_VAL_POS(grp , wedge_delay) / 10;
		_nBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) / 10;    // 10ns
		_nWedgeDelay += _nBeamDelay + _nPulseWidth  ;

		_nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
		_nMeasureData = (double)(_nData & 0xfffff)  -_nWedgeDelay ;
		_nMeasureData = _nMeasureData * _nVelocity * cos(_nAngle) / 200000    ;
		_nMeasureData = (_nMeasureData - _nThickness) / _nThickness;
		_nMeasureData = fabs(_nMeasureData * 100); // is this always positive???
	}

	return _nMeasureData ;
}


double getMeasureData_RA(int grp , int _nIndex)
{
	int _nData  ;
	double _nGateHeight ;
	double _nVelocity;
	int _nPulseWidth ,_nWedgeDelay  ,_nBeamDelay  ;
	//double _nThickness ;//工件厚度
	double _nAngle ;
	double _nMeasureData  ;
	int _nBeamNo = TMP(beam_num[grp]) ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ;
	_nData = TMP(measure_data[_nIndex][1]) ;

	if(_bRectifier)
	{
		_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
	}
	else
	{
		_nMeasureData = (signed short)(_nData >> 16) ;
		_nMeasureData = _nMeasureData / (10.24 * 16);
	}
	_nGateHeight = GROUP_VAL_POS(grp , gate[0].height);
	if(_nGateHeight > fabs(_nMeasureData))
	{
		_nMeasureData = MEASURE_DATA_ND  ;
	}
	else
	{
		_nAngle = TMP(current_angle[grp]);
		_nPulseWidth  = GROUP_VAL_POS(grp , pulser_width1) / (10 * PW_DIV);
		_nWedgeDelay  = GROUP_VAL_POS(grp , wedge_delay) / 10;
		_nBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) / 10;    // 10ns
		_nWedgeDelay += _nBeamDelay + _nPulseWidth  ;

		_nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
		_nMeasureData = (double)(_nData & 0xfffff) - _nWedgeDelay  ;
		_nMeasureData = _nMeasureData * _nVelocity * sin(_nAngle) / 200000  ;
		if (UNIT_INCH == get_unit(pp->p_config))
		{
			MM2INCH(_nMeasureData) ;
		}
	}

	return _nMeasureData ;
}


double getMeasureData_RB(int grp , int _nIndex)
{
	int _nData  ;
	double _nGateHeight ;
	double _nVelocity;
	int _nPulseWidth ,_nWedgeDelay  ,_nBeamDelay  ;
	//double _nThickness ;//工件厚度
	double _nAngle ;
	double _nMeasureData  ;
	int _nBeamNo = TMP(beam_num[grp]) ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ;
	_nData = TMP(measure_data[_nIndex][2]) ;

	if(_bRectifier)
	{
		_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
	}
	else
	{
		_nMeasureData = (signed short)(_nData >> 16) ;
		_nMeasureData = _nMeasureData / (10.24 * 16);
	}
	_nGateHeight = GROUP_VAL_POS(grp , gate[1].height);
	if(_nGateHeight > fabs(_nMeasureData))
	{
		_nMeasureData = MEASURE_DATA_ND  ;
	}
	else
	{
		_nAngle = TMP(current_angle[grp]);
		_nPulseWidth  = GROUP_VAL_POS(grp , pulser_width1) / (10 * PW_DIV);
		_nWedgeDelay  = GROUP_VAL_POS(grp , wedge_delay) / 10;
		_nBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) / 10;    // 10ns
		_nWedgeDelay += _nBeamDelay + _nPulseWidth  ;

		_nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
		_nMeasureData = (double)(_nData & 0xfffff) - _nWedgeDelay  ;
		_nMeasureData = _nMeasureData * _nVelocity * sin(_nAngle) / 200000    ;
		if (UNIT_INCH == get_unit(pp->p_config))
		{
			MM2INCH(_nMeasureData) ;
		}
	}

	return _nMeasureData ;
}


double getMeasureData_PA(int grp , int _nIndex)
{
	int _nData  ;
	int _nReflectTimes ;
	double _nGateHeight ;
	double _nVelocity;
	int _nPulseWidth ,_nWedgeDelay  ,_nBeamDelay  ;
	double _nThickness ;//工件厚度
	double _nAngle, _ncos, _nsin;
    double pathAngle, fTmp, fDis, pathL, fTest, fR, fRR;
	double _nMeasureData  ;
	int _nBeamNo = TMP(beam_num[grp]) ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ;
	_nData = TMP(measure_data[_nIndex][1]) ;

	if(_bRectifier)
	{
		_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
	}
	else
	{
		_nMeasureData = (signed short)(_nData >> 16) ;
		_nMeasureData = _nMeasureData / (10.24 * 16);
	}
	_nGateHeight = GROUP_VAL_POS(grp , gate[0].height);
	if(_nGateHeight > fabs(_nMeasureData))
	{
		_nMeasureData = MEASURE_DATA_ND  ;
	} else if (get_part_geometry(grp) == 1) {
	    fR = get_part_diameter(grp)/2000.0;
        fRR = fR*fR;
		_nAngle = TMP(current_angle[grp]);
        _ncos = cos(_nAngle);
        _nsin = sin(_nAngle);
		_nThickness = GROUP_VAL_POS(grp, part.Thickness)/1000.0;
		_nPulseWidth  = GROUP_VAL_POS(grp , pulser_width1) / (10 * PW_DIV);
		_nWedgeDelay  = GROUP_VAL_POS(grp , wedge_delay) / 10;
		_nBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) / 10;    // 10ns
		_nWedgeDelay += _nBeamDelay + _nPulseWidth  ;
		_nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
		_nMeasureData = (double)((_nData & 0xfffff) - _nWedgeDelay);
		_nMeasureData = _nMeasureData * _nVelocity / 200000 ;

        fDis = fR - _nThickness;
        fTest = fDis*fDis - fRR*_nsin*_nsin;
        if (fTest < 0) {
            pathL = fR*_ncos;
            pathAngle = PI/2-_nAngle;
        } else {
            pathL = fR*_ncos - sqrt(fTest);
            pathAngle = asin(pathL*_nsin/fDis);
        }

		_nReflectTimes = (int)(_nMeasureData / pathL);
        _nMeasureData = _nMeasureData - pathL * _nReflectTimes;
    	if (_nReflectTimes % 2) {
             fTmp = pathL-_nMeasureData;
             fTest = sqrt(fRR + fTmp*fTmp - 2*fR*fTmp*_ncos);
             _nMeasureData = asin(_nMeasureData*fR*_nsin/(fTest*fDis));
        } else {
            fTest = sqrt(fRR + _nMeasureData*_nMeasureData - 2*fR*_nMeasureData*_ncos);
            _nMeasureData = asin(_nMeasureData*_nsin/fTest);
        }
        _nMeasureData = (_nMeasureData+_nReflectTimes*pathAngle)*fR + GROUP_VAL_POS(grp , field_distance[_nBeamNo]);
	} else if (get_part_geometry(grp) == 2) {
	    fR = get_part_diameter(grp)/2000.0;
        fRR = fR*fR;
		_nAngle = TMP(current_angle[grp]);
        _ncos = cos(_nAngle);
        _nsin = sin(_nAngle);
		_nThickness = GROUP_VAL_POS(grp, part.Thickness)/1000.0;
		_nPulseWidth  = GROUP_VAL_POS(grp , pulser_width1) / (10 * PW_DIV);
		_nWedgeDelay  = GROUP_VAL_POS(grp , wedge_delay) / 10;
		_nBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) / 10;    // 10ns
		_nWedgeDelay += _nBeamDelay + _nPulseWidth  ;
		_nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
		_nMeasureData = (double)((_nData & 0xfffff) - _nWedgeDelay);
		_nMeasureData = _nMeasureData * _nVelocity / 200000 ;

        fDis = fR + _nThickness;
        fTest = fDis*fDis - fRR*_nsin*_nsin;
        pathL = sqrt(fTest) - fR*_ncos;
        pathAngle = asin(pathL*_nsin/fDis);

		_nReflectTimes = (int)(_nMeasureData / pathL);
        _nMeasureData = _nMeasureData - pathL * _nReflectTimes;
    	if (_nReflectTimes % 2) {
             fTmp = pathL-_nMeasureData;
             fTest = sqrt(fRR + fTmp*fTmp + 2*fR*fTmp*_ncos);
             _nMeasureData = pathAngle - asin(_nMeasureData/fTest*_nsin);
        } else {
            fTest = sqrt(fRR + _nMeasureData*_nMeasureData + 2*fR*_nMeasureData*_ncos);
            _nMeasureData = asin(_nMeasureData/fTest*_nsin);
        }
        _nMeasureData = (_nMeasureData+_nReflectTimes*pathAngle)*fR + GROUP_VAL_POS(grp , field_distance[_nBeamNo]);

	}
	else
	{
		_nAngle = TMP(current_angle[grp]);
		_nPulseWidth  = GROUP_VAL_POS(grp , pulser_width1) / (10 * PW_DIV);
		_nWedgeDelay  = GROUP_VAL_POS(grp , wedge_delay) / 10;
		_nBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) / 10;    // 10ns
		_nWedgeDelay += _nBeamDelay + _nPulseWidth  ;

		_nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
		_nMeasureData = (double)(_nData & 0xfffff) - _nWedgeDelay  ;
		_nMeasureData = _nMeasureData * _nVelocity * sin(_nAngle) / 200000 + GROUP_VAL_POS(grp , field_distance[_nBeamNo]) ; //TMP(field_distance[grp][_nBeamNo]);
		if (UNIT_INCH == get_unit(pp->p_config))
		{
			MM2INCH(_nMeasureData) ;
		}
	}

	return _nMeasureData ;
}

double getMeasureData_PB(int grp , int _nIndex)
{
	int _nData  ;
	double _nGateHeight ;
	double _nVelocity;
	int _nPulseWidth ,_nWedgeDelay  ,_nBeamDelay  ;
	//double _nThickness ;//工件厚度
	double _nAngle ;
	double _nMeasureData  ;
	int _nBeamNo = TMP(beam_num[grp]) ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ;
	_nData = TMP(measure_data[_nIndex][2]) ;

	if(_bRectifier)
	{
		_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
	}
	else
	{
		_nMeasureData = (signed short)(_nData >> 16) ;
		_nMeasureData = _nMeasureData / (10.24 * 16);
	}
	_nGateHeight = GROUP_VAL_POS(grp , gate[1].height);
	if(_nGateHeight > fabs(_nMeasureData))
	{
		_nMeasureData = MEASURE_DATA_ND  ;
	}
	else
	{
		_nAngle = TMP(current_angle[grp]);
		_nPulseWidth  = GROUP_VAL_POS(grp , pulser_width1) / (10 * PW_DIV);
		_nWedgeDelay  = GROUP_VAL_POS(grp , wedge_delay) / 10;
		_nBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) / 10;    // 10ns
		_nWedgeDelay += _nBeamDelay + _nPulseWidth  ;

		_nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
		_nMeasureData = (double)(_nData & 0xfffff) - _nWedgeDelay  ;
		_nMeasureData = _nMeasureData * _nVelocity * sin(_nAngle) / 200000 + GROUP_VAL_POS(grp , field_distance[_nBeamNo]) ; //TMP(field_distance[grp][_nBeamNo]);
		if (UNIT_INCH == get_unit(pp->p_config))
		{
			MM2INCH(_nMeasureData) ;
		}
	}

	return _nMeasureData ;
}

double getMeasureData_DA(int grp , int _nIndex)
{
	int _nData  ;
	int _nReflectTimes ;
	double _nGateHeight ;
	double _nVelocity;
	int _nPulseWidth ,_nWedgeDelay  ,_nBeamDelay  ;
	double _nThickness ;//工件厚度
	double _nAngle, cosAngle, sinAngle;
    double fDis, pathL, fTest, fR, fRR;
	double _nMeasureData  ;
	int _nBeamNo = TMP(beam_num[grp]) ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ;
	_nData = TMP(measure_data[_nIndex][1]) ;

	if(_bRectifier)
	{
		_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
	}
	else
	{
		_nMeasureData = (signed short)(_nData >> 16) ;
		_nMeasureData = _nMeasureData / (10.24 * 16);
	}
	_nGateHeight = GROUP_VAL_POS(grp , gate[0].height);
	if(_nGateHeight > fabs(_nMeasureData))
	{
		_nMeasureData = MEASURE_DATA_ND  ;
    } else if (get_part_geometry(grp) == 1) {
	    fR = get_part_diameter(grp)/2000.0;
		_nAngle = TMP(current_angle[grp]);
        cosAngle = cos(_nAngle);
        sinAngle = sin(_nAngle);
		_nThickness = GROUP_VAL_POS(grp, part.Thickness)/1000.0;
		_nPulseWidth  = GROUP_VAL_POS(grp , pulser_width1) / (10 * PW_DIV);
		_nWedgeDelay  = GROUP_VAL_POS(grp , wedge_delay) / 10;
		_nBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) / 10;
		_nWedgeDelay += _nBeamDelay + _nPulseWidth  ;
		_nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
		_nMeasureData = (double)((_nData & 0xfffff) - _nWedgeDelay);
		_nMeasureData = _nMeasureData * _nVelocity / 200000 ;

        fDis = (fR - _nThickness)*(fR - _nThickness);
        fTest = fDis - fR*fR*sin(_nAngle)*sin(_nAngle);
        if (fTest < 0) {
            pathL = fR*cosAngle;
        } else {
            fTest = sqrt(fTest);
            pathL = fR*cos(_nAngle) - fTest;
        }

        if (pathL < 0.01) {
            _nMeasureData = 0.01;
        } else {
    		_nReflectTimes = (int)(_nMeasureData / pathL);
            _nMeasureData = _nMeasureData - pathL * _nReflectTimes ;
    	    if(_nReflectTimes % 2) {
                fTest = pathL-_nMeasureData;
                _nMeasureData = fR*fR + fTest*fTest - 2*fTest*fR*cos(_nAngle);
            } else {
                _nMeasureData = fR*fR + _nMeasureData*_nMeasureData - 2*_nMeasureData*fR*cos(_nAngle);
            }
            _nMeasureData = fR - sqrt(_nMeasureData);
        }

    	if (UNIT_INCH == get_unit(pp->p_config)) {
		    MM2INCH(_nMeasureData) ;
    	}
    } else if (get_part_geometry(grp) == 2) {
	    fR = get_part_diameter(grp)/2000.0;
        fRR = fR*fR;
		_nAngle = TMP(current_angle[grp]);
        cosAngle = cos(_nAngle);
        sinAngle = sin(_nAngle);
		_nThickness = GROUP_VAL_POS(grp, part.Thickness)/1000.0;
		_nPulseWidth  = GROUP_VAL_POS(grp , pulser_width1) / (10 * PW_DIV);
		_nWedgeDelay  = GROUP_VAL_POS(grp , wedge_delay) / 10;
		_nBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) / 10;
		_nWedgeDelay += _nBeamDelay + _nPulseWidth  ;
		_nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
		_nMeasureData = (double)((_nData & 0xfffff) - _nWedgeDelay);
		_nMeasureData = _nMeasureData * _nVelocity / 200000 ;

        fDis = (fR + _nThickness)*(fR + _nThickness);
        fTest = fDis - fRR*sinAngle*sinAngle;
        pathL = sqrt(fTest) - fR*cosAngle;

        if (pathL < 0.01) {
            _nMeasureData = 0.01;
        } else {
    		_nReflectTimes = (int)(_nMeasureData / pathL);
            _nMeasureData = _nMeasureData - pathL * _nReflectTimes ;
    	    if (_nReflectTimes % 2) {
                fTest = pathL-_nMeasureData;
            }
            _nMeasureData = fRR + _nMeasureData*_nMeasureData + 2*_nMeasureData*fR*cos(_nAngle);
            _nMeasureData = sqrt(_nMeasureData) - fR;
        }

    	if (UNIT_INCH == get_unit(pp->p_config)) {
		    MM2INCH(_nMeasureData) ;
    	}
    } else
	{
		_nAngle = TMP(current_angle[grp]);
		_nThickness = (double)(GROUP_VAL_POS(grp , part.Thickness)/1000.0);//工件厚度
		_nPulseWidth  = GROUP_VAL_POS(grp , pulser_width1) / (10 * PW_DIV);
		_nWedgeDelay  = GROUP_VAL_POS(grp , wedge_delay) / 10;
		_nBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) / 10;    // 10ns
		_nWedgeDelay += _nBeamDelay + _nPulseWidth  ;
		_nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
		_nMeasureData = (double)((_nData & 0xfffff) - _nWedgeDelay);
		_nMeasureData = _nMeasureData * _nVelocity * cos(_nAngle) / 200000 ;
		_nReflectTimes = (int)(_nMeasureData / _nThickness) ;
		if(_nReflectTimes % 2)
			_nMeasureData = _nThickness * (_nReflectTimes + 1) - _nMeasureData ;
		else
			_nMeasureData = _nMeasureData - _nThickness * _nReflectTimes ;

        if (UNIT_INCH == get_unit(pp->p_config)) {
			MM2INCH(_nMeasureData) ;
		}
	}

	return _nMeasureData ;
}

double getMeasureData_DB(int grp , int _nIndex)
{
	int _nData  ;
	int _nReflectTimes ;
	double _nGateHeight ;
	double _nVelocity;
	int _nPulseWidth ,_nWedgeDelay  ,_nBeamDelay  ;
	double _nThickness ;//工件厚度
	double _nAngle ;
	double _nMeasureData  ;
	int _nBeamNo = TMP(beam_num[grp]) ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ;
	_nData = TMP(measure_data[_nIndex][2]) ;

	if(_bRectifier)
	{
		_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
	}
	else
	{
		_nMeasureData = (signed short)(_nData >> 16) ;
		_nMeasureData = _nMeasureData / (10.24 * 16);
	}
	_nGateHeight = GROUP_VAL_POS(grp , gate[1].height);
	if(_nGateHeight > fabs(_nMeasureData))
	{
		_nMeasureData = MEASURE_DATA_ND  ;
	}
	else
	{
		_nAngle = TMP(current_angle[grp]);
		_nThickness = (double)(GROUP_VAL_POS(grp , part.Thickness)/1000.0);//工件厚度
		_nPulseWidth  = GROUP_VAL_POS(grp , pulser_width1) / (10 * PW_DIV);
		_nWedgeDelay  = GROUP_VAL_POS(grp , wedge_delay) / 10;
		_nBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) / 10;    // 10ns
		_nWedgeDelay += _nBeamDelay + _nPulseWidth  ;
		_nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
		_nMeasureData = (double)((_nData & 0xfffff) - _nWedgeDelay);
		_nMeasureData = _nMeasureData * _nVelocity * cos(_nAngle) / 200000 ;
		_nReflectTimes = (int)(_nMeasureData / _nThickness) ;
		if(_nReflectTimes % 2)
			_nMeasureData = _nThickness * (_nReflectTimes + 1) - _nMeasureData ;
		else
			_nMeasureData = _nMeasureData - _nThickness * _nReflectTimes ;

		if (UNIT_INCH == get_unit(pp->p_config))
		{
			MM2INCH(_nMeasureData) ;
		}
	}

	return _nMeasureData ;
}

double getMeasureData_SA(int grp , int _nIndex)
{
	int _nData  ;
	double _nGateHeight ;
	double _nVelocity;
	int _nPulseWidth ,_nWedgeDelay  ,_nBeamDelay  ;

	double _nMeasureData  ;
	int _nBeamNo = TMP(beam_num[grp]) ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ;
	_nData = TMP(measure_data[_nIndex][1]) ;

	if(_bRectifier)
	{
		_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
	}
	else
	{
		_nMeasureData = (signed short)(_nData >> 16) ;
		_nMeasureData = _nMeasureData / (10.24 * 16);
	}
	_nGateHeight = GROUP_VAL_POS(grp , gate[0].height);
	if(_nGateHeight > fabs(_nMeasureData))
	{
		_nMeasureData = MEASURE_DATA_ND  ;
	}
	else
	{
		_nPulseWidth  = GROUP_VAL_POS(grp , pulser_width1) / (10 * PW_DIV);
		_nWedgeDelay  = GROUP_VAL_POS(grp , wedge_delay) / 10;
		_nBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) / 10;    // 10ns
		_nWedgeDelay += _nBeamDelay + _nPulseWidth  ;
		_nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
		_nMeasureData = (double)((_nData & 0xfffff) - _nWedgeDelay);
		_nMeasureData = _nMeasureData * _nVelocity  / 200000 ;

		if (UNIT_INCH == get_unit(pp->p_config))
		{
			MM2INCH(_nMeasureData) ;
		}
	}

	return _nMeasureData ;
}

double getMeasureData_SB(int grp , int _nIndex)
{
	int _nData  ;
	double _nGateHeight ;
	double _nVelocity;
	int _nPulseWidth ,_nWedgeDelay  ,_nBeamDelay  ;

	double _nMeasureData  ;
	int _nBeamNo = TMP(beam_num[grp]) ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ;
	_nData = TMP(measure_data[_nIndex][2]) ;

	if(_bRectifier)
	{
		_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
	}
	else
	{
		_nMeasureData = (signed short)(_nData >> 16) ;
		_nMeasureData = _nMeasureData / (10.24 * 16);
	}
	_nGateHeight = GROUP_VAL_POS(grp , gate[1].height);
	if(_nGateHeight > fabs(_nMeasureData))
	{
		_nMeasureData = MEASURE_DATA_ND  ;
	}
	else
	{
		_nPulseWidth  = GROUP_VAL_POS(grp , pulser_width1) / (10 * PW_DIV);
		_nWedgeDelay  = GROUP_VAL_POS(grp , wedge_delay) / 10;
		_nBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) / 10;    // 10ns
		_nWedgeDelay += _nBeamDelay + _nPulseWidth  ;
		_nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
		_nMeasureData = (double)((_nData & 0xfffff) - _nWedgeDelay);
		_nMeasureData = _nMeasureData * _nVelocity  / 200000 ;

		if (UNIT_INCH == get_unit(pp->p_config))
		{
			MM2INCH(_nMeasureData) ;
		}
	}

	return _nMeasureData ;
}

double getMeasureData_ViA(int grp , int _nIndex)
{
	int _nData  ;
	double _nGateHeight , _nAngle ;
	double _nVelocity;
	int _nPulseWidth ,_nWedgeDelay  ,_nBeamDelay  ;

	double _nMeasureData  ;
	int _nBeamNo = TMP(beam_num[grp]) ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ;
	_nData = TMP(measure_data[_nIndex][1]) ;

	if(_bRectifier)
	{
		_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
	}
	else
	{
		_nMeasureData = (signed short)(_nData >> 16) ;
		_nMeasureData = _nMeasureData / (10.24 * 16);
	}
	_nGateHeight = GROUP_VAL_POS(grp , gate[0].height);
	if(_nGateHeight > fabs(_nMeasureData))
	{
		_nMeasureData = MEASURE_DATA_ND  ;
	}
	else
	{
		_nAngle = TMP(current_angle[grp]);
		_nPulseWidth  = GROUP_VAL_POS(grp , pulser_width1) / (10 * PW_DIV);
		_nWedgeDelay  = GROUP_VAL_POS(grp , wedge_delay) / 10;
		_nBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) / 10;    // 10ns
		_nWedgeDelay += _nBeamDelay + _nPulseWidth  ;
		_nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
		_nMeasureData = (double)((_nData & 0xfffff) - _nWedgeDelay);
		_nMeasureData = _nMeasureData * _nVelocity * sin(_nAngle) / 200000 + GROUP_VAL_POS(grp , field_distance[_nBeamNo]) ; //TMP(field_distance[grp][_nBeamNo]);
		if(GROUP_VAL_POS(grp , skew) == 9000)
		{
			_nMeasureData += GROUP_VAL_POS(grp , index_offset) / 10.0 ;
			if (UNIT_INCH == get_unit(pp->p_config))
			{
				MM2INCH(_nMeasureData) ;
			}
		}
		else if(GROUP_VAL_POS(grp , skew) == 27000)
		{
			_nMeasureData  = GROUP_VAL_POS(grp , index_offset) / 10.0 - _nMeasureData;
			if (UNIT_INCH == get_unit(pp->p_config))
			{
				MM2INCH(_nMeasureData) ;
			}
		}
		else
		{
			_nMeasureData  = MEASURE_DATA_ND  ;
		}
	}

	return _nMeasureData ;
}

double getMeasureData_ViB(int grp , int _nIndex)
{
	int _nData  ;
	double _nGateHeight , _nAngle ;
	double _nVelocity;
	int _nPulseWidth ,_nWedgeDelay  ,_nBeamDelay  ;

	double _nMeasureData  ;
	int _nBeamNo = TMP(beam_num[grp]) ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ;
	_nData = TMP(measure_data[_nIndex][2]) ;

	if(_bRectifier)
	{
		_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
	}
	else
	{
		_nMeasureData = (signed short)(_nData >> 16) ;
		_nMeasureData = _nMeasureData / (10.24 * 16);
	}
	_nGateHeight = GROUP_VAL_POS(grp , gate[1].height);
	if(_nGateHeight > fabs(_nMeasureData))
	{
		_nMeasureData = MEASURE_DATA_ND  ;
	}
	else
	{
		_nAngle = TMP(current_angle[grp]);
		_nPulseWidth  = GROUP_VAL_POS(grp , pulser_width1) / (10 * PW_DIV);
		_nWedgeDelay  = GROUP_VAL_POS(grp , wedge_delay) / 10;
		_nBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) / 10;    // 10ns
		_nWedgeDelay += _nBeamDelay + _nPulseWidth  ;
		_nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
		_nMeasureData = (double)((_nData & 0xfffff) - _nWedgeDelay);
		_nMeasureData = _nMeasureData * _nVelocity * sin(_nAngle) / 200000 + GROUP_VAL_POS(grp , field_distance[_nBeamNo]) ;//TMP(field_distance[grp][_nBeamNo]);
		if(GROUP_VAL_POS(grp , skew) == 9000)
		{
			_nMeasureData += GROUP_VAL_POS(grp , index_offset) / 10.0 ;
			if (UNIT_INCH == get_unit(pp->p_config))
			{
				MM2INCH(_nMeasureData) ;
			}
		}
		else if(GROUP_VAL_POS(grp , skew) == 27000)
		{
			_nMeasureData  = GROUP_VAL_POS(grp , index_offset) / 10.0 - _nMeasureData;
			if (UNIT_INCH == get_unit(pp->p_config))
			{
				MM2INCH(_nMeasureData) ;
			}
		}
		else
		{
			_nMeasureData  = MEASURE_DATA_ND  ;
		}
	}

     return _nMeasureData ;
}

double getMeasureData_VsA(int grp , int _nIndex)
{
	int _nData  ;
	double _nGateHeight , _nAngle ;
	double _nVelocity;
	int _nPulseWidth ,_nWedgeDelay  ,_nBeamDelay  ;

	double _nMeasureData  ;
	int _nBeamNo = TMP(beam_num[grp]) ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ;
	_nData = TMP(measure_data[_nIndex][1]) ;

	if(_bRectifier)
	{
		_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
	}
	else
	{
		_nMeasureData = (signed short)(_nData >> 16) ;
		_nMeasureData = _nMeasureData / (10.24 * 16);
	}
	_nGateHeight = GROUP_VAL_POS(grp , gate[0].height);
	if(_nGateHeight > fabs(_nMeasureData))
	{
		_nMeasureData = MEASURE_DATA_ND  ;
	}
	else
	{
		_nAngle = TMP(current_angle[grp]);
		_nPulseWidth  = GROUP_VAL_POS(grp , pulser_width1) / (10 * PW_DIV);
		_nWedgeDelay  = GROUP_VAL_POS(grp , wedge_delay) / 10;
		_nBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) / 10;    // 10ns
		_nWedgeDelay += _nBeamDelay + _nPulseWidth  ;
		_nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
		_nMeasureData = (double)((_nData & 0xfffff) - _nWedgeDelay);
		_nMeasureData = _nMeasureData * _nVelocity * sin(_nAngle) / 200000 + GROUP_VAL_POS(grp , field_distance[_nBeamNo]) ;//TMP(field_distance[grp][_nBeamNo]);
		if(GROUP_VAL_POS(grp , skew) == 0)
		{
			_nMeasureData += GROUP_VAL_POS(grp , index_offset) / 10.0 ;
			if (UNIT_INCH == get_unit(pp->p_config))
			{
				MM2INCH(_nMeasureData) ;
			}
		}
		else if(GROUP_VAL_POS(grp , skew) == 18000)
		{
			_nMeasureData  = GROUP_VAL_POS(grp , index_offset) / 10.0 - _nMeasureData;
			if (UNIT_INCH == get_unit(pp->p_config))
			{
				MM2INCH(_nMeasureData) ;
			}
		}
		else
		{
			_nMeasureData  = MEASURE_DATA_ND  ;
		}
	}

     return _nMeasureData ;
}

double getMeasureData_VsB(int grp , int _nIndex)
{
	int _nData  ;
	double _nGateHeight , _nAngle ;
	double _nVelocity;
	int _nPulseWidth ,_nWedgeDelay  ,_nBeamDelay  ;

	double _nMeasureData  ;
	int _nBeamNo = TMP(beam_num[grp]) ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ;
	_nData = TMP(measure_data[_nIndex][2]) ;

	if(_bRectifier)
	{
		_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
	}
	else
	{
		_nMeasureData = (signed short)(_nData >> 16) ;
		_nMeasureData = _nMeasureData / (10.24 * 16);
	}
	_nGateHeight = GROUP_VAL_POS(grp , gate[1].height);
	if(_nGateHeight > fabs(_nMeasureData))
	{
		_nMeasureData = MEASURE_DATA_ND  ;
	}
	else
	{
		_nAngle = TMP(current_angle[grp]);
		_nPulseWidth  = GROUP_VAL_POS(grp , pulser_width1) / (10 * PW_DIV);
		_nWedgeDelay  = GROUP_VAL_POS(grp , wedge_delay) / 10;
		_nBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) / 10;    // 10ns
		_nWedgeDelay += _nBeamDelay + _nPulseWidth  ;
		_nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
		_nMeasureData = (double)((_nData & 0xfffff) - _nWedgeDelay);
		_nMeasureData = _nMeasureData * _nVelocity * sin(_nAngle) / 200000 + GROUP_VAL_POS(grp , field_distance[_nBeamNo]) ; //TMP(field_distance[grp][_nBeamNo]);
		if(GROUP_VAL_POS(grp , skew) == 0)
		{
			_nMeasureData += GROUP_VAL_POS(grp , index_offset) / 10.0 ;
			if (UNIT_INCH == get_unit(pp->p_config))
			{
				MM2INCH(_nMeasureData) ;
			}

		}
		else if(GROUP_VAL_POS(grp , skew) == 18000)
		{
			_nMeasureData  = GROUP_VAL_POS(grp , index_offset) / 10.0 - _nMeasureData;
			if (UNIT_INCH == get_unit(pp->p_config))
			{
				MM2INCH(_nMeasureData) ;
			}
		}
		else
		{
			_nMeasureData  = MEASURE_DATA_ND  ;
		}
	}

     return _nMeasureData ;
}

double getMeasureData_LA(int grp , int _nIndex)
{
	int _nData  ;
	int _nReflectTimes ;
	double _nGateHeight ;
	double _nVelocity;
	int _nPulseWidth ,_nWedgeDelay  ,_nBeamDelay  ;
	double _nThickness ;//工件厚度
	double _nAngle ;
	double _nMeasureData  ;
	int _nBeamNo = TMP(beam_num[grp]) ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ;
	_nData = TMP(measure_data[_nIndex][1]) ;

	if(_bRectifier)
	{
		_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
	}
	else
	{
		_nMeasureData = (signed short)(_nData >> 16) ;
		_nMeasureData = _nMeasureData / (10.24 * 16);
	}
	_nGateHeight = GROUP_VAL_POS(grp , gate[0].height);
	if(_nGateHeight > fabs(_nMeasureData))
	{
		_nMeasureData = MEASURE_DATA_ND  ;
	}
	else
	{
		_nAngle = TMP(current_angle[grp]);
		_nThickness = (double)(GROUP_VAL_POS(grp , part.Thickness)/1000.0);//工件厚度
		_nPulseWidth  = GROUP_VAL_POS(grp , pulser_width1) / (10 * PW_DIV);
		_nWedgeDelay  = GROUP_VAL_POS(grp , wedge_delay) / 10;
		_nBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) / 10;    // 10ns
		_nWedgeDelay += _nBeamDelay + _nPulseWidth  ;
		_nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
		_nMeasureData = (double)((_nData & 0xfffff) - _nWedgeDelay);
		_nMeasureData = _nMeasureData * _nVelocity * cos(_nAngle) / 200000 ;
		_nReflectTimes = (int)(_nMeasureData / _nThickness) ;
		_nMeasureData = _nReflectTimes ;
	}

	return _nMeasureData ;
}

double getMeasureData_LB(int grp , int _nIndex)
{
	int _nData  ;
	int _nReflectTimes ;
	double _nGateHeight ;
	double _nVelocity;
	int _nPulseWidth ,_nWedgeDelay  ,_nBeamDelay  ;
	double _nThickness ;//工件厚度
	double _nAngle ;
	double _nMeasureData  ;
	int _nBeamNo = TMP(beam_num[grp]) ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ;
	_nData = TMP(measure_data[_nIndex][2]) ;

	if(_bRectifier)
	{
		_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
	}
	else
	{
		_nMeasureData = (signed short)(_nData >> 16) ;
		_nMeasureData = _nMeasureData / (10.24 * 16);
	}
	_nGateHeight = GROUP_VAL_POS(grp , gate[1].height);
	if(_nGateHeight > fabs(_nMeasureData))
	{
		_nMeasureData = MEASURE_DATA_ND  ;
	}
	else
	{
		_nAngle = TMP(current_angle[grp]);
		_nThickness = (double)(GROUP_VAL_POS(grp , part.Thickness)/1000.0);//工件厚度
		_nPulseWidth  = GROUP_VAL_POS(grp , pulser_width1) / (10 * PW_DIV);
		_nWedgeDelay  = GROUP_VAL_POS(grp , wedge_delay) / 10;
		_nBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) / 10;    // 10ns
		_nWedgeDelay += _nBeamDelay + _nPulseWidth  ;
		_nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
		_nMeasureData = (double)((_nData & 0xfffff) - _nWedgeDelay);
		_nMeasureData = _nMeasureData * _nVelocity * cos(_nAngle) / 200000 ;
		_nReflectTimes = (int)(_nMeasureData / _nThickness) ;
		_nMeasureData = _nReflectTimes ;
	}

	return _nMeasureData ;
}

double getMeasureData_AWSD_A(int grp , int _nIndex)
{
    double _nMeasureData  , gate_height ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ;
	int _nData = TMP(measure_data[_nIndex][1]) ;
    AWS_D_15_P pInfo  = &GROUP_VAL_POS(grp , AwsCalibration);
	if(pInfo->bCalibrated)
	{
		if(_bRectifier)
		{
			_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
		}
		else
		{
			_nMeasureData = (signed short)(_nData >> 16) ;
			_nMeasureData = _nMeasureData / (10.24 * 16);
		}
		gate_height = GROUP_VAL_POS(grp , gate[0].height);
		if(gate_height > fabs(_nMeasureData))
		{
			_nMeasureData = MEASURE_DATA_NULL  ;
		}
		else
		{
			_nMeasureData =  80.0 / _nMeasureData  ;
			_nMeasureData = (short)(log10(_nMeasureData)*20) + pInfo->nReferenceGain;
		}
	}
	else
	{
		_nMeasureData = MEASURE_DATA_NULL ;
	}
	return _nMeasureData ;
}

double getMeasureData_AWSD_B(int grp , int _nIndex)
{
    double _nMeasureData  ;
    AWS_D_15_P pInfo  = &GROUP_VAL_POS(grp , AwsCalibration);
	if(pInfo->bCalibrated)
	{
		_nMeasureData = pInfo->nReferenceGain ;
	}
	else
	{
		_nMeasureData = MEASURE_DATA_NULL ;
	}
	return _nMeasureData ;
}

double getMeasureData_AWSD_C(int grp , int _nIndex)
{
    double _nMeasureData  , gate_height , _nVelocity;
	int _nData = TMP(measure_data[_nIndex][1]) ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ;
    AWS_D_15_P pInfo  = &GROUP_VAL_POS(grp , AwsCalibration);
    int _nPulseWidth ,_nWedgeDelay  ,_nBeamDelay  ;
    int _nBeamNo = TMP(beam_num[grp]) ;
	if(pInfo->bCalibrated)
	{
		if(_bRectifier)
		{
			_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
		}
		else
		{
			_nMeasureData = (signed short)(_nData >> 16) ;
			_nMeasureData = _nMeasureData / (10.24 * 16);
		}
		gate_height = GROUP_VAL_POS(grp , gate[0].height);
		if(gate_height > fabs(_nMeasureData))
		{
			_nMeasureData = MEASURE_DATA_NULL  ;
		}
		else
		{
			_nPulseWidth  = GROUP_VAL_POS(grp , pulser_width1) / (10 * PW_DIV);
			_nWedgeDelay  = GROUP_VAL_POS(grp , wedge_delay) / 10;
			_nBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) / 10;    // 10ns
			_nWedgeDelay += _nBeamDelay + _nPulseWidth  ;
			_nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
			_nMeasureData = (double)((_nData & 0xfffff) - _nWedgeDelay);
			_nMeasureData = _nMeasureData * _nVelocity  / 200000 ;
			_nMeasureData = (_nMeasureData * _nVelocity / 200000  - 25.4) * 0.079 ;
		}
	}
	else
	{
		_nMeasureData = MEASURE_DATA_NULL ;
	}

	return _nMeasureData  ;
}

double getMeasureData_AWSD_D(int fieldIndex , int grp , int _nIndex)
{
    double _nMeasureData  , gate_height , _nVelocity , _nTmpValue;
	int _nData = TMP(measure_data[_nIndex][1]) ;
	int _bRectifier = GROUP_VAL_POS(grp , rectifier1) ;
    AWS_D_15_P pInfo  = &GROUP_VAL_POS(grp , AwsCalibration);
    int _nPulseWidth ,_nWedgeDelay  ,_nBeamDelay  ;
    int _nBeamNo = TMP(beam_num[grp]) ;
	if(pInfo->bCalibrated)
	{
		if(_bRectifier)
		{
			_nMeasureData = ((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
		}
		else
		{
			_nMeasureData = (signed short)(_nData >> 16) ;
			_nMeasureData = _nMeasureData / (10.24 * 16);
		}
		gate_height = GROUP_VAL_POS(grp , gate[0].height);
		if(gate_height > fabs(_nMeasureData))
		{
			_nMeasureData = MEASURE_DATA_ND  ;
		}
		else
		{
			_nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
			_nMeasureData =  80.0 /_nMeasureData  ;
			_nMeasureData = (short)(log10(_nMeasureData)*20);
			_nPulseWidth  = GROUP_VAL_POS(grp , pulser_width1) / (10 * PW_DIV);
			_nWedgeDelay  = GROUP_VAL_POS(grp , wedge_delay) / 10;
			_nBeamDelay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) / 10;    // 10ns
			_nWedgeDelay += _nBeamDelay + _nPulseWidth  ;
			_nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
			_nTmpValue = (double)((_nData & 0xfffff) - _nWedgeDelay);
			_nTmpValue = _nTmpValue * _nVelocity  / 200000 ;
			_nTmpValue = (_nTmpValue * _nVelocity / 200000  - 25.4) * 0.079 ;
			_nMeasureData  -=  _nTmpValue;
		}
	}
	else
	{
		_nMeasureData = MEASURE_DATA_NULL ;
	}
    //case 53://AWS-D 45
	if(fieldIndex >  44 )
	{
		_nMeasureData = LookupTabelAWS(fieldIndex - 45, _nMeasureData) + MEASURE_DATA_CHAR_BASE;
	}
	return _nMeasureData  ;
}

#include "../string/getFieldString.h"
#include "../lzk/tofd.h"
double getMeasureData_Tofd(int fieldIndex , int grp , int _nIndex)
{
    double _nMeasureData;
	switch(fieldIndex)//field1
	{
		case FEILD_Depth1:
		{
			double u_reference = GROUP_VAL_POS(grp , u_reference)/1000.0 ;
            _nMeasureData = GetDepthCal(grp ,u_reference, 1);
		}
		break;
		case FEILD_Depth2:
		{
			double u_measure = GROUP_VAL_POS(grp , u_measure)/1000.0 ;
            _nMeasureData = GetDepthCal(grp ,u_measure, 1);
		}
		break;
		case FEILD_Length:
		{
			double s_reference = GROUP_VAL_POS(grp , s_reference)/100.0 ;
			double s_measure   = GROUP_VAL_POS(grp , s_measure)/100.0 ;
			if(0 == get_inspec_source (pp->p_config))
			{
				double rate =  GROUP_VAL_POS(0 , prf1) * get_area_scanresolution (pp->p_config) / 10000.0;
				s_reference = s_reference / rate ;
				s_measure   = s_measure / rate ;
			}
			_nMeasureData = fabs(s_measure - s_reference);
		}
		break;
		case FEILD_Height:
		{
			double u_reference = GROUP_VAL_POS(grp , u_reference)/1000.0 ;
			double u_measure   = GROUP_VAL_POS(grp , u_measure)/1000.0 ;
			_nMeasureData = GetHeight(grp ,u_reference, u_measure);
		}
		break;
	default :
		_nMeasureData = MEASURE_DATA_ND ;
		break;
	}
	return _nMeasureData;
}
/* 计算测量数值 */
//fieldIndex为索引值，根据索引值输出相应计算的测量值
double getMeasureData(int fieldIndex , int grp , int _nIndex)
{
	double _nMeasureData ;
	double velocity ;
	fieldIndex = GetMesureFeildIndex(grp ,fieldIndex);
	switch(fieldIndex)//field1
	{
		case FEILD_A100://A%
			_nMeasureData = getMeasureData_aHeight(grp , _nIndex);
			break;
		case FEILD_AdBA://AdBA
			_nMeasureData = getMeasureData_aDBa(grp , _nIndex);
			break;
		case FEILD_AdBr://AdBr
			_nMeasureData = getMeasureData_aDBr(grp , _nIndex);
			break;
		case FEILD_B100://B%
			_nMeasureData = getMeasureData_bHeight(grp , _nIndex);
			break;
		case FEILD_BdBB://BdBB
			_nMeasureData = getMeasureData_bDBb(grp , _nIndex);
			break;
		case FEILD_BdBr://BdBr
			_nMeasureData = getMeasureData_bDBr(grp , _nIndex);
			break;
		case FEILD_APeak://A^
		case FEILD_AEdge://A/
			_nMeasureData = getMeasureData_aPeak(grp , _nIndex);
			break;
		case FEILD_BPeak://B^
		case FEILD_BEdge://B/
			_nMeasureData = getMeasureData_bPeak(grp , _nIndex);
			break;
		case FEILD_IEdge://I/
			_nMeasureData = getMeasureData_iEdge(grp , _nIndex);
			break;
		case FEILD_DaDi:
			_nMeasureData = getMeasureData_aPeak(grp , _nIndex) - getMeasureData_iEdge(grp , _nIndex);
			break;
		case FEILD_IEdgeInWater://I(w)  use velocity of 1480 (sonic transfer speed in water)
			_nMeasureData = getMeasureData_iEdgeWater(grp , _nIndex);
			break;
		case FEILD_ThicknessAPeak://T(A^)
			_nMeasureData = getMeasureData_aThickness(grp , _nIndex);
			break;
		case FEILD_ML://ML
			_nMeasureData = getMeasureData_ML(grp , _nIndex);
			break;
		case FEILD_r100://%(r)
			_nMeasureData = GROUP_VAL_POS( grp , per_reference)/100.0;
			break;
		case FEILD_m100://%(m)
			_nMeasureData = GROUP_VAL_POS( grp , per_measure)/100.0;
			break;
		case FEILD_mr100://%(m-r)
			_nMeasureData = (GROUP_VAL_POS( grp , per_measure) - GROUP_VAL_POS( grp , per_reference))/100.0;
			break;
		case FEILD_Ur://U(r)
			_nMeasureData = GROUP_VAL_POS( grp , u_reference)/1000.0;
			if  (UT_UNIT_TIME != GROUP_VAL_POS( grp , ut_unit))
			{
				velocity = GROUP_VAL_POS(grp , velocity) / 200000.0 ;
				_nMeasureData *= velocity;
				if (UNIT_INCH == get_unit(pp->p_config))
				{
					MM2INCH(_nMeasureData) ;
				}
			}
			break;
		case FEILD_Um://U(m)
			_nMeasureData = GROUP_VAL_POS( grp , u_measure)/1000.0;
			if  (UT_UNIT_TIME != GROUP_VAL_POS( grp , ut_unit))
			{
				velocity = GROUP_VAL_POS(grp , velocity) / 200000.0 ;
				_nMeasureData *= velocity;
				if (UNIT_INCH == get_unit(pp->p_config))
				{
					MM2INCH(_nMeasureData) ;
				}
			}
			break;
		case FEILD_Umr://U(m-r)
			_nMeasureData = (GROUP_VAL_POS( grp , u_measure) - GROUP_VAL_POS( grp , u_reference))/1000.0;
			if  (UT_UNIT_TIME != GROUP_VAL_POS( grp , ut_unit))
			{
				velocity = GROUP_VAL_POS(grp , velocity) / 200000.0 ;
				_nMeasureData *= velocity;
				if (UNIT_INCH == get_unit(pp->p_config))
				{
					MM2INCH(_nMeasureData) ;
				}
			}
			break;
		case FEILD_Pr://P(r)
			if(GROUP_VAL_POS(grp , skew_pos) == 1 || GROUP_VAL_POS(grp , skew_pos) == 3)
			{
				_nMeasureData = GROUP_VAL_POS( grp , i_reference)/100.0 - GROUP_VAL_POS( grp , index_offset)/10.0;
			}
			else
			{
				_nMeasureData = GROUP_VAL_POS( grp , s_reference)/100.0 - GROUP_VAL_POS( grp , scan_offset)/10.0;
			}
			if (UNIT_INCH == get_unit(pp->p_config))
			{
				MM2INCH(_nMeasureData) ;
			}
			break;
		case FEILD_Pm://P(m)
			if(GROUP_VAL_POS(grp , skew_pos) == 1 || GROUP_VAL_POS(grp , skew_pos) == 3)
			{
				_nMeasureData = GROUP_VAL_POS( grp , i_measure)/100.0 - GROUP_VAL_POS( grp , index_offset)/10.0;
			}
			else
			{
				_nMeasureData = GROUP_VAL_POS( grp , s_measure)/100.0 - GROUP_VAL_POS( grp , scan_offset)/10.0;
			}
			if (UNIT_INCH == get_unit(pp->p_config))
			{
				MM2INCH(_nMeasureData) ;
			}
			break;
		case FEILD_Pmr://P(m-r)
			if(GROUP_VAL_POS(grp , skew_pos) == 1 || GROUP_VAL_POS(grp , skew_pos) == 3)
			{
				_nMeasureData = (GROUP_VAL_POS( grp , i_measure) - GROUP_VAL_POS( grp , i_reference))/100.0;
			}
			else
			{
				_nMeasureData = (GROUP_VAL_POS( grp , s_measure) - GROUP_VAL_POS( grp , s_reference))/100.0;
			}
			if (UNIT_INCH == get_unit(pp->p_config))
			{
				MM2INCH(_nMeasureData) ;
			}
			break;
		case FEILD_Sr://S(r)
			_nMeasureData =  GROUP_VAL_POS( grp , s_reference)/100.0 ;//* velocity  ;
			if (UNIT_INCH == get_unit(pp->p_config))
			{
				MM2INCH(_nMeasureData) ;
			}
			break;
		case FEILD_Sm://S(m)
			_nMeasureData =  GROUP_VAL_POS( grp , s_measure) /100.0 ;//* velocity;
			if (UNIT_INCH == get_unit(pp->p_config))
			{
				MM2INCH(_nMeasureData) ;
			}
			break;
		case FEILD_Smr://S(m-r)
			//velocity = GROUP_VAL_POS(grp , velocity) / 200000.0 ;
			_nMeasureData =  (GROUP_VAL_POS( grp , s_measure) - GROUP_VAL_POS( grp , s_reference))/100.0 ;
			if (UNIT_INCH == get_unit(pp->p_config))
			{
				MM2INCH(_nMeasureData) ;
			}
			//_nMeasureData *= velocity  ;
			break;
		case FEILD_Ir://I(r)
			//velocity = GROUP_VAL_POS(grp , velocity) / 200000.0 ;
			_nMeasureData =  GROUP_VAL_POS( grp , i_reference)/ 100.0 ;//* velocity  ;
			if (UNIT_INCH == get_unit(pp->p_config))
			{
				MM2INCH(_nMeasureData) ;
			}
			break;
		case FEILD_Im://I(m)
			//velocity = GROUP_VAL_POS(grp , velocity) / 200000.0 ;
			_nMeasureData =  GROUP_VAL_POS( grp , i_measure)/100.0 ;//* velocity ;
			if (UNIT_INCH == get_unit(pp->p_config))
			{
				MM2INCH(_nMeasureData) ;
			}
			break;
		case FEILD_Imr://I(m-r)
			//velocity = GROUP_VAL_POS(grp , velocity) / 200000.0 ;
			_nMeasureData  =  (GROUP_VAL_POS( grp , i_measure) - GROUP_VAL_POS( grp , i_reference))/100.0 ;
			if (UNIT_INCH == get_unit(pp->p_config))
			{
				MM2INCH(_nMeasureData) ;
			}
			//_nMeasureData *=  velocity  ;
			break;
		case FEILD_RAPeak://RA
		case FEILD_RAEdge://RA/
			_nMeasureData = getMeasureData_RA(grp , _nIndex);
			break;
		case FEILD_RBPeak://RB
		case FEILD_RBEdge://RB/
			_nMeasureData = getMeasureData_RB(grp , _nIndex);
			break;
		case FEILD_PAPeak://PA
		case FEILD_PAEdge://PA/
			_nMeasureData = getMeasureData_PA(grp , _nIndex);
			break;
		case FEILD_PBPeak://PB
		case FEILD_PBEdge://PB/
			_nMeasureData = getMeasureData_PB(grp , _nIndex);
			break;
		case FEILD_DAPeak://DA^
		case FEILD_DAEdge://DA/
			_nMeasureData = getMeasureData_DA(grp , _nIndex);
			break;
		case FEILD_DBPeak://DB^
		case FEILD_DBEdge://DB/
			_nMeasureData = getMeasureData_DB(grp , _nIndex);
			break;
		case FEILD_SAPeak://SA^
		case FEILD_SAEdge://SA/
			_nMeasureData = getMeasureData_SA(grp , _nIndex);
			break;
		case FEILD_SBPeak://SB^
		case FEILD_SBEdge://SB/
			_nMeasureData = getMeasureData_SB(grp , _nIndex);
			break;
		case FEILD_ViAPeak://ViA
		case FEILD_ViAEdge://ViA/
			_nMeasureData = getMeasureData_ViA(grp , _nIndex);
			break;
		case FEILD_ViBPeak://ViB
		case FEILD_ViBEdge://ViB/
			_nMeasureData = getMeasureData_ViB(grp , _nIndex);
			break;
		case FEILD_VsAPeak://VsA
		case FEILD_VsAEdge://VsA/
			_nMeasureData = getMeasureData_VsA(grp , _nIndex);
			break;
		case FEILD_VsBPeak://VsB
		case FEILD_VsBEdge://VsB/
			_nMeasureData = getMeasureData_VsB(grp , _nIndex);
			break;
		case FEILD_LAPeak://LA
		case FEILD_LAEdge://LA/
			_nMeasureData = getMeasureData_LA(grp , _nIndex);
			break;
		case FEILD_LBPeak://LB
		case FEILD_LBEdge://LB/
			_nMeasureData = getMeasureData_LB(grp , _nIndex);
			break;
		//case FEILD_E100://%E
		//	_nMeasureData = MEASURE_DATA_ND ;
		//	break;
		//case FEILD_EPeak://E^
		//	_nMeasureData = MEASURE_DATA_ND ;
		//	break;
		//case FEILD_E3dB://E-3db
		//	_nMeasureData = MEASURE_DATA_ND ;
		//	break;
		//case FEILD_E6dB://E-6db
		//	_nMeasureData = MEASURE_DATA_ND ;
		//	break;
		//case FEILD_E12dB://E-12db
		//	_nMeasureData = MEASURE_DATA_ND ;
		//	break;
		//case FEILD_E20dB://E-20db
		//	_nMeasureData = MEASURE_DATA_ND ;
		//	break;
		//case FEILD_APIDL://API-DL
		//	_nMeasureData = MEASURE_DATA_ND ;
		//	break;
		//case FEILD_APIHW://API-HW
		//	_nMeasureData = MEASURE_DATA_ND ;
		//	break;
		case FEILD_AWSDA://AWS-D A
			_nMeasureData = getMeasureData_AWSD_A(grp , _nIndex);
			break;
		case FEILD_AWSDB://AWS-D B
			_nMeasureData = getMeasureData_AWSD_B(grp , _nIndex);
			break;
		case FEILD_AWSDC://AWS-D C
			_nMeasureData = getMeasureData_AWSD_C(grp , _nIndex);
			break;
		case FEILD_AWSDD://AWS-D D
		case FEILD_AWSD45://AWS-D 45
		case FEILD_AWSD60://AWS-D 60
		case FEILD_AWSD70://AWS-D 70
		case FEILD_AWSDCL://AWS-D CL
			_nMeasureData = getMeasureData_AWSD_D(fieldIndex , grp , _nIndex);
			break;
		//case FEILD_Scale://Scale
		//	_nMeasureData = MEASURE_DATA_ND ;
		//	break;
		//case FEILD_Ur100://%(U(r))
		//	_nMeasureData = MEASURE_DATA_ND ;
		//	break;
		//case FEILD_Um100://%(U(m))
		//	_nMeasureData = MEASURE_DATA_ND ;
		//	break;

		case FEILD_Depth1:
		case FEILD_Depth2:
		case FEILD_Length:
		case FEILD_Height:
			_nMeasureData = getMeasureData_Tofd(fieldIndex , grp , _nIndex);
			break;

		default :
			_nMeasureData = MEASURE_DATA_ND ;
			break;
	}
	return _nMeasureData;
}



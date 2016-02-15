/*
 * limit.c
 *
 *  Created on: 2012-10-18
 *      Author: wolflord
 */


#include "../drawui.h"
#include "../callback.h"

#include "limit.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../ui/authorization.h"
#include "../calculation/limit.h"
// adjust gain offset
// if gain offset if changed return -1 , else return 0;
int ConfirmGainOffsetOfAllBeamInLimit(int grp)
{
	int ret = 0 ;
    int _nGain = group_get_gain(grp) ; // 0 - 8000 (80DB)
	int i , _nTmpValue;
	int _nBeamQty = TMP(beam_qty[grp]) ;
    int maxGain = 0;
	for(i = 0 ; i < _nBeamQty ; i++)
	{
		_nTmpValue = _nGain + GROUP_VAL_POS(grp , gain_offset[i]) * 10;


        if (PA_SCAN == GROUP_VAL_POS(grp, group_mode)
                     || UT_SCAN == GROUP_VAL_POS(grp, group_mode)) {
            maxGain = PA_MAX_GAIN * 100;
        } else {
            maxGain = UT_MAX_GAIN * 100;
        }

        if(_nTmpValue > maxGain)
		{
			ret = -1 ;
            GROUP_VAL_POS(grp , gain_offset[i]) = (maxGain - _nGain)/10 ;
		}
	}
	return ret ;
}


double GetGainOffsetLimit(int grp)
{
    int _nGain = group_get_gain(grp) ; // 0 - 8000 (80DB)
	double _nResult = (8000 - _nGain) / 100.0 ;
	if(_nResult > 40)  _nResult = 40 ;
	return _nResult ;
}

double GetMaxGainOffset(int grp)
{
	int i , _nTmpValue;
	int _nBeamQty = TMP(beam_qty[grp]) ;

	_nTmpValue = GROUP_VAL_POS(grp , gain_offset[0]);
	for(i = 1 ; i < _nBeamQty ; i++)
	{
		if(_nTmpValue < GROUP_VAL_POS(grp , gain_offset[i]))
		{
			_nTmpValue = GROUP_VAL_POS(grp , gain_offset[i]) ;
		}
	}

	return _nTmpValue / 100.0 ;
}

int GetGroupDataSize(int grp)
{
	int _nDataSize = GetGroupBeamQty(grp) * (GROUP_VAL_POS (grp, point_qty) + 32);
	return _nDataSize ;
}

int GetTotalDataSize()
{
	int _nTotalDataSize = DMA_DATA_OFFSET;
	int _nGroupQty      ;
	int i ;
	_nGroupQty = get_group_qty(pp->p_config);

	for(i = 0 ; i < _nGroupQty ; i++)
	{
		_nTotalDataSize += GetGroupBeamQty(i) * (GROUP_VAL_POS (i, point_qty) + 32);
	}
	return _nTotalDataSize ;
}


int getDataBlockSize()
{
	int _nDataSize = GetTotalDataSize();

	// OMAP3730 DMA data size should alien with a minimun size
	if( _nDataSize % DATA_SAVE_BLOCK_SIZE )
	{
		_nDataSize = _nDataSize / DATA_SAVE_BLOCK_SIZE + 1 ;
	}
	else
	{
		_nDataSize = _nDataSize / DATA_SAVE_BLOCK_SIZE ;
	}

	return _nDataSize * DATA_SAVE_BLOCK_SIZE ;
}

#define MAX_DATA_SIZE        (192*1024*1.0)
int RefreshPointQty()
{

	int _nDataSize = GetTotalDataSize() ;
	double _nScale ;
	// if data size is in limit, get out with no operation
	if(_nDataSize <= MAX_DATA_SIZE) return  0;

	_nScale  = _nDataSize / MAX_DATA_SIZE  ;
	_nScale += 0.1 ;
	int _nGroupQty = get_group_qty(pp->p_config);
	int i;    double _nCompressRate  ;
	for(i = 0 ; i < _nGroupQty ; i++)
	{
		GROUP_VAL_POS(i , point_qty) = GROUP_VAL_POS(i , point_qty) / _nScale ;
		_nCompressRate = GROUP_VAL_POS(i , range) / (10 * GROUP_VAL_POS(i , point_qty))   ;
		if(_nCompressRate < 1)
		{
			_nCompressRate = 1 ;
		}
		_nCompressRate = (int)_nCompressRate ;
		GROUP_VAL_POS(i , range)  = GROUP_VAL_POS(i , point_qty) * 10 * _nCompressRate  ;

	}
	return -1 ;
}

int GetCurrentGroupLawsQtyLimit(int grp)
{
	int _nResult ;
	int i , _nTotalLawQty = 0 , _nGroupQty = get_group_qty(pp->p_config);

	for(i = 0 ; i< _nGroupQty ; i++)
	{
		if(grp != i)
		{
			_nTotalLawQty += TMP(beam_qty[i]) ;
		}
	}

	_nResult = 1024 - _nTotalLawQty ;
	return MIN(_nResult , 256) ;

}

int isScanRangeInvalidate()
{
	double _nRangeStart ;
	double _nRangeStop  ;
	double _nRangeStep  ;

	int _nDataSize   =   getDataBlockSize() / 1024;

	_nRangeStart  = pp->p_config->inspection_scan_start / 1000.0;
	_nRangeStop   = pp->p_config->inspection_scan_end   / 1000.0;
	_nRangeStep   = pp->p_config->inspection_scan_resolution / 1000.0;
	// total scan beam number
	unsigned long _nScanStepQty     = (unsigned long) ((_nRangeStop - _nRangeStart) / _nRangeStep + 1) ;
	unsigned long _nScanDataSize    = _nScanStepQty * _nDataSize ;

	if(_nScanDataSize >= 255 * 1024 )
		return TRUE;

	return FALSE;
}


void ConfirmPulserReceiverInRange(int grp)
{
	int _nPulser   = GROUP_VAL_POS(grp , pulser1) ;
	int _nReceiver = GROUP_VAL_POS(grp , receiver1) ;

	if(LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
	{
		if(_nPulser > (gMaxElementRecieveQty+1-LAW_VAL_POS(grp , Elem_qty)))
			GROUP_VAL_POS(grp , pulser1) = 1;
		if(_nReceiver > (gMaxElementRecieveQty+1-LAW_VAL_POS(grp , Elem_qty)))
			GROUP_VAL_POS(grp , receiver1) = 1;
	}
	else
	{
		if(_nPulser > (gMaxElementRecieveQty+1-LAW_VAL_POS(grp , Last_tx_elem)))
			GROUP_VAL_POS(grp , pulser1) = 1;
		if(_nReceiver > (gMaxElementRecieveQty+1-LAW_VAL_POS(grp , Last_tx_elem)))
			GROUP_VAL_POS(grp , receiver1) = 1;
	}
}

int getPulserReceiverRange(int grp)
{
	int _nResult = gMaxElementRecieveQty;
	if(LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
	{
		_nResult = gMaxElementRecieveQty + 1 - LAW_VAL_POS(grp , Elem_qty) ;
	}
	else
	{
		_nResult = gMaxElementRecieveQty + 1 - LAW_VAL_POS(grp , Last_tx_elem) ;
	}
	return _nResult ;
}

int RefractAngleRange(double *lower, double *upper, GROUP *fgroup, int choose)
{
    double elemQty, pitch, hi_base, wg_elem_offset, c1, c2, wr, wl, wangle;
    double x0, z0, bevels, dis1, dis2, x1, z1, x2, z2, tAngle, eAngle, tmpR, tmp, k1, kk1, a, b;
	int grp = get_current_group(pp->p_config);
    if (choose == 0) {
	if (fgroup->part.Geometry == 2) {
	    elemQty = fgroup->law_info.Elem_qty;
		pitch = fgroup->probe.Pitch / 1000.0;
	    wangle = fgroup->wedge.Angle *M_PI/ 1800.0;
		hi_base = fgroup->wedge.Height / 1000.0;
		wg_elem_offset = -fabs(fgroup->wedge.Primary_offset / 1000.0);
	    wr = fgroup->part.Diameter/2000.0;
        wl = fgroup->wedge.length / 1000.0;
        c1 = fgroup->wedge.Velocity_PA / 1000.0;
        c2 = get_group_val (fgroup, GROUP_VELOCITY) / 100;
	    bevels = pitch*(fgroup->law_info.Last_tx_elem -
                fgroup->law_info.Elem_qty + (fgroup->law_info.Elem_qty-1)/2.0f);
        x0 = wg_elem_offset + wl/2 + cos(wangle)*bevels;
        z0 = hi_base + sin(wangle)*bevels - sqrt(wr*wr-wl*wl/4);
        tmpR = wr*wr;
        k1 = c1/c2;
        kk1 = k1*k1;
        x1 = wl/2;
        z1 = -sqrt(wr*wr-wl*wl/4);
        dis1 = x0*x0+z0*z0;
        dis2 = sqrt(dis1)*c2/(c1*wr);
        if (dis2 >1) {
            tAngle = 89;
            tmp = dis1-tmpR*kk1;
            if (tmp<0) {
                x2 = x1-0.001;
            } else {
                tmp = tmp*(1-kk1);
                tmp = tmpR*kk1+wr*sqrt(tmp);
                a = x0*tmp;
                b = z0*sqrt(dis1*tmpR-tmp*tmp);
                x2 = (a-b)/dis1;
                z2 = (tmp-x0*x2)/z0;
            }
        } else {
            tAngle = asin(dis2)*180/M_PI;
            x2 = x0 - z0*sqrt((wr*wr-dis1)/dis1);
            z2 = -sqrt(tmpR-x2*x2);
        }

        dis1 = (x0*z1-x1*z0)*c2;
        dis2 = wr*c1*sqrt((x0-x1)*(x0-x1)+(z0-z1)*(z0-z1));
        if (fabs(dis1) > dis2) {
            eAngle = 89;
        } else {
            eAngle = asin(dis1/dis2)*180/M_PI;
        }

        if (x2>x1) {
            *upper = eAngle;
        } else if (x2<x1) {
            *upper = 2*tAngle-eAngle;
        }
        if (*upper > 89) {
            *upper = 89;
        }

	    bevels = pitch*(fgroup->law_info.First_tx_elem - 1 + (fgroup->law_info.Elem_qty-1)/2.0f);
        x0 = wg_elem_offset + wl/2 + cos(wangle)*bevels;
        z0 = hi_base + sin(wangle)*bevels - sqrt(wr*wr-wl*wl/4);
        x1 = -wl/2;
        dis1 = sqrt((x0-x1)*(x0-x1)+(z0-z1)*(z0-z1));
        dis2 = (x0*z1-x1*z0)/(wr*dis1*k1);
        if (fabs(dis2)>1) {
            *lower = -89;
        } else {
            *lower = asin(dis2) * 180/M_PI;
        }
    } else if (fgroup->part.Geometry == 1) {
	    wr = fgroup->part.Diameter/2000.0;
	    dis1 = fgroup->law_info.Position_start/ 1000.0;
        if ((dis1 <0.01) || (dis1+0.01>wr)) {
            *lower = 0;
            *upper = 0;
        } else {
            tmp = asin((wr-dis1)/wr)*180/M_PI;
            *lower = -tmp;
            *upper = tmp;
        }
    } else {
        *lower = -89 ;
        *upper =  89 ;
    }
    } else if (choose == 1) {
	    float multiplier;
    	if(UNIT_MM == get_unit(pp->p_config)) {
    		multiplier = 1.0;
	    } else {
    		multiplier = 0.03937;
    	}
	    if (fgroup->part.Geometry == 1) {
	        wr = fgroup->part.Diameter/2000.0;
	        tAngle = fabs(fgroup->law_info.Angle_min) / 100.0;
	        eAngle = fabs(fgroup->law_info.Angle_max) / 100.0;
            tmp = tAngle>eAngle ? tAngle:eAngle;
            *upper = wr*(1-sin(tmp*M_PI/180));
            *lower = 0;
        }  else {
            *lower = 0.1 * multiplier;
            *upper = 10000.0 * multiplier;
        }
    }
    return 0;
}

/*
 * 1. set part diameter, adjust angle and depth
 *
 */
void ResolutionConflict(int index)
{
	float multiplier = 1.0;
    double lower, upper;
    short useLower, useUpper;
	int grp = get_current_group(pp->p_config);

	if(UNIT_MM == get_unit(pp->p_config))
	{
		multiplier = 1.0;
	}else{
		multiplier = 0.03937;
	}

    useLower = LAW_VAL_POS(grp , Angle_min)/100.0;
    useUpper = LAW_VAL_POS(grp , Angle_max)/100.0;
    switch (index)
    {
        case 0:
            if (get_part_geometry(grp) != 0) {
                RefractAngleRange(&lower, &upper, get_group_by_id (pp->p_config, grp), 0);
                if (lower<0 && upper >0) {
                    LAW_VAL_POS(grp , Angle_min) = 0;
                    LAW_VAL_POS(grp , Angle_max) = 0;
                } else if (lower>-0.0001 && upper >0) {
                    LAW_VAL_POS(grp , Angle_min) = lower*100.0;
                    LAW_VAL_POS(grp , Angle_max) = lower*100.0;
                } else {
                    LAW_VAL_POS(grp , Angle_min) = upper*100.0;
                    LAW_VAL_POS(grp , Angle_max) = upper*100.0;
                }
            }
            break;
        case 1:
            RefractAngleRange(&lower, &upper, get_group_by_id (pp->p_config, grp), 0);
            useLower = (short)lower*100;
            useUpper = (short)upper*100;
            if (LAW_VAL_POS(grp , Angle_max) > useUpper) {
                LAW_VAL_POS(grp , Angle_max) = useUpper;
            }
            if (LAW_VAL_POS(grp , Angle_min) < useLower) {
                LAW_VAL_POS(grp , Angle_min) = useLower;
            }
            break;
        default:
            break;
    }
}

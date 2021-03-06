/*
 * tofd.c
 *
 *  Created on: 2013-11-15
 *      Author: lzk
 */

#include "tofd.h"
#include "../base.h"
#include "../drawui.h"
#include <math.h>

#define POW(x) ((x)*(x))
#define MEASURE_DATA_ND    987654321.123

double GetDepth(int grp ,double pos)
{
    double _fPcs  = GROUP_VAL_POS(grp , field_distance[254]) ;  // 探头中心距
	double _fVelocity = GROUP_VAL_POS(grp , velocity) / 100000.0 ;
	pos = pos * _fVelocity ;

	if(GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_TRUE_DEPTH)
	{
		pos = pos / cos(TMP(current_angle[grp])) ;
	}

	double depth;
	//printf("pos %f pcs %f \n" , pos , _fPcs);
	if(pos - _fPcs < 0.001)
	{
		depth = MEASURE_DATA_ND;
	}
	else
	{
		depth = pos * pos - _fPcs * _fPcs ;
		depth = sqrt(depth) / 2;
	}
	return depth;
}

double GetDepthCal(int grp, double pos, int flage)
{
    double _fPcs  = GROUP_VAL_POS(grp , field_distance[254]) ; // 探头中心距
	double posTmp, depth;

    if (GROUP_VAL_POS(grp , field_distance[252]) < 0.1) {
        /*无深度校准*/
        depth = GetDepth(grp, pos);
    } else {
        /*尝试校准*/
        posTmp = (pos-GROUP_VAL_POS(grp , field_distance[251])) * GROUP_VAL_POS(grp , velocity)/100000.0;
    	if(GROUP_VAL_POS(grp , ut_unit) == UT_UNIT_TRUE_DEPTH) {
            posTmp = posTmp / cos(TMP(current_angle[grp])) ;
        }

        if(posTmp - _fPcs < -0.0001) {
            depth = -sqrt(_fPcs*_fPcs- posTmp*posTmp)/2;
        } else if (posTmp - _fPcs < 0.0001) {
            depth = 0;
        } else {
            depth = sqrt(posTmp * posTmp - _fPcs * _fPcs)/2;
        }
    }
    return depth;
}

//0.001mm //  /1000得到mm
double GetHeight(int grp ,double pos1 ,double pos2)
{
    double depth1 = GetDepthCal(grp , pos1, 1);
    double depth2 = GetDepthCal(grp , pos2, 1);
	if((fabs(MEASURE_DATA_ND - depth1) < 0.001) || (fabs(MEASURE_DATA_ND - depth2) < 0.001))
	{
		return MEASURE_DATA_ND;
	}
	return fabs(depth1 - depth2);

}

 double GetLength(struct TofdStruct* pTofd ,struct DefectStruct* pDefect1 ,struct DefectStruct* pDefect2)
{
	double length;
	length = fabs(pDefect2->x - pDefect1->x) * pTofd->mmPerPixel;
	return length;
}

void GetPointer(struct TofdStruct* pTofd ,struct DefectStruct* pDefect ,double* xx ,int numOfPoint)
{
	int i;
	double t_banshengcheng = pDefect->t / 2 ;
	double t_zhitong = pTofd->pcs / pTofd->c / 2 ;
	if(t_banshengcheng - t_zhitong < 0.001)
	{
		for(i = 0 ;i < numOfPoint ;i++)
		{
			xx[i] = pDefect->t;
		}
	}
	else
	{
		double tmp = POW(pTofd->c *t_banshengcheng);
		for(i = 0 ;i < numOfPoint ;i++)
		{
			xx[i] = (sqrt((tmp + POW((i + 1) * pTofd->mmPerPixel))));//声程
			xx[i] /= pTofd->c;//时间
			xx[i] *= 2;
		}
	}
}

#include <string.h>
#include "../drawfb.h"
struct TofdStraighteningStruct gTofdS[MAX_GROUP] ;

static int TofdGetGatePos(const unsigned char* pData)
{
	int averageStartPos = 0;
    int grp = get_current_group(pp->p_config);

	int lo,hi;
    if(gTofdS[grp].intUm > gTofdS[grp].intUr)
	{
        hi = gTofdS[grp].intUm;
        lo = gTofdS[grp].intUr;
	}
	else
	{
        hi = gTofdS[grp].intUr;
        lo = gTofdS[grp].intUm;
	}

	int i;
	int weight = 0;
	int pos = 0;
	for(i = lo ;i < hi ;i++)
	{
		weight += (ABS(pData[i] - 128)) * i;
		pos += ABS(pData[i] - 128);
	}
	if(pos > 100)
	{
		averageStartPos = weight / pos;
	}
	return averageStartPos;
}

void TofdStraightening(unsigned char* pData ,const unsigned char* pAverageData ,int dataPoints)
{
	int averageStartPos = TofdGetGatePos(pAverageData);
	if(!averageStartPos)//参考平均波形无直通波，无效
	{
		return;
	}
	int startPos = TofdGetGatePos(pData);
	if(!startPos)//无直通波，不移动
	{
		return;
	}
	int pointsToMove;
	pointsToMove = startPos - averageStartPos;
	if(pointsToMove)
	{
		int pointsForMove = dataPoints + ABS(pointsToMove);
		char dataForMove[pointsForMove];
		memset(dataForMove , 128 ,pointsForMove);
		if(pointsToMove > 0)//目标要往前移,后面补0
		{
			memcpy(dataForMove ,pData ,dataPoints);
			memcpy(pData ,dataForMove + pointsToMove ,dataPoints);
		}
		else//目标要往后移，前补0
		{
			memcpy(dataForMove - pointsToMove ,pData ,dataPoints);
			memcpy(pData ,dataForMove ,dataPoints);
		}
	}
}

void TofdRemoveLateralWave(unsigned char* pData ,const unsigned char* pAverageData)
{
	int lo,hi;
    int grp = get_current_group(pp->p_config);
    if(gTofdS[grp].intUm > gTofdS[grp].intUr)
	{
        hi = gTofdS[grp].intUm;
        lo = gTofdS[grp].intUr;
	}
	else
	{
        hi = gTofdS[grp].intUr;
        lo = gTofdS[grp].intUm;
	}

	int i;
	int tmp;
	for(i = lo ;i < hi ;i++)
	{
		tmp = pData[i] - pAverageData[i] + 128;
		if(tmp < 0) pData[i] = 0 ;
		else if (tmp > 255) pData[i] = 255 ;
		else pData[i] = tmp ;
	}
}

#include "../drawui.h"
void TofdGetPara(int grp)
{
    double start = get_area_scanstart (pp->p_config) / 1000.0;//mm
    double end = get_area_scanend(pp->p_config) / 1000.0;//mm
    double resolution = get_area_scanresolution(pp->p_config) / 1000.0;//mm

    double tofdRefLine = gTofdS[grp].refLine ;//mm
    double s_reference = GROUP_VAL_POS(grp , s_reference)/100.0 - GROUP_VAL_POS(grp, scan_offset)/10.0;//mm
    double s_measure   = GROUP_VAL_POS(grp , s_measure)/100.0 - GROUP_VAL_POS(grp, scan_offset)/10.0;//mm

    if (get_inspec_source(pp->p_config) == 0) {
        s_reference = GROUP_VAL_POS(grp , s_reference)/100.0;//mm
        s_measure   = GROUP_VAL_POS(grp , s_measure)/100.0;//mm
    }
// g_message("%s[%d]", __func__, __LINE__);

		gTofdS[grp].intAveragePos = tofdRefLine > end ?0 :(int)((tofdRefLine - start) / resolution) + 1;
        gTofdS[grp].intSr = s_reference > end ?0 :(int)((s_reference - start) / resolution) + 1;
        gTofdS[grp].intSm = s_measure > end ?0 :(int)((s_measure - start) / resolution) + 1;
        gTofdS[grp].dataPoints = pp->nPointQty[grp];
        if(gTofdS[grp].um < 0) gTofdS[grp].um = 0 ;
        if(gTofdS[grp].um > 1) gTofdS[grp].um = 1 ;
        if(gTofdS[grp].ur < 0) gTofdS[grp].ur = 0 ;
        if(gTofdS[grp].ur > 1) gTofdS[grp].ur = 1 ;
        gTofdS[grp].intUm = (int)(gTofdS[grp].um * gTofdS[grp].dataPoints);
        gTofdS[grp].intUr = (int)(gTofdS[grp].ur * gTofdS[grp].dataPoints);
}

void TofdHandler(int i ,int grp ,int width ,int _nDataOffset ,int _nDataSize)
{
	int dataPoints = pp->nPointQty[grp];
	unsigned char* _pData;
	unsigned char* pStoreBuffer = (unsigned char*)TMP(StoreBuffAddr);
	_pData = (unsigned char*)(pStoreBuffer + i * _nDataSize + _nDataOffset);
	unsigned char tmpData[dataPoints];

	unsigned char* pAverageData = NULL;

	memcpy(tmpData ,_pData ,dataPoints);
	_pData = tmpData;
    pAverageData = (unsigned char*)(pStoreBuffer + gTofdS[grp].intAveragePos * _nDataSize + _nDataOffset);


    if(gTofdS[grp].straighteningEnable)
	{
		TofdStraightening(_pData ,pAverageData ,dataPoints);
	}

	int lo,hi;
    if(gTofdS[grp].intSm > gTofdS[grp].intSr)
	{
        hi = gTofdS[grp].intSm;
        lo = gTofdS[grp].intSr;
	}
	else
	{
        hi = gTofdS[grp].intSr;
        lo = gTofdS[grp].intSm;
	}

    if((i <= hi) && (i >= lo))
	{
        if(gTofdS[grp].removeLateralWaveEnable)
		{
			TofdRemoveLateralWave(_pData ,pAverageData);
		}
	}

	getDisplayData( _pData,
		TMP(scan_data[grp]),
		pp->nPointQty[grp],
		width,
		GROUP_VAL_POS(grp , rectifier1) == RF_WAVE);
}

#include "../ui/ui.h"
#include "../string/_string.h"
int MenuHandler_TofdStraightening(void* p_para)
{
	int group = get_current_group(pp->p_config);
    if(TOFD == group_get_rx_tx_mode(group))//tofd
	{
        if(0 == gTofdS[group].straighteningEnable)
		{
			TofdGetPara(group);
            gTofdS[group].straighteningEnable = 1;
		} else {
            MenuHandler_TofdCancel(NULL);
        }
	}
	else
	{
			int response = dialogWarning(pp->window , getDictString(_STRING_Not_Tofd));
			if(!response)
			{

			}
	}
	return TRUE;
}

int MenuHandler_TofdRemoveLateralWave(void* p_para)
{
	int group = get_current_group(pp->p_config);

    if(TOFD == group_get_rx_tx_mode(group)) {
        /* TOFD */
        if(0 == gTofdS[group].removeLateralWaveEnable) {
			TofdGetPara(group);
            gTofdS[group].removeLateralWaveEnable = 1;
		} else {
            TofdGetPara(group);
            gTofdS[group].removeLateralWaveEnable = 0;
//            MenuHandler_TofdCancel(NULL);
        }
    } else {
			int response = dialogWarning(pp->window , getDictString(_STRING_Not_Tofd));
            if(!response) {
			}
	}
	return TRUE;
}


int MenuHandler_TofdCancel(void* p_para)
{
    int grp = get_current_group(pp->p_config);
    if(gTofdS[grp].straighteningEnable)
	{
        gTofdS[grp].straighteningEnable = 0;
	}

    if(gTofdS[grp].removeLateralWaveEnable)
	{
        gTofdS[grp].removeLateralWaveEnable = 0;
	}
	return TRUE;
}

int MenuHandler_TofdRefLine(void* p_para)
{
	if(NULL != p_para)
	{
        int grp = get_current_group(pp->p_config);
		float para = *(float*)p_para;
		if(get_inspec_source (pp->p_config) == 0)
		{
			double rate =  GROUP_VAL_POS(0 , prf1) * get_area_scanresolution (pp->p_config) / 10000.0;
            gTofdS[grp].refLine  =  para * rate;
		}
		else
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
                gTofdS[grp].refLine  =  para;
			}
			else/* 英寸 */
			{
                gTofdS[grp].refLine  =  para / 0.03937;
			}
		}
		gtk_widget_queue_draw (pp->vboxtable);
	}
	return TRUE;
}

int MenuHandler_TofdPcs(void* p_para)
{
	if(NULL != p_para)
	{
		float para = *(float*)p_para;
		SetPCS(get_current_group(pp->p_config) ,para);
		gtk_widget_queue_draw (pp->vboxtable);
	}
	return TRUE;
}

void SetDefaultPCS(int grp)
{
	double _nThickness = GROUP_VAL_POS(grp , part.Thickness)/ 1000.0  ;
	double _nAngle     = LAW_VAL_POS(grp , Angle_min) * G_PI / 18000.0;
	_nThickness = _nThickness * 2 / 3 ;  // default PCS calculation use 2/3 thickness   !!!
	GROUP_VAL_POS(grp , field_distance[254]) = 2 * _nThickness * tan(_nAngle);

	double _nRef = fabs(GROUP_VAL_POS (grp, wedge.Ref_point)/1000.0) ;
	GROUP_VAL_POS(grp , field_distance[255]) = GROUP_VAL_POS(grp , field_distance[254])- 2 * _nRef;
}

void SetPCS(int grp ,double wedgeSeparation)
{
	GROUP_VAL_POS(grp , field_distance[255]) = wedgeSeparation;
	double _nRef = fabs(GROUP_VAL_POS (grp, wedge.Ref_point)/1000.0) ;
	GROUP_VAL_POS(grp , field_distance[254]) = wedgeSeparation + 2 * _nRef;
}

void SetDefaultTOFTCal()
{
    int i;
	for (i = 0 ; i < get_group_qty(pp->p_config); i++) {
        if(TOFD == group_get_rx_tx_mode(i)) {
            GROUP_VAL_POS(i, field_distance[252]) = 0;
        }
    }
}

int MenuHandler_TofdRefLineDepth(void* p_para)
{
    double len, depth;
	int grp = get_current_group(pp->p_config);
	double _fPcs  = GROUP_VAL_POS(grp , field_distance[254]) ;
    if(NULL != p_para) {
        float para = *(float*)p_para;
        if (UNIT_MM == get_unit(pp->p_config))
        {
            depth = para;
        } else { /* 英寸 */
            depth = para / 0.03937;
        }
        GROUP_VAL_POS(grp , field_distance[250]) = depth;
        if (GROUP_VAL_POS(grp , field_distance[252]) > 0.1) {
            depth = sqrt(depth*depth*4 + _fPcs*_fPcs);
            GROUP_VAL_POS(grp , field_distance[251]) =
                GROUP_VAL_POS(grp , u_reference) / 1000.0 -
                depth*100000.0/GROUP_VAL_POS(grp , velocity);
        }
        gtk_widget_queue_draw (pp->vboxtable);
    }
    return TRUE;
}

int MenuHandler_TofdDepthCal(void* p_para)
{
    double depth, _fPcs;
	int group = get_current_group(pp->p_config);
    if(TOFD == group_get_rx_tx_mode(group))//tofd
	{
        if (GROUP_VAL_POS(group , field_distance[252]) > 0.1) {
            GROUP_VAL_POS(group , field_distance[252]) = 0;
        } else {
            GROUP_VAL_POS(group , field_distance[252]) = 1.0;
            depth = GROUP_VAL_POS(group , field_distance[250]);
	        _fPcs  = GROUP_VAL_POS(group , field_distance[254]) ;
            depth = sqrt(depth*depth*4 + _fPcs*_fPcs);
            GROUP_VAL_POS(group , field_distance[251]) =
                GROUP_VAL_POS(group , u_reference) / 1000.0 -
                depth*100000.0/GROUP_VAL_POS(group , velocity);
        }
		gtk_widget_queue_draw (pp->vboxtable);
	} else {
			int response = dialogWarning(pp->window , getDictString(_STRING_Not_Tofd));
			if(!response)
			{

			}
	}
	
	return TRUE;
}

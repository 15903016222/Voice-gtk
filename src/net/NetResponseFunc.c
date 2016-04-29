/*
 * NetResponseFunc.c
 *
 *  Created on: 2013-5-2
 *      Author: wolflord
 */


//Reset the CPU.
//Parameter: None.
//Return   : None.

#include "NetResponseFunc.h"
#include "DopplerNetServer.h"
#include "Operation.h"
#include "../threads/threadDrawData.h"

#include "../drawui.h"
#include "../file_op.h"
#include "../drawfb.h"
#include "../callback.h"
#include "../string/_string.h"
#include "../ui/ui.h"
#include "../ui/authorization.h"
#include "../calculation/sizingcurve.h"
#include "../calculation/limit.h"
#include "../lzk/output_spi.h"
#include <gdk/gdk.h>
#include <glib/gprintf.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <glib.h>

int Response_CMD_NONE(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	g_debug("Response_CMD_NONE");
	return err;
}



int Response_CMD_RESET_ALL(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	FreezingFPGA(TRUE) ;
	//生成聚焦法则
	GenerateFocallawOfAllGroup(TRUE) ;
	// redraw screen
	gdk_threads_enter();
	RedrawAllUserInterface()  ;
	gdk_threads_leave();
	// send parameters to fpga
	ResetFpgaAll()  ;
	// restart scan
	RestartSCAN();
	FreezingFPGA(FALSE) ;
	return err;
}

int Response_CMD_RESET_ENCODER(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	FreezingFPGA(TRUE) ;
	RestartSCAN();
	FreezingFPGA(FALSE) ;
	return err;
}

int Response_CMD_SET_CURRENT_GROUP(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	FreezingFPGA(TRUE) ;
	int* _pData = (int*)Parameter  ;
	gdk_threads_enter();
	SetCurrentGroup(*_pData) ;
	gdk_threads_leave();
	FreezingFPGA(FALSE) ;
	return err;
}


int Response_CMD_ADD_GROUP(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	FreezingFPGA(TRUE) ;
	int* _pData = (int*)Parameter ;
	gdk_threads_enter();
	NetAddOneGroup(*_pData) ;
	gdk_threads_leave();
	FreezingFPGA(FALSE) ;
	return err;
}

int Response_CMD_DEL_GROUP(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	FreezingFPGA(TRUE) ;
	int* _pData = (int*)Parameter ;
	gdk_threads_enter();
	NetDeleteOneGroup(*_pData) ;
	gdk_threads_leave();
	FreezingFPGA(FALSE) ;
	return err;
}

int Response_CMD_SET_ENCODER_TYPE(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	FreezingFPGA(TRUE) ;
	int* _pData = (int*)Parameter ;
	gdk_threads_enter();
	NetSetEncoderType(*_pData) ;
	gdk_threads_leave();
	FreezingFPGA(FALSE) ;
	return err;
}


int Response_CMD_GET_CONFIG_SCAN(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	//CONFIG_SCAN* _pScanInfo = (CONFIG_SCAN*)Parameter  ;
	NetGetScanInformation(Parameter , ParamSize - 1)  ;
	return err;
}


int Response_CMD_SET_CONFIG_SCAN(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	FreezingFPGA(TRUE) ;
	gdk_threads_enter();
	NetSetScanInformation(Parameter ) ;
	gdk_threads_leave();
	FreezingFPGA(FALSE) ;
	return err;
}

int Response_CMD_GET_CONFIG_FOCALLAW(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	NetGetFocallawInfo(Parameter , ParamSize)  ;
	return err;
}

int Response_CMD_SET_CONFIG_FOCALLAW(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	FreezingFPGA(TRUE) ;
	int _nGroupId = get_current_group(pp->p_config) ;
	gdk_threads_enter();
	NetSetFocallawInfo(Parameter , _nGroupId)  ;
	gdk_threads_leave();
	FreezingFPGA(FALSE) ;
	return err;
}


int Response_CMD_GET_CONFIG_PROBE(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	int _nGroupId = get_current_group(pp->p_config);
	NetGetProbeInfo(Parameter , _nGroupId)  ;
	return err;
}

//****************************************

int Response_CMD_SET_CONFIG_PROBE(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	FreezingFPGA(TRUE) ;
	int _nGroupId = get_current_group(pp->p_config) ;
	gdk_threads_enter();
	NetSetProbeInfo(Parameter , _nGroupId)  ;
	gdk_threads_leave();
	FreezingFPGA(FALSE) ;
	return err;
}

//****************************************
int Response_CMD_GET_CONFIG_WEDGE(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	int _nGroupId = get_current_group(pp->p_config);
	NetGetWedgeInfo(Parameter , _nGroupId)  ;
	return err;
}

//****************************************
int Response_CMD_SET_CONFIG_WEDGE(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	FreezingFPGA(TRUE) ;
	int _nGroupId = get_current_group(pp->p_config) ;
	gdk_threads_enter();
	NetSetWedgeInfo(Parameter , _nGroupId)  ;
	gdk_threads_leave();
	FreezingFPGA(FALSE) ;
	return err;
}

//****************************************
int Response_CMD_GET_CONFIG_PART(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	int _nGroupId = get_current_group(pp->p_config);
	NetGetPartInfo(Parameter , _nGroupId)  ;
	return err;
}

//****************************************
int Response_CMD_SET_CONFIG_PART(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	FreezingFPGA(TRUE) ;
	int _nGroupId = get_current_group(pp->p_config) ;
	gdk_threads_enter();
	NetSetPartInfo(Parameter , _nGroupId)  ;
	gdk_threads_leave();
	FreezingFPGA(FALSE) ;
	return err;
}

//****************************************
int Response_CMD_GET_CONFIG_UT_GENERAL(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	int _nGroupId = get_current_group(pp->p_config);
	NetGetUtGeneral(Parameter , _nGroupId)  ;
	return err;
}

//****************************************
int Response_CMD_SET_CONFIG_UT_GENERAL(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	FreezingFPGA(TRUE) ;
	int _nGroupId = get_current_group(pp->p_config) ;
	gdk_threads_enter();
	NetSetUtGeneral(Parameter , _nGroupId)  ;
	gdk_threads_leave();
	FreezingFPGA(FALSE) ;
	return err;
}

//****************************************
int Response_CMD_GET_CONFIG_UT_PULSER(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	int _nGroupId = get_current_group(pp->p_config);
	NetGetUtPulser(Parameter , _nGroupId)  ;
	return err;
}

//****************************************
int Response_CMD_SET_CONFIG_UT_PULSER(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	FreezingFPGA(TRUE) ;
	int _nGroupId = get_current_group(pp->p_config) ;
	gdk_threads_enter();
	NetSetUtPulser(Parameter , _nGroupId)  ;
	gdk_threads_leave();
	FreezingFPGA(FALSE) ;
	return err;
}

//****************************************
int Response_CMD_GET_CONFIG_UT_RECEIVER(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	int _nGroupId = get_current_group(pp->p_config);
	NetGetUtReceive(Parameter , _nGroupId)  ;
	return err;
}

//****************************************
int Response_CMD_SET_CONFIG_UT_RECEIVER(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	FreezingFPGA(TRUE) ;
	int _nGroupId = get_current_group(pp->p_config) ;
	gdk_threads_enter();
	NetSetUtReceive(Parameter , _nGroupId)  ;
	gdk_threads_leave();
	FreezingFPGA(FALSE) ;
	return err;
}

//****************************************
int Response_CMD_GET_CONFIG_UT_BEAM(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;

	return err;
}

//****************************************
int Response_CMD_SET_CONFIG_UT_BEAM(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;

	return err;
}
//****************************************
int Response_CMD_GET_CONFIG_UT_ADVANCED(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	int _nGroupId = get_current_group(pp->p_config);
	NetGetUtAdvanced(Parameter , _nGroupId)  ;
	return err;
}

//****************************************
int Response_CMD_SET_CONFIG_UT_ADVANCED(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	FreezingFPGA(TRUE) ;
	int _nGroupId = get_current_group(pp->p_config) ;
	gdk_threads_enter();
	NetSetUtAdvanced(Parameter , _nGroupId)  ;
	gdk_threads_leave();
	FreezingFPGA(FALSE) ;
	return err;
}

//****************************************
int Response_CMD_GET_CONFIG_GATE_INFO(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;

	int _nGroupId = get_current_group(pp->p_config);
	NetGetGateInfo(Parameter , _nGroupId , ParamSize )  ;
	return err;
}

//****************************************
int Response_CMD_SET_CONFIG_GATE_INFO(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	FreezingFPGA(TRUE) ;
	int _nGroupId = get_current_group(pp->p_config) ;
	gdk_threads_enter();
	NetSetGateInfo(Parameter , _nGroupId)  ;
	gdk_threads_leave() ;
	FreezingFPGA(FALSE) ;
	return err;
}

//****************************************
int Response_CMD_GET_CONFIG_DISPLAY_INFO(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	NetGetDisplayInfo(Parameter )  ;
	return err;
}

//****************************************
int Response_CMD_SET_CONFIG_DISPLAY_INFO(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	FreezingFPGA(TRUE) ;
	gdk_threads_enter();
	NetSetDisplayInfo(Parameter)  ;
	gdk_threads_leave() ;
	FreezingFPGA(FALSE) ;
	return err;
}

//****************************************
//###################################
//###################################
//****************************************
int Response_CMD_GET_GROUP_BEAMDELAY(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	int _nGroupId = get_current_group(pp->p_config);
	NetGetGroupBeamDelay(Parameter , _nGroupId)  ;
	return err;
}

//****************************************
int Response_CMD_SET_GROUP_BEAMDELAY(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	FreezingFPGA(TRUE) ;
	int _nGroupId = get_current_group(pp->p_config) ;
	gdk_threads_enter();
	NetSetGroupBeamDelay(Parameter , _nGroupId)  ;
	gdk_threads_leave();
	FreezingFPGA(FALSE) ;
	return err;
}


int Response_CMD_GET_GROUP_GAINOFFSET(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	int _nGroupId = get_current_group(pp->p_config);
	NetGetGroupGainOffset(Parameter , _nGroupId)  ;
	return err;
}

//****************************************
int Response_CMD_SET_GROUP_GAINOFFSET(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	FreezingFPGA(TRUE) ;
	int _nGroupId = get_current_group(pp->p_config) ;
	gdk_threads_enter();
	NetSetGroupGainOffset(Parameter , _nGroupId)  ;
	gdk_threads_leave();
	FreezingFPGA(FALSE) ;
	return err;
}
//****************************************
int Response_CMD_GET_SIZINE_CURVE(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	int _nGroupId = get_current_group(pp->p_config);
	NetGetGroupSizingCurve(Parameter , _nGroupId)  ;
	return err;
}

int Response_CMD_SET_SIZINE_CURVE(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	FreezingFPGA(TRUE) ;
	int _nGroupId = get_current_group(pp->p_config) ;
	gdk_threads_enter();
	NetSetGroupSizingCurve(Parameter , _nGroupId)  ;
	gdk_threads_leave();
	FreezingFPGA(FALSE) ;
	return err;
}

//****************************************
int Response_CMD_ENABLE_DATA_TRANSFER(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	int* _pData = (int*)Parameter ;
	g_bNetTransferData = *_pData  ;
	return err;
}


int Response_CMD_GET_VAL_GROUP_QTY(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	int* _pData = (int*)Parameter  ;
	*_pData = get_group_qty(pp->p_config);
	return err;
}
int Response_CMD_GET_VAL_TOTAL_DATA_SIZE(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	int* _pData = (int*)Parameter  ;
	*_pData = getDataBlockSize();
	return err;
}
int Response_CMD_GET_VAL_GROUP_DATA_SIZE(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	int* _pData = (int*)Parameter  ;

	*_pData = GetGroupDataSize(get_current_group(pp->p_config));
	return err;
}
int Response_CMD_GET_VAL_GROUP_DATA_OFFSET(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	int i  , sum;
	if(ParamSize< 0 || ParamSize >= get_group_qty(pp->p_config))
		ParamSize = 0 ;

	int* _pData = (int*)Parameter ;
	for(i = 0 , sum = DMA_DATA_OFFSET ; i < ParamSize ; i++)
	{
		sum += (GROUP_VAL_POS(i, point_qty) + 32) * TMP(beam_qty[i]);
	}
	*_pData = sum ;
	return err;
}
int Response_CMD_GET_VAL_GROUP_BEAM_OFFSET(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	if(ParamSize< 0 || ParamSize >= get_group_qty(pp->p_config))
		ParamSize = 0 ;
	int* _pData = (int*)Parameter ;
	*_pData = GROUP_VAL_POS(ParamSize, point_qty) + 32 ;
	return err;
}
int Response_CMD_GET_VAL_GROUP_BEAM_LENGTH(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	if(ParamSize< 0 || ParamSize >= get_group_qty(pp->p_config))
		ParamSize = 0 ;
	int* _pData = (int*)Parameter ;
	*_pData = GROUP_VAL_POS(ParamSize, point_qty)  + 32;
	return err;
}
int Response_CMD_GET_VAL_GAIN(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
    int grp = get_current_group(pp->p_config) ;
	int* _pGain = (int*)Parameter ;
    *_pGain = group_get_gain(grp) / 10;
	return err;
}
int Response_CMD_SET_VAL_GAIN(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	int _nGroupId = get_current_group(pp->p_config) ;
	int* _pGain = (int*)Parameter ;

	gdk_threads_enter();
	NetSetGainValue(_nGroupId , *_pGain / 10.0)  ;
	gdk_threads_leave();
	return err;
}
int Response_CMD_GET_VAL_START(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	int _nGroupId = get_current_group(pp->p_config) ;
	int* _pGain = (int*)Parameter ;
    *_pGain = group_get_start(_nGroupId) ;
	return err;
}
int Response_CMD_SET_VAL_START(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	FreezingFPGA(TRUE) ;
	int _nGroupId = get_current_group(pp->p_config) ;
	int* _pStart = (int*)Parameter ;

	gdk_threads_enter();
	NetSetGroupValueStart(_nGroupId , *_pStart)  ;
	gdk_threads_leave();
	FreezingFPGA(FALSE) ;
	return err;
}
int Response_CMD_GET_VAL_RANGE(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	int _nGroupId = get_current_group(pp->p_config) ;
	int* _pGain = (int*)Parameter ;
    *_pGain = group_get_range(_nGroupId);
	return err;
}
int Response_CMD_SET_VAL_RANGE(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	FreezingFPGA(TRUE) ;
	int _nGroupId = get_current_group(pp->p_config) ;
	int* _pRange = (int*)Parameter ;

	gdk_threads_enter();
	NetSetGroupValueRange(_nGroupId , *_pRange)  ;
	gdk_threads_leave();
	FreezingFPGA(FALSE) ;
	return err;
}
int Response_CMD_GET_VAL_POINT_QTY(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	int _nGroupId = get_current_group(pp->p_config) ;
	int* _pPointQty = (int*)Parameter ;
	*_pPointQty = GROUP_VAL_POS(_nGroupId , point_qty) ;
	return err;
}
int Response_CMD_SET_VAL_POINT_QTY(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	FreezingFPGA(TRUE) ;
	int _nGroupId = get_current_group(pp->p_config) ;
	int* _pPointQty = (int*)Parameter ;

	gdk_threads_enter();
	GROUP_VAL_POS(_nGroupId , point_qty_pos)  =  4 ;
	NetSetGroupValuePointQty(_nGroupId , *_pPointQty)  ;
	gdk_threads_leave();
	FreezingFPGA(FALSE) ;
	return err;
}
int Response_CMD_GET_VAL_WEDGEDELAY(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	int _nGroupId = get_current_group(pp->p_config) ;
	int* _pDelay = (int*)Parameter ;
	*_pDelay = GROUP_VAL_POS(_nGroupId , wedge_delay) ;
	return err;
}
int Response_CMD_SET_VAL_WEDGEDELAY(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	FreezingFPGA(TRUE) ;
	int _nGroupId = get_current_group(pp->p_config) ;
	int* _pPointQty = (int*)Parameter ;

	gdk_threads_enter();
	NetSetGroupValueWedgeDelay(_nGroupId , *_pPointQty)  ;
	gdk_threads_leave();
	FreezingFPGA(FALSE) ;
	return err;
}
int Response_CMD_GET_VAL_VELOCITY(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	int _nGroupId = get_current_group(pp->p_config) ;
	int* _pVelocity = (int*)Parameter ;
	*_pVelocity = GROUP_VAL_POS(_nGroupId , velocity)/100;
	return err;
}
int Response_CMD_SET_VAL_VELOCITY(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	FreezingFPGA(TRUE) ;
	int _nGroupId = get_current_group(pp->p_config) ;
	int* _pVelocity = (int*)Parameter ;

	gdk_threads_enter();
	NetSetGroupValueVelocity(_nGroupId , *_pVelocity)  ;
	gdk_threads_leave();
	FreezingFPGA(FALSE) ;
	return err;
}
int Response_CMD_GET_VAL_PRF(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	int* _pPRF = (int*)Parameter ;
	*_pPRF = GROUP_VAL_POS(0 , prf1) / 10;
	return err;
}
int Response_CMD_SET_VAL_PRF(USLONG Command, USLONG ParamSize, char* Parameter)
{
	int err = 0;
	FreezingFPGA(TRUE) ;
	int* _pPRF = (int*)Parameter ;
	GROUP_VAL_POS(0 , prf_pos1) = 3 ;
	gdk_threads_enter();
	NetSetGroupValuePRF(0 , *_pPRF)  ;
	gdk_threads_leave();
	FreezingFPGA(FALSE) ;
	return err;
}

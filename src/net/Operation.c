/*
 * Operation.c
 *
 *  Created on: 2013-5-3
 *      Author: wolflord
 */
#include "Operation.h"
#include "DopplerNetServer.h"

#include "../main.h"
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
#include <math.h>

void GenerateFocallawOneGroup(int nGroupId_ , int bRefreshCalibration)
{
	generate_focallaw (nGroupId_);

	if(bRefreshCalibration)
	{
		GROUP_VAL_POS(nGroupId_ , WedgeDelayCalibrated) = 0 ;
		GROUP_VAL_POS(nGroupId_ , SensationCalibrated)  = 0 ;
		GROUP_VAL_POS(nGroupId_ , VelocityCalibrated)   = 0 ;
	}

	if(!GROUP_VAL_POS(nGroupId_ , WedgeDelayCalibrated))
		RefreshBeamDelay(nGroupId_);
	if(!GROUP_VAL_POS(nGroupId_ , SensationCalibrated))
		RefreshGainOffset(nGroupId_);
}

void GenerateFocallawOfAllGroup(int bRefreshCalibration)
{
	int i;
	for(i = 0 ; i < get_group_qty(pp->p_config) ; ++i)
	{
		GenerateFocallawOneGroup(i , bRefreshCalibration) ;
	}
}


void RedrawAllUserInterface()
{

	if(!IsDisplayModalAvailable(get_display_pos(pp->p_config)))
	{
		set_display_pos(pp->p_config, A_SCAN) ;
	}

	draw_area_all();
	draw_field_name ();
	gtk_widget_queue_draw (pp->status_area) ;
	gtk_widget_queue_draw (pp->drawing_area) ;

	int _nEncodeSource = get_inspec_source (pp->p_config);
	if(_nEncodeSource)
		update_widget_bg(pp->event[18], 19);
	else
		update_widget_bg(pp->event[18], 18);

	int _nGroupId = get_current_group(pp->p_config);
	if(GROUP_VAL_POS(_nGroupId , group_mode) == PA_SCAN)
		gtk_widget_set_sensitive(pp->menuitem[6],TRUE);
	else
		gtk_widget_set_sensitive(pp->menuitem[6],FALSE);

	RefreshPrfMark()  ;
	RefreshGainMark(_nGroupId);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void ResetFpgaAll()
{
	output_init(GetChannelEnable()) ;
	MultiGroupSendFocalSpi();
	MultiGroupSendGroupSpi();
}

void RestartSCAN()
{
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;
	pp->ScanStart = 1       ;
}

void MainMenuRefresh(int nId_)
{
	if(GROUP_VAL_POS(nId_ , group_mode) == PA_SCAN) /*group mode 选择UT,UT1,UT2时，focal law 不可用*/
	{
		gtk_widget_set_sensitive(pp->menuitem[6],TRUE);
	}
	else
	{
		gtk_widget_set_sensitive(pp->menuitem[6],FALSE);
		if(pp->pos == 6)
		{
			pp->pos = 1 ;
			gtk_menu_item_set_label(GTK_MENU_ITEM (pp->menuitem_main), pp->con0_p[1]);
			draw_menu2(0);
		}
	}
}

void SetCurrentGroup(int nId_)
{
	// no need to refresh
	int _nGroupQty = get_group_qty(pp->p_config);
	if(nId_ >= _nGroupQty)  return ;
    int _nGroupId = get_current_group(pp->p_config) ;
    if(nId_ == _nGroupId)  return ;
    //##############################
    pp->p_config->groupId = nId_ ;
    //RefreshGainMark(nId_)  ;
    //MainMenuRefresh(nId_)  ;
    //###refresh main screen
    RedrawAllUserInterface() ;
    //pp->pos_pos = MENU3_STOP;
    //draw_menu3(0, NULL);
    // reset calibration status
	pp->ctype_pos = 1;
	pp->cmode_pos = 0;
}


void NetAddOneGroup(int nGroupId_)
{
	int _nGroupQty;
	_nGroupQty = get_group_qty(pp->p_config);
	if(nGroupId_ < _nGroupQty && nGroupId_ >= 0)
	{
		grpcpy (pp->p_config, _nGroupQty, nGroupId_);
	}
	else
	{
	    ClearOneGroupParameters(_nGroupQty) ;
		SettingGroupConfigure (_nGroupQty) ;
	}

	set_group_qty (pp->p_config, _nGroupQty + 1);
	if(CheckGroupConfigureAvailable())
	{
		set_group_qty (pp->p_config, _nGroupQty);
		return ;
	}
	pp->p_config->groupId = _nGroupQty ;

	GenerateFocallawOneGroup(_nGroupQty , TRUE) ;
	InitSizingCurveParameters(_nGroupQty) ;
	ResetFpgaAll() ;

	RedrawAllUserInterface() ;
	RestartSCAN()  ;
}


void NetDeleteOneGroup(int nGroupId_)
{
	int _nGroupQty = get_group_qty(pp->p_config);
	if (_nGroupQty <=  1) return ;

	if(nGroupId_ >= _nGroupQty || nGroupId_ < 0)
	{
		nGroupId_ = _nGroupQty - 1 ;
	}

	int i ;
	for(i = nGroupId_ ; i < _nGroupQty ; i++)
	{
		grpcpy (pp->p_config, i, i+1);
	}

	set_group_qty (pp->p_config, _nGroupQty - 1);
	pp->p_config->groupId = _nGroupQty - 2 ;

	if(_nGroupQty == 2)
	{
		set_display_group (pp->p_config, DISPLAY_CURRENT_GROUP);
	}

	ResetFpgaAll() ;
    RedrawAllUserInterface() ;
    RestartSCAN()  ;
}

void NetGetScanInformation(void* pData_ , int nEncoderId_)
{
	if(nEncoderId_ != 0 && nEncoderId_ != 1)
		nEncoderId_ = 0 ;

	CONFIG_SCAN* _pScanInfo = (CONFIG_SCAN*)pData_  ;
	int _nCurrentEcoderType ;
	_pScanInfo->eEncoderType =  nEncoderId_ + 1 ;
	_nCurrentEcoderType      =  get_enc_type (pp->p_config,  nEncoderId_);
	_pScanInfo->eEncoderMode =  _nCurrentEcoderType & 0x7   ;
	_pScanInfo->ePolarity    =  (_nCurrentEcoderType & 0x8) ? 1:0    ;
	//**************************************************************
	_pScanInfo->fResulotion  = get_enc_resolution(pp->p_config , nEncoderId_) / 1000.0 ;
	_pScanInfo->fOrigin      =  get_enc_origin(pp->p_config , nEncoderId_) / 1000.0 ;
	_pScanInfo->fMaxScanSpeed = get_inspec_speed(pp->p_config) / 1000.0 ;

	if(nEncoderId_)
	{
		_pScanInfo->fScanStart  =   get_area_indexstart (pp->p_config) / 1000.0 ;
		_pScanInfo->fScanStop   =   get_area_indexend (pp->p_config) / 1000.0 ;
		_pScanInfo->fScanStep   =   get_area_indexresolution (pp->p_config) / 1000.0 ;
	}
	else
	{
		_pScanInfo->fScanStart  =   get_area_scanstart (pp->p_config) / 1000.0 ;
		_pScanInfo->fScanStop   =   get_area_scanend (pp->p_config) / 1000.0   ;
		_pScanInfo->fScanStep   =   get_area_scanresolution (pp->p_config) / 1000.0 ;
	}
}

void NetSetScanInformation(void* pData_ )
{
	CONFIG_SCAN* _pScanInfo = (CONFIG_SCAN*)pData_  ;
	int _nEncoderId = _pScanInfo->eEncoderType ;
	if(_nEncoderId < 0 || _nEncoderId > 2)
		return  ;

	int _nCurrentEcoderType = 0;
	if(_pScanInfo->ePolarity)
		_nCurrentEcoderType = 8;
	_nCurrentEcoderType = _nCurrentEcoderType | _pScanInfo->eEncoderMode ;

	if(_nEncoderId == setup_ENCODER_TYPE_ENCODER_1)
	{
		set_area_scanstart (pp->p_config  , _pScanInfo->fScanStart * 1000 ) ;
		set_area_scanend   (pp->p_config  , _pScanInfo->fScanStop  * 1000 ) ;
		set_area_scanresolution (pp->p_config  , _pScanInfo->fScanStep * 1000 ) ;

		set_enc_resolution(pp->p_config , _pScanInfo->fResulotion * 1000  , 0) ;
		set_enc_origin    (pp->p_config , _pScanInfo->fOrigin * 1000      , 0) ;
		set_enc_type (pp->p_config , _nCurrentEcoderType , 0) ;
	}

	if (_nEncoderId == setup_ENCODER_TYPE_ENCODER_2)
	{
		set_area_indexstart (pp->p_config  , _pScanInfo->fScanStart * 1000 ) ;
		set_area_indexend   (pp->p_config  , _pScanInfo->fScanStop  * 1000 ) ;
		set_area_indexresolution (pp->p_config  , _pScanInfo->fScanStep * 1000 ) ;

		set_enc_resolution(pp->p_config , _pScanInfo->fResulotion * 1000  , 1) ;
		set_enc_origin    (pp->p_config , _pScanInfo->fOrigin * 1000      , 1) ;
		set_enc_type (pp->p_config , _nCurrentEcoderType , 1) ;

	}


	//****************************************************
	//set_inspec_speed  (pp->p_config , _pScanInfo->fMaxScanSpeed * 1000) ;

	RedrawAllUserInterface();
	RestartSCAN() ;

}

void NetSetEncoderType(int nType_)
{
	unsigned char _nCurrentEcoderType  ;
	if (nType_ == get_inspec_source (pp->p_config))
	{
		return ;
	}

	set_inspec_source (pp->p_config, nType_);
	if(nType_ == setup_ENCODER_TYPE_TIMER)
	{
		output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_ENCODE_X , 0 , 0);
		output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_ENCODE_Y , 0 , 0);
	}
	else if(nType_ == setup_ENCODER_TYPE_ENCODER_1)
	{
		_nCurrentEcoderType  = get_enc_type (pp->p_config,   0);
		output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_ENCODE_X , _nCurrentEcoderType , 0);
	}
	else if(nType_ == setup_ENCODER_TYPE_ENCODER_2)
	{
		_nCurrentEcoderType  = get_enc_type (pp->p_config,   1);
		output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_ENCODE_Y , _nCurrentEcoderType , 0);
	}
	output_write_one_reg_to_spi(0,OUTPUT_OTHER_COMMAND_ENCODE_Y);


	RedrawAllUserInterface();
	RestartSCAN() ;

}


void NetGetFocallawInfo(void* pData_ , int nGroupId_ )
{

	if(nGroupId_ < 0 || nGroupId_ >= get_group_qty(pp->p_config))
		return;

	CONFIG_FOCALLAW* _pLaw   = (CONFIG_FOCALLAW*)pData_  ;

	_pLaw->eLawType          = LAW_VAL_POS(nGroupId_ , Focal_type)          ;
	_pLaw->eFocalType        = LAW_VAL_POS(nGroupId_ , Focal_point_type)    ;

	_pLaw->uiAngleStart      = LAW_VAL_POS(nGroupId_ , Angle_min) / 10      ;
	_pLaw->uiAngleStop       = LAW_VAL_POS(nGroupId_ , Angle_max) / 10      ;
	_pLaw->uiAngleStep       = LAW_VAL_POS(nGroupId_ , Angle_step) / 10     ;

	_pLaw->uiElem_qty        = LAW_VAL_POS(nGroupId_ , Elem_qty)            ;
	_pLaw->uiFirst_tx_elem   = LAW_VAL_POS(nGroupId_ , First_tx_elem)       ;
	_pLaw->uiLast_tx_elem    = LAW_VAL_POS(nGroupId_ , Last_tx_elem)        ;
	_pLaw->uiFirst_rx_elem   = LAW_VAL_POS(nGroupId_ , First_rx_elem)       ;
	_pLaw->uiLast_rx_elem    = LAW_VAL_POS(nGroupId_ , Last_tx_elem)        ;
	_pLaw->uiElem_step       = LAW_VAL_POS(nGroupId_ , Elem_step)           ;

	_pLaw->fPositionStart    = LAW_VAL_POS(nGroupId_ , Position_start) / 1000.0 ;

	_pLaw->fOffsetStart      = LAW_VAL_POS(nGroupId_ , Offset_start)/1000.0 ;
	_pLaw->fOffsetEnd        = LAW_VAL_POS(nGroupId_ , Offset_end  )/1000.0 ;
	_pLaw->fDepthStart       = LAW_VAL_POS(nGroupId_ , Depth_start )/1000.0 ;
	_pLaw->fDepthEnd         = LAW_VAL_POS(nGroupId_ , Depth_end   )/1000.0 ;

}


void NetSetFocallawInfo(void* pData_ , int nGroupId_ )
{
	if(nGroupId_ < 0 || nGroupId_ >= get_group_qty(pp->p_config))
		return;

	CONFIG_FOCALLAW* _pLaw   = (CONFIG_FOCALLAW*)pData_  ;

	LAW_VAL_POS(nGroupId_ , Focal_type)         = _pLaw->eLawType     ;
	LAW_VAL_POS(nGroupId_ , Focal_point_type)   = _pLaw->eFocalType   ;

	LAW_VAL_POS(nGroupId_ , Angle_min)  = _pLaw->uiAngleStart * 10    ;
	LAW_VAL_POS(nGroupId_ , Angle_max)  = _pLaw->uiAngleStop  * 10    ;
	LAW_VAL_POS(nGroupId_ , Angle_step) = _pLaw->uiAngleStep  * 10    ;

	LAW_VAL_POS(nGroupId_ , Elem_qty)        = _pLaw->uiElem_qty           ;
	LAW_VAL_POS(nGroupId_ , First_tx_elem)   = _pLaw->uiFirst_tx_elem      ;
	LAW_VAL_POS(nGroupId_ , Last_tx_elem)    = _pLaw->uiLast_tx_elem       ;
	LAW_VAL_POS(nGroupId_ , First_rx_elem)   = _pLaw->uiFirst_rx_elem      ;
	LAW_VAL_POS(nGroupId_ , Last_tx_elem)    = _pLaw->uiLast_rx_elem       ;
	LAW_VAL_POS(nGroupId_ , Elem_step)       = _pLaw->uiElem_step          ;

	LAW_VAL_POS(nGroupId_ , Position_start)  = _pLaw->fPositionStart  * 1000 ;

	LAW_VAL_POS(nGroupId_ , Offset_start)    = _pLaw->fOffsetStart  *   1000 ;
	LAW_VAL_POS(nGroupId_ , Offset_end  )    = _pLaw->fOffsetEnd    *   1000 ;
	LAW_VAL_POS(nGroupId_ , Depth_start )    = _pLaw->fDepthStart   *   1000 ;
	LAW_VAL_POS(nGroupId_ , Depth_end   )    = _pLaw->fDepthEnd     *   1000 ;

	GenerateFocallawOneGroup(nGroupId_ ,  TRUE) ;
	ResetFpgaAll() ;
	RedrawAllUserInterface();
	RestartSCAN() ;

}


void NetGetProbeInfo(char* pData_ , int nGroupId_)
{
	if(nGroupId_ < 0 || nGroupId_ >= get_group_qty(pp->p_config))
		return;

	CONFIG_PROBE_INFO* _pInfo = (CONFIG_PROBE_INFO*)pData_ ;
	PROBE* _pProbe = &GROUP_VAL_POS(nGroupId_ , probe) ;
	int _nGroupMode = GROUP_VAL_POS(nGroupId_ , group_mode);
	if (_nGroupMode == PA_SCAN ||_nGroupMode == UT_SCAN)
	{

		_pInfo->uiElemQty  = _pProbe->Elem_qty  ;
		_pInfo->fReferencePoint  = _pProbe->Reference_Point / 1000.0  ;
	}
	else
	{
		_pInfo->uiElemQty  = 1 ;
		_pInfo->fReferencePoint  = 0.0 ;

	}
	_pInfo->fFrequency = _pProbe->Frequency /1000.0 ;
	_pInfo->fPitch     = _pProbe->Pitch / 1000.0     ;

	memcpy((void*)_pInfo->strMode  , _pProbe->Model , 20) ;
	memcpy((void*)_pInfo->strSerial  , _pProbe->Serial , 20) ;
}

void NetSetProbeInfo(char* pData_ , int nGroupId_)
{
	if(nGroupId_ < 0 || nGroupId_ >= get_group_qty(pp->p_config))
		return;
	CONFIG_PROBE_INFO* _pInfo = (CONFIG_PROBE_INFO*)pData_ ;
	PROBE* _pProbe = &GROUP_VAL_POS(nGroupId_ , probe) ;
	int _nGroupMode = GROUP_VAL_POS(nGroupId_ , group_mode);
	if (_nGroupMode == PA_SCAN ||_nGroupMode == UT_SCAN)
	{
		_pProbe->Elem_qty = _pInfo->uiElemQty  ;
		_pProbe->Reference_Point = _pInfo->fReferencePoint * 1000.0  ;
	}
	else
	{
		_pProbe->Elem_qty   = 1 ;
		_pProbe->Reference_Point = 0.0 ;
	}
	_pProbe->Frequency = _pInfo->fFrequency * 1000.0 ;
	_pProbe->Pitch     = _pInfo->fPitch     * 1000.0 ;

	memcpy(_pProbe->Model ,  _pInfo->strMode  , 20) ;
	memcpy(_pProbe->Serial,  _pInfo->strSerial , 20) ;

	GenerateFocallawOneGroup(nGroupId_ ,  TRUE) ;
	ResetFpgaAll() ;
	RedrawAllUserInterface();
	RestartSCAN() ;
}

void NetGetWedgeInfo(char* pData_ , int nGroupId_)
{
	if(nGroupId_ < 0 || nGroupId_ >= get_group_qty(pp->p_config))
		return;
	CONFIG_WEDGE_INFO* _pInfo = (CONFIG_WEDGE_INFO*)pData_ ;

	WEDGE* _pWedge = &GROUP_VAL_POS(nGroupId_ , wedge) ;
	int _nGroupMode = GROUP_VAL_POS(nGroupId_ , group_mode);
	if (_nGroupMode == PA_SCAN ||_nGroupMode == UT_SCAN)
	{
		_pInfo->eType  = setup_WEDGE_TYPE_PA  ;
		_pInfo->fAngle = _pWedge->Angle / 10.0  ;
		_pInfo->fHeight = _pWedge->Height / 1000.0  ;
		_pInfo->fProbeDelay = 0;
		_pInfo->fRefPoint = _pWedge->Primary_offset / 1000.0  ;
		_pInfo->fVelocity = _pWedge->Velocity_PA / 1000.0  ;
		_pInfo->eOrientation = _pWedge->Orientation  ;
	}
	else
	{
		_pInfo->eType  = setup_WEDGE_TYPE_UT  ;
		_pInfo->fAngle = _pWedge->Angle / 10.0  ;
		_pInfo->fProbeDelay = _pWedge->Probe_delay / 1000.0  ;
		_pInfo->fRefPoint = _pWedge->Ref_point / 1000.0  ;
		_pInfo->eOrientation = 1 ;
	}

	memcpy((void*)_pInfo->strMode  , _pWedge->Model , 20) ;
	memcpy((void*)_pInfo->strSerial  , _pWedge->Serial , 20) ;

}

void NetSetWedgeInfo(char* pData_ , int nGroupId_)
{
	if(nGroupId_ < 0 || nGroupId_ >= get_group_qty(pp->p_config))
		return;
	CONFIG_WEDGE_INFO* _pInfo = (CONFIG_WEDGE_INFO*)pData_ ;

	WEDGE* _pWedge = &GROUP_VAL_POS(nGroupId_ , wedge) ;
	int _nGroupMode = GROUP_VAL_POS(nGroupId_ , group_mode);

	if (_nGroupMode == PA_SCAN ||_nGroupMode == UT_SCAN)
	{
		if(_pInfo->eType  ==  setup_WEDGE_TYPE_UT)  return  ;
	}

	if (_nGroupMode == PA_SCAN ||_nGroupMode == UT_SCAN)
	{
		_pWedge->Angle = _pInfo->fAngle * 10.0  ;
		_pWedge->Height = _pInfo->fHeight * 1000.0  ;
		_pWedge->Primary_offset = _pInfo->fRefPoint * 1000.0  ;
		_pWedge->Velocity_PA = _pInfo->fVelocity * 1000.0  ;
		_pWedge->Orientation =_pInfo->eOrientation   ;
	}
	else
	{
		_pWedge->Angle = _pInfo->fAngle * 10.0  ;
		_pWedge->Probe_delay = _pInfo->fProbeDelay * 1000.0  ;
		_pWedge->Ref_point = _pInfo->fRefPoint * 1000.0  ;
		_pInfo->eOrientation = 1 ;
	}

	memcpy( _pWedge->Model , _pInfo->strMode , 20) ;
	memcpy( _pWedge->Serial , _pInfo->strSerial  , 20) ;

	GenerateFocallawOneGroup(nGroupId_ ,  TRUE) ;
	ResetFpgaAll() ;
	RedrawAllUserInterface();
	RestartSCAN() ;
}


void NetGetPartInfo(void* pData_ , int nGroupId_)
{
	if(nGroupId_ < 0 || nGroupId_ >= get_group_qty(pp->p_config))
		return;

	CONFIG_PART_INFO* _pPartR= (CONFIG_PART_INFO*)pData_  ;
	st_PART* _pPartL = &GROUP_VAL_POS(nGroupId_ , part)   ;
	_pPartR->fThickness  = _pPartL->Thickness /1000.0     ;
	_pPartR->eMaterial   = _pPartL->Material_pos          ;
	_pPartR->fScanOffset  = GROUP_VAL_POS(nGroupId_ , scan_offset ) /10.0    ;
	_pPartR->fIndexOffset  = GROUP_VAL_POS(nGroupId_ , index_offset ) /10.0    ;
	_pPartR->eSkew        = GROUP_VAL_POS(nGroupId_ , skew_pos)     ;

}

void NetSetPartInfo(void* pData_ , int nGroupId_)
{
	if(nGroupId_ < 0 || nGroupId_ >= get_group_qty(pp->p_config))
		return;

	CONFIG_PART_INFO* _pPartR= (CONFIG_PART_INFO*)pData_  ;
	st_PART* _pPartL = &GROUP_VAL_POS(nGroupId_ , part)   ;
	_pPartL->Thickness = _pPartR->fThickness * 1000.0     ;
	//###################################################
	_pPartL->Material_pos  = _pPartR->eMaterial           ;
	//######################################################
	GROUP_VAL_POS(nGroupId_ , scan_offset )  = _pPartR->fScanOffset * 10.0    ;
	GROUP_VAL_POS(nGroupId_ , index_offset ) = _pPartR->fIndexOffset * 10.0    ;
	GROUP_VAL_POS(nGroupId_ , skew_pos)      = _pPartR->eSkew       ;
	GROUP_VAL_POS(nGroupId_ , skew)  = 9000 * _pPartR->eSkew  ;

	RedrawAllUserInterface();
	RestartSCAN() ;

}


void NetGetUtGeneral(void* pData_ , int grp)
{
    if(grp < 0 || grp >= get_group_qty(pp->p_config))
		return;
	CONFIG_UT_GENERAL* _pInfo = (CONFIG_UT_GENERAL*)pData_ ;

    _pInfo->fGain  = group_get_gain(grp) / 100.0;
    _pInfo->uiStart= group_get_start(grp);
    _pInfo->uiRange= GROUP_VAL_POS(grp , range);

    _pInfo->uiWedgeDelay = GROUP_VAL_POS(grp , wedge_delay) ;
    _pInfo->fVelocity   = GROUP_VAL_POS(grp , velocity) / 100.0 ;


}

void NetSetUtGeneral(void* pData_ , int grp)
{
    if(grp < 0 || grp >= get_group_qty(pp->p_config))
		return;
	CONFIG_UT_GENERAL* _pInfo = (CONFIG_UT_GENERAL*)pData_ ;

    group_set_gain(grp, (gshort)(_pInfo->fGain*100));
    group_set_start(grp, _pInfo->uiStart);
    GROUP_VAL_POS(grp , range)= _pInfo->uiRange  ;

    GROUP_VAL_POS(grp , wedge_delay) = _pInfo->uiWedgeDelay ;
    GROUP_VAL_POS(grp , velocity)    = _pInfo->fVelocity * 100 ;

    GenerateFocallawOneGroup(grp ,  TRUE) ;
	ResetFpgaAll() ;
	RedrawAllUserInterface();
	RefreshGainMark(get_current_group(pp->p_config)) ;
	RestartSCAN() ;
}

void NetGetUtPulser(void* pData_ , int nGroupId_)
{
	if(nGroupId_ < 0 || nGroupId_ >= get_group_qty(pp->p_config))
		return;
	CONFIG_UT_PULSER* _pInfo = (CONFIG_UT_PULSER*)pData_ ;

	_pInfo->uiPulserFir     = GROUP_VAL_POS(nGroupId_ , pulser1) ;
    _pInfo->eTRMode         = group_get_rx_tx_mode(nGroupId_);
	_pInfo->fFequency      = GROUP_VAL_POS(nGroupId_ , frequency1)/1000.0;

	_pInfo->eVoltage        = get_voltage(pp->p_config, nGroupId_) ;
	_pInfo->uiPRF           = GROUP_VAL_POS(0 , prf1) / 10 ;
	_pInfo->uiPulserWidth    = GROUP_VAL_POS(nGroupId_ , pulser_width1)/100 ;

}



void NetSetUtPulser(void* pData_ , int nGroupId_)
{
	if(nGroupId_ < 0 || nGroupId_ >= get_group_qty(pp->p_config))
		return;
	CONFIG_UT_PULSER* _pInfo = (CONFIG_UT_PULSER*)pData_ ;

	GROUP_VAL_POS(nGroupId_ , pulser1)    = _pInfo->uiPulserFir  ;
    group_set_rx_tx_mode(nGroupId_, _pInfo->eTRMode);
	GROUP_VAL_POS(nGroupId_ , frequency1) = _pInfo->fFequency * 1000;

	set_voltage(pp->p_config, nGroupId_ , _pInfo->eVoltage)  ;
	GROUP_VAL_POS(0 , prf1)  = _pInfo->uiPRF * 10 ;
	GROUP_VAL_POS(nGroupId_ , pulser_width1)  = _pInfo->uiPulserWidth * 100 ;

	GenerateFocallawOneGroup(nGroupId_ ,  TRUE) ;
	ResetFpgaAll() ;
	RedrawAllUserInterface();
	RestartSCAN() ;

}

void NetGetUtReceive(void* pData_ , int nGroupId_)
{
	if(nGroupId_ < 0 || nGroupId_ >= get_group_qty(pp->p_config))
		return;
	CONFIG_UT_RECEIVER* _pInfo = (CONFIG_UT_RECEIVER*)pData_ ;

	_pInfo->uiReceiveFir   = GROUP_VAL_POS(nGroupId_ , receiver1) ;
	_pInfo->eFilterMode    = GROUP_VAL_POS(nGroupId_ , filter_pos1)   ;
	_pInfo->eRectifier     = GROUP_VAL_POS(nGroupId_ , rectifier1);

	_pInfo->bVideoFilter   = get_group_val (get_group_by_id(pp->p_config, nGroupId_), GROUP_VIDEO_FILTER)  ;
	_pInfo->eAverage       = GROUP_VAL_POS(nGroupId_ , averaging1) ;
	_pInfo->uiReject       = get_reject (pp->p_config) ;

}

void NetSetUtReceive(void* pData_ , int nGroupId_)
{
	if(nGroupId_ < 0 || nGroupId_ >= get_group_qty(pp->p_config))
		return;
	CONFIG_UT_RECEIVER* _pInfo = (CONFIG_UT_RECEIVER*)pData_ ;

	GROUP_VAL_POS(nGroupId_ , receiver1)    = _pInfo->uiReceiveFir  ;
	GROUP_VAL_POS(nGroupId_ , filter_pos1) = _pInfo->eFilterMode   ;
	GROUP_VAL_POS(nGroupId_ , rectifier1) = _pInfo->eRectifier;

	set_group_val (get_group_by_id(pp->p_config, nGroupId_), GROUP_VIDEO_FILTER ,_pInfo->bVideoFilter )  ;
	GROUP_VAL_POS(nGroupId_, averaging1)  = _pInfo->eAverage  ;
	set_reject (pp->p_config , _pInfo->uiReject) ;

	GenerateFocallawOneGroup(nGroupId_ ,  TRUE) ;
	ResetFpgaAll() ;
	RedrawAllUserInterface();
	RestartSCAN() ;
}


void NetGetGroupBeamDelay(void* pData_ , int nGroupId_)
{
	if(nGroupId_ < 0 || nGroupId_ >= get_group_qty(pp->p_config))
		return;
	CONFIG_GROUP_BEAMDELAY* _pInfoT = (CONFIG_GROUP_BEAMDELAY*)pData_ ;
	unsigned int* _pInfo = _pInfoT->auiBeamDelay  ;
	unsigned int* pBeamDelay  = GROUP_VAL_POS(nGroupId_ , beam_delay) ;
	memcpy((void*) _pInfo , (void*) pBeamDelay  , sizeof(unsigned int) * setup_MAX_GROUP_LAW_QTY ) ;
}


void NetSetGroupBeamDelay(void* pData_ , int nGroupId_)
{
	if(nGroupId_ < 0 || nGroupId_ >= get_group_qty(pp->p_config))
		return;
	CONFIG_GROUP_BEAMDELAY* _pInfoT = (CONFIG_GROUP_BEAMDELAY*)pData_ ;
	unsigned int* _pInfo = _pInfoT->auiBeamDelay  ;

	unsigned int* pBeamDelay  = GROUP_VAL_POS(nGroupId_ , beam_delay) ;
	memcpy((void*) pBeamDelay , (void*) _pInfo  , sizeof(unsigned int) * setup_MAX_GROUP_LAW_QTY ) ;

	MultiGroupSendFocalSpi();
	MultiGroupSendGroupSpi();

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void NetGetGroupGainOffset(void* pData_ , int nGroupId_)
{
	if(nGroupId_ < 0 || nGroupId_ >= get_group_qty(pp->p_config))
		return;
	CONFIG_GROUP_GAINOFFSET* _pInfoTmp = (CONFIG_GROUP_GAINOFFSET*)pData_ ;
	double* _pInfo        = _pInfoTmp->afGainOffset  ;
	unsigned short* _pGainOffset  = GROUP_VAL_POS(nGroupId_ , gain_offset)  ;
	int i  ;
	for(i = 0 ; i < setup_MAX_GROUP_LAW_QTY ; i++)
	{
		_pInfo[i] = _pGainOffset[i] / 10.0 ;
	}
}




void NetSetGroupGainOffset(void* pData_ , int nGroupId_)
{
	if(nGroupId_ < 0 || nGroupId_ >= get_group_qty(pp->p_config))
		return;
	CONFIG_GROUP_GAINOFFSET* _pInfoTmp = (CONFIG_GROUP_GAINOFFSET*)pData_ ;
	double* _pInfo        = _pInfoTmp->afGainOffset  ;
	unsigned short* _pGainOffset  = GROUP_VAL_POS(nGroupId_ , gain_offset)  ;
	int i  ;
	for(i = 0 ; i < setup_MAX_GROUP_LAW_QTY ; i++)
	{
		_pGainOffset[i]= (unsigned short)(_pInfo[i] * 10.0) ;
	}

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	MultiGroupSendFocalSpi() ;
}

void NetGetGroupSizingCurve(void* pData_ , int nGroupId_)
{
	if(nGroupId_ < 0 || nGroupId_ >= get_group_qty(pp->p_config))
		return;
	CONFIG_SIZING_CURVE* _pInfoTmp = (CONFIG_SIZING_CURVE*)pData_ ;
	SIZING_CURVES* _pInfoSrc  = &GROUP_VAL_POS(nGroupId_ , SizingCurves);
	_pInfoTmp->eCurType = _pInfoSrc->curve_pos  ;
	_pInfoTmp->nPointQty = _pInfoSrc->dac_point_qty;
	_pInfoTmp->nCurveStep = _pInfoSrc->curve_step  ;
	//_pInfoTmp->nRefAmpOffset = _pInfoSrc->ref_ampl_offset ;
	int _nLawQty =  GetGroupBeamQty(nGroupId_) ;

	memcpy((void*)_pInfoTmp->anRefAmp , _pInfoSrc->dac_ref_ampl , sizeof(int) * _nLawQty);
	memcpy((void*)_pInfoTmp->anAmp , _pInfoSrc->amplitude , sizeof(int) * _nLawQty * 16);
	memcpy((void*)_pInfoTmp->anPos , _pInfoSrc->position , sizeof(int) * _nLawQty * 16);
}

void NetSetGroupSizingCurve(void* pData_ , int nGroupId_)
{
	if(nGroupId_ < 0 || nGroupId_ >= get_group_qty(pp->p_config))
		return;
	CONFIG_SIZING_CURVE* _pInfoTmp = (CONFIG_SIZING_CURVE*)pData_ ;
	SIZING_CURVES* _pInfoDist   = &GROUP_VAL_POS(nGroupId_ , SizingCurves);
	_pInfoDist->curve_pos       = _pInfoTmp->eCurType;
	_pInfoDist->dac_point_qty   = _pInfoTmp->nPointQty ;
	_pInfoDist->curve_step      = _pInfoTmp->nCurveStep  ;

	int _nLawQty =  GetGroupBeamQty(nGroupId_) ;
	memcpy((void*)_pInfoDist->dac_ref_ampl , (void*)_pInfoTmp->anRefAmp , sizeof(int) * _nLawQty);
	memcpy((void*)_pInfoDist->amplitude , (void*)_pInfoTmp->anAmp , sizeof(int) * _nLawQty * 16);
	memcpy((void*)_pInfoDist->position , (void*)_pInfoTmp->anPos , sizeof(int) * _nLawQty * 16);

	if(GROUP_VAL_POS(nGroupId_ , SizingCurves.curve_pos) == 3)
	{
		SendTcgSpi(nGroupId_ , -1) ;
		enable_tcg(nGroupId_ , TRUE) ;
	}
	else
	{
		enable_tcg(nGroupId_ , FALSE) ;
	}

	ResetFpgaAll() ;
	RedrawAllUserInterface();
	RestartSCAN() ;
}


void NetGetUtAdvanced(void* pData_ , int nGroupId_)
{
	if(nGroupId_ < 0 || nGroupId_ >= get_group_qty(pp->p_config))
		return;

	CONFIG_UT_ADVANCED* _pInfo = (CONFIG_UT_ADVANCED*)pData_ ;
	unsigned int _nSumGain = GROUP_VAL_POS(nGroupId_ , sum_gain) ;
	_pInfo->fSumGain  = log10(_nSumGain / 16.0) * 20;
	_pInfo->uiPointQty  = GROUP_VAL_POS(nGroupId_, point_qty) ;

}

void NetSetUtAdvanced(void* pData_ , int nGroupId_)
{
	if(nGroupId_ < 0 || nGroupId_ >= get_group_qty(pp->p_config))
		return;

	CONFIG_UT_ADVANCED* _pInfo = (CONFIG_UT_ADVANCED*)pData_ ;
	double _nSumGain = _pInfo->fSumGain ;
	GROUP_VAL_POS(nGroupId_ , sum_gain) = (unsigned short)(16 * pow(10.0 , _nSumGain / 20.0)) ;


	int _nPointQty = _pInfo->uiPointQty   ;
	int _nRange = GROUP_VAL_POS(nGroupId_ , range) / 10 ;
	_nRange = (_nRange / _nPointQty + 1) * _nPointQty  ;

	GROUP_VAL_POS(nGroupId_ , range) = _nRange * 10 ;
	GROUP_VAL_POS(nGroupId_ , point_qty) = _nPointQty  ;

	ResetFpgaAll() ;
	RedrawAllUserInterface();
	//RefreshGainMark(get_current_group(pp->p_config)) ;
	RestartSCAN() ;

}

void NetGetGateInfo(void* pData_ , int nGroupId_ , int nGatePos_)
{
	if(nGroupId_ < 0 || nGroupId_ >= get_group_qty(pp->p_config))
		return;
	CONFIG_GATE_INFO* _pInfo = (CONFIG_GATE_INFO*)pData_ ;

	if(nGatePos_ < 0 || nGatePos_ > 2) nGatePos_  =  0 ;

	GATE_INFO* _pGATE = &(GROUP_VAL_POS(nGroupId_  , gate[nGatePos_])) ;
	_pInfo->eGate      = nGatePos_  ;
	_pInfo->uiStart    = _pGATE->start  ;
	_pInfo->uiWidth    = _pGATE->width  ;
	_pInfo->eSynChro    = _pGATE->synchro  ;
	_pInfo->eMeasure    = _pGATE->measure  ;
	_pInfo->uiThreshold = _pGATE->height   ;
}

void NetSetGateInfo(void* pData_ , int nGroupId_)
{
	if(nGroupId_ < 0 || nGroupId_ >= get_group_qty(pp->p_config))
		return;


	CONFIG_GATE_INFO* _pInfo = (CONFIG_GATE_INFO*)pData_ ;
	int _nGatePos  = _pInfo->eGate ; //GROUP_VAL_POS(nGroupId_ , gate_pos) ;
	GATE_INFO* _pGATE = &(GROUP_VAL_POS(nGroupId_  , gate[_nGatePos])) ;

	_pGATE->start = _pInfo->uiStart  ;
	_pGATE->width = _pInfo->uiWidth  ;
	_pGATE->synchro = _pInfo->eSynChro  ;
	_pGATE->measure = _pInfo->eMeasure  ;
	_pGATE->height  = _pInfo->uiThreshold   ;


	UpdateGateForSpiSending(nGroupId_);
	RefreshGroupFocalLawSpi(nGroupId_) ;
	send_focal_spi(nGroupId_ , FALSE);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}


void NetGetDisplayInfo(void* pData_)
{
	CONFIG_DISPLAY_INFO* _pInfo = (CONFIG_DISPLAY_INFO*)pData_ ;
	_pInfo->eDisplay     = get_display_pos (pp->p_config) ;
	_pInfo->bDisplayAll  = get_display_group (pp->p_config) ;
	_pInfo->eCScan1  = get_cscan_source(pp->p_config , 0)  ;
	_pInfo->eCScan2  = get_cscan_source(pp->p_config , 1)  ;

	int i = 0 ;
	for(i = 0 ; i < 8 ; i++)
		_pInfo->aeUtUnit[i]  = GROUP_VAL_POS(i , ut_unit) ;

}


void NetSetDisplayInfo(void* pData_)
{
	CONFIG_DISPLAY_INFO* _pInfo = (CONFIG_DISPLAY_INFO*)pData_ ;
	set_display_pos (pp->p_config , _pInfo->eDisplay ) ;
	set_display_group (pp->p_config , _pInfo->bDisplayAll) ;
	set_cscan_source(pp->p_config , _pInfo->eCScan1 , 0 )  ;
	set_cscan_source(pp->p_config , _pInfo->eCScan2 , 1 )  ;

	int i = 0 ;
	for(i = 0 ; i < 8 ; i++)
	{
		GROUP_VAL_POS(i , ut_unit) = _pInfo->aeUtUnit[i]  ;
		UpdateGateForSpiSending(i);
		RefreshGroupFocalLawSpi(i) ;
		send_focal_spi(i , FALSE);
	}

	RedrawAllUserInterface();
	RestartSCAN ()  ;


}


void NetSetGainValue(int grp , double nVal_)
{
    group_set_gain(grp, (gshort)(nVal_ * 100));

    RefreshGainMark(grp);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);

    init_group_spi(grp)  ;
    group_spi_send(grp)  ;

}

void NetSetGroupValueStart(int grp , int val)
{

    int tmp = ((val + 5) / 10 ) * 10 ;
	// if the value is not changed  return
    if(tmp == group_get_start(grp)) {
        return ;
    }
    group_set_start(grp, tmp);

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);

	ResetFpgaAll() ;
	RedrawAllUserInterface();
	RefreshGainMark(get_current_group(pp->p_config)) ;
	RestartSCAN() ;


}

void NetSetGroupValueRange(int nGroupId_ , int nVal_)
{

	int _nValue = ((nVal_ + 5) / 10 ) * 10 ;
	// if the value is not changed  return
	if(_nValue == GROUP_VAL_POS(nGroupId_ , range))  return ;

	int _nPointQty = GROUP_VAL_POS(nGroupId_ , point_qty);
	_nValue = (_nValue / _nPointQty + 1) * _nPointQty  ;

	GROUP_VAL_POS(nGroupId_ , range) = _nValue  ;


	ResetFpgaAll() ;
	RedrawAllUserInterface();
	RefreshGainMark(get_current_group(pp->p_config)) ;
	RestartSCAN() ;

}

void NetSetGroupValuePointQty(int nGroupId_ , int nVal_)
{
	if(nVal_ == GROUP_VAL_POS(nGroupId_ , point_qty))  return ;

	int _nRange = GROUP_VAL_POS(nGroupId_ , range) / 10 ;
	_nRange = (_nRange / nVal_ + 1) * nVal_  ;

	GROUP_VAL_POS(nGroupId_ , range) = _nRange * 10 ;
	GROUP_VAL_POS(nGroupId_ , point_qty) = nVal_  ;

	ResetFpgaAll() ;
	RedrawAllUserInterface();
	RefreshGainMark(get_current_group(pp->p_config)) ;
	RestartSCAN() ;
}

void NetSetGroupValueWedgeDelay(int nGroupId_ , int nVal_)
{
	if(nVal_ == GROUP_VAL_POS(nGroupId_ , wedge_delay))  return ;

	GROUP_VAL_POS(nGroupId_ , wedge_delay) = nVal_ ;

	ResetFpgaAll() ;
	RedrawAllUserInterface();
	RefreshGainMark(get_current_group(pp->p_config)) ;
	RestartSCAN() ;
}

void NetSetGroupValueVelocity(int nGroupId_ , int nVal_)
{
	int _nTmp = nVal_ * 100 ;
	if(_nTmp == GROUP_VAL_POS(nGroupId_ , velocity))  return  ;
	GROUP_VAL_POS(nGroupId_ , velocity) = _nTmp   ;

	GenerateFocallawOneGroup(nGroupId_ , TRUE) ;
	InitSizingCurveParameters(nGroupId_) ;
	ResetFpgaAll() ;

	RedrawAllUserInterface() ;
	RestartSCAN()  ;

}

void NetSetGroupValuePRF(int nGroupId_ , int nVal_)
{
	int _nTmp = nVal_ * 10 ;
	if(_nTmp == GROUP_VAL_POS(0 , prf1))  return  ;

	int	_nPrfMax = MultiGroupGetMaxPrf();
    if(_nTmp > _nPrfMax )
    	_nTmp = _nPrfMax ;

	GROUP_VAL_POS(0 , prf1) = _nTmp ;

	int _nScanResolution = get_area_scanresolution (pp->p_config);
	set_inspec_speed (pp->p_config, nVal_ * _nScanResolution);

	ResetFpgaAll() ;
	RedrawAllUserInterface() ;
	RestartSCAN()  ;

}

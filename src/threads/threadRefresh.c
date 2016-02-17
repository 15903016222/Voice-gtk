/*
 * threadRefresh.c
 *
 *  Created on: 2013-5-2
 *      Author: wolflord
 */
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


void ResponseForSampleStartChanged()
{
	gdk_threads_enter();
	draw_area_all();
	gdk_threads_leave();
	pp->bRefreshDraw = TRUE ;
	MultiGroupSendGroupSpi();
	RefreshScanInfor() ;

	FreezeScreen(FALSE) ;
}

void ResponseForVelocityChanged()
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , WedgeDelayCalibrated) = 0 ;
	//GROUP_VAL_POS(grp , SensationCalibrated) =  0  ;
	GROUP_VAL_POS(grp , VelocityCalibrated) =  0  ;
    generate_focallaw (grp);
	RefreshBeamDelay(grp);
	RefreshGainOffset(grp);
	gdk_threads_enter();
	//DrawDisplayWindowFrame() ;
	draw_area_all();
	gtk_widget_queue_draw (pp->status_area) ;
	gtk_widget_queue_draw (pp->drawing_area) ;
	gdk_threads_leave();

	MultiGroupSendGroupSpi();
	MultiGroupSendFocalSpi();
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;

	//if(TMP(freeze))
	//	FreezeScreen(TRUE) ;
	//else
		FreezeScreen(FALSE) ;
}

void ResponseForGateChange()
{
    int grp = get_current_group(pp->p_config);
    if(pp->clb_flag)
    {
        gdk_threads_enter();
        DrawDisplayWindowFrameCalibration();
        gdk_threads_leave();
    }
	UpdateGateForSpiSending(grp);
	send_focal_spi (grp , SPI_RESET_NO);

	if(TMP(freeze))
		FreezeScreen(TRUE) ;
	else
		FreezeScreen(FALSE) ;

}

void ResponseForSampleRangeChanged()
{
	gdk_threads_enter();
	draw_area_all();
	gdk_threads_leave();
	pp->bRefreshDraw = TRUE ;
	MultiGroupSendGroupSpi();
	RefreshScanInfor() ;

	//if(TMP(freeze))
	//	FreezeScreen(TRUE) ;
	//else
		FreezeScreen(FALSE) ;
}

void ResponseForFocallawTypeChanged()
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , WedgeDelayCalibrated) = 0 ;
    GROUP_VAL_POS(grp , SensationCalibrated) = 0 ;
    generate_focallaw (grp);
	RefreshBeamDelay(grp);
	RefreshGainOffset(grp);
	InitSizingCurveParameters(grp) ;
	gdk_threads_enter();
	//DrawDisplayWindowFrame() ;
	draw_area_all();
	gtk_widget_queue_draw (pp->status_area) ;
	gtk_widget_queue_draw (pp->drawing_area) ;
	gdk_threads_leave();
	MultiGroupSendGroupSpi();
	MultiGroupSendFocalSpi();
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;

	//if(TMP(freeze))
	//	FreezeScreen(TRUE) ;
	//else
		FreezeScreen(FALSE) ;
}

void ResponseForAngleChanged()
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , WedgeDelayCalibrated) = 0 ;
    GROUP_VAL_POS(grp , SensationCalibrated) = 0 ;
    generate_focallaw (grp);

    RefreshPointQty() ;
    RefreshBeamDelay(grp);
	RefreshGainOffset(grp);
	InitSizingCurveParameters(grp) ;
	gdk_threads_enter();
	gtk_widget_queue_draw (pp->status_area) ;
	gtk_widget_queue_draw (pp->drawing_area) ;
	draw_area_all();
	gdk_threads_leave();
	//******* to prevent data size out of limit
	MultiGroupSendGroupSpi();
	MultiGroupSendFocalSpi();
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;

	FreezeScreen(FALSE) ;
}

void ResponseForMinAngleChanged()
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , WedgeDelayCalibrated) = 0 ;
    GROUP_VAL_POS(grp , SensationCalibrated) = 0 ;
    generate_focallaw (grp);
    //******* to prevent data size out of limit
    RefreshPointQty() ;
	RefreshBeamDelay(grp);
	RefreshGainOffset(grp);
	InitSizingCurveParameters(grp) ;
	gdk_threads_enter();

	draw_area_all();
	gtk_widget_queue_draw (pp->status_area) ;
	gtk_widget_queue_draw (pp->drawing_area) ;
	gdk_threads_leave();

	MultiGroupSendGroupSpi();
	MultiGroupSendFocalSpi();
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;
	FreezeScreen(FALSE) ;
}

void ResponseForMaxAngleChanged()
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , WedgeDelayCalibrated) = 0 ;
    GROUP_VAL_POS(grp , SensationCalibrated) = 0 ;
    generate_focallaw (grp);
    RefreshPointQty();
	RefreshBeamDelay(grp);
	RefreshGainOffset(grp);
	InitSizingCurveParameters(grp) ;
	gdk_threads_enter();
	//******* to prevent data size out of limit
	draw_area_all();
	gtk_widget_queue_draw (pp->status_area) ;
	gtk_widget_queue_draw (pp->drawing_area) ;
	gdk_threads_leave();

	MultiGroupSendGroupSpi();
	MultiGroupSendFocalSpi();
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;

	//if(TMP(freeze))
	//	FreezeScreen(TRUE) ;
	//else
		FreezeScreen(FALSE) ;
}

void ResponseForStepAngleChanged()
{
	int grp = get_current_group (pp->p_config) ;
	int beam_no = (int)((LAW_VAL_POS(grp , Angle_max) - LAW_VAL_POS(grp , Angle_min)) / LAW_VAL_POS(grp , Angle_step));
	LAW_VAL_POS(grp , Angle_max) = LAW_VAL_POS(grp , Angle_min) + beam_no * LAW_VAL_POS(grp , Angle_step) ;
	GROUP_VAL_POS(grp , WedgeDelayCalibrated) = 0 ;
    GROUP_VAL_POS(grp , SensationCalibrated) = 0 ;
    generate_focallaw (grp);

	//******* to prevent data size out of limit
	RefreshPointQty() ;
	RefreshBeamDelay(grp);
	RefreshGainOffset(grp);
	InitSizingCurveParameters(grp) ;
	gdk_threads_enter();
	draw_area_all();
	gtk_widget_queue_draw (pp->status_area) ;
	gtk_widget_queue_draw (pp->drawing_area) ;
	draw_menu3(0, NULL);
	gdk_threads_leave();


	MultiGroupSendGroupSpi();
	MultiGroupSendFocalSpi();
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;

	//if(TMP(freeze))
	//	FreezeScreen(TRUE) ;
	//else
		FreezeScreen(FALSE) ;
}

void RespnseForFocusDepthChanged()
{
	int grp = get_current_group (pp->p_config) ;

	//GROUP_VAL_POS(grp , WedgeDelayCalibrated) = 0 ;
    generate_focallaw (grp);
	//RefreshBeamDelay(grp);

	gdk_threads_enter();
	DrawDisplayWindowFrame() ;
	gtk_widget_queue_draw (pp->status_area) ;
	gtk_widget_queue_draw (pp->drawing_area) ;
	gdk_threads_leave();

	MultiGroupSendGroupSpi();
	MultiGroupSendFocalSpi();
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;

	if(TMP(freeze))
		FreezeScreen(TRUE) ;
	else
		FreezeScreen(FALSE) ;
}

void ResponseForElmentQtyChanged()
{

	int grp = get_current_group (pp->p_config) ;
	if(LAW_VAL_POS(grp , Focal_type) == 0)//Azimuthal
		LAW_VAL_POS(grp , Last_tx_elem) = (unsigned char) (LAW_VAL_POS(grp , First_tx_elem) + LAW_VAL_POS(grp , Elem_qty)) - 1;
	else//Linear
	{
		if( LAW_VAL_POS(grp , Last_tx_elem) < ((unsigned char) (LAW_VAL_POS(grp , First_tx_elem) + LAW_VAL_POS(grp , Elem_qty)) - 1) )
			LAW_VAL_POS(grp , Last_tx_elem) = (unsigned char) (LAW_VAL_POS(grp , First_tx_elem) + LAW_VAL_POS(grp , Elem_qty)) - 1;
	}

	int _nGroupSumGain  ;
	if(!GROUP_VAL_POS(grp , sum_gain_pos))
	{
	   if (LAW_VAL_POS(grp, Elem_qty) == 1)
		    _nGroupSumGain	= 4095;
	   else
		    _nGroupSumGain	= 6400 / LAW_VAL_POS(grp, Elem_qty);
	   GROUP_VAL_POS(grp , sum_gain) = _nGroupSumGain ;
	   TMP(group_spi[grp]).sum_gain  = _nGroupSumGain ;
	}

	GROUP_VAL_POS(grp , WedgeDelayCalibrated) = 0 ;
    GROUP_VAL_POS(grp , SensationCalibrated) = 0 ;
    generate_focallaw (grp);
    ConfirmPulserReceiverInRange(grp);
	//******* to prevent data size out of limit
	RefreshPointQty() ;

    RefreshBeamDelay(grp);
	RefreshGainOffset(grp);
    InitSizingCurveParameters(grp) ;
	gdk_threads_enter();
	draw_area_all();
	draw_menu3(0, NULL);
	gtk_widget_queue_draw (pp->status_area) ;
	gtk_widget_queue_draw (pp->drawing_area) ;
	gdk_threads_leave();

	MultiGroupSendGroupSpi();
	MultiGroupSendFocalSpi();
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;

	FreezeScreen(FALSE) ;
}

void ResponseForElmentStartChanged()
{
	int grp = get_current_group (pp->p_config) ;

	GROUP_VAL_POS(grp , WedgeDelayCalibrated) = 0 ;
    GROUP_VAL_POS(grp , SensationCalibrated) = 0 ;
    ConfirmPulserReceiverInRange(grp);
    generate_focallaw (grp);

	//******* to prevent data size out of limit
	RefreshPointQty() ;
    RefreshBeamDelay(grp);
	RefreshGainOffset(grp);
    InitSizingCurveParameters(grp) ;
	gdk_threads_enter();
	draw_area_all();
	draw_menu3(0, NULL);
	gtk_widget_queue_draw (pp->status_area) ;
	gtk_widget_queue_draw (pp->drawing_area) ;
	gdk_threads_leave();

	MultiGroupSendGroupSpi();
	MultiGroupSendFocalSpi();
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;

	FreezeScreen(FALSE) ;
}

void ResponseForElmentStopChanged()
{

	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , WedgeDelayCalibrated) = 0 ;
    GROUP_VAL_POS(grp , SensationCalibrated) = 0 ;
    ConfirmPulserReceiverInRange(grp);
    generate_focallaw (grp);
	//******* to prevent data size out of limit
	RefreshPointQty() ;

	RefreshBeamDelay(grp);
	RefreshGainOffset(grp);
    InitSizingCurveParameters(grp) ;
	gdk_threads_enter();
	draw_area_all();
	draw_menu3(0, NULL);
	gtk_widget_queue_draw (pp->status_area) ;
	gtk_widget_queue_draw (pp->drawing_area) ;
	gdk_threads_leave();

	MultiGroupSendGroupSpi();
	MultiGroupSendFocalSpi();
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;

	FreezeScreen(FALSE) ;
}

void ResponseForElmentStepChanged()
{

	int grp = get_current_group (pp->p_config) ;
	int _nStep = LAW_VAL_POS(grp , Elem_step)    ;
	int _nProbeQty = MIN(gMaxElementRecieveQty , GROUP_VAL_POS (grp , probe.Elem_qty)) ;
	int _nLastElemnt = _nStep * ((LAW_VAL_POS(grp , Last_tx_elem) + 1 - LAW_VAL_POS(grp , Elem_qty)-
			LAW_VAL_POS(grp , First_tx_elem))/_nStep + 1) + LAW_VAL_POS(grp , First_tx_elem) + LAW_VAL_POS(grp , Elem_qty) - 1;

	if(_nLastElemnt >  _nProbeQty)
	{
		 _nLastElemnt = _nStep * ((_nProbeQty + 1 - LAW_VAL_POS(grp , Elem_qty)-
					LAW_VAL_POS(grp , First_tx_elem))/_nStep) + LAW_VAL_POS(grp , First_tx_elem) + LAW_VAL_POS(grp , Elem_qty) - 1;
	}
	LAW_VAL_POS(grp , Last_tx_elem) = _nLastElemnt  ;
	GROUP_VAL_POS(grp , WedgeDelayCalibrated) = 0 ;
    GROUP_VAL_POS(grp , SensationCalibrated) = 0 ;
    ConfirmPulserReceiverInRange(grp);
    generate_focallaw (grp);

	//******* to prevent data size out of limit
	RefreshPointQty() ;
	RefreshBeamDelay(grp);
	RefreshGainOffset(grp);
    InitSizingCurveParameters(grp) ;
	gdk_threads_enter();
	draw_area_all();
	draw_menu3(0, NULL);
	gtk_widget_queue_draw (pp->status_area) ;
	gtk_widget_queue_draw (pp->drawing_area) ;
	gdk_threads_leave();

	MultiGroupSendGroupSpi();
	MultiGroupSendFocalSpi();
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;

	FreezeScreen(FALSE) ;
}

void ResponseForPointQtyChanged()
{
	gdk_threads_enter();
	draw_area_all() ;
	gdk_threads_leave();
	// MultiGroupRefreshIdelTime() ;
	// display the frame : ruler  title mark .....
	MultiGroupSendGroupSpi();
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;

	FreezeScreen(FALSE) ;
}

void MultiGroupAction()
{

	if(get_group_qty(pp->p_config) == 1)
	{
		set_display_group (pp->p_config, DISPLAY_CURRENT_GROUP);
	}

	if(!IsDisplayModalAvailable(get_display_pos(pp->p_config)))
	{
		set_display_pos(pp->p_config, A_SCAN) ;
	}

	int grp = get_current_group(pp->p_config);
	gdk_threads_enter();
	draw_area_all();
	gtk_widget_queue_draw (pp->status_area);

	if(GROUP_VAL_POS(grp , group_mode) != PA_SCAN) /*group mode 选择UT,UT1,UT2时，focal law 不可用*/
		gtk_widget_set_sensitive(pp->menuitem[6], FALSE);
	else
		gtk_widget_set_sensitive(pp->menuitem[6], TRUE);

	RefreshGainMark(grp) ;

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	gdk_threads_leave();
	pp->bRefreshDraw = TRUE ;
	pp->ctype_pos = 1;
	pp->cmode_pos = 0;

	FreezeScreen(FALSE) ;

}

//extern void SetDefaultPCS(int grp);
void SetupReload()
{
	int i;
	int _nGroupId = get_current_group(pp->p_config);
	//生成聚焦法则
	int _nEncodeSource = get_inspec_source (pp->p_config);
	for(i = 0 ; i < get_group_qty(pp->p_config) ; ++i)
	{
		generate_focallaw (i);
		if(	!GROUP_VAL_POS(i , WedgeDelayCalibrated) )
			RefreshBeamDelay(i);
		if(	!GROUP_VAL_POS(i , SensationCalibrated) )
			RefreshGainOffset(i);
	}
	gdk_threads_enter();
	draw_area_all();
	draw_field_name ();
	gtk_widget_queue_draw (pp->status_area) ;
	gtk_widget_queue_draw (pp->drawing_area) ;

	if(_nEncodeSource)
	{
		update_widget_bg(pp->event[18], 19);
	}
	else
	{
		update_widget_bg(pp->event[18], 18);
	}

	for(i = 0 ; i < 10 ; i++)
		gtk_widget_set_sensitive(pp->menuitem[i],TRUE);

	if(GROUP_VAL_POS(_nGroupId , group_mode) != PA_SCAN) /*group mode 选择UT,UT1,UT2时，focal law 不可用*/
	{
		gtk_widget_set_sensitive(pp->menuitem[6],FALSE);
	}

	gdk_threads_leave();


	output_init(GetChannelEnable()) ;
	MultiGroupSendFocalSpi();
	MultiGroupSendGroupSpi();

	TMP(dataRecalling) = 0;
	RefreshScanInfor() ;
	//SetDefaultPCS(_nGroupId);

	pp->bRefreshDraw = TRUE ;
	FreezeScreen(FALSE) ;
    {
        gint encType = get_inspec_source(pp->p_config);
        guchar currentEcoderType = 0;
        if(encType == 0 ) {
             /*timer is no need to changed encoder parameters*/
            return ;
        }
        encType -= 1 ;
        currentEcoderType  = get_enc_type (pp->p_config,  encType );

        currentEcoderType &= 11 ;

        set_enc_type (pp->p_config,  currentEcoderType , encType );
        if(encType == 0)
        {
             output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_ENCODE_X , currentEcoderType , 0);
        }
        if(encType == 1)
        {
             output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_ENCODE_Y , currentEcoderType , 0);
        }
        output_write_one_reg_to_spi(0,OUTPUT_OTHER_COMMAND_ENCODE_Y);
    }
}

void ResponseForPrf()
{
	MultiGroupRefreshIdelTime() ;
	MultiGroupSendGroupSpi();
	gdk_threads_enter();
	draw_area_all();
	draw_menu3(0, NULL);
	gdk_threads_leave();

	RefreshScanInfor();
	pp->bRefreshDraw = TRUE ;

	int prf = 	GROUP_VAL_POS(0 , prf1) / 10 ;
	int _nScanResolution = get_area_scanresolution (pp->p_config);
	set_inspec_speed (pp->p_config, prf * _nScanResolution);

	if(TMP(freeze))
		FreezeScreen(TRUE) ;
	else
		FreezeScreen(FALSE) ;
}

void ResponseForScanResolution()
{
	gdk_threads_enter();
	draw_area_all() ;
	gdk_threads_leave();
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;

	if(TMP(freeze))
		FreezeScreen(TRUE) ;
	else
		FreezeScreen(FALSE) ;
}

void ResponseForThicknessChanged()
{
	int _nGroupId = get_current_group(pp->p_config);
	gdk_threads_enter();
	DrawDisplayWindowFrame() ;
	gdk_threads_leave();
    group_spi_send(_nGroupId);
	//RefreshScanInfor();
	TMP(scan)   = 0 ;
	TMP(index)  = 0 ;
    pp->bRefreshDraw = TRUE ;

	if(TMP(freeze))
		FreezeScreen(TRUE) ;
	else
		FreezeScreen(FALSE) ;
}

void ResponseForPartPositionChanged()
{
	int _nGroupId = get_current_group(pp->p_config);
	if((LAW_VAL_POS(_nGroupId , Focal_point_type)==PROJECTION_P)||(LAW_VAL_POS(_nGroupId , Focal_point_type)==FOCALPLANE_P))
	{
		generate_focallaw (_nGroupId);
		MultiGroupSendFocalSpi();
		MultiGroupSendFocalSpi();
	}
	gdk_threads_enter();
	DrawDisplayWindowFrame() ;
	gdk_threads_leave();

    //RefreshScanInfor() ;
    pp->bRefreshDraw = TRUE ;

	if(TMP(freeze))
		FreezeScreen(TRUE) ;
	else
		FreezeScreen(FALSE) ;
}

void ClearRowDataBuffer()
{
#if ARM
    unsigned char* pData = (unsigned char*) (pp->p_beam_data);
    memset(pData , 0 , 192*1024);
    pData += 256 * 1024 ;
    memset(pData , 0 , 192*1024);
    pData += 256 * 1024 ;
    memset(pData , 0 , 192*1024);
    pData += 256 * 1024 ;
    memset(pData , 0 , 192*1024);
#endif
}

void ResponseForDataLoad()
{
	int i;
	//生成聚焦法则？
    int _nEncodeSource ;
    int _nGroupId      ;

	for(i = 0 ; i < get_group_qty(pp->p_config) ; ++i)
	{
		generate_focallaw (i);
        if(	!GROUP_VAL_POS(i , WedgeDelayCalibrated) ) {
			RefreshBeamDelay(i);
        } else if (	!GROUP_VAL_POS(i , SensationCalibrated) ) {
			RefreshGainOffset(i);
        }
	}

	output_init(GetChannelEnable()) ;
	MultiGroupSendFocalSpi();
	MultiGroupSendGroupSpi();
	FreezingFPGA(TRUE);
	gdk_threads_enter();
	draw_area_all();
	draw_field_name ();
	gtk_widget_queue_draw (pp->status_area) ;
	gtk_widget_queue_draw (pp->drawing_area) ;
	_nEncodeSource = get_inspec_source (pp->p_config);
	_nGroupId   = get_current_group(pp->p_config);
	if(_nEncodeSource)
	{
		update_widget_bg(pp->event[18], 19);
	}
	else
	{
		update_widget_bg(pp->event[18], 18);
	}
	gdk_threads_leave();
	RefreshScanInfor() ;
	//SetDefaultPCS(_nGroupId);
	pp->bRefreshDraw = TRUE ;

	FreezeScreen(TRUE) ;
}

void ResponseForPulseWidthChanged()
{
	int grp = get_current_group (pp->p_config) ;
    group_spi_send (grp);

	if(TMP(freeze))
		FreezeScreen(TRUE) ;
	else
		FreezeScreen(FALSE) ;
}

void ResponseForBeamDelayChanged()
{
	int grp = get_current_group (pp->p_config) ;
	MultiGroupSendGroupSpi();
	UpdateGateForSpiSending(grp);
	send_focal_spi(grp , SPI_RESET_NO);

	if(TMP(freeze))
		FreezeScreen(TRUE) ;
	else
		FreezeScreen(FALSE) ;
}

void ResponseForPorbeLoad()
{
	int grp = get_current_group(pp->p_config);
	set_filter(grp);
	GROUP_VAL_POS(grp , WedgeDelayCalibrated) = 0 ;
    GROUP_VAL_POS(grp , SensationCalibrated) = 0 ;
    generate_focallaw (grp);
	RefreshBeamDelay(grp);
	RefreshGainOffset(grp);
	MultiGroupSendGroupSpi();
	MultiGroupSendFocalSpi();
	set_auto_detect (pp->p_config , 0);
	gdk_threads_enter();
	gtk_widget_queue_draw (pp->status_area) ;
	gtk_widget_queue_draw (pp->drawing_area) ;
	draw_area_all() ;
	draw_menu3(0, NULL);
	gdk_threads_leave();

	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;

	FreezeScreen(FALSE) ;
}

void ResponseForCurrentLawChanged()
{
	gdk_threads_enter();
	DrawDisplayWindowFrame() ;
	gdk_threads_leave();

	if(TMP(freeze))
		FreezeScreen(TRUE) ;
	else
		FreezeScreen(FALSE) ;
}

void ResponseForDrawAreaAll()
{
	gdk_threads_enter();
	draw_area_all();
	gdk_threads_leave();

	pp->bRefreshDraw = TRUE ;

	if(TMP(freeze))
		FreezeScreen(TRUE) ;
	else
		FreezeScreen(FALSE) ;
}

void ResponseForFreq()
{
	int grp = get_current_group(pp->p_config);
	GROUP_VAL_POS(grp , pulser_width1 )  = get_pw() * PW_DIV  ;

	gdk_threads_enter();
	draw_menu3(0, NULL);
	gdk_threads_leave();

	MultiGroupSendGroupSpi();
	MultiGroupSendFocalSpi();
	if(TMP(freeze))
		FreezeScreen(TRUE) ;
	else
		FreezeScreen(FALSE) ;
}

void ResponseForWedgeDelay()
{
	int grp = get_current_group(pp->p_config);
	UpdateGateForSpiSending(grp) ;
	//需要更新FOCAL SPI ，更新 TCG时，更新了 FOCAL SPI
	if(GROUP_VAL_POS(grp , SizingCurves.curve_pos) == 3)
		SendTcgSpi(grp , -1) ;
	else
		send_focal_spi(grp , SPI_RESET_NO);

	MultiGroupSendGroupSpi();
	if(TMP(freeze))
		FreezeScreen(TRUE) ;
	else
		FreezeScreen(FALSE) ;
}

void Refresh_all(int refresh_mark)
{
	switch(refresh_mark)
	{
		case REFRESH_START_MARK:
			ResponseForSampleStartChanged();
			break;
		case REFRESH_RANGE_MARK :
			ResponseForSampleStartChanged() ;
			break ;
		case REFRESH_WEDGE_DELAY_MARK :
			ResponseForWedgeDelay();
			break ;
		case REFRESH_VELOCITY_MARK :
			ResponseForVelocityChanged() ;
			break ;
		case REFRESH_FREQUENCY_MARK :
			ResponseForFreq();
			break ;
		case REFRESH_PULSE_WIDTH_MARK:
			ResponseForPulseWidthChanged();
			break ;
		case REFRESH_BEAM_DELAY_MARK:
			ResponseForBeamDelayChanged();
			break ;
		case REFRESH_POINT_MARK:
			ResponseForPointQtyChanged() ;
			break ;
		case REFRESH_FOCALLAW_TYPE_MARK:
			ResponseForFocallawTypeChanged() ;
			break ;
		case REFRESH_ANGLE_MARK:
			ResponseForAngleChanged()  ;
			break ;
		case REFRESH_ANGLE_MARK_MIN:
			ResponseForMinAngleChanged()  ;
			break ;
		case REFRESH_ANGLE_MARK_MAX:
			ResponseForMaxAngleChanged()  ;
			break ;
		case REFRESH_ANGLE_MARK_STEP:
			ResponseForStepAngleChanged()  ;
			break;
		case REFRESH_FOCAL_DEPTH_MARK:
			RespnseForFocusDepthChanged() ;
			break;
		case REFRESH_ELEMENT_MARK_QTY :
			ResponseForElmentQtyChanged() ;
			break;
		case REFRESH_ELEMENT_MARK_START :
			ResponseForElmentStartChanged() ;
			break;
		case REFRESH_ELEMENT_MARK_STOP:
			ResponseForElmentStopChanged() ;
			break;
		case REFRESH_ELEMENT_MARK_STEP :
			ResponseForElmentStepChanged() ;
			break ;
		case REFRESH_KEY_ENTER_MARK:
			break ;
		case REFRESH_GATE_POSITION:
			ResponseForGateChange();
			break ;
		case REFRESH_MULTI_GROUP_ACTION:
			MultiGroupAction();
			break ;
		case REFRESH_SETUP_CONFIG_LOAD:
			SetupReload();
			break;
		case REFRESH_PRF_CHANGED:
			ResponseForPrf();
			break;
		case REFRESH_SCAN_RESOLUTION:
			ResponseForScanResolution();
			break;
		case REFRESH_CALIBRATION_REDRAW:
			gdk_threads_enter();
			DrawDisplayWindowFrameCalibration();
			gdk_threads_leave();
			pp->bRefreshDraw = TRUE ;
			FreezeScreen(FALSE) ;
			break;
		case REFRESH_THICKNESS_CHANGED:
			ResponseForThicknessChanged();
			break;
		case REFRESH_PART_POSITION:
			ResponseForPartPositionChanged();
			break;
		case REFRESH_SCANDATA_LOAD:
			ResponseForDataLoad();
			break;
		case REFRESH_PROBE_LOAD:
			ResponseForPorbeLoad();
			break;
		case REFRESH_CURRENT_LAW_CHANGED:
			ResponseForCurrentLawChanged();
			break;
		case REFRESH_DRAW_AREA_ALL:
			ResponseForDrawAreaAll();
			break;
		default:
			break ;
	}
	gdk_threads_enter();
	RefreshPrfMark();
	gdk_threads_leave();
}

void RefressFocalLaw()
{
	REFRESH_REQUEST = 0 ;
	static int REFRESH_MARK = 0 ;
	static int REFRESH_RESPONSE = 0;
	while(1)
	{
		if(REFRESH_REQUEST || REFRESH_RESPONSE)
		{
			if(REFRESH_REQUEST)
			{
				REFRESH_MARK = REFRESH_REQUEST ;
				REFRESH_RESPONSE = 1 ;
				REFRESH_REQUEST  = 0 ;
			}
			else
			{
				pthread_mutex_lock(&draw_thread_mutex);
				REFRESH_RESPONSE = 0 ;
				Refresh_all(REFRESH_MARK);
				pthread_mutex_unlock(&draw_thread_mutex);
			}
		}
		usleep(300000) ;
	}
}

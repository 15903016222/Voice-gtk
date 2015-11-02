/*
 * threadDrawData.c
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
#include "../calculation/getMeasureData.h"
#include "../net/DopplerNetServer.h"
#include <gdk/gdk.h>
#include <glib/gprintf.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>


void draw_field_value ()
{
	char  *markup[4];
	char  *markup_encoder, *markup_vel;
	char  *DB_mark ;
	double cur_value;
	double _nStepPerMM ;
	int i ;

	gint grp = get_current_group(pp->p_config);//当前group
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	/* 4个测量值显示 */
	for(i = 0 ; i < 4 ; i++)
	{
		cur_value = TMP(field[0][i] ) ;
		if(cur_value >= MEASURE_DATA_BASE )
		{
			if(cur_value == MEASURE_DATA_ND )
				markup[i] = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>ND</span>");
			else if(cur_value == MEASURE_DATA_NS )
				markup[i] = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>NS</span>");
			else if(cur_value == MEASURE_DATA_A )
				markup[i] = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>A</span>");
			else if(cur_value == MEASURE_DATA_B )
				markup[i] = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>B</span>");
			else if(cur_value == MEASURE_DATA_C )
				markup[i] = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>C</span>");
			else if(cur_value == MEASURE_DATA_D )
				markup[i] = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>D</span>");
			else //if(cur_value == MEASURE_DATA_NULL )
				markup[i] = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>---</span>");

		}
		else
		    markup[i] = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>%.1f</span>", cur_value);
	}

	if(pp->nEncoderType)
	{
		_nStepPerMM =  pp->p_config->encoder1[0].Resolution / 1000.0 ;
		markup_encoder = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>X: %.1f mm</span>",
				TMP(measure_data[0][4])/_nStepPerMM);
	}
	else//Encoder scan time
	{
		_nStepPerMM = GROUP_VAL_POS(0 , prf1) / 10.0 ;
		markup_encoder = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>X: %.1f  s</span>",
				(g_nScanTimmerCounter + g_bScanTimmerCircled *  pp->ScanInfor.MaxStoreIndex) / _nStepPerMM );
	}

	if (get_group_db_ref (pp->p_config, grp))
		DB_mark = g_markup_printf_escaped (
				"<span foreground='white' font_desc='16'>%0.1f(%0.1f)</span>",
				(get_group_val (p_grp, GROUP_GAIN) -
				 get_group_val (p_grp, GROUP_GAINR)) / 100.0,
				get_group_val (p_grp, GROUP_GAINR) / 100.0);
	else
		DB_mark = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>%0.1f</span>",
				get_group_val (p_grp, GROUP_GAIN) / 100.0 );

	if(UNIT_MM == get_unit(pp->p_config))
	{
		//cur_value = get_inspec_speed (pp->p_config)/1000.0;
		cur_value = get_prf() * get_area_scanresolution (pp->p_config) / 10000.0;
		markup_vel = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>V: %.1f mm/s</span>", cur_value);
	}
	else
	{
		//cur_value = get_inspec_speed (pp->p_config)/1000.0 * 0.03937;
		cur_value = 0.03937 * get_prf() * get_area_scanresolution (pp->p_config) / 10000.0;
		markup_vel = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>V: %.1f inch/s</span>", cur_value);//inch
	}


	gdk_threads_enter();
	gtk_label_set_markup (GTK_LABEL (pp->label[5]), markup_vel);
	gtk_label_set_markup (GTK_LABEL(pp->label[7]),  markup_encoder);
	gtk_label_set_markup (GTK_LABEL(pp->label[9]),  markup[0]);
	gtk_label_set_markup (GTK_LABEL(pp->label[11]), markup[1]);
	gtk_label_set_markup (GTK_LABEL(pp->label[13]), markup[2]);
	gtk_label_set_markup (GTK_LABEL(pp->label[15]), markup[3]);
	gtk_label_set_markup (GTK_LABEL(pp->label[GAIN_VALUE]), DB_mark);
	gdk_threads_leave();
	g_free (markup_vel);
	g_free (markup[0]);
	g_free (markup[1]);
	g_free (markup[2]);
	g_free (markup[3]);
	g_free (DB_mark);
	g_free (markup_encoder);
}


/* 计算测量数值 */
//fieldIndex为索引值，根据索引值输出相应计算的测量值
void calc_measure_data()
{
	int offset,k;
	int *pData  ;
	double gate_a_data ;
	double A;

	int grp = get_current_group(pp->p_config);//当前group
	for (offset = 0, k = 0 ; k < grp; k++)
		offset += TMP(beam_qty[k]);
	int _nBeamNo = TMP(beam_num[grp]);
	int _nIndex = offset + _nBeamNo;

	pData = TMP(measure_data[_nIndex]) ;
	if(pp->p_config->group[grp].rectifier1)
	{
		A = (((pData[1])>>20) & 0x00000fff) /20.47;//满屏时200% 4095
	}
	else
	{
		A = (signed short)((pData[1]) >> 16) ;
		A = A / (10.24 * 16);
	}

	int gate_height = GROUP_VAL_POS(grp , gate[1].height);
	if(gate_height > A)
	{
		pp->thickness_refract_times = 0 ;
	}
	else
	{
		double a = TMP(current_angle[grp]);
		double cos_current_angle = cos(a);
		double velocity = GROUP_VAL_POS(grp , velocity) / 100.0 ; // m/s
		int _nPulseWidth  = GROUP_VAL_POS(grp , pulser_width1) / (10 * PW_DIV);
		int _nWedgeDelay  = GROUP_VAL_POS(grp , wedge_delay) / 10;
		int current_beam_delay = GROUP_VAL_POS(grp , beam_delay[_nBeamNo]) / 10;    // 10ns
		double thickness = (double)(GROUP_VAL_POS(grp , part.Thickness)/1000.0);//工件厚度
		_nWedgeDelay += current_beam_delay + _nPulseWidth  ;
		gate_a_data = (double)((pData[1] & 0xfffff) - _nWedgeDelay);
		gate_a_data = gate_a_data * velocity * cos_current_angle / 200000 ;
		pp->thickness_refract_times = (int)(gate_a_data / thickness) ;
	}


	for(k=0;k<4;k++)//4个field
	{
		TMP(field[0][k]) =  getMeasureData(DO_NOT_USE_CCFG(field[k]) , grp , _nIndex)  ;
		TMP(field[1][k]) =  getMeasureData(getFieldIndex(1,k) , grp , _nIndex)  ;
	}

}


void RefreshCurrentBeamWhenCalibration()
{
	static unsigned int _nDiver = 0;
    char* markup1 ;
    char* markup2 ;
	if((!pp->clb_flag) || (pp->cmode_pos == 0) || (pp->cmode_pos == 3) || (pp->ctype_pos==0))
	{// if not calibration return , if calibration and not SENSATION or WedgeDelay mode , return
		return ;
	}
	_nDiver++  ;
	if(_nDiver%3)  // 降低刷新频率，怕 CPU 累着
	{
		return ;
	}

	int _nMaxDB , _nTmpDB , _nTmpBeamNo;
	int _nBeamOffset = 0 ;
	int _nDataOffset = 0 ;
	int _nGroupId  = get_current_group(pp->p_config);
	int _nBeamQty  = TMP(beam_qty[_nGroupId]);
	int _nAngle  ;
	int i ;
	for ( i = 0 ; i < _nGroupId; i++)
	{
		_nDataOffset += (GROUP_VAL_POS(i, point_qty) + 32) * TMP(beam_qty[i]);
		_nBeamOffset += TMP(beam_qty[i]);
	}
	_nTmpBeamNo = 0 ;
	_nMaxDB = (unsigned int)((TMP(measure_data[_nBeamOffset][1])>>20) & 0x00000fff) ;
    for(i = 1 ; i < _nBeamQty ; i++ )
    {
    	_nTmpDB  = (unsigned int)((TMP(measure_data[_nBeamOffset + i][1])>>20) & 0x00000fff) ;
		if(_nMaxDB < _nTmpDB)
		{
			_nMaxDB = _nTmpDB ;
			_nTmpBeamNo = i  ;
		}
    }
    if(_nTmpBeamNo == TMP(beam_num[_nGroupId]))
    {
    	return ;
    }

    TMP(beam_num[_nGroupId]) = _nTmpBeamNo ;
    if(LAW_VAL_POS(_nGroupId , Focal_type) == AZIMUTHAL_SCAN )
    {
         _nAngle = LAW_VAL_POS( _nGroupId , Angle_min) + TMP(beam_num[_nGroupId]) * LAW_VAL_POS( _nGroupId ,Angle_step ) ;
         TMP(current_angle[_nGroupId]) = _nAngle * G_PI / 18000.0 ;
    }
    else
    {
    	_nAngle = LAW_VAL_POS( _nGroupId , Angle_min)  ;
    }

	g_sprintf ( pp->draw_area[0].title, "S scan|Gr %d|CH %3.1f|SK%3.1f|L%2d",
			_nGroupId + 1, _nAngle / 100.0, GROUP_VAL_POS(_nGroupId, skew) / 100.0, _nTmpBeamNo + 1);
    g_sprintf ( pp->draw_area[1].title, "A scan|Gr %d|CH %3.1f|SK%3.1f|L%2d",
    		_nGroupId + 1, _nAngle / 100.0, GROUP_VAL_POS(_nGroupId, skew) / 100.0, _nTmpBeamNo + 1);
	markup1 = g_markup_printf_escaped ("<span foreground='red' font_desc='9'>%s</span>", pp->draw_area[0].title);
	markup2 = g_markup_printf_escaped ("<span foreground='red' font_desc='9'>%s</span>", pp->draw_area[1].title);
	gdk_threads_enter();
	gtk_label_set_markup(GTK_LABEL(pp->draw_area[0].label) , markup1 );
	gtk_label_set_markup(GTK_LABEL(pp->draw_area[1].label) , markup2 );
	gtk_widget_queue_draw(pp->draw_area[0].drawing_area);
	gdk_threads_leave();
	g_free(markup1);
	g_free(markup2);
}

void GateSynchroRefresh()
{
	static int _nDiver = 0  ;
    if(!pp->bGateSynchro)
    {
    	return ;
    }
	_nDiver++ ;
	if(_nDiver%3)  // 降低刷新频率，怕 CPU 累着
	{
		return ;
	}
	gdk_threads_enter();
	gtk_widget_queue_draw(pp->vboxtable) ;
	gdk_threads_leave();
}

void draw_frame_thread(int nPosition_)
{
#if ARM
	unsigned int pBeamData    ;
	int i, j ,k, offset, offset1;

	// current scan index, used for all b and c scan drawing
	g_nScanTimmerCounter = DMA_MARK[9]  ;
	g_bScanTimmerCircled = DMA_MARK[10] ;

	pBeamData = (pp->p_beam_data) + DMA_DATA_OFFSET + REGION_SIZE * nPosition_;
	for (i = 0 ; i < get_group_qty(pp->p_config); i++)
	{
		for (offset = 0, offset1 = 0, k = 0 ; k < i; k++)
		{
			offset += (GROUP_VAL_POS(k, point_qty) + 32) * TMP(beam_qty[k]);
			offset1 += TMP(beam_qty[k]);
		}
		/* 获取数据 */
		/* 这里需要压缩数据 或者 插值数据 这里只有一个beam 同时最多处理256beam */
		for	(j = 0 ; j < TMP(beam_qty[i]); j++)
		{
				memcpy (TMP(measure_data[offset1 + j]), (void *)(pBeamData + offset +
							(GROUP_VAL_POS(i, point_qty) + 32) * j + GROUP_VAL_POS(i, point_qty)), 32);
		}
		// if the coming data is not stable , abandon
	}
	// 校准时， 同步当前BEAM
	RefreshCurrentBeamWhenCalibration() ;
    //闸门同步时，更新画图区
	GateSynchroRefresh()  ;

	calc_measure_data();//计算数据
	draw_field_value ();//显示

	// Draw A scan B scan C scan S scan in display buffer
	for (k = 0; ((k < 16) && (TMP(_scan_type[k]) != 0xff)); k++)
	{
		draw_scan(k, TMP(fb1_addr) , pBeamData);
	}
#endif

}

void ClearBackGround()
{
#if ARM
	unsigned char* pFB = (unsigned char*)(TMP(fb1_addr));
    memset(pFB , 0 , 768 * 400 * 2);
#endif
}
void RedrawCalc()
{
	int i ;
    pthread_mutex_lock(&draw_thread_mutex);
	//clear back ground
    ClearBackGround();
	memcpy((void*)TMP(_scan_type)   ,(void*)TMP(scan_type)   , 16) ;
	memcpy((void*)TMP(_scan_group)  ,(void*)TMP(scan_group)  , 16) ;
	memcpy((void*)TMP(_scan_xpos)   ,(void*)TMP(scan_xpos)   , 64) ;
	memcpy((void*)TMP(_scan_ypos)   ,(void*)TMP(scan_ypos)   , 64) ;
	memcpy((void*)TMP(_scan_width)  ,(void*)TMP(scan_width)  , 64) ;
	memcpy((void*)TMP(_scan_height) ,(void*)TMP(scan_height) , 64) ;
	pp->nEncoderType = get_inspec_source (pp->p_config)   ;
	for(i = 0 ; i < setup_MAX_GROUP_QTY ; i++)
	{
		pp->nPointQty[i] = GROUP_VAL_POS(i , point_qty) ;
	}
	// wait for data stable
	usleep(500000);
	pp->bRefreshDraw   = TRUE ;
	draw_frame_thread(0);
	pp->bRefreshDraw   = FALSE ;
    pthread_mutex_unlock(&draw_thread_mutex);
}



void DrawFrameBuffer()
{
	int i =  0  ;
	int _nCounter ;
	if(pp->bRefreshDraw)
	{
		RedrawCalc();
	}
	else
	{
		if(*DMA_MARK)
		{
			pthread_mutex_lock(&draw_thread_mutex);
			*DMA_MARK = 0 ;
		    _nCounter = DMA_MARK[1] ;
			i = (_nCounter + 3) & 0x00000003 ;
			if(i == DMA_MARK[2]) i = (i + 3) & 0x00000003 ;
			DMA_MARK[2] = i ;
			draw_frame_thread(i);
		    pthread_mutex_unlock(&draw_thread_mutex);
		}
		else if(TMP(freeze))
		{
			pthread_mutex_lock(&draw_thread_mutex);
			if(TMP(dataRecalling))
		    {//  记录数据重现时，重现数据全放在第一个缓冲
		    	i = 0 ;
		    }
		    else
		    {//  显示最后一次DMA的数据缓冲
				_nCounter = DMA_MARK[1] ;
				i = (_nCounter + 3) & 0x00000003 ;
				DMA_MARK[2] = i ;
		    }

			draw_frame_thread(i);
		    pthread_mutex_unlock(&draw_thread_mutex);
		    usleep(200000);
		}
	}

}

int CheckDiskVolume()
{
    FILE * fp;
    char buffer[80];
    int volume;
    fp=popen("df /dev/mmcblk0p2 -m | awk '$1==\"\/dev\/root\" {print $4}'", "r");
    fgets(buffer,sizeof(buffer),fp);
    volume = atoi(buffer);
    pclose(fp);
    return volume;
}

void SaveDataInThread()
{
    int volume, ret;
	pthread_mutex_lock(&draw_thread_mutex);
	if(!TMP(freeze)) FreezingFPGA(TRUE) ;
    volume = CheckDiskVolume();
    if (volume < 400) {
		gdk_threads_enter();
		dialogMessagePix(pp->window , getDictString(_STRING_DISK_VOLUME_LITTLE) , &color_red, 100);
		gdk_threads_leave();
    } else if (SaveDataAll()) {
		gdk_threads_enter();
		dialogMessage(pp->window , getDictString(_STRING_FILE_SAVE_ERROR) , &color_red) ;
		gdk_threads_leave();
        volume = CheckDiskVolume();
	} else if (volume < 1000) {
		gdk_threads_enter();
		dialogMessagePix(pp->window , getDictString(_STRING_DISK_VOLUME_THRESHOLD) , &color_blue, 100);
		gdk_threads_leave();
    } else {
		gdk_threads_enter();
		dialogMessage(pp->window , getDictString(_STRING_FILE_SAVE_FINISHED) , &color_blue) ;
		gdk_threads_leave();
	}
	pp->bSaveDataProcessing = FALSE ;

	if(!TMP(freeze)) FreezingFPGA(FALSE) ;
	pthread_mutex_unlock(&draw_thread_mutex);
	usleep(500000);
}

char g_cBuff[1024 * 192] ;

void TransferDataToPC()
{
#if ARM
	int i =  0  ;
	int _nCounter ;
	void* _pDataStart ; unsigned long _nDataSize  ;

	if(*DMA_MARK)
	{
		*DMA_MARK = 0 ;
		_nCounter = DMA_MARK[1] ;
		i = (_nCounter + 3) & 0x00000003 ;
		if(i == DMA_MARK[2]) i = (i + 3) & 0x00000003 ;
		DMA_MARK[2] = i ;
		_pDataStart = (void*)((pp->p_beam_data) + REGION_SIZE * i);
		_nDataSize  = pp->ScanInfor.DataSize  ;

		UdpSendBlocksToServer(_pDataStart , _nDataSize)  ;
	}
#else
	int i   ;
	unsigned char data ;
	static unsigned char g_nSendValue = 0;

	for(i = 0 ; i < 63 ; i++)
	{
		data = g_nSendValue + i ;
		memset(g_cBuff + i * 1024 , data , 1024);
	}
	g_nSendValue++ ;
	UdpSendBlocksToServer( g_cBuff , 1024*63)  ;
#endif

}

volatile int g_bNetTransferData = 0;
void signal_scan_thread(void)
{
	if(!pp->bRefreshDraw)
	{
		usleep(100000);
	}
	usleep(1000000);
	RedrawCalc();
	RefreshScanInfor();
	pp->bRefreshDraw = FALSE ;
	while(1)
	{
		if(pp->bSaveDataProcessing)
		{
			SaveDataInThread() ;
		}
		else
		{
			if(g_bNetTransferData)
				TransferDataToPC();
			else
				DrawFrameBuffer()  ;
		}
#if ARM
		usleep(1000);
#else
		usleep(50000);
#endif
	}
}

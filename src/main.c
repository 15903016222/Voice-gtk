/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include "drawui.h"
#include "drawfb.h"
#include "callback.h"
#include "spi_d.h"
#include "file_op.h"
#include "lzk/fileHandler.h"
#include "lzk/output_spi.h"
#include "ui/workpiece.h"
#include "ui/authorization.h"
#include "calculation/sizingcurve.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <gdk/gdkkeysyms.h>
#include "globalData.h"
#include "core/core.h"

#include "dev/dev.h"

volatile DRAW_UI_P	pp;
void init_group_spi (int group);
void group_spi_send (int group);
void MultiGroupSendFocalSpi();
void MultiGroupSendGroupSpi();
static int fd_lock ;
int isThisApplicationExisted()
{
	int ret = FALSE ;
	fd_lock = open(".lock" , O_RDWR );
	if(fd_lock == -1)
	{
		close(fd_lock);
		system("touch .lock");
		system("sync");
		fd_lock = open(".lock" , O_RDWR );
	}
	if(lockf(fd_lock , F_TLOCK , 1) == -1) ret = TRUE ;
	return ret;
}

/* config结构体的初始值 */
void set_config (unsigned int grp)
{
	gint i;
    GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	set_group_qty (pp->p_config, 1);
	pp->p_config->groupId = 0 ;
#if HIGH_POWER
	set_language (pp->p_config, LANGUAGE_CHINESE);
	GROUP_VAL_POS(nGroupId_ , group_mode) = UT1_SCAN;
#else
    GROUP_VAL_POS(grp , group_mode) = PA_SCAN;
#endif
	set_probe_select (pp->p_config, CHOOSE_PROBE);
	set_probe_fft (pp->p_config, NORMAL_OFF);
    GROUP_VAL_POS(grp , scan_offset)=0;
    GROUP_VAL_POS(grp , index_offset)=0;
    GROUP_VAL_POS(grp , skew)= 9000;
    GROUP_VAL_POS(grp , agate_start)=1000;
    GROUP_VAL_POS(grp , agate_width)=1000;

    set_part_thickness (grp , GUINT_TO_POINTER (50000));
    set_part_diameter (grp, GUINT_TO_POINTER (1000));
    set_part_material (grp, GUINT_TO_POINTER (1));
    set_part_weld (grp, GUINT_TO_POINTER (1));
	set_auto_detect (pp->p_config, NORMAL_OFF);
	set_auto_focal (pp->p_config, NORMAL_ON);
    set_voltage (pp->p_config, grp, VOLTAGE_LOW);
	/* UT settings */
	set_group_val (p_grp, GROUP_WEDGE_DELAY, 0);
    group_set_range(grp, 57022);
    group_set_start(grp, 0);
    group_set_gain(grp, 2000);
    group_set_refgain(grp, 0);
	set_group_val (p_grp, GROUP_VELOCITY, 324000);
    set_group_db_ref (pp->p_config, grp, NORMAL_OFF);

	set_group_val (p_grp, GROUP_PULSER, 1);
	set_group_val (p_grp, GROUP_RECEIVER, 1);

	set_group_val (p_grp, GROUP_FILTER_POS, 1);
	//set_group_val (p_grp, GROUP_FILTER, FILTER_AUTO);
	set_group_val (p_grp, GROUP_RECTIFIER, FULL_WAVE);
	set_group_val (p_grp, GROUP_AVERAGING, 0);
	set_group_val (p_grp, GROUP_VIDEO_FILTER, NORMAL_ON);
    group_set_rx_tx_mode(grp, PULSE_ECHO);
	set_group_val (p_grp, GROUP_FREQ_POS, 12);		/* 0是1Mhz 12AUserdef */
	set_group_val (p_grp, GROUP_FREQ_VAL, 5000);
	set_group_val (p_grp, GROUP_PW_POS, 0);			/* 0是Auto */
#if HIGH_POWER
	set_group_val (p_grp, GROUP_PW_VAL, 9000);
#else
	set_group_val (p_grp, GROUP_PW_VAL, 10000);
#endif
    GROUP_VAL_POS(0 , prf_pos1) = 3 ;
	GROUP_VAL_POS(0 , prf1) = 200 ;

    GROUP_VAL_POS(grp , point_qty)	= 605;			/* 0是Auto */
    GROUP_VAL_POS(grp , sum_gain)	= 400;			/* 0是Auto */
    GROUP_VAL_POS(grp , gate_pos)	= GATE_A;
    GROUP_GATE_VAL_POS( grp , GATE_A , start) = 0;
    GROUP_GATE_VAL_POS( grp , GATE_A , width) = 5000;
    GROUP_GATE_VAL_POS( grp , GATE_A , height) = 25; /*闸门默认高度为25*/
    GROUP_GATE_VAL_POS( grp , GATE_A , parameters) = 0; /*闸门默认高度为20*/
    GROUP_GATE_VAL_POS( grp , GATE_A , synchro)    = 0; /*闸门默认高度为20*/
    GROUP_GATE_VAL_POS( grp , GATE_B , start) = 2500;
    GROUP_GATE_VAL_POS( grp , GATE_B , width) = 2500;
    GROUP_GATE_VAL_POS( grp , GATE_B , height) = 20; /*闸门默认高度为20*/
    GROUP_GATE_VAL_POS( grp , GATE_B , parameters) = 0; /*闸门默认高度为20*/
    GROUP_GATE_VAL_POS( grp , GATE_B , synchro)    = 0; /*闸门默认高度为20*/
    GROUP_GATE_VAL_POS( grp , GATE_I , start) = 0;
    GROUP_GATE_VAL_POS( grp , GATE_I , width) = 0;
    GROUP_GATE_VAL_POS( grp , GATE_I , height)= 0;
    GROUP_GATE_VAL_POS( grp , GATE_A ,  measure) = 0  ;
    GROUP_GATE_VAL_POS( grp , GATE_B ,  measure) = 0  ;
    GROUP_GATE_VAL_POS( grp , GATE_I ,  measure) = 1  ;
	set_alarm_pos (pp->p_config, 0);
    set_alarm_groupa (pp->p_config, grp);
	set_alarm_conditiona (pp->p_config, ALARM_NONE);
	set_alarm_operator (pp->p_config, OPERATOR_AND);
    set_alarm_groupb (pp->p_config, grp);
	set_alarm_conditionb (pp->p_config, ALARM_NONE);

	set_output_pos (pp->p_config, OUTPUT1);
	for (i = 1; i < 17; i++) 
		set_output_alarm (pp->p_config, ALARM_OFF, i);
	set_output_count(pp->p_config,1);

    GROUP_VAL_POS(grp , selection)=0;   /*0 是 A-scan*/
    GROUP_VAL_POS(grp , per_reference)=1000.0;
    GROUP_VAL_POS(grp , per_measure)=1000.0;
    GROUP_VAL_POS(grp , u_reference)=1000.0;
    GROUP_VAL_POS(grp , u_measure)=1000.0;
    GROUP_VAL_POS(grp , s_reference)=1000.0;
    GROUP_VAL_POS(grp , s_measure)=100.0;
    GROUP_VAL_POS(grp , source)=0;   /*0是A^ */
    GROUP_VAL_POS(grp , min_thickness) = 500;
    GROUP_VAL_POS(grp , max_thickness) = 50000;

	set_reading_field1 (pp->p_config, 0);/*A%*/
	set_reading_field2 (pp->p_config, 31);/*DA*/
	set_reading_field3 (pp->p_config, 29);/*PA*/
	set_reading_field4 (pp->p_config, 33);/*SA*/

	pp->p_config->fieldIndex[0][0] = 0;
	pp->p_config->fieldIndex[0][1] = 31;
	pp->p_config->fieldIndex[0][2] = 29;
	pp->p_config->fieldIndex[0][3] = 33;
	pp->p_config->fieldIndex[1][0] = 0;
	pp->p_config->fieldIndex[1][1] = 50;
	pp->p_config->fieldIndex[1][2] = 51;
	pp->p_config->fieldIndex[1][3] = 53;//add by lzk

	pp->p_config->damping_pos_ut1 = 0;
	pp->p_config->damping_pos_ut2 = 0;
    GROUP_VAL_POS(grp , probe.Model[0]) = 32;

	set_display_table (pp->p_config, NORMAL_OFF);
	set_entry_image (pp->p_config, NORMAL_OFF);
	set_cur_entry (pp->p_config, 1);
	set_entry_qty (pp->p_config, 10);
	set_echo_qty (pp->p_config, 1);
	set_display_pos (pp->p_config, C_SCAN);
	set_cscan_source (pp->p_config, C_SCAN_A_HEIGHT, 0);

	set_display_pos (pp->p_config, S_SCAN);
	set_display_group(pp->p_config, DISPLAY_CURRENT_GROUP); /*0是All*/
	set_display_pos (pp->p_config, A_S_CC_SCAN);
	set_display_group(pp->p_config, DISPLAY_CURRENT_GROUP); /*0是All*/
	set_cscan_source (pp->p_config, C_SCAN_OFF, 0);

	set_display_pos (pp->p_config, A_C_CC_SCAN);
	set_display_group(pp->p_config, DISPLAY_CURRENT_GROUP); /*0是All*/
	set_cscan_source (pp->p_config, C_SCAN_A_HEIGHT, 0);
	set_cscan_source (pp->p_config, C_SCAN_OFF, 1);

	set_display_pos (pp->p_config, A_SCAN);
	set_display_group(pp->p_config, DISPLAY_CURRENT_GROUP); /*0是All*/

	set_stripscan_data1 (pp->p_config, STRIP_SCAN_A_HEIGHT);
	set_stripscan_data2 (pp->p_config, STRIP_SCAN_A_HEIGHT);
	set_stripscan_mode (pp->p_config, STRIP_SCAN_ALL_ASCAN);
	set_stripscan_disrange (pp->p_config, 100);
    GROUP_VAL_POS(grp , ut_unit)=2;  /* 2 是 true depth */
	set_overlay_grid(pp->p_config, 5);     /*5是 Off*/
	set_overlay_sizing_curves (pp->p_config, NORMAL_OFF);
	set_overlay_gate (pp->p_config, NORMAL_ON);
	set_overlay_cursor (pp->p_config, NORMAL_OFF);
	set_overlay_overlay (pp->p_config, NORMAL_ON);
	/**/
    GROUP_VAL_POS(grp , col_select_pos)=0;  /*0 Amplitude*/
    GROUP_VAL_POS(grp , col_start)   = 0.0;
    GROUP_VAL_POS(grp , col_end)     = 100.0;
    GROUP_VAL_POS(grp , col_contrast)= 0.0;
    GROUP_VAL_POS(grp , col_brightness)    = 50.0;
    GROUP_VAL_POS(grp , col_min)           = 5000.0;
    GROUP_VAL_POS(grp , col_max)           = 50000.0;
    GROUP_VAL_POS(grp , col_mode)    = 0;  /*0 Exclusion*/

	set_dis_prop_scan (pp->p_config, DIS_PROP_SCAN_A);/*0 A-Scan*/
    GROUP_VAL_POS(grp , ascan_color) = 3; 	/*3 Yellow*/
    GROUP_VAL_POS(grp , ascan_envelope)	=	0; /*0 None*/
    GROUP_VAL_POS(grp , ascan_appearance)	=	0; /*0 Hollow*/
    GROUP_VAL_POS(grp , ascan_overlay)	=	0; /*0 None*/
	set_dis_prop_boptimum (pp->p_config, NORMAL_OFF);
	set_dis_prop_cratio (pp->p_config, NORMAL_OFF);
	set_dis_prop_sinterpolation (pp->p_config, NORMAL_ON);
	set_dis_prop_strip_orientation (pp->p_config, NORMAL_OFF);

	set_fft_color (pp->p_config, FFT_BLUE);
    GROUP_VAL_POS(grp , skew_pos) = 1;
    GROUP_VAL_POS(grp , wedge.Orientation)    = 1 ;

    GROUP_VAL_POS(grp , probe.Model[0]) = 32;
    GROUP_VAL_POS(grp , probe.Elem_qty)  = 16  ;

    GROUP_VAL_POS(grp , pulser1 )         = 1;		/* 1~128 - elem_qty(聚焦阵元数最大为32) + 1 指定发射阵元 */
    GROUP_VAL_POS(grp , receiver1 )       = 1;		/* 接收阵元 必须是 PR 模式才能调节 */
    LAW_VAL_POS (grp , Tx_connect)	= 1;
    LAW_VAL_POS (grp , Rx_connect)	= 1;
    LAW_VAL_POS (grp , First_tx_elem)	= 1;
    LAW_VAL_POS (grp , First_rx_elem)	= 1;
    LAW_VAL_POS (grp , Last_tx_elem)	= 16;
    LAW_VAL_POS (grp , Last_rx_elem)	= 16;
    LAW_VAL_POS (grp , Elem_step)		=	1;
    LAW_VAL_POS (grp , Elem_qty)	    = 16;
    LAW_VAL_POS (grp , Wave_type)		=	LONGITUDINAL_WAVE;
    LAW_VAL_POS (grp , Angle_min)		=   3000;
    LAW_VAL_POS (grp , Angle_max)		=	6000;
    LAW_VAL_POS (grp , Angle_step)	=	100;
    LAW_VAL_POS (grp , Angle_beam_skew_min)		=	0;
    LAW_VAL_POS (grp , Angle_beam_skew_max)		=	0;
    LAW_VAL_POS (grp , Angle_beam_skew_step)		=	100;
    LAW_VAL_POS (grp , Focus_depth)	=	5000;
    LAW_VAL_POS (grp , Position_start)	=	30000;
    LAW_VAL_POS (grp , Position_end)	=	50000;
    LAW_VAL_POS (grp , Position_step)	=	1000;
    LAW_VAL_POS (grp , Offset_start)	=	2000;
    LAW_VAL_POS (grp , Offset_end)	=	2000;
    LAW_VAL_POS (grp , Depth_start)	=	2000;
    LAW_VAL_POS (grp , Depth_end)	    =	2000;
    LAW_VAL_POS (grp , Focal_type)	=   AZIMUTHAL_SCAN;
    LAW_VAL_POS (grp , Focal_point_type)	= DEPTH_P;

    LAW_VAL_POS (grp , law_index_start)= 0;
    LAW_VAL_POS (grp , law_index_end)	= 1;

	set_cur_encoder (pp->p_config, ENCODER_1);
	set_enc_type (pp->p_config,  3 , 0);
	set_enc_type (pp->p_config,  3 , 1);
	set_enc_resolution (pp->p_config, 48000, get_cur_encoder (pp->p_config));
	set_enc_origin (pp->p_config, 0, get_cur_encoder (pp->p_config));

	set_inspec_type (pp->p_config, TYPE_ONE_LINE);
	set_inspec_source (pp->p_config, SCAN_TIME);
	set_inspec_index (pp->p_config, INDEX_AUTO);
	set_inspec_rpmspeed (pp->p_config, 10);
	set_inspec_indexspeed (pp->p_config, 10);

	set_area_scanstart (pp->p_config, 0);
	set_area_scanend (pp->p_config, 800000);//346);
	set_area_scanresolution (pp->p_config, 1000);
	set_inspec_speed (pp->p_config, get_area_scanresolution (pp->p_config) * GROUP_VAL_POS(0, prf1) / 10);
	set_area_indexstart (pp->p_config, 0);
	set_area_indexend (pp->p_config, 800000);
	set_area_indexresolution (pp->p_config, 1000);

	set_start_mode (pp->p_config, RESET_ALL);

	set_data_storage (pp->p_config, STORAGE_LAST);
	set_data_inspec_data (pp->p_config, INSPEC_DATA_ALL_A_C);

	set_file_storage (pp->p_config, STORAGE_CARD);
	set_file_save_mode (pp->p_config, SAVE_MODE_INSPEC_DATA);

	set_report_format_userfield (pp->p_config, NORMAL_ON);
	set_report_format_probe (pp->p_config, NORMAL_ON);
	set_report_format_setup (pp->p_config, NORMAL_ON);
	set_report_format_note (pp->p_config, NORMAL_ON);
	set_report_format_view (pp->p_config, FILE_VIEW_TABLE);

	set_report_userfield_select (pp->p_config, 0);
	set_report_userfield_enable (pp->p_config, FALSE, 0);
	set_report_userfield_enable (pp->p_config, FALSE, 1);
	set_report_userfield_enable (pp->p_config, FALSE, 2);
	set_report_userfield_enable (pp->p_config, FALSE, 3);
	set_report_userfield_enable (pp->p_config, FALSE, 4);
	set_report_userfield_enable (pp->p_config, FALSE, 5);
	set_report_userfield_enable (pp->p_config, FALSE, 6);
	set_report_userfield_enable (pp->p_config, FALSE, 7);
	set_report_userfield_enable (pp->p_config, FALSE, 8);
	set_report_userfield_enable (pp->p_config, FALSE, 9);
	set_report_userfield_label (pp->p_config, "Author", 0);
	set_report_userfield_content (pp->p_config, "Content", 0);

    set_part_geometry (grp, GUINT_TO_POINTER (PLATE_PART));
	set_report_template (pp->p_config, REPORT_COMPLETE);
	set_report_paper_size (pp->p_config, PAPER_A4);

	for(i=0; i < 256; i++)
	{
        GROUP_VAL_POS(grp , gain_offset[i]) = 0;
	}

	set_output_pos (pp->p_config, 2);
	set_output_holdtime(pp->p_config,100000);
	set_output_pos (pp->p_config, 1);
	set_output_holdtime(pp->p_config,100000);
	set_output_pos (pp->p_config, 0);
	set_output_holdtime(pp->p_config,100000);

	pp->p_config->bright = 50 ;
    GROUP_VAL_POS(grp , VelocityCalibrated) = 0;
    GROUP_VAL_POS(grp , WedgeDelayCalibrated) = 0 ;
    GROUP_VAL_POS(grp , SensationCalibrated) =  0  ;
    GROUP_VAL_POS(grp , part.weldtype_pos )  =  0 ;
    memset ((void*)(GROUP_VAL_POS(grp , nReferenceData)), 0, 32);

	strcpy(pp->p_config->file_name_inspection_data , "InspectData");
	strcpy(pp->p_config->file_name_inspection_table , "InspectTable");
	strcpy(pp->p_config->file_name_screen , "Screen");
	strcpy(pp->p_config->file_name_report , "Report");

    GROUP_VAL_POS(grp , SizingCurves.mode_pos)= 0;				 /* 0是Setup */
    GROUP_VAL_POS(grp , SizingCurves.curve_pos)= 0;			/* 0是NOne */
    InitSizingCurveParameters(grp);
}

void SettingGroupConfigure (int grp)
{
	int i;
    GROUP *p_grp = get_group_by_id (pp->p_config, grp);
    GROUP_VAL_POS(grp , group_mode) = PA_SCAN;

    GROUP_VAL_POS(grp , scan_offset)=0;
    GROUP_VAL_POS(grp , index_offset)=0;
    GROUP_VAL_POS(grp , skew)=9000;
    GROUP_VAL_POS(grp , agate_start)=1000;
    GROUP_VAL_POS(grp , agate_width)=1000;
    set_voltage (pp->p_config, grp, VOLTAGE_LOW);
	/* UT settings */
	set_group_val (p_grp, GROUP_WEDGE_DELAY, 0);
    group_set_range(grp, 57022);
    group_set_start(grp, 0);
    group_set_gain(grp, 2000);
    group_set_refgain(grp, 0);
	set_group_val (p_grp, GROUP_VELOCITY, 324000);
    set_group_db_ref (pp->p_config, grp, NORMAL_OFF);
	set_group_val (p_grp, GROUP_PULSER, 1);
	set_group_val (p_grp, GROUP_RECEIVER, 1);
	set_group_val (p_grp, GROUP_FILTER_POS, 1);

	set_group_val (p_grp, GROUP_RECTIFIER, FULL_WAVE);
	set_group_val (p_grp, GROUP_AVERAGING, 0);
	set_group_val (p_grp, GROUP_VIDEO_FILTER, NORMAL_ON);
    group_set_rx_tx_mode(grp, PULSE_ECHO);
	set_group_val (p_grp, GROUP_FREQ_POS, 12);		/* 0是1Mhz 12AUserdef */
	set_group_val (p_grp, GROUP_FREQ_VAL, 5000);
	set_group_val (p_grp, GROUP_PW_POS, 0);			/* 0是Auto */
#if HIGH_POWER
	set_group_val (p_grp, GROUP_PW_VAL, 9000);
#else
	set_group_val (p_grp, GROUP_PW_VAL, 10000);
#endif

    GROUP_VAL_POS(grp , point_qty)	= 605;			/* 0是Auto */
    GROUP_VAL_POS(grp , sum_gain)	= 400;			/* 0是Auto */
    GROUP_VAL_POS(grp , gate_pos)	= GATE_A;
    GROUP_GATE_VAL_POS( grp , GATE_A , start) = 0;
    GROUP_GATE_VAL_POS( grp , GATE_A , width) = 5000;
    GROUP_GATE_VAL_POS( grp , GATE_A , height) = 25; /*闸门默认高度为20*/
    GROUP_GATE_VAL_POS( grp , GATE_A , parameters) = 0; /*闸门默认高度为20*/
    GROUP_GATE_VAL_POS( grp , GATE_A , synchro)    = 0; /*闸门默认高度为20*/
    GROUP_GATE_VAL_POS( grp , GATE_B , start) = 2500;
    GROUP_GATE_VAL_POS( grp , GATE_B , width) = 2500;
    GROUP_GATE_VAL_POS( grp , GATE_B , height) = 20; /*闸门默认高度为20*/
    GROUP_GATE_VAL_POS( grp , GATE_B , parameters) = 0; /*闸门默认高度为20*/
    GROUP_GATE_VAL_POS( grp , GATE_B , synchro)    = 0; /*闸门默认高度为20*/
    GROUP_GATE_VAL_POS( grp , GATE_I , start) = 0;
    GROUP_GATE_VAL_POS( grp , GATE_I , width) = 0;
    GROUP_GATE_VAL_POS( grp , GATE_I , height)= 0; /*闸门默认高度为20*/
    GROUP_GATE_VAL_POS( grp , GATE_A ,  measure) = 0  ;
    GROUP_GATE_VAL_POS( grp , GATE_B ,  measure) = 0  ;
    GROUP_GATE_VAL_POS( grp , GATE_I ,  measure) = 1  ;

    GROUP_VAL_POS(grp , selection)=0;   /*0 是 A-scan*/
    GROUP_VAL_POS(grp , per_reference)=1000.0;
    GROUP_VAL_POS(grp , per_measure)=1000.0;
    GROUP_VAL_POS(grp , u_reference)=1000.0;
    GROUP_VAL_POS(grp , u_measure)=1000.0;
    GROUP_VAL_POS(grp , s_reference)=1000.0;
    GROUP_VAL_POS(grp , s_measure)=100.0;
    GROUP_VAL_POS(grp , source)=0;   /*0是A^ */
    GROUP_VAL_POS(grp , min_thickness) = 500;
    GROUP_VAL_POS(grp , max_thickness) = 50000;
    GROUP_VAL_POS(grp , probe.Model[0]) = 32;
    GROUP_VAL_POS(grp , probe.Elem_qty)  = 16  ;

    GROUP_VAL_POS(grp , ut_unit)=2;  /* 2 是 true depth */
    GROUP_VAL_POS(grp , col_select_pos)=0;  /*0 Amplitude*/
    GROUP_VAL_POS(grp , col_start)   = 0.0;
    GROUP_VAL_POS(grp , col_end)     = 100.0;
    GROUP_VAL_POS(grp , col_contrast)= 0.0;
    GROUP_VAL_POS(grp , col_brightness)    = 50.0;
    GROUP_VAL_POS(grp , col_min)           = 5000.0;
    GROUP_VAL_POS(grp , col_max)           = 50000.0;
    GROUP_VAL_POS(grp , col_mode)    = 0;  /*0 Exclusion*/
    GROUP_VAL_POS(grp , ascan_color) = 3; 	/*3 Yellow*/
    GROUP_VAL_POS(grp , ascan_envelope)	=	0; /*0 None*/
    GROUP_VAL_POS(grp , ascan_appearance)	=	0; /*0 Hollow*/
    GROUP_VAL_POS(grp , ascan_overlay)	=	0; /*0 None*/
    GROUP_VAL_POS(grp , skew_pos) = 1;
	/* 探头信息 */
    GROUP_VAL_POS(grp , wedge.Orientation)    = 1 ;
    GROUP_VAL_POS(grp , pulser1 )         = 1;		/* 1~128 - elem_qty(聚焦阵元数最大为32) + 1 指定发射阵元 */
    GROUP_VAL_POS(grp , receiver1 )       = 1;		/* 接收阵元 必须是 PR 模式才能调节 */
    GROUP_VAL_POS(grp , part.Material_pos) = 1;
    LAW_VAL_POS (grp , Tx_connect)	= 1;
    LAW_VAL_POS (grp , Rx_connect)	= 1;
    LAW_VAL_POS (grp , First_tx_elem)	= 1;
    LAW_VAL_POS (grp , First_rx_elem)	= 1;
    LAW_VAL_POS (grp , Last_tx_elem)	= 16;
    LAW_VAL_POS (grp , Last_rx_elem)	= 16;
    LAW_VAL_POS (grp , Elem_step)		=	1;
    LAW_VAL_POS (grp , Elem_qty)	    = 16;
    LAW_VAL_POS (grp , Wave_type)		=	LONGITUDINAL_WAVE;
    LAW_VAL_POS (grp , Angle_min)		=   3000;
    LAW_VAL_POS (grp , Angle_max)		=	5000;
    LAW_VAL_POS (grp , Angle_step)	=	100;
    LAW_VAL_POS (grp , Angle_beam_skew_min)		=	0;
    LAW_VAL_POS (grp , Angle_beam_skew_max)		=	0;
    LAW_VAL_POS (grp , Angle_beam_skew_step)		=	100;
    LAW_VAL_POS (grp , Focus_depth)	=	5000;
    LAW_VAL_POS (grp , Position_start)	=	30000;
    LAW_VAL_POS (grp , Position_end)	=	50000;
    LAW_VAL_POS (grp , Position_step)	=	1000;
    LAW_VAL_POS (grp , Offset_start)	=	2000;
    LAW_VAL_POS (grp , Offset_end)	=	2000;
    LAW_VAL_POS (grp , Depth_start)	=	2000;
    LAW_VAL_POS (grp , Depth_end)	    =	20000;
    LAW_VAL_POS (grp , Focal_type)	=   AZIMUTHAL_SCAN;
    LAW_VAL_POS (grp , Focal_point_type)	= DEPTH_P;

    LAW_VAL_POS (grp , law_index_start)= 0;
    LAW_VAL_POS (grp , law_index_end)	= 1;

	for(i=0; i < 256; i++)
	{
        GROUP_VAL_POS(grp , gain_offset[i]) = 0;
	}
    GROUP_VAL_POS(grp , VelocityCalibrated) = 0  ;
    GROUP_VAL_POS(grp , WedgeDelayCalibrated) = 0 ;
    GROUP_VAL_POS(grp , SensationCalibrated) =  0  ;

    memset ((void*)(GROUP_VAL_POS(grp , nReferenceData)), 0, 32);

    GROUP_VAL_POS(grp , SizingCurves.mode_pos)= 0;				 /* 0是Setup */
    GROUP_VAL_POS(grp , SizingCurves.curve_pos)= 0;			/* 0是NOne */
    InitSizingCurveParameters(grp);
}

/* 设置config结构体之外的初始值  */
static void set_init_para()
{
	int i;
	pp->ctype_pos = 1;
	pp->cmode_pos = 2;
	pp->cstart_qty = 1;
	pp->count = 0;
	pp->clb_flag = 0;
	pp->clb_encoder = 0;
	pp->key_flag = 0;
	pp->keyboard_xpos = 30;
	pp->keyboard_ypos = 290;
	pp->thickness_refract_times = 0 ;

	pp->ref_amplitude = 8000;
	pp->tolerance_t = 500;
	pp->tolerance = 200;
	pp->distance = 10000.0;

	pp->radius1 =  50000 ;
	pp->radius2 =  100000;
	pp->depth1  =   25000;
	pp->depth2  =   50000;
	pp->thickness1 = 25000;
	pp->thickness2 = 50000;
	set_display_table (pp->p_config, 0);
	//TMP_CBA(WedgeSeparation) = 80 ; //mm
	//TMP_CBA(DistancePCS) = 80 ;
	TMP_CBA(bNewPoint)   = 0  ;
	TMP(velocity_data_p[0]) =  TMP(velocity_data[0]);
	TMP(velocity_data_p[1]) =  TMP(velocity_data[1]);
	TMP(velocity_data_p[2]) =  TMP(velocity_data[2]);

	for(i=0; i < 256; i++)
	{
		pp->gate_a_start[0][i]	=  GROUP_VAL_POS(0 , gate[0].start) / 10 ;
		pp->gate_a_end[0][i]	= (GROUP_VAL_POS(0 ,  gate[0].start) + GROUP_VAL_POS(0 ,  gate[0].width)) / 10;
	}
	pp->wstart_qty = 1;

	/*各步进初始值*/
	TMP(db_reg)=2;
	TMP(start_reg)=1;
	TMP(range_reg)=0;
	TMP(wedge_delay_reg)=1;
	TMP(velocity_reg)=3;
	TMP(pulser_reg)=0;
	TMP(frequency_reg)=2;
	TMP(prf_reg)=1;
	TMP(reject_reg)=0;
	TMP(gain_offset_reg)=2;
	TMP(beam_delay_reg)=0;
	TMP(point_qty_reg)=1;
	TMP(sum_gain_reg)=2;

	TMP(agate_start_reg)=2;
	TMP(gate_width_reg)=2;
	TMP(agate_height_reg)=0;
	TMP(count_reg)=0;
	TMP(active_delay_reg)=2;
	TMP(holdtime_reg)=2;
	TMP(ref_ampl_reg)=1;
	TMP(ref_ampl_offset_reg)=2;
	TMP(curve_step_reg)=2;
	TMP(ref_gain_reg)=2;
	TMP(position_reg)=2;
	TMP(mat_atten_reg)=1;
	TMP(delay_reg)=1;
	TMP(per_reference_reg)=1;
	TMP(per_measure_reg)=1;
	TMP(u_reference_reg)=2;
	TMP(u_measure_reg)=2;
	TMP(s_reference_reg)=2;
	TMP(s_measure_reg)=2;
	TMP(i_reference_reg)=2;
	TMP(i_measure_reg)=2;
	TMP(s_refmeas_reg)=2;
	TMP(cursors_angle_reg)=0;
	TMP(cursors_amplitude_reg)=1;
	TMP(cursors_ut_reg)=1;
	TMP(cursors_scan_reg)=2;	/* cursors_scan  步进 */
	TMP(cursors_index_reg)=2;	/*cursors_index步进*/
	TMP(min_thickness_reg)=0;	/*min_thickness步进*/
	TMP(max_thickness_reg)=0;	/*max_thickness步进*/

	TMP(echo_qty_reg)=0;		/*echo_qty步进*/
	TMP(dis_range_reg)=1;
	TMP(avg_scan_speed_reg)=1;
	TMP(color_contrast_reg)=0;	/* Display -> Color -> contrast  */
	TMP(brightness_reg)=0;	/* Display -> Color -> brightness*/
	TMP(color_start_reg)=0;	/* Display -> Color -> Start步进  */
	TMP(color_end_reg)=0;		/* Display -> Color -> end步进    */
	TMP(min_reg)=0;			/* Display -> Color -> min       */
	TMP(max_reg)=0;			/* Display -> Color -> max       */
	TMP(compress_reg)=0;		/* Display -> Properties -> compress */

	TMP(scanoffset_reg)=1;
	TMP(indexoffset_reg)=1;
	TMP(skew_reg)=1;
	TMP(part_thickness_reg)=0;	/* Probe/Part -> parts -> thickness步进*/
	TMP(diameter_reg)=1;		/* Probe/Part -> parts -> diameter步进*/
	TMP(weland_height_reg)=0;
	TMP(weland_offset_reg)=0;
	TMP(fizone_height_reg)=0;
	TMP(fizone_angle_reg)=0;
	TMP(fizone_radius_reg)=0;

	TMP(connection_P_reg)=0;	/* Focal_Law -> configuration -> connection P*/
	TMP(connection_R_reg)=0;	/*Focal_Law -> configuration -> connection R*/

	TMP(element_qty_reg)=0;	/* Focal_Law -> aperture -> element qty*/
	TMP(first_element_reg)=0;	/* Focal_Law -> aperture -> first element*/
	TMP(last_element_reg)=0;	/* Focal_Law -> aperture -> last_element*/
	TMP(element_step_reg)=0;	/* Focal_Law -> aperture -> element_step*/

	TMP(min_angle_reg)=1;		
	TMP(max_angle_reg)=1;	
	TMP(positions_reg)=2;
	TMP(offsets_reg)=1;
	TMP(positione_reg)=1;	
	TMP(positionstep_reg)=1;
	TMP(depths_reg)=1;
	TMP(depthe_reg)=1;
	TMP(offsete_reg)=1;		
	TMP(angle_step_reg)=1;
	TMP(beam_skew_min_angle_reg)=1;
	TMP(beam_skew_max_angle_reg)=1;
	TMP(beam_skew_angle_step_reg)=1;	

	TMP(encoder_resolution_reg)=1;	/* Scan -> Encoder -> resolution  */
	TMP(origin_reg)=1;
	TMP(scanspeed_reg)=0;
	TMP(scanspeed_rpm_reg)=0;
	TMP(indexspeed_reg)=0;

	TMP(scan_start_reg)=0;	/* Scan -> Area -> Scan start*/
	TMP(scan_end_reg)=0;		/* Scan -> Area -> Scan end*/
	TMP(scan_resolution_reg)=2;	/* Scan -> Area -> Scan resolution*/

	TMP(index_start_reg)=0;	/* Scan -> Area -> index_start*/
	TMP(index_end_reg)=0;	/* Scan -> Area -> index_end*/
	TMP(index_resolution_reg)=2;	/* Scan -> Area -> index_resolution*/

	TMP(bright_reg)=0;		/*preferences -> pref. -> bright*/
	TMP(distance_reg)=1;		/*preferences -> pref. -> bright*/

	TMP(WedgeSeparation_reg)=1;
	TMP(nDataSavePlace) = 0 ;
	set_start_pause (pp->p_config, 0);
	pp->bGateSynchro  = IsGateSynchro()  ;

	TMP(bUSBStrorage)  = 0 ;
	TMP(dataRecalling) = 0 ;
	pp->bRefreshDraw   = 0 ;

    workpiece_init();   /*初始化工件模块*/

	int k;
	for(k=0;k<4;k++)//4个field
	{
		TMP(field[0][k]) =  MEASURE_DATA_ND;
		TMP(field[1][k]) =  MEASURE_DATA_ND;
	}
}

void init_beam_qty()
{
	int i;
	int tmp;
	for(i = 0; i< get_group_qty (pp->p_config); i++ )
	{
		if (LAW_VAL_POS (i, Focal_type) == AZIMUTHAL_SCAN || DEPTH_SCAN == LAW_VAL_POS (i, Focal_type) )
		{
			tmp = (LAW_VAL_POS (i, Angle_max) - LAW_VAL_POS (i, Angle_min)) /
				LAW_VAL_POS (i, Angle_step) + 1;

		}
		else if(LAW_VAL_POS (i, Focal_type) == LINEAR_SCAN)
		{
			tmp = (gint)( ( LAW_VAL_POS (i, Last_tx_elem) - LAW_VAL_POS (i, First_tx_elem) - LAW_VAL_POS (i, Elem_qty) + 1 ) /
					LAW_VAL_POS (i, Elem_step) ) + 1;
		}
		else if(LAW_VAL_POS (i, Focal_type) == STATIC_SCAN)
		{
			tmp = 1 ;
		}
		TMP(beam_qty[i]) = tmp;
	}
}

unsigned int GetChannelEnable()
{
	unsigned int _nResult = 0;
	unsigned int _nTmpResult = 0 ;
	unsigned char _nGroupMode ;
	int i , j ;
	int _nGroupQty = get_group_qty(pp->p_config) ;
	for(i = 0 ; i < _nGroupQty ; i++)
	{
		_nGroupMode = GROUP_VAL_POS(i , group_mode);
		if(_nGroupMode == PA_SCAN)
		{
			if(LAW_VAL_POS (i, Focal_type) == AZIMUTHAL_SCAN )
			{
				for(j = LAW_VAL_POS (i, First_tx_elem) ; j < LAW_VAL_POS (i, First_tx_elem) + LAW_VAL_POS (i, Elem_qty) ; j++)
				{
					_nTmpResult = _nTmpResult | (1 << ((j - 1)%32) ) ;
				}
			}
			else
			{
				for(j = LAW_VAL_POS (i, First_tx_elem) ; j < LAW_VAL_POS (i, Last_tx_elem); j++)
				{
					_nTmpResult = _nTmpResult | (1 << ((j - 1) % 32) ) ;
				}
			}
		}

		if(_nGroupMode == UT_SCAN)
		{
			j = LAW_VAL_POS (i, First_tx_elem) ;
			_nTmpResult = _nTmpResult | (1 << ((j - 1) % 32) ) ;
		}
		_nResult = _nResult | _nTmpResult ;
	}

	return _nResult ;
}

void FocallawCalculation()
{
	int i;
	for(i = 0 ; i < get_group_qty(pp->p_config) ; ++i)
	{
		generate_focallaw (i);
		if(	!GROUP_VAL_POS(i , WedgeDelayCalibrated) )
			RefreshBeamDelay(i);
		if(	!GROUP_VAL_POS(i , SensationCalibrated) )
			RefreshGainOffset(i);
	}
}

void MainInit ()
{
	DRAW_UI_P		p_ui   = NULL;
	CONFIG			*p_config  = NULL;
	TMP_CONFIG_P	p_tmp_config = NULL;
	GtkWidget		*window  ;
	GdkColor		color_black_     = {0x0, 0x0, 0x0, 0x0};

	g_type_class_unref (g_type_class_ref (GTK_TYPE_IMAGE_MENU_ITEM));
	g_object_set (gtk_settings_get_default (), "gtk-menu-bar-accel", NULL, NULL);
	/* 封装后需要条用函数初始化 */
	p_ui		= (DRAW_UI_P)malloc(sizeof(DRAW_UI));
	p_tmp_config	= (TMP_CONFIG_P)malloc(sizeof(TMP_CONFIG));

	if (!p_ui || !p_tmp_config)	return ;

	memset (p_ui, 0x0, sizeof(DRAW_UI));
	memset (p_tmp_config, 0x0, sizeof(TMP_CONFIG));

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_decorated (GTK_WINDOW (window), FALSE);			/*不可以装饰*/
	gtk_window_set_default_size (GTK_WINDOW(window), 800, 600);		/*设置窗口大小*/
#if ARM
	gtk_window_fullscreen (GTK_WINDOW(window));						/*全屏幕*/
#endif
	gtk_widget_modify_bg (window, GTK_STATE_NORMAL, &color_black_);	/*黑色背景*/
	g_signal_connect (G_OBJECT(window), "delete_event",	G_CALLBACK(gtk_main_quit), NULL);
	/* 把屏幕按键函数改成内核驱动代发 */
	p_ui->disp = XOpenDisplay(NULL);
	if (p_ui->disp == NULL)   return;

	p_ui->fk = fakekey_init(p_ui->disp);

	p_ui->p_tmp_config	= p_tmp_config;
	p_ui->window		= window;
	pp = p_ui;

#if ARM
    core_init();
	init_fb ();
	init_mem ();
	init_spi ();
	init_serial ();
    group_spi_init();
	p_ui->p_beam_data = TMP(dma_data_add); // beam data from FPGA
	p_config = (CONFIG*)(TMP(dma_data_add) + 8 *1024 * 1024);
#else
	p_config = (CONFIG *)malloc(sizeof(CONFIG));
	if (!p_config)	return ;
	memset (p_config, 0x0, sizeof(CONFIG));
#endif
	p_ui->p_config		= p_config;
	g_debug("p_config:%p" ,p_config);

	//add by lzk
	globalDataInit();
	// load default setup file
	if(memcmp("abcdefgh" ,p_config->cfg_end ,8))
	{
		//共享内存无数据
		if(readConfigFile("/home/tt/TT/source/user/setup/default.cfg"))
		{
			//load失败
			memset((void*)(pp->p_config) , 0 , sizeof(CONFIG)) ;
			set_config(0);
		}
#if HIGH_POWER
		else
		{//load成功
			//大功率只有UT1,1个组
			if((1 != get_group_qty(pp->p_config))
					&& (UT1_SCAN != GROUP_VAL_POS(0 , group_mode)))
			{
				set_config(0);
			}
			set_language (pp->p_config, LANGUAGE_CHINESE);
		}
#endif
	}
	//add by lzk
	globalDataInitAfterReadCfg();
	set_init_para();//
	LoadPalete() ;
	memset (TMP(scan_type), 0xff, 16);
	init_beam_qty();  // for the virtual focal law needs to initialize the beam number at start

	set_bright (pp->p_config , pp->p_config->bright )  ;
	// draw the user interface
	init_ui (p_ui);
    //******************************
    FocallawCalculation();
	//add this function temporarily
	//for the menu 3 show not refresh
	draw_menu3(1, NULL);
	gtk_widget_show (window);
	// init scan parameters
	draw_area_all() ;

	output_init(GetChannelEnable()) ;
	MultiGroupSendGroupSpi();
	MultiGroupSendFocalSpi();

	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;
    StartPause();
    StartPause();
}

int main (int argc, char *argv[])
{
    // check application is already running!
	if(isThisApplicationExisted())
	{
		g_debug ("APPLICATION IS ALREADY RUNNING\n");
#if ARM
		return 0;
#endif
	}
#if !GLIB_CHECK_VERSION(2, 32, 0)
	g_thread_init(NULL);
#endif
	gdk_threads_init();
	gtk_init (&argc, &argv);

    dev_init();

    SetSoftwareVersion();

    MainInit();

#if ARM
	system("killall psplash");
#endif
	gdk_threads_enter();
	gtk_main();
	gdk_threads_leave();
	return 0;
}


void ExitApplication (GtkWidget *widget, GdkEventButton *event,	gpointer data)
{
	saveConfigFile("/home/tt/TT/source/user/setup/default.cfg");
	g_debug ("shut down normally!\n");
	lockf(fd_lock ,F_ULOCK , 1 );
	close(fd_lock);
	sync();
	int _nTmp = (int)data ;
#if ARM
	int i;
	// tell single chip to shut down power
	unsigned char key = 0xaa;
	if(_nTmp)
	{
		i = write(pp->fd_key, &key,1);
		i = write(pp->fd_key, &key,1);
		i = write(pp->fd_key, &key,1);
	}
	close(pp->fd_key);
	close(pp->fd_key1);
	FreezingFPGA(TRUE) ;
#endif
	if(!_nTmp)
	{
		gtk_main_quit();
	}

}

void send_focal_spi (int group, int reset)
{
	guint offset, beam_qty =TMP(beam_qty[group]), k,kk, i,enablet = 0, enabler = 0;
	guint tmp_tx, tmp_rx, index_tx, index_rx, channel_index_num_tx, channel_index_num_rx,cnt;
	GROUP *p_grp = get_group_by_id (pp->p_config, group);

	int beam_sum = get_beam_qty();
	unsigned char pulser, receiver;

	for (offset = 0, k = 0 ; k < group; k++)
		offset += TMP(beam_qty[k]);
	for (k = offset , kk = 0; k < offset + beam_qty; k++ , kk++)
	{ 
        if(group_get_mode(group) == PA_SCAN) {
			TMP(focal_spi[k]).group	= group;
			TMP(focal_spi[k]).all_beam_info	= beam_sum  - 1;
			TMP(focal_spi[k]).gain_offset	= GROUP_VAL_POS(group , gain_offset[kk]);
			TMP(focal_spi[k]).beam_delay	= GROUP_VAL_POS(group , beam_delay[kk]) / 10;
			TMP(focal_spi[k]).gate_a_start  = pp->gate_a_start[group][kk];
			TMP(focal_spi[k]).gate_a_end    = pp->gate_a_end[group][kk]  ;
			TMP(focal_spi[k]).gate_b_start  = pp->gate_b_start[group][kk];
			TMP(focal_spi[k]).gate_b_end    = pp->gate_b_end[group][kk]  ;
			TMP(focal_spi[k]).gate_i_start  = pp->gate_i_start[group][kk];
			TMP(focal_spi[k]).gate_i_end    = pp->gate_i_end[group][kk]  ;
			/*UT Settings->Pulser->Tx/Rx mode*/		
            if (group_get_rx_tx_mode(group) == PULSE_ECHO )/*单个探头收发模式*/
			{
				pulser   =  GROUP_VAL_POS(group , pulser1) ;//get_group_val ( &pp->p_config->group[group], GROUP_PULSER );
				receiver = pulser;
			}
            else// if (group_get_rx_tx_mode(group) == PITCH_CATCH)
			{
				pulser   = GROUP_VAL_POS(group , pulser1) ;  //get_group_val ( &pp->p_config->group[group], GROUP_PULSER );
				receiver = GROUP_VAL_POS(group , receiver1) ; //get_group_val ( &pp->p_config->group[group], GROUP_RECEIVER );
			}
	
			if(LAW_VAL_POS(group, Focal_type) == 1)/*Linear*/
			{
				TMP(focal_spi[k]).tx_sel	= 
					channel_select(
							(guint)(pulser) + (guint)(LAW_VAL_POS(group, First_tx_elem))+ kk * LAW_VAL_POS(group, Elem_step)-1 );
				TMP(focal_spi[k]).rx_sel	= 
					channel_select(
							(guint)(receiver) + (guint)(LAW_VAL_POS(group, First_tx_elem))+ kk * LAW_VAL_POS(group, Elem_step)-1 );
				tmp_tx = (guint) (pulser)     + (guint)(LAW_VAL_POS(group, First_tx_elem) + kk * LAW_VAL_POS(group, Elem_step))-1 ;
				tmp_rx = (guint) (receiver)   + (guint)(LAW_VAL_POS(group, First_tx_elem) + kk * LAW_VAL_POS(group, Elem_step))-1 ;
			}
			else
			{
				TMP(focal_spi[k]).tx_sel	= 
					channel_select( (guint)(pulser)   + (guint)(LAW_VAL_POS(group, First_tx_elem))-1 ); 
				TMP(focal_spi[k]).rx_sel	= 
					channel_select( (guint)(receiver) + (guint)(LAW_VAL_POS(group, First_tx_elem))-1 );
				tmp_tx = (guint) (pulser)   + (guint)(LAW_VAL_POS(group, First_tx_elem)) - 1;
				tmp_rx = (guint) (receiver) + (guint)(LAW_VAL_POS(group, First_tx_elem)) - 1;
			}
			channel_index_num_tx = (tmp_tx % 32 == 0)? 31 : (tmp_tx % 32 -1); 
			channel_index_num_rx = (tmp_rx % 32 == 0)? 31 : (tmp_rx % 32 -1); 
			cnt = pow(2,LAW_VAL_POS(group, Elem_qty))-1; 
			for (i = 0;i < TMP(focal_law_all_beam[group][kk].N_ActiveElements); i++)
			{
				index_tx = (channel_index_num_tx + i) & 0x0000001f; 
				index_rx = (channel_index_num_rx + i) & 0x0000001f; 
				TMP(focal_spi[k]).delay[index_tx].tx_time	=
					    2 + (unsigned int)(TMP(focal_law_all_elem[group][kk][i].T_delay) / 2.5);
				TMP(focal_spi[k]).delay[index_rx].rx_time	=
						(unsigned int)(TMP(focal_law_all_elem[group][kk][i].R_delay) / 2.5);
			}		
			enablet = (cnt >> (32-channel_index_num_tx)) | (cnt << channel_index_num_tx);//循环左移channel_index_num位 ，使能控制
			enabler = (cnt >> (32-channel_index_num_rx)) | (cnt << channel_index_num_rx);//循环左移channel_index_num位 ，使能控制
			TMP(focal_spi[k]).tx_enable	= enablet;
			TMP(focal_spi[k]).rx_enable	= enabler;
			write_focal_data (&TMP(focal_spi[k]), k , reset);
		}
		else//UT
		{ 
            int tmpGate = 0;
			TMP(focal_spi[k]).group	= group;
			TMP(focal_spi[k]).all_beam_info	= beam_sum  - 1;
			TMP(focal_spi[k]).gain_offset	= GROUP_VAL_POS(group , gain_offset[kk]);
			TMP(focal_spi[k]).beam_delay	= 0;

            if (dev_fpga_version() == 2
                    && (group_get_mode(group) == UT1_SCAN
                        || group_get_mode(group) == UT2_SCAN)) {
                tmpGate = 2500;
            }
            TMP(focal_spi[k]).gate_a_start  = pp->gate_a_start[group][kk] + tmpGate;
            TMP(focal_spi[k]).gate_a_end    = pp->gate_a_end[group][kk] + tmpGate;
            TMP(focal_spi[k]).gate_b_start  = pp->gate_b_start[group][kk] + tmpGate;
            TMP(focal_spi[k]).gate_b_end    = pp->gate_b_end[group][kk] + tmpGate;
            TMP(focal_spi[k]).gate_i_start  = pp->gate_i_start[group][kk] + tmpGate;
            TMP(focal_spi[k]).gate_i_end    = pp->gate_i_end[group][kk] + tmpGate;
            g_message("%s[%d] a(%d)", __func__, __LINE__, TMP(focal_spi[k]).gate_a_start);
			/*UT Settings->Pulser->Tx/Rx mode*/
            if (group_get_rx_tx_mode(group) == PULSE_ECHO )/*单个探头收发模式*/
			{  
				pulser   = get_group_val ( &pp->p_config->group[group], GROUP_PULSER );
				receiver = pulser;
			}	
            else// if (group_get_rx_tx_mode(group) == PITCH_CATCH)
			{
				pulser   = get_group_val ( &pp->p_config->group[group], GROUP_PULSER );
				receiver = get_group_val ( &pp->p_config->group[group], GROUP_RECEIVER );
			}

			TMP(focal_spi[k]).rx_sel	=  channel_select((guint)(pulser));
			TMP(focal_spi[k]).tx_sel	=  channel_select((guint)(receiver));
			tmp_tx = (guint) (pulser);
			tmp_rx = (guint) (receiver);
			channel_index_num_tx = (tmp_tx % 32 == 0)? 31 : (tmp_tx % 32 -1); 
			channel_index_num_rx = (tmp_rx % 32 == 0)? 31 : (tmp_rx % 32 -1);
			cnt = pow(2,LAW_VAL_POS(group, Elem_qty))-1; 
			for (i = 0;i < TMP(focal_law_all_beam[group][kk].N_ActiveElements); i++)
			{
				index_tx = (channel_index_num_tx + i) & 0x0000001f; 
				index_rx = (channel_index_num_rx + i) & 0x0000001f;
				TMP(focal_spi[k]).delay[index_tx].tx_time	=
					    2 + (unsigned int)(TMP(focal_law_all_elem[group][kk][i].T_delay) / 2.5);
				TMP(focal_spi[k]).delay[index_rx].rx_time	=
						(unsigned int)(TMP(focal_law_all_elem[group][kk][i].R_delay) / 2.5);
			}		
			enablet = (cnt >> (32-channel_index_num_tx)) | (cnt<<channel_index_num_tx);//循环左移channel_index_num位 ，使能控制
			enabler = (cnt >> (32-channel_index_num_rx)) | (cnt<<channel_index_num_rx);//循环左移channel_index_num位 ，使能控制	
			TMP(focal_spi[k]).tx_enable	= enablet;
			TMP(focal_spi[k]).rx_enable	= enabler;		
			write_focal_data (&TMP(focal_spi[k]), k , reset);
		}
	}
}

/* 初始化需要发给fpga的group参数 */
void init_group_spi (int group)
{
	unsigned char tmp = 0;
	//set filter
	set_filter (group);

	TMP(group_spi[group]).video_filter	= get_group_val (get_group_by_id(pp->p_config, group), GROUP_VIDEO_FILTER);
	TMP(group_spi[group]).rectifier		= 
		get_group_val (get_group_by_id (pp->p_config, group), GROUP_RECTIFIER);
	TMP(group_spi[group]).compress_rato	= 
        ((group_get_range(group) / 10.0) / GROUP_VAL_POS(group, point_qty)) > 1 ?
        ((group_get_range(group) / 10.0) / GROUP_VAL_POS(group, point_qty)) : 1;


    group_set_gain(group, group_get_gain(group));
//    TMP(group_spi[group]).gain			= group_get_gain(group) / 10.0;


	TMP(group_spi[group]).tcg_point_qty	= GROUP_VAL_POS(group , SizingCurves.dac_point_qty);
	if(GROUP_VAL_POS(group , SizingCurves.curve_pos) == 3)
	{
		TMP(group_spi[group]).tcg_en		= 1;
	}
	else
	{
		TMP(group_spi[group]).tcg_en		= 0;
	}

	TMP(group_spi[group]).UT2			=  0 ; //(GROUP_VAL_POS (group, group_mode) == 3) ? 1 : 0;
	TMP(group_spi[group]).UT1			=  0 ; //(GROUP_VAL_POS (group, group_mode) == 2) ? 1 : 0;
	TMP(group_spi[group]).PA			=  0 ; //(GROUP_VAL_POS (group, group_mode) == 1) ? 1 : 0;
	if(GROUP_VAL_POS (group, group_mode) == UT1_SCAN)
	{
		TMP(group_spi[group]).UT1 = 1 ;
	}
	else if (GROUP_VAL_POS (group, group_mode) == UT2_SCAN)
	{
		TMP(group_spi[group]).UT2 = 1 ;
	}
	else
	{
		TMP(group_spi[group]).PA = 1 ;
	}
	TMP(group_spi[group]).tx_start	= 2;
	TMP(group_spi[group]).tx_end	= GROUP_VAL_POS(group , pulser_width1) / (25 * PW_DIV / 10);

    if (dev_fpga_version() == 2
            && (group_get_mode(group) == UT1_SCAN
                || group_get_mode(group) == UT2_SCAN)) {
        TMP(group_spi[group]).sample_start	= (group_get_start(group) + 25*1000 + GROUP_VAL_POS(group , wedge_delay)) / 10;
    } else {
        TMP(group_spi[group]).sample_start	= (group_get_start(group) + GROUP_VAL_POS(group , wedge_delay)) / 10;
    }
    TMP(group_spi[group]).sample_range	= TMP(group_spi[group]).sample_start + group_get_range(group) / 10;
	TMP(group_spi[group]).sum_gain = GROUP_VAL_POS(group , sum_gain)  ;
	TMP(group_spi[group]).point_qty  = GROUP_VAL_POS(group , point_qty);
	TMP(group_spi[group]).gain1			= 0;
	TMP(group_spi[group]).gate_a_height	= GROUP_VAL_POS(group, gate[0].height) * 20.47 ;
	//	TMP(group_spi[group]).gate_a_start	=  GROUP_VAL_POS(group, gate[0].start) / 10 ;

	if (GROUP_VAL_POS(group, gate[0].synchro) == 0)
		tmp = (tmp & 0xf3) | 0x00;
	else if (GROUP_VAL_POS(group, gate[0].synchro) == 1)
		tmp = (tmp & 0xf3) | 0x04;
	if (GROUP_VAL_POS(group, gate[0].measure) == 0)
		tmp = (tmp & 0xfc) | 0x01;
	else if (GROUP_VAL_POS(group, gate[0].measure) == 1)
		tmp = (tmp & 0xfc) | 0x00;

	TMP(group_spi[group]).gate_a_logic	= tmp;	
	/* 0-1 表示测量选择 00 波前 Edge 01 波峰 Peak 
	 * 2-3 表示同步选择 00 发射同步Pulse 01 AGate 10 BGate 11 IGATE
	 * */
	TMP(group_spi[group]).gate_b_height	= GROUP_VAL_POS(group, gate[1].height) * 20.47 ;
	if (GROUP_VAL_POS(group, gate[1].synchro) == 0)
		tmp = (tmp & 0xf3) | 0x00;
	else if (GROUP_VAL_POS(group, gate[1].synchro) == 1)
		tmp = (tmp & 0xf3) | 0x04;
	else if (GROUP_VAL_POS(group, gate[1].synchro) == 2)
		tmp = (tmp & 0xf3) | 0x08;

	if (GROUP_VAL_POS(group, gate[1].measure) == 0)
		tmp = (tmp & 0xfc) | 0x01;
	else if (GROUP_VAL_POS(group, gate[1].measure) == 1)
		tmp = (tmp & 0xfc) | 0x00;

	TMP(group_spi[group]).gate_b_logic	= tmp;
	TMP(group_spi[group]).gate_i_height	= GROUP_VAL_POS(group, gate[2].height) * 20.47;
	if (GROUP_VAL_POS(group, gate[2].synchro) == 0)
		tmp = (tmp & 0xf3) | 0x00;

	if (GROUP_VAL_POS(group, gate[2].measure) == 0)
		tmp = (tmp & 0xfc) | 0x01;
	else if (GROUP_VAL_POS(group, gate[2].measure) == 1)
		tmp = (tmp & 0xfc) | 0x00;

	TMP(group_spi[group]).gate_i_logic	= tmp;	
	TMP(group_spi[group]).reject = get_reject(pp->p_config) * 40.95;	

	//把厚度换算成时间
	int velocity = get_group_val (get_group_by_id (pp->p_config, group), GROUP_VELOCITY); //cm/s
	double thickness = GROUP_VAL_POS(group , min_thickness) ;
	double temp = 10000000.0 / velocity;//mm / (cm/s) = 10^8 ns   单位为10ns
	TMP(group_spi[group]).thickness_min = (unsigned int)(thickness * temp );//

	thickness = GROUP_VAL_POS(group , max_thickness) ;
	TMP(group_spi[group]).thickness_max = (unsigned int)(thickness * temp );

	int value = TMP(group_spi[group]).thickness_max - TMP(group_spi[group]).thickness_min;
	if(value > 0)
	{
		TMP(group_spi[group]).thickness_factor = 0xffffff / value;
	}else{
		TMP(group_spi[group]).thickness_factor = 0xffffff;//模拟输出最大值
	}

	TMP(group_spi[group]).thickness_source = 0;

	TMP(group_spi[group]).rx_time		= TMP(group_spi[group]).sample_range  + GetGroupMaxBeamDelay(group) + 5;
	MultiGroupRefreshIdelTime() ;

    TMP(group_spi[group].average) =  GROUP_VAL_POS(group , averaging1);
}

void RefreshGroupFocalLawSpi (int group)
{
	guint offset, beam_qty =TMP(beam_qty[group]), k, kk , i,enablet = 0, enabler = 0;
	guint tmp_tx, tmp_rx, index_tx, index_rx, channel_index_num_tx, channel_index_num_rx,cnt;
	GROUP *p_grp = get_group_by_id (pp->p_config, group);

	int beam_sum = get_beam_qty();
	unsigned char pulser, receiver;

	for (offset = 0, k = 0 ; k < group; k++)
		offset += TMP(beam_qty[k]);
	for (k = offset , kk = 0 ; k < offset + beam_qty; k++ , kk++)
	{
		if(GROUP_VAL_POS(group , group_mode) == 1)//PA
		{
			TMP(focal_spi[k]).group	= group;
			TMP(focal_spi[k]).all_beam_info	= beam_sum  - 1;
			TMP(focal_spi[k]).gain_offset	= GROUP_VAL_POS(group , gain_offset[kk]);
			TMP(focal_spi[k]).beam_delay	= GROUP_VAL_POS(group , beam_delay[kk]) / 10;
			TMP(focal_spi[k]).gate_a_start  = pp->gate_a_start[group][kk];
			TMP(focal_spi[k]).gate_a_end    = pp->gate_a_end[group][kk]  ;
			TMP(focal_spi[k]).gate_b_start  = pp->gate_b_start[group][kk];
			TMP(focal_spi[k]).gate_b_end    = pp->gate_b_end[group][kk]  ;
			TMP(focal_spi[k]).gate_i_start  = pp->gate_i_start[group][kk];
			TMP(focal_spi[k]).gate_i_end    = pp->gate_i_end[group][kk]  ;
			/*UT Settings->Pulser->Tx/Rx mode*/
            if (group_get_rx_tx_mode(group) == PULSE_ECHO )/*单个探头收发模式*/
			{
				pulser   =  GROUP_VAL_POS(group , pulser1) ;//get_group_val ( &pp->p_config->group[group], GROUP_PULSER );
				receiver = pulser;
			}
            else// if (group_get_rx_tx_mode(group) == PITCH_CATCH)
			{
				pulser   = GROUP_VAL_POS(group , pulser1) ;  //get_group_val ( &pp->p_config->group[group], GROUP_PULSER );
				receiver = GROUP_VAL_POS(group , receiver1) ; //get_group_val ( &pp->p_config->group[group], GROUP_RECEIVER );
			}

			if(LAW_VAL_POS(group, Focal_type) == 1)/*Linear*/
			{
				TMP(focal_spi[k]).tx_sel	=
					channel_select(
							(guint)(pulser) + (guint)(LAW_VAL_POS(group, First_tx_elem))+kk*LAW_VAL_POS(group, Elem_step)-1 );
				TMP(focal_spi[k]).rx_sel	=
					channel_select(
							(guint)(receiver) + (guint)(LAW_VAL_POS(group, First_tx_elem))+ kk*LAW_VAL_POS(group, Elem_step)-1 );
				tmp_tx = (guint) (pulser)     + (guint)(LAW_VAL_POS(group, First_tx_elem) + kk*LAW_VAL_POS(group, Elem_step))-1 ;
				tmp_rx = (guint) (receiver)   + (guint)(LAW_VAL_POS(group, First_tx_elem) + kk*LAW_VAL_POS(group, Elem_step))-1 ;
			}
			else
			{
				TMP(focal_spi[k]).tx_sel	=
					channel_select( (guint)(pulser)   + (guint)(LAW_VAL_POS(group, First_tx_elem))-1 );
				TMP(focal_spi[k]).rx_sel	=
					channel_select( (guint)(receiver) + (guint)(LAW_VAL_POS(group, First_tx_elem))-1 );
				tmp_tx = (guint) (pulser)   + (guint)(LAW_VAL_POS(group, First_tx_elem)) - 1;
				tmp_rx = (guint) (receiver) + (guint)(LAW_VAL_POS(group, First_tx_elem)) - 1;
			}
			channel_index_num_tx = (tmp_tx % 32 == 0)? 31 : (tmp_tx % 32 -1);
			channel_index_num_rx = (tmp_rx % 32 == 0)? 31 : (tmp_rx % 32 -1);
			cnt = pow(2,LAW_VAL_POS(group, Elem_qty))-1;
			for (i = 0;i < TMP(focal_law_all_beam[group][kk].N_ActiveElements); i++)
			{
				index_tx = (channel_index_num_tx + i) & 0x0000001f;
				index_rx = (channel_index_num_rx + i) & 0x0000001f;
				TMP(focal_spi[k]).delay[index_tx].tx_time	=
					    (unsigned int)(TMP(focal_law_all_elem[group][kk][i].T_delay) / 2.5);
				TMP(focal_spi[k]).delay[index_rx].rx_time	=
						(unsigned int)(TMP(focal_law_all_elem[group][kk][i].R_delay) / 2.5);
			}
			enablet = (cnt >> (32-channel_index_num_tx)) | (cnt << channel_index_num_tx);//循环左移channel_index_num位 ，使能控制
			enabler = (cnt >> (32-channel_index_num_rx)) | (cnt << channel_index_num_rx);//循环左移channel_index_num位 ，使能控制
			TMP(focal_spi[k]).tx_enable	= enablet;
			TMP(focal_spi[k]).rx_enable	= enabler;

			InitTcgSpi(group , k) ;
		}
		else//UT
		{
			TMP(focal_spi[k]).group	= group;
			TMP(focal_spi[k]).all_beam_info	= beam_sum - 1;
			//TMP(focal_spi[k]).gain_offset	= pp->tmp_gain_off[k];
			TMP(focal_spi[k]).gain_offset	= GROUP_VAL_POS(group , gain_offset[kk]);
			TMP(focal_spi[k]).beam_delay	= 0;
			TMP(focal_spi[k]).gate_a_start  = pp->gate_a_start[group][kk];
			TMP(focal_spi[k]).gate_a_end    = pp->gate_a_end[group][kk];
			TMP(focal_spi[k]).gate_b_start  = pp->gate_b_start[group][kk];
			TMP(focal_spi[k]).gate_b_end    = pp->gate_b_end[group][kk];
			TMP(focal_spi[k]).gate_i_start  = pp->gate_i_start[group][kk];
			TMP(focal_spi[k]).gate_i_end    = pp->gate_i_end[group][kk];
			/*UT Settings->Pulser->Tx/Rx mode*/
            if (group_get_rx_tx_mode(group) == PULSE_ECHO )/*单个探头收发模式*/
			{
				pulser   = get_group_val ( &pp->p_config->group[group], GROUP_PULSER );
				receiver = pulser;
			}
            else// if (group_get_rx_tx_mode(group) == PITCH_CATCH)
			{
				pulser   = get_group_val ( &pp->p_config->group[group], GROUP_PULSER );
				receiver = get_group_val ( &pp->p_config->group[group], GROUP_RECEIVER );
			}
			//else
			//{
			//	//其他模式待加
			//}

			TMP(focal_spi[k]).rx_sel	=  channel_select((guint)(pulser));
			TMP(focal_spi[k]).tx_sel	=  channel_select((guint)(receiver));
			tmp_tx = (guint) (pulser);
			tmp_rx = (guint) (receiver);
			channel_index_num_tx = (tmp_tx % 32 == 0)? 31 : (tmp_tx % 32 -1);
			channel_index_num_rx = (tmp_rx % 32 == 0)? 31 : (tmp_rx % 32 -1);
			cnt = pow(2,LAW_VAL_POS(group, Elem_qty))-1;
			for (i = 0;i < TMP(focal_law_all_beam[group][kk].N_ActiveElements); i++)
			{
				index_tx = (channel_index_num_tx + i) & 0x0000001f;
				index_rx = (channel_index_num_rx + i) & 0x0000001f;
				TMP(focal_spi[k]).delay[index_tx].tx_time	=
					    (unsigned int)(TMP(focal_law_all_elem[group][kk][i].T_delay) / 2.5);
				TMP(focal_spi[k]).delay[index_rx].rx_time	=
						(unsigned int)(TMP(focal_law_all_elem[group][kk][i].R_delay) / 2.5);
			}
			enablet = (cnt >> (32-channel_index_num_tx)) | (cnt<<channel_index_num_tx);//循环左移channel_index_num位 ，使能控制
			enabler = (cnt >> (32-channel_index_num_rx)) | (cnt<<channel_index_num_rx);//循环左移channel_index_num位 ，使能控制
			TMP(focal_spi[k]).tx_enable	= enablet;
			TMP(focal_spi[k]).rx_enable	= enabler;

			InitTcgSpi( group , k ) ;
		}
	}
}

void RefreshGroupGroupSpi (guint group)
{
	gint tmp = 0;
	//set filter
	set_filter (group);

	TMP(group_spi[group]).video_filter	= get_group_val (get_group_by_id(pp->p_config, group), GROUP_VIDEO_FILTER);
	TMP(group_spi[group]).rectifier		=
		get_group_val (get_group_by_id (pp->p_config, group), GROUP_RECTIFIER);
	TMP(group_spi[group]).compress_rato	=
        ((group_get_range(group) / 10.0) / GROUP_VAL_POS(group, point_qty)) > 1 ?
        ((group_get_range(group) / 10.0) / GROUP_VAL_POS(group, point_qty)) : 1;

    group_set_gain(group, group_get_gain(group));
//    TMP(group_spi[group]).gain			= group_get_gain(group) / 10.0;

	TMP(group_spi[group]).tcg_point_qty	= GROUP_VAL_POS(group , SizingCurves.dac_point_qty);
	if(GROUP_VAL_POS(group , SizingCurves.curve_pos) == 3)
	{
		TMP(group_spi[group]).tcg_en		= 1;
	}
	else
	{
		TMP(group_spi[group]).tcg_en		= 0;
	}

	TMP(group_spi[group]).UT2			=  0 ; //(GROUP_VAL_POS (group, group_mode) == 3) ? 1 : 0;
	TMP(group_spi[group]).UT1			=  0 ; //(GROUP_VAL_POS (group, group_mode) == 2) ? 1 : 0;
	TMP(group_spi[group]).PA			=  0 ; //(GROUP_VAL_POS (group, group_mode) == 1) ? 1 : 0;
	if(GROUP_VAL_POS (group, group_mode) == UT1_SCAN)
	{
		TMP(group_spi[group]).UT1 = 1 ;
	}
	else if (GROUP_VAL_POS (group, group_mode) == UT2_SCAN)
	{
		TMP(group_spi[group]).UT2 = 1 ;
	}
	else
	{
		TMP(group_spi[group]).PA = 1 ;
	}

	TMP(group_spi[group]).tx_start	= 2;
	TMP(group_spi[group]).tx_end	= GROUP_VAL_POS(group , pulser_width1) / (25 * PW_DIV / 10);

    if (dev_fpga_version() == 2
            && (group_get_mode(group) == UT1_SCAN
                || group_get_mode(group) == UT2_SCAN)) {
        TMP(group_spi[group]).sample_start	= (group_get_start(group) + 25*1000 + GROUP_VAL_POS(group , wedge_delay)) / 10;
    } else {
        TMP(group_spi[group]).sample_start	= (group_get_start(group) + GROUP_VAL_POS(group , wedge_delay)) / 10;
    }
    TMP(group_spi[group]).sample_range	= TMP(group_spi[group]).sample_start + group_get_range(group) / 10;
	TMP(group_spi[group]).sum_gain = GROUP_VAL_POS(group , sum_gain)  ;
	TMP(group_spi[group]).point_qty  = GROUP_VAL_POS(group , point_qty);
	TMP(group_spi[group]).gain1			= 0;
	TMP(group_spi[group]).gate_a_height	= GROUP_VAL_POS(group, gate[0].height) * 20.47 ;

	if (GROUP_VAL_POS(group, gate[0].synchro) == 0)
		tmp = (tmp & 0xfffffff3) | 0x00;
	else if (GROUP_VAL_POS(group, gate[0].synchro) == 1)
		tmp = (tmp & 0xfffffff3) | 0x0c;

	if (GROUP_VAL_POS(group, gate[0].measure) == 0)
		tmp = (tmp & 0xfffffffc) | 0x01;
	else if (GROUP_VAL_POS(group, gate[0].measure) == 1)
		tmp = (tmp & 0xfffffffc) | 0x00;

	TMP(group_spi[group]).gate_a_logic	= tmp;
	/* 0-1 表示测量选择 00 波前 Edge 01 波峰 Peak
	 * 2-3 表示同步选择 00 发射同步Pulse 01 AGate 10 BGate 11 IGATE*/

	TMP(group_spi[group]).gate_b_height	= GROUP_VAL_POS(group, gate[1].height) * 20.47 ;

	if (GROUP_VAL_POS(group, gate[1].synchro) == 0)
		tmp = (tmp & 0xf3) | 0x00;
	else if (GROUP_VAL_POS(group, gate[1].synchro) == 1)
		tmp = (tmp & 0xf3) | 0x04;
	else if (GROUP_VAL_POS(group, gate[1].synchro) == 2)
		tmp = (tmp & 0xf3) | 0x08;


	if (GROUP_VAL_POS(group, gate[1].measure) == 0)
		tmp = (tmp & 0xfffffffc) | 0x01;
	else if (GROUP_VAL_POS(group, gate[1].measure) == 1)
		tmp = (tmp & 0xfffffffc) | 0x00;

	TMP(group_spi[group]).gate_b_logic	= tmp;

	TMP(group_spi[group]).gate_i_height	= GROUP_VAL_POS(group, gate[2].height) * 20.47;

	if (GROUP_VAL_POS(group, gate[2].synchro) == 0)
		tmp = (tmp & 0xfffffff3) | 0x00;

	if (GROUP_VAL_POS(group, gate[2].measure) == 0)
		tmp = (tmp & 0xfffffffc) | 0x01;
	else if (GROUP_VAL_POS(group, gate[2].measure) == 1)
		tmp = (tmp & 0xfffffffc) | 0x00;

	TMP(group_spi[group]).gate_i_logic	= tmp;
	TMP(group_spi[group]).reject        = get_reject(pp->p_config) * 40.95;

	//把厚度换算成时间
	int velocity = get_group_val (get_group_by_id (pp->p_config, group), GROUP_VELOCITY); //cm/s
	double thickness = GROUP_VAL_POS(group , min_thickness) ; //get_min_thickness(pp->p_config);
	double temp = 10000000.0 / velocity;//mm / (cm/s) = 10^8 ns   单位为10ns
	TMP(group_spi[group]).thickness_min = (unsigned int)(thickness * temp );//

	thickness = GROUP_VAL_POS(group , max_thickness) ;
	TMP(group_spi[group]).thickness_max = (unsigned int)(thickness * temp );

	int value = TMP(group_spi[group]).thickness_max - TMP(group_spi[group]).thickness_min;
	if(value > 0)
	{
		TMP(group_spi[group]).thickness_factor = 0xffffff / value;
	}
	else
	{
		TMP(group_spi[group]).thickness_factor = 0xffffff;//模拟输出最大值
	}

	TMP(group_spi[group]).thickness_source = 0;

	TMP(group_spi[group]).rx_time		= TMP(group_spi[group]).sample_range + GetGroupMaxBeamDelay(group) + 5;
	MultiGroupRefreshIdelTime() ;

	TMP(group_spi[group].average) =  GROUP_VAL_POS(group , averaging1);

}

void MultiGroupChannelEnable()
{
	unsigned int _nChannelEnable  =  GetChannelEnable() ;
	SetChannelEnableFlag(_nChannelEnable) ;
}


void MultiGroupSendFocalSpi()
{
	int i ;
	int _nGroupQty ;
    _nGroupQty = get_group_qty(pp->p_config) ;
	for(i = 0 ; i < _nGroupQty ; i++)
	{
		UpdateGateForSpiSending(i);
		RefreshGroupFocalLawSpi(i)           ;
	}
	MultiGroupSendAllFocalSpi();
}

void MultiGroupSendGroupSpi()
{
	int i ;
	int _nGroupQty ;

	_nGroupQty = get_group_qty(pp->p_config) ;
	for(i = 0 ; i < _nGroupQty ; i++)
	{
		RefreshGroupGroupSpi(i);
	}
	MultiGroupSendAllGroupSpi();
	// enable channel
	MultiGroupChannelEnable() ;
}


/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
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

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <gdk/gdkkeysyms.h>
#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <memory.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include "lzk/output_spi.h"
#include "string/_string.h"
#include "lzk/Ascan.h"
#include "uihandler/menu/menuDefine.h"
#include "uihandler/keyHandler.h"
#include "ui/menu/menuFunc.h"
#include "ui/ui.h"
#include "ui/workpiece.h"
#include "globalData.h"
#include "calculation/sizingcurve.h"
#include "calculation/limit.h"



//void   showHelp(MENU_UNION menu_union ,int enable);
int    menu3FuncTrans(MENU_UNION menu_union);
//void   wizardFocallawHandler_setStep(int step);
//void   wizardGroupHandler_setStep(int step);
int  get_beam_qty();
guint  get_skew(int grp);
guint  get_freq ();
double get_pw ();
int    get_prf ();
int    MultiGroupGetMaxPrf() ;
int    RefreshScanInfor() ;
void   reset_scan_data() ;
void   reset_encoder()   ;
void   UpdateGateForSpiSending(int nGroupId_);
void   generate_focallaw(int grp);

void   refresh_linear_dac_pointer_info() ;
int    StoreBuffLimitToGroupPointQty(int GroupId_) ;
void   UT_group_config_settting (unsigned char grp) ;
void   software_update();
void   hardware_update();

gboolean eventbox2_function0 (GtkWidget *widget, GdkEventButton *event,	gpointer data);
static inline void data_process(unsigned char *data, guint pa);
gboolean foo (GtkAccelGroup *accel_group, GObject *acceleratable,
		guint keyval, GdkModifierType modifier, gpointer data);
gboolean key_press_handler (GtkWidget* pWidget,
		GdkEventKey* pEvent, gpointer data);
static int SetDBEightPercentThread(gpointer data);

const int MAX_DMA_FREQUENCY  =  500 ;
const double PROBE_RESISTANCE   =  50.0  ;

double tttmp;
//把group向导的设置参数保存起来
_group_wizard g_group_wizard_struct;
/* 输入数字时候的其他快捷键 */
static guint key_fast_map[] =
{
	GDK_Return, GDK_Escape, GDK_F1, GDK_F2, GDK_F3, GDK_F4, GDK_F5, GDK_F6, 
	GDK_F7, GDK_F8, GDK_F9, GDK_F10, GDK_F11, GDK_F12, GDK_Super_L
};



gboolean data_function0 (GtkWidget *widget,	GdkEventButton *event,	gpointer       data);
void data_0021 (GtkMenuItem *menuitem, gpointer data);
void data_0022 (GtkMenuItem *menuitem, gpointer data);
void data_022 (GtkMenuItem *menuitem, gpointer data);
void data_0221 (GtkMenuItem *menuitem, gpointer data);
void data_0222 (GtkSpinButton *spinbutton, gpointer data);
void data_0223 (GtkSpinButton *spinbutton, gpointer data);
void data_0224 (GtkSpinButton *spinbutton, gpointer data);
void data_0225 (GtkMenuItem *menuitem, gpointer data);
void data_0226 (GtkSpinButton *spinbutton, gpointer data);
void data_0227 (GtkSpinButton *spinbutton, gpointer data);
void data_0228 (GtkSpinButton *spinbutton, gpointer data);

void data_023 (GtkMenuItem *menuitem, gpointer data); 
void data_0231 (GtkMenuItem *menuitem, gpointer data); 
void data_0232 (GtkSpinButton *spinbutton, gpointer data);
void data_0233 (GtkSpinButton *spinbutton, gpointer data);
void data_0234 (GtkSpinButton *spinbutton, gpointer data);
void data_0238 (GtkSpinButton *spinbutton, gpointer data);
void data_0239 (GtkSpinButton *spinbutton, gpointer data);
void data_024 (GtkSpinButton *spinbutton, gpointer data);
void data_0241 (GtkSpinButton *spinbutton, gpointer data);
void data_0242 (GtkSpinButton *spinbutton, gpointer data);
void data_0243 (GtkSpinButton *spinbutton, gpointer data);
void data_032 (GtkSpinButton *spinbutton, gpointer data);
void data_0321 (GtkSpinButton *spinbutton, gpointer data);
void data_033 (GtkMenuItem *menuitem, gpointer data);
void data_0331 (GtkSpinButton *spinbutton, gpointer data);
void data_0332 (GtkSpinButton *spinbutton, gpointer data);
void data_034 (GtkSpinButton *spinbutton, gpointer data);
void data_root_angle_callback (GtkSpinButton *spinbutton, gpointer data);


void data_100 (GtkSpinButton *spinbutton, gpointer data);
void data_101 (GtkSpinButton *spinbutton, gpointer data);
void data_102 (GtkSpinButton *spinbutton, gpointer data);
void data_103 (GtkSpinButton *spinbutton, gpointer data);
void data_104 (GtkSpinButton *spinbutton, gpointer data);

void data_110 (GtkSpinButton *spinbutton, gpointer data);
void data_120 (GtkSpinButton *spinbutton, gpointer data);
void data_111 (GtkMenuItem *menuitem, gpointer data);            /* 111 Tx/Rx Mode 收发模式 */
void data_1121(GtkSpinButton *spinbutton, gpointer data);        /* 112 freq 频率 */
void data_112 (GtkMenuItem *menuitem, gpointer data);            /* 112 Freq 频率 */
void data_113 (GtkMenuItem *menuitem, gpointer data);            /* 113 voltage  */
void data_1141(GtkSpinButton *spinbutton, gpointer data);        /* 114 PW 脉宽*/
void data_114 (GtkMenuItem *menuitem, gpointer data);            /* 114 PW 脉宽 */
void data_1151(GtkSpinButton *spinbutton, gpointer data);        /* 115 PRF  重复频率*/
void data_115 (GtkMenuItem *menuitem, gpointer data);            /* 115 PRF  重复频率 */

void data_1201 (GtkMenuItem *menuitem, gpointer data); 
void data_121 (GtkMenuItem *menuitem, gpointer data);        /* 121 Filter */
void data_122 (GtkMenuItem *menuitem, gpointer data);        /* 122 Rectifier */
void data_124 (GtkMenuItem *menuitem, gpointer data);        /* 124 Averaging */
void data_125 (GtkSpinButton *spinbutton, gpointer data); 

void data_130 (GtkSpinButton *spinbutton, gpointer data);
void data_131 (GtkSpinButton *spinbutton, gpointer data);
void data_132 (GtkSpinButton *spinbutton, gpointer data);
void data_134 (GtkSpinButton *spinbutton, gpointer data);
void data_135 (GtkSpinButton *spinbutton, gpointer data);

void data_140 (GtkSpinButton *spinbutton, gpointer data);

void data_1431(GtkSpinButton *spinbutton, gpointer data);        /* 143 Points Qty */
void data_143 (GtkMenuItem *menuitem, gpointer data);            /* 143 Points Qty */
void data_1451(GtkSpinButton *spinbutton, gpointer data);        /* 145 Sum Gain */
void data_145 (GtkMenuItem *menuitem, gpointer data);            /* 145 Sum Gain */

void data_200 (GtkMenuItem *menuitem, gpointer data);         /*  200 Gate */
void data_201 (GtkMenuItem *menuitem, gpointer data);         /*  201 parameter */
void data_202 (GtkSpinButton *spinbutton, gpointer data);
void data_2021 (GtkMenuItem *menuitem, gpointer data);         /*  2021 synchro */
void data_203 (GtkSpinButton *spinbutton, gpointer data);
void data_2031 (GtkMenuItem *menuitem, gpointer data);         /*  2031 measure */
void data_204 (GtkSpinButton *spinbutton, gpointer data);
void data_2041 (GtkMenuItem *menuitem, gpointer data);         /*  2041 rf */
void data_205(gpointer data);

void data_210 (GtkMenuItem *menuitem, gpointer data);         /*  210 Alarm */
void data_211 (GtkMenuItem *menuitem, gpointer data);         /*  211 Group A */
void data_212 (GtkMenuItem *menuitem, gpointer data);         /*  212 condition A */
void data_213 (GtkMenuItem *menuitem, gpointer data);         /*  213 Operator */
void data_214 (GtkMenuItem *menuitem, gpointer data);         /*  214 Group B */
void data_215 (GtkMenuItem *menuitem, gpointer data);         /*  215 condition B */

void data_220 (GtkMenuItem *menuitem, gpointer data);          /* 220 Output */
void data_221 (GtkMenuItem *menuitem, gpointer data);         /* Output->alarm # */
void data_2211 (GtkMenuItem *menuitem, gpointer data);         /* Output->group */
void data_222 (GtkSpinButton *spinbutton, gpointer data);	/* Output->count */
void data_2221 (GtkMenuItem *menuitem, gpointer data);		/* Output->data */
void data_223 (GtkMenuItem *menuitem, gpointer data);          /* 223 sound */
void data_224 (GtkSpinButton *spinbutton, gpointer data);
void data_225 (GtkSpinButton *spinbutton, gpointer data);
void data_230 (GtkMenuItem *menuitem, gpointer data); /* Gate/Alarm->Sizing Curves->Mode 230 */
void data_231 (GtkMenuItem *menuitem, gpointer data); /* Gate/Alarm->Sizing Curves->Curve 231 */
void data_2311 (GtkMenuItem *menuitem, gpointer data);
void data_2312 (GtkSpinButton *spinbutton, gpointer data);
void data_232 (GtkSpinButton *spinbutton, gpointer data);
void data_2321 (GtkSpinButton *spinbutton, gpointer data);
void data_2322 (GtkSpinButton *spinbutton, gpointer data);
void data_2323 (GtkSpinButton *spinbutton, gpointer data);
void data_233 (GtkSpinButton *spinbutton, gpointer data);
void data_2331 (GtkSpinButton *spinbutton, gpointer data);
void data_2332 (GtkSpinButton *spinbutton, gpointer data);
void data_234 (GtkSpinButton *spinbutton, gpointer data);
void data_235 (GtkSpinButton *spinbutton, gpointer data);
void data_234_add_point(DRAW_UI_P p);
void data_235_del_point(DRAW_UI_P p);


void data_300 (GtkMenuItem *menuitem, gpointer data); /* Measurements->Reading->list 300 */
void data_302 (GtkMenuItem *menuitem, gpointer data); /* Measurements->Reading->Field1 302 */
void data_303 (GtkMenuItem *menuitem, gpointer data); /* Measurements->Reading->Field2 303 */
void data_304 (GtkMenuItem *menuitem, gpointer data); /* Measurements->Reading->Field3 304 */
void data_305 (GtkMenuItem *menuitem, gpointer data); /* Measurements->Reading->Field4 305 */

void data_310 (GtkMenuItem *menuitem, gpointer data); /* Measurements->cursors->selection 310 */
void data_311 (GtkSpinButton *spinbutton, gpointer data);
void data_3111 (GtkSpinButton *spinbutton, gpointer data);
void data_3112 (GtkSpinButton *spinbutton, gpointer data);
void data_3113 (GtkSpinButton *spinbutton, gpointer data);
void data_312 (GtkSpinButton *spinbutton, gpointer data);
void data_3121 (GtkSpinButton *spinbutton, gpointer data);
void data_3122 (GtkSpinButton *spinbutton, gpointer data);
void data_313 (GtkSpinButton *spinbutton, gpointer data);
void data_3131 (GtkSpinButton *spinbutton, gpointer data);
void data_3132 (GtkSpinButton *spinbutton, gpointer data);
void data_3133 (GtkSpinButton *spinbutton, gpointer data);
void data_314 (GtkSpinButton *spinbutton, gpointer data);
void data_3141 (GtkSpinButton *spinbutton, gpointer data);
void data_3142 (GtkSpinButton *spinbutton, gpointer data);
void data_3143 (GtkSpinButton *spinbutton, gpointer data);
void data_315 (GtkSpinButton *spinbutton, gpointer data);
void data_324 (GtkSpinButton *spinbutton, gpointer data);

void data_330 (GtkMenuItem *menuitem, gpointer data); /* Measurements->Thickness->source 330 */
void data_331 (GtkSpinButton *spinbutton, gpointer data);
void data_332 (GtkSpinButton *spinbutton, gpointer data);
void data_333 (GtkSpinButton *spinbutton, gpointer data);

void data_400 (GtkMenuItem *menuitem, gpointer data);
void data_401 (GtkMenuItem *menuitem, gpointer data);
void data_4011 (GtkMenuItem *menuitem, gpointer data);
void data_4012 (GtkMenuItem *menuitem, gpointer data);
void data_4013 (GtkMenuItem *menuitem, gpointer data);
void data_402 (GtkMenuItem *menuitem, gpointer data);
void data_4021 (GtkMenuItem *menuitem, gpointer data);
void data_403 (GtkMenuItem *menuitem, gpointer data);
void data_404 (GtkSpinButton *spinbutton, gpointer data);
void data_405 (GtkSpinButton *spinbutton, gpointer data);
void data_410 (GtkMenuItem *menuitem, gpointer data);
void data_411 (GtkMenuItem *menuitem, gpointer data);
void data_420 (GtkMenuItem *menuitem, gpointer data);
void data_421 (GtkMenuItem *menuitem, gpointer data);
void data_422 (GtkSpinButton *spinbutton, gpointer data);
void data_4221 (GtkSpinButton *spinbutton, gpointer data);
void data_423 (GtkSpinButton *spinbutton, gpointer data);
void data_4231 (GtkSpinButton *spinbutton, gpointer data);
void data_424 (GtkSpinButton *spinbutton, gpointer data);
void data_4241 (GtkSpinButton *spinbutton, gpointer data);
void data_425 (GtkSpinButton *spinbutton, gpointer data);
void data_4251 (GtkSpinButton *spinbutton, gpointer data);
void data_430 (GtkMenuItem *menuitem, gpointer data);
void data_431 (GtkSpinButton *spinbutton, gpointer data);
void data_4311 (GtkSpinButton *spinbutton, gpointer data);
void data_4312 (GtkSpinButton *spinbutton, gpointer data);
void data_432 (GtkSpinButton *spinbutton, gpointer data);
void data_4321 (GtkSpinButton *spinbutton, gpointer data);
void data_4322 (GtkSpinButton *spinbutton, gpointer data);
void data_440 (GtkMenuItem *menuitem, gpointer data);
void data_434 (GtkMenuItem *menuitem, gpointer data);
void data_441 (GtkMenuItem *menuitem, gpointer data);
void data_4411 (GtkSpinButton *spinbutton, gpointer data);
void data_4414 (GtkMenuItem *menuitem, gpointer data);
void data_4415 (GtkMenuItem *menuitem, gpointer data);
void data_442 (GtkMenuItem *menuitem, gpointer data);
//void data_443 (GtkMenuItem *menuitem, gpointer data);
void data_444 (GtkMenuItem *menuitem, gpointer data);
void data_445 (GtkMenuItem *menuitem, gpointer data);

void data_500 (GtkMenuItem *menuitem, gpointer data);
void data_501 (GtkMenuItem *menuitem, gpointer data);
void data_502 (GtkMenuItem *menuitem, gpointer data);
void data_505 (GtkMenuItem *menuitem, gpointer data);
void data_510 (GtkSpinButton *spinbutton, gpointer data);
void data_511 (GtkSpinButton *spinbutton, gpointer data);
void data_512 (GtkMenuItem *menuitem, gpointer data);            /* 512 Skew (deg) */

void data_521 (GtkSpinButton *spinbutton, gpointer data);
void data_522 (GtkSpinButton *spinbutton, gpointer data);
void data_523 (GtkSpinButton *spinbutton, gpointer data);

void data_530 (GtkMenuItem *menuitem, gpointer data);
void data_531 (GtkSpinButton *spinbutton, gpointer data);
void data_532 (GtkSpinButton *spinbutton, gpointer data);
void data_533 (GtkMenuItem *menuitem, gpointer data);
void data_534 (GtkMenuItem *menuitem, gpointer data);

void data_600 (GtkMenuItem *menuitem, gpointer data);

void data_610 (GtkSpinButton *spinbutton, gpointer data);
void data_611 (GtkSpinButton *spinbutton, gpointer data);
void data_612 (GtkSpinButton *spinbutton, gpointer data);
void data_613 (GtkSpinButton *spinbutton, gpointer data);
void data_614 (GtkSpinButton *spinbutton, gpointer data);

void data_620 (GtkMenuItem *menuitem, gpointer data);
void data_621 (GtkSpinButton *spinbutton, gpointer data);
void data_6211 (GtkSpinButton *spinbutton, gpointer data);
void data_622 (GtkSpinButton *spinbutton, gpointer data);
void data_6221 (GtkSpinButton *spinbutton, gpointer data);
void data_623 (GtkSpinButton *spinbutton, gpointer data);
void data_6231 (GtkSpinButton *spinbutton, gpointer data);
void data_624 (GtkSpinButton *spinbutton, gpointer data);
void data_630 (GtkSpinButton *spinbutton, gpointer data);
void data_631 (GtkSpinButton *spinbutton, gpointer data);
void data_632 (GtkSpinButton *spinbutton, gpointer data);
void data_633 (GtkSpinButton *spinbutton, gpointer data);
void data_634 (GtkMenuItem *menuitem, gpointer data);
void data_643() ;

void data_700 (GtkMenuItem *menuitem, gpointer data);
void data_701 (GtkMenuItem *menuitem, gpointer data);
void data_702 (GtkMenuItem *menuitem, gpointer data);
void data_703 (GtkSpinButton *spinbutton, gpointer data);
void data_704 (GtkSpinButton *spinbutton, gpointer data);
void data_710 (GtkMenuItem *menuitem, gpointer data);
void data_711 (GtkMenuItem *menuitem, gpointer data);
void data_712 (GtkMenuItem *menuitem, gpointer data);
void data_713 (GtkSpinButton *spinbutton, gpointer data);
void data_714 (GtkSpinButton *spinbutton, gpointer data);
void data_715 (GtkSpinButton *spinbutton, gpointer data);
void data_720 (GtkSpinButton *spinbutton, gpointer data);
void data_721 (GtkSpinButton *spinbutton, gpointer data);
void data_722 (GtkSpinButton *spinbutton, gpointer data);
void data_723 (GtkSpinButton *spinbutton, gpointer data);
void data_724 (GtkSpinButton *spinbutton, gpointer data);
void data_725 (GtkSpinButton *spinbutton, gpointer data);
void data_730 (GtkMenuItem *menuitem, gpointer data);
void data_731 ();
void data_732 (GtkMenuItem *menuitem, gpointer data);
void data_740 (GtkMenuItem *menuitem, gpointer data);
void data_741 (GtkMenuItem *menuitem, gpointer data);

void data_800 (GtkMenuItem *menuitem, gpointer data);
void data_804 (GtkMenuItem *menuitem, gpointer data);
void data_810 (GtkMenuItem *menuitem, gpointer data);
void data_812 (GtkMenuItem *menuitem, gpointer data);
void data_824 (GtkMenuItem *menuitem, gpointer data);
void data_830 (GtkMenuItem *menuitem, gpointer data);

void data_900 (GtkMenuItem *menuitem, gpointer data);
void data_901 (GtkSpinButton *spinbutton, gpointer data);
void data_904 (GtkMenuItem *menuitem, gpointer data);
void data_903 (GtkMenuItem *menuitem, gpointer data);
void data_912 (GtkMenuItem *menuitem, gpointer data);
void data_913 (GtkMenuItem *menuitem, gpointer data);
void data_9131 (GtkMenuItem *menuitem, gpointer data);
//void data_923 (GtkMenuItem *menuitem, gpointer data);
void data_930 (GtkMenuItem *menuitem, gpointer data);


void request_refresh(int mark)
{
    FreezingFPGA(TRUE);
	REFRESH_REQUEST = mark;
}

void RefreshGainOffset(int grp)
{
	char* pGain  = (char*)(GROUP_VAL_POS(grp , gain_offset));
	memset(pGain , 0 , sizeof(short) * setup_MAX_GROUP_LAW_QTY) ;
}

void RefreshBeamDelay(int grp)
{
	char* pTempDelay = (char*)(TMP(beam_delay[grp]));
	char* pGroupDelay = (char*)(GROUP_VAL_POS(grp , beam_delay));
    if ( GROUP_VAL_POS(grp, group_mode) != PA_SCAN ) {
        memset(pGroupDelay, 0, sizeof(int)*setup_MAX_GROUP_LAW_QTY);
    } else {
        memcpy(pGroupDelay , pTempDelay , sizeof(int) * setup_MAX_GROUP_LAW_QTY) ;
    }
}


static void setup_para(PARAMETER_P p, guint group)
{
	GROUP *p_grp = get_group_by_id (pp->p_config, group);
	/* 探头 */
	p->probe_p->D1_D2 = 0;			/* 0 1d 1 2d*/
	p->probe_p->Pitch_Catch = 0 ;  // 目前不支持PC模式的 聚焦法则

	p->probe_p->transmi_trans_longi = 0;	/* 1 SW 横波慢 0 LW 纵波快 */
	p->probe_p->recev_trans_longi	= 0;	/* */
	p->probe_p->pb_skew_angle	= GROUP_VAL_POS(group, skew) / 100.0;	/* ffff*/
	p->probe_p->pb_frequency	= get_group_val (p_grp, GROUP_FREQ_VAL) / 1000.0;	/* */
	p->probe_p->ele_num_pri = GROUP_VAL_POS(group, probe.Elem_qty);	/* */
	p->probe_p->ele_num_sec = 1;	/* 2D 时候就不为1了 */

	p->probe_p->pb_skew_angle_r = 0;	/*ffff */
	p->probe_p->pb_frequency = 5.0;	/*ffff */
	p->probe_p->ele_num_pri_r = GROUP_VAL_POS(group, probe.Elem_qty);	/* */
	p->probe_p->ele_num_sec_r = 1;	/* */
	p->probe_p->pri_axis_pitch_r = GROUP_VAL_POS(group, probe.Pitch) / -1000.0;	/* */
	p->probe_p->sec_axis_pitch_r = 0;	/* */
	p->probe_p->pri_ele_size_r = 1;	/*ffff */
	p->probe_p->sec_ele_size_r = 0;	/*ffff */
	/* 楔块 */
	p->wedge_p->wg_wedge_angle = GROUP_VAL_POS (group, wedge.Angle) / 10.0; /* 楔块角 度 */
	p->wedge_p->wg_roof_angle	= 0;	/* 顶角 度 还没有找到对应的 */
	p->wedge_p->wg_lon_vel		= GROUP_VAL_POS (group, wedge.Velocity_PA) / 1000.0;		/*纵波声速m/s*/
	p->wedge_p->wg_trans_vel	= GROUP_VAL_POS (group, wedge.Velocity_PA) / 1000.0;
	p->wedge_p->wg_density= 7.8;/* 密度 */


	p->wedge_p->wg_sec_elem_offset_fir = -fabs(GROUP_VAL_POS (group, wedge.Secondary_offset) / 1000.0);/*11111111111第一次轴阵元偏移mm*/
	p->wedge_p->wg_pri_axis_reference = 0;/*主轴楔块参考位置mm*/
	p->wedge_p->wg_sec_axis_reference = 0;/*次轴楔块参考位置mm*/
	p->wedge_p->wg_length = GROUP_VAL_POS(group, wedge.length) / 1000.0;
	p->wedge_p->wg_width = 1;/*楔块宽度mm*/
	p->wedge_p->wg_height = 1;/*楔块高mm*/
	p->wedge_p->wg_separation = 0;

	/* BEAM */
	p->beam_angle->beam_pri_steer_angle_start = 0 ;
	p->beam_angle->beam_pri_steer_angle_stop = 0;
	p->beam_angle->beam_pri_steer_angle_resolution = 0;
	//
	p->beam_angle->beam_sec_steer_angle_start = 0;
	p->beam_angle->beam_sec_steer_angle_stop = 0;
	p->beam_angle->beam_sec_steer_angle_resolution = 0;
	//
	p->beam_angle->beam_refrac_angle_start = LAW_VAL_POS (group, Angle_min) / 100.0;  
	p->beam_angle->beam_refrac_angle_stop  = LAW_VAL_POS (group, Angle_max) / 100.0;
	p->beam_angle->beam_refrac_angle_resolution = LAW_VAL_POS (group, Angle_step) / 100.0; 

	// 
	p->beam_angle->beam_skew_angle_start = 0;  
	p->beam_angle->beam_skew_angle_stop = 0;
	p->beam_angle->beam_skew_angle_resolution = 0;

	//与P600 Law Type 对应起来
	p->beam_angle->beam_type = LAW_VAL_POS(group,Focal_type);
	p->beam_angle->beam_angle_sel = 0;//只有Refracted angle一种情况 何凡修改
	/* 样本 */
	p->specimen->speci_longitudinal_wave	= get_group_val (get_group_by_id (pp->p_config, group), GROUP_VELOCITY) / 100;
	p->specimen->speci_transverse_wave		= get_group_val (get_group_by_id (pp->p_config, group), GROUP_VELOCITY) / 100;
	p->specimen->speci_transverse_wave		= get_group_val (get_group_by_id (pp->p_config, group), GROUP_VELOCITY) / 100;
	p->specimen->flat_cylindrical = get_part_geometry(group);
	p->specimen->speci_outside_cylindrical = get_part_diameter(group)/1000.0;
	/* 聚焦点 */
	double scan_offset   = GROUP_VAL_POS(group , scan_offset)  / 10.0;
	double index_offset  = GROUP_VAL_POS(group , index_offset) / 10.0;
	int skew_angle       = GROUP_VAL_POS(group , skew)  ;
    if(skew_angle >= 0 && skew_angle < 9000)
    {
    	p->focal_point->offset_start = LAW_VAL_POS(group , Offset_start)/1000.0 - scan_offset;// focus depth for projection and focus plane
    	p->focal_point->offset_end   = LAW_VAL_POS(group , Offset_end)/1000.0  -  scan_offset;//
    }
    else if(skew_angle >= 9000 && skew_angle < 18000)
    {
    	p->focal_point->offset_start = LAW_VAL_POS(group , Offset_start)/1000.0 - index_offset;// focus depth for projection and focus plane
    	p->focal_point->offset_end   = LAW_VAL_POS(group , Offset_end)/1000.0  -  index_offset;//
    }
    else if(skew_angle >= 18000 && skew_angle < 27000)
	{
    	p->focal_point->offset_start = - (LAW_VAL_POS(group , Offset_start)/1000.0 - scan_offset);// focus depth for projection and focus plane
    	p->focal_point->offset_end   = - (LAW_VAL_POS(group , Offset_end)/1000.0  -  scan_offset);//
	}
    else
    {
    	p->focal_point->offset_start = - (LAW_VAL_POS(group , Offset_start)/1000.0 - index_offset);// focus depth for projection and focus plane
    	p->focal_point->offset_end   = - (LAW_VAL_POS(group , Offset_end)/1000.0  -  index_offset);//
    }

	p->focal_point->focal_focus_type = LAW_VAL_POS(group , Focal_point_type);	/* 0 half path 1 TURE DEPTH */
	//printf("LAW_VAL_POS(group,Focal_point_type)  %d \n" , LAW_VAL_POS(group,Focal_point_type));
	//  p->focal_point->focal_focus_point_start = LAW_VAL_POS (group, Focus_depth) / 1000.0;	/* type =0 是 声程 type =1 是深度 */   
	p->focal_point->focal_focus_point_start = LAW_VAL_POS(group , Position_start)/ 1000.0;	/* type =0 是 声程 type =1 是深度 */
	p->focal_point->focal_focus_point_stop = LAW_VAL_POS(group , Position_end)/ 1000.0;
	p->focal_point->focal_focus_point_resolution = LAW_VAL_POS(group , Position_step)/ 1000.0;
	//p->focal_point->offset_start = LAW_VAL_POS(group , Offset_start)/1000.0 ;// focus depth for projection and focus plane
	//p->focal_point->offset_end   = LAW_VAL_POS(group , Offset_end)/1000.0;//
	p->focal_point->depth_start  = LAW_VAL_POS(group , Depth_start)/1000.0;//
	p->focal_point->depth_end    = LAW_VAL_POS(group , Depth_end)/1000.0;//

	p->element_sel->pri_axis_ape = LAW_VAL_POS (group, Elem_qty);
	p->element_sel->sec_axis_ape = 1;
	p->element_sel->primary_axis_s = LAW_VAL_POS (group, First_tx_elem);
	p->element_sel->primary_axis_e = LAW_VAL_POS (group, Last_tx_elem) - LAW_VAL_POS (group, Elem_qty)+1;//
	p->element_sel->primary_axis_r = LAW_VAL_POS (group, Elem_step);//

	//处理探头反装的情况
	if(GROUP_VAL_POS(group , wedge.Orientation))// wedge inverse
	{
		p->probe_p->pri_axis_pitch = GROUP_VAL_POS(group, probe.Pitch) / 1000.0;	/* */
		p->wedge_p->wg_heigh_fir	= GROUP_VAL_POS (group, wedge.Height) / 1000.0;	/*第一阵元高度mm*/
		p->wedge_p->wg_pri_elem_offset_fir = -fabs(GROUP_VAL_POS (group, wedge.Primary_offset) / 1000.0);/*11111111111第一主轴阵元偏移mm*/
	}
	else // wedge normal
	{
		p->probe_p->pri_axis_pitch = GROUP_VAL_POS(group, probe.Pitch) / -1000.0;	/* */
		p->wedge_p->wg_heigh_fir	= GROUP_VAL_POS (group, wedge.Height) / 1000.0 +
					sin(p->wedge_p->wg_wedge_angle * G_PI / 180.0) * (GROUP_VAL_POS(group , probe.Elem_qty)-1) *  GROUP_VAL_POS(group, probe.Pitch) / 1000.0 ;	/*第一阵元高度mm*/
		p->wedge_p->wg_pri_elem_offset_fir = -fabs(GROUP_VAL_POS (group, wedge.Primary_offset) / 1000.0) +
				cos(p->wedge_p->wg_wedge_angle * G_PI / 180.0) * (GROUP_VAL_POS(group , probe.Elem_qty)-1) *  GROUP_VAL_POS(group, probe.Pitch) / 1000.0 ;/*11111111111第一主轴阵元偏移mm*/
	}
}

static void save_cal_law(int group, PARAMETER_P p)
{
	int i, j,k;
	int ElementStart;
	int ElementStop ;

	GROUP *p_grp = get_group_by_id (pp->p_config, group);
	for (i = 0; i < TMP(beam_qty[group]); i++)
	{
		//***************************************
		GROUP_VAL_POS(group , field_distance[i]) = p->field_distance[i];//每束 中心正元到出射点的距离 单位mm
		TMP(beam_delay[group][i]) = p->G_delay[i]  ;
		//***************************************
		if(LAW_VAL_POS(group, Focal_type) == 1)//Linear
		{
			ElementStart = LAW_VAL_POS (group, First_tx_elem) + i*LAW_VAL_POS (group, Elem_step) - 1 ;
			ElementStop  = ElementStart + LAW_VAL_POS (group, Elem_qty) ;
		}
		else
		{
			ElementStart = LAW_VAL_POS (group, First_tx_elem) - 1 ;
			ElementStop  = ElementStart + LAW_VAL_POS (group, Elem_qty) ;
		}
		TMP(focal_law_all_beam[group][i]).N_ActiveElements	= LAW_VAL_POS (group, Elem_qty);
        TMP(focal_law_all_beam[group][i]).frequency		= group_get_rx_tx_mode(group);
		TMP(focal_law_all_beam[group][i]).cycle			= 1;
		TMP(focal_law_all_beam[group][i]).mode			=
            !group_get_rx_tx_mode(group);	/* 0 PE 1 PT 与我的相反 */
		TMP(focal_law_all_beam[group][i]).filter			=
			get_group_val (get_group_by_id (pp->p_config, group), GROUP_FILTER);
		TMP(focal_law_all_beam[group][i]).R_angle			= (LAW_VAL_POS (group, Angle_min) +
				LAW_VAL_POS(group, Angle_step) * i) / 100.0;
		TMP(focal_law_all_beam[group][i]).S_angle			= GROUP_VAL_POS (group, skew) / 100.0;
		TMP(focal_law_all_beam[group][i]).T_first			= LAW_VAL_POS (group, First_tx_elem);
		TMP(focal_law_all_beam[group][i]).R_first			= LAW_VAL_POS (group, First_rx_elem);
		TMP(focal_law_all_beam[group][i]).Scan_offset		= GROUP_VAL_POS (group, scan_offset) * 100;
		TMP(focal_law_all_beam[group][i]).Index_offset   	= GROUP_VAL_POS (group, index_offset) * 100;
		TMP(focal_law_all_beam[group][i]).G_delay			= GROUP_VAL_POS(group , beam_delay[i])    ;
		TMP(focal_law_all_beam[group][i]).F_depth			= LAW_VAL_POS (group, Focus_depth);
		TMP(focal_law_all_beam[group][i]).M_velocity		= get_group_val (get_group_by_id (pp->p_config, group), GROUP_VELOCITY) / 100;
		TMP(focal_law_all_beam[group][i]).used1			    = ((unsigned int)p->focus_y[i])&0xffff;

		for(k=ElementStart,j = 0; k< ElementStop; k++,j++)
		{ 
			TMP(focal_law_all_elem[group][i][j]).E_number = j + 1;
            TMP(focal_law_all_elem[group][i][j]).FL_gain	= group_get_gain(group) / 100;
			TMP(focal_law_all_elem[group][i][j]).T_delay	= p->timedelay[i][k];
			TMP(focal_law_all_elem[group][i][j]).R_delay	= p->timedelay[i][k];
			TMP(focal_law_all_elem[group][i][j]).Amplitude = get_voltage (pp->p_config, get_current_group(pp->p_config));
			TMP(focal_law_all_elem[group][i][j]).P_width	=
				get_group_val (p_grp, GROUP_PW_VAL) / PW_DIV;
		} 
	}
	TMP(Junction[group]) = p->Junction ; // 实际已经没有什么用处，待PC上软件S SCAN 画图更改后，删除
}

void cal_focal_law (guint group)
{
	static PARAMETER _scParameter ;
	PARAMETER_P p;
	PROBEF        probe_p;
	WEDGEF        wedge_p;
	WEDGEF        wedge_r;
	BEAM_ANGLE   beam_angle;
	SPECIMEN     specimen;
	FOCAL_POINT  focal_point;
	ELEMENT_SEL  element_sel;
	LOCATION     location;

	if(GROUP_VAL_POS(group , group_mode) == PA_SCAN )
	{
		p = &_scParameter ;
		p->probe_p	= &probe_p;
		p->wedge_p	= &wedge_p;
		p->wedge_r	= &wedge_r;
		p->beam_angle = &beam_angle;
		p->specimen	= &specimen;
		p->focal_point = &focal_point;
		p->element_sel = &element_sel;
		p->location = &location;
		p->k = 0 ;
		setup_para(p, group);
		focal_law(p);
		/* 把聚集法则信息保存起来 */
		save_cal_law (group, p);
	}
	else
	{
		GROUP_VAL_POS(group , beam_delay[0])  = 0 ;
		TMP(focal_law_all_beam[group][0]).G_delay = 0 ;
		TMP(focal_law_all_elem[group][0][0]).T_delay	= 0;
		TMP(focal_law_all_elem[group][0][0]).R_delay	= 0;
		LAW_VAL_POS(group , Angle_min) = GROUP_VAL_POS (group, wedge.Angle) * 10 ;
		LAW_VAL_POS(group , Angle_max) = GROUP_VAL_POS (group, wedge.Angle) * 10 ;
		TMP(current_angle[group]) = GROUP_VAL_POS (group, wedge.Angle) * G_PI / 1800.0 ;
		GROUP_VAL_POS(group , field_distance[0]) = -fabs(GROUP_VAL_POS (group, wedge.Ref_point)/1000.0)  ;
		GROUP_VAL_POS(group , wedge_delay) = GROUP_VAL_POS (group, wedge.Probe_delay) ;
	}
	return ;
}


int IsDisplayModalAvailable(unsigned char nDisplayType_)
{
	int ret ;

	if ((get_display_group(pp->p_config) == DISPLAY_CURRENT_GROUP) || (get_group_qty(pp->p_config)==1))
	{
		if(nDisplayType_ == PA_TOFD || nDisplayType_ == PA_TOFD_CSCAN)
		{
			return Is_PATOFD_DisplayValidate() ;
		}
		ret = TRUE ;
	}
	else
	{
        //if ((nDisplayType_ == PA_TOFD || nDisplayType_ == PA_TOFD_CSCAN) && (get_group_qty(pp->p_config)<4))
        if (nDisplayType_ == PA_TOFD || nDisplayType_ == PA_TOFD_CSCAN)
		{
			return Is_PATOFD_DisplayValidate() ;
		}
		switch (get_group_qty(pp->p_config))
		{
			case 2:
				switch (nDisplayType_)
				{
					case A_SCAN:
					case B_SCAN:
					case S_SCAN:
                    case A_B_SCAN:
					case A_C_CC_SCAN:
					case A_S_CC_SCAN:
						ret = TRUE ;
						break;
					default:
						ret = FALSE ;
						break;
				}
				break;
			case 3:
				switch (nDisplayType_)
				{
					case A_SCAN:
					case B_SCAN:
					case S_SCAN:
                    case A_B_SCAN:
					case A_C_CC_SCAN:
					case A_S_CC_SCAN:
						ret = TRUE  ;
						break;
					default:
						ret = FALSE ;
						break;
				}
				break;
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
				switch (nDisplayType_)
				{
					case A_SCAN:
						ret = TRUE  ;
						break;
					default:
						ret = FALSE ;
						break;
				}
				break;
			default:break;
		}
	}

	return ret ;
}

int IsGateSynchro()
{
	unsigned char _bSynchroA , _bSynchroB ;
	int _nGroupQty = get_group_qty(pp->p_config);
	int _bNeedRedraw =  0 ;
	int i;
	for(i = 0 ; i < _nGroupQty ; i++)
	{
		_bSynchroA = GROUP_VAL_POS(i , gate[0].synchro) ;
		_bSynchroB = GROUP_VAL_POS(i , gate[1].synchro) ;
		if(_bSynchroA || _bSynchroB )
		{
			_bNeedRedraw = TRUE ;
			break;
		}
	}
	return _bNeedRedraw ;
}

void MultiGroupRefreshIdelTime()
{
	int _nGroupQty ;
	int _nBeamQty  ;
	int _nBeamTime ;
	int _nPrf      ;
	int i ;

	_nGroupQty = get_group_qty(pp->p_config);
	_nBeamQty  = get_beam_qty();

	GROUP_VAL_POS(0 , prf1) = get_prf() ;
	_nPrf  =  GROUP_VAL_POS(0 , prf1) / 10;
	_nBeamTime = 100000000 / (_nBeamQty * _nPrf) - 2048 ;
	for(i = 0 ; i < _nGroupQty ; i++)
	{
		TMP(group_spi[i]).idel_time = _nBeamTime - TMP(group_spi[i]).rx_time ;
	}
}

int GetGroupMaxBeamDelay(int nGroupId_)
{
	int _nMaxBeamDelay ;
	int _nBeamQty ;
	int i ;
	_nBeamQty = TMP(beam_qty[nGroupId_]) ;
	_nMaxBeamDelay =  GROUP_VAL_POS(nGroupId_ , beam_delay[0]);
	for(i = 1 ; i < _nBeamQty ; i++ )
	{
		if(_nMaxBeamDelay < GROUP_VAL_POS(nGroupId_ , beam_delay[i]))
		{
			_nMaxBeamDelay = GROUP_VAL_POS(nGroupId_ , beam_delay[i]);
		}
	}
	// unit : 10 ns
	return _nMaxBeamDelay / 10;
}

int GetGroupRxTime(int GroupId_)
{
	int _nRxTime ;
	int _nSampleRange ;
	int _nMaxBeamDelay;

    _nSampleRange	= (group_get_start(GroupId_) +
			+ GROUP_VAL_POS(GroupId_ , range)
			+ GROUP_VAL_POS(GroupId_ , wedge_delay)) / 10;
	_nMaxBeamDelay  = GetGroupMaxBeamDelay(GroupId_);

	_nRxTime = _nSampleRange + _nMaxBeamDelay + 5 ;
	return _nRxTime   ;
}


double GetGainLimit(int nGroupId_)
{
#if HIGH_POWER
	return 120;
#else
    if (PA_SCAN == GROUP_VAL_POS(nGroupId_, group_mode)
                 || UT_SCAN == GROUP_VAL_POS(nGroupId_, group_mode)) {
        return PA_MAX_GAIN;
    } else {
        return UT_MAX_GAIN;
    }
#endif
    /*
	unsigned char dac_point_qty = GROUP_VAL_POS(nGroupId_ , SizingCurves.dac_point_qty) ;
	int _nBeamNo = TMP(beam_num[nGroupId_]) ;

	double ret; double _nResult[16] ;
	int i ;
	for(i = 0 ; i < dac_point_qty ; i++ )
	{
		_nResult[i]  = 	GROUP_VAL_POS(nGroupId_ , SizingCurves.amplitude[_nBeamNo][0])
					/ ((double) GROUP_VAL_POS(nGroupId_ , SizingCurves.amplitude[_nBeamNo][i] ) ) ;
	}
	ret = _nResult[0] ;
	for(i = 1 ; i < dac_point_qty ; i++)
	{
		if(_nResult[i] > ret)  ret = _nResult[i] ;
	}
	//ret = GROUP_VAL_POS(nGroupId_ , SizingCurves.amplitude[_nBeamNo][0])
	//		/ ((double) GROUP_VAL_POS(nGroupId_ , SizingCurves.amplitude[_nBeamNo][dac_point_qty - 1 ] ) ) ;
	ret = log10 ( ret ) * 20 ;
	ret = 80 - ret ;
	if(ret < 0) ret = 0 ;
	if(ret > 80)  ret = 80 ;
	return ret;
    */
}


/* 取整函数 */
int rounding(int src1, int src2, int round)
{
	if (src1 == src2)
		return src2;
	else if (src1 < src2)
		return (((src2 - src1 + round / 2) / round) * round + src1);
	else if (src1 > src2)
		return (src1 - ((src1 - src2 + round / 2) / round) * round);
	else 
		return 0;
}

int GetGroupBeamQty(int grp)
{
	int _nResult  = 1;
	if(GROUP_VAL_POS(grp , group_mode) == PA_SCAN)
	{
		if (LAW_VAL_POS (grp , Focal_type) == AZIMUTHAL_SCAN)
		{
			_nResult = (LAW_VAL_POS(grp, Angle_max) - LAW_VAL_POS(grp, Angle_min)) /
				LAW_VAL_POS(grp, Angle_step) + 1;
		}
		else// if(LAW_VAL_POS (grp , Focal_type) == LINEAR_SCAN)
		{
			_nResult = (int)( ( LAW_VAL_POS (grp, Last_tx_elem) - LAW_VAL_POS(grp, First_tx_elem) - LAW_VAL_POS(grp, Elem_qty) + 1 ) /
					LAW_VAL_POS(grp, Elem_step) ) + 1;
		}
	}

	return _nResult;
}

int	get_beam_qty()
{
	int i, beam_qty = 0;
	for (i = 0; i < get_group_qty(pp->p_config); i++)
		beam_qty += GetGroupBeamQty(i) ; //TMP(beam_qty[i]);
	return beam_qty;
}


int get_current_proper_point_qty (int grp)
{
	double float_compress_rate ;
	int    int_compress_rate   ;
	int ret ;

	if(GROUP_VAL_POS(grp , range) <= 6400)
	{
		ret = GROUP_VAL_POS(grp , range) / 10 ;
	}
	else
	{
		float_compress_rate = GROUP_VAL_POS(grp , range) / 6400.0 ;
		int_compress_rate   = ( GROUP_VAL_POS(grp , range) % 6400) ? (int) (float_compress_rate + 1) : (int)float_compress_rate ;
		ret = GROUP_VAL_POS(grp , range) / int_compress_rate ;
		ret /= 10 ;
	}

	return ret ;
}
/* 取得压缩点的数量 */
void AutoSetPointQty (int grp)
{
	if (GROUP_VAL_POS(grp , point_qty_pos) == 4)
	{
		//return GROUP_VAL_POS(grp , point_qty);
	}
	else 
	{
		switch (GROUP_VAL_POS(grp , point_qty_pos))
		{
			case 0:GROUP_VAL_POS(grp , point_qty) = MIN(StoreBuffLimitToGroupPointQty(grp), get_current_proper_point_qty(grp));break;
			case 1:GROUP_VAL_POS(grp , point_qty) = MIN(StoreBuffLimitToGroupPointQty(grp), 160);break;
			case 2:GROUP_VAL_POS(grp , point_qty) = MIN(StoreBuffLimitToGroupPointQty(grp), 320);break;
			case 3:GROUP_VAL_POS(grp , point_qty) = MIN(StoreBuffLimitToGroupPointQty(grp), 640);break;
			default:break;
		}
	}
	//return GROUP_VAL_POS(grp , point_qty);
}

// the PRF is needed to be larger than 1
// return : the sample time in 10nm
int PrfLimitToGroupSampleRange(int GroupId_)
{
	int _nBeamQty = get_beam_qty();
	int _nMaxBeamDelay = GetGroupMaxBeamDelay(GroupId_) ;
	unsigned int _nResult ;
	// 100000000.0 / 4    idle_time + rx_time >= 4 * rx_time
	// one beam cycle = 2048(loading time) +  beam delay + wedge delay + sample start + sample range + 5
	_nResult   =  25000000.0 / _nBeamQty - 2048  - _nMaxBeamDelay - 5 ;
    _nResult -= GROUP_VAL_POS(GroupId_ , wedge_delay) / 10 ;
	if(_nResult > 100000)
	{
		_nResult = 100000 ;
	}
	return _nResult ;
}

// the max storage of the beam data is 192*1024
// so if the beam_qty is to big
// the sample data must be reduce
int StoreBuffLimitToGroupPointQty(int GroupId_)
{
	int _nGroupQty ;
	int _nCurrentGroupBeamQty  ;
	int _nDataSize ;
	int _nResult   ;
	int i ;

	_nGroupQty = get_group_qty(pp->p_config);
	_nCurrentGroupBeamQty = GetGroupBeamQty(GroupId_) ; //TMP(beam_qty[GroupId_]) ;

	_nDataSize = 0 ;
	for(i = 0 ; i < _nGroupQty ; i++)
	{
		if(i != GroupId_)
		{
			_nDataSize +=  (GROUP_VAL_POS(i , point_qty) + 32) * TMP(beam_qty[i]) ;
		}
	}
	_nDataSize = 192 * 1024 - _nDataSize ;
	_nResult = _nDataSize / _nCurrentGroupBeamQty - 32 ;

	if(_nResult > 8192)
	{
		_nResult = 8192 ;
	}
	return _nResult ;
}
int GetGroupVoltage(int GroupId_)
{
	int _nVoltage ;
	int _nVoltagePos ;
	_nVoltagePos  = get_voltage(pp->p_config , GroupId_) ;
	if(_nVoltagePos == 0)
	{
		_nVoltage = 50 ;
	}
	if(_nVoltagePos == 1)
	{
		_nVoltage = 100 ;
	}
	if(_nVoltagePos == 2 )
	{
		_nVoltage = 200 ;
	}

	return _nVoltage ;
}


// Max PRF when there is a Group model PA or UT
int MultiGroupGetMaxPrfPA()
{
	int i ;
	int _nResult   ;
	int _nBeamQty  ;
	int _nGroupQty ;
	int _nMaxRxTime ;
	int _nTotalDataSize ;
	unsigned char _nGroupModel ;
	int _nPulseWidth[setup_MAX_GROUP_QTY];
	int _nWedgeDelay[setup_MAX_GROUP_QTY];
	int _nProbeElementQty[setup_MAX_GROUP_QTY];
	int _nVoltage[setup_MAX_GROUP_QTY] 		 ;
	int _nRxTime[setup_MAX_GROUP_QTY] 		 ;

	double _nPrfLimit[3]  ;
	// get group number
	_nGroupQty = get_group_qty (pp->p_config) ;
	// get total beam qty
	_nBeamQty = get_beam_qty(pp->p_config);
	_nTotalDataSize = GetTotalDataSize() ;
	// get parameters of each group
	for(i = 0 ; i < _nGroupQty ; i++)
	{
		_nPulseWidth[i] = GROUP_VAL_POS(i , pulser_width1) / PW_DIV;
		_nWedgeDelay[i] = GROUP_VAL_POS(i , wedge_delay) ;
		_nVoltage[i]    = GetGroupVoltage(i);
		_nRxTime[i]     = GetGroupRxTime(i) ;
		_nProbeElementQty[i] = LAW_VAL_POS(i, Elem_qty) ;
	}
	// the rx_time used to calculate PRF is the max rx_time of each group
	// then the idel_time will be 3*max_rx_time minus current group rx_time
	_nMaxRxTime = _nRxTime[0] ;
	for(i = 1; i< _nGroupQty; i++)
	{
		if(_nMaxRxTime < _nRxTime[i])
		{
			_nMaxRxTime = _nRxTime[i]  ;
		}
	}
	//single_beam_time = 4 * max_rx_time                        ;   // unit -- 10nm
	_nPrfLimit[0]   =  25000000.0 / (_nMaxRxTime * _nBeamQty )  ;
	int _nSingleElementPower = 0;
	double _nTotalElementPower = 0.0 ;// 不能用INT型  会越界
	double _nTmpPower = 0 ;
	for(i = 0 ; i < _nGroupQty ; i++)
	{
		_nGroupModel = GROUP_VAL_POS(i , group_mode) ;
		if(_nGroupModel == PA_SCAN || _nGroupModel == UT_SCAN)
		{
			_nTmpPower = ((double)_nPulseWidth[i]) * ((double)_nVoltage[i]) * ((double)_nVoltage[i]) / PROBE_RESISTANCE ;
			_nSingleElementPower +=  GetGroupBeamQty(i) * _nTmpPower ;
			_nTotalElementPower += _nProbeElementQty[i] * GetGroupBeamQty(i) * _nTmpPower ;
		}
	}
	if(_nSingleElementPower)
		_nPrfLimit[1]  =  125000000.0 / _nSingleElementPower ;
	else
		_nPrfLimit[1]  =  6000 ;

	if(_nTotalElementPower)
		_nPrfLimit[2]  =  (int)(2000000000 / _nTotalElementPower) ;
	else
		_nPrfLimit[2]  =  6000 ;
	// get minimum result
	_nResult = _nPrfLimit[0]  ;
	for(i = 1 ; i < 3 ; i ++)
	{
		if(_nResult > _nPrfLimit[i])
		{
			_nResult = _nPrfLimit[i] ;
		}
	}
	// total prf limit
	if(_nResult * _nBeamQty > 20000)  _nResult = 20000 / _nBeamQty   ;
	// total data size limit
	if(_nResult * _nTotalDataSize > 8 *1024 * 1024)  _nResult = 8 * 1024 * 1024 / _nTotalDataSize  ;
	// max interrupt limit
	if(_nResult > 6000)  _nResult = 6000 ;
	return _nResult * 10;
}


int MultiGroupGetMaxPrf()
{
	int ret = MultiGroupGetMaxPrfPA() ;
	if(ret < 10)   ret = 10 ;
	return  ret ;
}


/* 计算prf,并且附加限制 限制计算 */
int get_prf ()
{
	int _nResult ;
	int	_nPrfTmp = MultiGroupGetMaxPrf();
	int _nPrfPos = GROUP_VAL_POS(0 , prf_pos1)  ;

	switch (_nPrfPos)
	{
		case 0:
			_nResult = _nPrfTmp ;
			break;
		case 1:
			if(_nPrfTmp >= 20)
				_nResult = _nPrfTmp / 2 ;
			else
				_nResult = 10 ;
			break;
		case 2:
			if(_nPrfTmp > 600)
			{
				_nPrfTmp = 600 ;
			}
			_nResult = _nPrfTmp ;
			break;
		case 3:
			_nResult = GROUP_VAL_POS(0 , prf1) ;
			if (_nResult > _nPrfTmp )
			{
				_nResult = _nPrfTmp ;
			}
			break;
		default:break;
	}

	return (_nResult/10)*10;
}


int CheckGroupConfigureAvailable()
{
	int i ;int _nBeamQty  ;

	int _nGroupQty = get_group_qty(pp->p_config) ;//get_current_group(pp->p_config);
	for(i = 0 ; i< _nGroupQty ; i++)
	{
		_nBeamQty = GetGroupBeamQty(i) ;//TMP(beam_qty[i]) ;
		if(_nBeamQty > 256)  return TRUE ;
	}

	_nBeamQty = get_beam_qty(pp->p_config);
	if(_nBeamQty > 1024)  return TRUE ;

	int _nTotalDataSize = GetTotalDataSize() ;
	if(_nTotalDataSize >= 192 * 1024) return TRUE  ;

	int _nMaxPrf = MultiGroupGetMaxPrfPA() ;
	if(_nMaxPrf < 10)  return TRUE ;

	return FALSE ;
}

void ClearOneGroupParameters(int nGroupId_)
{
	GROUP* _pGroup = get_group_by_id(pp->p_config , nGroupId_) ;
	memset((void*)_pGroup  , 0 , sizeof(GROUP));
}


void MultiGroupAdd()
{
	int _nGroupQty;
	_nGroupQty = get_group_qty(pp->p_config);
	int response = dialogWarning(pp->window, getDictString(_STRING_COPY_GROUP_QUEST));

	set_group_qty (pp->p_config, _nGroupQty + 1);
	if(response)
	{
		grpcpy (pp->p_config, _nGroupQty, 0);
	}
	else
	{
		ClearOneGroupParameters(_nGroupQty);
		SettingGroupConfigure (_nGroupQty);
	}

	if(CheckGroupConfigureAvailable())
	{
		set_group_qty (pp->p_config, _nGroupQty);
		dialogError(pp->window, getDictString(_STRING_GROUP_ADD_NOT_AVAILABEL));
		return ;
	}
	pp->p_config->groupId = _nGroupQty ;

	GROUP_VAL_POS(_nGroupQty , WedgeDelayCalibrated) = 0 ;
	GROUP_VAL_POS(_nGroupQty , SensationCalibrated) =  0  ;
	generate_focallaw (_nGroupQty);
	RefreshBeamDelay(_nGroupQty);
	RefreshGainOffset(_nGroupQty);
	InitSizingCurveParameters(_nGroupQty) ;
	MultiGroupSendGroupSpi();
	MultiGroupSendFocalSpi();
	RefreshScanInfor() ;
	//pp->bRefreshDraw = TRUE ;
}


void MultiGroupDelete()
{
	int _nGroupQty;
	_nGroupQty = get_group_qty(pp->p_config);
	if (_nGroupQty <=  1) return ;

	_nGroupQty -= 1 ;
	set_group_qty (pp->p_config, _nGroupQty);
	_nGroupQty -= 1 ;
	pp->p_config->groupId = _nGroupQty ;
	MultiGroupSendGroupSpi();
	MultiGroupSendFocalSpi();
	RefreshScanInfor() ;
	//pp->bRefreshDraw = TRUE ;
}

/* 计算脉冲宽度 */
double get_pw ()
{
	int grp = get_current_group (pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	double _nResult ;

	if (get_group_val (p_grp, GROUP_PW_POS))
	{
		_nResult = (get_group_val (p_grp, GROUP_PW_VAL)) / (double)PW_DIV;
	}
	else
	{
		_nResult = ( 500000 / get_group_val (p_grp, GROUP_FREQ_VAL)) ;
	}
    _nResult = (unsigned int)(_nResult / 2.5 );
    _nResult *= 2.5 ;

	return _nResult ;
}

// get the most close filter according to probe frequency
int get_auto_filter(unsigned int grp)
{
	int ret;

	int _nFrequency = GROUP_VAL_POS (grp, frequency1) ;
	if (_nFrequency <= 1250)        ret = 1 ;
	else if (_nFrequency <= 2750)   ret = 2 ;
	else if (_nFrequency <= 6250)   ret = 3 ;
	else if (_nFrequency <= 8750)   ret = 4 ;
	else                            ret = 5 ;
	return ret ;
}

/* 计算滤波 0 1 None 和 Auto 时候怎么计算 */
void set_filter (int grp)
{
	int tmp = GROUP_VAL_POS (grp, filter_pos1);
	switch (tmp) 
	{
		case 0:
			TMP(group_spi[grp]).freq_band	= 0 ;
			break;
		case 1:
			TMP(group_spi[grp]).freq_band	= get_auto_filter ( grp );
			break;
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		//case 7:
		//case 8:
		//case 9:
		//case 10:
		//case 11:
		//case 12:
		//case 13:
		//case 14:
			TMP(group_spi[grp]).freq_band = tmp - 1 ;
			break;
		default:break;
	}
}

guint get_freq ()
{
	int grp = get_current_group (pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	switch (get_group_val (p_grp, GROUP_FREQ_POS))
	{
		case 0:return 1000;break;
		case 1:return 1500;break;
		case 2:return 2000;break;
		case 3:return 2250;break;
		case 4:return 3500;break;
		case 5:return 4000;break;
		case 6:return 5000;break;
		case 7:return 7500;break;
		case 8:return 10000;break;
		case 9:return 12000;break;
		case 10:return 15000;break;
		case 11:return 20000;break;
		default:break;
	}
	return get_group_val (p_grp, GROUP_FREQ_VAL);
}

/* 返回skew */
guint get_skew (int grp)
{
	switch (GROUP_VAL_POS(grp , skew_pos))
	{
		case 0:return 0;break;
		case 1:return 9000;break;
		case 2:return 18000;break;
		case 3:return 27000;break;
		default:break;
	}
	return GROUP_VAL_POS(grp , skew);
}

int cal_menu3_qty()
{
	switch (pp->pos)
	{
		case 0:
			switch (pp->pos1[pp->pos])
			{
				case 0:return 2;break;
				case 1:return 2;break;
				case 2:return 2;break;
				default:break;
			}
			break;
		case 1:
			break;
		default:break;

	}
	return 0;
}

/* 设置LABEL特别显示的函数 字符串 */
void tt_label_show_string (GtkWidget *label, const char *s1, const char *s2,
		const char *s3, const char *color,  guint font_size)
{
	char *markup;

	markup = g_markup_printf_escaped(
			"<span foreground=\'%s\' font_desc=\'%d\'>%s%s%s</span>", color, font_size, s1, s2, s3);
	gtk_label_set_markup (GTK_LABEL (label), markup);

	g_free (markup);
}

/* 设置LABEL特别显示的函数 字符串与数值 */
void tt_label_show_float (GtkWidget *label, const char *s1, double value, guint digit,
		const char *s3, const char *color,  guint font_size)
{
	char *markup;

	markup = g_markup_printf_escaped(
			"<span foreground=\'%s\' font_desc=\'%d\'>%s%0.*f%s</span>", color, font_size, s1, digit,  value, s3);
	gtk_label_set_markup (GTK_LABEL (label), markup);

	g_free (markup);
}

/*二级菜单按钮的回调函数*/
void b2_fun0(DRAW_UI_P p, guint pos)
{
    if(pp->pos_pos == MENU3_PRESSED)
    {
    	pp->pos_pos = MENU3_STOP ;
    	draw_menu3(0, NULL);
    }

	gLastMenuPos.pos.menu2 = gCurrentMenuPos.pos.menu2;
	gCurrentMenuPos.pos.menu2 = pos;
	gCurrentMenuPos.pos.menu3 = CUR_POS_P(p);
//	gCurrentMenuPos.pos.menu4 = 0;
	if((8 == gCurrentMenuPos.pos.menu1)
	&&(1 == gCurrentMenuPos.pos.menu2))
	{
	    time_t timep;
	    struct tm *p;
	    time(&timep);
	    p=localtime(&timep);
	    char str[100];
	    memset(str ,0 ,100);
	    sprintf(str ,"Report_%d-%02d-%02d_%02d.%02d.%02d" ,1900 + p->tm_year ,1 + p->tm_mon ,p->tm_mday ,p->tm_hour ,p->tm_min ,p->tm_sec);
		set_file_name_info(pp->p_config ,str);
	}
	if(gtk_widget_get_visible(p->eventbox2[pos]))
	{
		if (p->pos1[p->pos] != pos)
		{
			gLastMenuPos.pos.menu3 = 0xff;
			p->pos_last1 = p->pos1[p->pos];
			p->pos1[p->pos] = pos;
			p->pos_last2 = 0xff;
			menu2Update();
		}

		if((p->pos == 0) && (p->pos1[p->pos] == 2))//Calibration
		{
			//	gtk_widget_set_sensitive(p->eventbox2[0],FALSE);
			//	gtk_widget_set_sensitive(p->eventbox2[1],FALSE);
			//	gtk_widget_set_sensitive(p->eventbox2[2],FALSE);
			//	gtk_widget_set_sensitive(p->menubar,FALSE);
			//	pp->clb_flag = 1;
			pp->cstart_qty = 1;
			//	draw_menu3(0, NULL);
		}
	}
}

gboolean eventbox2_function0 (GtkWidget *widget, GdkEventButton *event,	gpointer data)
{
	guint p = GPOINTER_TO_UINT (data);
	b2_fun0(pp, p);
	return TRUE;
}

static inline void data_process(unsigned char* data, guint pa)
{
	//(*data > 0) ? (*data)-- : (*data = pa);
	(*data < pa) ? (*data)++ : (*data = 0);
	return ;
}

/*6个数值区域共有的处理函数 */
/* 第一个数值按键 快捷键是F12  */
void b3_fun0(gpointer pt)
{
	DRAW_UI_P p = pp;
	/* 之前的位置 */
//	p->pos_last2 = p->pos2[p->pos][p->pos1[p->pos]];
//	p->pos2[p->pos][p->pos1[p->pos]] = 0;
	/* 单击处理 */
	switch (p->pos)
	{
		case 0: /* 向导 Wizard */
			switch (p->pos1[0])
			{
				case 0:	/* P000 什么向导 */
					break;
				case 1:
					break;
				case 2:
					switch(p->ctype_pos)
					{
						case 0://编码器
							((p->cstart_qty) > 1) ? (p->cstart_qty) -- : ((p->cstart_qty) = 4);
							break;
						case 1://超声
							switch(p->cmode_pos)
							{
								case 0://声速
									((p->cstart_qty) > 1) ? (p->cstart_qty) -- : ((p->cstart_qty) = 6);
									break;
								case 1://延迟
									((p->cstart_qty) > 1) ? (p->cstart_qty) -- : ((p->cstart_qty) = 5);
									if(PA_SCAN != get_group_by_id (pp->p_config, get_current_group (pp->p_config))->group_mode)//UT
									{
										if(3 == p->cstart_qty)//没有角度设置
										{
											--(p->cstart_qty);
										}
									}
									break;
								case 2://灵敏度
									((p->cstart_qty) > 1) ? (p->cstart_qty) -- : ((p->cstart_qty) = 6);
									break;
								case 3://TCG
									((p->cstart_qty) > 1) ? (p->cstart_qty) -- : ((p->cstart_qty) = 5);
									break;
								default:break;
							}
							break;
						case 2://标准
							((p->cstart_qty) > 1) ? (p->cstart_qty) -- : ((p->cstart_qty) = 5);
							break;
						default:break;
					}
					if(p->cstart_qty == 1)
					{
						esc_calibration();
					}
					break;
				case 3:
					((p->wstart_qty) > 1) ? (p->wstart_qty) -- : ((p->wstart_qty) = 4);
				/*	if(p->wstart_qty == 1)
					{
						esc_calibration();
					}//*/
					break;
				default:break;
			}
			break;
		case 1: /* UT Settings*/
			switch (p->pos1[1])
			{
				case 4:
					data_140 (NULL, (gpointer)p);
					break; /* P140 自动80%  */
				default:break;
			}
			break;
		case 3:
			switch (p->pos1[3])
			{
				case 2:
					set_display_table (pp->p_config, !get_display_table(pp->p_config)); /* P320 */
					break;
				default:break;
			}
			break;
		case 4:
			switch(p->pos1[4])
			{
			}
			break;
		case 5:
			switch (p->pos1[5])
			{
				case 2: 
				//	set_probe_fft (pp->p_config, !get_probe_fft(pp->p_config)); /* P520 */
					break; 
				default:break;
			}
			break;
		case 6:
			switch (p->pos1[6])
			{
				case 4: 
					set_auto_focal (pp->p_config, !get_auto_focal(pp->p_config)); /* P640 */
					break; 
				default:break;
			}
			break;
		case 8:
			switch (p->pos1[8])
			{
				case 2: 
					set_report_format_userfield (pp->p_config,!get_report_format_userfield (pp->p_config)); /* P820 */
					break; 
				default:break;
			}
			break;
		case 9 :
			g_debug ("software update\n");
			software_update();
			break;
		default:break;
	}

	/* 处理微调 */
	if ((p->pos_last2 == p->pos2[p->pos][p->pos1[p->pos]])
	&& (pp->pos_pos == MENU3_PRESSED))
	{
		switch (p->pos) 
		{
			case 0: /* Wizard */
				break;
			case 1: /* UT Settings */
				switch (p->pos1[1])
				{
					case 0: data_process (&(p->p_tmp_config->db_reg), 4);			break; /* 100增益 5种步进 */
					case 1: data_process (&(p->p_tmp_config->pulser_reg), 2);		break; /* 110pulser发射 3种步进 */
					case 2: data_process (&(p->p_tmp_config->receiver_reg), 2);		break; /* 120receiver接收 3种步进 */
					case 3: data_process (&(p->p_tmp_config->scanoffset_reg), 2);	break; /* 130 */
					case 4: break; /* 140 自动80%  */
					default:break;
				}
				break;
			case 2: /* Gate/Alarm */
				switch (p->pos1[2])
				{
					case 0: /* 弹出一个菜单选择  */			break; /* 200闸门3种选择  */
					case 1: /* 弹出一个菜单选择  */			break; /* 210 Alarm  */
					case 2: /* 弹出一个菜单选择  */			break; /* 220 Alarm  */
					case 3: break; /* 230 */
					default:break;
				}
				break;
			case 3: /* Display Table */
				switch (p->pos1[3])
				{
					case 0: /* 弹出一个菜单选择  */			break; /* 300 */
					case 1: /* 弹出一个菜单选择  */			break; /* 310  */
					case 2: break; /* 320 display table  */
					case 3: break; /* 330 */
					default:break;
				}
				break;
			case 4:
				break;
			case 5:
				switch (p->pos1[5])
				{
					case 0: break; /*   */
					case 1: data_process (&(p->p_tmp_config->scanoffset_reg), 2);  break; /*510 */
					case 2: break; /*   */
					case 3: break; 
					default:break;
				}
				break;
			case 6:
				switch (p->pos1[6])
				{
					case 0: break; /*   */
					case 1: data_process (&(p->p_tmp_config->min_angle_reg), 2);  break;   /*610  */
					case 2: break; /*620 */
					case 3: data_process (&(p->p_tmp_config->element_qty_reg), 2);break; /*630*/ 
					default:break;
				}
				break;
			case 7:
				switch (p->pos1[7])
				{
					case 0: break; /*   */
					case 1: break; /*710 */
					case 2: data_process (&(p->p_tmp_config->scan_start_reg), 2);  break;   /*720  */
					case 3: break; 
					default:break;
				}
				break;
			case 8:
				switch (p->pos1[8])
				{
					case 0: break; 
					case 1: break;
					case 2: break;   /*820  */
					case 3: break; 
					default:break;
				}
				break;
			default:break;
		}
	}
	else
	{
		tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[0]));
	}
	p->pos_pos = MENU3_PRESSED;
	draw_menu2 (0);
	draw_menu3 (0, p);

	return ;
}



void b3_fun1(gpointer p)
{
	int grp = get_current_group(pp->p_config);
	/* 一次点击处理 */
	switch (pp->pos)
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0://Group
					break;
				case 1://Focal Law
					break;
				case 2://Calibration
					CalibrationCallback021();
					break;
				case 3:((pp->wstart_qty) < 4) ? (pp->wstart_qty) ++ : ( pp->wstart_qty = 1);break;
				default:break;
			}
			break;
		case 1:
			switch (pp->pos1[1])
			{
				case 4: 
                    group_set_refgain(grp , group_get_gain(grp));
					pp->pos_pos = MENU3_STOP;
					RefreshGainMark(grp) ;
					break; /* Set Ref P141 设置参考增益值*/
				default:break;
			}
			break;

		case 3:
			switch (pp->pos1[3])
			{
				case 2: 
					set_entry_image (pp->p_config, !get_entry_image (pp->p_config));/* P321 */
					break;
				default:break;
			}
			break;
		case 4:
			switch (pp->pos1[4])
			{
				case 3:
					switch(get_dis_prop_scan(pp->p_config)) 
					{
						case 0:break;
						case 1:break;
						case 2:set_dis_prop_cratio (pp->p_config, !get_dis_prop_cratio (pp->p_config));break;
						//case 3:set_dis_prop_sinterpolation (pp->p_config, !get_dis_prop_sinterpolation (pp->p_config));break;
						default:break;
					}
					break;  
					/* P431 */
				default:break;
			}
			break;
		case 7:
			switch (pp->pos1[7])
			{
				case 3: 
					/* P731 */
					data_731();
					break;
				default:break;
			}
			break;
		case 8:
			switch (pp->pos1[8])
			{
				case 2: 
					set_report_format_probe (pp->p_config, !get_report_format_probe (pp->p_config));/* P821 */
					break;
				case 3: 
					set_report_userfield_enable (pp->p_config, 
							!get_report_userfield_enable (pp->p_config, get_report_userfield_select (pp->p_config)),
							get_report_userfield_select (pp->p_config));
					break;
				default:break;
			}
			break;
		case 9 :
			printf("hardware update\n");
			hardware_update();
			break;
		default:break;
	}


	/*处理微调*/
	if ((pp->pos_last2 == pp->pos2[pp->pos][pp->pos1[pp->pos]])
	&& (pp->pos_pos == MENU3_PRESSED))
	{
		switch (pp->pos) 
		{
			case 0:
				break;
			case 1:
				switch (pp->pos1[1])
				{
					case 0:data_process(&(pp->p_tmp_config->start_reg), 2); break; /* 101start扫描延时 3种步进 */
					case 1: /* 弹出一个选择菜单,选择 */ break; /*111 收发模式Tx/Rx Mode  */
					case 2: /* 弹出一个选择菜单,选择 */ break; /*121 滤波Filter  */
					case 3: data_process (&(pp->p_tmp_config->indexoffset_reg), 2);	break; /* 131 */
					case 4: /* Set Ref. */ break; /*141 Set Ref.参考增益  */
					default:break;
				}
				break;
			case 2: 
				switch (pp->pos1[2])
				{
					case 0: break; 
					case 1: break;
					case 2: break;/* 221 Alarm#*/ 
					case 3: data_process(&(pp->p_tmp_config->mat_atten_reg), 2);break;  /*231 Mat.Attenuatior*/
					case 4: break; 
					default:break;
				}
				break;
			case 3:
				switch (pp->pos1[3])
				{
					case 0: break; 
					case 1:
							switch(GROUP_VAL_POS(grp , selection))
							{
								case 0: data_process(&(TMP(per_reference_reg)), 2);break;
								case 1:
								case 2: data_process(&(TMP(s_reference_reg)), 3);break;
								case 3:
								case 4:
								case 5:
								case 6:
								case 7: data_process(&(TMP(cursors_angle_reg)), 1);break;
								default:break;
							}
							break;	/*p311*/
					case 2: break; /* 321 entry image*/ 
					case 3: data_process(&(pp->p_tmp_config->min_thickness_reg), 2); break;  /*331 */
					case 4: /* Set Ref. */ break; 
					default:break;
				}
				break;
			case 4: 
				switch (pp->pos1[4])
				{
					case 0: break; 
					case 1: break;
					case 2:
							switch(GROUP_VAL_POS(grp , col_select_pos))
							{
								case 0:data_process(&(TMP(color_start_reg)), 1 );break;
								case 1:data_process(&(TMP(color_contrast_reg)), 1 );break;
								case 2:data_process(&(TMP(min_reg)), 2);break;
							}
							break; 

					case 3:
							switch(get_dis_prop_scan(pp->p_config)) 
							{
								case 0:break;
								case 1:data_process(&(TMP(compress_reg)), 2 );break;
								case 2:break;
								case 3:break;
							}
							break; 
					default:break;
				}
				break;

			case 5:
				switch (pp->pos1[5])
				{
					case 0: break; 
					case 1: data_process (&(pp->p_tmp_config->indexoffset_reg), 2);  break; /*511 */
					case 2: data_process (&(pp->p_tmp_config->db_reg), 4);  break;     /*521*/
					case 3: data_process (&(pp->p_tmp_config->part_thickness_reg), 2);  break;     /*531*/
					case 4: break; 
					default:break;
				}
				break;

			case 6:
				switch (pp->pos1[6])
				{
					case 0: data_process (&(pp->p_tmp_config->connection_P_reg), 3);  break;  /*601 */
					case 1: data_process (&(pp->p_tmp_config->max_angle_reg), 2);  break;     /*611 */
					case 2:
							if((LAW_VAL_POS(grp , Focal_point_type)==HALFPATH_P)||(LAW_VAL_POS(grp , Focal_point_type)==DEPTH_P))
								data_process (&(pp->p_tmp_config->positions_reg), 2);
							else if((LAW_VAL_POS(grp , Focal_point_type)==PROJECTION_P)||(LAW_VAL_POS(grp , Focal_point_type)==FOCALPLANE_P))
								data_process (&(pp->p_tmp_config->offsets_reg), 2);
							break; /*621 */
					case 3: data_process (&(TMP(first_element_reg)), 2); break; 
					default:break;
				}
				break;
			case 7:
				switch (pp->pos1[7])
				{
					case 0: break; /*    */
					case 1: break; /*710 */
					case 2: data_process (&(pp->p_tmp_config->scan_end_reg), 2);  break;   /*721  */
					case 3: break; /*731*/ 
					default:break;
				}
				break;
			case 8:
				switch (pp->pos1[8])
				{
					case 0: break; 
					case 1: break;
					case 2: break;   /*821  */
					case 3: break;          /*831  */ 
					default:break;
				}
				break;
			case 9:
				switch (pp->pos1[9])
				{
					case 0: data_process (&(pp->p_tmp_config->bright_reg), 2);  break; /*901  */
					case 1: break; /*710 */
					case 2: break;   /*721  */
					case 3: //pp->p_config->ezview = !pp->p_config->ezview;break;  /*931*/
					default:break;
				}
				break;
			default:break;
		}
	}
	else
		tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[1]));
	pp->pos_pos = MENU3_PRESSED;
	draw_menu2(0);
	draw_menu3(0, NULL);                          /**/
	return ;
}
void StepSlection022()
{
	int grp = get_current_group(pp->p_config);
	if ( pp->cstart_qty == 2 )
	{
		if(pp->ctype_pos == 0)
			data_process(&(TMP(origin_reg)), 2);
		else if ((pp->ctype_pos == 1) && (pp->cmode_pos == 0))
		{
			if(GROUP_VAL_POS(grp , group_mode) == PA_SCAN)
				data_process(&(TMP(cangle_reg)), 2);
			else
				data_process(&(TMP(WedgeSeparation_reg)), 2);
		}
		else if((pp->ctype_pos == 1) && ((pp->cmode_pos == 2)||(pp->cmode_pos == 3)))
			data_process(&(TMP(ref_amplitude_reg)), 2);
	}
	else if ( pp->cstart_qty == 3 )
	{
		if(pp->ctype_pos == 0)
			data_process(&(TMP(distance_reg)), 2);
		else if(pp->ctype_pos == 1) {
			data_process(&(TMP(first_element_reg)),2);
            if (group_get_rx_tx_mode(grp) != TOFD) {
                data_process(&(TMP(cstart_reg)), 2);
            } else {
				data_process(&TMP(u_reference_reg) , 3);
            }
        }
	}
	else if ( pp->cstart_qty == 4 )
	{
		if (pp->ctype_pos == 1) {
            if (pp->cmode_pos == 0) {
				data_process(&TMP(u_reference_reg) , 3);
            } else if (pp->cmode_pos == 1) {
                    data_process(&(TMP(cstart_reg)), 2);
            } else if (pp->cmode_pos == 3) {
                    data_process(&(TMP(db_reg)), 4);
            }
		data_process(&(TMP(cstart_reg)), 2);
        } else {
            if (group_get_rx_tx_mode(grp) != TOFD) {
                data_process(&(TMP(cstart_reg)), 2);
            } else {
				data_process(&TMP(u_reference_reg) , 3);
            }
		}
	}
	else if ( pp->cstart_qty == 5 )
	{
		if((pp->ctype_pos == 1) && (pp->cmode_pos == 2))
		{
			if(GROUP_VAL_POS(grp , group_mode) == PA_SCAN)
				data_process(&(TMP(compdb_reg)), 1);
			else
				data_process(&(TMP(cstart_reg)), 2);
		}
		else if ((pp->ctype_pos == 1) && ((pp->cmode_pos == 1)||(pp->cmode_pos == 3)) )
			data_process(&(TMP(db_reg)), 4);
		else
			data_process(&(TMP(cstart_reg)), 2);
	}
	else if(pp->cstart_qty == 6)
	{
		if ((pp->ctype_pos == 1) && (pp->cmode_pos == 2))
			data_process (&(TMP(db_reg)), 4);
		if ((pp->ctype_pos == 1) && (pp->cmode_pos == 3))
			data_process (&(TMP(position_reg)), 4);
	}
}
void b3_fun2(gpointer p)
{
	/* 之前的位置 */
	int grp = get_current_group(pp->p_config) ;
	switch (pp->pos)
	{
	case 0:
		if(pp->pos1[0] == 2)
		{
			CalibrationCallback022();
		}
		else if(pp->pos1[0] == 3)
		{
			if((pp->wstart_qty) == 1)
			{
				set_weld_display (!get_weld_display()); /* P033 */
				gtk_widget_queue_draw (pp->vboxtable);
			}
		}
		break;
	case 1:
		switch (pp->pos1[1])
		{
			case 4:
				set_group_db_ref (pp->p_config, get_current_group(pp->p_config),
						!get_group_db_ref (pp->p_config, get_current_group(pp->p_config)));
				pp->pos_pos = MENU3_STOP;
				RefreshGainMark(grp) ;
				break; /* dB Ref P142 开关 */
			default:break;
		}
		break;
	case 4:
		switch (pp->pos1[4])
		{
			case 1:
				set_overlay_sizing_curves(pp->p_config, !get_overlay_sizing_curves(pp->p_config));
				break; /* p412 */
			case 3:
				set_dis_prop_boptimum (pp->p_config, !get_dis_prop_boptimum (pp->p_config));
				break; /* p432 */
			default:break;
		}
		break;
	case 7:
		switch (pp->pos1[7])
		{
			case 0: break; /*    */
			case 1: break; /*712 */
			case 2: break; /*722  */
			case 3: /*732*/
					data_732(NULL , NULL);
					break;
			default:break;
		}
		break;
	case 8:
		switch (pp->pos1[8])
		{
			case 2:
				set_report_format_setup (pp->p_config,  !get_report_format_setup (pp->p_config));/* P822 */
				break;
			default:break;
		}
		break;
	case 9:
		switch (pp->pos1[9])
		{
			case 3:break;
			default:break;
		}
		break;
	default:break;
	}
	/*处理微调*/
	if ((pp->pos_last2 == pp->pos2[pp->pos][pp->pos1[pp->pos]])
	&& (pp->pos_pos == MENU3_PRESSED))
	{
		switch (pp->pos) 
		{
			case 0:
				switch (pp->pos1[0])
				{
					case 0:	/* 002 */
						break;
					case 1: /* 012 */
						break;
					case 2:	/* 022 */
						StepSlection022();
						break;
					case 3:
						if(pp->wstart_qty == 3)
							data_process (&(TMP(weland_height_reg)), 2);
						else if(pp->wstart_qty == 4)
							data_process (&(TMP(fizone_height_reg)), 2);

						break;/*p032*/
					default:break;
				}
				break;
			case 1:
				switch (pp->pos1[1])
				{
					case 0:data_process(&(pp->p_tmp_config->range_reg), 2); break; /* 102range范围 3种步进 */
					case 1:
						   if (pp->mark_pop_change)
							   data_process(&(TMP(frequency_reg)), 2); /* 112 频率范围 3种步进 */
						   break; /* 112 频率 Freq.  */
					case 2: /* 弹出一个选择菜单,选择 */ break; /* 122 检波 Recitify  */
					case 3:  /* 132 角度 PA 不能更改 UT 可以修改 */  data_process(&(TMP(angle_reg)), 2); break; 
					case 4: break;/* dB Ref P142 开关 */
					default:break;
				}
				break;
			case 2:
				switch (pp->pos1[2])
				{
					case 0:data_process(&(pp->p_tmp_config->agate_start_reg), 3); break; /* 202 agate start*/
					case 1:
						   /* 弹出一个选择菜单,选择 */                         break; 
					case 2:data_process(&(pp->p_tmp_config->count_reg), 1); break; /* 222 output count*/
					case 3:
						   if(GROUP_VAL_POS(grp , SizingCurves.mode_pos) == 1)
						   {
							   if( GROUP_VAL_POS(grp , SizingCurves.curve_pos) == 1|| GROUP_VAL_POS(grp , SizingCurves.curve_pos)==3)
								   data_process(&(pp->p_tmp_config->position_reg),3);
							   else if(GROUP_VAL_POS(grp , SizingCurves.curve_pos) == 2)
								   data_process(&(pp->p_tmp_config->delay_reg), 3);
						   }
						   else
						   {
							   if(GROUP_VAL_POS(grp , SizingCurves.curve_pos) == 1|| GROUP_VAL_POS(grp , SizingCurves.curve_pos)==2)
								   data_process(&(pp->p_tmp_config->ref_ampl_reg), 2);
						   }
						   break; /*232 Ref.Amplitude*/

					case 4: break;
					default:break;
				}
				break;
			case 3:
				switch (pp->pos1[3])
				{
					case 0: break; 
					case 1:/*312*/
							switch(GROUP_VAL_POS(grp , selection))
							{
								case 0: data_process(&(TMP(per_measure_reg)), 2);break;
								case 1:
								case 2: data_process(&(TMP(s_measure_reg)), 3);break;
								case 3: data_process(&(TMP(u_reference_reg)), 3);break;
								case 4:
								case 5:
								case 6: data_process(&(TMP(cursors_amplitude_reg)), 2);break;
								case 7: data_process(&(TMP(cursors_scan_reg)), 3);break;
								default:break;
							}
							break;

					case 2: break;
					case 3: data_process(&(pp->p_tmp_config->max_thickness_reg), 2); break; /*332 */
					case 4: break;
					default:break;
				}
				break;
			case 4:
				switch (pp->pos1[4])
				{
					case 0: break; 
					case 1: break;  /*412*/
					case 2: 
							if (GROUP_VAL_POS(grp , col_select_pos) == 0)
								data_process(&(TMP(color_end_reg)), 1);  /*422 */
							else if (GROUP_VAL_POS(grp , col_select_pos) == 1)
								data_process(&(TMP(brightness_reg)), 1);
							else if (GROUP_VAL_POS(grp , col_select_pos) == 2)
								data_process(&(TMP(max_reg)), 2);
							break;  /*432 */
					case 3: break;
					default:break;
				}
				break;
			case 5:
				switch (pp->pos1[5])
				{
					case 0: break; 
					case 1: data_process(&(TMP(skew_reg)), 2); break;  /*512*/ 
					case 2: data_process(&(TMP(agate_start_reg)), 3); break;  /*522*/
					case 3: data_process(&(TMP(diameter_reg)), 3); break;  /*532*/
					case 4: break;
					default:break;
				}
				break;
			case 6:
				switch (pp->pos1[6])
				{
					case 0: data_process(&(TMP(connection_R_reg)), 2); break;	/*602*/
					case 1: data_process(&(TMP(angle_step_reg)), 2); break;		/*612*/
					case 2:
							if((LAW_VAL_POS(grp , Focal_point_type)==HALFPATH_P)||(LAW_VAL_POS(grp , Focal_point_type)==DEPTH_P))
								data_process (&(pp->p_tmp_config->positione_reg), 2);
							else if((LAW_VAL_POS(grp , Focal_point_type)==PROJECTION_P)||(LAW_VAL_POS(grp , Focal_point_type)==FOCALPLANE_P))
								data_process (&(pp->p_tmp_config->offsete_reg), 2);
							break;	/*622 */ 
					case 3: data_process(&(TMP(last_element_reg)), 2); break;  /*632*/
					case 4: break;
					default:break;
				}
				break;
			case 7:
				switch (pp->pos1[7])
				{
					case 0: break; /*    */
					case 1: break; /*712 */
					case 2: data_process (&(TMP(scan_resolution_reg)), 2);  break;   /*722  */
					case 3: break;
					default:break;
				}
				break;
			case 8:
				switch (pp->pos1[8])
				{
					case 0: break; 
					case 1: break;
					case 2: break;   /*822  */
					case 3: break; 
					default:break;
				}
				break;
			default:break;
		}
	}
	else
		tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[2]));
	pp->pos_pos = MENU3_PRESSED;
	draw_menu2(0);
	draw_menu3(0, NULL);                          /**/
	return ;
}

void b3_fun3(gpointer p)
{
	int	grp = get_current_group (pp->p_config);
	/* 之前的位置 */
	switch (pp->pos)
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 2:
					if(CalibrationCallback023())
					{
						return;
					}
					break;
			}
			break;
		case 1:
			switch (pp->pos1[1])
			{
				case 2: 
					set_group_val (get_group_by_id(pp->p_config, grp), GROUP_VIDEO_FILTER, 
							!get_group_val (get_group_by_id(pp->p_config, grp), GROUP_VIDEO_FILTER));
					TMP(group_spi[grp]).video_filter	= 
						get_group_val (get_group_by_id(pp->p_config, grp), GROUP_VIDEO_FILTER);
                    group_spi_send (grp);
					/* 视频滤波 P123 */
					break; 
				default:break;
			}
			break;
		case 4:
			switch (pp->pos1[4])
			{
				case 1: 
					set_overlay_gate(pp->p_config, !get_overlay_gate(pp->p_config));
					break; /* p413 */
				default:break;
			}
			break;
		case 6:
			switch (pp->pos1[6])
			{
				case 4:
					data_643();
					break;  
				default:break;
			}
			break;
		case 8:
			switch (pp->pos1[8])
			{
				case 2: 
					set_report_format_note (pp->p_config, !get_report_format_note (pp->p_config));/* P823 */
					break;
				default:break;
			}
			break;
		default:break;
	}

	/*处理微调*/
	if ((pp->pos_last2 == pp->pos2[pp->pos][pp->pos1[pp->pos]])
	&& (pp->pos_pos == MENU3_PRESSED))
	{
		switch (pp->pos) 
		{
			case 0:
				switch (pp->pos1[0])
				{
					case 0:	/* 003 */
						if ( pp->start_qty == 7 )
							data_process (&(TMP(indexoffset_reg)), 2);
						break;
					case 1: /* 013 */
						if ( pp->fstart_qty == 3 )
							data_process(&(TMP(first_element_reg)), 2);
						else if ( pp->fstart_qty == 4 )
							data_process (&(TMP(max_angle_reg)), 2);
						break;
					case 2:	/* 023 */
						switch(pp->cstart_qty)
						{
							case 2:
								if ((pp->ctype_pos == 1) && (pp->cmode_pos == 1))
								{
									data_process(&(TMP(clb_reg)) , 2);
								}
								else if ((pp->ctype_pos == 1) && ((pp->cmode_pos == 2)||(pp->cmode_pos == 3)))
								{
									if(GROUP_VAL_POS(grp , group_mode) == PA_SCAN )
										data_process (&(TMP(tolerance_t_reg)), 2);
									else
										data_process (&(TMP(db_reg)), 4);
								}
								else if ((pp->ctype_pos == 1) && (pp->cmode_pos == 0))
								{
                                    if (group_get_rx_tx_mode(grp) == TOFD)
										data_process (&(pp->p_tmp_config->part_thickness_reg), 2);
									else
									    data_process (&(TMP(db_reg)), 4);
								}
								break;
							case 3:
								if ((pp->ctype_pos == 1) && (pp->cmode_pos == 0) )
								{
                                    if(group_get_rx_tx_mode(grp) == TOFD)
									{
										data_process (&(TMP(db_reg)), 4);
									}
									else
									{
										data_process(&(TMP(clb_reg)) , 2);
									}
								}
								else if ((pp->ctype_pos == 1) && ((pp->cmode_pos == 1)||(pp->cmode_pos == 2)||(pp->cmode_pos == 3)))
								{
									if(GROUP_VAL_POS(grp , group_mode) != PA_SCAN)
									{
										data_process(&(TMP(clb_reg)) , 2);
									}
                                    if( group_get_rx_tx_mode(grp) != TOFD)
									    data_process(&(TMP(cwidth_reg)), 3);
								    else
									    data_process(&TMP(u_measure_reg) , 3);
								}
								else if ((pp->ctype_pos == 2) && (pp->scode_pos == 1))
								{
									data_process (&(TMP(cwidth_reg)), 3);
								}
								break;
							case 4:
							case 5:
                                if(group_get_rx_tx_mode(grp) != TOFD)
									data_process(&(TMP(cwidth_reg)), 3);
								else
									data_process(&TMP(u_measure_reg) , 3);
								break;
							case 6:
								data_process(&(TMP(tcg_gain_reg)), 5);
								break;
							default: break;
						}
						break;
					case 3:
						if ( pp->wstart_qty == 3 )
							data_process (&(TMP(weland_offset_reg)), 2);
						else if(pp->wstart_qty == 4) {
							data_process (&(TMP(fizone_angle_reg)), 2);
							data_process (&(TMP(fizone_radius_reg)), 2);
                        }
						break;/*p033*/
					default:break;
				}
				break;
			case 1:
				switch (pp->pos1[1])
				{
					case 0:data_process(&(pp->p_tmp_config->wedge_delay_reg), 2); break;   /* 103wedge delay 楔款延时 3种步进 */
					case 1: /* 弹出一个选择菜单,选择 */ break; 
					case 2: 
														break;   /* 123 视频滤波 on or off  */
					case 3:break; /* 133 Skew (deg)  */
					case 4:
						   if (pp->mark_pop_change)
							   data_process(&(pp->p_tmp_config->point_qty_reg), 2); /* 143 Points Qty. 3种步进 */
						   break; /* 143 Points Qty.  */

					default:break;
				}
				break;
			case 2:
				switch (pp->pos1[2])
				{
					case 0:data_process(&(pp->p_tmp_config->gate_width_reg), 3); break; /* 203 agate width 3种步进 */
					case 1: /* 弹出一个选择菜单,选择 */ break;
					case 2: /* 视频滤波 */ break; /* 123 视频滤波 on or off  */
					case 3: 
					   if( GROUP_VAL_POS(grp , SizingCurves.curve_pos)==1 || GROUP_VAL_POS(grp , SizingCurves.curve_pos)==2|| GROUP_VAL_POS(grp , SizingCurves.curve_pos)==3 )
					   {
						   if((GROUP_VAL_POS(grp, SizingCurves.mode_pos)==0 )&&(GROUP_VAL_POS(grp , SizingCurves.curve_pos)==1 || GROUP_VAL_POS(grp, SizingCurves.curve_pos)==2))
							   data_process(&(pp->p_tmp_config->ref_ampl_offset_reg), 4); /* 233 ref.ampl.offset  */
						   else if((GROUP_VAL_POS(grp , SizingCurves.mode_pos)==1 )&&(GROUP_VAL_POS(grp , SizingCurves.curve_pos)==1 || GROUP_VAL_POS(grp , SizingCurves.curve_pos)==2))
							   data_process(&(pp->p_tmp_config->amplitude_reg), 2);
						   else if((GROUP_VAL_POS(grp , SizingCurves.mode_pos)==1 )&&(GROUP_VAL_POS(grp , SizingCurves.curve_pos)==3))
							   data_process(&(pp->p_tmp_config->tcg_gain_reg), 4);
					   }
					   break; /*233 Ref.Amplitude*/

					case 4: /* 弹出一个选择菜单,选择 */ break;
					default:break;
				}
				break;
			case 3:
				switch (pp->pos1[3])
				{
					case 0: break;
					case 1: /* 313 */
							switch(GROUP_VAL_POS(grp , selection))
							{
								case 0:
								case 1: data_process(&(TMP(u_reference_reg)), 3);break;
								case 3: data_process(&(TMP(u_measure_reg)), 3);break;
								case 2: data_process(&(TMP(i_reference_reg)), 3);break;
								case 4:
								case 5:
								case 6: data_process(&(TMP(cursors_ut_reg)), 3);break;
								case 7: data_process(&(TMP(cursors_index_reg)), 3);break;
								default:break;
							}
							break;

					case 2: /* 视频滤波 */ break; 
					case 3: data_process(&(TMP(echo_qty_reg)), 1); break;  /*333 */
					case 4: /* 弹出一个选择菜单,选择 */ break; 
					default:break;
				}
				break;
			case 4:
				switch (pp->pos1[4])
				{
					case 0: break;
					case 1: break; /*413*/
					case 2: break; /*423*/
					case 3: break;  
					case 4: break; 
					default:break;
				}
				break;

			case 5:
				switch (pp->pos1[5])
				{
					case 0: break;
					case 1: break;
					case 2: data_process(&(pp->p_tmp_config->gate_width_reg), 3);  break; /* 523 */
					case 3: break;  
					case 4: break; 
					default:break;
				}
				break;

			case 6:
				switch (pp->pos1[6])
				{
					case 0: break;
					case 1: data_process(&TMP(beam_skew_min_angle_reg), 2);  break; /*613 */
					case 2: 
							if((LAW_VAL_POS(grp , Focal_point_type)==HALFPATH_P)||(LAW_VAL_POS(grp , Focal_point_type)==DEPTH_P))
								data_process (&(pp->p_tmp_config->positionstep_reg), 2);
							else if((LAW_VAL_POS(grp , Focal_point_type)==PROJECTION_P)||(LAW_VAL_POS(grp , Focal_point_type)==FOCALPLANE_P))
								data_process (&(pp->p_tmp_config->depths_reg), 2);
							break;	/*623 */
					case 3:  data_process(&TMP(element_step_reg), 2);break;  
					case 4: break; 
					default:break;
				}
				break;
			case 7:
				switch (pp->pos1[7])
				{
					case 0: data_process(&(pp->p_tmp_config->encoder_resolution_reg), 2);break;
					case 1: data_process(&(pp->p_tmp_config->scanspeed_reg), 2);  break; 
					case 2: data_process(&(pp->p_tmp_config->index_start_reg), 2);  break; 
					case 3: break;  
					case 4: break; 
					default:break;
				}
				break;
			case 8:
				switch (pp->pos1[8])
				{
					case 0: break; 
					case 1: break;
					case 2: break;   /*823  */
					case 3: break; 
					default:break;
				}
				break;
			case 9:
				switch (pp->pos1[9])
				{
					case 0: break; 
					case 1: break;
					case 2: break;   /*923  */
					case 3: break;   /*933  */ 
					default:break;
				}
				break;

			default:break;
		}
	}
	else
		tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[3]));
	pp->pos_pos = MENU3_PRESSED;
	draw_menu2(0);
	draw_menu3(0, NULL);                          /**/
	return ;
}

void b3_fun4(gpointer p)
{
	int grp = get_current_group(pp->p_config);//当前group
	switch (pp->pos)
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 0:	/* p004 */
					break;
				case 1:break;
				case 2:/*p024*/
					   CalibrationCallback024() ;
					break;
				case 3:
					break;
				default:break;
			}
			break;
		case 4:
			switch (pp->pos1[4])
			{
				case 1: 
					set_overlay_cursor (pp->p_config, !get_overlay_cursor (pp->p_config));
					gtk_widget_queue_draw (pp->vboxtable);
					break; /* p414 */
				default:break;
			}
			break;
		default:break;
	}

	/*处理微调*/
	if ((pp->pos_last2 == pp->pos2[pp->pos][pp->pos1[pp->pos]])
	&& (pp->pos_pos == MENU3_PRESSED))
	{
		switch (pp->pos) 
		{
			case 0:
				switch (pp->pos1[0])
				{
					case 0:	/* 004 */
						if ( pp->start_qty == 7 )
							data_process(&(TMP(skew_reg)), 2); 
						break;
					case 1: /* 014 */
						if ( pp->fstart_qty == 3 )
							data_process(&(TMP(last_element_reg)), 2);
						else if ( pp->fstart_qty == 4 )
							data_process(&(TMP(angle_step_reg)), 2);
						break;
					case 2: /* 024 */
						if(pp->cstart_qty == 2)
						{
							data_process(&(pp->p_tmp_config->start_reg), 2);
							if ((pp->ctype_pos == 1) && (pp->cmode_pos == 1))
							{
								data_process (&(TMP(tolerance_reg)), 2);
							}
						}
						else if(pp->cstart_qty == 3)
						{
							if ((pp->ctype_pos == 1) && ((pp->cmode_pos == 0)||(pp->cmode_pos == 2)))
							{
                                if(group_get_rx_tx_mode(grp) == TOFD)
									data_process(&(pp->p_tmp_config->start_reg), 2);
								else
								    data_process(&(TMP(clb_reg)) , 2);
							}
							else if(pp->ctype_pos == 2)
								data_process (&(TMP(clb_reg)), 2);
							data_process (&(TMP(cheight_reg)), 1);
						}
						else if(pp->cstart_qty == 4)
						{
							if(pp->ctype_pos == 1)
								data_process (&(TMP(cheight_reg)), 1);
							else if(pp->ctype_pos == 2)
								data_process (&(TMP(clb_reg)), 2);
						}
						else if(pp->cstart_qty == 5)
						{
							if ((pp->ctype_pos == 1) && (pp->cmode_pos == 0))
								data_process (&(TMP(cheight_reg)), 1);
							else if ((pp->ctype_pos == 1) && (pp->cmode_pos == 2))
							{
								if(GROUP_VAL_POS(grp , group_mode) == PA_SCAN)
									data_process (&(TMP(db_reg)), 4);
								else
									data_process (&(TMP(cheight_reg)), 1);
							}

						}

						break;
					case 3:
                        data_process (&(TMP(fizone_radius_reg)), 2);
                        data_process (&(TMP(fizone_angle_reg)), 2);
                        break;
					default:break;
				}
				break;
			case 1:
				switch (pp->pos1[1])
				{
					case 0:data_process(&(pp->p_tmp_config->velocity_reg), 3); break; /* 104velocity声速  3种步进 */
					case 1:
						   if (pp->mark_pop_change)
							   data_process(&(pp->p_tmp_config->pulser_width_reg), 2); /* 114 脉宽范围 3种步进 */
						   break; /* 114 脉冲宽度 PW   */
					case 2: /* 弹出一个选择菜单,选择 */ break; /* 124 平均 averaging  */
					case 3:data_process(&(pp->p_tmp_config->beam_delay_reg), 2); break; /* 134 beamdelay 波束延时 */
					case 4: /*  */ break; /* 144 Scale Factor 多少点压缩一点 只能看  */
					default:break;
				}
				break;
			case 2:
				switch (pp->pos1[2])
				{
					case 0:data_process(&(pp->p_tmp_config->agate_height_reg), 1); break; /* 204 threshold  2种步进 */
					case 1: /* 弹出一个选择菜单,选择 */ break;
					case 2: data_process(&(pp->p_tmp_config->active_delay_reg), 3); break; /*224 Delay*/
					case 3: data_process(&(pp->p_tmp_config->curve_step_reg), 2);  break; /* 234  */
					case 4: /*  */ break; /* 244 Scale Factor 多少点压缩一点 只能看  */
					default:break;
				}
				break;
			case 3:
				switch (pp->pos1[3])
				{
					case 0: break;
					case 1:  /* 314 */ 
							switch(GROUP_VAL_POS(grp , selection))
							{
								case 0:
								case 1: data_process(&(TMP(u_measure_reg)), 3);break;
								case 3: data_process(&(TMP(i_reference_reg)), 3);break;
								case 2: data_process(&(TMP(i_measure_reg)), 3);break;
								case 4:
								case 5:
								case 6: data_process(&(TMP(cursors_scan_reg)), 3);break;
								default:break;
							}
							break;

					case 2: data_process(&(pp->p_tmp_config->entry_reg), 1); break;  /*324 */
					case 3: break;  /*334 */
					case 4: break; 
					default:break;
				}
				break;
			case 4:
				switch (pp->pos1[4])
				{
					case 0:
						if( get_display_pos(pp->p_config)==10) 
							data_process(&(TMP(dis_range_reg)), 2);
						break; /*404*/
					case 1:break; /*414*/
					case 2:break; /*424*/
					case 3:break; 
					case 4:break; 
					default:break;
				}
				break;
			case 6:
				switch (pp->pos1[6])
				{
					case 0:break;
					case 1: data_process(&TMP(beam_skew_max_angle_reg), 2);  break; /*614 */
					case 2: data_process(&TMP(depthe_reg), 2);  break;
					case 3:break; 
					case 4:break; 
					default:break;
				}
				break ;
			case 7:
				switch (pp->pos1[7])
				{
					case 0:data_process(&(pp->p_tmp_config->origin_reg), 2);break; /* 704 */
					case 1:data_process(&(pp->p_tmp_config->scanspeed_rpm_reg), 2);break;  /* 714 */
					case 2:data_process(&(pp->p_tmp_config->index_end_reg), 2);  break; /* 724 */
					case 3:break; 
					case 4:break; 
					default:break;
				}
				break;

			default:break;

		}
	}
	else
		tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[4]));
	pp->pos_pos = MENU3_PRESSED;
	draw_menu2(0);
	draw_menu3(0, NULL);                          /**/

	return ;
}

void b3_fun5(gpointer p)
{
	/* 之前的位置 */
//	pp->pos_last2 = pp->pos2[pp->pos][pp->pos1[pp->pos]];
//	pp->pos2[pp->pos][pp->pos1[pp->pos]] = 5;
	int grp = get_current_group(pp->p_config);//当前group

	switch (pp->pos)
	{
		case 0:
			switch (pp->pos1[0])
			{
				case 2:
					if(CalibrationCallback025())
					{
						pp->pos_pos = MENU2_STOP;
						draw_menu2(0);
						draw_menu3(0, NULL);
						return;
					}
					break;
			}
			break;
        case 2:
			if (pp->pos1[2] == 0) {
                data_205(NULL);
            }
            break;
		case 4:
			switch (pp->pos1[4])
			{
				case 1: 
					set_overlay_overlay (pp->p_config, !get_overlay_overlay (pp->p_config));
					gtk_widget_queue_draw (pp->vboxtable);
					break; /* p415 */
				default:break;
			}
			break;
		case 5:
			switch (pp->pos1[5])
			{
				case 0: 
					data_505(NULL , NULL);
					break; /* p505 */
				default:break;
			}
			break;
		case 7:
			switch (pp->pos1[7])
			{
				case 0:	
					//cscan数据清零
					data_711(NULL,(gpointer)((int)get_inspec_source (pp->p_config)) );
					break; /* p705 */
				default:break;
			}
			break;
		default:break;
	}

	/*处理微调*/
	if ((pp->pos_last2 == pp->pos2[pp->pos][pp->pos1[pp->pos]])
	&& (pp->pos_pos == MENU3_PRESSED))
	{
		switch (pp->pos) 
		{
			case 0:
				switch (pp->pos1[0])
				{
					case 0:	/* 005 */
						break;
					case 1: /* 015 */
						if ( pp->fstart_qty == 3 )
							data_process(&(TMP(element_step_reg)), 2);
						break;
					case 2: /* 025 */
						if(pp->cstart_qty == 4 || pp->cstart_qty == 5)
						{
							if (pp->ctype_pos == 1)
							{
								data_process(&(TMP(clb_reg)) , 2);
							}
						}
						else if(pp->cstart_qty == 2 || pp->cstart_qty == 3)
						{
							data_process(&(pp->p_tmp_config->range_reg), 2);
							data_process(&(TMP(clb_reg)) , 2);
						}
						break;
					case 3:break;
					default:break;
				}
				break;
			case 1:
				switch (pp->pos1[1])
				{
					case 0: break; /* 105 空 */
					case 1: 
							if (pp->mark_pop_change)
								data_process(&(pp->p_tmp_config->prf_reg), 2); /* 115 PRF重复频率 3种步进 */
							break; /* 115 PRF重复频率  */

					case 2:data_process(&(pp->p_tmp_config->reject_reg), 2); break; /* 125 Reject 抑制  */
					case 3:data_process(&(pp->p_tmp_config->gain_offset_reg), 4); break; /* 135 Gain Offset  */
					case 4: 
						   if (pp->mark_pop_change)
							   data_process(&(pp->p_tmp_config->sum_gain_reg), 4); /* 145 Sum Gain  5种步进 */
						   break; /* 145 Sum Gain   */

					default:break;
				}
				break;
			case 2:
				switch (pp->pos1[2])
				{
					case 0: break; 
					case 1: break;
					case 2: data_process(&(pp->p_tmp_config->holdtime_reg), 3); break; 
					case 3: data_process(&(pp->p_tmp_config->ref_gain_reg), 4); break; 
					case 4: break; 
					default:break;
				}
				break;
			case 3:
				switch (pp->pos1[3])
				{
					case 0: break; 
					case 1: /* P315 */ 
							switch(GROUP_VAL_POS(grp , selection))
							{
								case 3: data_process(&(TMP(i_measure_reg)), 3);break;
								case 4:
								case 5:
								case 6: data_process(&(TMP(cursors_index_reg)), 3);break;
								default:break;
							}
							break;
					case 2: break; 
					case 3: break; 
					case 4: break; 
					default:break;
				}
				break;
			case 4:
				switch (pp->pos1[4])
				{
					case 0:break; /*405*/
					case 1:break; /*415*/
					case 2:break; /*425*/
					case 3:break; 
					case 4:break; 
					default:break;
				}
				break;
			case 5:
				switch (pp->pos1[5])
				{
					case 0:break;  /*505*/
					case 1:break; /*515*/
					case 2:break;
					case 3:break; 
					case 4:break; 
					default:break;
				}
				break;
			case 6:
				switch (pp->pos1[6])
				{
					case 0:break;
					case 1: data_process(&TMP(beam_skew_angle_step_reg), 2);  break; /*615*/
					case 2:break;
					case 3:break; 
					case 4:break; 
					default:break;
				}
				break;
			case 7:
				switch (pp->pos1[7])
				{
					case 0:break; 
					case 1:data_process(&(pp->p_tmp_config->indexspeed_reg), 2);break;  /* 715 */
					case 2:data_process(&(pp->p_tmp_config->index_resolution_reg), 2);  break; /*725*/
					case 3:break; 
					case 4:break; 
					default:break;
				}
				break;
			default:break;
		}
	}
	else
		tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[5]));
	pp->pos_pos = MENU3_PRESSED;
	draw_menu2(0);
	draw_menu3(0, NULL);                          /**/

	return ;
}

static int keypress_event_main_bak(GtkWidget *widget, GdkEventKey *event)			/* 自己的按键处理*/
{
	//g_print("main bak press\n");
	return window_keypress_event_orig (widget, event); 
}

static int keypress_event_main(GtkWidget *widget, GdkEventKey *event)			/* 自己的按键处理*/
{
	//g_print("main press\n");
	return handler_key(event->keyval, NULL);
}

static int keypress_event_main_spinbutton(GtkWidget *widget, GdkEventKey *event)
{
	//g_print("main spinbutton press\n");
	guint i, keyval = event->keyval;

	for ( i = sizeof (key_fast_map) / sizeof (guint) ; i; i--)
	{
		if (keyval == key_fast_map[i - 1])
		{
			return handler_key(keyval, NULL);
		}
	}
	if (!i)
	{
		//g_print("main spinbutton press 1234567890\n");
		return window_keypress_event_orig (widget, event); 
	}

	return 0;
}

static int keypress_event_main_entry(GtkWidget *widget, GdkEventKey *event)			/* 自己的按键处理*/
{
	g_print("main entry press\n");
	return 0;
}

static int keypress_event_dialog(GtkWidget *widget, GdkEventKey *event)			/* 自己的按键处理*/
{
	//g_print("dialog press\n");
	guint keyval = event->keyval;

	switch (keyval)
	{
		case GDK_Return:
			gtk_dialog_response (GTK_DIALOG(widget), GTK_RESPONSE_OK);
			return 0;
			break;
		case GDK_Escape:
			gtk_dialog_response (GTK_DIALOG(widget), GTK_RESPONSE_CANCEL);
			return 0;
			break;
		case GDK_F12:
			if((pp->pos==6)&&(pp->pos1[6]==4)&&((pp->pos2[6][4]==1)||(pp->pos2[6][4]==2))&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget), GTK_RESPONSE_OK);

			if((pp->pos==5)&&(pp->pos1[pp->pos]==0)&&(pp->pos2[5][0]==3)&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_CHANGE_FOCAL);

			if((pp->pos==5)&&(pp->pos1[pp->pos]==0)&&(pp->pos2[5][0]==4)&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_CHANGE_FOCAL);			

			if((pp->pos==6)&&(pp->pos1[pp->pos]==4)&&(pp->pos2[6][4]==2)&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_LAW_SAVE);			

			if((pp->pos==6)&&(pp->pos1[pp->pos]==4)&&(pp->pos2[6][4]==1)&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_LAW_READ);			
			return 0;
			break;
		case GDK_F11:
			if((pp->pos==6)&&(pp->pos1[6]==4)&&((pp->pos2[6][4]==1)||(pp->pos2[6][4]==2))&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget), GTK_RESPONSE_CANCEL);

			if((pp->pos==8)&&(pp->pos1[pp->pos]==0)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==1)&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget), GTK_RESPONSE_CANCEL);

			if((pp->pos==8)&&(pp->pos1[pp->pos]==0)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==2)&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget), GTK_RESPONSE_CANCEL);

			if((pp->pos==9)&&(pp->pos1[pp->pos]==2)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==1)&&(pp->pos_pos==MENU3_PRESSED))
			{
				gtk_dialog_response (GTK_DIALOG(widget), GTK_RESPONSE_CANCEL);
			}
			return 0;
			break;
		case GDK_Super_L:
			if((pp->pos==8)&&(pp->pos1[pp->pos]==0)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==1)&&(pp->pos_pos==MENU3_PRESSED))
			{
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_FILE_OPEN);
			}

			if((pp->pos==8)&&(pp->pos1[pp->pos]==0)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==2)&&(pp->pos_pos==MENU3_PRESSED))
			{
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_CONFIG_SAVE);
			}
			return 0;
			break;
		case GDK_F2:
			if((pp->pos==8)&&(pp->pos1[pp->pos]==0)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==2)&&(pp->pos_pos==MENU3_PRESSED))
			{
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_CONFIG_FILE_FOCAL);
			}
			if((pp->pos==9)&&(pp->pos1[pp->pos]==2)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==1)&&(pp->pos_pos==MENU3_PRESSED))
			{
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_SELECT_ALL);
			}
			return 0;
			break;
		case GDK_F3:
			if((pp->pos==9)&&(pp->pos1[pp->pos]==2)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==1)&&(pp->pos_pos==MENU3_PRESSED))
			{
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_COPY_FILE);
			}
			return 0;
			break;
		case GDK_F4:
			if((pp->pos==9)&&(pp->pos1[pp->pos]==2)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==1)&&(pp->pos_pos==MENU3_PRESSED))
			{
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_MOVE_FILE);
			}
			return 0;
			break;

		case GDK_F6:

			if((pp->pos==9)&&(pp->pos1[pp->pos]==2)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==0)&&(pp->pos_pos==MENU3_PRESSED))
			{
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_SAVE_AND_CLOSE);
			}

			if((pp->pos==8)&&(pp->pos1[pp->pos]==1)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==3)&&(pp->pos_pos==MENU3_PRESSED))
			{
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_SAVE_AND_CLOSE);
			}

			if((pp->pos==5)&&(pp->pos1[pp->pos]==0)&&(pp->pos2[5][0]==3)&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget), GTK_RESPONSE_OK );

			if((pp->pos==5)&&(pp->pos1[pp->pos]==0)&&(pp->pos2[5][0]==4)&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget),GTK_RESPONSE_OK );	

			if((pp->pos==9)&&(pp->pos1[pp->pos]==2)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==1)&&(pp->pos_pos==MENU3_PRESSED))
			{
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_RENAME_FILE);
			}

			return 0;
			break;

		case GDK_F1:

			if((pp->pos==9)&&(pp->pos1[pp->pos]==2)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==0)&&(pp->pos_pos==MENU3_PRESSED))
			{
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_CLOSE);
			}

			if((pp->pos==8)&&(pp->pos1[pp->pos]==1)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==3)&&(pp->pos_pos==MENU3_PRESSED))
			{
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_CLOSE);
			}

			if((pp->pos==5)&&(pp->pos1[pp->pos]==0)&&(pp->pos2[5][0]==3)&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget), GTK_RESPONSE_CANCEL );

			if((pp->pos==5)&&(pp->pos1[pp->pos]==0)&&(pp->pos2[5][0]==4)&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget),GTK_RESPONSE_CANCEL );	

			return 0;
			break;

		case GDK_F5:
			if((pp->pos==5)&&(pp->pos1[pp->pos]==0)&&(pp->pos2[5][0]==3)&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_SELECT_AND_CLOSE);
			if((pp->pos==5)&&(pp->pos1[pp->pos]==0)&&(pp->pos2[5][0]==4)&&(pp->pos_pos==MENU3_PRESSED))
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_SELECT_AND_CLOSE);	
			if((pp->pos==9)&&(pp->pos1[pp->pos]==2)&&(pp->pos2[pp->pos][pp->pos1[pp->pos]]==1)&&(pp->pos_pos==MENU3_PRESSED))
			{
				gtk_dialog_response (GTK_DIALOG(widget), RESPONSE_DELECT_FILE);
			}
			return 0;
			break;
		default:break;
	}

	return dialog_keypress_event_orig (widget, event); 
	return 0;
}

static int keypress_event_dialog_spinbutton(GtkWidget *widget, GdkEventKey *event)			/* 自己的按键处理*/
{
	g_print("dialog spinbutton press\n");
	return 0;
}

static int keypress_event_dialog_entry(GtkWidget *widget, GdkEventKey *event)			/* 自己的按键处理*/
{
	g_print("dialog entry press\n");
	return 0;
}

static int keypress_event_warning(GtkWidget *widget, GdkEventKey *event)			/* 自己的按键处理*/
{
//	g_print("warning press\n");
	return window_keypress_event_orig (widget, event);
//	return 0;
}

/* 修改窗口的按键处理程序 */
void change_keypress_event(int window_type)
{
	switch (window_type)
	{
		case KEYPRESS_MAIN:
			widget_window_class->key_press_event = keypress_event_main;
			break;
		case KEYPRESS_MAIN_SPINBUTTON:
			widget_window_class->key_press_event = keypress_event_main_spinbutton;
			break;
		case KEYPRESS_MAIN_ENTRY:
			widget_window_class->key_press_event = keypress_event_main_entry;
			break;
		case KEYPRESS_DIALOG:
			widget_window_class->key_press_event = keypress_event_main_bak;
			dialog_window_class->key_press_event = keypress_event_dialog;
			break;
		case KEYPRESS_DIALOG_SPINBUTTON:
			dialog_window_class->key_press_event = keypress_event_dialog_spinbutton;
			break;
		case KEYPRESS_DIALOG_ENTRY:
			dialog_window_class->key_press_event = keypress_event_dialog_entry;
			break;
		case KEYPRESS_WARNING:
			widget_window_class->key_press_event = keypress_event_warning;
			break;
		case KEYPRESS_MENUITEM:
			break;
		default:break;
	}
}


/* 快捷键处理函数 */
gboolean key_press_handler (GtkWidget* pWidget,	GdkEventKey* pEvent, gpointer data)
{
	if (pEvent->type == GDK_KEY_PRESS)
	{
		handler_key(pEvent->keyval, data);
	}
	return TRUE;
}

gboolean foo (GtkAccelGroup *accel_group, GObject *acceleratable,
		guint keyval, GdkModifierType modifier, gpointer data)
{
	handler_key(keyval, data);
	return 0;
}

void B3func(int num)
{
	gpointer p = pp;
	if(!pp->help_yn)
	{
		gLastMenuPos.pos.menu3 = gCurrentMenuPos.pos.menu3;
		gCurrentMenuPos.pos.menu3 = num;
		/* 之前的位置 */
		pp->pos_last2 = pp->pos2[pp->pos][pp->pos1[pp->pos]];
		pp->pos2[pp->pos][pp->pos1[pp->pos]] = num;
		//按下时先更新，因为spin按下后不会更新
		guchar pos_pos = pp->pos_pos;
		pp->pos_pos = MENU3_STOP;
		draw_menu3(0, NULL);
		pp->pos_pos = pos_pos;
		switch (num)
		{
			case 0:b3_fun0(p);break;
			case 1:b3_fun1(p);break;
			case 2:b3_fun2(p);break;
			case 3:b3_fun3(p);break;
			case 4:b3_fun4(p);break;
			case 5:b3_fun5(p);break;
			default:break;
		}
	}
	else
	{
		if(gCurrentMenuPos.pos.menu1 == 0)
		{
			if((gCurrentMenuPos.pos.menu2 == 0)//wizard group
			||(gCurrentMenuPos.pos.menu2 == 1))//wizard law
			{
				gLastMenuPos.pos.menu3 = gCurrentMenuPos.pos.menu3;
				gCurrentMenuPos.pos.menu3 = num;
				/* 之前的位置 */
				pp->pos_last2 = pp->pos2[pp->pos][pp->pos1[pp->pos]];
				pp->pos2[pp->pos][pp->pos1[pp->pos]] = num;
				//按下时先更新，因为spin按下后不会更新
				pp->pos_pos = MENU3_STOP;
				draw_menu3(0, NULL);
				pp->pos_pos = MENU3_PRESSED;
				menu3FuncTrans(gCurrentMenuPos);

			}
		}
	}
}

gboolean data_function0 (GtkWidget *widget,	GdkEventButton *event,	gpointer data)
{
	guint num = GPOINTER_TO_UINT (data);
	gReturnButtonRelease = TRUE;
	B3func(num);
	return TRUE;
}

void data_0021 (GtkMenuItem *menuitem, gpointer data) /* Wizard->Group->Operation */
{
	pp->operation_pos = (unsigned char) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	/* 发送给硬件 */
}

void data_0022 (GtkMenuItem *menuitem, gpointer data) /* Wizard->Group->Group */
{
	pp->wgroup_pos = (unsigned char) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	/* 发送给硬件 */
}

void data_0023 (GtkMenuItem *menuitem, gpointer data) /* */
{
	g_tmp_group_struct.group_mode = (char) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_00241 (GtkSpinButton *spinbutton, gpointer data) /* Pulser 发射 P110 */
{
	set_group_val (&g_tmp_group_struct, GROUP_PULSER, (int) (gtk_spin_button_get_value (spinbutton)));
    if (PULSE_ECHO == g_tmp_group_struct.rx_tx_mode)
		set_group_val (&g_tmp_group_struct, GROUP_RECEIVER, (int) (gtk_spin_button_get_value (spinbutton)));
}

void data_00242 (GtkMenuItem *menuitem, gpointer data) /* 收发模式 Tx/Rx Mode P111 */
{
    g_tmp_group_struct.rx_tx_mode = (int) (GPOINTER_TO_UINT (data));
    if (PULSE_ECHO == g_tmp_group_struct.rx_tx_mode)
		set_group_val (&g_tmp_group_struct, 
				GROUP_RECEIVER, get_group_val (&g_tmp_group_struct, GROUP_PULSER));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	/* 发送给硬件 */
}

void data_0025 (GtkMenuItem *menuitem, gpointer data) /* Probe/Part->Select->Select 502 */
{
	set_probe_select (pp->p_config, (gboolean) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_0027 (GtkSpinButton *spinbutton, gpointer data) /*scanoffset */
{
	if(get_unit(pp->p_config) == UNIT_MM)
	{
		g_tmp_group_struct.scan_offset =  (int) (gtk_spin_button_get_value (spinbutton) * 10.0);
	}
	else
	{
		g_tmp_group_struct.scan_offset =  (int) (gtk_spin_button_get_value (spinbutton) * 10.0 / 0.03937);
	}

    draw_area_all();
	pp->bRefreshDraw = TRUE ;
}

void data_032 (GtkSpinButton *spinbutton, gpointer data) /*part_thickness*/
{
	int grp = get_current_group(pp->p_config);
	if(UNIT_MM == get_unit(pp->p_config))
		set_weland_height (grp, (GUINT_TO_POINTER
					((unsigned int) (gtk_spin_button_get_value (spinbutton) * 1000.0))));
	else
		set_weland_height (grp, (GUINT_TO_POINTER
					((unsigned int) (gtk_spin_button_get_value (spinbutton) * 1000.0/ 0.03937 ))));
}

void data_0321 (GtkSpinButton *spinbutton, gpointer data) /*fill zone height*/
{
	int grp = get_current_group(pp->p_config);
	if(UNIT_MM == get_unit(pp->p_config))
		set_fizone_height (grp, (GUINT_TO_POINTER
					((guint) (gtk_spin_button_get_value (spinbutton) * 1000.0))));
	else
		set_fizone_height (grp, (GUINT_TO_POINTER
					((guint) (gtk_spin_button_get_value (spinbutton) * 1000.0/ 0.03937 ))));
}

void data_033 (GtkMenuItem *menuitem, gpointer data) /* wizard weld->symmetry */
{
	int grp = get_current_group(pp->p_config);
	set_part_symmetry (grp , data);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_0331 (GtkSpinButton *spinbutton, gpointer data) /*weld land offset*/
{
	int grp = get_current_group(pp->p_config);
	if(UNIT_MM == get_unit(pp->p_config))
		set_weland_offset (grp, (GUINT_TO_POINTER
					((guint) (gtk_spin_button_get_value (spinbutton) * 1000.0))));
	else
		set_weland_offset (grp, (GUINT_TO_POINTER
					((guint) (gtk_spin_button_get_value (spinbutton) * 1000.0/ 0.03937 ))));
}
void data_0332 (GtkSpinButton *spinbutton, gpointer data) /*fill zone angle*/
{
	int grp = get_current_group(pp->p_config);
	set_fizone_angle (grp, (GUINT_TO_POINTER
				((guint) (gtk_spin_button_get_value (spinbutton) * 1000.0))));
}

void data_root_angle_callback (GtkSpinButton *spinbutton, gpointer data) /*root angle*/
{
        int grp = get_current_group(pp->p_config);
        set_weland_offset(grp, (GUINT_TO_POINTER((guint) (gtk_spin_button_get_value (spinbutton) * 1000.0))));
}

void data_034 (GtkSpinButton *spinbutton, gpointer data) /*fill zone radius*/
{
	int grp = get_current_group(pp->p_config);
	set_fizone_radius (grp, (GUINT_TO_POINTER
				((guint) (gtk_spin_button_get_value (spinbutton) * 1000.0))));
}

void data_00341 (GtkSpinButton *spinbutton, gpointer data) /* Pulser 发射 P120 */
{
	set_group_val (&g_tmp_group_struct, GROUP_RECEIVER, (int) (gtk_spin_button_get_value (spinbutton)));
}

void data_00342 (GtkSpinButton *spinbutton, gpointer data) /* Pulser 发射 P110 */
{
	set_group_val (&g_tmp_group_struct, GROUP_PULSER, (int) (gtk_spin_button_get_value (spinbutton)));
    if (PULSE_ECHO == g_tmp_group_struct.rx_tx_mode)
		set_group_val (&g_tmp_group_struct, GROUP_RECEIVER, (int) (gtk_spin_button_get_value (spinbutton)));

	/*发送给硬件*/
}

void data_0037 (GtkSpinButton *spinbutton, gpointer data) /*indexoffset */
{
	if(get_unit(pp->p_config) == UNIT_MM)
		g_tmp_group_struct.index_offset =  (int) (gtk_spin_button_get_value (spinbutton) * 10.0);
	else
		g_tmp_group_struct.index_offset =  (int) (gtk_spin_button_get_value (spinbutton) * 10.0 / 0.03937);
}

void data_0044 (GtkSpinButton *spinbutton, gpointer data) /* Pulser 发射 P120 */
{
	set_group_val (&g_tmp_group_struct, GROUP_RECEIVER, (int) (gtk_spin_button_get_value (spinbutton)));
}

void data_00471 (GtkSpinButton *spinbutton, gpointer data) /* Skew (deg) */
{
	g_tmp_group_struct.skew =  (gushort) (gtk_spin_button_get_value (spinbutton) * 100.0);
}

void data_00472 (GtkMenuItem *menuitem, gpointer data) /* Skew (deg) */
{
	short temp = GPOINTER_TO_UINT (data);
	g_tmp_group_struct.skew_pos = (unsigned char) (GPOINTER_TO_UINT (data));

	switch (g_tmp_group_struct.skew_pos)
	{
		case 0:g_tmp_group_struct.skew = 0;break;
		case 1:g_tmp_group_struct.skew = 9000;break;
		case 2:g_tmp_group_struct.skew =18000;break;
		case 3:g_tmp_group_struct.skew =27000;break;
		default:break;
	}

	if (temp != 4)
	{
		pp->pos_pos = MENU3_STOP;
		draw_menu3(0, NULL);
	}
	else
	{
		pp->mark_pop_change = 1;
		pp->pos_pos = MENU3_PRESSED;
		draw_menu3(0, NULL);
	}
}

void data_022 (GtkMenuItem *menuitem, gpointer data) /* Wizard->Calibration->Type */
{
	pp->ctype_pos = (unsigned char) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_0221 (GtkMenuItem *menuitem, gpointer data) /* Wizard->Calibration->Echo Type */
{
	unsigned char temp_value ;
	int grp = get_current_group(pp->p_config) ;
	temp_value  = (unsigned char) (GPOINTER_TO_UINT (data));
	if(temp_value == pp->echotype_pos)  return ;

	switch(temp_value)
	{
		case 0:
			GROUP_VAL_POS(grp , ut_unit) = UT_UNIT_SOUNDPATH;
			TMP_CBA(distance) = pp->radius1 / 1000;
			UpdateGateForSpiSending(grp) ;
			//draw_area_all();
			send_focal_spi (grp , SPI_RESET_NO);
			break ;
		case 1:
			GROUP_VAL_POS(grp , ut_unit) = UT_UNIT_TRUE_DEPTH;
			TMP_CBA(distance) = pp->depth1 / 1000;
			UpdateGateForSpiSending(grp) ;
	        //draw_area_all();
			send_focal_spi (grp , SPI_RESET_NO);
			break ;
		case 2:
			GROUP_VAL_POS(grp , ut_unit) = UT_UNIT_TRUE_DEPTH;
			TMP_CBA(distance) = pp->thickness1 / 1000;
			UpdateGateForSpiSending(grp) ;
	        //draw_area_all();
			send_focal_spi (grp , SPI_RESET_NO);
			break ;
		default:
			break ;

	}
	pp->echotype_pos = temp_value ;
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	request_refresh(REFRESH_DRAW_AREA_ALL);
	//draw_area_all();
	//pp->bRefreshDraw = TRUE ;
	/* 发送给硬件 */
}

void data_0222 (GtkSpinButton *spinbutton, gpointer data) /* wizard  First Angle  */
{
	int _nTmpValue = (int) (gtk_spin_button_get_value (spinbutton) * 100.0);
	if(_nTmpValue == TMP_CBA(clb_start)) return ;
	TMP_CBA(clb_start) =  _nTmpValue;
	if(TMP_CBA(clb_start)>TMP_CBA(clb_end))
		TMP_CBA(clb_end) = TMP_CBA(clb_start)   ;
	request_refresh(REFRESH_CALIBRATION_REDRAW) ;
}

void data_0223 (GtkSpinButton *spinbutton, gpointer data) /* wizard  Ref.Amplitude  */
{
	unsigned short temp_value ;
	temp_value = (gushort) (gtk_spin_button_get_value (spinbutton) * 100.0);
	if(pp->ref_amplitude == temp_value)  return ;
	pp->ref_amplitude = temp_value ;

	request_refresh(REFRESH_CALIBRATION_REDRAW)  ;

}
void data_0224 (GtkSpinButton *spinbutton, gpointer data) /* wizard  Comp Gain  */
{
	pp->comp_gain =  (gushort) (gtk_spin_button_get_value (spinbutton) * 100.0);
}
void data_0225 (GtkMenuItem *menuitem, gpointer data) /* wizard  Weld Type  */
{
	int grp = get_current_group(pp->p_config);
	GROUP_VAL_POS(grp , part.weldtype_pos) = (unsigned char) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}
void data_0226 (GtkSpinButton *spinbutton, gpointer data) /* wizard  Comp Gain  */
{
	int temp_value = 0;
	int beam_num      ;
	unsigned char grp = get_current_group(pp->p_config) ;

	if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
	{
		temp_value = (short) ((gtk_spin_button_get_value (spinbutton)) * 100 );
		if(temp_value < LAW_VAL_POS(grp, Angle_min))  temp_value = LAW_VAL_POS(grp , Angle_min) ;
		if(temp_value > LAW_VAL_POS(grp, Angle_max))  temp_value = LAW_VAL_POS(grp , Angle_max) ;

		beam_num = (temp_value - LAW_VAL_POS(grp, Angle_min)) / LAW_VAL_POS(grp, Angle_step);
		if(beam_num == TMP(beam_num[grp])) return ;
		TMP(beam_num[grp]) = beam_num ;
		temp_value = LAW_VAL_POS(grp , Angle_min) + beam_num * LAW_VAL_POS(grp , Angle_step) ;
		TMP(current_angle[grp]) = temp_value * G_PI / 18000.0 ;
	}
	else if(LAW_VAL_POS(grp , Focal_type) == LINEAR_SCAN)
	{
		temp_value = (short) (gtk_spin_button_get_value (spinbutton)) - 1 ;
		if(temp_value  == TMP(beam_num[grp]))    return ;
		TMP(beam_num[grp]) = temp_value ;
	}
	request_refresh(REFRESH_CALIBRATION_REDRAW)  ;
}

void data_0227 (GtkSpinButton *spinbutton, gpointer data) /* distance */
{
	double _nTmpValue  ;
	_nTmpValue =  (gtk_spin_button_get_value (spinbutton) * 1000.0);
	//pp->distance =  (int) (gtk_spin_button_get_value (spinbutton) * 1000.0);
	if(get_unit(pp->p_config) == UNIT_MM)
	{
		pp->distance = (int) _nTmpValue  ;
	}
	else
	{
		pp->distance = (int)( _nTmpValue / 0.03937 ) ;
	}
	//request_refresh(REFRESH_CALIBRATION_REDRAW)  ;
}
//***** response for encoder calibration origin setting, invalidate now
void data_0228 (GtkSpinButton *spinbutton, gpointer data)
{
}

void data_023 (GtkMenuItem *menuitem, gpointer data) /* Wizard->Calibration->Mode */
{
	pp->cmode_pos = (unsigned char) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_0231 (GtkMenuItem *menuitem, gpointer data) /* Wizard->Calibration->Select code */
{
	//printf("data_0242 %d \n", (guint)data) ;
	pp->scode_pos = (unsigned char) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_0232 (GtkSpinButton *spinbutton, gpointer data) /* wizard  Radius 1 */
{
	//printf("data_0232 %d \n", (guint)data) ;
	double _nTmpValue ;
	_nTmpValue = gtk_spin_button_get_value (spinbutton)   ;
	if(get_unit(pp->p_config) == UNIT_MM)
	{
		pp->radius1 =  (int) (_nTmpValue * 1000.0);
		TMP_CBA(distance)  = _nTmpValue;
	}
	else
	{
		pp->radius1 =  (int) (_nTmpValue * 1000.0 / 0.03937);
		TMP_CBA(distance)  = _nTmpValue / 0.03937 ;
	}
	//pp->radius1 =  (int) (gtk_spin_button_get_value (spinbutton) * 1000.0);
	//TMP_CBA(distance)  = pp->radius1 / 1000.0 ;
	request_refresh(REFRESH_CALIBRATION_REDRAW)  ;
}
void data_0233 (GtkSpinButton *spinbutton, gpointer data) /* wizard  depth 1 */
{
	//printf("data_0233 %d \n", (guint)data) ;
	double _nTmpValue ;
	_nTmpValue = gtk_spin_button_get_value (spinbutton)   ;
	if(get_unit(pp->p_config) == UNIT_MM)
	{
		pp->depth1 =  (int) (_nTmpValue * 1000.0);
		TMP_CBA(distance)  = _nTmpValue;
	}
	else
	{
		pp->depth1 =  (int) (_nTmpValue * 1000.0 / 0.03937);
		TMP_CBA(distance)  = _nTmpValue / 0.03937 ;
	}

	request_refresh(REFRESH_CALIBRATION_REDRAW)  ;
}
void data_0234 (GtkSpinButton *spinbutton, gpointer data) /* wizard  thickness 1 */
{
	double _nTmpValue ;
	_nTmpValue = gtk_spin_button_get_value (spinbutton)   ;
	if(get_unit(pp->p_config) == UNIT_MM)
	{
		pp->thickness1 =  (int) (_nTmpValue * 1000.0);
		TMP_CBA(distance)  = _nTmpValue;
	}
	else
	{
		pp->thickness1  =  (int) (_nTmpValue * 1000.0 / 0.03937);
		TMP_CBA(distance)  = _nTmpValue / 0.03937 ;
	}

	request_refresh(REFRESH_CALIBRATION_REDRAW)  ;
}

void data_0238 (GtkSpinButton *spinbutton, gpointer data) /* wizard  thickness A */
{
	int _nTmpValue = (int) (gtk_spin_button_get_value (spinbutton) * 100.0);
	if(_nTmpValue == TMP_CBA(clb_end)) return ;
	TMP_CBA(clb_end) =  _nTmpValue ;
	if(TMP_CBA(clb_start) > TMP_CBA(clb_end))
		TMP_CBA(clb_start) = TMP_CBA(clb_end)   ;
	request_refresh(REFRESH_CALIBRATION_REDRAW)  ;
}

void data_0239 (GtkSpinButton *spinbutton, gpointer data) /* wizard  Tolerance*/
{
	unsigned int temp_value ;
	temp_value =  (unsigned int) (gtk_spin_button_get_value (spinbutton) * 100.0);
	if(pp->tolerance_t == temp_value)  return ;
	pp->tolerance_t = temp_value ;
	//printf("pp->tolerance_t %d  \n" , pp->tolerance_t);
	request_refresh(REFRESH_CALIBRATION_REDRAW)  ;
}



void data_024 (GtkSpinButton *spinbutton, gpointer data) /* wizard  Radius 2 */
{
	double _nTmpValue ;
	_nTmpValue = gtk_spin_button_get_value (spinbutton)   ;
	if(get_unit(pp->p_config) == UNIT_MM)
	{
		pp->radius2 =  (int) (_nTmpValue * 1000.0);
	}
	else
	{
		pp->radius2  =  (int) (_nTmpValue * 1000.0 / 0.03937);
	}
}

void data_0241 (GtkSpinButton *spinbutton, gpointer data) /* wizard  depth 2 */
{
	double _nTmpValue ;
	_nTmpValue = gtk_spin_button_get_value (spinbutton)   ;
	if(get_unit(pp->p_config) == UNIT_MM)
	{
		pp->depth2 =  (int) (_nTmpValue * 1000.0);

	}
	else
	{
		pp->depth2  =  (int) (_nTmpValue * 1000.0 / 0.03937);

	}
}

void data_0242 (GtkSpinButton *spinbutton, gpointer data) /* wizard  thickness 2 */
{
	double _nTmpValue ;
	_nTmpValue = gtk_spin_button_get_value (spinbutton)   ;
	if(get_unit(pp->p_config) == UNIT_MM)
	{
		pp->thickness2 =  (int) (_nTmpValue * 1000.0);
	}
	else
	{
		pp->thickness2  =  (int) (_nTmpValue * 1000.0 / 0.03937);
	}
}
void data_0243 (GtkSpinButton *spinbutton, gpointer data) /* wizard  tolerance  */
{
	//printf("data_0243 %d \n", (guint)data) ;
	int _nTmpValue ;
	if(get_unit(pp->p_config) == UNIT_MM)
	{
		_nTmpValue = gtk_spin_button_get_value (spinbutton) * 100.0  ;

	}
	else
	{
		_nTmpValue = gtk_spin_button_get_value (spinbutton) * 100.0  / 0.03937  ;
	}
	//int temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 100.0);
	if(pp->tolerance == _nTmpValue)  return ;
	pp->tolerance =  _nTmpValue;

	request_refresh(REFRESH_CALIBRATION_REDRAW)  ;
}

void data_100 (GtkSpinButton *spinbutton, gpointer data) /* 增益Gain P100 */
{
	int grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	int gain = ((int)(gtk_spin_button_get_value(spinbutton) * 1000) + 1) / 10;
    if (get_group_db_ref (pp->p_config, grp)) {
        group_set_gain(grp, gain+group_get_refgain(grp));
    } else {
        group_set_gain(grp, gain);
    }

	RefreshGainMark(grp);

	group_data_spi new, *p1;
	memcpy (&new, &TMP(group_spi[grp]), sizeof (group_data_spi));
	p1 = &new;
	p1->offset = 16 * grp;
	p1->addr = 0x2;
	little_to_big ((unsigned int *)(p1),2);
#if ARM
	write (fd_array, (unsigned char *)(p1), 8);
#endif

	int _bRefreshGainOffset = ConfirmGainOffsetOfAllBeamInLimit(grp) ;
	RefreshDacPointInfo(grp);
	if(GROUP_VAL_POS(grp , SizingCurves.curve_pos) == 1 )
	{
		gtk_widget_queue_draw (pp->vboxtable);
	}
	if(GROUP_VAL_POS(grp , SizingCurves.curve_pos) == 3)
	{
		SendTcgSpi(grp , -1) ;
		gtk_widget_queue_draw (pp->vboxtable);
	}
	else if(_bRefreshGainOffset)// if gain offset + gain bigger than 80 db
	{
		send_focal_spi(grp , SPI_RESET_NO)     ;
	}
}

void data_101 (GtkSpinButton *spinbutton, gpointer data) /*Start 扫描延时 P101 */
{
	int temp_value ;
	int grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	//int temp_prf;
	double velocity ;
	int _nMaxBeamDelay ;
	velocity = get_group_val (p_grp, GROUP_VELOCITY) / 100.0 ;

	if (UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS(grp , ut_unit))
	{
		if (UNIT_MM == get_unit(pp->p_config))
			temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 2000000.0 /
					(cos(TMP(current_angle[grp])) * velocity)) ;
		else  /* 英寸 */
			temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 2000000.0 /
					(cos(TMP(current_angle[grp])) * 0.03937 * velocity)) ;
	}
	else if(UT_UNIT_SOUNDPATH == GROUP_VAL_POS(grp , ut_unit))
	{
		if (UNIT_MM == get_unit(pp->p_config))
			temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 2000000.0 / velocity ) ;
		else  /* 英寸 */
			temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 2000000.0 / (0.03937 * velocity)) ;
	}
	else /* 显示方式为时间 */
		temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 1000.0) ;


	temp_value = ((temp_value + 5) / 10 ) * 10 ;
	// if the value is not changed  return
    if(temp_value == group_get_start(grp)) {
        return;
    }
    group_set_start(grp, temp_value);

    TMP(group_spi[grp]).sample_start	= (group_get_start(grp) + GROUP_VAL_POS(grp , wedge_delay)) / 10;
	TMP(group_spi[grp]).sample_range	= TMP(group_spi[grp]).sample_start + GROUP_VAL_POS(grp , range) / 10;

	_nMaxBeamDelay = GetGroupMaxBeamDelay(grp) ;
	TMP(group_spi[grp]).rx_time	= TMP(group_spi[grp]).sample_range  + _nMaxBeamDelay + 5;

	MultiGroupRefreshIdelTime() ;
	//**************************************************************
	//if  linear DAC curve is displayed
	//refresh the dac line status
	if (GROUP_VAL_POS(grp , SizingCurves.curve_pos) == 2) refresh_linear_dac_pointer_info() ;
	//**************************************************************
	request_refresh(REFRESH_START_MARK) ;
}

void data_102 (GtkSpinButton *spinbutton, gpointer data) /*Range 范围 P102 */
{

	int temp_value ;
	int temp_value1;
	double float_compress_rate ;
	int    int_compress_rate   ;
	int grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	double velocity ;
	int _nMaxBeamDelay ;
	velocity = get_group_val (p_grp, GROUP_VELOCITY) / 100.0 ;

	if (UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS(grp , ut_unit))
	{
		if (UNIT_MM == get_unit(pp->p_config))
			temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 2000000.0 /
					(cos(TMP(current_angle[grp])) * velocity)) ;
		else  /* 英寸 */
			temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 2000000.0 /
					(cos(TMP(current_angle[grp])) * 0.03937 * velocity)) ;
	}
	else if(UT_UNIT_SOUNDPATH == GROUP_VAL_POS(grp , ut_unit))
	{
		if (UNIT_MM == get_unit(pp->p_config))
			temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 2000000.0 / velocity ) ;
		else  /* 英寸 */
			temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 2000000.0 / (0.03937 * velocity)) ;
	}
	else /* 显示方式为时间 */
		temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 1000.0) ;

	temp_value1 = GROUP_VAL_POS(grp , point_qty);
	if(GROUP_VAL_POS(grp , point_qty_pos) == 0) // point quantity is auto --- adjust the point quantity
	{
		if(temp_value <= 6400)
		{
			temp_value1 = temp_value / 10 ;
		}
		else
		{
			float_compress_rate = temp_value / 6400.0 ;
			int_compress_rate   = (temp_value % 6400) ? (int) (float_compress_rate + 1) : (int)float_compress_rate ;
			temp_value1 = temp_value / (int_compress_rate * 10 );
		}
	}
	// if current value is not changed , return
	temp_value = rounding(0, temp_value, temp_value1 * 10) ;
    if(temp_value == get_group_val(p_grp , GROUP_RANGE)) {
        return ;
    }

	set_group_val(p_grp , GROUP_RANGE , temp_value) ;
	GROUP_VAL_POS(grp , point_qty) = temp_value1;

	TMP(group_spi[grp]).point_qty = temp_value1 ;
	TMP(group_spi[grp]).compress_rato	= 
		((get_group_val (p_grp, GROUP_RANGE) / 10.0) / GROUP_VAL_POS(grp, point_qty)) > 1 ? 
		((get_group_val (p_grp, GROUP_RANGE) / 10.0) / GROUP_VAL_POS(grp, point_qty)) : 1;
	TMP(group_spi[grp]).sample_range	= TMP(group_spi[grp]).sample_start + 
		get_group_val (p_grp, GROUP_RANGE) / 10;		

	_nMaxBeamDelay = GetGroupMaxBeamDelay(grp) ;
	TMP(group_spi[grp]).rx_time	= TMP(group_spi[grp]).sample_range  + _nMaxBeamDelay + 5;

	MultiGroupRefreshIdelTime() ;
	//if  linear DAC curve is displayed
	//refresh the dac line status
	if (GROUP_VAL_POS(grp , SizingCurves.curve_pos) == 2) refresh_linear_dac_pointer_info() ;
	//****************************************************************
	request_refresh(REFRESH_RANGE_MARK) ;
}

void data_103 (GtkSpinButton *spinbutton, gpointer data) /*楔块延时  P103 */
{
	int temp_value ;
	int _nMaxBeamDelay ;
	int grp = get_current_group(pp->p_config);

	temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 1000.0) ;
	if(temp_value == GROUP_VAL_POS(grp , wedge_delay))  return ;
	GROUP_VAL_POS(grp , wedge_delay) = temp_value ;

    TMP(group_spi[grp]).sample_start	= (group_get_start(grp) + GROUP_VAL_POS(grp , wedge_delay)) / 10;
	TMP(group_spi[grp]).sample_range	= TMP(group_spi[grp]).sample_start + GROUP_VAL_POS(grp , range) / 10;
	_nMaxBeamDelay = GetGroupMaxBeamDelay(grp) ;
	TMP(group_spi[grp]).rx_time		= TMP(group_spi[grp]).sample_range  + _nMaxBeamDelay + 5;
	MultiGroupRefreshIdelTime() ;
	request_refresh(REFRESH_WEDGE_DELAY_MARK) ;


}

void data_104 (GtkSpinButton *spinbutton, gpointer data) /*声速 P104 */
{

	int temp_value ;
	int grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	if (UNIT_MM == get_unit(pp->p_config))
		temp_value = (int)(gtk_spin_button_get_value (spinbutton) * 100) ;
	else   /* 英寸/微秒 */
		temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 25400 * 100) ;
	// if current value not changed , return
	if(temp_value == get_group_val( p_grp , GROUP_VELOCITY))  return  ;

	set_group_val (p_grp, GROUP_VELOCITY,  temp_value );
	request_refresh(REFRESH_VELOCITY_MARK) ;
}

void data_110 (GtkSpinButton *spinbutton, gpointer data) /* Pulser 发射 P110 */
{
	int _nTmpValue = (int) (gtk_spin_button_get_value (spinbutton))  ;
	int grp = get_current_group(pp->p_config);
	if(_nTmpValue == GROUP_VAL_POS(grp , pulser1))
	{
		return ;
	}
	GROUP_VAL_POS(grp , pulser1)  = _nTmpValue ;
    if (group_get_rx_tx_mode(grp) == PULSE_ECHO)
	{
		GROUP_VAL_POS(grp , receiver1)  = _nTmpValue ;
	}
	send_focal_spi(grp , SPI_RESET_YES);
}

void data_120 (GtkSpinButton *spinbutton, gpointer data) /* Pulser 发射 P120 */
{
	int _nTmpValue = (int) (gtk_spin_button_get_value (spinbutton))  ;
	int grp = get_current_group(pp->p_config);
	if(_nTmpValue == GROUP_VAL_POS(grp , receiver1))
	{
		return ;
	}
	GROUP_VAL_POS(grp , receiver1)  = _nTmpValue ;
	send_focal_spi(grp , SPI_RESET_YES );
}

void data_111 (GtkMenuItem *menuitem, gpointer data) /* 收发模式 Tx/Rx Mode P111 */
{
	unsigned int temp_value ;
	int grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	temp_value = GPOINTER_TO_UINT (data) ;
    if(temp_value == group_get_rx_tx_mode(grp)) {
		return ;
	}
    if(group_get_rx_tx_mode(grp) == TOFD)
    {
    	set_group_val (p_grp, GROUP_RECTIFIER, 3);  // P/ECHO mode
    	TMP(group_spi[grp]).rectifier = 3  ;
    	if(get_display_pos(pp->p_config) == PA_TOFD) set_display_pos (pp->p_config, A_SCAN) ;
    	draw_area_all() ;
    	pp->bRefreshDraw = TRUE ;
    }
    group_set_rx_tx_mode(grp, temp_value);
    if (temp_value == PULSE_ECHO )
	{
		if(GROUP_VAL_POS(grp , group_mode) == UT1_SCAN)
		{
			output_set_parameter(0 , OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT1 ,0 ,0);//双晶关
		}
		if(GROUP_VAL_POS(grp , group_mode) == UT2_SCAN)
		{
			output_set_parameter(0 , OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT2 ,0 ,0);//双晶关
		}
		set_group_val (p_grp, GROUP_RECEIVER , get_group_val(p_grp , GROUP_PULSER));
	}
	else// if (temp_value == TOFD)
	{
		if (temp_value == TOFD)
		{
			set_group_val (get_group_by_id(pp->p_config, grp), GROUP_VIDEO_FILTER,  0);
			TMP(group_spi[grp]).video_filter	= 0 ;
			set_group_val (p_grp, GROUP_RECTIFIER, 0);
			TMP(group_spi[grp]).rectifier = 0 ;
		    DrawDisplayWindowFrame() ;
		}
		if(GROUP_VAL_POS(grp , group_mode) == UT1_SCAN)
		{
			output_set_parameter(0 , OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT1 ,1 ,0);//双晶关
		}
		if(GROUP_VAL_POS(grp , group_mode) == UT2_SCAN)
		{
			output_set_parameter(0 , OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT2 ,1 ,0);//双晶关
		}
	}
	output_write_one_reg_to_spi(0 ,OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT1);

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
    group_spi_send(grp);
	send_focal_spi(grp , SPI_RESET_YES );
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;
}


void data_1121 (GtkSpinButton *spinbutton, gpointer data) /* 频率 Freq 数值改变 */
{
	int temp_value ;
	int grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	temp_value = (int) ((gtk_spin_button_get_value (spinbutton)) * 1000.0) ;
	if(temp_value == get_group_val(p_grp , GROUP_FREQ_VAL)) return ;
	set_group_val (p_grp , GROUP_FREQ_VAL , temp_value) ;
	request_refresh(REFRESH_FREQUENCY_MARK) ;
}

void data_112 (GtkMenuItem *menuitem, gpointer data) /* 频率 Freq P112 */
{
	guint temp = GPOINTER_TO_UINT (data);
	int grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	if (temp == get_group_val (p_grp, GROUP_FREQ_POS) && temp != 12)
	{
		return ;
	}

	set_group_val (p_grp, GROUP_FREQ_POS, (int) (GPOINTER_TO_UINT (data)));
	if (temp != 12)
	{
		set_group_val (p_grp, GROUP_FREQ_VAL, get_freq());
		pp->pos_pos = MENU3_STOP;
		draw_menu3(0, NULL);
		gtk_spin_button_set_value (GTK_SPIN_BUTTON (pp->sbutton[2]), get_group_val (p_grp, GROUP_FREQ_VAL) / 1000.0);
		request_refresh(REFRESH_FREQUENCY_MARK) ;
	}
	else
	{
		pp->mark_pop_change = 1;
		pp->pos_pos = MENU3_PRESSED;
		draw_menu3(0, NULL);
		tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[2]));
	}

}


void data_113 (GtkMenuItem *menuitem, gpointer data)  /* Voltage  P113 */
{
	int grp = get_current_group (pp->p_config);
	set_voltage (pp->p_config, grp, (unsigned char)(GPOINTER_TO_UINT (data)));
	switch ((pp->p_config->group[grp].group_mode))
	{
		case UT1_SCAN:
		case UT2_SCAN:
			output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_VOLTAGE_UT ,GPOINTER_TO_UINT (data) ,0);
			output_write_one_reg_to_spi(0 ,OUTPUT_OTHER_COMMAND_VOLTAGE_UT);
			break;
		case UT_SCAN:
		case PA_SCAN:
			output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_VOLTAGE_PA ,GPOINTER_TO_UINT (data) ,0);
			output_write_one_reg_to_spi(0 ,OUTPUT_OTHER_COMMAND_VOLTAGE_PA);
			break;
		default:printf ("Voltage get value error\n");
				break;
	}
	pp->pos_pos = MENU3_STOP;
	//draw_menu3(0, NULL);

	GROUP_VAL_POS(0 , prf1) = get_prf() ;
	request_refresh(REFRESH_PRF_CHANGED) ;
}


void data_1141 (GtkSpinButton *spinbutton, gpointer data) /* PW  P114 */
{

	int temp =  (int) (gtk_spin_button_get_value (spinbutton) * PW_DIV);
	int grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	int div = PW_DIV * 25 / 10;
	temp = (temp / div) * div;
	if(temp == get_group_val(p_grp , GROUP_PW_VAL ))  return ;
	request_refresh(REFRESH_PULSE_WIDTH_MARK);

	set_group_val (p_grp, GROUP_PW_VAL, temp);
	MultiGroupRefreshIdelTime() ;
	TMP(group_spi[grp]).tx_start	= 2;
	TMP(group_spi[grp]).tx_end	= (int)(GROUP_VAL_POS(grp , pulser_width1) / div);//2 + pw
}

void data_114 (GtkMenuItem *menuitem, gpointer data) /* PW */
{

	int temp = GPOINTER_TO_UINT (data);
	int grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	set_group_val (p_grp, GROUP_PW_POS, (int) temp);
	set_group_val (p_grp, GROUP_PW_VAL, get_pw() * PW_DIV);
	if (temp == AUTO_SET)
	{
		request_refresh(REFRESH_PULSE_WIDTH_MARK) ;
		gtk_spin_button_set_value (GTK_SPIN_BUTTON (pp->sbutton[4]), 
				get_group_val (p_grp, GROUP_PW_VAL) / (double)PW_DIV);

		TMP(group_spi[grp]).tx_start	= 2;
		TMP(group_spi[grp]).tx_end	=  (int)(get_pw() / 2.5 + 0.5);

		draw_menu3(0, NULL);
		MultiGroupRefreshIdelTime() ;

	}
	else
	{
		pp->mark_pop_change = 1;
		pp->pos_pos = MENU3_PRESSED;
		draw_menu3(0, NULL);
		tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[4]));
	}

	/* 发送给硬件 */
}

void data_1151 (GtkSpinButton *spinbutton, gpointer data) /* PRF P115 */
{
	int temp_value ;

	temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 10) ;
	if(temp_value == GROUP_VAL_POS(0 , prf1)) return ;
	GROUP_VAL_POS(0 , prf1) = temp_value ;

	request_refresh(REFRESH_PRF_CHANGED) ;
	/* 发送给硬件 */
}

void data_115 (GtkMenuItem *menuitem, gpointer data) /* PRF */
{
	//char *markup;
	int temp = GPOINTER_TO_UINT (data);
	GROUP_VAL_POS(0 , prf_pos1) = temp ;
	GROUP_VAL_POS(0 , prf1 )    = get_prf();

	if (temp != 3)
	{
		pp->pos_pos = MENU3_STOP;
		draw_menu3(0, NULL);
		gtk_spin_button_set_value (GTK_SPIN_BUTTON (pp->sbutton[5]), GROUP_VAL_POS(0 , prf1) / 10.0);

		request_refresh(REFRESH_PRF_CHANGED) ;
	}
	else
	{
		pp->mark_pop_change = 1;
		pp->pos_pos = MENU3_PRESSED;
		draw_menu3(0, NULL);
		tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[5]));
	}


}

void data_1201 (GtkMenuItem *menuitem, gpointer data) /* damping p120 */
{
	int grp = get_current_group(pp->p_config);
	unsigned int _nTmpValue = (unsigned int) data  ;
	if(_nTmpValue == get_damping_pos (pp->p_config))
	{
		return ;
	}
	set_damping_pos (pp->p_config, _nTmpValue);
	if(_nTmpValue)
	{
		if(GROUP_VAL_POS(grp , group_mode) == UT1_SCAN)
		{
			output_set_parameter(0 , OUTPUT_OTHER_COMMAND_DAMPING_UT1 ,0 ,0);
		}
		if(GROUP_VAL_POS(grp , group_mode) == UT2_SCAN)
		{
			output_set_parameter(0 , OUTPUT_OTHER_COMMAND_DAMPING_UT2 ,0 ,0);
		}
		//output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_DAMPING , 0 , 0 );
	}
	else
	{
		if(GROUP_VAL_POS(grp , group_mode) == UT1_SCAN)
		{
			output_set_parameter(0 , OUTPUT_OTHER_COMMAND_DAMPING_UT1 ,3 ,0);
		}
		if(GROUP_VAL_POS(grp , group_mode) == UT2_SCAN)
		{
			output_set_parameter(0 , OUTPUT_OTHER_COMMAND_DAMPING_UT2 ,3 ,0);
		}
		//output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_DAMPING , 3 , 0 );
	}

	output_write_one_reg_to_spi(0 ,OUTPUT_OTHER_COMMAND_DAMPING_UT1);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

/* filter 滤波 P121 */
void data_121 (GtkMenuItem *menuitem, gpointer data)  
{
	int grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	set_group_val (p_grp, GROUP_FILTER_POS, (int) (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	set_filter (grp) ;
    group_spi_send ( grp );
}

void data_122 (GtkMenuItem *menuitem, gpointer data)  /* Rectifier 检波 P122 */
{
	int temp_value ;

	int grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	temp_value = GPOINTER_TO_UINT(data)  ;
	if(temp_value == get_group_val (p_grp, GROUP_RECTIFIER))
	{
		return ;
	}
	/* 发送给硬件 */
	TMP(group_spi[grp]).rectifier = temp_value;
    group_spi_send (grp);
	// because the PF model ruler is -100 to +100 % , and other model is 0 to +100  %
	// the draw area should refresh
	if(temp_value == 0 || get_group_val (p_grp, GROUP_RECTIFIER) == 0 )
	{
		set_group_val (p_grp, GROUP_RECTIFIER, temp_value);
		DrawDisplayWindowFrame() ;
	}
	else
		set_group_val (p_grp, GROUP_RECTIFIER, temp_value);

	pp->bRefreshDraw = TRUE;
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_124 (GtkMenuItem *menuitem, gpointer data)  /* averaging 平均 TAN1 P124 */
{
	int grp = get_current_group(pp->p_config);

	GROUP_VAL_POS(grp , averaging1) = GPOINTER_TO_UINT(data);
	TMP(group_spi[grp].average) =  GPOINTER_TO_UINT(data);
    group_spi_send(grp);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_125 (GtkSpinButton *spinbutton, gpointer data) /*抑制 Reject P125 */
{
	int grp = get_current_group(pp->p_config);
	set_reject(pp->p_config, (unsigned char) (gtk_spin_button_get_value (spinbutton)));

	/*发送给硬件*/
	TMP(group_spi[grp]).reject = get_reject(pp->p_config) * 40.95;	
    group_spi_send (grp);
}

void data_130 (GtkSpinButton *spinbutton, gpointer data) /*scan offset */
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , scan_offset) =  (int) (gtk_spin_button_get_value (spinbutton) * 100.0);

	/*发送给硬件*/
}

void data_131 (GtkSpinButton *spinbutton, gpointer data) /*scan offset */
{
	int grp = get_current_group(pp->p_config)  ;
	GROUP_VAL_POS(grp , index_offset) =  (int) (gtk_spin_button_get_value (spinbutton) * 10.0);
	/*发送给硬件*/
}

void data_132 (GtkSpinButton *spinbutton, gpointer data) /*scan offset */
{
	int _nTmpValue ;
	int grp = get_current_group (pp->p_config) ;
	_nTmpValue = (int) (gtk_spin_button_get_value (spinbutton) * 100.0);
	if(_nTmpValue == LAW_VAL_POS(grp , Angle_min))
	{
		return ;
	}
	LAW_VAL_POS(grp , Angle_min) =  _nTmpValue ;
	LAW_VAL_POS(grp , Angle_max) =  _nTmpValue ;
	TMP(current_angle[grp])  = _nTmpValue * G_PI / 18000.0 ;
	DrawDisplayWindowFrame() ;
}

void data_134 (GtkSpinButton *spinbutton, gpointer data) /* beam delay */
{
	int group = get_current_group(pp->p_config);
	int index = TMP(beam_num[group]);

	unsigned int _nTmpValue = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
	if(_nTmpValue == GROUP_VAL_POS(group , beam_delay[index]))
		return ;

	request_refresh(REFRESH_BEAM_DELAY_MARK) ;

	GROUP_VAL_POS(group , beam_delay[index]) = _nTmpValue;
	TMP(focal_law_all_beam[group][index]).G_delay			= _nTmpValue;

}

void data_135 (GtkSpinButton *spinbutton, gpointer data) /*gain offset */
{
	int _nOffset, k;
	int _nGroupId = get_current_group(pp->p_config);
	int _nIndex = TMP(beam_num[_nGroupId]);
	int _nIndexGlobal  ;
	_nOffset = 0  ;
	for ( k = 0 ; k < _nGroupId ;  k++)
		_nOffset += TMP(beam_qty[k])   ;
	_nIndexGlobal = _nOffset + _nIndex ;
	GROUP_VAL_POS(_nGroupId , gain_offset[_nIndex]) =  (unsigned short) (gtk_spin_button_get_value (spinbutton) * 10.0);
	/*发送给硬件*/
	TMP(focal_spi[_nIndexGlobal]).gain_offset	= GROUP_VAL_POS(_nGroupId , gain_offset[_nIndex]);
	write_focal_data (&TMP(focal_spi[_nIndexGlobal]), _nIndexGlobal , SPI_RESET_NO);
	//send_focal_spi(grp, SPI_RESET_NO);
}

void data_140 (GtkSpinButton *spinbutton, gpointer data)
{
	pthread_t thread_id;
	int ret;
	ret = pthread_create (&thread_id, NULL, (void*)SetDBEightPercentThread, data);
	if(ret){
		perror("in1:");
		return;
	}
}

void data_1431 (GtkSpinButton *spinbutton, gpointer data) /* point qty P143 */
{
	unsigned short temp_value ;
	int _nMaxBeamDelay  ;
	int grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	temp_value  =  (unsigned short)(gtk_spin_button_get_value (spinbutton));
	if(temp_value == GROUP_VAL_POS(grp , point_qty))  return ;

	request_refresh(REFRESH_POINT_MARK) ;
	GROUP_VAL_POS(grp , point_qty) = temp_value;
	TMP(group_spi[grp]).point_qty  = temp_value;//get_point_qty(grp);

	double _nCompressRate = GROUP_VAL_POS(grp , range) / (10 * GROUP_VAL_POS(grp , point_qty))   ;
	if(_nCompressRate < 1)
	{
		_nCompressRate = 1 ;
	}
	_nCompressRate = (int)_nCompressRate ;
	set_group_val (p_grp, GROUP_RANGE, GROUP_VAL_POS(grp , point_qty) * 10 * _nCompressRate);
	GROUP_VAL_POS(0 , prf1 )     = get_prf();
	TMP(group_spi[grp]).compress_rato	=_nCompressRate ;
	TMP(group_spi[grp]).sample_range	= TMP(group_spi[grp]).sample_start + 
		get_group_val (p_grp, GROUP_RANGE) / 10;		

	_nMaxBeamDelay = GetGroupMaxBeamDelay(grp) ;
	TMP(group_spi[grp]).rx_time		= TMP(group_spi[grp]).sample_range  + _nMaxBeamDelay + 5;

	char *markup;
	markup=g_markup_printf_escaped(
				"<span foreground='white' font_desc='10'>PRF: %d(%d)</span>", GROUP_VAL_POS (0, prf1) / 10, GROUP_VAL_POS(0 , prf1) * get_beam_qty() / 10);

	gtk_label_set_markup (GTK_LABEL(pp->label[3]),markup);
	g_free(markup);
	markup = g_markup_printf_escaped (
			"<span foreground='white' font_desc='10'>V: %.2f mm/s</span>",(double)(GROUP_VAL_POS(0 , prf1)/ 10.0));
	gtk_label_set_markup (GTK_LABEL (pp->label[5]), markup);
	g_free(markup);



}

void data_143 (GtkMenuItem *menuitem, gpointer data) /* point qty P143 */
{
	unsigned int temp = GPOINTER_TO_UINT (data);
	int grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	if(GROUP_VAL_POS(grp , point_qty_pos) == temp && temp != 4) return ;

	GROUP_VAL_POS(grp , point_qty_pos) = temp ;
	AutoSetPointQty(grp);
	//GROUP_VAL_POS(grp , point_qty) = get_point_qty(grp);
	TMP(group_spi[grp]).point_qty  = GROUP_VAL_POS(grp , point_qty);

	double _nCompressRate = GROUP_VAL_POS(grp , range) / (10 * GROUP_VAL_POS(grp , point_qty)) ;
	if(_nCompressRate < 1)
	{
		_nCompressRate = 1 ;
	}
	_nCompressRate = (int)_nCompressRate ;
	//if ((GROUP_VAL_POS(grp , point_qty) * _nCompressRate ) >= (get_group_val (p_grp, GROUP_RANGE)))
	set_group_val (p_grp, GROUP_RANGE, GROUP_VAL_POS(grp , point_qty) * 10 * _nCompressRate);
	GROUP_VAL_POS(0 , prf1) =  get_prf();
	if (temp != 4)
	{
		MENU_STATUS = MENU3_STOP;
		draw_menu3(0, NULL);
		gtk_spin_button_set_value (GTK_SPIN_BUTTON (pp->sbutton[3]), GROUP_VAL_POS(grp , point_qty)) ;

		TMP(group_spi[grp]).compress_rato	= _nCompressRate  ;
		TMP(group_spi[grp]).sample_range	= TMP(group_spi[grp]).sample_start + get_group_val (p_grp, GROUP_RANGE) / 10;
		TMP(group_spi[grp]).rx_time		= TMP(group_spi[grp]).sample_range  + GetGroupMaxBeamDelay(grp) + 5;

		char *markup;
		markup=g_markup_printf_escaped(
					"<span foreground='white' font_desc='10'>PRF: %d(%d)</span>", GROUP_VAL_POS (0, prf1) / 10, GROUP_VAL_POS(0 , prf1) * get_beam_qty() / 10);
		gtk_label_set_markup (GTK_LABEL(pp->label[3]), markup);
		g_free(markup);
		markup = g_markup_printf_escaped (
				"<span foreground='white' font_desc='10'>V: %.2f mm/s</span>",(double)(GROUP_VAL_POS(0 , prf1)/ 10.0));
		gtk_label_set_markup (GTK_LABEL (pp->label[5]), markup);
		g_free(markup);
		request_refresh(REFRESH_POINT_MARK) ;
	}
	else
	{
		pp->mark_pop_change = 1;
		MENU_STATUS = MENU3_PRESSED;
		draw_menu3(0, NULL);
		tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[3]));
	}
}

void data_1451 (GtkSpinButton *spinbutton, gpointer data) /* Sum Gain ->user_define*/
{
	double _nTmpValue ;
	int  _nGroupSumGain ;
	int grp = get_current_group (pp->p_config) ;
	_nTmpValue = gtk_spin_button_get_value (spinbutton) ;
	_nTmpValue = 16 * pow(10.0 , _nTmpValue / 20.0) ;
	_nGroupSumGain = (int) _nTmpValue ;
	if(_nGroupSumGain == GROUP_VAL_POS(grp , sum_gain))
	{
	    return  ;
	}
	GROUP_VAL_POS(grp , sum_gain) =  _nGroupSumGain ;
    TMP(group_spi[grp]).sum_gain  =  _nGroupSumGain ;
    group_spi_send (grp) ;
}


void data_145 (GtkMenuItem *menuitem, gpointer data) /* Sum Gain ->Atuo*/
{
	unsigned int	temp = GPOINTER_TO_UINT (data);
	int	grp	= get_current_group(pp->p_config);
	int _nGroupSumGain ;

	GROUP_VAL_POS(grp , sum_gain_pos) = temp  ;
	if (!temp)
	{
		if (LAW_VAL_POS(grp, Elem_qty) == 1)
			_nGroupSumGain	= 4095;
		else
			_nGroupSumGain	= 6400 / LAW_VAL_POS(grp, Elem_qty);
		GROUP_VAL_POS(grp , sum_gain) =  _nGroupSumGain ;
	    TMP(group_spi[grp]).sum_gain  =  _nGroupSumGain ;
        group_spi_send (grp) ;
		gtk_spin_button_set_value (GTK_SPIN_BUTTON (pp->sbutton[5]), 20 * log10(_nGroupSumGain / 16.0));
		MENU_STATUS = MENU3_STOP;
		draw_menu3(0, NULL);
	}
	else
	{
		pp->mark_pop_change = 1;
		MENU_STATUS = MENU3_PRESSED;
		draw_menu3(0, NULL);
		tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[5]));
	}

}

void data_200 (GtkMenuItem *menuitem, gpointer data) /* Gate 闸门选择 P200 */
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , gate_pos) = (char) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_201 (GtkMenuItem *menuitem, gpointer data) /* parameter 闸门参数选择 位置或者模式 P201 */
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , gate[GROUP_VAL_POS(grp , gate_pos)].parameters) = (char) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_202 (GtkSpinButton *spinbutton, gpointer data)	/* 闸门开始位置 P202 */
{
	int temp_value ;
	int grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS(grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS(grp , ut_unit)))
	{
		if (UNIT_MM == get_unit(pp->p_config))
			temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 2000.0 /
					(get_group_val (p_grp, GROUP_VELOCITY) / 100000.0));
		else  /* 英寸 */
			temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 2000.0 /
					(0.03937 * get_group_val (p_grp, GROUP_VELOCITY) / 100000.0));
	}
	else /* 显示方式为时间 */
	{
		temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 1000.0) ;
	}

	if(pp->clb_flag || pp->pos == 5)
	{
		if(temp_value == GROUP_VAL_POS(grp , gate[0].start))  return ;
		else   GROUP_VAL_POS(grp , gate[0].start) = temp_value ;
	}
	else
	{
		if(temp_value == GROUP_GATE_POS(start))  return ;
		else   GROUP_GATE_POS(start) = temp_value ;
	}
	gtk_widget_queue_draw (pp->vboxtable);

	request_refresh(REFRESH_GATE_POSITION) ;
}

void data_2021 (GtkMenuItem *menuitem, gpointer data)	/* 闸门同步 */
{
	unsigned char _nTmpValue ;
	unsigned char _nGatePos  ;
	unsigned char  tmp  = 0   ;
	int grp = get_current_group(pp->p_config);
	int group = get_current_group(pp->p_config);
	
	_nTmpValue  = (unsigned char) (GPOINTER_TO_UINT (data));
	_nGatePos   = GROUP_VAL_POS(group , gate_pos);
	GROUP_GATE_VAL_POS(group , _nGatePos ,  synchro) = _nTmpValue ;


	if (_nGatePos == GATE_A)
	{
		tmp = TMP(group_spi[group]).gate_a_logic;
		if(_nTmpValue == 0)
		{
			tmp = (tmp & 0xf3) | 0x00;
		}
		if(_nTmpValue == 1)
		{
			tmp = (tmp & 0xf3) | 0x04;
		}
		TMP(group_spi[group]).gate_a_logic = tmp;
	}
	else if (_nGatePos == GATE_B)
	{
		tmp = TMP(group_spi[group]).gate_b_logic;
		if(_nTmpValue == 0)
		{
			tmp = (tmp & 0xf3) | 0x00;
		}
		if(_nTmpValue == 1)
		{
			tmp = (tmp & 0xf3) | 0x04 ;
		}
		if(_nTmpValue == 2)
		{
			tmp = (tmp & 0xf3) | 0x08;
		}
		TMP(group_spi[group]).gate_b_logic = tmp;
	}
	else if (_nGatePos == GATE_I)
	{
		tmp = TMP(group_spi[group]).gate_i_logic;
		if(_nTmpValue == 0)
		{
			tmp = (tmp & 0xf3) | 0x00;
		}
		if(_nTmpValue == 1)
		{
			tmp = (tmp & 0xf3) | 0x04;
		}
		TMP(group_spi[group]).gate_i_logic = tmp;
	}

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
    group_spi_send (group);
	UpdateGateForSpiSending(grp);
	send_focal_spi (group , SPI_RESET_NO) ;
	pp->bGateSynchro  = IsGateSynchro() ;
	gtk_widget_queue_draw (pp->vboxtable);

}

void data_203 (GtkSpinButton *spinbutton, gpointer data) /* 闸门宽度 P203 */
{
	int temp_value ;
	int grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS(grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS(grp , ut_unit)))
	{
		if (UNIT_MM == get_unit(pp->p_config))
			temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 2000.0 /
					(get_group_val (p_grp, GROUP_VELOCITY) / 100000.0));
		else  /* 英寸 */
			temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 2000.0 /
					(0.03937 * get_group_val (p_grp, GROUP_VELOCITY) / 100000.0));
	}
	else /* 显示方式为时间 */
	{
		temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 1000.0) ;
	}
	if(pp->clb_flag || pp->pos == 5)
	{
		if(temp_value == GROUP_VAL_POS(grp , gate[0].width))   return ;
		GROUP_VAL_POS(grp , gate[0].width) = temp_value ;
	}
	else
	{
		if(temp_value == (GROUP_GATE_POS(width)))   return ;
		GROUP_GATE_POS(width) = temp_value ;
	}
	gtk_widget_queue_draw (pp->vboxtable);
	request_refresh(REFRESH_GATE_POSITION) ;

}

void data_2031 (GtkMenuItem *menuitem, gpointer data)	/* 波峰或者前沿 测量选项 */
{
	guint tmp=0;
	guint group = get_current_group(pp->p_config);
	GROUP_VAL_POS(group , gate[GROUP_VAL_POS(group , gate_pos)].measure) = (unsigned char) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);

	if (GROUP_VAL_POS(group , gate_pos) == GATE_A)
		tmp = TMP(group_spi[group]).gate_a_logic;
	else if (GROUP_VAL_POS(group , gate_pos) == GATE_B)
		tmp = TMP(group_spi[group]).gate_b_logic;
	else if (GROUP_VAL_POS(group , gate_pos) == GATE_I)
		tmp = TMP(group_spi[group]).gate_i_logic;

	if (GROUP_VAL_POS (group , gate[GROUP_VAL_POS(group , gate_pos)].measure) == 0)
		tmp = (tmp & 0xfffffffc) | 0x01;
	else if (GROUP_VAL_POS (group , gate[GROUP_VAL_POS(group , gate_pos)].measure) == 1)
		tmp = (tmp & 0xfffffffc) | 0x00;

	if (GROUP_VAL_POS(group , gate_pos) == GATE_A)
		TMP(group_spi[group]).gate_a_logic = tmp;
	else if (GROUP_VAL_POS(group , gate_pos) == GATE_B)
		TMP(group_spi[group]).gate_b_logic = tmp;
	else if (GROUP_VAL_POS(group , gate_pos) == GATE_I)
		TMP(group_spi[group]).gate_i_logic = tmp;

	change_language (get_language(pp->p_config), pp);//初始化语言 language init
	draw_field_name ();
    group_spi_send (group);
	gtk_widget_queue_draw (pp->vboxtable);
}

void data_204 (GtkSpinButton *spinbutton, gpointer data) /* 闸门高度 P204 */
{
	int grp = get_current_group(pp->p_config);
	unsigned int _nTmpValue = (unsigned int) ( gtk_spin_button_get_value (spinbutton));
	int _nGatePos = GROUP_VAL_POS(grp , gate_pos) ;
	if(pp->clb_flag)
	{
		if(_nTmpValue == GROUP_VAL_POS(grp , gate[GATE_A].height))  return ;
		GROUP_VAL_POS(grp , gate[GATE_A].height) = _nTmpValue ;
	}
	else
	{
		if(_nTmpValue == GROUP_VAL_POS(grp , gate[_nGatePos].height))  return ;
		GROUP_VAL_POS(grp , gate[_nGatePos].height) = _nTmpValue ;
	}

	_nTmpValue = _nTmpValue * 20.47 ;
	if (_nGatePos == GATE_A)
	{
		TMP(group_spi[grp]).gate_a_height	= _nTmpValue;
	}
	else if (_nGatePos == GATE_B)
	{
		TMP(group_spi[grp]).gate_b_height	= _nTmpValue;
	}
	else if (_nGatePos == GATE_I)
	{
		TMP(group_spi[grp]).gate_i_height	= _nTmpValue;
	}

    group_spi_send (grp);
	gtk_widget_queue_draw (pp->vboxtable);
}

void data_2041 (GtkMenuItem *menuitem, gpointer data) /* 闸门RF 选择 射频时候才可以调节 */
{
	int grp = get_current_group(pp->p_config) ;

	GROUP_VAL_POS(grp , gate[GROUP_VAL_POS(grp , gate_pos)].rectifier_freq) = (char) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	gtk_widget_queue_draw (pp->vboxtable);

}

void data_205(gpointer data) /* 闸门开关 F205 */
{
	int grp = get_current_group(pp->p_config) ;
	unsigned char mask = 0;

    if (get_overlay_gate(pp->p_config) == 1) {
        mask = 1<<grp;
        TMP(gateShow)[GROUP_VAL_POS(grp , gate_pos)]  = TMP(gateShow)[GROUP_VAL_POS(grp , gate_pos)] ^ mask;
    }
}

void data_210 (GtkMenuItem *menuitem, gpointer data) /* Alarm  P210 */
{
	set_alarm_pos (pp->p_config, (unsigned char) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	/* 发送给硬件 */
}

void data_211 (GtkMenuItem *menuitem, gpointer data) /* Group A P211 */
{
	set_alarm_groupa (pp->p_config,	(unsigned char) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	/* 发送给硬件 */
}
void data_212 (GtkMenuItem *menuitem, gpointer data) /* Condition A P212 */
{
	set_alarm_conditiona (pp->p_config,	(unsigned char) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	/* 发送给硬件 */
}

void data_213 (GtkMenuItem *menuitem, gpointer data) /* operator P213 */
{
	set_alarm_operator (pp->p_config, (unsigned char) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_214 (GtkMenuItem *menuitem, gpointer data) /* Group B P214 */
{
	set_alarm_groupb (pp->p_config,	(unsigned char) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_215 (GtkMenuItem *menuitem, gpointer data) /* condition B P215 */
{
	set_alarm_conditionb (pp->p_config,	(unsigned char) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_220 (GtkMenuItem *menuitem, gpointer data) /* Output P220 */
{	
	set_output_pos (pp->p_config, (unsigned char) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_221 (GtkMenuItem *menuitem, gpointer data) /* Output->alarm # P221 */
{
	int temp = (GPOINTER_TO_INT (data));
	int i;

	if (temp == 0)
	{
		for (i = 1; i < 17; i++) 
			set_output_alarm (pp->p_config, ALARM_OFF, i);
	}
	else if (temp == 1)
	{
		for (i = 1; i < 17; i++) 
			set_output_alarm (pp->p_config, ALARM_ON, i);
	}
	else if (temp ==  18)
	{
	}
	else 
	{
		set_output_alarm (pp->p_config, !get_output_alarm_pos (pp->p_config, temp -1), temp - 1);
	}

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_2211 (GtkMenuItem *menuitem, gpointer data) /* Output->group */
{
	set_output_group (pp->p_config, (unsigned char) (GPOINTER_TO_UINT (data)));

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_222 (GtkSpinButton *spinbutton, gpointer data) /* count P222 */
{
	set_output_count (pp->p_config, (unsigned char) (gtk_spin_button_get_value (spinbutton)));
}

void data_2221 (GtkMenuItem *menuitem, gpointer data) /* count */
{
	set_output_data (pp->p_config, (unsigned char) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_223 (GtkMenuItem *menuitem, gpointer data) /* sound P223 */
{
	set_output_sound (pp->p_config,	(unsigned char) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_224 (GtkSpinButton *spinbutton, gpointer data) /*active_delay */
{
	set_output_delay (pp->p_config,	(guint) (gtk_spin_button_get_value (spinbutton) * 1000.0));
}

void data_225 (GtkSpinButton *spinbutton, gpointer data) /* holdtime P225*/
{
	set_output_holdtime (pp->p_config,	(guint) (gtk_spin_button_get_value (spinbutton) * 1000.0));
}


void data_230 (GtkMenuItem *menuitem, gpointer data) /* Gate/Alarm->Sizing Curves->Mode P230 */
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , SizingCurves.mode_pos) = (unsigned char) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	// if chose setup, get out
	if(GROUP_VAL_POS(grp , SizingCurves.mode_pos) == 0)   return ;
	// quest if change all DAC or TCG parameters of all laws
	int ret ;
	if(GROUP_VAL_POS(grp , SizingCurves.curve_pos) == 1)  // DAC
	{
		ret = dialogWarning(pp->window , getDictString(_STRING_APPLY_EDIT_TO_ALL_LAWS_DAC));
	}
	else if(GROUP_VAL_POS(grp , SizingCurves.curve_pos) == 3)  // TCG
	{
		ret = dialogWarning(pp->window , getDictString(_STRING_APPLY_EDIT_TO_ALL_LAWS_TCG));
	}
	if(ret)
	{
		GROUP_VAL_POS(grp , SizingCurves.bApplyToAllLaws) = TRUE ;
		SynocDACParameters(grp , TMP(beam_num[grp])) ;
		SendTcgSpi(grp , -1 );
	}
	else
	{
		GROUP_VAL_POS(grp , SizingCurves.bApplyToAllLaws) = FALSE ;
	}
}


void data_231 (GtkMenuItem *menuitem, gpointer data) /* Gate/Alarm->Sizing Curves->Curve P231 */
{
	unsigned char temp_value ;
	pp->pos_pos = MENU3_STOP ;
	int grp = get_current_group(pp->p_config);
	int _nBeamNo = TMP(beam_num[grp]) ;
	temp_value = (unsigned char) (GPOINTER_TO_UINT (data));
	if( temp_value == GROUP_VAL_POS(grp , SizingCurves.curve_pos)) return ;
	// dac
	if(temp_value) set_overlay_sizing_curves (pp->p_config, 1)  ;
	if(temp_value == 1) {
		set_group_db_ref (pp->p_config, grp , 1) ;
        if (GROUP_VAL_POS(grp , SizingCurves.position[_nBeamNo][0]) < group_get_start (grp) )
		{
            GROUP_VAL_POS(grp , SizingCurves.position[_nBeamNo][0]) = group_get_start (grp) +
				get_group_val (get_group_by_id (pp->p_config, grp), GROUP_RANGE) / 10 ;
			GROUP_VAL_POS(grp , SizingCurves.position[_nBeamNo][1]) = GROUP_VAL_POS(grp , SizingCurves.position[_nBeamNo][0]) + get_group_val (get_group_by_id (pp->p_config, grp), GROUP_RANGE) / 10 ;
			GROUP_VAL_POS(grp , SizingCurves.position[_nBeamNo][2]) = GROUP_VAL_POS(grp , SizingCurves.position[_nBeamNo][1]) + get_group_val (get_group_by_id (pp->p_config, grp), GROUP_RANGE) / 10 ;
		}
		tt_label_show_string (pp->label[0], con2_p[1][0][6], "\n", "(dB)", "white", 10);
	}
	// linear dac
	if(temp_value == 2)
	{
		set_group_db_ref (pp->p_config, grp , 1) ;
		refresh_linear_dac_pointer_info()   ;
		tt_label_show_string (pp->label[0], con2_p[1][0][6], "\n", "(dB)", "white", 10);
	}
	// tcg
	if(temp_value == 3)
	{
		SendTcgSpi(grp , -1) ;
        enable_tcg(grp , TRUE) ;
	}
	if(GROUP_VAL_POS(grp , SizingCurves.curve_pos) == 3)
	{
		enable_tcg(grp , FALSE) ;
	}

	GROUP_VAL_POS(grp , SizingCurves.curve_pos) = temp_value;
	gtk_widget_queue_draw (pp->vboxtable);
	gtk_widget_queue_draw (pp->status_area) ;
	draw_menu3(0, NULL);
}


void data_2311 (GtkMenuItem *menuitem, gpointer data) /* Gate/Alarm->Sizing Curves->Point 231 */
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , SizingCurves.point_pos) = (unsigned char) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	gtk_widget_queue_draw (pp->vboxtable);
}

void data_2312 (GtkSpinButton *spinbutton, gpointer data) /* Mat.Attenuatior P2312 */
{
	unsigned short temp_value ;
	int grp = get_current_group (pp->p_config) ;
	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS(grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS(grp , ut_unit)))
	{
		if (UNIT_MM == get_unit(pp->p_config))
			temp_value = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 );
		else  /* 英寸 */
			temp_value = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 * 0.03937);
	}
	else /* 显示方式为时间 */
		temp_value = (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 * 0.338) ;

	if(temp_value == GROUP_VAL_POS(grp , SizingCurves.mat_atten))  return ;

	GROUP_VAL_POS(grp , SizingCurves.mat_atten) = temp_value ;
	refresh_linear_dac_pointer_info() ;
	gtk_widget_queue_draw (pp->vboxtable);
}

void data_232 (GtkSpinButton *spinbutton, gpointer data) /* Ref.Amplitude P232 */
{
	int i;
	int temp_value ;
	int grp = get_current_group (pp->p_config) ;
	int _nBeamNo = TMP(beam_num[grp]) ;
	temp_value = (unsigned short) (gtk_spin_button_get_value (spinbutton) * 10.0);

	// if linear DAC  refresh the points information
	if(GROUP_VAL_POS(grp , SizingCurves.curve_pos) == 2)
	{
		if(temp_value == GROUP_VAL_POS(grp , SizingCurves.linear_ref_ampl))  return  ;
		GROUP_VAL_POS(grp , SizingCurves.linear_ref_ampl) =  temp_value ;
		GROUP_VAL_POS(grp , SizingCurves.amplitude[_nBeamNo][0]) = GROUP_VAL_POS(grp , SizingCurves.linear_ref_ampl) * 100;
		refresh_linear_dac_pointer_info () ;
	}
	else
	{// 确保所有点的幅度都小于参考幅度
		if(temp_value == GROUP_VAL_POS(grp , SizingCurves.dac_ref_ampl[_nBeamNo]))  return  ;
		GROUP_VAL_POS(grp , SizingCurves.dac_ref_ampl[_nBeamNo]) =  temp_value ;
		GROUP_VAL_POS(grp , SizingCurves.amplitude[_nBeamNo][0]) = GROUP_VAL_POS(grp , SizingCurves.dac_ref_ampl[_nBeamNo]) * 100;

		for(i =0; i< GROUP_VAL_POS(grp , SizingCurves.dac_point_qty); i++)
		{
			if(100 * GROUP_VAL_POS(grp , SizingCurves.dac_ref_ampl[_nBeamNo]) < GROUP_VAL_POS(grp , SizingCurves.amplitude[_nBeamNo][i]))
				GROUP_VAL_POS(grp , SizingCurves.amplitude[_nBeamNo][i]) = 100 * GROUP_VAL_POS(grp , SizingCurves.dac_ref_ampl[_nBeamNo]);
		}

		if(GROUP_VAL_POS(grp , SizingCurves.bApplyToAllLaws))
		{
			SynocDACParameters(grp , _nBeamNo) ;
		}

	}

	gtk_widget_queue_draw (pp->vboxtable);

}
void data_2321 (GtkSpinButton *spinbutton, gpointer data) /* Position P2321 */
{
	int i ;
	int temp_value ;
	int      grp = get_current_group(pp->p_config);
	int _nBeamNo = TMP(beam_num[grp]);
	GROUP  *p_grp = get_group_by_id (pp->p_config, grp);
	int point_pos = GROUP_VAL_POS(grp , SizingCurves.point_pos) ;
	int point_count = (int)GROUP_VAL_POS(grp , SizingCurves.dac_point_qty) ;
	double current_angle_cos = cos(TMP(current_angle[grp]));

	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS(grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS(grp , ut_unit)))
	{
		if (UNIT_MM == get_unit(pp->p_config))
			temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 2000.0 /
					( current_angle_cos * get_group_val (p_grp, GROUP_VELOCITY) / 100000.0)) ;
		else  /* 英寸 */
			temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 2000.0 /
					( current_angle_cos * 0.03937 * get_group_val (p_grp, GROUP_VELOCITY) / 100000.0)) ;
	}
	else /* 显示方式为时间 */
		temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 1000.0) ;

	temp_value = ((temp_value + 5) / 10 ) * 10 ;
	if(temp_value == GROUP_VAL_POS(grp , SizingCurves.position[_nBeamNo][point_pos])) return  ;
	//*********************************************************************************
	GROUP_VAL_POS(grp , SizingCurves.position[_nBeamNo][point_pos]) = temp_value ;

	for(i = point_pos ; i< point_count ; i++)
	{
		if(GROUP_VAL_POS(grp , SizingCurves.position[_nBeamNo][i]) < temp_value )
			GROUP_VAL_POS(grp , SizingCurves.position[_nBeamNo][i]) = temp_value  ;
	}

	if(GROUP_VAL_POS(grp , SizingCurves.bApplyToAllLaws))
	{
		SynocDACParameters(grp , _nBeamNo) ;
	}

	if(GROUP_VAL_POS(grp , SizingCurves.curve_pos) == 3)
	{
		SendTcgSpi(grp , -1) ;
	}
	gtk_widget_queue_draw (pp->vboxtable);

}



void data_2322 (GtkSpinButton *spinbutton, gpointer data) /* Delay P2322 */
{
	int temp_value ;
	int      grp = get_current_group(pp->p_config);
	GROUP  *p_grp = get_group_by_id (pp->p_config, grp);
	double current_angle_cos = cos(TMP(current_angle[grp]));
	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS(grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS(grp , ut_unit)))
	{
		if (UNIT_MM == get_unit(pp->p_config))
			temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 2000.0 /
					( current_angle_cos * get_group_val (p_grp, GROUP_VELOCITY) / 100000.0)) ;
		else  /* 英寸 */
			temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 2000.0 /
					( current_angle_cos * 0.03937 * get_group_val (p_grp, GROUP_VELOCITY) / 100000.0)) ;
	}
	else /* 显示方式为时间 */
		temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 1000.0) ;

	temp_value = ((temp_value + 5) / 10 ) * 10 ;
	if(temp_value == GROUP_VAL_POS(grp , SizingCurves.delay)) return  ;
	GROUP_VAL_POS(grp , SizingCurves.delay) = temp_value  ;
	refresh_linear_dac_pointer_info() ;
	gtk_widget_queue_draw (pp->vboxtable);
}

void data_2323 (GtkSpinButton *spinbutton, gpointer data) /* pcs : tofd calibration*/
{
	double _fValue = gtk_spin_button_get_value (spinbutton);
	if (UNIT_INCH == get_unit(pp->p_config))
	{
		_fValue /= 0.03937 ;
	}
	int grp = get_current_group(pp->p_config);
	if (fabs(_fValue - GROUP_VAL_POS(grp , field_distance[255])) > 0.001)
	{
		//printf("data 2323!\n");
		GROUP_VAL_POS(grp , field_distance[255]) = _fValue ;
	}
}

void data_233 (GtkSpinButton *spinbutton, gpointer data) /*Ref.Amplitude.Offset */
{
	int grp = get_current_group (pp->p_config) ;
	int temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 100.0) ;
	if(temp_value == GROUP_VAL_POS(grp , SizingCurves.ref_ampl_offset)) return ;

	GROUP_VAL_POS(grp , SizingCurves.ref_ampl_offset) =  temp_value;
	gtk_widget_queue_draw (pp->vboxtable);
}

void data_2331 (GtkSpinButton *spinbutton, gpointer data) /*DAC point .Amplitude */
{
	int grp = get_current_group (pp->p_config) ;
	int _nBeamNo = TMP(beam_num[grp]) ;
	int point_pos = GROUP_VAL_POS(grp , SizingCurves.point_pos) ;

	unsigned int temp_value =  (unsigned int) (gtk_spin_button_get_value (spinbutton) * 1000.0);
	if(temp_value == GROUP_VAL_POS(grp , SizingCurves.amplitude[_nBeamNo][point_pos]))  return ;
	GROUP_VAL_POS(grp , SizingCurves.amplitude[_nBeamNo][point_pos]) = temp_value ;

	if(GROUP_VAL_POS(grp , SizingCurves.bApplyToAllLaws))
	{
		SynocDACParameters(grp , _nBeamNo) ;
	}

	gtk_widget_queue_draw (pp->vboxtable);
}


void data_2332 (GtkSpinButton *spinbutton, gpointer data) /* TCG GAIN */
{
	int temp_value ;
	//int i ;
	double current_db ;
	int      grp = get_current_group(pp->p_config);
	int _nBeamNo = TMP(beam_num[grp]) ;
	unsigned char point_pos = GROUP_VAL_POS(grp , SizingCurves.point_pos) ;
	//int point_count = (int)GROUP_VAL_POS(grp , SizingCurves.dac_point_qty) ;
	temp_value = (unsigned int) (gtk_spin_button_get_value (spinbutton) * 100.0) ;
	current_db = pow(10.0 , temp_value / 2000.0) ;
	temp_value = (unsigned int)(GROUP_VAL_POS(grp , SizingCurves.amplitude[_nBeamNo][0]) / current_db) ;
	if(temp_value == GROUP_VAL_POS(grp , SizingCurves.amplitude[_nBeamNo][point_pos]))  return ;

	GROUP_VAL_POS(grp , SizingCurves.amplitude[_nBeamNo][point_pos]) = temp_value ;


	if(GROUP_VAL_POS(grp , SizingCurves.bApplyToAllLaws))
	{
		SynocDACParameters(grp , _nBeamNo) ;
	}

	SendTcgSpi(grp , -1) ;
	//enable_tcg(grp , TRUE) ;
	gtk_widget_queue_draw (pp->vboxtable);
}

void data_234 (GtkSpinButton *spinbutton, gpointer data) /*   */
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , SizingCurves.curve_step) =  (guint) (gtk_spin_button_get_value (spinbutton) * 100.0);
	gtk_widget_queue_draw (pp->vboxtable);
}

void data_234_add_point (DRAW_UI_P p) /*   */
{
	int      grp = get_current_group(pp->p_config);
	AddDacPoint(grp) ;
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	gtk_widget_queue_draw (pp->vboxtable);

	if(GROUP_VAL_POS(grp , SizingCurves.curve_pos) == 3)
	{
		SendTcgSpi(grp , -1) ;
		enable_tcg(grp , TRUE) ;
	}

}

void data_235_del_point  (DRAW_UI_P p) /*   */
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , SizingCurves.dac_point_qty) = GROUP_VAL_POS(grp , SizingCurves.dac_point_qty)-1 ;
	if(GROUP_VAL_POS(grp , SizingCurves.dac_point_qty)<1)
	{
		GROUP_VAL_POS(grp , SizingCurves.dac_point_qty)= 1 ;
	}
	else
	{
		GROUP_VAL_POS(grp , SizingCurves.point_pos) = GROUP_VAL_POS(grp , SizingCurves.dac_point_qty)-1;
		pp->pos_pos = MENU3_STOP;
		draw_menu3(0, NULL);
		gtk_widget_queue_draw (pp->vboxtable);
	}

	if(GROUP_VAL_POS(grp , SizingCurves.curve_pos) == 3)
	{
		SendTcgSpi(grp , -1) ;
		enable_tcg(grp , TRUE) ;
	}
}

void data_235 (GtkSpinButton *spinbutton, gpointer data) /*  reference gain */
{
	int grp = get_current_group(pp->p_config);
    int gain = 0;
	int temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 100.0) ;

    if(group_get_refgain(grp) == temp_value) {
        return ;
    }

    gain = group_get_gain(grp) - group_get_refgain(grp);

    group_set_refgain(grp, (gshort)temp_value);

    group_set_gain(grp, gain+temp_value);

	RefreshGainMark(grp) ;

	group_data_spi new, *p1;
	memcpy (&new, &TMP(group_spi[grp]), sizeof (group_data_spi));
	p1 = &new;
	p1->offset = 16 * grp;
	p1->addr = 0x2;
	little_to_big ((unsigned int *)(p1),2);

#if ARM
	write (fd_array, (unsigned char *)(p1), 8);
#endif

	if(ConfirmGainOffsetOfAllBeamInLimit(grp))
	{
		send_focal_spi(grp , SPI_RESET_NO)     ;
	}
}

void setFieldIndex(int list ,int fieldNO ,unsigned char value);
unsigned char getFieldIndex(int list ,int fieldNO);
void updateColumn(int columnEnum);
void data_300 (GtkMenuItem *menuitem, gpointer data) /* Measurements->Reading->list 300 */
{
	set_reading_list (pp->p_config, (char)(GPOINTER_TO_UINT (data)));

	set_reading_field1 (pp->p_config ,getFieldIndex(GPOINTER_TO_UINT(data) ,0));
	set_reading_field2 (pp->p_config ,getFieldIndex(GPOINTER_TO_UINT(data) ,1));
	set_reading_field3 (pp->p_config ,getFieldIndex(GPOINTER_TO_UINT(data) ,2));
	set_reading_field4 (pp->p_config ,getFieldIndex(GPOINTER_TO_UINT(data) ,3));

	draw_field_name ();
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}
#if COLUMN_FIELD1

#else
#define COLUMN_FIELD1 5
#endif
void data_302 (GtkMenuItem *menuitem, gpointer data) /* Measurements->Reading->Field1 302 */ 
{
	set_reading_field1 (pp->p_config, (char) (GPOINTER_TO_UINT (data)));
	setFieldIndex(get_reading_list(pp->p_config) ,0 ,(char) (GPOINTER_TO_UINT (data)));
	updateColumn(get_reading_list(pp->p_config) * 4 + 0 + COLUMN_FIELD1);
	pp->pos_pos = MENU3_STOP;
	draw_field_name ();
	draw_menu3(0, NULL);
}
void data_303 (GtkMenuItem *menuitem, gpointer data) /* Measurements->Reading->Field2 303 */
{
	set_reading_field2 (pp->p_config, (char) (GPOINTER_TO_UINT (data)));
	setFieldIndex(get_reading_list(pp->p_config) ,1 ,(char) (GPOINTER_TO_UINT (data)));
	updateColumn(get_reading_list(pp->p_config) * 4 + 1 + COLUMN_FIELD1);
	pp->pos_pos = MENU3_STOP;
	draw_field_name ();
	draw_menu3(0, NULL);
}
void data_304 (GtkMenuItem *menuitem, gpointer data) /* Measurements->Reading->Field3 304 */
{
	set_reading_field3 (pp->p_config, (char) (GPOINTER_TO_UINT (data)));
	setFieldIndex(get_reading_list(pp->p_config) ,2 ,(char) (GPOINTER_TO_UINT (data)));
	updateColumn(get_reading_list(pp->p_config) * 4 + 2 + COLUMN_FIELD1);
	pp->pos_pos = MENU3_STOP;
	draw_field_name ();
	draw_menu3(0, NULL);
}
void data_305 (GtkMenuItem *menuitem, gpointer data) /* Measurements->Reading->Field4 305 */
{
	set_reading_field4 (pp->p_config, (char) (GPOINTER_TO_UINT (data)));
	setFieldIndex(get_reading_list(pp->p_config) ,3 ,(char) (GPOINTER_TO_UINT (data)));
	updateColumn(get_reading_list(pp->p_config) * 4 + 3 + COLUMN_FIELD1);
	pp->pos_pos = MENU3_STOP;
	draw_field_name ();
	draw_menu3(0, NULL);
}

void data_310 (GtkMenuItem *menuitem, gpointer data) /* Measurements->cursors->selection 310 */
{
	int grp = get_current_group (pp->p_config) ;

	GROUP_VAL_POS(grp , selection) = (unsigned char) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	draw_area_all();
}

void data_311 (GtkSpinButton *spinbutton, gpointer data) 
{
	unsigned short temp_value ;
	int grp = get_current_group (pp->p_config) ;
	temp_value = (unsigned short) (gtk_spin_button_get_value (spinbutton)*100.0);
	if(temp_value == GROUP_VAL_POS(grp , per_reference))  return ;
	GROUP_VAL_POS(grp , per_reference) =  temp_value ;
	gtk_widget_queue_draw (pp->vboxtable);

}
void data_3111 (GtkSpinButton *spinbutton, gpointer data) 
{
	int temp_value ;
	int grp = get_current_group (pp->p_config) ;

	if(get_inspec_source (pp->p_config) == 0)
	{
		double rate =  GROUP_VAL_POS(0 , prf1) * get_area_scanresolution (pp->p_config) / 10000.0;
		temp_value  =  (int) (gtk_spin_button_get_value (spinbutton) * rate * 100.0);
		if(temp_value == GROUP_VAL_POS(grp , s_reference))  return ;
	}
	else
	{
		if(get_unit(pp->p_config) == UNIT_MM)
			temp_value  =  (int) (gtk_spin_button_get_value (spinbutton) * 100.0);
		else
			temp_value =  (int) (gtk_spin_button_get_value (spinbutton) * 100.0 / 0.03937);
		if(temp_value == GROUP_VAL_POS(grp , s_reference))  return ;

	}


	GROUP_VAL_POS(grp , s_reference) = temp_value ;
	gtk_widget_queue_draw (pp->vboxtable);
}

void data_3112 (GtkSpinButton *spinbutton, gpointer data) 
{
	int temp_value = 0;
	int beam_num    ;
	unsigned char grp = get_current_group(pp->p_config) ;

	if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
	{
		temp_value = (short) ((gtk_spin_button_get_value (spinbutton)) * 100 );
		if(temp_value < LAW_VAL_POS(grp, Angle_min))  temp_value = LAW_VAL_POS(grp , Angle_min) ;
		if(temp_value > LAW_VAL_POS(grp, Angle_max))  temp_value = LAW_VAL_POS(grp , Angle_max) ;

		beam_num = (temp_value - LAW_VAL_POS(grp, Angle_min)) / LAW_VAL_POS(grp, Angle_step);
		if(beam_num == TMP(beam_num[grp])) return ;
		TMP(beam_num[grp]) = beam_num ;
		temp_value = LAW_VAL_POS(grp , Angle_min) + beam_num * LAW_VAL_POS(grp , Angle_step) ;
		TMP(current_angle[grp]) = temp_value * G_PI / 18000.0 ;
	}
	else if(LAW_VAL_POS(grp , Focal_type) == LINEAR_SCAN)
	{
		temp_value = (short) (gtk_spin_button_get_value (spinbutton)) - 1 ;
		if(temp_value  == TMP(beam_num[grp]))    return ;
		TMP(beam_num[grp]) = temp_value ;
	}
//    request_refresh(REFRESH_CURRENT_LAW_CHANGED);
    bscan_set_refresh(TRUE);
    gtk_widget_queue_draw (pp->vboxtable);//DrawDisplayWindowFrame() ;
	UpdateWindowTitle()  ;
}

void data_3113 (GtkSpinButton *spinbutton, gpointer data) 
{
	short temp_value ;
	temp_value = (short) (gtk_spin_button_get_value (spinbutton)*100.0);
	if(GROUP_CURSORS_POS(angle) == temp_value ) return ;
	GROUP_CURSORS_POS(angle) = temp_value  ;
	gtk_widget_queue_draw (pp->vboxtable);
}

void data_312 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	int grp = get_current_group (pp->p_config) ;
	unsigned short temp_value = (unsigned short) (gtk_spin_button_get_value (spinbutton)*100.0);
	if(GROUP_VAL_POS(grp , per_measure) == temp_value)  return ;
	GROUP_VAL_POS(grp , per_measure) = temp_value  ;
	gtk_widget_queue_draw (pp->vboxtable);
}

void data_3121 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	int temp_value ;
	int grp = get_current_group (pp->p_config) ;

	if(get_inspec_source (pp->p_config) == 0)
	{
		double rate =  GROUP_VAL_POS(0 , prf1) * get_area_scanresolution (pp->p_config) / 10000.0;
		temp_value  =  (int) (gtk_spin_button_get_value (spinbutton) * rate * 100.0);
		if(temp_value == GROUP_VAL_POS(grp , s_measure))  return ;
	}
	else
	{
		if(get_unit(pp->p_config) == UNIT_MM)
			temp_value =  (int) (gtk_spin_button_get_value (spinbutton)*100.0);
		else
			temp_value =  (int) (gtk_spin_button_get_value (spinbutton)*100.0/0.03937);
		if(temp_value == GROUP_VAL_POS(grp , s_measure)) return ;

	}

	GROUP_VAL_POS(grp , s_measure) = temp_value ;
	gtk_widget_queue_draw (pp->vboxtable);
}

void data_3122 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	unsigned short temp_value ;
	temp_value = (gushort) (gtk_spin_button_get_value (spinbutton)*100.0);
	if(temp_value == GROUP_CURSORS_POS(amplitude))  return ;
	GROUP_CURSORS_POS(amplitude) =  temp_value ;
	gtk_widget_queue_draw (pp->vboxtable);
}

void data_313 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	unsigned int temp_value ;
	int grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS(grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS(grp , ut_unit)))
	{
        if (UNIT_MM == get_unit(pp->p_config)) {
			temp_value = (guint) (gtk_spin_button_get_value (spinbutton) * 2000.0 /
					(get_group_val (p_grp, GROUP_VELOCITY) / 100000.0));
        } else { /* 英寸 */
			temp_value = (guint) (gtk_spin_button_get_value (spinbutton) * 2000.0 /
					(0.03937 * get_group_val (p_grp, GROUP_VELOCITY) / 100000.0));
        }
	}
	else /* 显示方式为时间 */
		temp_value = gtk_spin_button_get_value (spinbutton) * 1000.0 ;
	if(GROUP_VAL_POS(grp , u_reference) == temp_value)  return ;
	GROUP_VAL_POS(grp , u_reference) = temp_value ;
	gtk_widget_queue_draw (pp->vboxtable);
}

void data_3131 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	int temp_value ;
	int grp = get_current_group (pp->p_config) ;
	if(get_unit(pp->p_config) == UNIT_MM)
		temp_value =  (int) (gtk_spin_button_get_value (spinbutton)*100.0);
	else
		temp_value =  (int) (gtk_spin_button_get_value (spinbutton)*100.0/0.03937);
	if(temp_value == GROUP_VAL_POS(grp , i_reference))   return ;
	GROUP_VAL_POS(grp , i_reference)  = temp_value  ;
	gtk_widget_queue_draw (pp->vboxtable);
}

void data_3132 (GtkSpinButton *spinbutton, gpointer data) /* */
{

	int temp_value ;
	int grp = get_current_group (pp->p_config) ;
	if(get_unit(pp->p_config) == UNIT_MM)
		temp_value =  (int) (gtk_spin_button_get_value (spinbutton)*100.0);
	else
		temp_value =  (int) (gtk_spin_button_get_value (spinbutton)*100.0/0.03937);
	if(temp_value == GROUP_VAL_POS(grp , s_reference))   return ;
	GROUP_VAL_POS(grp , s_reference)  = temp_value  ;
	gtk_widget_queue_draw (pp->vboxtable);
#if 0
	int temp_value ;
	int grp = get_current_group (pp->p_config) ;
	if(get_unit(pp->p_config) == UNIT_MM)
		temp_value =  (gushort) (gtk_spin_button_get_value (spinbutton)*100.0);
	else
		temp_value =  (gushort) (gtk_spin_button_get_value (spinbutton)*100.0/0.03937);
	if(GROUP_VAL_POS(grp , s_refmeas) == temp_value)  return ;
	GROUP_VAL_POS(grp , s_refmeas) = temp_value ;
	gtk_widget_queue_draw (pp->vboxtable);
#endif
}

void data_3133 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_CURSORS_POS(UT) =  (gshort) (gtk_spin_button_get_value (spinbutton)*100.0);

	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS(grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS(grp , ut_unit)))
	{
		if(get_unit(pp->p_config) == UNIT_MM)
			GROUP_CURSORS_POS(UT) =  (gshort) (gtk_spin_button_get_value (spinbutton)*100.0);
		else
			GROUP_CURSORS_POS(UT) =  (gshort) (gtk_spin_button_get_value (spinbutton)*100.0/0.03937);
	}
	else
		GROUP_CURSORS_POS(UT) =  (gshort) (gtk_spin_button_get_value (spinbutton)*100.0);
	gtk_widget_queue_draw (pp->vboxtable);
}

void data_314 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	unsigned int temp_value ;
	int grp = get_current_group(pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS(grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS(grp , ut_unit)))
	{
		if (UNIT_MM == get_unit(pp->p_config))
			temp_value = (guint) (gtk_spin_button_get_value (spinbutton) * 2000.0 /
					(get_group_val (p_grp, GROUP_VELOCITY) / 100000.0));
		else  /* 英寸 */
			temp_value = (guint) (gtk_spin_button_get_value (spinbutton) * 2000.0 /
					(0.03937 * get_group_val (p_grp, GROUP_VELOCITY) / 100000.0));
	}
	else /* 显示方式为时间 */
		temp_value = gtk_spin_button_get_value (spinbutton) * 1000.0 ;

	if(GROUP_VAL_POS(grp , u_measure) == temp_value)   return ;
	GROUP_VAL_POS(grp , u_measure) = temp_value ;

	gtk_widget_queue_draw (pp->vboxtable);
}

void data_3141 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	int temp_value ;
	int grp = get_current_group (pp->p_config) ;
	if(get_unit(pp->p_config) == UNIT_MM)
		temp_value =  (int) (gtk_spin_button_get_value (spinbutton)*100.0);
	else
		temp_value =  (int) (gtk_spin_button_get_value (spinbutton)*100.0/0.03937);
	if(GROUP_VAL_POS(grp , i_measure) == temp_value)   return  ;
	GROUP_VAL_POS(grp , i_measure) = temp_value ;
	gtk_widget_queue_draw (pp->vboxtable);
}

void data_3143 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	int temp_value ;
	int grp = get_current_group (pp->p_config) ;
	if(get_unit(pp->p_config) == UNIT_MM)
		temp_value =  (int) (gtk_spin_button_get_value (spinbutton)*100.0);
	else
		temp_value =  (int) (gtk_spin_button_get_value (spinbutton)*100.0/0.03937);
	if(GROUP_VAL_POS(grp , s_measure) == temp_value)   return  ;
	GROUP_VAL_POS(grp , s_measure) = temp_value ;
	gtk_widget_queue_draw (pp->vboxtable);
}


void data_3142 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	int _nTempValue ;
	int _nIndex ;
	if(get_unit(pp->p_config) == UNIT_MM)
		_nTempValue = (int) (gtk_spin_button_get_value (spinbutton)*100.0);
	else
		_nTempValue =(int) (gtk_spin_button_get_value (spinbutton)*100.0/0.03937);
	if(TMP(scan) == _nTempValue) return ;

	TMP(scan) =  _nTempValue ;
	if(!TMP(dataRecalling))  return ;

	int _nDataSize   =   getDataBlockSize();
	if(get_inspec_source(pp->p_config))
	{
		_nIndex = (_nTempValue * 10 - pp->p_config->inspection_scan_start ) / pp->p_config->inspection_scan_resolution ;
	}
	else
	{
		_nIndex =    _nTempValue * 10  / pp->p_config->inspection_scan_resolution ;
		DMA_MARK[9] = _nIndex ;
	}
	if(ScanDataMark[_nIndex])
	{
		memcpy((void*)TMP(dma_data_add) ,(void*)(TMP(StoreBuffAddr) + _nIndex * _nDataSize) ,_nDataSize);
	}
	else
	{
		memset((void*)TMP(dma_data_add) ,0 , _nDataSize);
	}
	gtk_widget_queue_draw (pp->vboxtable);
}

void data_315 (GtkSpinButton *spinbutton, gpointer data) /* */
{
	if(get_unit(pp->p_config) == UNIT_MM)
		TMP(index) =  (int) (gtk_spin_button_get_value (spinbutton)*100.0);
	else
		TMP(index) =  (int) (gtk_spin_button_get_value (spinbutton)*100.0/0.03937);
	gtk_widget_queue_draw (pp->vboxtable);
}

void tableSelect(int entry);
void data_324 (GtkSpinButton *spinbutton, gpointer data) /* entry_qty P324 */
{
	set_cur_entry (pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton)));
	tableSelect(get_cur_entry(pp->p_config));
	gtk_widget_queue_draw (pp->vboxtable);
}

void data_330 (GtkMenuItem *menuitem, gpointer data) /* Measurements->Thickness->source p330 */
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , source) = (unsigned char) (GPOINTER_TO_UINT (data));


	TMP(group_spi[grp]).thickness_source = GPOINTER_TO_UINT (data);//
	//发送group参数
    group_spi_send(grp);

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);

	request_refresh(REFRESH_THICKNESS_CHANGED) ;
}


#include <assert.h>
static inline void get_thickness_factor(int group)
{
	assert(group < 16);
	int value = TMP(group_spi[group]).thickness_max - TMP(group_spi[group]).thickness_min;
	if(value > 0)
	{
		TMP(group_spi[group]).thickness_factor = 0xffffff / value;
	}else{
		TMP(group_spi[group]).thickness_factor = 0xffffff;//模拟输出最大值
	}
}

void data_331 (GtkSpinButton *spinbutton, gpointer data) /*min_thickness p331 */
{
	int _nTmpValue ;
	int grp = get_current_group(pp->p_config);
	if(UNIT_MM == get_unit(pp->p_config))
		_nTmpValue =  (int) (gtk_spin_button_get_value (spinbutton) * 1000.0);
	else
		_nTmpValue =  (int) ((gtk_spin_button_get_value (spinbutton) * 1000.0) / 0.03937 );
	if(_nTmpValue == GROUP_VAL_POS(grp , min_thickness))  return ;
	GROUP_VAL_POS(grp , min_thickness) = _nTmpValue  ;
	//把厚度换算成时间
	int velocity = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY); //cm/s
	int group = get_current_group(pp->p_config);
	double thickness = gtk_spin_button_get_value (spinbutton);
	double temp = 10000000.0 / velocity;//mm / (cm/s) = 10^8 ns   单位为10ns
	TMP(group_spi[group]).thickness_min = (unsigned int)(thickness * temp );//
	get_thickness_factor(group);
    //
    request_refresh(REFRESH_THICKNESS_CHANGED) ;
}

void data_332 (GtkSpinButton *spinbutton, gpointer data) /*max_thickness p332 */
{
	int _nTmpValue ;
	int grp = get_current_group(pp->p_config);
	if(UNIT_MM == get_unit(pp->p_config))
		_nTmpValue =  (int) (gtk_spin_button_get_value (spinbutton) * 1000.0);
	else
		_nTmpValue =  (int) ((gtk_spin_button_get_value (spinbutton) * 1000.0) / 0.03937 );
	if(_nTmpValue == GROUP_VAL_POS(grp , max_thickness))  return ;
	GROUP_VAL_POS(grp , max_thickness) = _nTmpValue  ;
	//把厚度换算成时间
	int velocity = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY); //cm/s
	int group = get_current_group(pp->p_config);
	double thickness = gtk_spin_button_get_value (spinbutton);
	double xx = 10000000.0 / velocity; //mm / (cm/s) = 10^8 ns   单位为10ns
	TMP(group_spi[group]).thickness_max = (unsigned int)(thickness * (xx) );//
	get_thickness_factor(group);

	request_refresh(REFRESH_THICKNESS_CHANGED) ;
}

void data_333 (GtkSpinButton *spinbutton, gpointer data) /*echo_qty p333 */
{
	set_echo_qty(pp->p_config, (unsigned char) (gtk_spin_button_get_value (spinbutton)));
	//更改模拟参数
	unsigned int value = (unsigned int) gtk_spin_button_get_value (spinbutton);
	output_set_parameter(0 ,OUTPUT_ANALOG_COMMAND_FACTOR_ECHO ,value ,0);
	//发送所有模拟参数到spi
	output_write_one_reg_to_spi(0 ,OUTPUT_ANALOG_COMMAND_FACTOR_ECHO);
}

void data_400 (GtkMenuItem *menuitem, gpointer data) /* Display->Selection->display p400 */
{
	unsigned char temp_value ;
	temp_value = (unsigned char)(GPOINTER_TO_UINT (data)) ;
	if(temp_value == get_display_pos(pp->p_config))  return ;
	int ret = IsDisplayModalAvailable(temp_value) ;
	if(!ret)
	{
		return ;
	}
	set_display_pos (pp->p_config, temp_value) ;

	if(temp_value == PA_TOFD || temp_value == PA_TOFD_CSCAN)
	{
		set_display_group(pp->p_config , DISPLAY_ALL_GROUP);
	}
	pp->pos_pos = MENU3_STOP;
	draw_menu3 (0, NULL);
	//request_refresh(REFRESH_DRAW_AREA_ALL);
	draw_area_all();
	pp->bRefreshDraw = TRUE ;
}

void data_401 (GtkMenuItem *menuitem, gpointer data) /* Display->Selection Ascan->group p401 */
{
	unsigned char temp_value ;
    int i, countB = 0, countC = 0, countS = 0;
	int grp = get_current_group (pp->p_config) ;
	int _nDisplayModal = get_display_pos(pp->p_config) ;
	temp_value = (unsigned char)(GPOINTER_TO_UINT (data)) ;

	if(temp_value == get_display_group(pp->p_config))
	{
		return ;
	}
	//设成all再看合适否，不合适再设回来，bug edit by lzk
	set_display_group (pp->p_config, temp_value);
	int ret = IsDisplayModalAvailable(get_display_pos(pp->p_config)) ;
	if(!ret)
	{
		set_display_group (pp->p_config, DISPLAY_CURRENT_GROUP);
		pp->pos_pos = MENU3_STOP;
		draw_menu3(0, NULL);
		return ;
	}

    if (_nDisplayModal == PA_TOFD || _nDisplayModal == PA_TOFD_CSCAN) {
        for (i=0; i<16; i++) {
            if (pp->draw_area[i].group == grp) {
                switch (pp->draw_area[i].scan_type)
                {
                    case B_SCAN:
                    case B_SCAN_VERT:
                    case TOFD_B_SCAN:
                        countB++;
                        break;
                    case S_SCAN:
                    case S_SCAN_A:
                    case S_SCAN_L:
                        countS++;
                        break;
                    case C_SCAN:
                    case C_SCAN_VE:
                        countC++;
                        break;
                    default:
                        break;
                }
            }
        }
        if (countB>0) {
            set_display_pos(pp->p_config, A_B_SCAN);
        } else if (countS>0) {
            set_display_pos(pp->p_config, A_S_CC_SCAN);
        } else if (countC>0) {
            set_display_pos(pp->p_config, A_C_CC_SCAN);
        }
    }
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	draw_area_all();
	if(!pp->ScanStart)
	{
		RefreshScanInfor() ;
	}
	pp->bRefreshDraw = TRUE ;
	/*	gtk_widget_queue_draw (pp->vboxtable);*/
}

void data_4011 (GtkMenuItem *menuitem, gpointer data) /* Display->Selection->C Scan source p401 */
{
	unsigned char temp_value ;
	temp_value = (unsigned char) (GPOINTER_TO_UINT (data)) ;
	if(temp_value == get_cscan_source(pp->p_config , 0))  return ;
	set_cscan_source (pp->p_config, temp_value, 0);

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	draw_area_all();
	pp->bRefreshDraw = TRUE ;

}

void data_4012 (GtkMenuItem *menuitem, gpointer data) /* ASC显示模式后时候Cscan的source P401 */
{
	unsigned char temp_value ;
	temp_value = (unsigned char) (GPOINTER_TO_UINT (data)) ;
	if(temp_value == get_cscan_source(pp->p_config , 0))  return ;

	set_cscan_source (pp->p_config, temp_value, 0);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	draw_area_all();
	pp->bRefreshDraw = TRUE ;

}

void data_4013 (GtkMenuItem *menuitem, gpointer data) /* Display->Selection->Data1 p401 */
{
	set_stripscan_data1 (pp->p_config, (unsigned char) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_402 (GtkMenuItem *menuitem, gpointer data) /* Display->Selection->C-Scan2 p402 */
{
	set_cscan_source (pp->p_config, (unsigned char) (GPOINTER_TO_UINT (data)), 1);

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	draw_area_all();
	pp->bRefreshDraw = TRUE ;
}

void data_4021 (GtkMenuItem *menuitem, gpointer data) /* Display->Selection->C-Scan2 p402 */
{
	set_stripscan_data2 (pp->p_config, (unsigned char) (GPOINTER_TO_UINT (data)));

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	draw_area_all();
	pp->bRefreshDraw = TRUE ;
}

void data_403 (GtkMenuItem *menuitem, gpointer data) /* Display->Selection->Mode p403 */
{
	set_stripscan_mode (pp->p_config, (unsigned char) (GPOINTER_TO_UINT (data)));

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	draw_area_all();
	pp->bRefreshDraw = TRUE ;
}
//已经没有 STRIPCHART 啦
void data_404 (GtkSpinButton *spinbutton, gpointer data) /*Display Range P404 */
{
	set_stripscan_disrange (pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton)*100.0));

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	draw_area_all();
	pp->bRefreshDraw = TRUE ;
}

void data_405 (GtkSpinButton *spinbutton, gpointer data) /*Avg.Scan Speed P405 */
{
}

void data_410 (GtkMenuItem *menuitem, gpointer data) /* Display->Overlay->UT Unit p410 */
{
	int temp_value ;
	int grp = get_current_group (pp->p_config) ;
	temp_value = (unsigned char) (GPOINTER_TO_UINT (data)) ;
	if(temp_value == GROUP_VAL_POS(grp , ut_unit)) return ;
	GROUP_VAL_POS(grp , ut_unit) = temp_value;

	UpdateGateForSpiSending(grp) ;
	send_focal_spi (grp, SPI_RESET_NO);

	draw_field_name () ;

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	draw_area_all();
	pp->bRefreshDraw = TRUE ;

}

void data_411 (GtkMenuItem *menuitem, gpointer data) /* 选择栅格颜色  P411 */
{
	set_overlay_grid (pp->p_config,(unsigned char) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	gtk_widget_queue_draw (pp->vboxtable);
	draw_menu3(0, NULL);
}

void data_420 (GtkMenuItem *menuitem, gpointer data) /* Display->Zoom->Display p420 */
{
}

void data_421 (GtkMenuItem *menuitem, gpointer data) /* Display->Zoom->Type p421 */
{
}

void data_422 (GtkSpinButton *spinbutton, gpointer data) /* start USound p422 */
{
}

void data_4221 (GtkSpinButton *spinbutton, gpointer data) /* start USound p422 */
{
}

void data_423 (GtkSpinButton *spinbutton, gpointer data) /* end USound p423 */
{
	draw_dialog_all (DIALOG_COLOR_PALETTE);
}

void data_4231 (GtkSpinButton *spinbutton, gpointer data) /* range USound p423 */
{
}

void data_424 (GtkSpinButton *spinbutton, gpointer data) /* start Amplitude p424 */
{
}

void data_4241 (GtkSpinButton *spinbutton, gpointer data) /* center Amplitude p424 */
{
}

void data_425 (GtkSpinButton *spinbutton, gpointer data) /* end Amplitude p425 */
{
}

void data_4251 (GtkSpinButton *spinbutton, gpointer data) /* range Amplitude p4251 */
{
}

void data_430 (GtkMenuItem *menuitem, gpointer data) /* Display->Color->select p430 */
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , col_select_pos) = (unsigned char) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;

	draw_menu3(0, NULL);
}

void data_431 (GtkSpinButton *spinbutton, gpointer data) /*color_start p431 */
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , col_start) =  (unsigned char) (gtk_spin_button_get_value (spinbutton));
	pp->bRefreshDraw  = TRUE  ;
}

void data_4311 (GtkSpinButton *spinbutton, gpointer data) /*color_contrast */
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , col_contrast) =  (unsigned char) (gtk_spin_button_get_value (spinbutton));
}
void data_4312 (GtkSpinButton *spinbutton, gpointer data) /*color_min */
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , col_min) =  (guint) ((gtk_spin_button_get_value (spinbutton)) * 1000.0);
}

void data_432 (GtkSpinButton *spinbutton, gpointer data) /*color_end */
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , col_end) =  (unsigned char) (gtk_spin_button_get_value (spinbutton));
}

void data_4321 (GtkSpinButton *spinbutton, gpointer data) /*color_brightness */
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , col_brightness) =  (unsigned char) (gtk_spin_button_get_value (spinbutton));
}
void data_4322 (GtkSpinButton *spinbutton, gpointer data) /*color_max */
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , col_max) =  (guint) ((gtk_spin_button_get_value (spinbutton)) * 1000.0);
}

void data_434 (GtkMenuItem *menuitem, gpointer data) /* Display->color->mode 434 */
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , col_mode) = (unsigned char) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_440 (GtkMenuItem *menuitem, gpointer data) /* Display->Properties->scan 440 */
{
	set_dis_prop_scan (pp->p_config, (unsigned char) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_441 (GtkMenuItem *menuitem, gpointer data) /* Display->Properties->Color 441 */
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , ascan_color) = (unsigned char) (GPOINTER_TO_UINT (data));
	pp->pos_pos = MENU3_STOP;
	ascanClearDisp(grp);
	draw_menu3(0, NULL);
}

void data_4411 (GtkSpinButton *spinbutton, gpointer data) /* Display->Properties->Compress 4411 */
{
	set_b_compress (pp->p_config, (guint) ((gtk_spin_button_get_value (spinbutton)) * 1000.0));
}

void data_4414 (GtkMenuItem *menuitem, gpointer data) /* Display->Properties->Color P4414 */
{
	set_fft_color (pp->p_config, (unsigned char) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_4415 (GtkMenuItem *menuitem, gpointer data) /* Display->Properties->Color P4415 */
{
	set_dis_prop_strip_orientation (pp->p_config, (unsigned char) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_442 (GtkMenuItem *menuitem, gpointer data) /* Display->Properties->Envelope 442 */
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , ascan_envelope) = (unsigned char) (GPOINTER_TO_UINT (data));
	if(GROUP_VAL_POS(grp , ascan_envelope))
	{
		ascanResetEnvelope(grp);
	}
	ascanClearDisp(grp);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

//void data_443 (GtkMenuItem *menuitem, gpointer data) /* Display->Properties->Source 443 */
//{
//	printf("443\n");
//	int grp = get_current_group (pp->p_config) ;
//	GROUP_VAL_POS(grp , ascan_source) = (unsigned char) (GPOINTER_TO_UINT (data));
//	pp->pos_pos = MENU3_STOP;
//	draw_menu3(0, NULL);
//}

void data_444 (GtkMenuItem *menuitem, gpointer data) /* Display->Properties->Appearance 444 */
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , ascan_appearance) = (unsigned char) (GPOINTER_TO_UINT (data));

	ascanClearDisp(grp);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_445 (GtkMenuItem *menuitem, gpointer data) /* Display->Properties->Overlay 445 */
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , ascan_overlay) = (unsigned char) (GPOINTER_TO_UINT (data));

	if(2 == GPOINTER_TO_UINT (data))//REF
	{
		ascanResetRef(grp);
	}
	ascanClearDisp(grp);

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_500 (GtkMenuItem *menuitem, gpointer data) /* 增加删除选择group P500 */
{
	unsigned char temp = (unsigned char)(GPOINTER_TO_UINT (data));
	int  grp = get_current_group(pp->p_config);
	if(grp == (temp - 1))
	{
		return ;
	}
	switch (temp)
	{
		case 0:
			MultiGroupAdd() ;
			break;						/* 增加*/
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
			/* 把参数切换到当前选择的group 未完成 */
			pp->p_config->groupId = temp - 1  ;
			break;
		case 9:
			MultiGroupDelete() ;
			break;
		default:break;
	}

	request_refresh(REFRESH_MULTI_GROUP_ACTION) ;
}

void data_501 (GtkMenuItem *menuitem, gpointer data) /* Probe/Part->Select->Group Mode 501 */
{
	char temp_value ;
	int group = get_current_group(pp->p_config);

#if FPGA_VERSION > 1
    gshort gain = group_get_gain(group);
    gshort refgain = group_get_refgain(group);
#endif

	temp_value = (char) (GPOINTER_TO_UINT (data));
#if HIGH_POWER
#else
	if(temp_value == GROUP_VAL_POS (group, group_mode)) {
		return;
	}
	int response = dialogWarning(pp->window , getDictString(_STRING_GROUP_MODE_CHANGE));
	if(!response)
	{
		return ;
	}
#endif
	/*group mode 选择UT,UT1,UT2时，focal law 不可用*/
	GROUP_VAL_POS(group ,  group_mode) = temp_value;

#if FPGA_VERSION > 1
    /*更新增益值*/
    group_set_gain(group, gain);
    group_set_refgain(group, refgain);
#endif

	UT_group_config_settting (group) ;
	if(temp_value == PA_SCAN)
	{
		//UT_group_config_settting (group) ;
		gtk_widget_set_sensitive(pp->menuitem[6], TRUE);
	}
	else// if(temp_value == UT1_SCAN)
	{
		//output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_TWIN_UT ,0 ,0);
		gtk_widget_set_sensitive(pp->menuitem[6], FALSE);
		//UT_group_config_settting (group) ;
	}
	//*******************************************************************
	// 星号内代码为临时的，当前UT 双晶控制协议存在问题，
    int _nTmpModel = group_get_rx_tx_mode(group);
	if (_nTmpModel == PULSE_ECHO )
	{
		if(GROUP_VAL_POS(group , group_mode) == UT1_SCAN)
		{
			output_set_parameter(0 , OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT1 ,1 ,0);//双晶关
			output_write_one_reg_to_spi(0 ,OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT1);
		}
		if(GROUP_VAL_POS(group , group_mode) == UT2_SCAN)
		{
			output_set_parameter(0 , OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT2 ,1 ,0);//双晶关
			output_write_one_reg_to_spi(0 ,OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT1);
		}
		//output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_TWIN_SWITCH ,0 ,0);
	}
	else
	{
		if(GROUP_VAL_POS(group , group_mode) == UT1_SCAN)
		{
			output_set_parameter(0 , OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT1 ,0 ,0);//双晶关
			output_write_one_reg_to_spi(0 ,OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT1);
		}
		if(GROUP_VAL_POS(group , group_mode) == UT2_SCAN)
		{
			output_set_parameter(0 , OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT2 ,0 ,0);//双晶关
			output_write_one_reg_to_spi(0 ,OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT1);
		}
		//output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_TWIN_SWITCH ,1 ,0);
	}
	//output_write_one_reg_to_spi(0 ,OUTPUT_OTHER_COMMAND_TWIN_SWITCH);
	//*******************************************************************
	GROUP_VAL_POS(group , WedgeDelayCalibrated) = 0 ;
	GROUP_VAL_POS(group , SensationCalibrated) =  0  ;
	generate_focallaw (group);
	RefreshBeamDelay(group);
	RefreshGainOffset(group);
	InitSizingCurveParameters(group) ;
	MultiGroupSendGroupSpi();
	MultiGroupSendFocalSpi();

	//*******************************************************************
	// 星号内代码为临时的，当前UT 双晶控制协议存在问题，
	if (_nTmpModel == PULSE_ECHO )
	{
		if(GROUP_VAL_POS(group , group_mode) == UT1_SCAN)
		{
			output_set_parameter(0 , OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT1 ,0 ,0);//双晶关
			output_write_one_reg_to_spi(0 ,OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT1);
		}
		if(GROUP_VAL_POS(group , group_mode) == UT2_SCAN)
		{
			output_set_parameter(0 , OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT2 ,0 ,0);//双晶关
			output_write_one_reg_to_spi(0 ,OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT1);
		}
		//output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_TWIN_SWITCH ,0 ,0);
	}
	else
	{
		if(GROUP_VAL_POS(group , group_mode) == UT1_SCAN)
		{
			output_set_parameter(0 , OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT1 ,1 ,0);//双晶关
			output_write_one_reg_to_spi(0 ,OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT1);
		}
		if(GROUP_VAL_POS(group , group_mode) == UT2_SCAN)
		{
			output_set_parameter(0 , OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT2 ,1 ,0);//双晶关
			output_write_one_reg_to_spi(0 ,OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT1);
		}
		//output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_TWIN_SWITCH ,1 ,0);
	}
	//*******************************************************************
	gtk_widget_queue_draw (pp->status_area);
	// redraw menu
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	//request_refresh(REFRESH_DRAW_AREA_ALL);
	draw_area_all();
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;
}

void data_502 (GtkMenuItem *menuitem, gpointer data) /* Probe/Part->Select->Select 502 */
{
	set_probe_select (pp->p_config, (gboolean) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_505(GtkMenuItem *menuitem, gpointer data)
{
	set_auto_detect (pp->p_config, !get_auto_detect(pp->p_config));
	if(get_auto_detect (pp->p_config))
	{
		RequestProbeAutoDetect();
	}
}

void data_510 (GtkSpinButton *spinbutton, gpointer data) /*scanoffset */
{
	int _nTmpValue ;
	int grp = get_current_group (pp->p_config) ;
	if(get_unit(pp->p_config) == UNIT_MM)
		_nTmpValue =  (int) (gtk_spin_button_get_value (spinbutton) * 10.0);
	else
		_nTmpValue =  (int) (gtk_spin_button_get_value (spinbutton) * 10.0 / 0.03937);

	if(_nTmpValue == GROUP_VAL_POS(grp , scan_offset))   return ;
	GROUP_VAL_POS(grp , scan_offset) = _nTmpValue ;
	request_refresh(REFRESH_PART_POSITION) ;
}

void data_511 (GtkSpinButton *spinbutton, gpointer data)
{
	int _nTmpValue ;
	int grp = get_current_group (pp->p_config) ;
	if(get_unit(pp->p_config) == UNIT_MM)
		_nTmpValue =  (int) (gtk_spin_button_get_value (spinbutton) * 10.0);
	else
		_nTmpValue =  (int) (gtk_spin_button_get_value (spinbutton) * 10.0 / 0.03937);
	if(_nTmpValue == GROUP_VAL_POS(grp , index_offset))   return ;
	GROUP_VAL_POS(grp , index_offset) = _nTmpValue  ;
	request_refresh(REFRESH_PART_POSITION) ;
}


void data_512 (GtkMenuItem *menuitem, gpointer data) /* Skew (deg) */
{
	gushort temp = GPOINTER_TO_UINT (data);
	int grp = get_current_group(pp->p_config);

	GROUP_VAL_POS(grp , skew_pos) = (unsigned char) (GPOINTER_TO_UINT (data));
	GROUP_VAL_POS(grp , skew) = get_skew(grp);
	if (temp != 4)
	{
		pp->pos_pos = MENU3_STOP;
		draw_menu3(0, NULL);
		gtk_spin_button_set_value (GTK_SPIN_BUTTON (pp->sbutton[2]), GROUP_VAL_POS(grp , skew)/100.0);
		request_refresh(REFRESH_DRAW_AREA_ALL);
		//RefreshScanInfor() ;
		//draw_area_all();
		//pp->bRefreshDraw = TRUE ;
	}
	else
	{
		pp->mark_pop_change = 1;
		pp->pos_pos = MENU3_PRESSED;
		draw_menu3(0, NULL);
		tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[2]));
	}
}

void data_520()
{
	int group = get_current_group(pp->p_config);
//	if(get_probe_fft(pp->p_config))
	if(gData->fft[group].enableFft)
	{
//		set_probe_fft (pp->p_config, 0);
		gData->fft[group].enableFft = 0;
		//request_refresh(REFRESH_DRAW_AREA_ALL);

		draw_area_all();
		RefreshScanInfor() ;
		pp->bRefreshDraw = TRUE ;

		//pp->bRefreshDraw = TRUE ;
		pp->pos_pos = MENU3_STOP;
		draw_menu3(0, NULL);
	}
	else
	{
		//add by lzk
/*		if(GROUP_VAL_POS(group ,point_qty) * 10 == get_group_val (get_group_by_id (pp->p_config, group), GROUP_RANGE))
		{
			gData->fft[group].fFtValid = 1;
		}
		else
		{
			gData->fft[group].fFtValid = 0;
		}//*/
		if( (RF_WAVE == GROUP_VAL_POS(group , rectifier1))//RF模式
			&&	 1 == get_group_val (get_group_by_id (pp->p_config, group), GROUP_RANGE) / 10
					/ GROUP_VAL_POS(group ,point_qty))//factor == 1
//			&&	(0 != gData->fft[group].fFtValid)	)
		{
			//set_probe_fft (pp->p_config, 1);
			gData->fft[group].enableFft = 1;
			request_refresh(REFRESH_DRAW_AREA_ALL);
			//draw_area_all();
			//RefreshScanInfor() ;
			//pp->bRefreshDraw = TRUE ;
			pp->pos_pos = MENU3_STOP;
			draw_menu3(0, NULL);
		}
		else
		{
			dialogError(pp->window , getDictString(_STRING_Range_is_too_large_nOr_Not_RF_Mode));
		}
	}
}

void data_521 (GtkSpinButton *spinbutton, gpointer data) /*gain */
{
	int grp = get_current_group(pp->p_config);
    group_set_start(grp, (gshort) (gtk_spin_button_get_value (spinbutton) * 10.0));
}

void data_522 (GtkSpinButton *spinbutton, gpointer data) /*agate_start */
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , agate_start) =  (int) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}

void data_523 (GtkSpinButton *spinbutton, gpointer data) /*agate_width */
{
	int grp = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(grp , agate_width) =  (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0);
}

void data_530 (GtkMenuItem *menuitem, gpointer data) /* Probe/Part->Parts->Geometry 530 */
{
	int grp = get_current_group(pp->p_config);
    int val;
    if (get_part_geometry(grp) != (unsigned char)((unsigned int) (data))) {
	    set_part_geometry (grp, data);
        if ((unsigned char)((unsigned int) (data)) != 0) {
            val = get_part_thickness(grp) * 2;
		    if (val < 80000) {
                val = 80000;
            }
            set_part_diameter(grp, (GUINT_TO_POINTER(val)));
            LAW_VAL_POS(grp , Position_start) = get_part_thickness(grp);
        }
        ResolutionConflict(0);
	    request_refresh(REFRESH_FOCALLAW_TYPE_MARK) ;
    }
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_531 (GtkSpinButton *spinbutton, gpointer data) /*part_thickness*/
{
	int grp = get_current_group(pp->p_config);
	if(UNIT_MM == get_unit(pp->p_config))
		set_part_thickness (grp , (GUINT_TO_POINTER
					((unsigned int) (gtk_spin_button_get_value (spinbutton) * 1000.0))));
	else
		set_part_thickness (grp, (GUINT_TO_POINTER
					((unsigned int) (gtk_spin_button_get_value (spinbutton) * 1000.0/ 0.03937 ))));
	gtk_widget_queue_draw (pp->vboxtable);
}

void data_532 (GtkSpinButton *spinbutton, gpointer data) /*part_thickness*/
{
	int grp = get_current_group(pp->p_config);
    unsigned int valueNew, valueOld = get_part_diameter(grp);
	if(UNIT_MM == get_unit(pp->p_config)) {
        valueNew = (guint)(gtk_spin_button_get_value(spinbutton) * 1000.0);
    } else {
        valueNew = (guint)(gtk_spin_button_get_value(spinbutton) * 1000.0/ 0.03937 );
    }
    if (valueNew != valueOld) {
        set_part_diameter(grp, (GUINT_TO_POINTER(valueNew)));
        ResolutionConflict(0);
	    request_refresh(REFRESH_FOCALLAW_TYPE_MARK) ;
    }
}

/* P533 选择被测物的材料 */
void data_533 (GtkMenuItem *menuitem, gpointer data) 
{
	int _nTmpValue = (int)data  ;
	int grp = get_current_group(pp->p_config);
	if(_nTmpValue == get_part_material(grp))
	{
		return ;
	}

	set_part_material (grp , data);
	GROUP_VAL_POS(grp , velocity) =  get_material_lw (_nTmpValue) ;

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	request_refresh(REFRESH_VELOCITY_MARK) ;
}

void data_534 (GtkMenuItem *menuitem, gpointer data) /* Probe/Part -> Parts -> Weld 534 */
{
	//printf("data 534 !\n");
	int grp = get_current_group(pp->p_config) ;
	set_part_weld (grp , data);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}
/* 聚焦法则类型 P600 */
void data_600 (GtkMenuItem *menuitem, gpointer data) 
{
	unsigned char _nTmpValue ;
	_nTmpValue =  (unsigned char) (GPOINTER_TO_UINT (data)) ;
	int grp = get_current_group (pp->p_config) ;
	if(LAW_VAL_POS(grp , Focal_type) == _nTmpValue )  return ;
	if(_nTmpValue > 1)
	{
		g_debug("not availabel!\n");
		return ;
	}
	LAW_VAL_POS(grp , Focal_type) = (unsigned char) (GPOINTER_TO_UINT (data)) ;

	if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
	{
		LAW_VAL_POS(grp , Angle_min) = 0 ;
		LAW_VAL_POS(grp , Angle_max) = 0 ;
	}

	if (LAW_VAL_POS(grp , Focal_type) != LINEAR_SCAN)
	{
		LAW_VAL_POS(grp , Last_tx_elem) = (unsigned char) (LAW_VAL_POS(grp , First_tx_elem) + LAW_VAL_POS(grp , Elem_qty)) - 1;
	}

	if ((LAW_VAL_POS(grp , Focal_type) == LINEAR_SCAN) || (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN))
	{   // avoid law qty bigger than 256
		LAW_VAL_POS(grp , Angle_min) = 0 ;
		LAW_VAL_POS(grp , Angle_max) = 0 ;
	}

	if (LAW_VAL_POS(grp , Focal_type) == DEPTH_SCAN)
	{
		LAW_VAL_POS(grp , Focal_point_type) = DEPTH_P;
	}

	if (LAW_VAL_POS(grp , Focal_type) == STATIC_SCAN)
	{
		LAW_VAL_POS(grp , Focal_point_type) = DEPTH_P;
	}
	request_refresh(REFRESH_FOCALLAW_TYPE_MARK) ;
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

/* min_angle P610 */
void data_610 (GtkSpinButton *spinbutton, gpointer data)
{
	short temp_value ;
	int beam_num = 0; //
	int grp = get_current_group (pp->p_config) ;
	temp_value = (int)(gtk_spin_button_get_value (spinbutton) * 100.0) ;

	if(gtk_widget_get_sensitive(pp->eventbox30[1]))
	{
		if(temp_value >= 0)
			temp_value = ((temp_value + 5) / 10) * 10 ;
		else temp_value = ((temp_value - 5) / 10) * 10 ;
		beam_num = (int)(( LAW_VAL_POS(grp , Angle_max) - temp_value) / LAW_VAL_POS(grp , Angle_step)) ;
		temp_value = LAW_VAL_POS(grp , Angle_max) - beam_num * LAW_VAL_POS(grp , Angle_step) ;
	}

	if(LAW_VAL_POS(grp , Angle_min) == temp_value ) return ;
	if(beam_num >= GetCurrentGroupLawsQtyLimit(grp))     return ;

	request_refresh(REFRESH_ANGLE_MARK) ;
	LAW_VAL_POS(grp , Angle_min) = temp_value ;
	if(temp_value > LAW_VAL_POS(grp , Angle_max))
	{
		LAW_VAL_POS(grp , Angle_max) = temp_value ;
		draw_menu3(0, NULL);
	}
}

/* max_angle P611 */
void data_611 (GtkSpinButton *spinbutton, gpointer data) 
{
	short temp_value ;
	int grp = get_current_group (pp->p_config) ;
	temp_value =  (short)(gtk_spin_button_get_value (spinbutton) * 100.0);
	int beam_num ;

	if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
	{
		if(temp_value >= 0)
			temp_value = ((temp_value + 5) / 10) * 10 ;
		else temp_value = ((temp_value - 5) / 10) * 10 ;
		beam_num = (int)(( temp_value - LAW_VAL_POS(grp , Angle_min)) / LAW_VAL_POS(grp , Angle_step)) ;
		temp_value = LAW_VAL_POS(grp , Angle_min) + beam_num * LAW_VAL_POS(grp , Angle_step) ;

		if(LAW_VAL_POS(grp , Angle_max) == temp_value )   return ;
		if(beam_num >= GetCurrentGroupLawsQtyLimit(grp))     return ;

		request_refresh(REFRESH_ANGLE_MARK) ;
		LAW_VAL_POS(grp , Angle_max) = temp_value ;
		if(temp_value < LAW_VAL_POS(grp , Angle_min))
		{
			LAW_VAL_POS(grp , Angle_min)  = temp_value  ;
			draw_menu3(0, NULL);
		}
	}

}
/* Angle Step P612 */
void data_612 (GtkSpinButton *spinbutton, gpointer data) 
{
	short temp_value ;
	int beam_no ;
	int grp = get_current_group (pp->p_config) ;

	if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
	{
		temp_value = (gushort) (gtk_spin_button_get_value (spinbutton) * 100.0);
		if(temp_value >= 0)
			temp_value = ((temp_value + 5) / 10) * 10 ;
		else temp_value = ((temp_value - 5) / 10) * 10 ;
		if(LAW_VAL_POS(grp , Angle_step) == temp_value)  return ;
		beam_no = (int)((LAW_VAL_POS(grp , Angle_max) - LAW_VAL_POS(grp , Angle_min)) / temp_value);
		if(beam_no >= GetCurrentGroupLawsQtyLimit(grp))     return ;
		request_refresh(REFRESH_ANGLE_MARK_STEP) ;
		LAW_VAL_POS(grp , Angle_step) = temp_value ;
		//LAW_VAL_POS(grp , Angle_max) = LAW_VAL_POS(grp , Angle_min) + beam_no * temp_value ;
	}
}


/* beam skew min 2D 的偏斜角min P613 */
void data_613 (GtkSpinButton *spinbutton, gpointer data) 
{
	int grp = get_current_group (pp->p_config);
	LAW_VAL_POS(grp , Angle_beam_skew_min) = (gshort) (gtk_spin_button_get_value (spinbutton) * 100.0);
}

/* beam skew max 2D 的偏斜角min P614 */
void data_614 (GtkSpinButton *spinbutton, gpointer data) 
{
	int grp = get_current_group (pp->p_config);
	LAW_VAL_POS(grp , Angle_beam_skew_max) = (gshort) (gtk_spin_button_get_value (spinbutton) * 100.0);
}

/* focalpoint 聚焦点计算方法 P620 */
void data_620 (GtkMenuItem *menuitem, gpointer data) 
{
	int grp = get_current_group (pp->p_config) ;
	if(LAW_VAL_POS(grp , Focal_point_type) == (unsigned char) (GPOINTER_TO_UINT (data)))  return ;
	LAW_VAL_POS(grp , Focal_point_type) = (unsigned char) (GPOINTER_TO_UINT (data)) ;

	GROUP_VAL_POS(grp , WedgeDelayCalibrated) = 0 ;
    generate_focallaw (grp);
	RefreshBeamDelay(grp);

	draw_area_all();
	gtk_widget_queue_draw (pp->status_area) ;
	gtk_widget_queue_draw (pp->drawing_area) ;
	//gdk_threads_leave();
	MultiGroupSendGroupSpi();
	MultiGroupSendFocalSpi();
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_621 (GtkSpinButton *spinbutton, gpointer data) /* Position start P621 */
{
	int grp = get_current_group (pp->p_config) ;
	float multiplier = 1.0;
	if(UNIT_MM == get_unit(pp->p_config))
	{
		multiplier = 1.0;
	}else{
		multiplier = 0.03937;
	}

	if(LAW_VAL_POS(grp , Position_start) == (int) (gtk_spin_button_get_value (spinbutton) * 1000.0 / multiplier)) return ;
	request_refresh(REFRESH_FOCAL_DEPTH_MARK) ;
	LAW_VAL_POS(grp , Position_start) = (int) (gtk_spin_button_get_value (spinbutton) * 1000.0 / multiplier) ;
}

void data_6211 (GtkSpinButton *spinbutton, gpointer data) /* Offset start P621 */
{
	int grp = get_current_group (pp->p_config) ;
	float multiplier = 1.0;
	if(UNIT_MM == get_unit(pp->p_config))
	{
		multiplier = 1.0;
	}else{
		multiplier = 0.03937;
	}

	if(LAW_VAL_POS(grp , Offset_start) == (int) (gtk_spin_button_get_value (spinbutton)* 1000.0 / multiplier)) return ;
	request_refresh(REFRESH_FOCAL_DEPTH_MARK) ;
	LAW_VAL_POS(grp , Offset_start) = (int) (gtk_spin_button_get_value (spinbutton)* 1000.0 / multiplier);
}

void data_622 (GtkSpinButton *spinbutton, gpointer data) /* Offset end  P622 */
{
	int grp = get_current_group (pp->p_config);
	float multiplier = 1.0;
	if(UNIT_MM == get_unit(pp->p_config))
	{
		multiplier = 1.0;
	}else{
		multiplier = 0.03937;
	}

	if(LAW_VAL_POS(grp , Position_end) == (int) (gtk_spin_button_get_value (spinbutton)* 1000.0 / multiplier)) return ;
	request_refresh(REFRESH_FOCAL_DEPTH_MARK) ;
	LAW_VAL_POS(grp , Position_end) = (int) (gtk_spin_button_get_value (spinbutton)* 1000.0 / multiplier);
}

void data_6221 (GtkSpinButton *spinbutton, gpointer data) /*Offset step P622 */
{
	int grp = get_current_group (pp->p_config);
	float multiplier = 1.0;
	if(UNIT_MM == get_unit(pp->p_config))
	{
		multiplier = 1.0;
	}else{
		multiplier = 0.03937;
	}

	if(LAW_VAL_POS(grp , Offset_end) == (int) (gtk_spin_button_get_value (spinbutton)* 1000.0 / multiplier)) return ;
	request_refresh(REFRESH_FOCAL_DEPTH_MARK) ;
	LAW_VAL_POS(grp , Offset_end) = (int) (gtk_spin_button_get_value (spinbutton)* 1000.0 / multiplier);
}

void data_623 (GtkSpinButton *spinbutton, gpointer data) /* Position Step P623 */
{
	int grp = get_current_group (pp->p_config) ;
	float multiplier = 1.0;
	if(UNIT_MM == get_unit(pp->p_config))
	{
		multiplier = 1.0;
	}else{
		multiplier = 0.03937;
	}

	if(LAW_VAL_POS(grp , Position_step) ==  (int) (gtk_spin_button_get_value (spinbutton) * 1000.0 / multiplier))
		return ;
	request_refresh(REFRESH_FOCAL_DEPTH_MARK) ;
	LAW_VAL_POS(grp , Position_step) =  (int) (gtk_spin_button_get_value (spinbutton) * 1000.0 / multiplier) ;
}
void data_6231 (GtkSpinButton *spinbutton, gpointer data) /* Position Step P623 */
{
	int grp = get_current_group (pp->p_config);
	float multiplier = 1.0;
	if(UNIT_MM == get_unit(pp->p_config))
	{
		multiplier = 1.0;
	}else{
		multiplier = 0.03937;
	}

	if(LAW_VAL_POS(grp , Depth_start) ==  (int) (gtk_spin_button_get_value (spinbutton) * 1000.0 / multiplier))
		return ;
	request_refresh(REFRESH_FOCAL_DEPTH_MARK) ;
	LAW_VAL_POS(grp , Depth_start) =  (int) (gtk_spin_button_get_value (spinbutton) * 1000.0 / multiplier);
}

void data_624 (GtkSpinButton *spinbutton, gpointer data) /* Position Step P624 */
{
	int grp = get_current_group (pp->p_config);
	float multiplier = 1.0;
	if(UNIT_MM == get_unit(pp->p_config))
	{
		multiplier = 1.0;
	}else{
		multiplier = 0.03937;
	}

	if(LAW_VAL_POS(grp , Depth_end) ==  (int) (gtk_spin_button_get_value (spinbutton) * 1000.0 / multiplier))
		return ;
	request_refresh(REFRESH_FOCAL_DEPTH_MARK) ;
	LAW_VAL_POS(grp , Depth_end) =  (int) (gtk_spin_button_get_value (spinbutton) * 1000.0 / multiplier);
}

//	GROUP_VAL_POS(grp , pulser1)  = _nTmpValue ;
//	GROUP_VAL_POS(grp , receiver1)  = _nTmpValue ;

/* element_qty 聚集 法则一次激发的阵元数量 P620 */
void data_630 (GtkSpinButton *spinbutton, gpointer data) 
{
	int	grp	= get_current_group(pp->p_config);
	unsigned char _nTmpValue ;
	_nTmpValue = (unsigned char) (gtk_spin_button_get_value (spinbutton)) ;
	if( LAW_VAL_POS(grp , Elem_qty) == _nTmpValue )  return ;
	LAW_VAL_POS(grp , Elem_qty) = _nTmpValue;
	request_refresh(REFRESH_ELEMENT_MARK_QTY) ;
}

/* first_element 第一个接收阵元 */
void data_631 (GtkSpinButton *spinbutton, gpointer data) 
{
	int grp = get_current_group (pp->p_config) ;
	if(LAW_VAL_POS(grp , First_tx_elem) ==  (unsigned char) (gtk_spin_button_get_value (spinbutton)))
		return ;


	LAW_VAL_POS(grp , First_tx_elem) =  (unsigned char) (gtk_spin_button_get_value (spinbutton));

	if(LAW_VAL_POS(grp , Focal_type) == 0)//Azimuthal
		LAW_VAL_POS(grp , Last_tx_elem) = (unsigned char) (LAW_VAL_POS(grp , First_tx_elem) + LAW_VAL_POS(grp , Elem_qty)) - 1;
	else//Linear
	{
		if( LAW_VAL_POS(grp , Last_tx_elem) < ((unsigned char) (LAW_VAL_POS(grp , First_tx_elem) + LAW_VAL_POS(grp , Elem_qty)) - 1) )
			LAW_VAL_POS(grp , Last_tx_elem) = (unsigned char) (LAW_VAL_POS(grp , First_tx_elem) + LAW_VAL_POS(grp , Elem_qty)) - 1;
	}
    ResolutionConflict(1);
	request_refresh(REFRESH_ELEMENT_MARK_START) ;

}
/* last_element 最后一个阵元编号 */
void data_632 (GtkSpinButton *spinbutton, gpointer data) 
{
	int temp_value ;
	int grp = get_current_group (pp->p_config) ;
	if( LAW_VAL_POS(grp , Last_tx_elem) < ((unsigned char) (LAW_VAL_POS(grp , First_tx_elem) + LAW_VAL_POS(grp , Elem_qty)) - 1) )
		temp_value = (unsigned char) (LAW_VAL_POS(grp , First_tx_elem) + LAW_VAL_POS(grp , Elem_qty)) - 1;
	else
		temp_value =  (unsigned char) (gtk_spin_button_get_value (spinbutton));

	temp_value = LAW_VAL_POS(grp , Elem_step) * ((temp_value + 1 - LAW_VAL_POS(grp , Elem_qty)-
				LAW_VAL_POS(grp , First_tx_elem))/LAW_VAL_POS(grp , Elem_step)) + LAW_VAL_POS(grp , First_tx_elem) + LAW_VAL_POS(grp , Elem_qty) - 1;

	if(temp_value == LAW_VAL_POS(grp , Last_tx_elem))
		return ;

	LAW_VAL_POS(grp , Last_tx_elem) = temp_value ;
	request_refresh(REFRESH_ELEMENT_MARK_STOP) ;
}

void data_633 (GtkSpinButton *spinbutton, gpointer data) /*element_step*/
{
	int element_step ;
	int grp = get_current_group (pp->p_config);
	element_step =   (unsigned char) (gtk_spin_button_get_value (spinbutton));
	if(LAW_VAL_POS(grp , Elem_step) ==  element_step) return ;
	request_refresh(REFRESH_ELEMENT_MARK_STEP) ;
	LAW_VAL_POS(grp , Elem_step)  = element_step ;
}

/* velocity type  */
void data_634 (GtkMenuItem *menuitem, gpointer data) 
{
	guint temp = (unsigned char) (GPOINTER_TO_UINT (data));
	int grp = get_current_group (pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	int material = get_part_material (grp);
	if (temp == 0)
		set_group_val (p_grp, GROUP_VELOCITY, get_material_lw (material));
	else if (temp == 1) 
		set_group_val (p_grp, GROUP_VELOCITY, get_material_sw (material));
	request_refresh(REFRESH_VELOCITY_MARK) ;

	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}


void data_643()
{
	//*********  冻结的时候不需要更新
	if(TMP(freeze)) return ;
	/* 计算聚焦法则 P643 */
	int _nGroupId = get_current_group (pp->p_config) ;
	GROUP_VAL_POS(_nGroupId , WedgeDelayCalibrated) = 0 ;
	GROUP_VAL_POS(_nGroupId , SensationCalibrated) =  0  ;
	gtk_widget_queue_draw (pp->status_area);
	generate_focallaw (_nGroupId);
	RefreshBeamDelay(_nGroupId);
	RefreshGainOffset(_nGroupId);
	DrawDisplayWindowFrame() ;
	MultiGroupSendGroupSpi();
	MultiGroupSendFocalSpi();
	RefreshScanInfor() ;
	gtk_widget_queue_draw (pp->status_area);
	pp->bRefreshDraw = TRUE ;
}

void data_700 (GtkMenuItem *menuitem, gpointer data) /* Encoder */
{
	set_cur_encoder (pp->p_config, (unsigned char) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);

}

void data_701 (GtkMenuItem *menuitem, gpointer data) /* Polarity */
{
	int _nTmpValue = (int)data  ;
	unsigned char _nCurrentEcoderType ;
	unsigned char _nPoly ;
	int i = (int)get_inspec_source (pp->p_config) - 1;
	if(i != 0 && i != 1)  return  ;

	_nCurrentEcoderType  = get_enc_type (pp->p_config,  i);
	_nPoly  = ((_nCurrentEcoderType >> 3) & 1)   ;
	if(_nTmpValue == _nPoly)
	{
		return ;
	}
	if(_nTmpValue)
	{
		_nCurrentEcoderType = _nCurrentEcoderType | 0x8  ;
	}
	else
	{
		_nCurrentEcoderType = _nCurrentEcoderType & 0x7  ;
	}
	set_enc_type (pp->p_config, _nCurrentEcoderType , i) ;
	if(i)
	{
		output_set_parameter(0 , OUTPUT_OTHER_COMMAND_ENCODE_Y ,_nCurrentEcoderType ,0);
	}
	else
	{
		output_set_parameter(0 , OUTPUT_OTHER_COMMAND_ENCODE_X ,_nCurrentEcoderType ,0);
	}
	output_write_one_reg_to_spi(0, OUTPUT_OTHER_COMMAND_ENCODE_Y );
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;

}

void data_702 (GtkMenuItem *menuitem, gpointer data) /* Type */
{

	unsigned char _nCurrentEcoderType ;
	int i = (int)get_inspec_source (pp->p_config) - 1;
	if(i != 0 && i != 1)  return  ;
	_nCurrentEcoderType  = get_enc_type (pp->p_config,  i);
	_nCurrentEcoderType  = _nCurrentEcoderType & 0xc ;

	switch( GPOINTER_TO_UINT (data) )
	{
		case 0:
			_nCurrentEcoderType |= 3 ;
			break;
		case 1://up
			_nCurrentEcoderType |= 2 ;
			break;
		case 2://down
			_nCurrentEcoderType |= 1 ;
			break;
		default:
			_nCurrentEcoderType |= 3 ;
			break;
	}

	if(i)//ENC2
	{
		output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_ENCODE_Y ,_nCurrentEcoderType ,0);
	}
	else//ENC1
	{
		output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_ENCODE_X ,_nCurrentEcoderType ,0);
	}
	set_enc_type (pp->p_config, _nCurrentEcoderType , i) ;
	output_write_one_reg_to_spi(0 , OUTPUT_OTHER_COMMAND_ENCODE_Y);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;
}

void data_703 (GtkSpinButton *spinbutton, gpointer data) /* Resolution */
{
	int _nTmpValue ;
	int _nEncNo = get_cur_encoder(pp->p_config);
	_nTmpValue = (unsigned int)(gtk_spin_button_get_value (spinbutton) * 1000.0) ;
	if(_nTmpValue == get_enc_resolution(pp->p_config , _nEncNo))
	{
		return ;
	}
	set_enc_resolution (pp->p_config,  _nTmpValue, _nEncNo );
	// reset scan
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;
}

void data_704 (GtkSpinButton *spinbutton, gpointer data) /* Origin */
{
    int tmp ;
    int encId = get_cur_encoder(pp->p_config);
    tmp = (unsigned int)(gtk_spin_button_get_value (spinbutton) * 1000.0) ;
    if(tmp == get_enc_origin (pp->p_config, encId) ) {
		return ;
	}

    if(get_enc_resolution(pp->p_config , encId)) {
        set_enc_origin(pp->p_config , tmp , encId);
    } else {
        set_enc_origin(pp->p_config , 0 , encId);
	}
    enc_set_preset(pp->p_config, encId, FALSE);
	// reset scan
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;

}

void data_710 (GtkMenuItem *menuitem, gpointer data) /* Scan->Inspection->type */
{
	set_inspec_type (pp->p_config, (unsigned char) (GPOINTER_TO_UINT (data)));

	if( GPOINTER_TO_UINT (data) == 1)
	{
		set_enc_type (pp->p_config, ENC_QUAD, ENCODER_2);
		set_enc_resolution (pp->p_config, 12000,ENCODER_2);
	}
	//********** do not delete
	//if( GPOINTER_TO_UINT (data) == 2)
	//{
	//	if( pp->count == 0)
	//		set_part_geometry (pp->p_config, (void*)(1) );
	//	pp->count++;
	//}
	//*************************
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	draw_area_all();
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;
}

void data_711 (GtkMenuItem *menuitem, gpointer data) /* Scan->Inspection->scan */
{
	unsigned char _nCurrentEcoderType  ;
	unsigned int _nEncodeSource = GPOINTER_TO_UINT (data) ;
	if (_nEncodeSource == get_inspec_source (pp->p_config))
	{
		return ;
	}

	set_inspec_source (pp->p_config, _nEncodeSource);
	if(_nEncodeSource == 0)
	{
		update_widget_bg(pp->event[18], 18);
		output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_ENCODE_X , 0 , 0);
		output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_ENCODE_Y , 0 , 0);
	}
	else if(_nEncodeSource == 1)
	{
		update_widget_bg(pp->event[18], 19);
		 _nCurrentEcoderType  = get_enc_type (pp->p_config,   0);
		output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_ENCODE_X , _nCurrentEcoderType , 0);
	}
	else if(_nEncodeSource == 2)
	{
		update_widget_bg(pp->event[18], 19);
		_nCurrentEcoderType  = get_enc_type (pp->p_config,   1);
		output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_ENCODE_Y , _nCurrentEcoderType , 0);
	}
	output_write_one_reg_to_spi(0,OUTPUT_OTHER_COMMAND_ENCODE_Y);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	DrawDisplayWindowFrame() ;
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;

}

void data_712 (GtkMenuItem *menuitem, gpointer data) /* Scan->Inspection->Index */
{
	unsigned int _nTmpValue = (unsigned int)data ;
	if(_nTmpValue == get_inspec_index(pp->p_config))
	{
		return ;
	}
	set_inspec_index (pp->p_config, (unsigned char)_nTmpValue );
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);

	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;

}

void data_713 (GtkSpinButton *spinbutton, gpointer data) /*scan_speed*/
{
	int _nTmpValue   ;
	int _nScanResolution ;

	if(UNIT_MM == get_unit(pp->p_config))
		_nTmpValue = (int) (gtk_spin_button_get_value (spinbutton) * 1000.0) ;
	else
		_nTmpValue = (int) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937) ;

    if(_nTmpValue == get_inspec_speed(pp->p_config))  return ;

    _nScanResolution = get_area_scanresolution (pp->p_config);
    _nTmpValue = 10 * _nTmpValue /_nScanResolution  ;
	if(_nTmpValue < 10)  _nTmpValue = 10 ;
    GROUP_VAL_POS(0 , prf1) = _nTmpValue  ;
	GROUP_VAL_POS(0 , prf_pos1) = 3 ;
	_nTmpValue /= 10 ;
	set_inspec_speed (pp->p_config,	_nTmpValue * _nScanResolution );

	request_refresh(REFRESH_PRF_CHANGED);
}

void data_714 (GtkSpinButton *spinbutton, gpointer data) /*scan_speed*/
{
	unsigned int _nTmpValue = (unsigned int) (gtk_spin_button_get_value (spinbutton) * 10.0)  ;
	if(_nTmpValue == get_inspec_rpmspeed (pp->p_config))
	{
		return ;
	}
	set_inspec_rpmspeed (pp->p_config, _nTmpValue);
	// reset scan
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;

}

void data_715 (GtkSpinButton *spinbutton, gpointer data) /*index_speed*/
{

	unsigned int _nTmpValue = (unsigned int) (gtk_spin_button_get_value (spinbutton) * 10.0)  ;
	if(_nTmpValue == get_inspec_indexspeed (pp->p_config))
	{
		return ;
	}
	set_inspec_indexspeed (pp->p_config, _nTmpValue);
	// reset scan
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;
}

void data_720 (GtkSpinButton *spinbutton, gpointer data) /* P720 scan_start */
{
	int temp_value ;

	if(UNIT_MM == get_unit(pp->p_config))
		temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 1000.0) ;
	else
		temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937 ) ;

	int _nOldValue = get_area_scanstart (pp->p_config);
	if(temp_value == _nOldValue)
	{
		return ;
	}
	set_area_scanstart (pp->p_config, temp_value );

	if(isScanRangeInvalidate())
	{
		set_area_scanstart (pp->p_config, _nOldValue );
	}
	request_refresh(REFRESH_SCAN_RESOLUTION);

}

void data_721 (GtkSpinButton *spinbutton, gpointer data) /* P721 scan_end*/
{
	int temp_value ;
	if(UNIT_MM == get_unit(pp->p_config))
		temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 1000.0) ;
	else
		temp_value = (int) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937 ) ;

	int _nOldValue = get_area_scanend (pp->p_config) ;
	if(temp_value == _nOldValue)
	{
		return ;
	}

	set_area_scanend (pp->p_config, temp_value );

	if(isScanRangeInvalidate())
	{
		set_area_scanend (pp->p_config, _nOldValue );
	}

	request_refresh(REFRESH_SCAN_RESOLUTION);

}

void data_722 (GtkSpinButton *spinbutton, gpointer data) /* P722 scan_resolution*/
{
	int _nTmpValue ;
	_nTmpValue = gtk_spin_button_get_value (spinbutton) * 1000.0  ;
	if(UNIT_INCH == get_unit(pp->p_config))
		_nTmpValue = _nTmpValue /  0.03937  ;

	int _nOldValue = get_area_scanresolution (pp->p_config) ;
	if(_nTmpValue == _nOldValue)
	{
		return ;
	}
	set_area_scanresolution (pp->p_config, _nTmpValue );
	if(isScanRangeInvalidate())
	{
		set_area_scanresolution (pp->p_config, _nOldValue );
	}

    int _nScanResolution = get_area_scanresolution (pp->p_config);
	set_inspec_speed (pp->p_config,	GROUP_VAL_POS(0 , prf1) * _nScanResolution / 10);

	request_refresh(REFRESH_SCAN_RESOLUTION);
}

void data_723 (GtkSpinButton *spinbutton, gpointer data) /* P723 index_start*/
{
	if(UNIT_MM == get_unit(pp->p_config))
		set_area_indexstart (pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0));
	else
		set_area_indexstart (pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937));
	DrawDisplayWindowFrame() ;
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;
}

void data_724 (GtkSpinButton *spinbutton, gpointer data) /* P724 index_end*/
{
	if(UNIT_MM == get_unit(pp->p_config))
		set_area_indexend (pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0));
	else
		set_area_indexend (pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937));

	DrawDisplayWindowFrame() ;
	RefreshScanInfor() ;
	pp->bRefreshDraw = TRUE ;

}

void data_725 (GtkSpinButton *spinbutton, gpointer data) /* P725 index_resolution*/
{
	if(UNIT_MM == get_unit(pp->p_config))
		set_area_indexresolution (pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0));
	else
		set_area_indexresolution (pp->p_config, (guint) (gtk_spin_button_get_value (spinbutton) * 1000.0 / 0.03937));
}

void data_730 (GtkMenuItem *menuitem, gpointer data) /* Scan->start->start mode */
{
	unsigned char temp_value = (unsigned char) (GPOINTER_TO_UINT (data)) ;
	if(temp_value == get_start_mode (pp->p_config))
	{
		return ;
	}
	set_start_mode (pp->p_config, temp_value);
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_731() /* P731 : Encoder start pause */
{
	unsigned char _nCurrentEcoderType ;
	int _nEncType = (int) get_inspec_source (pp->p_config) ;
	int _nEncStatus = get_start_pause (pp->p_config) ;
	_nEncStatus = !_nEncStatus ;
	set_start_pause (pp->p_config, _nEncStatus);

	if(_nEncStatus)
	{
		FreezingFPGA(TRUE) ;
		update_widget_bg(pp->event[19], 15);
	}
	else
	{
		update_widget_bg(pp->event[19],  16);
		FreezingFPGA(FALSE) ;
	}
	//*********************
	if(_nEncType == 0 ) // timer is no need to changed encoder parameters
	{
		return ;
	}
	//*********************
	_nEncType -= 1 ;
	_nCurrentEcoderType  = get_enc_type (pp->p_config,  _nEncType );
	if(_nEncStatus)
	{
		_nCurrentEcoderType |=  4 ;
	}
	else
	{
		_nCurrentEcoderType &= 11 ;
	}
	set_enc_type (pp->p_config,  _nCurrentEcoderType , _nEncType );
	if(_nEncType == 0)
	{
		 output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_ENCODE_X , _nCurrentEcoderType , 0);
	}
	if(_nEncType == 1)
	{
		 output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_ENCODE_Y , _nCurrentEcoderType , 0);
	}
	output_write_one_reg_to_spi(0,OUTPUT_OTHER_COMMAND_ENCODE_Y);
}

void data_732 (GtkMenuItem *menuitem, gpointer data) /* Scan->start->start */
{
	int ret;
	unsigned char  reset_mode = get_start_mode (pp->p_config);

	if (reset_mode == 0)  // reset all
	{
		ret = RefreshScanInfor();
		if(ret)
		{
			g_debug ("Scan init error!\n");
			pp->ScanStart = 0 ;
		}
		else
		{
			g_debug ("Scan Start!\n");
			pp->bRefreshDraw = TRUE ;
			pp->ScanStart = 1       ;
		}
	}
	else if (reset_mode == 1)  // reset encoder
	{
		reset_encoder();
		pp->bRefreshDraw = TRUE ;
		pp->ScanStart = 1 ;
	}
	else if (reset_mode == 2) // reset data ;
	{
		reset_scan_data() ;
		pp->bRefreshDraw = TRUE ;
		pp->ScanStart   = 1 ;
	}

}

void data_740 (GtkMenuItem *menuitem, gpointer data) /* Scan->Data->storage */
{
	set_data_storage (pp->p_config, (unsigned char) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_741 (GtkMenuItem *menuitem, gpointer data) /* P741  Scan->Data->inspec.data */
{
	set_data_inspec_data (pp->p_config, (unsigned char) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_800 (GtkMenuItem *menuitem, gpointer data) /* P800 File->File->Storage */
{
	set_file_storage (pp->p_config, (unsigned char) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}


void data_810 (GtkMenuItem *menuitem, gpointer data) /* P810 File->Report->Template */
{
	set_report_template (pp->p_config, (unsigned char) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_804 (GtkMenuItem *menuitem, gpointer data) /* File->File->Save Mode */
{
	set_file_save_mode (pp->p_config, (char) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_812 (GtkMenuItem *menuitem, gpointer data) /* P812 File->Report->paper size */
{
	set_report_paper_size (pp->p_config, (char) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_824 (GtkMenuItem *menuitem, gpointer data) /* File->Format->view*/
{
	set_report_format_view (pp->p_config, (unsigned char) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_830(GtkMenuItem *menuitem, gpointer data) /* P830 File->User Field->select */
{
	set_report_userfield_select (pp->p_config, (unsigned char) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
}

void data_900(GtkMenuItem *menuitem, gpointer data) /* Preferences->Pref.->Units */
{
	set_unit (pp->p_config, (unsigned char) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	draw_menu3(0, NULL);
	DrawDisplayWindowFrame() ;
	draw_field_name () ;
}

void data_903(GtkMenuItem *menuitem, gpointer data) /* Preferences->Pref.->scheme */
{
}

void data_904(GtkMenuItem *menuitem, gpointer data) /* Preferences->Pref.->gate mode */
{
}

void data_912 (GtkMenuItem *menuitem, gpointer data)
{
	set_language (pp->p_config, (unsigned char) (GPOINTER_TO_UINT (data)));
	pp->pos_pos = MENU3_STOP;
	//draw_menu3(0, NULL);
	change_language (get_language(pp->p_config), pp);
	draw_menu1();
	draw_menu2(0);
	draw_menu3(0, NULL);
	draw_field_name () ;
	//update label "Gain"
	if (get_group_db_ref (pp->p_config, get_current_group (pp->p_config)))
		tt_label_show_string (pp->label[0], con2_p[1][0][6], "\n", "(dB)", "white", 10);
	else
		tt_label_show_string (pp->label[0], con2_p[1][0][0], "\n", "(dB)", "white", 10);
}

void data_913(GtkMenuItem *menuitem, gpointer data) /* Preferences->system->assign key*/
{
}

void data_9131(GtkMenuItem *menuitem, gpointer data) /* Preferences->system->assign key*/
{
}

void data_930(GtkMenuItem *menuitem, gpointer data) /* Preferences->system->assign key*/
{
}

void data_901 (GtkSpinButton *spinbutton, gpointer data) /*scan_resolution*/
{
	set_bright (pp->p_config, (unsigned char) (gtk_spin_button_get_value (spinbutton)));
}




void RefreshFocallawParameters(int grp)
{

	TMP(beam_qty[grp])	= GetGroupBeamQty(grp);
	TMP(beam_num[grp]) 	= 0        ;
	TMP(current_angle[grp]) = LAW_VAL_POS(grp, Angle_min) * G_PI / 18000.0 ;

	if (LAW_VAL_POS(grp, Elem_qty) == 1)
		GROUP_VAL_POS(grp , sum_gain)	=  4095;
	else
		GROUP_VAL_POS(grp , sum_gain)	=  6400 / LAW_VAL_POS(grp, Elem_qty) ;
    TMP(group_spi[grp].sum_gain)  = GROUP_VAL_POS(grp , sum_gain);
}

//*********生存聚焦法则************
//  2011.6.25 何凡添加
//*********生存聚焦法则************
void generate_focallaw(int grp)
{
	// refresh beam_qty .......
	RefreshFocallawParameters(grp) ;
	cal_focal_law (grp);
	UpdateGateForSpiSending(grp);
}

// auto set 80%
// read 8 times gate amplitude at most , adjust Gain to proper value till the gate a go to desire amplitude
static int SetDBEightPercentThread(gpointer data)
{
	DRAW_UI_P pp = (DRAW_UI_P) data;
	int i = 8;
	int k;
	int offset = 0;
	double scale   ;
    char* markup   ;
	int grp = pp->p_config->groupId ;   //当前group
	for (k = 0 ; k < grp; k++)
		offset += TMP(beam_qty[k]);
	int index = offset + TMP(beam_num[grp]);
	int _bRefDB = get_group_db_ref (pp->p_config, grp) ;
    int _nRefDB = group_get_refgain(grp);

    double _nGateValue ;
    short  _nTmp       ;
    gshort gain = 0;
	while(i)
	{
		if(GROUP_VAL_POS(grp , rectifier1))
		{
			_nGateValue = ((TMP(measure_data[index][1])>>20) & 0x00000fff)/20.47;//满屏时200% 4095
		}
		else
		{
			_nTmp = (signed short)(TMP(measure_data[index][1]) >> 16) ;
			_nGateValue = fabs(_nTmp / (10.24 * 16));
		}

		if(fabs(_nGateValue - 80.0) <=1 )  break  ;
		scale =  80.0/_nGateValue  ;

        gain = group_get_gain(grp);
        gain += (gshort)(log10(scale)*2000);

        if(gain > 8000)	{
            gain = 8000;
        } else if(gain < 0)	{
            gain = 0;
		}

        group_set_gain(grp, gain);

        group_spi_send (grp);
		i--;
        if(_bRefDB) {
			markup = g_markup_printf_escaped (
								"<span foreground='white' font_desc='16'>%0.1f(%0.1f)</span>",
                                (gain - _nRefDB ) / 100.0,
								_nRefDB / 100.0);
		}
		else
		{
			markup = g_markup_printf_escaped ("<span foreground='white' font_desc='24'>%0.1f</span>",
                        gain / 100.0 );
		}
		gdk_threads_enter() ;
		gtk_label_set_markup (GTK_LABEL(pp->label[GAIN_VALUE]),markup);
		gdk_threads_leave() ;
		g_free(markup);

        if( gain  >= 8000) {
            break;
        } else if( gain <= 0) {
            break;
        }

		usleep(400000);
	}

	return 0;
}


void UpdateGateForSpiSending(int nGroupId_)
{
	double delay;
	int  k ;
	int _nBeamQty = TMP(beam_qty[nGroupId_]);
	double _nAngleCos ;


	int _nGateAStart , _nGateAWidth ;
	int _nGateBStart , _nGateBWidth ;
	int _nGateIStart , _nGateIWidth ;
	unsigned char _nGateASynchro  , _nGateBSynchro ;
	unsigned int _nPulseWidth = GROUP_VAL_POS(nGroupId_ , pulser_width1) / (10 * PW_DIV) ;
	unsigned int _nWedgeDelay = GROUP_VAL_POS(nGroupId_ , wedge_delay) / 10 + _nPulseWidth;
	_nGateAStart = GROUP_GATE_VAL_POS(nGroupId_ , GATE_A , start) / 10 ;
	_nGateAWidth = GROUP_GATE_VAL_POS(nGroupId_ , GATE_A , width) / 10 ;
    _nGateBStart = GROUP_GATE_VAL_POS(nGroupId_ , GATE_B , start) / 10 ;
    _nGateBWidth = GROUP_GATE_VAL_POS(nGroupId_ , GATE_B , width) / 10 ;
	_nGateIStart = GROUP_GATE_VAL_POS(nGroupId_ , GATE_I , start) / 10 ;
	_nGateIWidth = GROUP_GATE_VAL_POS(nGroupId_ , GATE_I , width) / 10 ;
	_nGateASynchro = GROUP_GATE_VAL_POS(nGroupId_ , GATE_A ,  synchro) ;
	_nGateBSynchro = GROUP_GATE_VAL_POS(nGroupId_ , GATE_B ,  synchro) ;

	_nAngleCos = 1;

	if(LAW_VAL_POS(nGroupId_ , Focal_type) == AZIMUTHAL_SCAN)
	{
		for ( k = 0; k < _nBeamQty ; k++)//k:每个beam
		{
			delay = GROUP_VAL_POS(nGroupId_ , beam_delay[k]) / 10 + _nWedgeDelay ;
			if(GROUP_VAL_POS(nGroupId_ , ut_unit)  == UT_UNIT_TRUE_DEPTH )
			{
				_nAngleCos = LAW_VAL_POS(nGroupId_ , Angle_min)/100.0 + k * LAW_VAL_POS(nGroupId_ , Angle_step) / 100.0 ;
				_nAngleCos = _nAngleCos * G_PI / 180.0 ;
				_nAngleCos = cos(_nAngleCos) ;
			}

			if(_nGateASynchro)
			{
				pp->gate_a_start[nGroupId_][k]	= (int)( _nGateAStart /  _nAngleCos );
				pp->gate_a_end[nGroupId_][k]	= (int)( (_nGateAStart + _nGateAWidth) / _nAngleCos);
			}
			else
			{
				pp->gate_a_start[nGroupId_][k]	= (int)( _nGateAStart /  _nAngleCos + delay  );
				pp->gate_a_end[nGroupId_][k]	= (int)( (_nGateAStart + _nGateAWidth) / _nAngleCos + delay );
			}

            if(_nGateBSynchro)
            {
    			pp->gate_b_start[nGroupId_][k]	= (int)( _nGateBStart /  _nAngleCos);
    			pp->gate_b_end[nGroupId_][k]	= (int)( (_nGateBStart + _nGateBWidth) / _nAngleCos);
            }
            else
            {
    			pp->gate_b_start[nGroupId_][k]	= (int)( _nGateBStart / _nAngleCos + delay );
    			pp->gate_b_end[nGroupId_][k]	= (int)( (_nGateBStart + _nGateBWidth) / _nAngleCos + delay );
            }

			pp->gate_i_start[nGroupId_][k]	= (int)( _nGateIStart / _nAngleCos + delay );
			pp->gate_i_end[nGroupId_][k]	= (int)( (_nGateIStart + _nGateIWidth) / _nAngleCos + delay );
		}
	}
	else// if(LAW_VAL_POS(grp , Focal_type) == LINEAR_SCAN) || UT_SCAN
	{
		if(GROUP_VAL_POS(nGroupId_ , ut_unit)  == UT_UNIT_TRUE_DEPTH &&
				GROUP_VAL_POS(nGroupId_ , group_mode) == PA_SCAN)
		{
			_nAngleCos = LAW_VAL_POS(nGroupId_ , Angle_min) / 100.0   ;
			_nAngleCos = _nAngleCos * G_PI / 180.0 ;
			_nAngleCos = cos(_nAngleCos) ;
		}

		for ( k = 0; k <  _nBeamQty ; k++)//k:每个beam
		{
			delay = GROUP_VAL_POS(nGroupId_ , beam_delay[k]) / 10 + _nWedgeDelay ;
			if(_nGateASynchro)
			{
				pp->gate_a_start[nGroupId_][k]	= (int)( _nGateAStart /  _nAngleCos);
				pp->gate_a_end[nGroupId_][k]	= (int)( (_nGateAStart + _nGateAWidth) / _nAngleCos);
			}
			else
			{
				pp->gate_a_start[nGroupId_][k]	= (int)( _nGateAStart / _nAngleCos + delay );
				pp->gate_a_end[nGroupId_][k]	= (int)( (_nGateAStart + _nGateAWidth) / _nAngleCos + delay );
			}

			if(_nGateBSynchro)
			{
				pp->gate_b_start[nGroupId_][k]	= (int)( _nGateBStart / _nAngleCos);
				pp->gate_b_end[nGroupId_][k]	= (int)( (_nGateBStart + _nGateBWidth) / _nAngleCos);
			}
			else
			{
				pp->gate_b_start[nGroupId_][k]	= (int)( _nGateBStart / _nAngleCos + delay );
				pp->gate_b_end[nGroupId_][k]	= (int)( (_nGateBStart + _nGateBWidth) / _nAngleCos + delay );
			}

			pp->gate_i_start[nGroupId_][k]	= (int)( _nGateIStart / _nAngleCos + delay );
			pp->gate_i_end[nGroupId_][k]	= (int)( (_nGateIStart + _nGateIWidth) / _nAngleCos + delay );
			//}
		}
	}
}


void data_file_type(GtkMenuItem *menuitem, gpointer data) /* file_open -> file_type*/
{
	set_file_type (pp->p_config, (unsigned char) (GPOINTER_TO_UINT (data)));

	Set_Source_File_Path(USER_CFG_PATH);

	switch (GPOINTER_TO_UINT (data))
	{
		case SETUP_FILE:
			selection_file_type(g_source_list, USER_CFG_PATH,	".cfg",FALSE);
			break;
		case DATA_FILE:
			selection_file_type(g_source_list, USER_CFG_PATH,	".data",FALSE);
			break;
		case REPORT_FILE:
			selection_file_type(g_source_list, USER_CFG_PATH,	".html",FALSE);
			break;
		case IMAGE_FILE:
			selection_file_type(g_source_list, USER_CFG_PATH,	".jpg",FALSE);
			break;
		default:
			break;
	}
}



void UTgroupConfigSetting(unsigned char grp)
{
	// set display Current group A_SCAN
	//set_display_pos (pp->p_config, A_SCAN);
	//set_display_group(pp->p_config, DISPLAY_CURRENT_GROUP); /*0是All*/
	GROUP_VAL_POS(grp , ut_unit) = UT_UNIT_SOUNDPATH  ;
	LAW_VAL_POS (grp , Wave_type)		=	LONGITUDINAL_WAVE;
	LAW_VAL_POS (grp , Angle_min)		=	0 ;
	LAW_VAL_POS (grp , Angle_max)		=	0 ;
	LAW_VAL_POS (grp , Focal_type)      =   0 ;

	LAW_VAL_POS (grp, Elem_qty)         =   1 ;
	LAW_VAL_POS (grp, First_tx_elem)    =   1 ;
	LAW_VAL_POS (grp, First_rx_elem)    =   1 ;
	LAW_VAL_POS (grp, Last_tx_elem )    =   1 ;
	LAW_VAL_POS (grp, Last_rx_elem )    =   1 ;

	GROUP_VAL_POS(grp , VelocityCalibrated) =  0  ;
	GROUP_VAL_POS(grp , WedgeDelayCalibrated) = 0 ;
	GROUP_VAL_POS(grp , SensationCalibrated) =  0  ;

}

void UT_group_config_settting (unsigned char grp)
{
	PROBE_P pProbe = &(GROUP_VAL_POS(grp , probe)) ;
	WEDGE_P pWedge = &(GROUP_VAL_POS(grp , wedge)) ;
	memset((void*)pProbe , 0 , sizeof(PROBE));
	memset((void*)pWedge , 0 , sizeof(WEDGE));
	GROUP_VAL_POS(grp , probe.Model[0])	=  32; /*  */

	if(GROUP_VAL_POS(grp , group_mode) == PA_SCAN)
	{
		GROUP_VAL_POS(grp , probe.Elem_qty)	=  16; /*  */
		GROUP_VAL_POS(grp , probe.Frequency)	=  5000; /*  */
		GROUP_VAL_POS(grp , probe.Pitch)	    =  500; /*  */
		GROUP_VAL_POS(grp , wedge.Orientation) = 1 ;
	}
	// set display Current group A_SCAN
	//set_display_pos (pp->p_config, A_SCAN);
	//set_display_group(pp->p_config, DISPLAY_CURRENT_GROUP); /*0是All*/
	GROUP_VAL_POS(grp , ut_unit) = UT_UNIT_SOUNDPATH  ;
	LAW_VAL_POS (grp , Wave_type)		=	LONGITUDINAL_WAVE;
	LAW_VAL_POS (grp , Angle_min)		=	0 ;
	LAW_VAL_POS (grp , Angle_max)		=	0 ;
	LAW_VAL_POS (grp , Focal_type)      =   0 ;
	LAW_VAL_POS (grp, Elem_qty)         =   1 ;
	LAW_VAL_POS (grp, First_tx_elem)    =   1 ;
	LAW_VAL_POS (grp, First_rx_elem)    =   1 ;
	LAW_VAL_POS (grp, Last_tx_elem )    =   1 ;
	LAW_VAL_POS (grp, Last_rx_elem )    =   1 ;
	GROUP_VAL_POS(grp , VelocityCalibrated)   =  0  ;
	GROUP_VAL_POS(grp , WedgeDelayCalibrated) =  0  ;
	GROUP_VAL_POS(grp , SensationCalibrated)  =  0  ;
    group_set_rx_tx_mode(grp, PULSE_ECHO);      // PE Mode
	// reset calibration status
	pp->ctype_pos = 1;
	pp->cmode_pos = 0;
}
void reset_scan_data()
{
#if ARM
	int i ;
	int _nDataSize   = pp->ScanInfor.DataSize ;
	int _nScanLength = pp->ScanInfor.ScanStepQty ;

	for(i = 0 ; i < MAX_SCAN_STEP ; i++)
	{
		ScanDataMark[i] = 0 ;
	}
	char* pData = (char*)TMP(StoreBuffAddr);
	memset(pData, 0 , _nDataSize * _nScanLength);
#endif
}



void reset_encoder ()
{
	unsigned char _nEncTypeX  = get_enc_type (pp->p_config,  0);
	unsigned char _nEncTypeY  = get_enc_type (pp->p_config,  1);

	output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_ENCODE_Y , 0 ,0);
	output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_ENCODE_X , 0 ,0);
	output_write_one_reg_to_spi(0,OUTPUT_OTHER_COMMAND_ENCODE_Y);

	usleep(10000);

	output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_ENCODE_Y , _nEncTypeY , 0 );
    output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_ENCODE_X , _nEncTypeX , 0 );
	output_write_one_reg_to_spi(0,OUTPUT_OTHER_COMMAND_ENCODE_Y);

}

// refresh parameters used in Scan process
//  return :abnormal return TRUE
//          else     return FALSE

#define _DmaFrameBuffer         DMA_MARK[0]
#define _DataDmaCounter         DMA_MARK[1]
#define _BufferInUse            DMA_MARK[2]
#define _ScanSource             DMA_MARK[3]
#define _StoreFrameCount        DMA_MARK[4]
#define _EncoderCounterOffset   DMA_MARK[5]
#define _StepsPerResolution     DMA_MARK[6]
#define _ScanZeroIndexOffset    DMA_MARK[7]
#define _MaxStoreIndex          DMA_MARK[8]
#define _ScanTimmerCounter      DMA_MARK[9]
#define _ScanTimmerCircled      DMA_MARK[10]



int RefreshScanInfor()
{
	int steps_per_mm    ;
	double _nRangeStart ;
	double _nRangeStop  ;
	double _nRangeStep  ;

	int _nScanSource  = get_inspec_source (pp->p_config)  ;
	// init parameter for dma module
	// get encoder type
	pp->ScanInfor.EncoderType = _nScanSource  ;
	int _nDataSize   =   getDataBlockSize();
	pp->ScanInfor.DataSize  = _nDataSize ;
	pp->ScanInfor.MaxStoreIndex  =  MIN( 255 * 1024 * 1024 / pp->ScanInfor.DataSize  , 255 * 1024 );

	if(_nScanSource)
	{
		// get the setting encoder information
		_nRangeStart  = pp->p_config->inspection_scan_start / 1000.0;
		_nRangeStop   = pp->p_config->inspection_scan_end   / 1000.0;
		_nRangeStep   = pp->p_config->inspection_scan_resolution / 1000.0;
		// total scan beam number
		pp->ScanInfor.ScanStepQty     = (int) ((_nRangeStop - _nRangeStart) / _nRangeStep + 1) ;

		steps_per_mm = pp->p_config->encoder1[_nScanSource - 1].Resolution / 1000.0 ;
		pp->ScanInfor.StepsPerResolution = steps_per_mm * _nRangeStep  ;
        if (enc_get_preset(pp->p_config, get_cur_encoder(pp->p_config))) {
            pp->ScanInfor.ScanZeroOffset = -(_nRangeStart - get_enc_origin(pp->p_config, get_cur_encoder(pp->p_config))/1000.0)/_nRangeStep;
        } else {
            pp->ScanInfor.ScanZeroOffset =  -(_nRangeStart / _nRangeStep);
        }
		// if store size needed is larger than 256M
		// return  ERROR
		if(pp->ScanInfor.ScanStepQty > pp->ScanInfor.MaxStoreIndex )
		{
			pp->ScanInfor.ScanStepQty = pp->ScanInfor.MaxStoreIndex  ;
			_nRangeStop = _nRangeStep * pp->ScanInfor.ScanStepQty + _nRangeStart ;
			pp->p_config->inspection_scan_end  =  _nRangeStop * 1000;
			return TRUE;
		}
		reset_encoder();
	}

#if ARM
	_ScanSource       = _nScanSource   ;
	_StoreFrameCount  = _nDataSize / DATA_SAVE_BLOCK_SIZE  ;
	if(_nScanSource == 1)
	{
		_EncoderCounterOffset =  GROUP_VAL_POS(0 , point_qty) + DMA_DATA_OFFSET + 4 * sizeof(int);
	}
	if(_nScanSource == 2)
	{
		_EncoderCounterOffset =  GROUP_VAL_POS(0 , point_qty) + DMA_DATA_OFFSET + 5 * sizeof(int) ;
	}
	_StepsPerResolution   =    pp->ScanInfor.StepsPerResolution  ;
	_ScanZeroIndexOffset  =    pp->ScanInfor.ScanZeroOffset      ;
	_MaxStoreIndex        =    pp->ScanInfor.MaxStoreIndex       ;
	_ScanTimmerCounter    =    0                                 ;
	_ScanTimmerCircled    =    0                                 ;
	int i ;
	if(!TMP(dataRecalling))
	{// if is recalling data , set recalling end
		for(i = 0 ; i < 10000 ; i++)
		{
			ScanDataMark[i] = 0 ;
		}
	}
#endif
	pp->ScanStart = FALSE ;
	return FALSE;
}


#define GPIO99_LOW		0x6004 
#define GPIO99_HIGH		0x6005 
#define GPIO100_LOW		0x6006 
#define GPIO100_HIGH	0x6007 
#define GPIO101_LOW		0x6008 
#define GPIO101_HIGH	0x6009

void software_update()
{

}

void hardware_update()
{
	static int fd_mtd,fd_gpio;
	int hd_low, size, val, mode;
	char data[0x150000];

	if((fd_mtd = open("/dev/spidev3.1", O_RDWR)) == -1)
	{
		perror("/dev/spidev3.1");
	}
	if( (fd_gpio = open("/dev/tt", O_RDWR)) == -1 )
	{
		perror("/dev/tt");
	}
	/*
	 * spi mode
	 */
	if ((ioctl(fd_mtd, SPI_IOC_WR_MODE, &mode)) == -1)
		perror("can't set spi wr_mode");
	if ((ioctl(fd_mtd, SPI_IOC_RD_MODE, &mode)) == -1)
		perror("can't get spi rd_mode");
	ioctl(fd_gpio, GPIO101_LOW, &val);	
	ioctl(fd_gpio, GPIO99_HIGH, &val);
	ioctl(fd_gpio, GPIO100_LOW, &val);//升级底板omap
	hd_low = open("/media/sda1/hd_low.sof",O_RDONLY);
	if(hd_low < 0)
	{
		close(hd_low);
	}
	else
	{	
		size = read(hd_low,data,0x150000);
		close(hd_low);
		lseek(fd_mtd, 0, SEEK_SET);
		write(fd_mtd,data,size);
	}
}


//当显示窗口被单击时，切换当前GROUP为该窗口的GROUP ID
int DrawAreaButtonClick (GtkWidget* widget, GdkEvent* event, gpointer data)
{

	DRAW_AREA* _pDrawArea = (DRAW_AREA*)data  ;
    int _nGroupId = get_current_group(pp->p_config) ;
    int _nAreaGroupId = _pDrawArea->group  ;
    if(_nGroupId == _nAreaGroupId || _nAreaGroupId >= get_group_qty(pp->p_config))
    {
         return  FALSE;
    }

    if(pp->pos_pos == MENU3_PRESSED)
    {
    	pp->pos_pos = MENU3_STOP ;
    	draw_menu3(0, NULL);
    }

    RefreshGainMark(_nAreaGroupId) ;
	pp->p_config->groupId = _nAreaGroupId ;
	if(GROUP_VAL_POS(_nAreaGroupId , group_mode) == PA_SCAN) /*group mode 选择UT,UT1,UT2时，focal law 不可用*/
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

    DrawDisplayWindowFrame()  ;
    pp->pos_pos = MENU3_STOP;
    draw_menu3(0, NULL);
    // reset calibration status
	pp->ctype_pos = 1;
	pp->cmode_pos = 0;

    return FALSE ;
}

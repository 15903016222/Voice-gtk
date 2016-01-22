/*
 * drawmenu1.c
 *
 *  Created on: 2012-5-9
 *      Author: shensheng
 */
#include "../../drawui.h"
#include "../../file_op.h"
#include "../../drawfb.h"
#include "../../draw_dialog.h"
#include "../../callback.h"
#include "../../main.h"
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <glib/gprintf.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <linux/rtc.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <stdio.h>
#include <math.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../../calculation/limit.h"
#include "../ui.h"
#include "menu.h"
#include "../../string/_string.h"
#include "../../lzk/fileHandler.h"
#include "../authorization.h"
#include "../workpiece.h"

void DrawMenu001()
{

}
void DrawMenu011()
{

}

void DrawMenu021Encoder()
{
	if((get_inspec_type (pp->p_config)==0)&&(get_inspec_source (pp->p_config)==0))/*Type选择one-Line Scan   &&   Scan选择Time时，此键不可调*/
	{
		gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
	}
	else
	{
		if (pp->cstart_qty == 2)
			draw3_popdown_offset (NULL, 1, 1, 19 );
		else if (pp->cstart_qty == 3)
			draw3_popdown_offset (NULL, 1, 1, 30 );
		else if (pp->cstart_qty == 4)
			draw3_popdown_offset (NULL, 1, 1, 32 );
	}
}

int IfCalibrationFinished()
{
	int ret  = FALSE ;
	int grp = get_current_group(pp->p_config);

	if(LAW_VAL_POS(grp,Focal_type) == AZIMUTHAL_SCAN)
	{
		if(TMP_CBA(clb_end) >= LAW_VAL_POS(grp , Angle_max))
		{
			ret = FALSE ;
		}
		else
		{
			ret = TRUE ;
		}
	}
	else if (LAW_VAL_POS(grp,Focal_type) == LINEAR_SCAN)
	{
		if((TMP_CBA(clb_end)/100) >= TMP(beam_qty[grp]))
		{
			ret = FALSE ;
		}
		else
		{
			ret = TRUE ;
		}
	}

	return ret ;
}

void DrawMenu021UltrasoundTCGStep6()
{
	//draw3_popdown_offset(NULL, 4, 1 , 59);
	int grp = get_current_group(pp->p_config);
	if(TMP_CBA(bNewPoint))
	{
		draw3_popdown_offset(NULL, 1, 1 , 31);
	}
	else
	{
		//if(GROUP_VAL_POS(grp , SizingCurves.point_pos) >= (GROUP_VAL_POS(grp , SizingCurves.dac_point_qty) - 1))
		//{
		//	draw3_popdown_offset(NULL, 1, 1 , 60);
		//}
		//else
		//{
			draw3_popdown_offset(NULL, 1, 1 , 59);
		//}

		if(GROUP_VAL_POS(get_current_group(pp->p_config) , SizingCurves.dac_point_qty) == 16)
		{
			gtk_widget_set_sensitive(pp->eventbox30[4],FALSE);
			gtk_widget_set_sensitive(pp->eventbox31[4],FALSE);
		}
	}
}

void DrawMenu021UltraSound()
{
	int grp = get_current_group (pp->p_config);

	if(PA_SCAN == GROUP_VAL_POS(grp , group_mode))
	{
		if( (pp->cstart_qty >1) && (pp->cstart_qty < 4) )
			draw3_popdown_offset (NULL, 1, 1, 19 );
		else if (pp->cstart_qty == 4)
		{
			if (pp->cmode_pos == 0)
				draw3_popdown_offset (NULL, 1, 1, 47 );//
			else if (pp->cmode_pos == 3)
				draw3_popdown_offset (NULL, 1, 1, 37 );
			else if(pp->cmode_pos ==1)
				draw3_popdown_offset (NULL, 1, 1, 19 );
		}
		else if (pp->cstart_qty == 5)
		{

			if (pp->cmode_pos == 0)
				draw3_popdown_offset (NULL, 1, 1, 48 );//
			else if (pp->cmode_pos == 1)
				if( IfCalibrationFinished())
					draw3_popdown_offset (NULL, 1, 1, 56 );
				else
				    draw3_popdown_offset (NULL, 1, 1, 32 );
			else if(pp->cmode_pos == 3) //TCG
                DrawMenu021UltrasoundTCGStep6();
			else
				draw3_popdown_offset (NULL, 1, 1, 19 );
		}
		else if (pp->cstart_qty == 6)
		{
			if (pp->cmode_pos == 0)
			{
				draw3_popdown_offset (NULL, 1, 1, 32 );
			}
			else if(pp->cmode_pos == 2)
			{
				if( IfCalibrationFinished())
					draw3_popdown_offset (NULL, 1, 1, 56 );
				else
				    draw3_popdown_offset (NULL, 1, 1, 32 );
			}
			//else if(pp->cmode_pos == 3) //TCG
			//{
			//	draw3_popdown_offset (NULL, 1, 1, 62 );
			//}
			else
				draw3_popdown_offset (NULL, 1, 1, 19 );
		}
		else
			draw3_popdown (NULL, 1, 1);
	}
	else
	{
		if( (pp->cstart_qty >1) && (pp->cstart_qty < 4) )
			draw3_popdown_offset (NULL, 1, 1, 19 );
		else if (pp->cstart_qty == 4)
		{
			draw3_popdown_offset (NULL, 1, 1, 47 );
		}
		else if (pp->cstart_qty == 5)
		{
			if(GROUP_VAL_POS(grp , tx_rxmode1) == TOFD)
			{
				draw3_popdown_offset (NULL, 1, 1, 32 );//
				//draw3_popdown_offset (NULL, 1, 1, 48 );//
			}
			else
			{
				if (pp->cmode_pos == 0)//声速
					draw3_popdown_offset (NULL, 1, 1, 48 );//
				else if (pp->cmode_pos == 1)//延迟
					draw3_popdown_offset (NULL, 1, 1, 32 );
				else if (pp->cmode_pos == 2)//wedge delay + velocity
						draw3_popdown_offset (NULL, 1, 1, 48 );
			}

		}
		else if (pp->cstart_qty == 6)
		{
			//if(GROUP_VAL_POS(grp , tx_rxmode1) == TOFD)
		    //{
			//	draw3_popdown_offset (NULL, 1, 1, 32 );//
			//}
			//else
			//{
				if ((pp->cmode_pos == 0)||(pp->cmode_pos == 2))
					draw3_popdown_offset (NULL, 1, 1, 32 );
				else
					draw3_popdown_offset (NULL, 1, 1, 19 );
			//}
		}
		else
			draw3_popdown (NULL, 1, 1);
	}

}

void DrawMenu021()
{
	if(1 == pp->cstart_qty)
	{
		gtk_widget_set_sensitive(pp->eventbox2[0],TRUE);
		gtk_widget_set_sensitive(pp->eventbox2[1],TRUE);
		gtk_widget_set_sensitive(pp->eventbox2[3],TRUE);
		gtk_widget_set_sensitive(pp->menubar,TRUE);
	}

	if((pp->pos == 0) && (pp->pos1[pp->pos] == 2) && (pp->cstart_qty == 2))//Calibration
	{
		if(!pp->ctype_pos)//当位Encoder时无需更新扫描
		{
			pp->clb_encoder = 1;
			gtk_widget_set_sensitive(pp->eventbox2[0],FALSE);
			gtk_widget_set_sensitive(pp->eventbox2[1],FALSE);
			gtk_widget_set_sensitive(pp->eventbox2[3],FALSE);
			gtk_widget_set_sensitive(pp->menubar,FALSE);
		}
		else
		{
			gtk_widget_set_sensitive(pp->eventbox2[0],FALSE);
			gtk_widget_set_sensitive(pp->eventbox2[1],FALSE);
			gtk_widget_set_sensitive(pp->eventbox2[3],FALSE);
			gtk_widget_set_sensitive(pp->menubar,FALSE);
			//set_overlay_gate(pp->p_config,1);
		}
	}
	draw3_popdown (NULL, 1, 1);
	if(pp->ctype_pos == 0)//编码器
	{
		DrawMenu021Encoder();
	}
	else if(pp->ctype_pos == 1)//超声
	{
		DrawMenu021UltraSound() ;
	}
	else if(pp->ctype_pos == 2)
	{
		if (pp->cstart_qty == 1)
			draw3_popdown (NULL, 1, 1);
		else if (pp->cstart_qty == 4)
			draw3_popdown_offset (NULL, 1, 1, 30 );
		else if (pp->cstart_qty == 5)
			draw3_popdown_offset (NULL, 1, 1, 32 );
		else
			draw3_popdown_offset (NULL, 1, 1, 19 );
	}
}

void DrawMenu031()
{
	int grp  =  get_current_group(pp->p_config);
	draw3_popdown (NULL, 1, 1);
	unsigned char language = get_language(pp->p_config);
	const char* lang;
	const char* weld = "http://127.0.0.1/home/tt/TT/pic/weld/";
	switch(language)
	{
	case 1:
		lang = "Chinese/";
		break;
	case 0:
	default:
		lang = "English/";
		break;
	}
	if(pp->wstart_qty == 2)
	{
		memset(pp->file_path,0,128);
		strcpy(pp->file_path, weld);
		strcat(pp->file_path ,lang);
		strcat(pp->file_path, "1.jpg");
		webkit_web_view_load_uri (pp->web_view, pp->file_path);
		gtk_widget_hide(pp->hbox211);
		gtk_widget_show(pp->sw);
	}
	else if(pp->wstart_qty == 3)
	{
		memset(pp->file_path,0,128);
		switch(get_part_weld(grp))
		{
			case 0:
				strcpy(pp->file_path, weld);
				strcat(pp->file_path ,lang);
				strcat(pp->file_path, "2.jpg");
				break;
			case 1:
				strcpy(pp->file_path, weld);
				strcat(pp->file_path ,lang);
				strcat(pp->file_path, "3.jpg");
				break;
			case 2:
				strcpy(pp->file_path, weld);
				strcat(pp->file_path ,lang);
				strcat(pp->file_path, "5.jpg");
				break;
			case 3:
				strcpy(pp->file_path, weld);
				strcat(pp->file_path ,lang);
				strcat(pp->file_path, "7.jpg");
				break;
			case 4:
				strcpy(pp->file_path, weld);
				strcat(pp->file_path ,lang);
				strcat(pp->file_path, "9.jpg");
				break;
			case 5:
				strcpy(pp->file_path, weld);
				strcat(pp->file_path ,lang);
				strcat(pp->file_path, "12.jpg");
				break;

			default:break;
		}
		webkit_web_view_load_uri (pp->web_view, pp->file_path);
		gtk_widget_hide(pp->hbox211);
		gtk_widget_show(pp->sw);
	}
	else if(pp->wstart_qty == 4)
	{
		memset(pp->file_path,0,128);
		switch(get_part_weld(grp))
		{
			case 0:
				strcpy(pp->file_path, weld);
				strcat(pp->file_path ,lang);
				strcat(pp->file_path, "2.jpg");
				break;
			case 1:
				strcpy(pp->file_path, weld);
				strcat(pp->file_path ,lang);
				strcat(pp->file_path, "4.jpg");
				break;
			case 2:
				strcpy(pp->file_path, weld);
				strcat(pp->file_path ,lang);
				strcat(pp->file_path, "6.jpg");
				break;
			case 3:
				strcpy(pp->file_path, weld);
				strcat(pp->file_path ,lang);
				strcat(pp->file_path, "8.jpg");
				break;
                        case 4:
                                strcpy(pp->file_path, weld);
                                strcat(pp->file_path ,lang);
                                strcat(pp->file_path, "10.jpg");
                                break;
                        case 5:
                                strcpy(pp->file_path, weld);
                                strcat(pp->file_path ,lang);
                                strcat(pp->file_path, "11.jpg");
                                break;

			default:break;
		}
		webkit_web_view_load_uri (pp->web_view, pp->file_path);
		gtk_widget_hide(pp->hbox211);
		gtk_widget_show(pp->sw);
	}
	else
	{
		if(pp->help_yn)/*帮助文档打开时则显示帮助文档*/
		{
			gtk_widget_hide(pp->hbox211);
			gtk_widget_show(pp->sw);
		}
		else
		{
			gtk_widget_show(pp->hbox211);
			gtk_widget_hide(pp->sw);
		}
	}

	if( (pp->wstart_qty >1) && (pp->wstart_qty < 4) )
	{
		draw3_popdown_offset (NULL, 1, 1, 6 );
		gtk_widget_set_sensitive(pp->eventbox2[0],FALSE);
		gtk_widget_set_sensitive(pp->eventbox2[1],FALSE);
		gtk_widget_set_sensitive(pp->eventbox2[2],FALSE);
		gtk_widget_set_sensitive(pp->menubar,FALSE);

	}
	else if (pp->wstart_qty ==4)
	{
		draw3_popdown_offset (NULL, 1, 1, 9 );
		gtk_widget_set_sensitive(pp->eventbox2[0],FALSE);
		gtk_widget_set_sensitive(pp->eventbox2[1],FALSE);
		gtk_widget_set_sensitive(pp->eventbox2[2],FALSE);
		gtk_widget_set_sensitive(pp->menubar,FALSE);
	}
	else
	{
		gtk_widget_set_sensitive(pp->eventbox2[0],TRUE);
		gtk_widget_set_sensitive(pp->eventbox2[2],TRUE);
		gtk_widget_set_sensitive(pp->menubar,TRUE);
		gtk_widget_set_sensitive(pp->eventbox2[1],TRUE);
	}

}
void DrawMenu041()
{
	if ( !con2_p[0][4][1] )
	{
		gtk_widget_hide (pp->eventbox30[1]);
	    gtk_widget_hide (pp->eventbox31[1]);
	}
}


void DrawMenu101()
{
	double tmpf ;

	double cur_value=0.0, lower, upper, step;
	int digit, pos, unit;
	int grp = get_current_group (pp->p_config);

	switch (TMP(start_reg))
	{
		case 0:	tmpf = (GROUP_VAL_POS(grp , range) / 1000.0) / (double)(GROUP_VAL_POS( grp , point_qty)); break;
		case 1:	tmpf = (GROUP_VAL_POS(grp , range) / 1000.0) / 20.0 ; break;
		case 2:	tmpf = (GROUP_VAL_POS(grp , range) / 1000.0) / 10.0 ; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
	{

		if(UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit))
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
				cur_value = (GROUP_VAL_POS(grp , start) / 1000.0) * (GROUP_VAL_POS(grp , velocity) / 200000.0);   /* 当前显示的范围数值mm */
				cur_value = cur_value * cos(TMP(current_angle[grp]));
				lower = 0;
				upper =	( PrfLimitToGroupSampleRange(grp)/ 100.0 - GROUP_VAL_POS(grp , range)/ 1000.0) * (GROUP_VAL_POS(grp , velocity) / 200000.0);
				upper = upper * cos(TMP(current_angle[grp])) ;
				step = tmpf * (GROUP_VAL_POS(grp , velocity) / 200000.0);
				step = step * cos(TMP(current_angle[grp]));
				if (step < 0.01) step = 0.01 ;
				digit = 2;
				pos = 1;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = (GROUP_VAL_POS(grp , start) / 1000.0) * 0.03937 * (GROUP_VAL_POS(grp , velocity) / 200000.0); /* 当前显示的范围inch */
				cur_value = cur_value * cos(TMP(current_angle[grp]));
				lower = 0;
				upper =	(PrfLimitToGroupSampleRange(grp)/ 100.0 - GROUP_VAL_POS(grp , range) / 1000.0 )
								  * 0.03937 * GROUP_VAL_POS(grp , velocity) / 200000.0;
				upper = upper * cos(TMP(current_angle[grp]));
				step = tmpf * 0.03937 * GROUP_VAL_POS(grp , velocity) / 200000.0;
				step = step * cos(TMP(current_angle[grp]));
				digit = 3;
				pos = 1;
				unit = UNIT_INCH;
			}
		}
		else if  (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit))
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
				cur_value = (GROUP_VAL_POS(grp , start) / 1000.0) * (GROUP_VAL_POS(grp , velocity) / 200000.0);   /* 当前显示的范围数值mm */
				lower = 0;
				upper =	(PrfLimitToGroupSampleRange(grp)/ 100.0  - GROUP_VAL_POS(grp , range) / 1000.0) * (GROUP_VAL_POS(grp , velocity) / 200000.0);
				step = tmpf * (GROUP_VAL_POS(grp , velocity) / 200000.0);
				if (step < 0.01) step = 0.01 ;
				digit = 2;
				pos = 1;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = (GROUP_VAL_POS(grp , start)/ 1000.0) * 0.03937 * (GROUP_VAL_POS(grp , velocity) / 200000.0); /* 当前显示的范围inch */
				lower = 0;
				upper =	(PrfLimitToGroupSampleRange(grp)/ 100 - GROUP_VAL_POS(grp , range) / 1000.0 ) * 0.03937 * GROUP_VAL_POS(grp , velocity) / 200000.0;
				step = tmpf * 0.03937 * GROUP_VAL_POS(grp , velocity) / 200000.0;
				digit = 3;
				pos = 1;
				unit = UNIT_INCH;
			}
		}
		else
		{
			cur_value = GROUP_VAL_POS(grp , start) / 1000.0 ;
			lower =	0 ;
			upper =	PrfLimitToGroupSampleRange(grp)/ 100.0 - GROUP_VAL_POS(grp , range ) / 1000.0;
			step = tmpf;
			if (step < 0.01)  (step = 0.01) ;
			pos = 1;
			digit = 2;
			unit = UNIT_US;
		}
		draw3_digit_pressed (data_101, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 0);
	}
	else
	{
		if(UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit))
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
				cur_value = (GROUP_VAL_POS(grp , start) / 1000.0) * (GROUP_VAL_POS(grp , velocity) / 200000.0);   /* 当前显示的范围数值mm */
				cur_value = cur_value * cos(TMP(current_angle[grp]));
				unit = UNIT_MM;
				digit = 2;
				pos = 1;
			}
			else
			{
				cur_value = (GROUP_VAL_POS(grp , start)/ 1000.0) * 0.03937 * (GROUP_VAL_POS(grp , velocity) / 200000.0); /* 当前显示的范围inch */
				cur_value = cur_value * cos(TMP(current_angle[grp]));
				unit = UNIT_INCH;
				digit = 3;
				pos = 1;
			}
		}
		else if(UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit))
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
				cur_value = (GROUP_VAL_POS(grp , start) / 1000.0) * (GROUP_VAL_POS(grp , velocity) / 200000.0);   /* 当前显示的范围数值mm */
				unit = UNIT_MM;
				digit = 2;
				pos = 1;
			}
			else
			{
				cur_value = (GROUP_VAL_POS(grp , start) / 1000.0) * 0.03937 * (GROUP_VAL_POS(grp , velocity) / 200000.0); /* 当前显示的范围inch */
				unit = UNIT_INCH;
				digit = 3;
				pos = 1;
			}
		}
		else
		{
			cur_value = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START) / 1000.0 ;
			unit = UNIT_US;
			digit = 2;
			pos = 1;
		}

		draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
	}
}

void DrawMenu111()
{
	int grp = get_current_group (pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	pp->x_pos = 478, pp->y_pos = 224 ;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
	{
		/* PA时候如何能够选择 TT PC */
		if (GROUP_VAL_POS (grp , group_mode) == PA_SCAN)
			draw3_pop_tt (data_111, NULL,
					menu_content[TX_RX_MODE + 4 + get_group_val (p_grp, GROUP_TX_RX_MODE)],
					menu_content + TX_RX_MODE, 3, 1, get_group_val (p_grp, GROUP_TX_RX_MODE), 0x0);
		else //if (GROUP_VAL (group_mode) == UT_SCAN)
		{
			unsigned int status = gTofdEnable ? 0 : 0x8 ;
			draw3_pop_tt (data_111, NULL,
					menu_content[TX_RX_MODE + 4 + get_group_val (p_grp, GROUP_TX_RX_MODE)],
#if HIGH_POWER
					menu_content + TX_RX_MODE, 4, 1, get_group_val (p_grp, GROUP_TX_RX_MODE), 0xc);
#else
					menu_content + TX_RX_MODE, 4, 1, get_group_val (p_grp, GROUP_TX_RX_MODE), status);
#endif
		}

	}
	else
		draw3_popdown (menu_content[TX_RX_MODE + 4 + get_group_val (p_grp, GROUP_TX_RX_MODE)], 1, 0);

	if(!gPithCatchEnable && GROUP_VAL_POS (grp , group_mode) <= PA_SCAN)
	{
		gtk_widget_set_sensitive (pp->eventbox30[1], FALSE);
		gtk_widget_set_sensitive (pp->eventbox31[1], FALSE);
	}
}

void DrawMenu121()
{
	pp->x_pos = 516, pp->y_pos = 201 - 26;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
	{
		if (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_FILTER_POS))
			draw3_pop_tt (data_121, NULL,
					menu_content[FILTER + get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_FILTER_POS)],
					menu_content + FILTER , 7, 1, get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_FILTER_POS), 0);
		else
			draw3_pop_tt (data_121, NULL, "None",
					menu_content + FILTER , 7, 1, get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_FILTER_POS), 0);
	}
	else
	{
		if (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_FILTER_POS))
			draw3_popdown (menu_content[FILTER + get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_FILTER_POS)], 1, 0);
		else
			draw3_popdown ("None", 1, 0);
	}
}
void DrawMenu131()
{
	double tmpf ;
	double cur_value=0.0, lower, upper, step;
	int digit, pos, unit;
	int grp = get_current_group (pp->p_config);

	if (GROUP_VAL_POS( grp , group_mode) == PA_SCAN)	/* 脉冲回波模式不可以调节 */
	{
		gtk_widget_set_sensitive (pp->eventbox30[1], FALSE);
		gtk_widget_set_sensitive (pp->eventbox31[1], FALSE);
	}
	switch (TMP(indexoffset_reg))
	{
		case 0:	tmpf = 1; break;
		case 1:	tmpf = 5; break;
		case 2:	tmpf = 0.1; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
	{
		cur_value = GROUP_VAL_POS( grp , index_offset)/10.0;
		lower = -10000.0;
		upper =  9999.0;
		step = tmpf;
		digit = 1;
		pos = 1;
		unit = UNIT_MM;
		draw3_digit_pressed (data_511, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 0);
	}
	else
	{
		cur_value =GROUP_VAL_POS( grp , index_offset)/10.0;
		digit = 1;
		pos = 1;
		unit = UNIT_MM;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
	}
}
void DrawMenu141()
{
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
	{
		ascanResetRef(get_current_group(pp->p_config));
	}
	draw3_popdown (NULL, 1, 1);
}

void DrawMenu201()
{
	pp->x_pos = 590, pp->y_pos = 253 - 26;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
		draw3_pop_tt(data_201, NULL,
				menu_content[PARAMETERS + GROUP_GATE_POS(parameters)],
				menu_content + PARAMETERS, 2, 1, GROUP_GATE_POS(parameters), 0);
	else
		draw3_popdown(menu_content[PARAMETERS + GROUP_GATE_POS(parameters)], 1, 0);
}
void DrawMenu211()
{

}
void DrawMenu221()
{

}
void DrawMenu231()
{
	double tmpf ;
	char *str ;
	double cur_value=0.0, lower, upper, step;
	int digit, pos, unit;
	int grp = get_current_group (pp->p_config);
	pp->x_pos = 570, pp->y_pos = 250-26;
	if (GROUP_VAL_POS( grp , SizingCurves.mode_pos) == 0)  /* MODE 为Setup 时候 */
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
			draw3_pop_tt (data_231, NULL,
					menu_content[CURVE_POS+GROUP_VAL_POS( grp , SizingCurves.curve_pos)],
					menu_content + CURVE_POS, 4, 1, GROUP_VAL_POS( grp , SizingCurves.curve_pos), 0);
		else
			draw3_popdown (menu_content[CURVE_POS+GROUP_VAL_POS( grp , SizingCurves.curve_pos)], 1, 0);
	}
	else if (GROUP_VAL_POS( grp , SizingCurves.mode_pos) == 1)  /* MODE 为 Edit的时候 */
	{
		if (GROUP_VAL_POS( grp , SizingCurves.curve_pos) == 1 || GROUP_VAL_POS( grp , SizingCurves.curve_pos) == 3)
			/* DAC 或者 TCG 时候 */
		{
			if((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
			{
				draw3_pop_tt (data_2311, NULL,
						menu_content[ ALARM_POS + GROUP_VAL_POS( grp , SizingCurves.point_pos) ],
						menu_content + ALARM_POS, GROUP_VAL_POS( grp , SizingCurves.dac_point_qty), 1, GROUP_VAL_POS( grp , SizingCurves.point_pos), 0);
				str = g_strdup_printf ("%s", con2_p[2][3][10]);
				gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);
			}
			else
			{
				draw3_popdown (menu_content[ALARM_POS + GROUP_VAL_POS( grp , SizingCurves.point_pos)], 1, 0);
				str = g_strdup_printf ("%s", con2_p[2][3][10]);
				gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);
			}
		}
		else if (GROUP_VAL_POS( grp , SizingCurves.curve_pos) == 2) /* linear DAC */
		{
			/* 当前步进 */
			switch (pp->p_tmp_config->mat_atten_reg)
			{
				case 0:	tmpf = 0.01; break;
				case 1:	tmpf = 0.1; break;
				case 2:	tmpf = 1; break;
				default: break;
			}
			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
			{
				if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
				{
					if (UNIT_MM == get_unit(pp->p_config))
					{
						cur_value = GROUP_VAL_POS( grp , SizingCurves.mat_atten) / 1000.0;
						lower = 0.00;
						upper = 4002000.0/get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY);
						step = tmpf;
						digit = 2;
						pos = 1;
						unit = UNIT_DB_MM;
					}
					else
					{
						cur_value = GROUP_VAL_POS( grp , SizingCurves.mat_atten) /1000.0 / 0.03937;
						lower = 0.00;
						upper = 4002000.0/get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/0.03937;
						step = tmpf/10.0;
						digit = 3;
						pos = 1;
						unit = UNIT_DB_INCH;
					}
				}
				else
				{
					cur_value = GROUP_VAL_POS( grp , SizingCurves.mat_atten) / 1000.0/0.338;
					lower = 0.00;
					upper = 20.0;
					step = tmpf/10.0;
					digit = 2;
					pos = 1;
					unit = UNIT_DB_US;
				}
				draw3_digit_pressed (data_2312, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 15);
			}
			else
			{
				if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
				{
					if (UNIT_MM == get_unit(pp->p_config))
					{
						cur_value = GROUP_VAL_POS( grp , SizingCurves.mat_atten) / 1000.0;
						digit = 2;
						pos = 1;
						unit = UNIT_DB_MM;
					}
					else
					{
						cur_value = GROUP_VAL_POS( grp , SizingCurves.mat_atten) /1000.0 / 0.03937;
						digit = 3;
						pos = 1;
						unit = UNIT_DB_INCH;
					}
				}
				else
				{
					cur_value = GROUP_VAL_POS( grp , SizingCurves.mat_atten) / 1000.0/0.338;
					digit = 2;
					pos = 1;
					unit = UNIT_DB_US;
				}
				draw3_digit_stop (cur_value, units[unit], digit, pos, 15);
			}
		}
	}
}

void DrawMenu241()
{
	if ( !con2_p[2][4][1] )
		gtk_widget_hide (pp->eventbox30[1]);
	gtk_widget_hide (pp->eventbox31[1]);
}

void DrawMenu301()
{
	gtk_label_set_text (GTK_LABEL (pp->label3[1]), con2_p[3][0][1]);
	gtk_label_set_text (GTK_LABEL (pp->data3[1]) ,getMainDictString(MAINSTRINGDICT_WELD ,get_reading_list(pp->p_config)));
	/* 显示和隐藏控件 */
	gtk_widget_show (pp->eventbox30[1]);
	gtk_widget_show (pp->eventbox31[1]);
	gtk_widget_show (pp->data3[1]);
	gtk_widget_set_sensitive(pp->eventbox30[1] ,FALSE);
	gtk_widget_set_sensitive(pp->eventbox31[1] ,FALSE);

}
void DrawMenu311()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower, upper, step;
	int digit, pos, unit;

	int grp = get_current_group (pp->p_config);
	int _nSelection = GROUP_VAL_POS(grp , selection) ;
	if(_nSelection == 0)
	{
		/* 当前步进 */
		switch (pp->p_tmp_config->per_reference_reg)
		{
			case 0:	tmpf = 1; break;
			case 1:	tmpf = 10; break;
			case 2:	tmpf = 0.1; break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
		{
			cur_value = GROUP_VAL_POS( grp , per_reference)/100.0;
			lower = 0.0;
			upper = 100.0;
			step = tmpf;
			digit = 1;
			pos = 1;
			unit = UNIT_BFH;
			draw3_digit_pressed (data_311, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 0);
		}
		else
		{
			cur_value = GROUP_VAL_POS( grp , per_reference)/100.0;
			digit = 1;
			pos = 1;
			unit = UNIT_BFH;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		}

	}
	else if (_nSelection==1 || _nSelection==2 )
	{
		/* 当前步进 */
		switch (pp->p_tmp_config->s_reference_reg)
		{
			case 0:	tmpf = 1; break;
			case 1:	tmpf = 5; break;
			case 2:	tmpf = 0.1; break;
			case 3:	tmpf = 0.5; break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
		{
			if(get_inspec_source (pp->p_config) == 0)
			{
				cur_value = GROUP_VAL_POS( grp , s_reference)/100.0;
				double rate =  GROUP_VAL_POS(0 , prf1) * get_area_scanresolution (pp->p_config) / 10000.0;
				cur_value /= rate ;
				lower = -1000.0 ;
				upper =  1000.0;
				step = tmpf;
				digit = 1;
				pos = 1;
				unit = UNIT_S;
			}
			else
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					cur_value = GROUP_VAL_POS( grp , s_reference)/100.0;
					lower = -10000.0;
					upper =  10000.0;
					step = tmpf;
					digit = 1;
					pos = 1;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = GROUP_VAL_POS( grp , s_reference)/100.0*0.03937;
					lower = -10000.0*0.03937;
					upper =  10000.0*0.03937;
					step = tmpf;
					digit = 3;
					pos = 1;
					unit = UNIT_INCH;
				}
			}

			draw3_digit_pressed (data_3111, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 6);
		}
		else
		{
			if(get_inspec_source (pp->p_config) == 0)
			{
				cur_value = GROUP_VAL_POS( grp , s_reference)/100.0;
				double rate =  GROUP_VAL_POS(0 , prf1) * get_area_scanresolution (pp->p_config) / 10000.0;
				cur_value /= rate ;
				digit = 1;
				pos = 1;
				unit = UNIT_S;
			}
			else
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					cur_value = GROUP_VAL_POS( grp , s_reference)/100.0;
					digit = 1;
					pos = 1;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = GROUP_VAL_POS( grp , s_reference)/100.0*0.03937;
					digit = 3;
					pos = 1;
					unit = UNIT_INCH;
				}
			}
			draw3_digit_stop (cur_value, units[unit], digit, pos, 6);
		}
	}
	else if (_nSelection == 3 || _nSelection == 7)
	{
		tmpf = LAW_VAL_POS(grp, Angle_step) / 100.0 ;
		pos = 1;
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
		{
			if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
			{
				cur_value = LAW_VAL_POS(grp, Angle_min)/100.0 + TMP(beam_num[grp]) * LAW_VAL_POS(grp, Angle_step)/100.0;
				lower =	LAW_VAL_POS(grp , Angle_min) / 100.0;
				upper =	LAW_VAL_POS(grp , Angle_max) / 100.0;
				step = tmpf;
				digit = 1;
				unit = UNIT_DEG;
				draw3_digit_pressed (data_3112, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 12);
			}
			else
			{
				cur_value = TMP(beam_num[grp]) + 1.0;
				lower =	1;
				upper =	TMP(beam_qty[grp]) ;
				step = 1 ;
				digit = 0;
				draw3_digit_pressed (data_3112, units[UNIT_NONE], cur_value , lower, upper, step, digit, NULL, pos, 18);
			}
		}
		else
		{
			if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
			{
				cur_value = LAW_VAL_POS(grp, Angle_min)/100.0 + TMP(beam_num[grp]) * LAW_VAL_POS(grp, Angle_step)/100.0;
				unit = UNIT_DEG;
				digit = 1;
				draw3_digit_stop (cur_value, units[unit], digit, pos, 12);
			}
			else
			{
				cur_value = TMP(beam_num[grp]) + 1.0;
				digit = 0;
				draw3_digit_stop (cur_value, units[UNIT_NONE], digit, pos, 18);
			}

		}
	}

	else if (_nSelection==4||_nSelection==5||_nSelection==6)
	{
		/* 当前步进 */
		switch (TMP(cursors_angle_reg))
		{
			case 0:	tmpf = 1.0; break;
			case 1:	tmpf = 10.0; break;
			default:break;
		}
		if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
		{

			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
			{
				cur_value = GROUP_CURSORS_POS(angle)/100.0;
				lower = -30.0;
				upper = 40.0;
				step = tmpf;
				digit = 1;
				pos = 1;
				unit = UNIT_DEG;
				draw3_digit_pressed (data_3113, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 12);
			}
			else
			{
				cur_value = GROUP_CURSORS_POS(angle)/100.0;
				digit = 1;
				pos = 1;
				unit = UNIT_DEG;
				draw3_digit_stop (cur_value, units[unit], digit, pos, 12);
			}
		}
		else//Linear scan
		{
			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
			{
				cur_value = GROUP_CURSORS_POS(angle)/100.0;
				lower = 1;
				upper = 512;
				step = tmpf;
				digit = 1;
				pos = 1;
				unit = UNIT_NONE;
				draw3_digit_pressed (data_3113, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 18);//VPA
			}
			else
			{
				cur_value = GROUP_CURSORS_POS(angle)/100.0;
				digit = 1;
				pos = 1;
				unit = UNIT_NONE;
				draw3_digit_stop (cur_value, units[unit], digit, pos, 18);

			}

		}
	}
	gtk_widget_queue_draw(pp->draw_area->drawing_area);

}
void DrawMenu321()
{
	draw3_popdown(menu_content[OFF_ON + get_entry_image(pp->p_config)],1,0);
	if(get_display_table(pp->p_config))
	{
		gtk_widget_set_sensitive(pp->eventbox30[1] ,FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[1] ,FALSE);
	}
}
void DrawMenu331()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower, upper, step;
	int digit, pos, unit;
	int grp = get_current_group(pp->p_config);
	/* 当前步进 */
	switch (TMP(min_thickness_reg))
	{
		case 0:	tmpf = 1; break;
		case 1:	tmpf = 10; break;
		case 2:	tmpf = 0.1; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
	{
		if (get_unit(pp->p_config)==UNIT_MM)
		{
			cur_value = GROUP_VAL_POS(grp , min_thickness) /1000.0 ;
			lower = 0.05;
			upper = GROUP_VAL_POS(grp , max_thickness) /1000.0 - 0.01;
			step = tmpf;
			digit = 2;
			pos = 1;
			unit = UNIT_MM;
		}
		else
		{
			cur_value = GROUP_VAL_POS(grp , min_thickness) /1000.0 * 0.03937;
			lower = 0.002;
			upper = (GROUP_VAL_POS(grp , max_thickness) /1000.0) * 0.03937 - 0.005;
			step = tmpf/2.0;
			digit = 3;
			pos = 1;
			unit = UNIT_INCH;
		}

		draw3_digit_pressed (data_331, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 0);
	}
	else
	{
		if (get_unit(pp->p_config)==UNIT_MM)
		{
			cur_value = GROUP_VAL_POS(grp , min_thickness) /1000.0 ;
			digit = 2;
			pos = 1;
			unit = UNIT_MM;
		}
		else
		{
			cur_value = GROUP_VAL_POS(grp , min_thickness) /1000.0 * 0.03937;
			digit = 3;
			pos = 1;
			unit = UNIT_INCH;
		}
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
	}
}
void DrawMenu341()
{
	if ( !con2_p[3][4][1] )
		gtk_widget_hide (pp->eventbox30[1]);
	gtk_widget_hide (pp->eventbox31[1]);
}

void DrawMenu401()
{
	char *str;
	int  menu_status = 0;
	pp->x_pos = 593, pp->y_pos = 175;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))/*选中401这个位置*/
	{
		if(get_display_pos(pp->p_config) == A_SCAN ||
				get_display_pos(pp->p_config) == B_SCAN ||
				get_display_pos(pp->p_config) == S_SCAN ||
				get_display_pos(pp->p_config) == A_B_SCAN ||
				get_display_pos(pp->p_config) == PA_TOFD ||
				get_display_pos(pp->p_config) == PA_TOFD_CSCAN)
		{
			if (get_group_qty(pp->p_config) == 1)
				menu_status = 0x01;
			draw3_pop_tt (data_401, NULL,
					menu_content[GROUP_POS + get_display_group(pp->p_config)],
					menu_content + GROUP_POS, 2, 1, get_display_group(pp->p_config), menu_status);
		}
		else if ((get_display_pos(pp->p_config) == C_SCAN) ||
				(get_display_pos(pp->p_config) == A_B_C_SCAN) ||
				(get_display_pos(pp->p_config) == A_C_CC_SCAN) ||
				(get_display_pos(pp->p_config) == PA_TOFD_CSCAN))
		{
			draw3_pop_tt (data_4011, NULL,
					menu_content[C_SCAN1 + get_cscan_source(pp->p_config, 0)],
					menu_content + C_SCAN1, 4, 1, get_cscan_source(pp->p_config, 0), 0);
			str = g_strdup_printf ("%s", con2_p[4][0][6]);
			gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);
		}
		else if (get_display_pos(pp->p_config) == A_S_CC_SCAN)
		{
			draw3_pop_tt (data_4012, NULL,
					menu_content[C_SCAN1+get_cscan_source(pp->p_config, 0)],
					menu_content + C_SCAN1, 5, 1, get_cscan_source(pp->p_config, 0), 0);
			str = g_strdup_printf ("%s", con2_p[4][0][6]);
			gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);
		}
		else if(get_display_pos(pp->p_config)==Strip_Chart_AA)
			/*Display 为 Strip Chart-[A]*/
		{
			draw3_pop_tt (data_4013, NULL,
					menu_content[C_SCAN1+get_stripscan_data1(pp->p_config)],
					menu_content + C_SCAN1, 3, 1, get_stripscan_data1(pp->p_config), 0);
			str = g_strdup_printf ("%s", con2_p[4][0][8]);
			gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);

		}
		//else if(get_display_pos(pp->p_config)==1 || get_display_pos(pp->p_config)==4 || get_display_pos(pp->p_config)==6)
		else {
			gtk_widget_hide (pp->eventbox30[1]);
			gtk_widget_hide (pp->eventbox31[1]);
		}
	}
	else
	{
		if(get_display_pos(pp->p_config) == A_SCAN ||
				get_display_pos(pp->p_config) == B_SCAN ||
                get_display_pos(pp->p_config) == S_SCAN ||
				get_display_pos(pp->p_config) == A_B_SCAN ||
				get_display_pos(pp->p_config) == PA_TOFD ||
				get_display_pos(pp->p_config) == PA_TOFD_CSCAN)
		{
			draw3_popdown (menu_content[GROUP_POS+get_display_group(pp->p_config)], 1, 0);
		}
		else if ((get_display_pos(pp->p_config) == C_SCAN) ||
				(get_display_pos(pp->p_config) == A_B_C_SCAN) ||
				(get_display_pos(pp->p_config) == A_C_CC_SCAN) ||
                (get_display_pos(pp->p_config)==PA_TOFD_CSCAN))
		{
			draw3_popdown (menu_content[C_SCAN1+get_cscan_source(pp->p_config, 0)], 1, 0);
			str = g_strdup_printf ("%s", con2_p[4][0][6]);
			gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);
		}
		else if(get_display_pos(pp->p_config)==A_S_CC_SCAN)
		{
			draw3_popdown (menu_content[C_SCAN1+get_cscan_source(pp->p_config, 0)], 1, 0);
			str = g_strdup_printf ("%s", con2_p[4][0][6]);
			gtk_label_set_text (GTK_LABEL (pp->label3[1]), str);
		}
		else if(get_display_pos(pp->p_config)==B_SCAN ||
				get_display_pos(pp->p_config)==A_B_SCAN ||
				get_display_pos(pp->p_config)==A_B_S_SCAN)
		{
			gtk_widget_hide (pp->eventbox30[1]);
			gtk_widget_hide (pp->eventbox31[1]);
		}
	}
}
void DrawMenu411()
{
	pp->x_pos = 600, pp->y_pos = 175;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
		draw3_pop_tt (data_411, NULL,
				menu_content[GRID + get_overlay_grid(pp->p_config)],
				menu_content + GRID, 6, 1, get_overlay_grid(pp->p_config), 0);
	else
		draw3_popdown (menu_content[GRID + get_overlay_grid(pp->p_config)], 1, 0);
}

void DrawMenu421()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower, upper, step;
	int digit, pos, unit;
	int grp = get_current_group (pp->p_config);
	if(GROUP_VAL_POS( grp , col_select_pos) == 0)
	{
		/* 当前步进 */
		switch (TMP(color_start_reg))
		{
			case 0:	tmpf = 1.0; break;
			case 1:	tmpf = 10.0; break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
		{
			cur_value = GROUP_VAL_POS( grp , col_start);
			lower = 0.0;
			upper = GROUP_VAL_POS( grp , col_end)-1.0;
			step = tmpf;
			digit = 0;
			pos = 1;
			unit = UNIT_BFH;
			draw3_digit_pressed (data_431, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 0);
		}
		else
		{
			cur_value = GROUP_VAL_POS( grp , col_start);
			digit = 0;
			pos = 1;
			unit = UNIT_BFH;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		}
	}

	else if(GROUP_VAL_POS( grp , col_select_pos) == 1)
	{
		/* 当前步进 */
		switch (TMP(color_contrast_reg))
		{
			case 0:	tmpf = 1.0; break;
			case 1:	tmpf = 10.0; break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
		{
			cur_value = GROUP_VAL_POS( grp , col_contrast);
			lower = 0.0;
			upper = 99.0;
			step = tmpf;
			digit = 0;
			pos = 1;
			unit = UNIT_BFH;
			draw3_digit_pressed (data_4311, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 6);
		}
		else
		{
			cur_value = GROUP_VAL_POS( grp , col_contrast);
			digit = 0;
			pos = 1;
			unit = UNIT_BFH;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 6);
		}

	}
	else if (GROUP_VAL_POS( grp , col_select_pos) == 2)
	{
		/* 当前步进 */
		switch (TMP(min_reg))
		{
			case 0:	tmpf =   1 ; break;
			case 1:	tmpf =  10 ; break;
			case 2:	tmpf =  0.1; break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
		{
			cur_value = GROUP_VAL_POS( grp , col_min)/1000.0;
			lower = 0.05;
			upper = GROUP_VAL_POS( grp , col_max)/1000.0-0.01;
			step = tmpf;
			digit = 2;
			pos = 1;
			unit = UNIT_MM;
			draw3_digit_pressed (data_4312, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 8);
		}
		else
		{
			cur_value = GROUP_VAL_POS( grp , col_min)/1000.0;
			digit = 2;
			pos = 1;
			unit = UNIT_MM;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 8);
		}
	}
	gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
	gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
}
void DrawMenu431()
{
	char temp[52];
	double tmpf = 0.0;
	double cur_value=0.0, lower, upper, step;
	int digit, pos, unit;
	int grp = get_current_group (pp->p_config);

	switch(get_dis_prop_scan(pp->p_config))
		{
			case 0:	/* properties scan 选择Ascan时候 */
				pp->x_pos = 598, pp->y_pos = 175;
				if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
				{
					draw3_pop_tt (data_441, NULL,
							menu_content[PROP_COLOR + GROUP_VAL_POS( grp , ascan_color)],
							menu_content+PROP_COLOR, 6, 1, GROUP_VAL_POS( grp , ascan_color), 0x10);
				}
				else
				{
					draw3_popdown (menu_content[PROP_COLOR + GROUP_VAL_POS( grp , ascan_color)], 1, 0);
				}
				break;
			case 1:
				switch (pp->p_tmp_config->compress_reg)
				{
					case 0:	tmpf =  1  ; break;
					case 1:	tmpf =  10 ; break;
					case 2:	tmpf =  0.1; break;
					default:break;
				}
				if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
				{
					cur_value = get_b_compress (pp->p_config) / 1000.0;
					lower = 0.10;
					upper = 65536.0;
					step = tmpf;
					digit = 2;
					pos = 1;
					unit = UNIT_NONE;
					draw3_digit_pressed (data_4411, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 6);
				}
				else
				{
					cur_value = get_b_compress(pp->p_config) / 1000.0;
					digit = 2;
					pos = 1;
					unit = UNIT_NONE;
					draw3_digit_stop (cur_value, units[unit], digit, pos, 6);
				}
				break;
			case 2:
				draw3_popdown(menu_content[OFF_ON + get_dis_prop_cratio(pp->p_config)],1,0);
				g_sprintf (temp,"%s", con2_p[4][3][8]);
				gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
				break;
			case 3:
				pp->x_pos = 600, pp->y_pos = 210;
				if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
					draw3_pop_tt (data_4414, NULL,
							menu_content[FFT_COLOR+get_fft_color (pp->p_config)],
							menu_content+FFT_COLOR, 4, 1, get_fft_color (pp->p_config), 0x08);
				else
					draw3_popdown (menu_content[FFT_COLOR+get_fft_color (pp->p_config)], 1, 0);
				g_sprintf (temp,"%s", con2_p[4][3][10]);
				gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);
				break;
			default:break;
		}
}
void DrawMenu441()
{
	if ( !con2_p[4][4][1] )
		gtk_widget_hide (pp->eventbox30[1]);
	gtk_widget_hide (pp->eventbox31[1]);
}

void DrawMenu501()
{
	int grp = get_current_group (pp->p_config);
#if HIGH_POWER
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
	{
		data_501(NULL ,(gpointer)UT1_SCAN);
	}

	draw3_popdown (menu_content[GROUP_MODE_P + GROUP_VAL_POS( grp , group_mode)], 1, 0);
#else
	pp->x_pos = 454, pp->y_pos = 203-26;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
		draw3_pop_tt (data_501, NULL,
				menu_content[GROUP_MODE_P + GROUP_VAL_POS( grp , group_mode)],
				menu_content + GROUP_MODE, 4, 1, GROUP_VAL_POS( grp , group_mode), 0);
	else
		draw3_popdown (menu_content[GROUP_MODE_P + GROUP_VAL_POS( grp , group_mode)], 1, 0);
#endif
}
void DrawMenu511()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower, upper, step;
	int digit, pos, unit;
	int grp = get_current_group (pp->p_config);

	switch (TMP(indexoffset_reg))
	{
		case 0:	tmpf = 1; break;
		case 1:	tmpf = 5; break;
		case 2:	tmpf = 0.1; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
	{
		if(UNIT_MM == get_unit(pp->p_config))
		{
			cur_value = GROUP_VAL_POS( grp , index_offset)/10.0;
			lower = -10000.0;
			upper =  99999.0;
			step = tmpf;
			digit = 1;
			pos = 1;
			unit = UNIT_MM;
		}
		else
		{
			cur_value = GROUP_VAL_POS( grp , index_offset)/10.0*0.03937;
			lower = -10000.0*0.03937;
			upper =  99999.0*0.03937;
			step = tmpf/10.0;
			digit = 1;
			pos = 1;
			unit = UNIT_INCH;
		}
		draw3_digit_pressed (data_511, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 0);
	}
	else
	{
		if(UNIT_MM == get_unit(pp->p_config))
		{
			cur_value =GROUP_VAL_POS( grp , index_offset)/10.0;
			digit = 1;
			pos = 1;
			unit = UNIT_MM;
		}
		else
		{
			cur_value =GROUP_VAL_POS( grp , index_offset)/10.0*0.03937;
			digit = 1;
			pos = 1;
			unit = UNIT_INCH;
		}
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
	}
}
void DrawMenu521()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower, upper, step;
	int digit, pos, unit , content_pos;
	int grp = get_current_group (pp->p_config);
	GROUP* p_grp = get_group_by_id (pp->p_config, grp) ;
	/* 当前步进 */
	switch (TMP(db_reg))
	{
		case 0:	tmpf = 0.1; break;
		case 1:	tmpf = 0.5; break;
		case 2:	tmpf = 1.0; break;
		case 3:	tmpf = 2.0; break;
		case 4:	tmpf = 6.0; break;
		default:break;
	}
	if (get_group_db_ref (pp->p_config, grp))
		content_pos = 6;
	else
		content_pos = 0;

	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
	{
		cur_value = ((int)(get_group_val (p_grp, GROUP_GAIN)) - (int)(get_group_val (p_grp, GROUP_GAINR) * get_group_db_ref (pp->p_config, grp))) / 100.0;
		lower = 0.0 - (int)(get_group_val (p_grp, GROUP_GAINR)) * get_group_db_ref (pp->p_config, grp) / 100.0 ;
		upper = GAIN_MAX - get_group_val (p_grp, GROUP_GAINR) * get_group_db_ref (pp->p_config, grp) / 100.0 ;
		step = tmpf;
		digit = 1;
		pos = 1;
		unit = UNIT_DB;
		draw3_digit_pressed (data_100, units[unit], cur_value , lower, upper, step, digit, NULL, pos, content_pos);
	}
	else
	{
		cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAIN) - get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_GAINR) * get_group_db_ref (pp->p_config, get_current_group (pp->p_config))) / 100.0;
		digit = 1;
		pos = 1;
		unit = UNIT_DB;
		draw3_digit_stop (cur_value, units[unit], digit, pos, content_pos);
	}
}
void DrawMenu531()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower, upper, step;
	int digit, pos, unit ;
	/* 当前步进 */
	int grp = get_current_group(pp->p_config);
	switch (TMP(part_thickness_reg))
	{
		case 0:	tmpf = 1; break;
		case 1:	tmpf = 10; break;
		case 2:	tmpf = 0.1; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
	{
		if(UNIT_MM == get_unit(pp->p_config))
		{
			cur_value = GROUP_VAL_POS(grp , part.Thickness) / 1000.0;
			lower = 0.05;
			if (get_part_geometry(grp) == PLATE_PART)
				upper = 1000.0;
			else if (get_part_geometry(grp) == OD_PART)
				upper = get_part_diameter (grp) / 2000.0;
			else
				upper = (get_part_diameter (grp) - 10.0) / 2000.0;
			step = tmpf;
			digit = 2;
			pos = 1;
			unit = UNIT_MM;
		}
		else
		{
			cur_value = GROUP_VAL_POS(grp , part.Thickness) / 1000.0*0.03937;
			lower = 0.002;
			if (get_part_geometry(grp) == PLATE_PART)
				upper = 1000.0 *0.03937 ;
			else if (get_part_geometry(grp) == OD_PART)
				upper = get_part_diameter (grp) / 2000.0 *0.03937 ;
			else
				upper = (get_part_diameter (grp) - 10.0) / 2000.0 *0.03937;
			step = tmpf*0.03937;
			digit = 3;
			pos = 1;
			unit = UNIT_INCH;
		}
		draw3_digit_pressed (data_531, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 0);
	}
	else
	{
		if(UNIT_MM == get_unit(pp->p_config))
		{
			cur_value = GROUP_VAL_POS(grp , part.Thickness) / 1000.0;
			digit = 2;
			pos = 1;
			unit = UNIT_MM;
		}
		else
		{
			cur_value = GROUP_VAL_POS(grp , part.Thickness) / 1000.0*0.03937;
			digit = 3;
			pos = 1;
			unit = UNIT_INCH;
		}
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
	}
}

void DrawMenu541()
{
    if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1)) {
		pp->pos_pos = MENU3_STOP;
		draw3_popdown(NULL,1,1);
        workpiece_clean() ;
		gtk_widget_queue_draw(pp->vboxtable)  ;
    } else {
        draw3_popdown(NULL,1,1);
    }

}

void DrawMenu601()
{
	double tmpf = 0.0;
	char *str ;
	double cur_value=0.0, lower, upper, step;
	int digit, pos ;
	int grp = get_current_group(pp->p_config);
	GROUP* p_grp = get_group_by_id(pp->p_config , grp);
	switch (TMP(pulser_reg))
	{
		case 0:	tmpf = 1.0; break;
		case 1:	tmpf = 10.0; break;
		case 2:	tmpf = 100.0; break;
		default:break;
	}
	upper = (double)getPulserReceiverRange (grp);
	if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 1))
	{
		cur_value = (double) (get_group_val (p_grp, GROUP_PULSER));
		lower = 1.0;
		step = tmpf;
		digit = 0;
		pos = 1;
		str = g_strdup_printf ("( 1 to %d)", (unsigned int)(upper));
		draw3_digit_pressed (data_110, str, cur_value , lower, upper, step, digit, NULL ,  pos, 0);
	}
	else
	{
		cur_value = (double) (get_group_val (p_grp, GROUP_PULSER));
		digit = 0;
		pos = 1;
		str = g_strdup_printf ("( 1 to %d)", (unsigned int)(upper));
		draw3_digit_stop (cur_value, str, digit, pos, 0);
	}
	g_free(str);
}
void DrawMenu611()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower, upper, step;
	int digit, pos, unit, temp_beam;
	int grp = get_current_group(pp->p_config);

	switch (TMP(max_angle_reg))
	{
		case 0:	tmpf = 0.1; break;
		case 1:	tmpf = 10.0; break;
		case 2:	tmpf = 1.0; break;
		default:break;
	}
	if ((LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN) &&
			(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON))
		/* 聚焦法则自动计算开启时并且是角度扫查, Max Angle 才可调节 */
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
		{
			/* 计算lower为妙 */
			cur_value = LAW_VAL_POS(grp , Angle_max) / 100.0;
			temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[get_current_group(pp->p_config)]);
			TMP(beam_skew_num)	= (LAW_VAL_POS(grp , Angle_beam_skew_max) -
					LAW_VAL_POS(grp , Angle_beam_skew_min)) /
				LAW_VAL_POS(grp , Angle_beam_skew_step) + 1;
			temp_beam = temp_beam / TMP(beam_skew_num);
			//lower = LAW_VAL_POS(grp , Angle_min) / 100.0;
			//upper = MIN ((gint)(LAW_VAL_POS(grp , Angle_min) +
			//			(gint)(temp_beam * LAW_VAL_POS(grp , Angle_step))) / 100.0, 89);
            RefractAngleRange(&lower, &upper, get_group_by_id (pp->p_config, grp), 0);
			step = tmpf;
			digit = 1;
			pos = 1;
			unit = UNIT_DEG;
			draw3_digit_pressed (data_611, units[unit], cur_value , lower, upper,
					step, digit, NULL, pos, 0);
		}
		else
		{
			cur_value = LAW_VAL_POS(grp , Angle_max) / 100.0;
			digit = 1;
			pos = 1;
			unit = UNIT_DEG;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		}
	}
	else
	{
		cur_value = LAW_VAL_POS(grp , Angle_max) / 100.0;
		digit = 1;
		pos = 1;
		unit = UNIT_DEG;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
	}
}
void DrawMenu621()
{
	/* P621
   0 Halfpath  时候 这里设置positon start
   1 TrueDepth 时候 这里设置positon start
   2 Halfpath  时候 这里设置offset  start
   3 Halfpath  时候 这里设置offset  start
   4 Automatic 时候 不能设置 */
	double tmpf = 0.0;
	double cur_value=0.0, lower, upper, step;
	int digit, pos, unit;
	int grp = get_current_group(pp->p_config);

	float multiplier;
	if(UNIT_MM == get_unit(pp->p_config))
	{
		multiplier = 1.0;
	}
	else
	{
		multiplier = 0.03937;
	}

	if(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON)
		/* 聚焦法则自动计算开启时, Max Angle 才可调节 */
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
		{
			if((LAW_VAL_POS(grp , Focal_point_type)==HALFPATH_P)||(LAW_VAL_POS(grp , Focal_point_type)==DEPTH_P))
				/*type为half path 或 true depth 时*/
			{
				switch (TMP(positions_reg))
				{
					case 0:	tmpf = 1; break;
					case 1:	tmpf = 5; break;
					case 2:	tmpf = 10.0; break;
					default:break;
				}
				if(UNIT_MM == get_unit(pp->p_config))
				{
					unit = UNIT_MM;
					digit = 1;
				}else{
					unit = UNIT_INCH;
					digit = 2;
				}
				cur_value = LAW_VAL_POS(grp , Position_start) / 1000.0 * multiplier;
                RefractAngleRange(&lower, &upper, get_group_by_id (pp->p_config, grp), 1);
				step = tmpf;
				pos = 1;
			    if (LAW_VAL_POS(grp , Focal_point_type)==HALFPATH_P) {
				    draw3_digit_pressed (data_621, units[unit], cur_value , lower, upper,step, digit, NULL, pos, 9);
                } else {
				    draw3_digit_pressed (data_621, units[unit], cur_value , lower, upper,step, digit, NULL, pos, 0);
                }
			}
			else if((LAW_VAL_POS(grp , Focal_point_type)==PROJECTION_P)||(LAW_VAL_POS(grp , Focal_point_type)==FOCALPLANE_P))
			{	/*type 为 projection  或 focal plane 时*/
				switch (TMP(offsets_reg))
				{
					case 0:	tmpf = 0.5; break;
					case 1:	tmpf = 5.0; break;
					case 2:	tmpf = 50.0; break;
					default:break;
				}
				if(UNIT_MM == get_unit(pp->p_config))
				{
					unit = UNIT_MM;
					digit = 2;
				}else{
					unit = UNIT_INCH;
					digit = 3;
				}
				cur_value = LAW_VAL_POS(grp , Offset_start) / 1000.0 * multiplier;
				lower = -10000 * multiplier;
				upper = 10000.0 * multiplier;
				step = tmpf;
				pos = 1;

				draw3_digit_pressed (data_6211, units[unit], cur_value , lower, upper,
						step, digit, NULL , pos, 6);
			}
			else
			{
				if(UNIT_MM == get_unit(pp->p_config))
				{
					unit = UNIT_MM;
					digit = 1;
				}else{
					unit = UNIT_INCH;
					digit = 2;
				}
				cur_value = LAW_VAL_POS(grp , Position_start) / 1000.0 * multiplier;
				pos = 1;
				draw3_digit_stop (cur_value, units[unit], digit, pos, 0);

				gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
				gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
			}

		}
		else
		{
			if((LAW_VAL_POS(grp , Focal_point_type)==HALFPATH_P)||(LAW_VAL_POS(grp , Focal_point_type)==DEPTH_P))
				/*type为half path 或 true depth 时*/
			{
				if(UNIT_MM == get_unit(pp->p_config))
				{
					unit = UNIT_MM;
					digit = 1;
				}else{
					unit = UNIT_INCH;
					digit = 2;
				}
				cur_value = LAW_VAL_POS(grp , Position_start) / 1000.0 * multiplier;
				pos = 1;
			    if(LAW_VAL_POS(grp , Focal_point_type)==HALFPATH_P) {
				    draw3_digit_stop (cur_value, units[unit], digit, pos, 9);
                } else {
				    draw3_digit_stop (cur_value, units[unit], digit, pos, 1);
                }
			}
			else if((LAW_VAL_POS(grp , Focal_point_type)==PROJECTION_P)||(LAW_VAL_POS(grp , Focal_point_type)==FOCALPLANE_P))
				/*type 为 projection  或 focal plane 时*/
			{
				if(UNIT_MM == get_unit(pp->p_config))
				{
					unit = UNIT_MM;
					digit = 2;
				}else{
					unit = UNIT_INCH;
					digit = 3;
				}
				cur_value = LAW_VAL_POS(grp , Offset_start) / 1000.0 * multiplier;
				pos = 1;
				draw3_digit_stop (cur_value, units[unit], digit, pos, 6);
			}
			else
			{
				if(UNIT_MM == get_unit(pp->p_config))
				{
					unit = UNIT_MM;
					digit = 1;
				}else{
					unit = UNIT_INCH;
					digit = 2;
				}
				cur_value = LAW_VAL_POS(grp , Position_start) / 1000.0 * multiplier;
				pos = 1;
				draw3_digit_stop (cur_value, units[unit], digit, pos, 0);

				gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
				gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
			}
		}
	}
	else
	{
		if((LAW_VAL_POS(grp , Focal_point_type)==HALFPATH_P)||(LAW_VAL_POS(grp , Focal_point_type)==DEPTH_P))
		{
			if(UNIT_MM == get_unit(pp->p_config))
			{
				unit = UNIT_MM;
				digit = 1;
			}else{
				unit = UNIT_INCH;
				digit = 2;
			}
			cur_value = LAW_VAL_POS(grp , Position_start) / 1000.0 * multiplier;
			pos = 1;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		}
		else if((LAW_VAL_POS(grp , Focal_point_type)==PROJECTION_P)||(LAW_VAL_POS(grp , Focal_point_type)==FOCALPLANE_P))
		{
			if(UNIT_MM == get_unit(pp->p_config))
			{
				unit = UNIT_MM;
				digit = 2;
			}else{
				unit = UNIT_INCH;
				digit = 3;
			}
			cur_value = LAW_VAL_POS(grp , Offset_start) / 1000.0 * multiplier;
			pos = 1;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 6);
		}
		gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
	}
}

void DrawMenu631()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower, upper, step;
	int digit, pos, unit;
	int grp = get_current_group(pp->p_config);

	switch (TMP(first_element_reg))
	{
		case 0:	tmpf = 1.0; break;
		case 1:	tmpf = 10.0; break;
		case 2:	tmpf = 100.0; break;
		default:break;
	}
	if(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON)/* 聚焦法则自动计算开启时, First Element才可调节 */
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
		{
			cur_value = LAW_VAL_POS(grp , First_tx_elem);
			lower = 1.0;
			/* 计算最大值 */
			//upper = (double)MIN((GROUP_VAL_POS( grp , probe.Elem_qty) - LAW_VAL_POS(grp , Elem_qty) + 1),
			//		(gMaxElementRecieveQty + 1 - GROUP_VAL_POS(grp , pulser1)));
			upper = (double)(GROUP_VAL_POS( grp , probe.Elem_qty) - LAW_VAL_POS(grp , Elem_qty) + 1) ;
			upper = MIN(upper , gMaxElementRecieveQty + 1 - LAW_VAL_POS(grp , Elem_qty)) ;
			step  = tmpf;
			digit = 0;
			pos   = 1;
			unit = UNIT_NONE;
			draw3_digit_pressed (data_631, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 0);
		}
		else
		{
			cur_value = LAW_VAL_POS(grp , First_tx_elem);
			digit = 0;
			pos = 1;
			unit = UNIT_NONE;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		}
	}
	else /* 聚焦法则自动计算为off时, First Element 不可以调节 */
	{
		cur_value = LAW_VAL_POS(grp , First_tx_elem);
		digit = 0;
		pos = 1;
		unit = UNIT_NONE;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
	}
}

void DrawMenu641()
{
#if 0
	if (get_auto_focal (pp->p_config) == AUTO_FOCAL_ON )
	{
		draw3_popdown(NULL, 1, 1);
		gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
	}
	else
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
		{
			draw_dialog_all (DIALOG_LAW_READ);
		}
		else
			draw3_popdown(NULL, 1, 1);
	}
#endif
	draw3_popdown(NULL, 1, 1);
	gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
	gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
}

void DrawMenu701()
{
	unsigned char _nCurrentEcoderType ;
	unsigned char _nInspecSource = (int)get_inspec_source (pp->p_config) - 1;
	if(_nInspecSource != 0 && _nInspecSource != 1)
	{
		_nCurrentEcoderType  = 0 ;
	}
	else
	{
		_nCurrentEcoderType  = get_enc_type (pp->p_config,  _nInspecSource);
		_nCurrentEcoderType  = (_nCurrentEcoderType >> 3)  & 1 ;
	}

	pp->x_pos = 596, pp->y_pos = 204-26;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
		draw3_pop_tt (data_701, NULL,
				menu_content[POLARITY + _nCurrentEcoderType],
				menu_content + POLARITY, 2, 1, _nCurrentEcoderType, 0);
	else
		draw3_popdown (menu_content[POLARITY + _nCurrentEcoderType ], 1, 0);

	if (get_inspec_source (pp->p_config) == 0)
	{
		gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
	}

}

void DrawMenu711()
{
	pp->x_pos = 580, pp->y_pos = 201-26;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
	{
		//if(get_inspec_type (pp->p_config)==1)
		//	draw3_pop_tt (data_711, NULL,
		//			menu_content[I_SCAN+get_inspec_source (pp->p_config)],
		//			menu_content+I_SCAN, 3, 1, get_inspec_source (pp->p_config), 0x01);
		//else
		draw3_pop_tt (data_711, NULL,
					menu_content[I_SCAN+get_inspec_source (pp->p_config)],
					menu_content+I_SCAN, 3, 1, get_inspec_source (pp->p_config), 0x4);
	}
	else
		draw3_popdown (menu_content[I_SCAN+get_inspec_source (pp->p_config)], 1, 0);

}
void DrawMenu721()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower, upper, step;
	int digit, pos, unit;
	/* 当前步进 */
	switch (TMP(scan_end_reg))
	{
		case 0:	tmpf = 0.01 * get_area_scanresolution (pp->p_config); break;
		case 1:	tmpf = 0.1 * get_area_scanresolution (pp->p_config); break;
		case 2:	tmpf = 0.001 * get_area_scanresolution (pp->p_config); break;
		default:break;
	}

	if(get_inspec_type (pp->p_config)==0 || get_inspec_type (pp->p_config)==1)
		/* Inspection -> Type 选择 One-Line Scan \ Raster Scan  时 */
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
		{
			if(get_unit(pp->p_config) == UNIT_MM)
			{
				cur_value = get_area_scanend (pp->p_config)/1000.0;
				upper  = 9999.0;
				step   = tmpf;
                lower  = get_area_scanstart (pp->p_config)/1000.0+1;
				digit  = 2;
				pos    = 1;
				unit   = UNIT_MM;
			}
			else
			{
				cur_value = get_area_scanend (pp->p_config)/1000.0 * 0.03937;
				upper     = 9999.0;
				step      = tmpf*0.03937;
                lower     = get_area_scanstart (pp->p_config)/1000.0 * 0.03937+0.03937;

				digit     = 3;
				pos = 1;
				unit = UNIT_INCH;
			}
			draw3_digit_pressed (data_721, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 0);
		}
		else
		{
			if(get_unit(pp->p_config) == UNIT_MM)
			{
				cur_value = get_area_scanend (pp->p_config)/1000.0;
				digit = 2;
				pos = 1;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = get_area_scanend (pp->p_config)/1000.0*0.03937;
				digit = 3;
				pos = 1;
				unit = UNIT_INCH;
			}
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		}
	}
	else  /* Inspection -> Type 选择 Helicoidal Scan  时  ????get_area_scanstart (pp->p_config)的值有待确定 */
	{
		if(get_unit(pp->p_config) == UNIT_MM)
		{
			//set_area_scanstart (pp->p_config, 346);
			cur_value = get_area_scanend (pp->p_config)/1000.0;
			digit = 2;
			pos = 1;
			unit = UNIT_MM;
		}
		else
		{
			cur_value = get_area_scanend (pp->p_config)/1000.0*0.03937;
			//set_area_scanstart (pp->p_config, 346.0*0.03937);
			digit = 3;
			pos = 1;
			unit = UNIT_INCH;
		}
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);

		gtk_widget_set_sensitive(pp->eventbox30[1],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[1],FALSE);
	}
}
void DrawMenu731()
{
	draw3_popdown (menu_content[OFF_ON + get_start_pause (pp->p_config)], 1, 0);
}
void DrawMenu741()
{
	pp->x_pos = 545, pp->y_pos = 202;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
		draw3_pop_tt (data_741, NULL,
				menu_content[INSPEC_DATA + get_data_inspec_data (pp->p_config)],
				menu_content + INSPEC_DATA, 2, 1, get_data_inspec_data (pp->p_config), 0);
	else
		draw3_popdown (menu_content[INSPEC_DATA + get_data_inspec_data (pp->p_config)], 1, 0);
}

extern GtkWidget* fileOpenNew(GtkWidget* fatherWidget);
void DrawMenu801()
{
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
	{
//		draw_dialog_all (DIALOG_FILE_OPEN);
		GtkWidget* w = fileOpenNew(pp->window);
		gtk_dialog_run(GTK_DIALOG(w));
	//	gtk_widget_show(w);
		pp->pos_pos = MENU3_STOP;
		draw3_popdown(NULL,1,1);
	}
	else
		draw3_popdown(NULL,1,1);
}
void DrawMenu811()
{
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
	{
	//	draw_dialog_all (DIALOG_FILE_NAME);
		char* str = NULL;
        unsigned int len, len2;
		str = dialogGetAString( pp->window
							    ,getDictString(_STRING_File_Name)
		                        ,get_file_name_info(pp->p_config));
		if(str)
		{
            len2 = 0;
            len = 0;
            while (str[len] != '\0') {
                if (str[len] != '/') {
                    str[len2] = str[len];
                    len2++;
                }
                len++;
            }
            str[len2] = '\0';

			set_file_name_info(pp->p_config ,str);
			free(str);
		}
		draw3_popdown ( get_file_name_info(pp->p_config) ,1 ,0);
	}
	else
	{
		draw3_popdown (get_file_name_info(pp->p_config) ,1 ,0);
	}
}
void DrawMenu821()
{
	draw3_popdown (menu_content[OFF_ON + get_report_format_probe (pp->p_config)], 1, 0);
}
void DrawMenu831()
{
	draw3_popdown (menu_content[OFF_ON + get_report_userfield_enable (pp->p_config, get_report_userfield_select (pp->p_config))], 1, 0);

}
void DrawMenu841()
{
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
	{
		//draw_dialog_all (DIALOG_EDIT_HEADER);
		char* str = NULL;
		str = dialogGetAString( pp->window
							    ,con2_p[8][4][1]
		                        ,get_header_info(pp->p_config));
		if(str)
		{
			set_header_info(pp->p_config, str);
			free(str);
		}
	}
	else
		draw3_popdown(NULL,1,1);
}

void DrawMenu901()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower, upper, step;
	int digit, pos, unit;
	/* 当前步进 */
	switch (pp->p_tmp_config->bright_reg)
	{
		case 0:	tmpf = 1.0; break;
		case 1:	tmpf = 10.0; break;
		case 2:	tmpf = 100.0; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
	{
		cur_value = get_bright (pp->p_config);
		lower = 1.0;
		upper = 100.0;
		step = tmpf;
		digit = 0;
		pos = 1;
		unit = UNIT_BFH;
		draw3_digit_pressed (data_901, units[unit], cur_value , lower, upper, step, digit, NULL , pos, 0);
	}
	else
	{
		cur_value = get_bright (pp->p_config);
		digit = 0;
		pos = 1;
		unit = UNIT_BFH;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
	}
}
void DrawMenu911()
{
	char temp[52];
	char buffer[32] ;
	time_t curtime;
	struct tm *loctime;
	curtime = time(NULL);
	loctime = localtime(&curtime);
	strftime (buffer, 32, "%F %T", loctime);

	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
	{
		draw_dialog_all (DIALOG_DATE);
	}
	else
	{
		memcpy (temp, buffer, 11);
		temp[10] = 0;
		draw3_popdown (temp, 1, 0);
	}
}

extern GtkWidget* fileManageNew(GtkWidget* fatherWidget);
void DrawMenu921()
{
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
	{
	//	draw_dialog_all(DIALOG_FILE_MANAGE);
		GtkWidget* w = fileManageNew(pp->window);
		gtk_widget_show(w);
	}
	else
		draw3_popdown(NULL,1,1);
}
void DrawMenu931()
{
	char temp[52];
	int inet_sock;
	struct ifreq ifr;
	static char mask_temp[256];
	/* 格式化字符串 */
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 1))
	{
		draw_dialog_all (DIALOG_MASK);
	}
	else
	{
		inet_sock = socket(AF_INET, SOCK_DGRAM, 0);
		strcpy(ifr.ifr_name, "usb0");
		ioctl(inet_sock, SIOCGIFNETMASK, &ifr);
		sprintf(mask_temp,"%s\n", inet_ntoa(((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr));
		gtk_label_set_text (GTK_LABEL (pp->data3[1]), mask_temp);
	}
	g_sprintf (temp,"%s", con2_p[9][3][1]);

	/* 设置label */
	gtk_label_set_text (GTK_LABEL (pp->label3[1]), temp);

	/* 显示和隐藏控件 */
	//gtk_widget_set_sensitive(pp->eventbox30[1] , FALSE) ;
	//gtk_widget_set_sensitive(pp->eventbox31[1] , FALSE) ;
	gtk_widget_show (pp->eventbox30[1]);
	gtk_widget_show (pp->eventbox31[1]);
	gtk_widget_show (pp->data3[1]);
}
void DrawMenu941()
{
}


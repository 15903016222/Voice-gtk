/*
 * drawmenu3.c
 *
 *  Created on: 2012-5-10
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

#include "../../string/_string.h"
#include "menu.h"
#include "../ui.h"
#include "menuFunc.h"
#include "../../calculation/sizingcurve.h"
#include "../authorization.h"

void CalibrationLastLaw()
{
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);
	if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
	{
		if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 3))
		{
			cur_value = TMP_CBA(clb_end) / 100.0 ;
			lower =	TMP_CBA(clb_start) / 100.0  ;
			upper =	LAW_VAL_POS(grp , Angle_max) / 100.0;
			step  = LAW_VAL_POS(grp , Angle_step) / 100.0;
			digit = 1;
			pos = 3;
			unit = UNIT_DEG;
			draw3_digit_pressed (data_0238, units[unit], cur_value,
					lower, upper, step, digit, pp, pos, 28);
		}
		else
		{
			cur_value = TMP_CBA(clb_end)/100.0 ;
			unit = UNIT_DEG;
			pos = 3;
			digit = 1;
			draw3_digit_stop (cur_value , units[unit], digit, pos, 28);
		}
	}
	else if(LAW_VAL_POS(grp , Focal_type) == LINEAR_SCAN)
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
		{
			cur_value = TMP_CBA(clb_end) / 100.0;
			lower = TMP_CBA(clb_start) / 100.0;
			upper = TMP(beam_qty[grp]);
			step =  1;
			digit = 0;
			pos = 3;
			unit = UNIT_VPA;
			draw3_digit_pressed (data_0238, units[unit], cur_value , lower, upper, step, digit, pp, pos, 46);
		}
		else
		{
			cur_value = TMP_CBA(clb_end) / 100.0;
			digit = 0;
			pos = 3;
			unit = UNIT_VPA;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 46);
		}
	}
}

void DrawGateAWidth()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);
    double velocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
	switch (pp->p_tmp_config->cwidth_reg)
	{
		case 0:	tmpf = 1; break;
		case 1:	tmpf = 10; break;
		case 2:	tmpf = 0.01; break;
		case 3:	tmpf = 0.1; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
	{
		if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
				cur_value = (GROUP_VAL_POS( grp , gate[0].width) / 1000.0) * (velocity / 2000.0);   /* 当前显示的范围数值mm */
				lower = 0.02 * (velocity / 2000.0);
				//lower = 0;
				upper = ((MAX_RANGE_US - GROUP_VAL_POS( grp , gate[0].start)/ 1000.0) > 6400.0 ? 6400.0 :
						(MAX_RANGE_US - GROUP_VAL_POS( grp , gate[0].start) / 1000.0)) * (velocity / 2000.0);
				step = tmpf ; //* (velocity / 2000.0);
				digit = 2;
				pos = 3;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = (GROUP_VAL_POS( grp , gate[0].width) / 1000.0) * 0.03937 * (velocity / 2000.0); /* 当前显示的范围inch */
				lower =	0.02 * 0.03937 * (velocity / 2000.0);
				upper =	((MAX_RANGE_US - GROUP_VAL_POS( grp , gate[0].start) / 1000.0) > 6400.0 ? 6400.0 :
						(MAX_RANGE_US - GROUP_VAL_POS( grp , gate[0].start) / 1000.0)) * 0.03937 * (velocity / 2000.0);
				step = tmpf * 0.03937 ; //* (velocity / 2000.0);
				digit = 3;
				pos = 3;
				unit = UNIT_INCH;
			}
		}
		else
		{
			cur_value = GROUP_GATE_POS(width) / 1000.0;   /* us */
			lower = 0.02 ;
			upper = ((MAX_RANGE_US - GROUP_VAL_POS( grp , gate[0].start) / 1000.0) > 6400.0 ? 6400.0 :
					(MAX_RANGE_US - GROUP_VAL_POS( grp , gate[0].start) / 1000.0)) ;
			step = tmpf;
			digit = 2;
			pos = 3;
			unit = UNIT_US;
		}
		draw3_digit_pressed (data_203, units[unit], cur_value , lower, upper, step, digit, pp, pos, 20);
	}
	else
	{
		if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
				cur_value = (GROUP_VAL_POS( grp , gate[0].width)/ 1000.0) * (velocity / 2000.0);   /* 当前显示的范围数值mm */
				unit = UNIT_MM;
				digit = 2;
				pos = 3;
			}
			else
			{
				cur_value = (GROUP_VAL_POS( grp , gate[0].width)/ 1000.0) * 0.03937 * (velocity / 2000.0); /* 当前显示的范围inch */
				unit = UNIT_INCH;
				digit = 3;
				pos = 3;
			}
		}
		else
		{
			cur_value =GROUP_VAL_POS( grp , gate[0].width) / 1000.0;   /* us */
			digit = 2;
			pos = 3;
			unit = UNIT_US;
		}
		draw3_digit_stop (cur_value , units[unit], digit, pos, 20);
	}
}

void DrawMenu003(){}
void DrawMenu013(){}

void DrawMenu023EncoderStep1()
{
	char* str = NULL ;
	if ((get_inspec_type (pp->p_config) == 1) || (get_inspec_type (pp->p_config) == 2))
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
			draw3_pop_tt (data_700, NULL,
					menu_content[ENCODER+get_cur_encoder (pp->p_config)],
					menu_content+ENCODER, 2, 3,get_cur_encoder (pp->p_config), 0);
		else
			draw3_popdown (menu_content[ENCODER+get_cur_encoder (pp->p_config)], 3, 0);
	}
	else
	{
		if ((get_inspec_source (pp->p_config) == 1) || (get_inspec_source (pp->p_config) == 2))
			draw3_popdown (menu_content[ENCODER+(get_inspec_source (pp->p_config)-1)], 3, 0);
		else
			draw3_popdown (menu_content[ENCODER+ 0 ], 3, 0);
		gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
	}
	str = g_strdup_printf ("%s", con2_p[0][2][40]);
	gtk_label_set_text (GTK_LABEL (pp->label3[3]), str);
	g_free(str);
}
void DrawMenu023EncoderStep2()
{
	gtk_widget_hide (pp->eventbox30[3]);
	gtk_widget_hide (pp->eventbox31[3]);
}
void DrawMenu023EncoderStep3()
{
	gtk_widget_hide (pp->eventbox30[3]);
	gtk_widget_hide (pp->eventbox31[3]);
}
void DrawMenu023EncoderStep4()
{
	gtk_widget_hide (pp->eventbox30[3]);
	gtk_widget_hide (pp->eventbox31[3]);
}

void DrawMenu023EncoderStep5()
{
	gtk_widget_hide (pp->eventbox30[3]);
	gtk_widget_hide (pp->eventbox31[3]);
}

void DrawMenu023EncoderStep6()
{
	gtk_widget_hide (pp->eventbox30[3]);
	gtk_widget_hide (pp->eventbox31[3]);
}

void DrawMenu023UltrasoundStep1()
{
	int grp = get_current_group(pp->p_config);

	if(PA_SCAN == GROUP_VAL_POS(grp , group_mode))
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
			draw3_pop_tt (data_023, NULL,
					menu_content[CMODE + pp->cmode_pos],
					menu_content+CMODE, 4, 3, pp->cmode_pos, 0x0);
		else
			draw3_popdown (menu_content[CMODE + pp->cmode_pos], 3, 0);
	}
	else
	{
		int size = getMainDictSize(MAINSTRINGDICT_CMODE_UT);
		const char* str[size];
		getMainDictStrings(MAINSTRINGDICT_CMODE_UT ,str ,size);

		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
		{
			draw3_pop_tt (data_023, NULL,
				str[pp->cmode_pos],
				str, 3, 3, pp->cmode_pos, 0x0);
		}
		else
		{
			draw3_popdown (str[pp->cmode_pos], 3, 0);
		}
	}

}

void DrawMenu023UltrasoundVelocityStep2()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);
    int _nGain = group_get_gain(grp);
    int _nGainR= group_get_refgain(grp);

	switch (TMP(db_reg))
	{
		case 0:	tmpf = 1; break;
		case 1:	tmpf = 2; break;
		case 2:	tmpf = 6; break;
		case 3:	tmpf = 0.5; break;
		case 4:	tmpf = 0.1; break;
		default:break;
	}


	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
	{
		cur_value = (_nGain - _nGainR * get_group_db_ref (pp->p_config, grp)) / 100.0;
		lower = 0.0 - _nGainR * get_group_db_ref (pp->p_config, grp) / 100.0 ;
		upper = GAIN_MAX - _nGainR * get_group_db_ref (pp->p_config, grp) / 100.0;
		step = tmpf;
		digit = 1;
		pos = 3;
		unit = UNIT_DB;
		draw3_digit_pressed (data_100, units[unit], cur_value ,
				lower, upper, step, digit, pp, pos, 9);
	}
	else
	{
		cur_value = (_nGain - _nGainR * get_group_db_ref (pp->p_config, grp)) / 100.0;
		digit = 1;
		pos = 3;
		unit = UNIT_DB;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 9);
	}
}

void DrawMenu023UltrasoundVelocityStep3()
{
	int grp = get_current_group (pp->p_config);
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))	
		{
			draw3_pop_tt(data_442, NULL,menu_content[ENVELOPE + GROUP_VAL_POS( grp , ascan_envelope)],menu_content+ENVELOPE, 2, 3, GROUP_VAL_POS( grp , ascan_envelope), 0);
		}
		else
			draw3_popdown (menu_content[ENVELOPE + GROUP_VAL_POS( grp , ascan_envelope)], 3, 0);
}

void DrawMenu023UltrasoundVelocityStep4()
{
    DrawGateAWidth();
}

void DrawMenu023UltrasoundVelocityStep5()
{
	DrawGateAWidth();
}

void DrawMenu023UltrasoundVelocityStep6()
{
	//gtk_widget_hide (pp->eventbox30[3]);
	//gtk_widget_hide (pp->eventbox31[3]);
	//int grp = get_current_group(pp->p_config);
	//if(GROUP_VAL_POS(grp , group_mode) == PA_SCAN )
	//{
		draw3_popdown_offset(NULL, 3,1,31);
	//}

}

void DrawMenu023UltrasoundWedgeDelayStep2()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos, unit = 0;

	if(pp->echotype_pos == 0)
	{
		switch (TMP(clb_reg))
		{
			case 0:	tmpf = 1; break;
			case 1:	tmpf = 10; break;
			case 2:	tmpf = 0.1; break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
		{
			cur_value = pp->radius1 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 3;
			unit = UNIT_MM;
			if(get_unit(pp->p_config) == UNIT_INCH)
			{
				cur_value *= 0.03937;
				lower *= 0.03937    ;
				upper *= 0.03937    ;
				step  *= 0.03937    ;
				digit = 3;
				unit = UNIT_INCH;
			}
			draw3_digit_pressed (data_0232, units[unit], cur_value , lower, upper, step, digit, pp, pos, 23);
		}
		else
		{
			cur_value = pp->radius1 / 1000.0;
			digit = 2;
			pos = 3;
			unit = UNIT_MM;
			if(get_unit(pp->p_config) == UNIT_INCH)
			{
				cur_value *= 0.03937;
				digit = 3;
				unit = UNIT_INCH;
			}
			draw3_digit_stop (cur_value, units[unit], digit, pos, 23);
		}

	}
	else if(pp->echotype_pos == 1)
	{
		switch (TMP(clb_reg))
		{
			case 0:	tmpf = 1; break;
			case 1:	tmpf = 10; break;
			case 2:	tmpf = 0.1; break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
		{
			cur_value = pp->depth1 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 3;
			unit = UNIT_MM;
			if(get_unit(pp->p_config) == UNIT_INCH)
			{
				cur_value *= 0.03937;
				lower *= 0.03937    ;
				upper *= 0.03937    ;
				step  *= 0.03937    ;
				digit = 3;
				unit = UNIT_INCH;
			}
			draw3_digit_pressed (data_0233, units[unit], cur_value , lower, upper, step, digit, pp, pos, 24);
		}
		else
		{
			cur_value = pp->depth1 / 1000.0;
			digit = 2;
			pos = 3;
			unit = UNIT_MM;
			if(get_unit(pp->p_config) == UNIT_INCH)
			{
				cur_value *= 0.03937;
				digit = 3;
				unit = UNIT_INCH;
			}
			draw3_digit_stop (cur_value, units[unit], digit, pos, 24);
		}
	}
	else
	{
		switch (TMP(clb_reg))
		{
			case 0:	tmpf = 1; break;
			case 1:	tmpf = 10; break;
			case 2:	tmpf = 0.1; break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
		{
			cur_value = pp->thickness1 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 3;
			unit = UNIT_MM;
			if(get_unit(pp->p_config) == UNIT_INCH)
			{
				cur_value *= 0.03937;
				lower *= 0.03937    ;
				upper *= 0.03937    ;
				step  *= 0.03937    ;
				digit = 3;
				unit = UNIT_INCH;
			}
			draw3_digit_pressed (data_0234, units[unit], cur_value , lower, upper, step, digit, pp, pos, 25);
		}
		else
		{
			cur_value = pp->thickness1 / 1000.0;
			digit = 2;
			pos = 3;
			unit = UNIT_MM;
			if(get_unit(pp->p_config) == UNIT_INCH)
			{
				cur_value *= 0.03937;
				digit = 3;
				unit = UNIT_INCH;
			}
			draw3_digit_stop (cur_value, units[unit], digit, pos, 25);
		}
	}
}

void DrawMenu023UltrasoundWedgeDelayStep3()
{
	 CalibrationLastLaw() ;
}

void DrawMenu023UltrasoundWedgeDelayStep4()
{
	DrawGateAWidth();
}

void DrawMenu023UltrasoundWedgeDelayStep5()
{
	int grp = get_current_group(pp->p_config);
	if(GROUP_VAL_POS(grp , group_mode) == PA_SCAN)
	{
		draw3_popdown_offset(NULL, 3,1,29);
	}
	else
	{
		draw3_popdown_offset(NULL, 3,1,31);
	}
}

void DrawMenu023UltrasoundWedgeDelayStep6()
{
	gtk_widget_hide (pp->eventbox30[3]);
	gtk_widget_hide (pp->eventbox31[3]);
}

void DrawMenu023UltrasoundSensationStep2()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos, unit = 0;

	switch(TMP(tolerance_t_reg))
	{
		case 0:	tmpf = 0.1; break;
		case 1:	tmpf = 1.0; break;
		case 2:	tmpf = 10.0; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
	{
		cur_value = pp->tolerance_t / 100.0 ;
		lower = 0.0;
		upper = 50.0;
		step = tmpf;
		digit = 1;
		pos = 3;
		unit = UNIT_BFH;
		draw3_digit_pressed (data_0239, units[unit], cur_value , lower, upper, step, digit, pp, pos, 26);
	}
	else
	{
		cur_value = pp->tolerance_t / 100.0 ;
		digit = 1;
		pos = 3;
		unit = UNIT_BFH;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 26);
	}
}

void DrawMenu023UltrasoundSensationStep3()
{
	 CalibrationLastLaw();
}

void DrawMenu023UltrasoundSensationStep4()
{
     DrawGateAWidth();
}

void DrawMenu023UltrasoundSensationStep5()
{
	draw3_popdown_offset(NULL, 3,1,35);
}

void DrawMenu023UltrasoundSensationStep6()
{
	draw3_popdown_offset(NULL, 3,1,29);
}

void DrawMenu023UltrasoundUtWedgedelayAndVelocityStep2()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);
    int _nGain  = group_get_gain(grp) ;
    int _nGainR = group_get_refgain(grp);

	switch (TMP(db_reg))
	{
		case 0:	tmpf = 1; break;
		case 1:	tmpf = 2; break;
		case 2:	tmpf = 6; break;
		case 3:	tmpf = 0.5; break;
		case 4:	tmpf = 0.1; break;
		default:break;
	}

	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
	{
		cur_value = (_nGain - _nGainR * get_group_db_ref (pp->p_config, grp)) / 100.0;
		lower = 0.0 - _nGainR * get_group_db_ref (pp->p_config, grp) / 100.0;
		upper = GAIN_MAX - _nGainR * get_group_db_ref (pp->p_config, grp) / 100.0;
		step = tmpf;
		digit = 1;
		pos = 3;
		unit = UNIT_DB;
		draw3_digit_pressed (data_100, units[unit], cur_value ,
				lower, upper, step, digit, pp, pos, 9);
	}
	else
	{
		cur_value = (_nGain - _nGainR * get_group_db_ref (pp->p_config, grp)) / 100.0;
		digit = 1;
		pos = 3;
		unit = UNIT_DB;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 9);
	}
}

void DrawMenu023UltrasoundUtWedgedelayAndVelocityStep3()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos, unit = 0;

	if(pp->echotype_pos == 0)
	{
		switch (TMP(clb_reg))
		{
			case 0:	tmpf = 1; break;
			case 1:	tmpf = 10; break;
			case 2:	tmpf = 0.1; break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
		{
			cur_value = pp->radius1 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 3;
			unit = UNIT_MM;
			if(get_unit(pp->p_config) == UNIT_INCH)
			{
				cur_value *= 0.03937;
				lower *= 0.03937    ;
				upper *= 0.03937    ;
				step  *= 0.03937    ;
				digit = 3;
				unit = UNIT_INCH;
			}
			draw3_digit_pressed (data_0232, units[unit], cur_value , lower, upper, step, digit, pp, pos, 13);
		}
		else
		{
			cur_value = pp->radius1 / 1000.0;
			digit = 2;
			pos = 3;
			unit = UNIT_MM;
			if(get_unit(pp->p_config) == UNIT_INCH)
			{
				cur_value *= 0.03937;
				digit = 3;
				unit = UNIT_INCH;
			}
			draw3_digit_stop (cur_value, units[unit], digit, pos, 13);
		}

	}
	else if(pp->echotype_pos == 1)
	{
		switch (TMP(clb_reg))
		{
			case 0:	tmpf = 1; break;
			case 1:	tmpf = 10; break;
			case 2:	tmpf = 0.1; break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
		{
			cur_value = pp->depth1 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 3;
			unit = UNIT_MM;
			if(get_unit(pp->p_config) == UNIT_INCH)
			{
				cur_value *= 0.03937;
				lower *= 0.03937    ;
				upper *= 0.03937    ;
				step  *= 0.03937    ;
				digit = 3;
				unit = UNIT_INCH;
			}
			draw3_digit_pressed (data_0233, units[unit], cur_value , lower, upper, step, digit, pp, pos, 15);
		}
		else
		{
			cur_value = pp->depth1 / 1000.0;
			digit = 2;
			pos = 3;
			unit = UNIT_MM;
			if(get_unit(pp->p_config) == UNIT_INCH)
			{
				cur_value *= 0.03937;
				digit = 3;
				unit = UNIT_INCH;
			}
			draw3_digit_stop (cur_value, units[unit], digit, pos, 15);
		}
	}
	else
	{
		switch (TMP(clb_reg))
		{
			case 0:	tmpf = 1; break;
			case 1:	tmpf = 10; break;
			case 2:	tmpf = 0.1; break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
		{
			cur_value = pp->thickness1 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 3;
			unit = UNIT_MM;
			if(get_unit(pp->p_config) == UNIT_INCH)
			{
				cur_value *= 0.03937;
				lower *= 0.03937    ;
				upper *= 0.03937    ;
				step  *= 0.03937    ;
				digit = 3;
				unit = UNIT_INCH;
			}
			draw3_digit_pressed (data_0234, units[unit], cur_value , lower, upper, step, digit, pp, pos, 17);
		}
		else
		{
			cur_value = pp->thickness1 / 1000.0;
			digit = 2;
			pos = 3;
			unit = UNIT_MM;
			if(get_unit(pp->p_config) == UNIT_INCH)
			{
				cur_value *= 0.03937;
				digit = 3;
				unit = UNIT_INCH;
			}
			draw3_digit_stop (cur_value, units[unit], digit, pos, 17);
		}

	}
}

void DrawMenu023UltrasoundUtWedgedelayAndVelocityStep4()
{
	DrawGateAWidth();
}

void DrawMenu023UltrasoundUtWedgedelayAndVelocityStep5()
{
	DrawGateAWidth();
}

void DrawMenu023UltrasoundUtWedgedelayAndVelocityStep6()
{
	char* str = NULL ;
	str = g_strdup_printf("%.2f", TMP_CBA(CalibrationWedgeDelay)/1000.0);
	menuSetLabel(3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE
			,getMainDictString(MAINSTRINGDICT_CMODE_UT ,1)
			,str);
	g_free(str);
	gtk_widget_set_sensitive (pp->eventbox30[3],FALSE);
	gtk_widget_set_sensitive (pp->eventbox31[3],FALSE);
}

void DrawMenu023UltrasoundTCGStep2()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos, unit = 0;

	switch(TMP(tolerance_t_reg))
	{
		case 0:	tmpf = 0.1; break;
		case 1:	tmpf = 1.0; break;
		case 2:	tmpf = 10.0; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
	{
		cur_value = pp->tolerance_t / 100.0 ;
		lower = 0.0;
		upper = 50.0;
		step = tmpf;
		digit = 1;
		pos = 3;
		unit = UNIT_BFH;
		draw3_digit_pressed (data_0239, units[unit], cur_value , lower, upper, step, digit, pp, pos, 26);
	}
	else
	{
		cur_value = pp->tolerance_t / 100.0 ;
		digit = 1;
		pos = 3;
		unit = UNIT_BFH;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 26);
	}
}

void DrawMenu023UltrasoundTCGStep3()
{
	 CalibrationLastLaw();
}

void DrawMenu023UltrasoundTCGStep4()
{
	DrawGateAWidth();
}

void DrawMenu023UltrasoundTCGStep5()
{
	draw3_popdown_offset(NULL, 3,1,29);
}

void DrawMenu023UltrasoundTCGStep6()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);
	int _nBeamNo = TMP(beam_num[grp]) ;
	int _nPointPos = GROUP_VAL_POS( grp , SizingCurves.point_pos) ;
	/* 当前步进 */
	switch (TMP(tcg_gain_reg))
	{
		case 0:	tmpf = 1; break;
		case 1:	tmpf = 2; break;
		case 2:	tmpf = 6; break;
		case 3:	tmpf = 0.1; break;
		case 4:	tmpf = 0.5; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
	{
		if(GROUP_VAL_POS( grp , SizingCurves.point_pos == 0))  cur_value = 0 ;
		else
		{
			cur_value = 20* log10( GROUP_VAL_POS( grp , SizingCurves.amplitude[_nBeamNo][ _nPointPos - 1]) * 1.0
					/ GROUP_VAL_POS( grp , SizingCurves.amplitude[_nBeamNo][_nPointPos])) ;
		}
		lower = 0.0;
		upper = CurrentPointTcgGainLimit(grp);
		step = tmpf;
		digit = 1;
		pos = 3;
		unit = UNIT_DB;
		draw3_digit_pressed (data_2332, units[unit], cur_value , lower, upper, step, digit, pp, pos, 58);
	}
	else
	{
		if(GROUP_VAL_POS( grp , SizingCurves.point_pos == 0))  cur_value = 0 ;
		else
		{
			cur_value = 20 * log10( GROUP_VAL_POS( grp , SizingCurves.amplitude[_nBeamNo][_nPointPos - 1]) * 1.0
					/ GROUP_VAL_POS( grp , SizingCurves.amplitude[_nBeamNo][_nPointPos])) ;
		}
		digit = 1;
		pos = 3  ;
		unit = UNIT_DB;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 58);
	}

	if(TMP_CBA(bNewPoint))
	{
		gtk_widget_set_sensitive(pp->eventbox30[3],TRUE);
		gtk_widget_set_sensitive(pp->eventbox31[3],TRUE);
	}
	else
	{
		gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
	}

}

void DrawMenu023CodeAPIStep2()
{
	gtk_widget_hide (pp->eventbox30[3]);
	gtk_widget_hide (pp->eventbox31[3]);
}

void DrawMenu023CodeAPIStep3()
{
	gtk_widget_hide (pp->eventbox30[3]);
	gtk_widget_hide (pp->eventbox31[3]);
}

void DrawMenu023CodeAPIStep4()
{
	gtk_widget_hide (pp->eventbox30[3]);
	gtk_widget_hide (pp->eventbox31[3]);
}

void DrawMenu023CodeAPIStep5()
{
	gtk_widget_hide (pp->eventbox30[3]);
	gtk_widget_hide (pp->eventbox31[3]);
}

void DrawMenu023CodeAPIStep6()
{
	gtk_widget_hide (pp->eventbox30[3]);
	gtk_widget_hide (pp->eventbox31[3]);
}

void DrawMenu023CodeStep1()
{
	char temp[52];

	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
		draw3_pop_tt (data_0231, NULL,
				menu_content[SCODE + pp->scode_pos],
				menu_content+SCODE, 2, 3, pp->scode_pos, 0x0);
	else
		draw3_popdown (menu_content[SCODE + pp->scode_pos], 3, 0);

	g_sprintf (temp,"%s", con2_p[0][2][6]);
	gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
}

void DrawMenu023CodeAWSStep2()
{
	gtk_widget_hide (pp->eventbox30[3]);
	gtk_widget_hide (pp->eventbox31[3]);
}
// Gate width
void DrawMenu023CodeAWSStep3()
{
	DrawGateAWidth();
}

void DrawMenu023CodeAWSStep4()
{
	draw3_popdown_offset(NULL, 3, 1, 29);
}

void DrawMenu023CodeAWSStep5()
{
	draw3_popdown_offset(NULL, 3 , 1, 31);
}

void DrawMenu023CodeAWSStep6()
{

}

void DrawMenu023UltrasoundVelocity()
{
	switch(pp->cstart_qty)
	{
	case 0:
	case 1:
		DrawMenu023UltrasoundStep1();
		break;
	case 2:
		DrawMenu023UltrasoundVelocityStep2();
		break;
	case 3:
		DrawMenu023UltrasoundVelocityStep3();
		break;
	case 4:
		DrawMenu023UltrasoundVelocityStep4();
		break;
	case 5:
		DrawMenu023UltrasoundVelocityStep5();
		break;
	case 6:
		DrawMenu023UltrasoundVelocityStep6();
		break;
	default:
		break;

	}
}
void DrawMenu023UltrasoundWedgeDelay()
{
	switch(pp->cstart_qty)
	{
	case 0:
	case 1:
		DrawMenu023UltrasoundStep1();
		break;
	case 2:
		DrawMenu023UltrasoundWedgeDelayStep2();
		break;
	case 3:
		DrawMenu023UltrasoundWedgeDelayStep3();
		break;
	case 4:
		DrawMenu023UltrasoundWedgeDelayStep4();
		break;
	case 5:
		DrawMenu023UltrasoundWedgeDelayStep5();
		break;
	case 6:
		DrawMenu023UltrasoundWedgeDelayStep6();
		break;
	default:
		break;

	}
}
void DrawMenu023UltrasoundSensation()
{
	int _nGroupId = get_current_group(pp->p_config);
	if(PA_SCAN == GROUP_VAL_POS(_nGroupId , group_mode))
	{
		switch(pp->cstart_qty)
		{
		case 0:
		case 1:
			DrawMenu023UltrasoundStep1();
			break;
		case 2:
			DrawMenu023UltrasoundSensationStep2();
			break;
		case 3:
			DrawMenu023UltrasoundSensationStep3();
			break;
		case 4:
			DrawMenu023UltrasoundSensationStep4();
			break;
		case 5:
			DrawMenu023UltrasoundSensationStep5();
			break;
		case 6:
			DrawMenu023UltrasoundSensationStep6();
			break;
		default:
			break;
		}
	}
	else
	{
		switch(pp->cstart_qty)
		{
		case 0:
		case 1:
			DrawMenu023UltrasoundStep1();
			break;
		case 2:
			DrawMenu023UltrasoundUtWedgedelayAndVelocityStep2();
			break;
		case 3:
			DrawMenu023UltrasoundUtWedgedelayAndVelocityStep3();
			break;
		case 4:
			DrawMenu023UltrasoundUtWedgedelayAndVelocityStep4();
			break;
		case 5:
			DrawMenu023UltrasoundUtWedgedelayAndVelocityStep5();
			break;
		case 6:
			DrawMenu023UltrasoundUtWedgedelayAndVelocityStep6();
			break;
		default:
			break;
		}
	}

}
void DrawMenu023UltrasoundTCG()
{
	switch(pp->cstart_qty)
	{
	case 0:
	case 1:
		DrawMenu023UltrasoundStep1();
		break;
	case 2:
		DrawMenu023UltrasoundTCGStep2();
		break;
	case 3:
		DrawMenu023UltrasoundTCGStep4();
		break;
	case 4:
		DrawMenu023UltrasoundTCGStep5();
		break;
	case 5:
		DrawMenu023UltrasoundTCGStep6();
		break;
	case 6:
		break;
	default:
		break;
	}
}

void DrawMenu023Encoder()
{
	switch(pp->cstart_qty)
	{
	case 0:
	case 1:
		DrawMenu023EncoderStep1();
		break;
	case 2:
		DrawMenu023EncoderStep2();
		break;
	case 3:
		DrawMenu023EncoderStep3();
		break;
	case 4:
		DrawMenu023EncoderStep4();
		break;
	case 5:
		DrawMenu023EncoderStep5();
		break;
	case 6:
		DrawMenu023EncoderStep6();
		break;
	default:
		break;
	}
}

void DrawMenu023TOFDStep1()
{
	draw3_popdown (menu_content[TX_RX_MODE + 7], 3, 0);
}

void DrawMenu023TOFDStep3()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);
    int _nGain  = group_get_gain(grp);
    int _nGainR = group_get_refgain(grp);

	switch (TMP(db_reg))
	{
		case 0:	tmpf = 1; break;
		case 1:	tmpf = 2; break;
		case 2:	tmpf = 6; break;
		case 3:	tmpf = 0.5; break;
		case 4:	tmpf = 0.1; break;
		default:break;
	}

	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
	{
		cur_value = (_nGain - _nGainR * get_group_db_ref (pp->p_config, grp)) / 100.0;
		lower = 0.0 - _nGainR * get_group_db_ref (pp->p_config, grp) / 100.0;
		upper = GAIN_MAX - _nGainR * get_group_db_ref (pp->p_config, grp) / 100.0;
		step = tmpf;
		digit = 1;
		pos = 3;
		unit = UNIT_DB;
		draw3_digit_pressed (data_100, units[unit], cur_value ,
				lower, upper, step, digit, pp, pos, 9);
	}
	else
	{
		cur_value = (_nGain - _nGainR * get_group_db_ref (pp->p_config, grp)) / 100.0;
		digit = 1;
		pos = 3;
		unit = UNIT_DB;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 9);
	}
}
void DrawMenu023TOFDStep2()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower, upper, step;
	int digit, pos, unit ;
	int grp = get_current_group(pp->p_config);
	int _nPartGeometry = get_part_geometry(grp) ;
	unsigned int _nDiameter = get_part_diameter (grp) ;
	/* 当前步进 */
	pos = 3 ;
	switch (TMP(part_thickness_reg))
	{
		case 0:	tmpf = 1; break;
		case 1:	tmpf = 10; break;
		case 2:	tmpf = 0.1; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
	{
		if(UNIT_MM == get_unit(pp->p_config))
		{
			cur_value = GROUP_VAL_POS(grp , part.Thickness) / 1000.0;
			lower = 0.05;
			if (_nPartGeometry == PLATE_PART)
				upper = 1000.0;
			else if (_nPartGeometry == OD_PART)
				upper = _nDiameter / 2000.0;
			else
				upper = (_nDiameter - 10.0) / 2000.0;
			step = tmpf;
			digit = 2;
			unit = UNIT_MM;
		}
		else
		{
			cur_value = GROUP_VAL_POS(grp , part.Thickness) / 1000.0*0.03937;
			lower = 0.002;
			if (_nPartGeometry == PLATE_PART)
				upper = 1000.0 *0.03937 ;
			else if (_nPartGeometry == OD_PART)
				upper = _nDiameter / 2000.0 *0.03937 ;
			else
				upper = (_nDiameter - 10.0) / 2000.0 *0.03937;
			step = tmpf*0.03937;
			digit = 3;
			unit = UNIT_INCH;
		}
		draw3_digit_pressed (data_531, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 54);
	}
	else
	{
		if(UNIT_MM == get_unit(pp->p_config))
		{
			cur_value = GROUP_VAL_POS(grp , part.Thickness) / 1000.0;
			digit = 2;
			unit = UNIT_MM;
		}
		else
		{
			cur_value = GROUP_VAL_POS(grp , part.Thickness) / 1000.0*0.03937;
			digit = 3;
			unit = UNIT_INCH;
		}
		draw3_digit_stop (cur_value, units[unit], digit, pos, 54);
	}
}

void DrawMenu023TOFDStep4()
{
	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos = 3 , unit;
	int grp = get_current_group (pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	switch (TMP(u_measure_reg))
	{
		case 0:	tmpf = 0.01; break;
		case 1:	tmpf = 0.1;  break;
		case 2:	tmpf = 1.0;  break;
		case 3:	tmpf = 10.0;  break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
	{
		double _nVelocity = GROUP_VAL_POS(grp , velocity) / 200000 ;
		if((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
		{
			if(get_unit(pp->p_config) == UNIT_MM)
			{
				cur_value = GROUP_VAL_POS( grp , u_measure)/1000.0*(get_group_val (p_grp, GROUP_VELOCITY)/200000.0);
				lower = 0 ;
				upper = 1000 * _nVelocity ;
				step = tmpf;
				digit = 2;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = GROUP_VAL_POS( grp , u_measure)/1000.0 * (get_group_val (p_grp, GROUP_VELOCITY)/200000.0);
				lower = 0 ;
				upper = 1000 * _nVelocity ;
				cur_value  *= 0.03937 ;
				lower      *= 0.03937 ;
				upper      *= 0.03937 ;
				step = tmpf;
				digit = 2;
				unit = UNIT_INCH;
			}
		}
		else	/*us*/
		{
			cur_value = GROUP_VAL_POS( grp , u_measure)/1000.0;
			lower = 0 ;
			upper = 1000 ;
			step = tmpf;
			digit = 2;
			unit = UNIT_US;
		}
		draw3_digit_pressed (data_314, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 64);
	}
	else
	{
		if((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
		{
			if(get_unit(pp->p_config) == UNIT_MM)
			{
				cur_value = GROUP_VAL_POS( grp , u_measure)/1000.0 * (GROUP_VAL_POS(grp , velocity)/200000.0);
				digit = 2;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = 0.03937 * GROUP_VAL_POS( grp , u_measure)/1000.0 * (GROUP_VAL_POS(grp , velocity)/200000.0);
				digit = 3;
				unit = UNIT_INCH;
			}
		}
		else
		{
			cur_value = GROUP_VAL_POS( grp , u_measure)/1000.0;
			digit = 2;
			unit = UNIT_US;
		}
		draw3_digit_stop (cur_value, units[unit], digit, pos, 64);
	}

}

void DrawMenu023TOFDStep5()
{
	DrawGateAWidth();
}

void DrawMenu023TOFDStep6()
{
	char* str = NULL ;
	str = g_strdup_printf("%.2f", TMP_CBA(CalibrationWedgeDelay)/1000.0);
	menuSetLabel(3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE
			,getMainDictString(MAINSTRINGDICT_CMODE_UT ,1)
			,str);
	g_free(str);
	gtk_widget_set_sensitive (pp->eventbox30[3],FALSE);
	gtk_widget_set_sensitive (pp->eventbox31[3],FALSE);
}


void DrawMenu023TOFD()
{
	switch(pp->cstart_qty)
	{
	case 0:
	case 1:
		DrawMenu023TOFDStep1();
		break;
	case 2:
		DrawMenu023TOFDStep2();
		break;
	case 3:
		DrawMenu023TOFDStep3();
		break;
	case 4:
		DrawMenu023TOFDStep4();
		break;
	case 5:
		DrawMenu023TOFDStep6() ;
		break;
	default:
		break;
	}
}


void DrawMenu023Ultrasound()
{
	int grp = get_current_group(pp->p_config);
	if(GROUP_VAL_POS(grp , tx_rxmode1) == TOFD)
	{
		DrawMenu023TOFD();
	}
	else
	{
		switch(pp->cmode_pos)
		{
		case 0:
			DrawMenu023UltrasoundVelocity();
			break;
		case 1:
			DrawMenu023UltrasoundWedgeDelay();
			break;
		case 2:
			DrawMenu023UltrasoundSensation();
			break;
		case 3:
			DrawMenu023UltrasoundTCG();
			break;
		default:
			break;
		}
	}

}

void DrawMenu023CodeAPI()
{
	switch(pp->cstart_qty)
	{
	case 0:
	case 1:
		DrawMenu023CodeStep1();
		break;
	case 2:
		DrawMenu023CodeAPIStep2();
		break;
	case 3:
		DrawMenu023CodeAPIStep3();
		break;
	case 4:
		DrawMenu023CodeAPIStep4();
		break;
	case 5:
		DrawMenu023CodeAPIStep5();
		break;
	case 6:
		DrawMenu023CodeAPIStep6();
		break;
	default:
		break;
	}
}

void DrawMenu023CodeAWS()
{
	switch(pp->cstart_qty)
	{
	case 0:
	case 1:
		DrawMenu023CodeStep1();
		break;
	case 2:
		DrawMenu023CodeAWSStep2();
		break;
	case 3:
		DrawMenu023CodeAWSStep3();
		break;
	case 4:
		DrawMenu023CodeAWSStep4();
		break;
	case 5:
		DrawMenu023CodeAWSStep5();
		break;
	case 6:
		DrawMenu023CodeAWSStep6();
		break;
	default:
		break;
	}
}

void DrawMenu023Code()
{
	switch(pp->scode_pos)
	{
	case 0:
		DrawMenu023CodeAPI();
		break;
	case 1:
		DrawMenu023CodeAWS();
		break;
	default:
		break;
	}
}

void DrawMenu023()
{
	pp->x_pos = 560, pp->y_pos = 380;
	switch (pp->ctype_pos)
	{
	case 0:// encoder calibration
		DrawMenu023Encoder();
		break;
	case 1:
		DrawMenu023Ultrasound();
		break;
	case 2:
		DrawMenu023Code();
		break;
	default:
		break;
	}

}
void DrawMenu033()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group(pp->p_config);
	pp->x_pos = 560, pp->y_pos = 380;
	switch(pp->wstart_qty)
	{
		case 0:
		case 1:
			gtk_widget_hide (pp->eventbox30[3]);
			gtk_widget_hide (pp->eventbox31[3]);
			break;
		case 2:
			if(get_part_weld(grp)==0)
			{
				gtk_widget_set_sensitive (pp->eventbox30[3],FALSE);
				gtk_widget_set_sensitive (pp->eventbox31[3],FALSE);
			}
			else
			{
				if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
					draw3_pop_tt (data_033, NULL,
							menu_content[SYMMETRY + get_part_symmetry(grp)],
							menu_content+SYMMETRY, 3, 3, get_part_symmetry(grp), 0x0);
				else
					draw3_popdown (menu_content[SYMMETRY + get_part_symmetry(grp)], 3, 0);
			}
			break;
		case 3:
			/* 当前步进 */
			switch (TMP(weland_offset_reg))
			{
				case 0:	tmpf = 0.1; break;
				case 1:	tmpf = 1.0; break;
				case 2:	tmpf = 10.0; break;
				default:break;
			}
			if(get_part_weld(grp) == 5)
			{
                                if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
                                {
                                        cur_value = get_weland_offset(grp) / 1000.0;
                                        lower = 0;
                                        upper = 89.90;
                                        step = tmpf;
                                        digit = 2;
                                        pos = 3;
                                        unit = UNIT_DEG;
                                        draw3_digit_pressed (data_root_angle_callback, units[unit], cur_value , lower, upper, step, digit, pp, pos, 15);
                                }
				else
				{
                                        cur_value = get_weland_offset(grp) / 1000.0;
                                        digit = 2;
                                        pos = 3;
                                        unit = UNIT_DEG;
                                        draw3_digit_stop (cur_value, units[unit], digit, pos, 15);
				}
				break;
			}
			else
			{
                                if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
                                {
                                        if(UNIT_MM == get_unit(pp->p_config))
                                        {
                                                cur_value = get_weland_offset(grp) / 1000.0;
                                                lower = 0;
                                                upper = 1000.00;
                                                step = tmpf;
                                                digit = 2;
                                                pos = 3;
                                                unit = UNIT_MM;
                                        }
                                        else
                                        {
                                                cur_value = get_weland_offset(grp) / 1000.0 * 0.03937;
                                                lower = 0;
                                                upper = 1000.00 * 0.03937;
                                                step = tmpf;
                                                digit = 3;
                                                pos = 3;
                                                unit = UNIT_INCH;
                                        }
					draw3_digit_pressed (data_0331, units[unit], cur_value , lower, upper, step, digit, pp, pos, 8);
				}
				else
				{
					if(UNIT_MM == get_unit(pp->p_config))
					{
						cur_value = get_weland_offset(grp) / 1000.0;
						digit = 2;
						pos = 3;
						unit = UNIT_MM;
					}
					else
					{
						cur_value = get_weland_offset(grp) / 1000.0 * 0.03937;
						digit = 3;
						pos = 3;
						unit = UNIT_INCH;
					}
					draw3_digit_stop (cur_value, units[unit], digit, pos, 8);
				}	
			}
			break;
		case 4:
			if(get_part_weld(grp)==0)
			{
				gtk_widget_hide (pp->eventbox30[3]);
				gtk_widget_hide (pp->eventbox31[3]);
			}
			else if(get_part_weld(grp) == 2)
			{
				switch (pp->p_tmp_config->fizone_radius_reg)
				{
					case 0:	tmpf = 0.1; break;
					case 1:	tmpf = 1.0; break;
					case 2:	tmpf = 10.0; break;
					default:break;
				}
				if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
				{
					cur_value = get_fizone_radius(grp) / 1000.0;
					lower = 0;
					upper = 89.90;
					step = tmpf;
					digit = 2;
					pos = 3;
					unit = UNIT_DEG;
                    			draw3_digit_pressed (data_034, units[unit], cur_value , lower, upper, step, digit, pp, pos, 19);
                		}
	            		else
	            		{
					cur_value = get_fizone_radius(grp) / 1000.0;
					digit = 2;
					pos = 3;
					unit = UNIT_DEG;
                    			draw3_digit_stop (cur_value, units[unit], digit, pos, 19);
                 		}
	            		 break;					
			}
                        else if((get_part_weld(grp) == 4))
                        {
                                switch (pp->p_tmp_config->fizone_angle_reg)
                                {
                                        case 0: tmpf = 0.1; break;
                                        case 1: tmpf = 1.0; break;
                                        case 2: tmpf = 10.0; break;
                                        default:break;
                                }
                                if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
                                {
                                        cur_value = get_fizone_angle(grp) / 1000.0;
                                        lower = 0;
                                        upper = 89.90;
                                        step = tmpf;
                                        digit = 2;
                                        pos = 3;
                                        unit = UNIT_DEG;
                                        draw3_digit_pressed (data_0332, units[unit], cur_value , lower, upper, step, digit, pp, pos, 20);

                                }
                                else
                                {
                                        cur_value = get_fizone_angle(grp) / 1000.0;
                                        digit = 2;
                                        pos = 3;
                                        unit = UNIT_DEG;
                                        draw3_digit_stop (cur_value, units[unit], digit, pos, 20);
                                }
                                break;
                        }
			else
			{
				/* 当前步进 */
				switch (pp->p_tmp_config->fizone_angle_reg)
				{
					case 0:	tmpf = 0.1; break;
					case 1:	tmpf = 1.0; break;
					case 2:	tmpf = 10.0; break;
					default:break;
				}
				if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
				{
					cur_value = get_fizone_angle(grp) / 1000.0;
					lower = 0;
					upper = 89.90;
					step = tmpf;
					digit = 2;
					pos = 3;
					unit = UNIT_DEG;
					draw3_digit_pressed (data_0332, units[unit], cur_value , lower, upper, step, digit, pp, pos, 11);
				}
				else
				{
					cur_value = get_fizone_angle(grp) / 1000.0;
					digit = 2;
					pos = 3;
					unit = UNIT_DEG;
					draw3_digit_stop (cur_value, units[unit], digit, pos, 11);
				}
			}
			break;
		default:break;
	}
}

void DrawMenu043()
{
	if ( !con2_p[0][4][3] )
		gtk_widget_hide (pp->eventbox30[3]);
	gtk_widget_hide (pp->eventbox31[3]);
}

void DrawMenu103()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos, unit = 0;
    int grp = get_current_group(pp->p_config);
	switch (TMP(wedge_delay_reg))
	{
		case 0:	tmpf = 0.1; break;
		case 1:	tmpf = 1; break;
		case 2:	tmpf = 0.01; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
	{
		cur_value = GROUP_VAL_POS(grp , wedge_delay) / 1000.0;
		lower = 0;
		upper = 50.0;
		step = tmpf;
		digit = 2;
		pos = 3;
		unit = UNIT_US;
		draw3_digit_pressed (data_103, units[unit], cur_value , lower, upper, step, digit, pp, pos, 0);
	}
	else
	{
		cur_value = GROUP_VAL_POS(grp , wedge_delay) / 1000.0;
		digit = 2;
		pos = 3;
		unit = UNIT_US;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
	}
}

void DrawMenu113()
{
    gint grp = get_current_group (pp->p_config);
    guchar grpMode = pp->p_config->group[grp].group_mode;

    gint offset = PA_VOLTAGE;
    gint num = 2;

    if (UT1_SCAN == grpMode || UT2_SCAN == grpMode) {
        offset = UT_VOLTAGE;
        num = 3;
    }

	pp->x_pos = 570, pp->y_pos = 423 - 26;
    if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3)) {
            draw3_pop_tt(data_113, NULL,
                    menu_content[offset + num + get_voltage(pp->p_config, grp)],
                    menu_content + offset, num, 3, get_voltage(pp->p_config, grp),
                    0);

    } else {
        draw3_popdown (menu_content[offset + num + get_voltage (pp->p_config, grp)], 3, 0);
	}
}

void DrawMenu123()
{
	int grp = get_current_group(pp->p_config);
	if (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RECTIFIER) == RF_WAVE)
	{
		gtk_widget_set_sensitive (pp->eventbox30[3], FALSE);
		gtk_widget_set_sensitive (pp->eventbox31[3], FALSE);
	}
	draw3_popdown (menu_content
			[OFF_ON + get_group_val (get_group_by_id(pp->p_config, grp), GROUP_VIDEO_FILTER)], 3, 0);
}

void DrawMenu133()
{
    int grp = get_current_group(pp->p_config);
    double cur_value ;
	int digit = 0, pos, unit;

	if (GROUP_VAL_POS( grp , group_mode) != PA_SCAN)
	{
		gtk_widget_hide (pp->eventbox30[3]);
		gtk_widget_hide (pp->eventbox31[3]);
	}
	else if (GROUP_VAL_POS( grp , group_mode) == PA_SCAN)
	{
		gtk_widget_set_sensitive (pp->eventbox30[3], FALSE);
		gtk_widget_set_sensitive (pp->eventbox31[3], FALSE);
		cur_value = GROUP_VAL_POS( grp , skew)/100.0 ;
		unit = UNIT_NULL;
		pos = 3;
		digit = 1;
		draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
	}
}

void DrawMenu143()
{
	double tmpf = 0.0;
	char *str ;
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);

	pp->x_pos = 585, pp->y_pos = 371-26;
	switch (TMP(point_qty_reg))
	{
		case 0:	tmpf = 1.0; break;
		case 1:	tmpf = 10.0; break;
		case 2:	tmpf = 100.0; break;
		default:break;
	}
	if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 3))
	{
		if (pp->mark_pop_change)
		{
			cur_value = GROUP_VAL_POS( grp , point_qty) ;
			lower =	32.0;
			upper = (double)(StoreBuffLimitToGroupPointQty(grp)) ;
			step = tmpf;
			digit = 0;
			pos = 3;
			unit = UNIT_NONE;
			draw3_digit_pressed (data_1431, units[unit], cur_value , lower, upper, step, digit, pp, pos, 0);
		}
		else
		{
			if (GROUP_VAL_POS( grp , point_qty_pos) == 4)	/* 自定义数值时候按下显示数值 */
			{
				/* 更新当前增益值显示 */
				str = g_strdup_printf ("%d", GROUP_VAL_POS( grp , point_qty));
				draw3_pop_tt (data_143, NULL,
						str, menu_content + POINT_QTY, 5, 3,
						GROUP_VAL_POS( grp , point_qty_pos), 0);
				g_free(str);
			}
			else	/* Auto ==时候 显示当前选项 */
				draw3_pop_tt (data_143, NULL,
						menu_content[POINT_QTY + GROUP_VAL_POS( grp , point_qty_pos)],
						menu_content + POINT_QTY, 5, 3, GROUP_VAL_POS( grp , point_qty_pos), 0);
		}
	}
	else
	{
		cur_value = GROUP_VAL_POS( grp , point_qty) ;
		unit = UNIT_NULL;
		pos = 3;
		digit = 0;
		draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
	}
}

void DrawMenu203()
{
	double tmpf = 0.0;
	char *str;
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);

	pp->x_pos = 580, pp->y_pos = 380 - 26;
	if (GROUP_GATE_POS(parameters) == 0)
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
		{
			if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) ||
					(UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
			{
				switch (TMP(gate_width_reg))
				{
				    case 2:	tmpf = 1.0; break;
				    case 3:	tmpf = 10.0; break;
					case 0:	tmpf = 0.01; break;
					case 1:	tmpf = 0.1; break;

					default:break;
				}
				if (UNIT_MM == get_unit(pp->p_config))
				{
					cur_value = (GROUP_GATE_POS(width) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
					lower = 0.02 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
					upper = ((MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0) > 6400.0 ? 6400.0 :
							(MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0)) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
					//step = tmpf * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
					step = tmpf;
					digit = 2;
					pos = 3;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = (GROUP_GATE_POS(width) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
					lower =	0.02 * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
					upper =	((MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0) > 6400.0 ? 6400.0 :
							(MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0)) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
					//step = tmpf * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
					step = tmpf;
					digit = 3;
					pos = 3;
					unit = UNIT_INCH;
				}
			}
			else /*ut_unit 选择 time 时*/
			{
				switch (TMP(gate_width_reg))
				{
					case 0:	tmpf = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) / (double)(GROUP_VAL_POS( grp , point_qty)); break;
					case 1:	tmpf = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) / 20.0 ; break;
					case 2:	tmpf = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) / 10.0 ; break;
					case 3:	tmpf = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) / 4.0 ; break;
					default:break;
				}
				cur_value = GROUP_GATE_POS(width) / 1000.0 ;
				lower =	0.02;
                upper =	((MAX_RANGE_US - GROUP_GATE_POS(start) /1000.0 ) > 6400.0 ? 6400.0 : (MAX_RANGE_US - group_get_start (grp) / 1000.0));
				step = tmpf;
				digit = 2;
				pos = 3;
				unit = UNIT_US;
			}
			draw3_digit_pressed (data_203, units[unit], cur_value ,
					lower, upper, step, digit, pp, pos, 0);
		}
		else
		{
			if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) ||
					(UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
			{
				if (UNIT_MM == get_unit(pp->p_config))
				{
					cur_value = (GROUP_GATE_POS(width) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
					unit = UNIT_MM;
					digit = 2;
					pos = 3;
				}
				else
				{
					cur_value = (GROUP_GATE_POS(width) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
					unit = UNIT_INCH;
					digit = 3;
					pos = 3;
				}
			}
			else
			{
				cur_value = GROUP_GATE_POS(width) / 1000.0 ;
				unit = UNIT_US;
				pos = 3;
				digit = 2;
			}
			draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
		}
	}
	else
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
		{
			draw3_pop_tt (data_2031, NULL,
					menu_content[MEASURE + GROUP_GATE_POS(measure)],
					menu_content + MEASURE, 2, 3, GROUP_GATE_POS(measure), 0);
			str = g_strdup_printf ("%s", con2_p[2][0][7]);
			gtk_label_set_text (GTK_LABEL (pp->label3[3]), str);
		}
		else
		{
			draw3_popdown (menu_content[MEASURE + GROUP_GATE_POS(measure)], 3, 0);
			str = g_strdup_printf ("%s", con2_p[2][0][7]);
			gtk_label_set_text (GTK_LABEL (pp->label3[3]), str);
		}
		if (str) {
			g_free(str);
        }
	}
}

void DrawMenu213(){}
void DrawMenu223(){}

void DrawMenu233()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);
	int _nBeamNo = TMP(beam_num[grp]) ;

	if( GROUP_VAL_POS( grp , SizingCurves.mode_pos)==0 ) /* Mode 为 setup 时 */
	{
		if(GROUP_VAL_POS( grp , SizingCurves.curve_pos)==1 || GROUP_VAL_POS( grp , SizingCurves.curve_pos)==2) /*Curve 选择 DAC or Linear DAC 时*/
		{
			/* 当前步进 */
			switch (TMP(ref_ampl_offset_reg))
			{
				case 0:	tmpf = 1; break;
				case 1:	tmpf = 2; break;
				case 2:	tmpf = 6; break;
				case 3:	tmpf = 0.1; break;
				case 4:	tmpf = 0.5; break;
				default:break;
			}
			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
			{
				cur_value =GROUP_VAL_POS( grp , SizingCurves.ref_ampl_offset)/100.0;
				lower = -40;
				upper = 40;
				step = tmpf;
				digit = 1;
				pos = 3;
				unit = UNIT_DB;
				draw3_digit_pressed (data_233, units[unit], cur_value , lower, upper, step, digit, pp, pos, 7);
			}
			else
			{
				cur_value = GROUP_VAL_POS( grp , SizingCurves.ref_ampl_offset)/100.0;
				digit = 1;
				pos = 3;
				unit = UNIT_DB;
				draw3_digit_stop (cur_value, units[unit], digit, pos, 7);
			}
		}
		else
		{
			gtk_widget_hide (pp->eventbox30[3]);
			gtk_widget_hide (pp->eventbox31[3]);
			gtk_widget_hide (pp->sbutton[3]);
		}
	}
	else if( GROUP_VAL_POS( grp , SizingCurves.mode_pos)==1 ) /* Mode 为 edit 时 */
	{
		if(GROUP_VAL_POS( grp , SizingCurves.curve_pos)==1)/*Mode 选择 Edit , Curve 选择 DAC 时*/
		{
			/* 当前步进 */
			switch (TMP(amplitude_reg))
			{
				case 0:	tmpf = 1; break;
				case 1:	tmpf = 10; break;
				case 2:	tmpf = 0.1; break;
				default:break;
			}
			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
			{
				cur_value =GROUP_VAL_POS( grp , SizingCurves.amplitude[_nBeamNo][GROUP_VAL_POS( grp , SizingCurves.point_pos)])/1000.0;
				upper = GROUP_VAL_POS(grp , SizingCurves.dac_ref_ampl[_nBeamNo]) / 10 ; //80 ;
				lower = DacAmptitudeMinimumLimit(grp) ;
				step = tmpf;
				digit = 2;
				pos = 3;
				unit = UNIT_BFH;
				draw3_digit_pressed (data_2331, units[unit], cur_value , lower, upper, step, digit, pp, pos, 12);
			}
			else
			{
				cur_value = GROUP_VAL_POS( grp , SizingCurves.amplitude[_nBeamNo][GROUP_VAL_POS( grp ,SizingCurves.point_pos)])/1000.0;
				digit = 2;
				pos = 3;
				unit = UNIT_BFH;
				draw3_digit_stop (cur_value, units[unit], digit, pos, 12);
			}

			if(GROUP_VAL_POS( grp , SizingCurves.point_pos) == 0)
			{
				gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
				gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
			}
		}
		else if(GROUP_VAL_POS( grp , SizingCurves.curve_pos)==2) /*Mode 选择 Edit , Curve 选择 Linear DAC 时*/
		{
			gtk_widget_hide (pp->eventbox30[3]);
			gtk_widget_hide (pp->eventbox31[3]);
			gtk_widget_hide (pp->sbutton[3]);
		}
		else if(GROUP_VAL_POS( grp , SizingCurves.curve_pos)==3) /*Mode 选择 Edit , Curve 选择 TCG 时*/
		{
			/* 当前步进 */
			switch (TMP(tcg_gain_reg))
			{
				case 0:	tmpf = 1; break;
				case 1:	tmpf = 2; break;
				case 2:	tmpf = 6; break;
				case 3:	tmpf = 0.1; break;
				case 4:	tmpf = 0.5; break;
				default:break;
			}
			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
			{
				cur_value = GetTcgCurrentPointValue(grp) ;
				lower = 0.0;
				upper = CurrentPointTcgGainLimit(grp);
				step = tmpf;
				digit = 1;
				pos = 3;
				unit = UNIT_DB;
				draw3_digit_pressed (data_2332, units[unit], cur_value , lower, upper, step, digit, pp, pos, 17);
			}
			else
			{
				cur_value = GetTcgCurrentPointValue(grp) ;
				digit = 1;
				pos = 3  ;
				unit = UNIT_DB;
				draw3_digit_stop (cur_value, units[unit], digit, pos, 17);
			}
			if(GROUP_VAL_POS( grp , SizingCurves.point_pos)==0)
			{
				gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
				gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
			}
		}
		else
		{
			gtk_widget_hide (pp->eventbox30[3]);
			gtk_widget_hide (pp->eventbox31[3]);
			gtk_widget_hide (pp->sbutton[3]);
		}

	}

}

void DrawMenu243()
{
	if ( !con2_p[2][4][3] )
		gtk_widget_hide (pp->eventbox30[3]);
	gtk_widget_hide (pp->eventbox31[3]);
}

void DrawMenu303()
{
	pp->x_pos = 200, pp->y_pos = 0;
	int feildNum = GetMesureFeildNum();
	const char* pSimpleStr[feildNum];
	const char* pStr[feildNum];
	GetMesureFeildString(pSimpleStr ,pStr);
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
		draw3_pop_tt_shen (data_303, NULL,
				pSimpleStr[get_reading_field2(pp->p_config)],
				pStr, feildNum, 3, get_reading_field2(pp->p_config), 0);
	else
		draw3_popdown (pSimpleStr[get_reading_field2(pp->p_config)], 3, 0);
}
void DrawMenu313()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);
	int _nSelection = GROUP_VAL_POS(grp , selection )  ;
	if( _nSelection == 0 || _nSelection == 1)
	{
		/* 当前步进 */
		switch (TMP(u_reference_reg))
		{
			case 0:	tmpf = 0.01; break;
			case 1:	tmpf = 0.1;  break;
			case 2:	tmpf = 1.0;  break;
			case 3:	tmpf = 10.0;  break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
		{
			double _nVelocity = GROUP_VAL_POS(grp , velocity) / 200000;
			if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					cur_value = GROUP_VAL_POS( grp , u_reference)/1000.0 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
					//lower = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
					//upper = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)
					//	+get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
					lower = 0 ;
					upper = 1000 * _nVelocity;
					step = tmpf;
					digit = 2;
					pos = 3;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = GROUP_VAL_POS( grp , u_reference) / 1000.0 * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
					//lower = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)*0.03937;
					//upper = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)*0.03937+get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0)*0.03937;
					lower = 0 ;
					upper = 1000 * _nVelocity * 0.03937;
					step = tmpf/2.0;
					digit = 3;
					pos = 3;
					unit = UNIT_INCH;
				}
			}
			else	/* us */
			{
				cur_value = GROUP_VAL_POS( grp , u_reference)/1000.0;
				//lower = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START) / 1000.0;
				//upper = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START) / 1000.0 + get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0 ;
				lower = 0 ;
				upper = 1000;
				step = tmpf;
				digit = 2;
				pos = 3;
				unit = UNIT_US;
			}
			draw3_digit_pressed (data_313, units[unit], cur_value , lower, upper, step, digit, pp, pos, 0);
		}
		else
		{
			if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					cur_value = GROUP_VAL_POS( grp , u_reference)/1000.0 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
					digit = 2;
					pos = 3;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = GROUP_VAL_POS( grp , u_reference)/1000.0 * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
					digit = 3;
					pos = 3;
					unit = UNIT_INCH;
				}
			}
			else
			{
				cur_value = GROUP_VAL_POS( grp , u_reference)/1000.0;
				digit = 2;
				pos = 3;
				unit = UNIT_US;
			}
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		}
	}
	else if( _nSelection == 2)
	{
		/* 当前步进 */
		switch (TMP(i_reference_reg))
		{
			case 0:	tmpf = 0.1; break;
			case 1:	tmpf = 0.5;  break;
			case 2:	tmpf = 1.0;  break;
			case 3:	tmpf = 1.5;  break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
		{
			if(get_unit(pp->p_config) == UNIT_MM)
			{
				cur_value = GROUP_VAL_POS( grp , i_reference)/100.0;
				lower = -10000.0;
				upper = 10000.0;
				step = tmpf;
				digit = 1;
				pos = 3;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = GROUP_VAL_POS( grp , i_reference)/100.0*0.03937;
				lower = -10000.0*0.03937;
				upper = 10000*0.03937;
				step = tmpf;
				digit = 3;
				pos = 3;
				unit = UNIT_INCH;
			}
			draw3_digit_pressed (data_3131, units[unit], cur_value , lower, upper, step, digit, pp, pos, 9);
		}
		else
		{
			if(get_unit(pp->p_config) == UNIT_MM)
			{
				cur_value = GROUP_VAL_POS( grp , i_reference)/100.0;
				digit = 1;
				pos = 3;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = GROUP_VAL_POS( grp , i_reference)/100.0*0.03937;
				digit = 3;
				pos = 3;
				unit = UNIT_INCH;
			}
			draw3_digit_stop (cur_value, units[unit], digit, pos, 9);
		}
	}
	else if( _nSelection == 3)
	{
		/* 当前步进 */
		switch (TMP(u_measure_reg))
		{
			case 0:	tmpf = 0.01; break;
			case 1:	tmpf = 0.1;  break;
			case 2:	tmpf = 1.0;  break;
			case 3:	tmpf = 10.0;  break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
		{
			double _nVelocity = GROUP_VAL_POS(grp , velocity) / 200000;
			if((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					cur_value = GROUP_VAL_POS( grp , u_measure)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
					lower = 0 ;
					upper = 1000 * _nVelocity;
					step = tmpf;
					digit = 2;
					pos = 3;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = GROUP_VAL_POS( grp , u_measure)/1000.0*0.03937*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
					lower = 0 ;
					upper = 1000 * _nVelocity * 0.03937;
					step = tmpf;
					digit = 2;
					pos = 3;
					unit = UNIT_INCH;
				}
			}
			else	/*us*/
			{
				cur_value = GROUP_VAL_POS( grp , u_measure)/1000.0;
				lower = 0 ;
			    upper = 1000 ;
				step = tmpf;
				digit = 2;
				pos = 3;
				unit = UNIT_US;
			}
			draw3_digit_pressed (data_314, units[unit], cur_value , lower, upper, step, digit, pp, pos, 4);
		}
		else
		{
			if((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					cur_value = GROUP_VAL_POS( grp , u_measure)/1000.0*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
					digit = 2;
					pos = 3;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = GROUP_VAL_POS( grp , u_measure)/1000.0*0.03937*(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
					digit = 3;
					pos = 3;
					unit = UNIT_INCH;
				}
			}
			else	/*????*/
			{
				cur_value = GROUP_VAL_POS( grp , u_measure)/1000.0;
				digit = 2;
				pos = 3;
				unit = UNIT_US;
			}
			draw3_digit_stop (cur_value, units[unit], digit, pos, 4);
		}

	}
	else if (_nSelection == 4|| _nSelection == 5||_nSelection == 6)
	{
		/* 当前步进 */
		switch (TMP(cursors_ut_reg))
		{
			case 0:	tmpf = 0.01; break;
			case 1:	tmpf = 0.1; break;
			case 2:	tmpf = 1.0; break;
			case 3:	tmpf = 10.0; break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
		{
			if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					cur_value = GROUP_CURSORS_POS(UT)/100.0;
					lower = -0.07;
					upper = 58.3;
					step = tmpf;
					digit = 2;
					pos = 3;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = GROUP_CURSORS_POS(UT)/100.0*0.03937;
					lower = -0.07*0.03937;
					upper = 58.3*0.03937;
					step = tmpf;
					digit = 3;
					pos = 3;
					unit = UNIT_INCH;
				}
			}
			else
			{
				cur_value = GROUP_CURSORS_POS(UT)/100.0;
				lower = -0.07;
				upper = 58.3;
				step = tmpf;
				digit = 2;
				pos = 3;
				unit = UNIT_US;

			}
			draw3_digit_pressed (data_3133, units[unit], cur_value , lower, upper, step, digit, pp, pos, 14);
		}
		else
		{
			if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					cur_value = GROUP_CURSORS_POS(UT)/100.0;
					digit = 2;
					pos = 3;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = GROUP_CURSORS_POS(UT)/100.0*0.03937;
					digit = 3;
					pos = 3;
					unit = UNIT_INCH;
				}
			}
			else
			{
				cur_value = GROUP_CURSORS_POS(UT)/100.0;
				digit = 2;
				pos = 3;
				unit = UNIT_US;
			}
			draw3_digit_stop (cur_value, units[unit], digit, pos, 14);
		}
	}
	else if (_nSelection == 7)
	{
		/* 当前步进 */
		switch (TMP(cursors_index_reg))
		{
			case 0:	tmpf = 0.5; break;
			case 1:	tmpf = 2.5; break;
			case 2:	tmpf = 5.0; break;
			case 3:	tmpf = 7.5; break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
		{
			if(get_unit(pp->p_config) == UNIT_MM)
			{
				cur_value = TMP(index)/100.0;
				lower = -100000.0;
				upper = 100000;
				step = tmpf;
				digit = 1;
				pos = 3;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = TMP(index)/100.0*0.03937;
				lower = -100000.0*0.03937;
				upper = 100000*0.03937;
				step = tmpf;
				digit = 3;
				pos = 3;
				unit = UNIT_INCH;
			}
			draw3_digit_pressed (data_315, units[unit], cur_value , lower, upper, step, digit, pp, pos, 16);
		}
		else
		{
			if(get_unit(pp->p_config) == UNIT_MM)
			{
				cur_value = TMP(index)/100.0;
				digit = 1;
				pos = 3;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = TMP(index)/100.0*0.03937;
				digit = 3;
				pos = 3;
				unit = UNIT_INCH;
			}
			draw3_digit_stop (cur_value, units[unit], digit, pos, 16);
		}

	}
	gtk_widget_queue_draw(pp->draw_area->drawing_area);
}

void DrawMenu323()
{
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
		tableDeleteEntry();
	draw3_popdown(NULL,3,1);
	if(!get_display_table(pp->p_config))
	{
		gtk_widget_set_sensitive(pp->eventbox30[3] ,FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[3] ,FALSE);
	}
	else
	{
	}
}

void DrawMenu333()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos, unit = 0;
	/* 当前步进 */
	switch (TMP(echo_qty_reg))
	{
		case 0:	tmpf = 1.0; break;

		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
	{
		cur_value = get_echo_qty(pp->p_config);
		lower = 1.0;
		upper = 10.0;
		step = tmpf;
		digit = 0;
		pos = 3;
		unit = UNIT_NONE;
		draw3_digit_pressed (data_333, units[unit], cur_value , lower, upper, step, digit, pp, pos, 0);
	}
	else
	{
		cur_value = get_echo_qty(pp->p_config);
		digit = 0;
		pos = 3;
		unit = UNIT_NONE;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
	}
}

void DrawMenu343()
{
	if ( !con2_p[3][4][3] )
		gtk_widget_hide (pp->eventbox30[3]);
	gtk_widget_hide (pp->eventbox31[3]);
}

void DrawMenu403()
{
	char *str;
	int menu_status = 0;
    unsigned char displayPos = get_display_pos(pp->p_config);

	pp->x_pos = 550, pp->y_pos = 410;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))/*选中403这个位置*/
	{
		if(get_display_pos(pp->p_config)==A_C_CC_SCAN)
			/*Display 为 A-C-[C]*/
		{
			if (get_group_qty(pp->p_config) == 1)
				menu_status = 0x01;
			draw3_pop_tt (data_401, NULL,
					menu_content[GROUP_POS+get_display_group(pp->p_config)],
					menu_content + GROUP_POS, 2, 3, get_display_group(pp->p_config), menu_status);
			str = g_strdup_printf ("%s", con2_p[4][0][1]);
			gtk_label_set_text (GTK_LABEL (pp->label3[3]), str);

		}
		else if(get_display_pos(pp->p_config)==Strip_Chart_AA)
			/*Display 为 Strip Chart-[A]*/
		{
			draw3_pop_tt (data_403, NULL,
					menu_content[DIS_MODE+get_stripscan_mode(pp->p_config)],
					menu_content + DIS_MODE, 4, 3, get_stripscan_mode(pp->p_config), 0);
			str = g_strdup_printf ("%s", con2_p[4][0][10]);
			gtk_label_set_text (GTK_LABEL (pp->label3[3]), str);

		}
		else
		{
			gtk_widget_hide (pp->eventbox30[3]);
			gtk_widget_hide (pp->eventbox31[3]);
		}

	}
	else
	{
		if(displayPos == A_C_CC_SCAN)/* Display 为 A-C-[C] */
		{
			draw3_popdown (menu_content[GROUP_POS+get_display_group(pp->p_config)], 3, 0);
			str = g_strdup_printf ("%s", con2_p[4][0][1]);
			gtk_label_set_text (GTK_LABEL (pp->label3[3]), str);
		}
		else if(displayPos == Strip_Chart_AA)
			/*Display 为 Strip Chart-[A]*/
		{
			draw3_popdown (menu_content[DIS_MODE+get_stripscan_mode(pp->p_config)], 3, 0);
			str = g_strdup_printf ("%s", con2_p[4][0][10]);
			gtk_label_set_text (GTK_LABEL (pp->label3[3]), str);

		}

		else
		{
			gtk_widget_hide (pp->eventbox30[3]);
			gtk_widget_hide (pp->eventbox31[3]);
		}
	}
}

void DrawMenu413()
{
	draw3_popdown(menu_content[OFF_ON + get_overlay_gate (pp->p_config)],3,0);
	gtk_widget_queue_draw(pp->vboxtable);
}

void DrawMenu423()
{
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
	data_423(NULL , NULL);
	 draw3_popdown(NULL, 3, 1);
}
void DrawMenu433()
{
	int grp ; //= get_current_group(pp->p_config);
	pp->x_pos = 567, pp->y_pos =368-26;
	switch(get_dis_prop_scan(pp->p_config))
	{
		case 0:	/* Ascan 时候的source */
			grp = get_current_group(pp->p_config);
			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
				draw3_pop_tt (data_444, NULL,
						menu_content[PROP_APP + GROUP_VAL_POS( grp , ascan_appearance)],
						menu_content+PROP_APP, 4, 3, GROUP_VAL_POS( grp , ascan_appearance), 0);
			else
				draw3_popdown (menu_content[PROP_APP + GROUP_VAL_POS( grp , ascan_appearance)], 3, 0);
			break;
		case 1:
			gtk_widget_hide(pp->eventbox30[3]);
			gtk_widget_hide(pp->eventbox31[3]);
			break;
		case 2:
		case 3:
			gtk_widget_hide(pp->eventbox30[3]);
			gtk_widget_hide(pp->eventbox31[3]);
			break;
		default:break;

	}
}

void DrawMenu443()
{
	if ( !con2_p[4][4][3] )
		gtk_widget_hide (pp->eventbox30[3]);
	gtk_widget_hide (pp->eventbox31[3]);
}

//extern GtkWidget* ProbeDialogNew(GtkWidget* fatherWidget);
void DrawMenu503()
{
	char temp[52];
    int grp = get_current_group(pp->p_config) ;

	if(get_auto_detect (pp->p_config))
	{
		if(get_probe_select (pp->p_config)==0)
		{
			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
			{
				draw_dialog_all (DIALOG_PROBE);
			}
			else
				draw3_popdown(GROUP_VAL_POS( grp , probe.Model), 3,0);

			g_sprintf (temp,"%s", con2_p[5][0][3]);
			gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
		}
		else if(get_probe_select (pp->p_config)==1)
		{
			draw3_popdown(NULL,3,1);
			g_sprintf (temp,"%s", con2_p[5][0][6]);
			gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
		}
		gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
	}
	else
	{
		if(get_probe_select (pp->p_config)==0)
		{
			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
			{
				draw_dialog_all (DIALOG_PROBE);
			}
			else
				draw3_popdown(GROUP_VAL_POS( grp , probe.Model), 3, 0);

			g_sprintf (temp,"%s", con2_p[5][0][3]);
			gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
			gtk_label_set_text (GTK_LABEL (pp->data3[3]), GROUP_VAL_POS( grp , probe.Model));
		}
		else if( get_probe_select (pp->p_config)==1 )
		{
			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
			{
				draw_dialog_all (DIALOG_DEFINE_PROBE);
			}
			else
			{
				draw3_popdown(GROUP_VAL_POS( grp , probe.Model), 3,0);
			}
			g_sprintf (temp,"%s", con2_p[5][0][6]);
			gtk_label_set_text (GTK_LABEL (pp->label3[3]), temp);
		}
	}
	if( (GROUP_VAL_POS( grp , group_mode)==1) && get_auto_detect (pp->p_config))	/* 自动检测开启时同时又在PA模式时，探头不可选 */
	{
		gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
	}
	else
	{
		gtk_widget_set_sensitive(pp->eventbox30[3],TRUE);
		gtk_widget_set_sensitive(pp->eventbox31[3],TRUE);
	}
}

void DrawMenu513()
{
	int grp = get_current_group(pp->p_config);
	if(GROUP_VAL_POS(grp , tx_rxmode1) != TOFD)
	{
		gtk_widget_hide (pp->eventbox30[3]);
	    gtk_widget_hide (pp->eventbox31[3]);
	    return ;
	}
	double cur_value=0.0;
	int digit = 0, pos, unit = 0;
	if (UNIT_MM == get_unit(pp->p_config))
	{
		cur_value = GROUP_VAL_POS(grp , field_distance[254]);   /* 当前显示的范围数值mm */
		unit = UNIT_MM;
		digit = 2;
		pos = 3;
	}
	else
	{
		cur_value = GROUP_VAL_POS(grp , field_distance[254]) * 0.03937 ; /* 当前显示的范围inch */
		unit = UNIT_INCH;
		digit = 3;
		pos = 3;
	}

	draw3_digit_stop (cur_value , units[unit], digit, pos, 0);

	gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
	gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);

}

void DrawMenu523()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);
	/* 当前步进 */
	switch (TMP(gate_width_reg))
	{
	    case 0:	tmpf = 1; break;
	    case 1:	tmpf = 10; break;
		case 2:	tmpf = 0.01; break;
		case 3:	tmpf = 0.1; break;

		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
	{
		if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
				cur_value = (GROUP_VAL_POS( grp , gate[0].width) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
				lower = 0.01 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
				upper = ((MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0) > 6400.0 ? 6400.0 :
						(MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0)) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
				step = tmpf ;//* (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
				digit = 2;
				pos = 3;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = (GROUP_VAL_POS( grp , gate[0].width) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
				lower = 0.01 * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
				upper =	((MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0) > 6400.0 ? 6400.0 :
						(MAX_RANGE_US - GROUP_GATE_POS(start) / 1000.0)) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
				step = tmpf;// * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
				digit = 3;
				pos = 3;
				unit = UNIT_INCH;
			}
		}
		else
		{
			cur_value = GROUP_VAL_POS( grp , gate[0].width) / 1000.0 ;
			lower = 0.01;
            upper =	((MAX_RANGE_US - GROUP_GATE_POS(start) /1000.0 ) > 6400.0 ? 6400.0 : (MAX_RANGE_US - group_get_start (grp) / 1000.0));
			step = tmpf;
			digit = 2;
			pos = 3;
			unit = UNIT_US;
		}
		draw3_digit_pressed (data_203, units[unit], cur_value , lower, upper, step, digit, pp, pos, 0);
	}
	else
	{
		if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
				cur_value = (GROUP_VAL_POS( grp , gate[0].width) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
				unit = UNIT_MM;
				digit = 2;
				pos = 3;
			}
			else
			{
				cur_value = (GROUP_VAL_POS( grp , gate[0].width) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
				unit = UNIT_INCH;
				digit = 3;
				pos = 3;
			}
		}
		else
		{
			cur_value = GROUP_VAL_POS( grp , gate[0].width) / 1000.0 ;
			unit = UNIT_US;
			pos = 3;
			digit = 2;
		}
		draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
	}
}

void DrawMenu533()
{
	pp->x_pos = 520, pp->y_pos = 50;
	int grp = get_current_group(pp->p_config);
	int _nMPos = GROUP_VAL_POS(grp , part.Material_pos) ;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
		draw3_pop_tt (data_533, NULL,
				get_material_name(_nMPos),
				menu_content + MATERIAL, 24, 3, get_part_material(grp), 0);
	else
		draw3_popdown (get_material_name(_nMPos), 3, 0);
}

void DrawMenu543()
{
	if ( !con2_p[5][4][3] )
		gtk_widget_hide (pp->eventbox30[3]);
	gtk_widget_hide (pp->eventbox31[3]);
}

void DrawMenu603()
{
	//if ( !con2_p[6][0][3] )
	gtk_widget_hide (pp->eventbox30[3]);
	gtk_widget_hide (pp->eventbox31[3]);
}

void DrawMenu613()
{
	gtk_widget_hide (pp->eventbox30[3]);
    gtk_widget_hide (pp->eventbox31[3]);
    return ;
    //*******************************************************
	double tmpf = 0.0;
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos, unit = 0, temp_beam, temp_beam1;
	int grp = get_current_group (pp->p_config);

	switch (TMP(beam_skew_min_angle_reg))
	{
		case 0:	tmpf = 0.1; break;
		case 1:	tmpf = 1.0; break;
		case 2:	tmpf = 10.0; break;
		default:break;
	}
	if ((LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN) &&
			(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON)
			&& (GROUP_VAL_POS( grp , probe.PA_probe_type) == 9)
			/* 如何判断2D探头 */
	   )
		/* 聚焦法则自动计算开启时, Min Angle 才可调节 */
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
		{
			/* 最大不能超过最大Angle_beam_skew_max */
			cur_value = LAW_VAL_POS(grp , Angle_beam_skew_min) / 100.0;
			/* 计算lower为妙 */
			temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[get_current_group(pp->p_config)]);
			TMP(angle_num)	= (LAW_VAL_POS(grp , Angle_max) - LAW_VAL_POS(grp , Angle_min)) /
				LAW_VAL_POS(grp , Angle_step) + 1;
			temp_beam1 = temp_beam / TMP(angle_num);
			lower = MAX (((gint)(LAW_VAL_POS(grp , Angle_beam_skew_max)) -
						(gint)(temp_beam1 * LAW_VAL_POS(grp , Angle_beam_skew_step))) / 100.0, -89.9);
			upper = LAW_VAL_POS(grp , Angle_beam_skew_max) / 100.0;
			step = tmpf;
			digit = 1;
			pos = 3;
			unit = UNIT_DEG;
			draw3_digit_pressed (data_613, units[unit], cur_value , lower,
					upper, step, digit, pp, pos, 0);
		}
		else
		{
			cur_value = LAW_VAL_POS(grp , Angle_beam_skew_min) / 100.0;
			digit = 1;
			pos = 3;
			unit = UNIT_DEG;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		}
	}
	else /* 聚焦法则自动计算为OFF, Min.Angle 不可调节 */
	{
		cur_value = LAW_VAL_POS(grp , Angle_beam_skew_min) / 100.0;
		digit = 1;
		pos = 3;
		unit = UNIT_DEG;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
	}
}
void DrawMenu623()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);
	float multiplier = 1.0;
	if(UNIT_MM == get_unit(pp->p_config))
	{
		multiplier = 1.0;
	}else{
		multiplier = 0.03937;
	}
	if(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON)
		/* 聚焦法则自动计算开启时, Max Angle 才可调节 */
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
		{
			if(LAW_VAL_POS(grp , Focal_type) == 2)
				/*law type 为 depth sector 时*/
			{
				switch (TMP(positionstep_reg))
				{
					case 0:	tmpf = 0.1; break;
					case 1:	tmpf = 1.0; break;
					case 2:	tmpf = 10.0; break;
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
				cur_value = LAW_VAL_POS(grp , Position_step) / 1000.0 * multiplier;
				lower = 0.1 * multiplier;
				upper = 10000.0 * multiplier;
				step = tmpf;
				pos = 3;

				draw3_digit_pressed (data_623, units[unit], cur_value , lower, upper,
						step, digit, pp, pos, 0);
			}
			else if(LAW_VAL_POS(grp , Focal_point_type)==FOCALPLANE_P)
				/*type 为 focal plane 时*/
			{
				switch (TMP(depths_reg))
				{
					case 0:	tmpf = 0.5; break;
					case 1:	tmpf = 5.0; break;
					case 2:	tmpf = 50.0;break;
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
				cur_value = LAW_VAL_POS(grp , Depth_start) / 1000.0 * multiplier;
				lower = 0.1 * multiplier;
				upper = 10000.0 * multiplier;
				step = tmpf;
				pos = 3;

				draw3_digit_pressed (data_6231, units[unit], cur_value , lower, upper,
						step, digit, pp, pos, 8);
			}
			else
			{
				if(UNIT_MM == get_unit(pp->p_config))
				{
					unit = UNIT_MM;
					digit = 2;
				}else{
					unit = UNIT_INCH;
					digit = 3;
				}
				cur_value = LAW_VAL_POS(grp , Position_step) / 1000.0 * multiplier;
				pos = 3;
				draw3_digit_stop (cur_value, units[unit], digit, pos, 0);

				gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
				gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
			}
		}
		else
		{
			if(LAW_VAL_POS(grp , Focal_type) == 2)
				/*law type 为 depth sector 时*/
			{
				if(UNIT_MM == get_unit(pp->p_config))
				{
					unit = UNIT_MM;
					digit = 2;
				}else{
					unit = UNIT_INCH;
					digit = 3;
				}
				cur_value = LAW_VAL_POS(grp , Position_step) / 1000.0 * multiplier;
				pos = 3;
				draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
			}
			else if(LAW_VAL_POS(grp , Focal_point_type)==FOCALPLANE_P)
				/*type 为 focal plane 时*/
			{
				if(UNIT_MM == get_unit(pp->p_config))
				{
					unit = UNIT_MM;
					digit = 2;
				}else{
					unit = UNIT_INCH;
					digit = 3;
				}
				cur_value = LAW_VAL_POS(grp , Depth_start) / 1000.0 * multiplier;
				lower = 0.1 * multiplier;
				upper = 10000.0 * multiplier;
				step = tmpf;
				pos = 3;
				draw3_digit_stop (cur_value, units[unit], digit, pos, 8);
			}
			else
			{
				if(UNIT_MM == get_unit(pp->p_config))
				{
					unit = UNIT_MM;
					digit = 2;
				}else{
					unit = UNIT_INCH;
					digit = 3;
				}
				cur_value = LAW_VAL_POS(grp , Position_step) / 1000.0 * multiplier;
				pos = 3;
				draw3_digit_stop (cur_value, units[unit], digit, pos, 0);

				gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
				gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
			}
		}
	}
	else
	{
		if(UNIT_MM == get_unit(pp->p_config))
		{
			unit = UNIT_MM;
			digit = 2;
		}else{
			unit = UNIT_INCH;
			digit = 3;
		}
		cur_value = LAW_VAL_POS(grp , Position_step) / 1000.0 * multiplier;
		pos = 3;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);

		gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
	}
}

void DrawMenu633()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);

	switch (pp->p_tmp_config->element_step_reg)
	{
		case 0:	tmpf = 1.0; break;
		case 1:	tmpf = 10.0; break;
		case 2:	tmpf = 100.0; break;
		default:break;
	}
	if ((get_auto_focal (pp->p_config) == AUTO_FOCAL_ON) &&
			(LAW_VAL_POS(grp , Focal_type) == LINEAR_SCAN))
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
		{
			cur_value = LAW_VAL_POS(grp , Elem_step);
			lower = 1.0;
			upper = MIN(gMaxElementRecieveQty , GROUP_VAL_POS (grp , probe.Elem_qty)) ;
			step = tmpf;
			digit = 1;
			pos = 3;
			unit = UNIT_NONE;
			draw3_digit_pressed (data_633, units[unit], cur_value , lower, upper, step, digit, pp, pos, 0);
		}
		else
		{
			cur_value = LAW_VAL_POS(grp , Elem_step);
			digit = 1;
			pos = 3;
			unit = UNIT_NONE;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		}
	}
	else /*Law Config 不为 Linear 时，Element Step  Unsensitive*/
	{
		cur_value = LAW_VAL_POS(grp , Elem_step);
		digit = 1;
		pos = 3;
		unit = UNIT_NONE;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
	}
}

void DrawMenu643()
{
	draw3_popdown (NULL, 3, 1);
	if (get_auto_focal (pp->p_config) == AUTO_FOCAL_OFF)
	{
		//gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
		//gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
	}
}

void DrawMenu703()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos, unit = 0;
	/* 当前步进 */
	switch (TMP(encoder_resolution_reg))
	{
		case 0:	tmpf = 0.01; break;
		case 1:	tmpf = 0.1; break;
		case 2:	tmpf = 1.0; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
	{
		cur_value = get_enc_resolution (pp->p_config, get_cur_encoder (pp->p_config))/1000.0;
		lower   = pp->p_config->inspection_scan_resolution / 1000.0;
		lower = 2 / lower ;
		lower = MAX(lower , 0.1) ;

		upper = 2000.00;
		step = tmpf;
		digit = 3;
		pos = 3;
		unit = UNIT_STEP_MM;
		draw3_digit_pressed (data_703, units[unit], cur_value , lower, upper, step, digit, pp, pos, 0);
	}
	else
	{
		cur_value = get_enc_resolution (pp->p_config, get_cur_encoder (pp->p_config))/1000.0;
		digit = 3;
		pos = 3;
		unit = UNIT_STEP_MM;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
	}
	if( ((get_inspec_type (pp->p_config)==0) && (get_inspec_source (pp->p_config)==0)) || ((get_inspec_type (pp->p_config)==2)&&(get_inspec_source (pp->p_config)==0)&& ((get_inspec_index (pp->p_config)==0)||(get_inspec_index (pp->p_config)==3))) )
	{
		gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
	}
}

void DrawMenu713()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos, unit = 0 , content_pos = 0;

	switch (TMP(scanspeed_reg))
	{
		case 0:	tmpf = 1.0; break;
		case 1:	tmpf = 10.0; break;
		case 2:	tmpf = 100.0; break;
		default:break;
	}

	if(get_inspec_source (pp->p_config)==0)
		content_pos=0;
	else
		content_pos=6;

	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
	{
		cur_value = get_inspec_speed (pp->p_config) / 1000.0;
		lower = get_area_scanresolution (pp->p_config) / 1000.0;
		upper = get_area_scanresolution (pp->p_config) / 10000.0;
		upper = lower * MultiGroupGetMaxPrf() / 10;
		upper = MIN(10000 , upper);
		step = tmpf;
		pos = 3;

		if(UNIT_MM == get_unit(pp->p_config))
		{
			digit = 2;
			unit = UNIT_MM_S;
		}
		else
		{
			cur_value *=  0.03937;
			lower *=  0.03937;
			upper *=  0.03937;
			step  *=  0.03937;
			digit = 3;
			unit = UNIT_INCH_S;
		}
		draw3_digit_pressed (data_713, units[unit], cur_value , lower, upper, step, digit, pp, pos, content_pos);
	}
	else
	{
		if(UNIT_MM == get_unit(pp->p_config))
		{
			cur_value = get_inspec_speed (pp->p_config)/1000.0;
			digit = 2;
			pos = 3;
			unit = UNIT_MM_S;
		}
		else
		{
			cur_value = get_inspec_speed (pp->p_config)/1000.0 * 0.03937;
			digit = 3;
			pos = 3;
			unit = UNIT_INCH_S;
		}
		draw3_digit_stop (cur_value, units[unit], digit, pos, content_pos);
	}
}
void DrawMenu723()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos, unit = 0;
	/* 当前步进 */
	switch (TMP(index_start_reg))
	{
		case 0:	tmpf = 0.001 * get_area_indexresolution (pp->p_config); break;
		case 1:	tmpf = 0.01 * get_area_indexresolution (pp->p_config); break;
		case 2:	tmpf = 0.1 * get_area_indexresolution (pp->p_config); break;
		default:break;
	}
	if(get_inspec_type (pp->p_config)==1 || get_inspec_type (pp->p_config)==2)
		/* Inspection -> Type 选择 Helicoidal Scan \ Raster Scan  时 */
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
		{
			if(get_unit(pp->p_config) == UNIT_MM)
			{
				cur_value = get_area_indexstart (pp->p_config)/1000.0;
				lower = -99999.0;
				upper = 99999.0;
				step = tmpf;
				digit = 2;
				pos = 3;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = get_area_indexstart (pp->p_config) / 1000.0 * 0.03937;
				lower = -99999.0 * 0.03937;
				upper = 99999.0 * 0.03937;
				step = tmpf*0.03937;
				digit = 3;
				pos = 3;
				unit = UNIT_INCH;
			}
			draw3_digit_pressed (data_723, units[unit], cur_value , lower, upper, step, digit, pp, pos, 0);
		}
		else
		{
			if(get_unit(pp->p_config) == UNIT_MM)
			{
				cur_value = get_area_indexstart (pp->p_config)/1000.0;
				digit = 2;
				pos = 3;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = get_area_indexstart (pp->p_config)/1000*0.03937;
				digit = 3;
				pos = 3;
				unit = UNIT_INCH;
			}
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		}
	}
	else
		/* Inspection -> Type 选择 One-Line Scan 时 */
	{
		if(get_unit(pp->p_config) == UNIT_MM)
		{
			cur_value = get_area_indexstart (pp->p_config);
			digit = 2;
			pos = 3;
			unit = UNIT_MM;
		}
		else
		{
			cur_value = get_area_indexstart (pp->p_config)/1000*0.03937;
			digit = 3;
			pos = 3;
			unit = UNIT_INCH;
		}
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		gtk_widget_set_sensitive (pp->eventbox30[3], FALSE);
		gtk_widget_set_sensitive (pp->eventbox31[3], FALSE);
	}
}

void DrawMenu733()
{
	if ( !con2_p[7][3][3] )
		gtk_widget_hide (pp->eventbox30[3]);
	gtk_widget_hide (pp->eventbox31[3]);
}

void DrawMenu743()
{
	if ( !con2_p[7][4][3] )
		gtk_widget_hide (pp->eventbox30[3]);
	gtk_widget_hide (pp->eventbox31[3]);
}

void DrawMenu803()
{
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
	{
		if(!pp->bSaveDataProcessing)
		     SaveDataProcess("Saving Data!") ;
	}
	draw3_popdown(NULL,3,1);
}

GtkWidget* reportPreviewNew(GtkWidget* fatherWidget);
void DrawMenu813()
{
	draw3_popdown(NULL,3,1);
	//gtk_widget_set_sensitive(pp->eventbox30[3],FALSE);
	//gtk_widget_set_sensitive(pp->eventbox31[3],FALSE);
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
	{
		//draw_dialog_all(DIALOG_REPORT_BUILD);
		const char* filename = get_file_name_info(pp->p_config);
		if((NULL == filename) || (0 == strcmp("" , filename)) || (0 == strcmp(".html" , filename)))
		{
			dialogWarning(pp->window ,getDictString(_STRING_Please_Enter_File_Name));
			return;
		}
		GtkWidget* dialog = reportPreviewNew(pp->window);
		gtk_widget_show(dialog);
	}
}
void DrawMenu823()
{
	draw3_popdown (menu_content[OFF_ON + get_report_format_note (pp->p_config)], 3, 0);
}

void DrawMenu833()
{
	unsigned char userFieldSelect = get_report_userfield_select(pp->p_config);
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
	{
		char* str = NULL;
		str = dialogGetAString( pp->window
							    ,con2_p[8][3][3]
		                        ,get_report_userfield_content(pp->p_config ,userFieldSelect));
		if(str)
		{
			set_report_userfield_content(pp->p_config, str ,userFieldSelect);
			free(str);
		}
		draw3_popdown(get_report_userfield_content(pp->p_config ,userFieldSelect) ,3 ,0);
	}
	else
	{
		draw3_popdown(get_report_userfield_content(pp->p_config ,userFieldSelect) ,3 ,0);
	}
}

void DrawMenu843()
{
	if ( !con2_p[8][4][3] )
		gtk_widget_hide (pp->eventbox30[3]);
	gtk_widget_hide (pp->eventbox31[3]);
}

void DrawMenu903()
{
	if ( !con2_p[9][0][3] )
			gtk_widget_hide (pp->eventbox30[3]);
		gtk_widget_hide (pp->eventbox31[3]);
}

void DrawMenu913()
{
	if ( !con2_p[9][1][3] )
		gtk_widget_hide (pp->eventbox30[3]);
	gtk_widget_hide (pp->eventbox31[3]);
}

void DrawMenu923()
{
	if ( !con2_p[9][2][3] )
			gtk_widget_hide (pp->eventbox30[3]);
		gtk_widget_hide (pp->eventbox31[3]);
}

void DrawMenu933()
{
	if ( !con2_p[9][3][3] )
		gtk_widget_hide (pp->eventbox30[3]);
	gtk_widget_hide (pp->eventbox31[3]);
}

void DrawMenu943()
{
	if ( !con2_p[9][4][3] )
		gtk_widget_hide (pp->eventbox30[3]);
	gtk_widget_hide (pp->eventbox31[3]);
}

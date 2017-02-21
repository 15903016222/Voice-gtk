/*
 * DrawMenu2.c
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

#include "../ui.h"
#include "../../string/_string.h"
#include "menu.h"
#include "../../globalData.h"
#include "../../calculation/limit.h"
#include "../authorization.h"
#include "../../dev/dev.h"

void CalibrationFirstLaw()
{
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos = 2, unit = 0;
	int grp = get_current_group (pp->p_config);
	cur_value = TMP_CBA(clb_start) / 100.0 ;
	if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
	{
		if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 2))
		{
			lower =	LAW_VAL_POS(grp , Angle_min) / 100.0;
			upper =	TMP_CBA(clb_end) / 100.0;
			step  = LAW_VAL_POS(grp , Angle_step) / 100.0;
			digit = 1;
			unit = UNIT_DEG;
			draw3_digit_pressed (data_0222, units[unit], cur_value,
					lower, upper, step, digit, pp, pos, 27);
		}
		else
		{
			unit = UNIT_DEG;
			digit = 1;
			draw3_digit_stop (cur_value , units[unit], digit, pos, 27);
		}
	}
	else if(LAW_VAL_POS(grp , Focal_type) == LINEAR_SCAN)
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
		{
			lower = 1; //TMP_CBA(clb_start) / 100.0;
			upper = TMP_CBA(clb_end) ; //TMP(beam_qty[grp]);
			step =  1;
			digit = 0;
			unit = UNIT_VPA;
			draw3_digit_pressed (data_0222, units[unit], cur_value , lower, upper, step, digit, pp, pos, 45);
		}
		else
		{
			digit = 0;
			unit = UNIT_VPA;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 45);
		}
	}

}

void DrawGateAStart()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower=0, upper=0, step=0;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);
	double velocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;

	switch (TMP(cstart_reg))
	{
		case 0:	tmpf = 1; break;
		case 1:	tmpf = 10; break;
		case 2:	tmpf = 0.1; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
	{
		if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
				cur_value = (GROUP_VAL_POS( grp , gate[0].start) / 1000.0) * (velocity / 2000.0);   /* 当前显示的起位数值mm */
				lower = 0.0;
				upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0) * (velocity/ 2000.0);
				step = tmpf ; //* (velocity / 2000.0);
				digit = 2;
				pos = 2;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = (GROUP_VAL_POS( grp , gate[0].start) / 1000.0) * 0.03937 * (velocity / 2000.0); /* 当前显示的范围inch */
				lower = 0.0;
				upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0 ) * 0.03937 * (velocity / 2000.0) ;
				step = tmpf * 0.03937 ; //* (velocity / 2000.0);
				digit = 3;
				pos = 2;
				unit = UNIT_INCH;
			}
		}
		else
		{
			cur_value = GROUP_VAL_POS( grp , gate[0].start) / 1000.0 ;   /* us */
			lower = 0.0;
			upper =	MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0;
			step = tmpf;
			digit = 2;
			pos = 2;
			unit = UNIT_US;
		}
		draw3_digit_pressed (data_202, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 65);
	}
	else
	{
		if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
				cur_value = (GROUP_VAL_POS( grp , gate[0].start) / 1000.0) * (velocity / 2000.0);   /* 当前显示的范围数值mm */
				unit = UNIT_MM;
				digit = 2;
				pos = 2;
			}
			else
			{
				cur_value = (GROUP_VAL_POS( grp , gate[0].start) / 1000.0) * 0.03937 * (velocity / 2000.0); /* 当前显示的范围inch */
				unit = UNIT_INCH;
				digit = 3;
				pos = 2;
			}
		}
		else
		{
			cur_value = GROUP_VAL_POS( grp , gate[0].start) / 1000.0 ;
			digit = 2;
			pos = 2;
			unit = UNIT_US;
		}
		draw3_digit_stop (cur_value , units[unit], digit, pos, 65);
	}
}

void CalibrationReferenceAmp()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower=0, upper=0, step=0;
	int digit = 0, pos, unit = 0;
	switch (TMP(ref_amplitude_reg))
	{
		case 0:	tmpf = 1; break;
		case 1:	tmpf = 10; break;
		case 2:	tmpf = 0.1; break;
		default:break;
	}
	if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 2))
	{
		cur_value = pp->ref_amplitude/100.0 ;
		lower =	0.0;
		upper =	100.0;
		step = tmpf;
		digit = 1;
		pos = 2;
		unit = UNIT_BFH;
		draw3_digit_pressed (data_0223, units[unit], cur_value,
				lower, upper, step, digit, NULL , pos, 33);
	}
	else
	{
		cur_value = pp->ref_amplitude/100.0 ;
		unit = UNIT_BFH;
		pos = 2;
		digit = 1;
		draw3_digit_stop (cur_value , units[unit], digit, pos, 33);
	}
}

void CalibrationGain()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower=0, upper=0, step=0;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);

	if(GROUP_VAL_POS(grp , group_mode) == PA_SCAN)
	{
		switch (TMP(db_reg))
		{
			case 0:	tmpf = 0.1; break;
			case 1:	tmpf = 0.5; break;
			case 2:	tmpf = 1.0; break;
			case 3:	tmpf = 2.0; break;
			case 4:	tmpf = 6.0; break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
		{
            cur_value = (group_get_gain(grp) - group_get_refgain(grp) * get_group_db_ref (pp->p_config, grp)) / 100.0;
            lower = 0.0 - group_get_refgain(grp) * get_group_db_ref (pp->p_config, grp) / 100.0 ;

            if (PA_SCAN == GROUP_VAL_POS(grp, group_mode)
                         || UT_SCAN == GROUP_VAL_POS(grp, group_mode)) {
                upper = PA_MAX_GAIN - group_get_refgain(grp) * get_group_db_ref (pp->p_config, grp) / 100.0 ;
            } else if (dev_fpga_version() == 2){
                upper = UT_MAX_GAIN_2 - group_get_refgain(grp) * get_group_db_ref (pp->p_config, grp) / 100.0 ;
            } else {
                upper = UT_MAX_GAIN_1 - group_get_refgain(grp) * get_group_db_ref (pp->p_config, grp) / 100.0 ;
            }
			step = tmpf;
			digit = 1;
			pos = 2;
			unit = UNIT_DB;
			draw3_digit_pressed (data_100, units[unit], cur_value ,
					lower, upper, step, digit, NULL, pos, 9);
		}
		else
		{
            cur_value = (group_get_gain(grp) - group_get_refgain(grp) * get_group_db_ref (pp->p_config, grp)) / 100.0;
			digit = 1;
			pos = 2;
			unit = UNIT_DB;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 9);
		}
	}
	else
	{
		cur_value = TMP_CBA(CalibrationWedgeDelay) / 1000.0;
		digit = 2;
		pos = 2;
		unit = UNIT_US;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 51);
		gtk_widget_set_sensitive(pp->eventbox30[2] ,FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[2] ,FALSE);
	}
}

void DrawMenu002()
{
}
void DrawMenu012()
{
}
void DrawMenu022Step1()
{
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
		draw3_pop_tt (data_022, NULL,
				menu_content[CTYPE + pp->ctype_pos],
				menu_content+CTYPE, 3, 2, pp->ctype_pos, 0x0);
	else
	{
		draw3_popdown (menu_content[CTYPE + pp->ctype_pos], 2, 0);
	}
}

void DrawMenu022EncoderStep2()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower=0, upper=0, step=0;
	int digit = 0, pos, unit = 0;

	switch (TMP(origin_reg))
	{
		case 0:	tmpf = 1; break;
		case 1:	tmpf = 10; break;
		case 2:	tmpf = 0.1; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
	{
		cur_value = 0  ; //get_enc_origin (pp->p_config, get_cur_encoder (pp->p_config))/1000.0;
		lower = -1000;
		upper = 1000.00;
		step = tmpf;
		digit = 2;
		pos = 2;
		unit = UNIT_MM;
		draw3_digit_pressed (data_0228, units[unit], cur_value , lower, upper, step, digit, NULL , pos, 41);
	}
	else
	{
		cur_value = 0 ; //get_enc_origin (pp->p_config, get_cur_encoder (pp->p_config))/1000.0;
		digit = 2;
		pos = 2;
		unit = UNIT_MM;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 41);
	}
	gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
	gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);

}

void DrawMenu022EncoderStep3()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower=0, upper=0, step=0;
	int digit = 0, pos, unit = 0;

	switch (TMP(distance_reg))
	{
		case 0:	tmpf = 1; break;
		case 1:	tmpf = 10; break;
		case 2:	tmpf = 0.1; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
	{
		cur_value = pp->distance /1000.0;
		lower = -9999.00;
		upper = 9999.00;
		step = tmpf;
		digit = 2;
		pos = 2;
		unit = UNIT_MM;
		if(get_unit(pp->p_config) == UNIT_INCH)
		{
			cur_value *= 0.03937 ;
			lower *= 0.03937  ;
			upper *= 0.03937  ;
			step  *= 0.03937  ;
			digit = 3;
			unit = UNIT_INCH;
		}
		draw3_digit_pressed (data_0227, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 42);
	}
	else
	{
		cur_value = pp->distance /1000.0;
		digit = 2;
		pos = 2;
		unit = UNIT_MM;
		if(get_unit(pp->p_config) == UNIT_INCH)
		{
			cur_value *= 0.03937 ;
			digit = 3;
			unit = UNIT_INCH;
		}
		draw3_digit_stop (cur_value, units[unit], digit, pos, 42);
	}
}

void DrawMenu022EncoderStep4()
{
	double cur_value=0.0;
	int digit = 0, pos, unit = 0;
	cur_value = get_enc_resolution (pp->p_config, get_cur_encoder (pp->p_config))/1000.0;
	digit = 3;
	pos = 2;
	unit = UNIT_STEP_MM;
	draw3_digit_stop (cur_value, units[unit], digit, pos, 43);
	gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
	gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
}



void DrawMenu022UltrasoundVelocityStep2()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower=0, upper=0, step=0;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);

	switch (TMP(cangle_reg))
	{
		case 0:
		case 1:
		case 2:
		default:
			tmpf = LAW_VAL_POS(grp, Angle_step) / 100.0 ;
			break ;
	}
	if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 2))
	{
		if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
		{
			cur_value = LAW_VAL_POS(grp, Angle_min)/100.0 + TMP(beam_num[grp]) * LAW_VAL_POS(grp, Angle_step)/100.0;
			lower =	LAW_VAL_POS(grp , Angle_min) / 100.0;
			upper =	LAW_VAL_POS(grp , Angle_max) / 100.0;
			step = tmpf;
			digit = 1;
			pos = 2;
			unit = UNIT_DEG;
			draw3_digit_pressed (data_0226, units[unit], cur_value,
					lower, upper, step, digit, NULL , pos, 8);
		}
		else
		{
			cur_value = TMP(beam_num[grp]) + 1.0;
			lower =	1;
			upper =	TMP(beam_qty[grp]) ;
			step = 1 ;
			digit = 0;
			pos = 2;
			draw3_digit_pressed (data_0226, units[UNIT_NONE], cur_value,
					lower, upper, step, digit, NULL , pos, 49);
		}
	}
	else
	{
		if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
		{
			cur_value = LAW_VAL_POS(grp, Angle_min)/100.0 + TMP(beam_num[grp]) * LAW_VAL_POS(grp, Angle_step)/100.0;
			unit = UNIT_DEG;
			pos = 2;
			digit = 1;
			draw3_digit_stop (cur_value , units[unit], digit, pos, 8);
		}
		else
		{
			cur_value = TMP(beam_num[grp]) + 1.0;
			pos = 2;
			digit = 0;
			draw3_digit_stop (cur_value , units[UNIT_NONE], digit, pos, 49);
		}

	}
	if(PA_SCAN != GROUP_VAL_POS(grp , group_mode))//UT
	{
		gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
	}
}

void DrawMenu022UltrasoundVelocityStep3()
{
	char *str = NULL;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
		draw3_pop_tt (data_0221, NULL,
				menu_content[ECHOTYPE + pp->echotype_pos],
				menu_content+ECHOTYPE, 3, 2, pp->echotype_pos, 0x0);
	else
		draw3_popdown (menu_content[ECHOTYPE + pp->echotype_pos], 2, 0);
	str = g_strdup_printf ("%s", con2_p[0][2][12]);
	gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
}

void DrawMenu022UltrasoundVelocityStep4()
{
	DrawGateAStart();
}

void DrawMenu022UltrasoundVelocityStep5()
{
	DrawGateAStart();
}

void DrawMenu022UltrasoundVelocityStep6()
{
	double cur_value=0.0;
	int digit = 0, pos, unit = 0;

	if (get_unit(pp->p_config) == 0)
	{
		cur_value = pp->vel;
		digit = 1;
		unit = UNIT_M_S;
	}
	else
	{
		cur_value = pp->vel * 0.0000394;
		digit = 4;
		unit = UNIT_IN_US;
	}
	pos = 2;
	draw3_digit_stop (pp->vel, units[unit], digit, pos, 44);
	gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
	gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
}

void DrawMenu022UltrasoundWedgeDelayStep2()
{
	char *str = NULL;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
		draw3_pop_tt (data_0221, NULL,
				menu_content[ECHOTYPE + pp->echotype_pos],
				menu_content+ECHOTYPE, 3, 2, pp->echotype_pos, 0x0);
	else
		draw3_popdown (menu_content[ECHOTYPE + pp->echotype_pos], 2, 0);
	str = g_strdup_printf ("%s", con2_p[0][2][12]);
	gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
	g_free (str);
}

void DrawMenu022UltrasoundWedgeDelayStep3()
{
	CalibrationFirstLaw();
	gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
	gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
}

void DrawMenu022UltrasoundWedgeDelayStep4()
{
	DrawGateAStart();
}

void DrawMenu022UltrasoundWedgeDelayStep5()
{
	CalibrationGain();
}

void DrawMenu022UltrasoundSensationStep2()
{
	CalibrationReferenceAmp() ;
}

void DrawMenu022UltrasoundSensationStep3()
{
	CalibrationFirstLaw();
	gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
	gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
}

void DrawMenu022UltrasoundSensationStep4()
{
     DrawGateAStart();
}

void DrawMenu022UltrasoundSensationStep5()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower=0, upper=0, step=0;
	int digit = 0, pos, unit = 0;

	switch (TMP(compdb_reg))
	{
		case 0:	tmpf = 1; break;
		case 1:	tmpf = 0.1; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
	{
		cur_value = pp->comp_gain / 100.0;
		lower = 0.0;
		upper = 10.0;
		step = tmpf;
		digit = 1;
		pos = 2;
		unit = UNIT_DB_US;
		draw3_digit_pressed (data_0224, units[unit], cur_value ,
				lower, upper, step, digit, NULL, pos, 34);
	}
	else
	{
		cur_value = pp->comp_gain / 100.0;
		digit = 1;
		pos = 2;
		unit = UNIT_DB_US;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 34);
	}
}

void DrawMenu022UltrasoundSensationStep6()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower=0, upper=0, step=0;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);

	switch (TMP(db_reg))
	{
		case 0:	tmpf = 1; break;
		case 1:	tmpf = 2; break;
		case 2:	tmpf = 6; break;
		case 3:	tmpf = 0.1; break;
		case 4:	tmpf = 0.5; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
	{
        cur_value = (group_get_gain(grp)  - group_get_refgain(grp) * get_group_db_ref (pp->p_config, grp)) / 100.0;
        lower = 0.0 - group_get_refgain(grp) * get_group_db_ref (pp->p_config, grp) / 100.0 ;

        if (PA_SCAN == GROUP_VAL_POS(grp, group_mode)
                     || UT_SCAN == GROUP_VAL_POS(grp, group_mode)) {
            upper = PA_MAX_GAIN - group_get_refgain(grp) * get_group_db_ref (pp->p_config, grp) / 100.0 ;
        } else if (dev_fpga_version() == 2) {
            upper = UT_MAX_GAIN_2 - group_get_refgain(grp) * get_group_db_ref (pp->p_config, grp) / 100.0 ;
		} else {
			upper = UT_MAX_GAIN_1 - group_get_refgain(grp) * get_group_db_ref (pp->p_config, grp) / 100.0 ;
        }

		step = tmpf;
		digit = 1;
		pos = 2;
		unit = UNIT_DB;
		draw3_digit_pressed (data_100, units[unit], cur_value ,
				lower, upper, step, digit, NULL, pos, 9);
	}
	else
	{
        cur_value = (group_get_gain(grp) - group_get_refgain(grp) * get_group_db_ref (pp->p_config, grp)) / 100.0;
		digit = 1;
		pos = 2;
		unit = UNIT_DB;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 9);
	}
}

void DrawMenu022UltrasoundUtWedgedelayAndVelocityStep2()
{
	double cur_value=0.0;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);

	if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
	{
		cur_value = LAW_VAL_POS(grp, Angle_min)/100.0 + TMP(beam_num[grp]) * LAW_VAL_POS(grp, Angle_step)/100.0;
		unit = UNIT_DEG;
		pos = 2;
		digit = 1;
		draw3_digit_stop (cur_value , units[unit], digit, pos, 8);
	}
	else
	{
		cur_value = TMP(beam_num[grp]) + 1.0;
		pos = 2;
		digit = 0;
		draw3_digit_stop (cur_value , units[UNIT_NONE], digit, pos, 49);
	}

	gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
	gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);

}

void DrawMenu022UltrasoundUtWedgedelayAndVelocityStep3()
{
	char* str = NULL ;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
		draw3_pop_tt (data_0221, NULL,
				menu_content[ECHOTYPE + pp->echotype_pos],
				menu_content+ECHOTYPE, 3, 2, pp->echotype_pos, 0x0);
	else
		draw3_popdown (menu_content[ECHOTYPE + pp->echotype_pos], 2, 0);
	str = g_strdup_printf ("%s", con2_p[0][2][12]);
	gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
}

void DrawMenu022UltrasoundUtWedgedelayAndVelocityStep4()
{
	DrawGateAStart();
}

void DrawMenu022UltrasoundUtWedgedelayAndVelocityStep5()
{
	DrawGateAStart();
}

void DrawMenu022UltrasoundUtWedgedelayAndVelocityStep6()
{
	double cur_value=0.0;
	int digit = 0, pos, unit = 0;

	if (get_unit(pp->p_config) == 0)
	{
		cur_value = pp->vel;
		digit = 1;
		unit = UNIT_M_S;
	}
	else
	{
		cur_value = pp->vel * 0.0000394;
		digit = 4;
		unit = UNIT_IN_US;
	}
	pos = 2;
	draw3_digit_stop (pp->vel, units[unit], digit, pos, 44);
	gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
	gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
}

void DrawMenu022CodeAPIStep2()
{

}
void DrawMenu022CodeAPIStep3()
{

}
void DrawMenu022CodeAPIStep4()
{

}
void DrawMenu022CodeAPIStep5()
{

}
void DrawMenu022CodeAPIStep6()
{

}

void DrawMenu022CodeAWSStep2()
{
	char* str = NULL ;
	int grp = get_current_group(pp->p_config);
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
		draw3_pop_tt (data_0225, NULL,
				menu_content[WELDTYPE + GROUP_VAL_POS(grp ,part.weldtype_pos)],
				menu_content+WELDTYPE, 2, 2, GROUP_VAL_POS(grp ,part.weldtype_pos), 0x0);
	else
		draw3_popdown (menu_content[WELDTYPE + GROUP_VAL_POS(grp ,part.weldtype_pos)], 2, 0);

	str = g_strdup_printf ("%s", con2_p[0][2][38]);
	gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
}
void DrawMenu022CodeAWSStep3()
{
    DrawGateAStart();
}
void DrawMenu022CodeAWSStep4()
{
	double tmpf = 0.0;
	double cur_value=0.0 , lower=0, upper=0, step=0;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);

	switch (TMP(db_reg))
	{
		case 0:	tmpf = 0.1; break;
		case 1:	tmpf = 0.5; break;
		case 2:	tmpf = 1.0; break;
		case 3:	tmpf = 2.0; break;
		case 4:	tmpf = 6.0; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
	{
        cur_value = (group_get_gain(grp) - group_get_refgain(grp) * get_group_db_ref (pp->p_config, grp)) / 100.0;
        lower = 0.0 - group_get_refgain(grp) * get_group_db_ref (pp->p_config, grp) / 100.0 ;

        if (PA_SCAN == GROUP_VAL_POS(grp, group_mode)
                     || UT_SCAN == GROUP_VAL_POS(grp, group_mode)) {
            upper = PA_MAX_GAIN - group_get_refgain(grp) * get_group_db_ref (pp->p_config, grp) / 100.0 ;
        } else if (dev_fpga_version() == 2) {
            upper = UT_MAX_GAIN_2 - group_get_refgain(grp) * get_group_db_ref (pp->p_config, grp) / 100.0 ;
        } else {
            upper = UT_MAX_GAIN_1 - group_get_refgain(grp) * get_group_db_ref (pp->p_config, grp) / 100.0 ;
        }
		step = tmpf;
		digit = 1;
		pos = 2;
		unit = UNIT_DB;
		draw3_digit_pressed (data_100, units[unit], cur_value ,
				lower, upper, step, digit, NULL, pos, 9);
	}
	else
	{
        cur_value = (group_get_gain(grp) - group_get_refgain(grp) * get_group_db_ref (pp->p_config, grp)) / 100.0;
		digit = 1;
		pos = 2;
		unit = UNIT_DB;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 9);
	}
}
void DrawMenu022CodeAWSStep5()
{
	draw3_popdown_offset(NULL, 2, 1, 29);
	//gtk_widget_hide (pp->eventbox30[2]);
	//gtk_widget_hide (pp->eventbox31[2]);
}
void DrawMenu022CodeAWSStep6()
{

}

void DrawMenu022UltrasoundVelocity()
{
	switch(pp->cstart_qty)
	{
	case 0:
	case 1:
		DrawMenu022Step1();
		break;
	case 2:
		DrawMenu022UltrasoundVelocityStep2();
		break;
	case 3:
		DrawMenu022UltrasoundVelocityStep3();
		break;
	case 4:
		DrawMenu022UltrasoundVelocityStep4();
		break;
	case 5:
		DrawMenu022UltrasoundVelocityStep5();
		break;
	case 6:
		DrawMenu022UltrasoundVelocityStep6();
		break;
	default:
		break;

	}
}

void DrawMenu022UltrasoundWedgeDelay()
{
	switch(pp->cstart_qty)
	{
	case 0:
	case 1:
		DrawMenu022Step1();
		break;
	case 2:
		DrawMenu022UltrasoundWedgeDelayStep2();
		break;
	case 3:
		DrawMenu022UltrasoundWedgeDelayStep3();
		break;
	case 4:
		DrawMenu022UltrasoundWedgeDelayStep4();
		break;
	case 5:
		DrawMenu022UltrasoundWedgeDelayStep5();
		break;
	default:
		break;
	}
}


void DrawMenu022UltrasoundSensation()
{
	int _nGroupId = get_current_group(pp->p_config);
	if(PA_SCAN == GROUP_VAL_POS(_nGroupId , group_mode))
	{
		switch(pp->cstart_qty)
		{
		case 0:
		case 1:
			DrawMenu022Step1();
			break;
		case 2:
			DrawMenu022UltrasoundSensationStep2();
			break;
		case 3:
			DrawMenu022UltrasoundSensationStep3();
			break;
		case 4:
			DrawMenu022UltrasoundSensationStep4();
			break;
		case 5:
			DrawMenu022UltrasoundSensationStep5();
			break;
		case 6:
			DrawMenu022UltrasoundSensationStep6();
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
			DrawMenu022Step1();
			break;
		case 2:
			DrawMenu022UltrasoundUtWedgedelayAndVelocityStep2();
			break;
		case 3:
			DrawMenu022UltrasoundUtWedgedelayAndVelocityStep3();
			break;
		case 4:
			DrawMenu022UltrasoundUtWedgedelayAndVelocityStep4();
			break;
		case 5:
			DrawMenu022UltrasoundUtWedgedelayAndVelocityStep5();
			break;
		case 6:
			DrawMenu022UltrasoundUtWedgedelayAndVelocityStep6();
			break;
		default:
			break;
		}

	}

}
void DrawMenu022Encoder()
{
	switch(pp->cstart_qty)
	{
	case 0:
	case 1:
		DrawMenu022Step1();
		break;
	case 2:
		DrawMenu022EncoderStep2();
		break;
	case 3:
		DrawMenu022EncoderStep3();
		break;
	case 4:
		DrawMenu022EncoderStep4();
		break;
	default:
		break;
	}
}

void DrawMenu022UltrasoundTCGStep2()
{
	CalibrationReferenceAmp() ;
}

void DrawMenu022UltrasoundTCGStep3()
{
	CalibrationFirstLaw();
}

void DrawMenu022UltrasoundTCGStep4()
{
	DrawGateAStart();
}

void DrawMenu022UltrasoundTCGStep5()
{
	CalibrationGain();
}

void DrawMenu022UltrasoundTCGStep6()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower=0, upper=0, step=0 ;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);
	int _nBeamNo = TMP(beam_num[grp]) ;
	int _nPointPos = GROUP_VAL_POS( grp , SizingCurves.point_pos) ;
	double _nVelocity = GROUP_VAL_POS(grp , velocity) / 200000.0  ;
	switch (pp ->p_tmp_config->position_reg)
	{
		case 0:	tmpf = 0.01; break;
		case 1:	tmpf = 0.1; break;
		case 2:	tmpf = 1.0; break;
		case 3:	tmpf = 10.0; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
	{
		if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
				cur_value = (GROUP_VAL_POS( grp , SizingCurves.position[_nBeamNo][_nPointPos]) / 1000.0) * _nVelocity ;
				cur_value *= cos(TMP(current_angle[grp])) ;
				if(_nPointPos)
				{
					lower = (GROUP_VAL_POS( grp , SizingCurves.position[_nBeamNo][_nPointPos - 1]) / 1000.0) * _nVelocity ;
					lower *= cos(TMP(current_angle[grp])) ;
				}
				else
					lower = 0.0;
				upper = 20000 ; //get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/100.0 * 4.95;
				step = tmpf;
				digit = 2;
				pos = 2;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = (GROUP_VAL_POS( grp , SizingCurves.position[_nBeamNo][_nPointPos]) / 1000.0) * _nVelocity ;
				cur_value *= cos(TMP(current_angle[grp])) * 0.03937 ;
				if(_nPointPos){
					lower = (GROUP_VAL_POS( grp , SizingCurves.position[_nBeamNo][_nPointPos - 1]) / 1000.0) * _nPointPos ;
					lower *= cos(TMP(current_angle[grp])) * 0.03937 ;
				}
				else
					lower = 0.0;
				upper = 20000 * 0.03937 ; //get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/100.0 * 4.95 * 0.03937;
				step = tmpf/2.0;
				digit = 3;
				pos = 2;
				unit = UNIT_INCH;
			}
		}
		else
		{
			cur_value = GROUP_VAL_POS( grp , SizingCurves.position[_nBeamNo][_nPointPos]) / 1000.0 ;
			if(GROUP_VAL_POS( grp , SizingCurves.point_pos))
				lower = GROUP_VAL_POS( grp , SizingCurves.position[_nBeamNo][_nPointPos - 1]) / 1000.0 ;
			else
				lower = 0.0;
			upper = 9900.80;
			step = tmpf*5.0;
			digit = 2;
			pos = 2;
			unit = UNIT_US;
		}
		draw3_digit_pressed (data_2321, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 57);
	}
	else
	{
		if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
				cur_value = (GROUP_VAL_POS( grp , SizingCurves.position[_nBeamNo][_nPointPos]) / 1000.0) *_nVelocity ;
				cur_value *= cos(TMP(current_angle[grp])) ;
				digit = 2;
				pos = 2;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = (GROUP_VAL_POS( grp , SizingCurves.position[_nBeamNo][_nPointPos]) / 1000.0) *_nVelocity ;
				cur_value *= cos(TMP(current_angle[grp])) * 0.03937 ;
				digit = 3;
				pos = 2;
				unit = UNIT_INCH;
			}
		}
		else
		{
			cur_value = GROUP_VAL_POS( grp , SizingCurves.position[_nBeamNo][_nPointPos]) / 1000.0 ;
			digit = 2;
			pos = 2;
			unit = UNIT_US;
		}
		draw3_digit_stop (cur_value, units[unit], digit, pos, 57);
	}
	if(TMP_CBA(bNewPoint))
	{
		gtk_widget_set_sensitive(pp->eventbox30[2],TRUE);
		gtk_widget_set_sensitive(pp->eventbox31[2],TRUE);
	}
	else
	{
		gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
	}
}

void DrawMenu022UltrasoundTCG()
{
	switch(pp->cstart_qty)
	{
	case 0:
	case 1:
		DrawMenu022Step1();
		break;
	case 2:
		DrawMenu022UltrasoundTCGStep2();
		break;
	case 3:
		DrawMenu022UltrasoundTCGStep4();
		break;
	case 4:
		DrawMenu022UltrasoundTCGStep5();
		break;
	case 5:
		DrawMenu022UltrasoundTCGStep6() ;
		break;
	case 6:
		break;
	default:
		break;
	}
}


void DrawMenu022TOFDStep3()
{
	int grp = get_current_group (pp->p_config);
	double cur_value = LAW_VAL_POS(grp, Angle_min)/100.0 ;
	int unit = UNIT_DEG;
	int pos = 2;
	int digit = 1;
	draw3_digit_stop (cur_value , units[unit], digit, pos, 8);

	gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
	gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
}

void DrawMenu022TOFDStep2()
{
	double cur_value;
	int digit = 0, pos, unit = 0;

	int grp = get_current_group(pp->p_config);
	double step ;
	switch (TMP(WedgeSeparation_reg))
	{
		case 0:	step = 0.1; break;
		case 1:	step = 1; break;
		case 2:	step = 10; break;
		default:break;
	}

	double lower = 1 ;
	double upper = 1000 ;
	if (UNIT_MM == get_unit(pp->p_config))
	{
		cur_value = GROUP_VAL_POS(grp , field_distance[255]);
		unit  = UNIT_MM;
		digit = 2;
		pos   = 2;
	}
	else
	{
		cur_value = GROUP_VAL_POS(grp , field_distance[255]) * 0.03937;
		unit   = UNIT_INCH ;
		lower *= 0.03937  ;
		upper *= 0.03937  ;
		step  *= 0.03937  ;
		digit = 3;
		pos   = 2;
	}

	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
		draw3_digit_pressed (data_2323, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 52);
	else
	    draw3_digit_stop (cur_value , units[unit], digit, pos, 52);
}

void DrawMenu022TOFDStep4()
{
	//DrawGateAStart();
	double tmpf = 0.0;
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos = 2 , unit = 0;
	int grp = get_current_group (pp->p_config);

	switch (TMP(u_reference_reg))
	{
		case 0:	tmpf = 0.01; break;
		case 1:	tmpf = 0.1;  break;
		case 2:	tmpf = 1.0;  break;
		case 3:	tmpf = 10.0;  break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
	{
		double _nVelocity = GROUP_VAL_POS(grp , velocity) / 200000;
		if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
		{
			if(get_unit(pp->p_config) == UNIT_MM)
			{
				cur_value = GROUP_VAL_POS( grp , u_reference)/1000.0 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
				lower = 0 ;
				upper = 1000 * _nVelocity;
				step = tmpf;
				digit = 2;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = GROUP_VAL_POS( grp , u_reference) / 1000.0 * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
				lower = 0 ;
				upper = 1000 * _nVelocity * 0.03937;
				step = tmpf/2.0;
				digit = 3;
				unit = UNIT_INCH;
			}
		}
		else	/* us */
		{
			cur_value = GROUP_VAL_POS( grp , u_reference)/1000.0;
			lower = 0 ;
			upper = 1000;
			step = tmpf;
			digit = 2;
			unit = UNIT_US;
		}
		draw3_digit_pressed (data_313, units[unit], cur_value , lower, upper, step, digit, pp, pos, 63);
	}
	else
	{
		if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
		{
			if(get_unit(pp->p_config) == UNIT_MM)
			{
				cur_value = GROUP_VAL_POS( grp , u_reference)/1000.0 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
				digit = 2;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = GROUP_VAL_POS( grp , u_reference)/1000.0 * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
				digit = 3;
				unit = UNIT_INCH;
			}
		}
		else
		{
			cur_value = GROUP_VAL_POS( grp , u_reference)/1000.0;
			digit = 2;
			unit = UNIT_US;
		}
		draw3_digit_stop (cur_value, units[unit], digit, pos, 63);
	}

}

void DrawMenu022TOFDStep5()
{
	DrawGateAStart();
}

void DrawMenu022TOFDStep6()
{
	double cur_value=0.0;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group(pp->p_config) ;
	if (get_unit(pp->p_config) == 0)
	{
		cur_value = GROUP_VAL_POS(grp , field_distance[254]);
		digit = 1;
		unit = UNIT_MM;
	}
	else
	{
		cur_value = GROUP_VAL_POS(grp , field_distance[254]) *  0.03937;
		digit = 4;
		unit = UNIT_INCH;
	}
	pos = 2;
	draw3_digit_stop (cur_value , units[unit], digit, pos, 53);
	gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
	gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
}

void DrawMenu022TOFD()
{
	switch(pp->cstart_qty)
	{
	case 0:
	case 1:
		DrawMenu022Step1();
		break;
	case 2:
		DrawMenu022TOFDStep2();
		break;
	case 3:
		DrawMenu022TOFDStep3();
		break;
	case 4:
		DrawMenu022TOFDStep4();
		break;
	case 5:
		DrawMenu022TOFDStep6() ;
		//DrawMenu022TOFDStep5();
		break;
	case 6:
		DrawMenu022TOFDStep6();
		break;
	default:
		break;
	}
}

void DrawMenu022Ultrasound()
{
	int grp = get_current_group(pp->p_config);
    if(group_get_rx_tx_mode(grp) == TOFD)
	{
		DrawMenu022TOFD();
	}
	else
	{
		switch(pp->cmode_pos)
		{
		case 0:
			DrawMenu022UltrasoundVelocity();
			break;
		case 1:
			DrawMenu022UltrasoundWedgeDelay();
			break;
		case 2:
			DrawMenu022UltrasoundSensation();
			break;
		case 3:
			DrawMenu022UltrasoundTCG();
			break;
		default:
			break;
		}
	}
}


void DrawMenu022CodeAPI()
{
	switch(pp->cstart_qty)
	{
	case 0:
	case 1:
		DrawMenu022Step1();
		break;
	case 2:
		DrawMenu022CodeAPIStep2();
		break;
	case 3:
		DrawMenu022CodeAPIStep3();
		break;
	case 4:
		DrawMenu022CodeAPIStep4();
		break;
	case 5:
		DrawMenu022CodeAPIStep5();
		break;
	case 6:
		DrawMenu022CodeAPIStep6();
		break;
	default:
		break;
	}
}

void DrawMenu022CodeAWS()
{
	switch(pp->cstart_qty)
	{
	case 0:
	case 1:
		DrawMenu022Step1();
		break;
	case 2:
		DrawMenu022CodeAWSStep2();
		break;
	case 3:
		DrawMenu022CodeAWSStep3();
		break;
	case 4:
		DrawMenu022CodeAWSStep4();
		break;
	case 5:
		DrawMenu022CodeAWSStep5();
		break;
	case 6:
		DrawMenu022CodeAWSStep6();
		break;
	default:
		break;
	}
}

void DrawMenu022Code()
{
	switch(pp->scode_pos)
	{
	case 0:
		DrawMenu022CodeAPI();
		break;
	case 1:
		DrawMenu022CodeAWS();
		break;
	default:
		break;
	}
}

void DrawMenu022()
{
	pp->x_pos = 570, pp->y_pos = 295;
	switch (pp->ctype_pos)
	{
	case 0:// encoder calibration
		DrawMenu022Encoder();
		break;
	case 1:
		DrawMenu022Ultrasound();
		break;
	case 2:
		DrawMenu022Code();
		break;
	default:
		break;
	}
}
void DrawMenu032()
{
	double tmpf ;
	double cur_value=0.0, lower=0, upper=0, step=0;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group(pp->p_config);
	float multiplier = 1.0;
	if(UNIT_MM == get_unit(pp->p_config))
	{
		multiplier = 1.0;
	}else
	{
		multiplier = 0.03937;
	}

	if ( !con2_p[0][3][2] )
	gtk_widget_hide (pp->eventbox30[2]);
	gtk_widget_hide (pp->eventbox31[2]);
	pp->x_pos = 570, pp->y_pos = 295;
	switch(pp->wstart_qty)
	{
		case 0:
		case 1:
			draw3_popdown_offset (menu_content[OFF_ON + get_weld_display()], 2, 0, 13) ;
			break;
		case 2:
			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
				draw3_pop_tt (data_534, NULL,
						menu_content[WELD + get_part_weld(grp)],
						menu_content+WELD, 6, 2, get_part_weld(grp), 0x0);
			else
				draw3_popdown (menu_content[WELD + get_part_weld(grp)], 2, 0);
			break;
		case 3:
			/* 当前步进 */
			switch (pp->p_tmp_config->weland_height_reg)
			{
				case 0:	tmpf = 0.1; break;
				case 1:	tmpf = 1.0; break;
				case 2:	tmpf = 10.0; break;
				default:break;
			}
			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
			{
				if(UNIT_MM == get_unit(pp->p_config))
				{
					cur_value = get_weland_height(grp) / 1000.0;
					lower = 0;
					upper = 1000.00;
					step = tmpf;
					digit = 2;
					pos = 2;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = get_weland_height(grp) / 1000.0 * 0.03937;
					lower = 0;
					upper = 1000.00 * 0.03937;
					step = tmpf;
					digit = 3;
					pos = 2;
					unit = UNIT_INCH;
				}
				if(get_part_weld(grp) == 5)
				{
					draw3_digit_pressed (data_032, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 14);
				}
				else
					draw3_digit_pressed (data_032, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 7);
			}
			else
			{
				if(UNIT_MM == get_unit(pp->p_config))
				{
					cur_value = get_weland_height(grp) / 1000.0;
					digit = 2;
					pos = 2;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = get_weland_height(grp) / 1000.0 * 0.03937;
					digit = 3;
					pos = 2;
					unit = UNIT_INCH;
				}
				if(get_part_weld(grp) == 5)
				{
					cur_value = get_weland_height(grp) / 1000.0;
					draw3_digit_stop (cur_value, units[unit], digit, pos, 14);
				}
				else
					draw3_digit_stop (cur_value, units[unit], digit, pos, 7);
			}
			break;
		case 4:
			if(get_part_weld(grp)==0)
			{
				gtk_widget_hide (pp->eventbox30[2]);
				gtk_widget_hide (pp->eventbox31[2]);
			}
			else
			{
				/* 当前步进 */
				switch (TMP(fizone_height_reg))
				{
					case 0:	tmpf = 0.1; break;
					case 1:	tmpf = 1.0; break;
					case 2:	tmpf = 10.0; break;
					default:break;
				}
				if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
				{
					if(UNIT_MM == get_unit(pp->p_config))
					{
						cur_value = get_fizone_height(grp) / 1000.0;
						lower = 0;
						upper = 1000.00;
						step = tmpf;
						digit = 2;
						pos = 2;
						unit = UNIT_MM;
					}
					else
					{
						cur_value = get_fizone_height(grp) / 1000.0 * 0.03937;
						lower = 0;
						upper = 1000.00 * 0.03937;
						step = tmpf;
						digit = 3;
						pos = 2;
						unit = UNIT_INCH;
					}
					draw3_digit_pressed (data_0321, units[unit], cur_value , lower, upper, step, digit, NULL , pos, 10);
				}
				else
				{
					if(UNIT_MM == get_unit(pp->p_config))
					{
						cur_value = get_fizone_height(grp) / 1000.0;
						digit = 2;
						pos = 2;
						unit = UNIT_MM;
					}
					else
					{
						cur_value = get_fizone_height(grp) / 1000.0 * 0.03937;
						digit = 3;
						pos = 2;
						unit = UNIT_INCH;
					}
					draw3_digit_stop (cur_value, units[unit], digit, pos, 10);
				}
			}
			break;
		default:break;
	}
}
void DrawMenu042()
{
	if ( !con2_p[0][4][2] )
		gtk_widget_hide (pp->eventbox30[2]);
	gtk_widget_hide (pp->eventbox31[2]);
}

void DrawMenu102()
{
	double tmpf = 0.0 , tmpfm;
	double cur_value=0.0, lower=0, upper=0, step=0 , max_tmp=0, max_tmp1=0;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);

	switch (TMP(range_reg))
	{
		case 0:	tmpf = GROUP_VAL_POS( grp , point_qty) / 100.0; break;
		case 1:	tmpf = GROUP_VAL_POS( grp , point_qty) / 20.0; break;
		case 2:	tmpf = GROUP_VAL_POS( grp , point_qty) / 10.0; break;
		default:break;
	}
	tmpfm = GROUP_VAL_POS( grp , point_qty) / 100.0;

    max_tmp = (PrfLimitToGroupSampleRange(grp)/ 100  - group_get_start (grp) / 1000.0);
	max_tmp1 = GROUP_VAL_POS( grp , point_qty) * 20.0;

	if(UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit))
	{
		if (UNIT_MM == get_unit(pp->p_config))
		{
            cur_value = (group_get_range (get_current_group(pp->p_config)) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
			cur_value = cur_value * cos(TMP(current_angle[grp]));
			if(GROUP_VAL_POS( grp , point_qty_pos)== 0)
			{
				lower = ( 32 / 100.0) * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
			}
			else
			{
				lower = (GROUP_VAL_POS( grp , point_qty) / 100.0) * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
			}
			lower = lower * cos(TMP(current_angle[grp]));
			upper = MIN(max_tmp, max_tmp1) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
			upper = upper * cos(TMP(current_angle[grp]));
			if(GROUP_VAL_POS( grp , point_qty_pos)== 0)
			{
				switch (TMP(range_reg))
				{
					case 0:	tmpf = 10.0; break;
					case 1:	tmpf = 1.0; break;
					case 2:	tmpf = 100.0; break;
					default:break;
				}
				step = tmpf ;
			}
			else
			{
				step = tmpf * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
				step = step * cos(TMP(current_angle[grp]));
			}
			tmpfm = tmpfm * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
			digit = 2;
			pos = 2;
			unit = UNIT_MM;
		}
		else
		{
            cur_value = (group_get_range( get_current_group(pp->p_config)) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
			cur_value = cur_value * cos(TMP(current_angle[grp]));
			if(GROUP_VAL_POS( grp , point_qty_pos)== 0)
			{
				lower =	( 32 / 100.0) * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
			}
			else
			{
				lower =	(GROUP_VAL_POS( grp , point_qty) / 100.0) * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
			}
			lower = lower * cos(TMP(current_angle[grp]));
			upper = MIN(max_tmp, max_tmp1) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
			upper = upper * cos(TMP(current_angle[grp]));
			if(GROUP_VAL_POS( grp , point_qty_pos)== 0)
			{
				switch (TMP(range_reg))
				{
					case 0:	tmpf = 10.0; break;
					case 1:	tmpf = 1.0; break;
					case 2:	tmpf = 0.1; break;
					default:break;
				}
				step = tmpf ;
			}
			else
			{
				step = tmpf * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
				step = step * cos(TMP(current_angle[grp]));
			}
			tmpfm = tmpfm * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
			digit = 3;
			pos = 2;
			unit = UNIT_INCH;
		}
	}
	else if(UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit))
	{
		if (UNIT_MM == get_unit(pp->p_config))
		{
            cur_value = (group_get_range( get_current_group(pp->p_config) ) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */

			if(GROUP_VAL_POS( grp , point_qty_pos)== 0)
			{
				lower = ( 32 / 100.0) * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
			}
			else
			{
				lower = (GROUP_VAL_POS( grp , point_qty) / 100.0) * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
			}

			upper = MIN(max_tmp, max_tmp1) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
			if(GROUP_VAL_POS( grp , point_qty_pos)== 0)
			{
				switch (TMP(range_reg))
				{
					case 0:	tmpf = 100.0; break;
					case 1:	tmpf = 10.0; break;
					case 2:	tmpf = 1.0; break;
					default:break;
				}
				step = tmpf ;
			}
			else
			{
				step = tmpf * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
			}

			tmpfm = tmpfm * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
			digit = 2;
			pos = 2;
			unit = UNIT_MM;
		}
		else
		{
            cur_value = (group_get_range ( get_current_group(pp->p_config) ) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
			if(GROUP_VAL_POS( grp , point_qty_pos)== 0)
			{
				lower =	(32 / 100.0) * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
			}
			else
			{
				lower =	(GROUP_VAL_POS( grp , point_qty) / 100.0) * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
			}

			upper = MIN(max_tmp, max_tmp1) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
			if(GROUP_VAL_POS( grp , point_qty_pos)== 0)
			{
				switch (TMP(range_reg))
				{
					case 0:	tmpf = 10.0; break;
					case 1:	tmpf = 1.0; break;
					case 2:	tmpf = 0.1; break;
					default:break;
				}
				step = tmpf ;
			}
			else
			{
				step = tmpf * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
			}
			tmpfm = tmpfm * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
			digit = 3;
			pos = 2  ;
			unit = UNIT_INCH;
		}
	}
	else
	{
        cur_value = group_get_range ( get_current_group(pp->p_config) ) / 1000.0 ;
		if(GROUP_VAL_POS( grp , point_qty_pos)== 0)
		{
			lower =	32 / 100.0;
		}
		else
		{
			lower =	GROUP_VAL_POS( grp , point_qty) / 100.0;
		}

		upper = MIN(max_tmp, max_tmp1);
		if(GROUP_VAL_POS( grp , point_qty_pos)== 0)
		{
			switch (TMP(range_reg))
			{
				case 0:	tmpf = 10.0; break;
				case 1:	tmpf = 1.0; break;
				case 2:	tmpf = 0.1; break;
				default:break;
			}
			step = tmpf ;
		}
		else
		{
			step = tmpf;
		}
		digit = 2;
		pos = 2;
		unit = UNIT_US;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
	{//edit by lzk，条件放到这的目的是为了else状态时也计算lower
		draw3_digit_pressed (data_102, units[unit], cur_value , lower, upper, step, digit, pp, pos, 0);
	}
	else
	{
		if(UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit))
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
                cur_value = (group_get_range ( get_current_group(pp->p_config) ) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
				cur_value = cur_value * cos(TMP(current_angle[grp]));
				unit = UNIT_MM;
				digit = 2;
				pos = 2;
			}
			else
			{
                cur_value = (group_get_range ( get_current_group(pp->p_config) ) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
				cur_value = cur_value * cos(TMP(current_angle[grp]));
				unit = UNIT_INCH;
				digit = 3;
				pos = 2;
			}
		}
		else if(UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit))
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
                cur_value = (group_get_range ( get_current_group(pp->p_config)) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
				unit = UNIT_MM;
				digit = 2;
				pos = 2;
			}
			else
			{
                cur_value = (group_get_range ( get_current_group(pp->p_config) ) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
				unit = UNIT_INCH;
				digit = 3;
				pos = 2;
			}
		}
		else
		{
            cur_value = group_get_range ( get_current_group(pp->p_config) ) / 1000.0 ;
			unit = UNIT_US;
			pos = 2;
			digit = 2;
		}
		draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
	}
}
void DrawMenu112()
{
	double tmpf = 0.0 ;
	char *str ;
	double cur_value=0.0, lower=0, upper=0, step=0 ;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	pp->x_pos = 587, pp->y_pos = 288-26;
	switch (TMP(frequency_reg))
	{
		case 0:	tmpf = 0.01;	break;
		case 1:	tmpf = 0.1;		break;
		case 2:	tmpf = 1.0;		break;
		default:break;
	}
	if (GROUP_VAL_POS( grp , probe.Model[0]) != 32) 	/* 选择 unknown 时候可以调节频率 */
	{
		gtk_widget_set_sensitive (pp->eventbox30[2], FALSE);
		gtk_widget_set_sensitive (pp->eventbox31[2], FALSE);
	}

	if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 2) &&
			(GROUP_VAL_POS( grp , probe.Model[0]) == 32))
	{
		if (pp->mark_pop_change)
		{
			cur_value = get_group_val (p_grp, GROUP_FREQ_VAL) / 1000.0;
			lower =	1.0;
			upper =	20.0;
			step = tmpf;
			digit = 2;
			pos = 2;
			unit = UNIT_NONE;
			draw3_digit_pressed (data_1121, units[unit], cur_value,
					lower, upper, step, digit, NULL, pos, 0);
		}
		else
		{
			str = g_strdup_printf ("%0.2f", get_group_val (p_grp, GROUP_FREQ_VAL) / 1000.0);
			draw3_pop_tt (data_112, NULL, str,
					menu_content + FREQUENCY, 13, 2, get_group_val (p_grp, GROUP_FREQ_POS), 0);
			g_free (str);
		}
	}
	else
	{
		cur_value = get_group_val (p_grp, GROUP_FREQ_VAL) / 1000.0;
		unit = UNIT_NULL;
		pos = 2;
		digit = 2;
		draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
	}
}

void DrawMenu122()
{
	int grp = get_current_group(pp->p_config);
	GROUP* p_grp = get_group_by_id(pp->p_config , grp);
	pp->x_pos = 611, pp->y_pos = 288 - 26;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
		draw3_pop_tt (data_122, NULL,
				menu_content[RECTIFIER + get_group_val (p_grp, GROUP_RECTIFIER)],
				menu_content + RECTIFIER, 4, 2, get_group_val (p_grp, GROUP_RECTIFIER),
				((get_group_val (get_group_by_id(pp->p_config, grp), GROUP_VIDEO_FILTER)  == NORMAL_ON) ? 0x01 : 0x0));
	else
		draw3_popdown (menu_content[RECTIFIER + get_group_val (p_grp, GROUP_RECTIFIER)], 2, 0);

}
void DrawMenu132()
{
	double tmpf = 0.0 ;
	double cur_value=0.0, lower=0, upper=0, step=0 ;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);

	if (GROUP_VAL_POS( grp , group_mode) == PA_SCAN)
	{
		gtk_widget_set_sensitive (pp->eventbox30[2], FALSE);
		gtk_widget_set_sensitive (pp->eventbox31[2], FALSE);
		/* 计算当前beam的角度 */
		switch (LAW_VAL_POS(grp , Focal_type))
		{
			case AZIMUTHAL_SCAN:
				cur_value = 0.0;break;
			case LINEAR_SCAN:
				cur_value = 0.0;break;
			default:
				cur_value = 0.0;break;
		}
		unit = UNIT_NULL;
		pos = 2;
		digit = 1;
		draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
	}
	else// if (GROUP_VAL_POS( grp , group_mode) == UT_SCAN)
	{
		switch (TMP(angle_reg))
		{
			case 0:	tmpf = 1.0; break;
			case 1:	tmpf = 10.0; break;
			case 2:	tmpf = 100.0; break;
			default:break;
		}
		if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 2))
		{
			cur_value = LAW_VAL_POS(grp , Angle_min)/100.0 ;
			lower =	0.0;
			upper =	89.9;
			step = tmpf;
			digit = 1;
			pos = 2;
			unit = UNIT_NONE;
			draw3_digit_pressed (data_132, units[unit], cur_value,
					lower, upper, step, digit, NULL, pos, 0);
		}
		else
		{
			cur_value = LAW_VAL_POS(grp , Angle_min)/100.0 ;
			unit = UNIT_NULL;
			pos = 2;
			digit = 1;
			draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
		}
	}
}
void DrawMenu142()
{
	draw3_popdown (menu_content[OFF_ON + get_group_db_ref (pp->p_config, get_current_group (pp->p_config))], 2, 0);
	if (get_group_db_ref (pp->p_config, get_current_group (pp->p_config)))
		tt_label_show_string (pp->label[0], con2_p[1][0][6], "\n", "(dB)", "white", 10);
	else
		tt_label_show_string (pp->label[0], con2_p[1][0][0], "\n", "(dB)", "white", 10);
	gtk_widget_queue_draw (pp->status_area) ;
}

void DrawMenu202()
{
	double tmpf = 0.0;
	char *str = NULL;
	double cur_value=0.0, lower=0, upper=0, step=0 ;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);

	pp->x_pos = 608, pp->y_pos = 295 - 26;
	if (GROUP_GATE_POS(parameters) == 0) /* 闸门起点 */
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
		{
            if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit))
                    || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
			{
				switch (TMP(agate_start_reg))
				{
					case 0:	tmpf = 0.01; break;
					case 1:	tmpf = 0.1; break;
					case 2:	tmpf = 1.0; break;
					case 3:	tmpf = 10.0; break;
					default:break;
				}
				if (UNIT_MM == get_unit(pp->p_config))
				{
					cur_value = (GROUP_GATE_POS(start) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的起位数值mm */
					lower = 0;
					upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
					//step = tmpf * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
					step = tmpf;
					digit = 2;
					pos = 2;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = (GROUP_GATE_POS(start) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
					lower = 0;
					upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0 ) * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
					//step = tmpf * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
					step = tmpf;
					digit = 3;
					pos = 2;
					unit = UNIT_INCH;
				}
            } else {
                /*ut_unit 选择 time 时 */
				switch (TMP(agate_start_reg))
				{
                    case 0:	tmpf = (group_get_range (get_current_group(pp->p_config)) / 1000.0) / (double)(GROUP_VAL_POS( grp , point_qty)); break;
                    case 1:	tmpf = (group_get_range (get_current_group(pp->p_config)) / 1000.0) / 20.0 ; break;
                    case 2:	tmpf = (group_get_range (get_current_group(pp->p_config)) / 1000.0) / 10.0 ; break;
                    case 3:	tmpf = (group_get_range (get_current_group(pp->p_config)) / 1000.0) / 4.0 ; break;
					default:break;
				}
				cur_value = GROUP_GATE_POS(start) / 1000.0 ;
				lower = 0;
				upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0);
				step = tmpf;
				digit = 2;
				pos = 2;
				unit = UNIT_US;
			}
			draw3_digit_pressed (data_202, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 0);
		}
		else
		{
			if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
			{
				if (UNIT_MM == get_unit(pp->p_config))
				{
					cur_value = (GROUP_GATE_POS(start) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
					unit = UNIT_MM;
					digit = 2;
					pos = 2;
				}
				else
				{
					cur_value = (GROUP_GATE_POS(start) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
					unit = UNIT_INCH;
					digit = 3;
					pos = 2;
				}
			}
			else
			{
				cur_value = GROUP_GATE_POS(start)/1000.0;
				unit = UNIT_US;
				digit = 2;
				pos = 2;
			}
			draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
		}
	}
	else  /* 同步模式 */
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
		{
			if (GROUP_VAL_POS( grp , gate_pos) == GATE_B) /* 闸门选择B时候 有3个同步选项 */
			{
				if( GROUP_VAL_POS(grp , gate[GATE_A].measure) == 0 )//I/ A^
				{
					draw3_pop_tt (data_2021, NULL,
							menu_content[SYNCHRO + GROUP_GATE_POS(synchro)],
							menu_content + SYNCHRO, 3, 2, GROUP_GATE_POS(synchro), 0);
				}
				else//I/  A/
				{
					draw3_pop_tt (data_2021, NULL,
							menu_content[SYNCHRO + GROUP_GATE_POS(synchro)],
							menu_content + SYNCHRO_, 3, 2, GROUP_GATE_POS(synchro), 0);
				}
			}
			else if (GROUP_VAL_POS( grp , gate_pos) == GATE_A) /* 闸门选择A时候 有2个同步选项 */
			{
				draw3_pop_tt (data_2021, NULL,
						menu_content[SYNCHRO + GROUP_GATE_POS(synchro)],
						menu_content + SYNCHRO, 2, 2, GROUP_GATE_POS(synchro), 0);
			}
			else if (GROUP_VAL_POS( grp , gate_pos) == GATE_I) /* 闸门选择I时候 有1个同步选项 */
				draw3_pop_tt (data_2021, NULL,
						menu_content[SYNCHRO + GROUP_GATE_POS(synchro)],
						menu_content + SYNCHRO, 2, 2, GROUP_GATE_POS(synchro), 0);
		}
		else
			draw3_popdown (menu_content[SYNCHRO +GROUP_GATE_POS(synchro)], 2, 0);
		str = g_strdup_printf ("%s", con2_p[2][0][6]);
		gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
		if (str) {
			g_free (str);
        }
	}
	//gtk_widget_queue_draw(pp->draw_area->drawing_area);
	gtk_widget_queue_draw (pp->vboxtable);
}
void DrawMenu212()
{
}
void DrawMenu222()
{
}
void DrawMenu232()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower=0, upper=0, step=0 ;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);
	int _nBeamNo = TMP(beam_num[grp]) ;

	if (GROUP_VAL_POS( grp , SizingCurves.mode_pos) == 0)  /* MODE 为 Setup 时候 */
	{
		if ((GROUP_VAL_POS( grp , SizingCurves.curve_pos) == 1) || (GROUP_VAL_POS( grp , SizingCurves.curve_pos) == 2)) /* DAC or linear Dac*/
		{
			/* 当前步进 */
			switch (pp->p_tmp_config->ref_ampl_reg)
			{
				case 0:	tmpf = 0.1; break;
				case 1:	tmpf = 1.0; break;
				case 2:	tmpf = 10.0; break;
				default:break;
			}
			if(GROUP_VAL_POS(grp , SizingCurves.curve_pos) == 2)
			{// linear dac
				cur_value = GROUP_VAL_POS( grp , SizingCurves.linear_ref_ampl)/10.0;
			}
			else
			{// dac
				cur_value = GROUP_VAL_POS( grp , SizingCurves.dac_ref_ampl[_nBeamNo])/10.0;
			}
			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
			{
				lower = 0.0;
				upper = 100.0;
				step = tmpf;
				digit = 1;
				pos = 2;
				unit = UNIT_BFH;
				draw3_digit_pressed (data_232, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 6);
			}
			else
			{
				digit = 1;
				pos = 2;
				unit = UNIT_BFH;
				draw3_digit_stop (cur_value, units[unit], digit, pos, 6);
			}
		}
		else
		{
			gtk_widget_hide (pp->eventbox30[2]);
			gtk_widget_hide (pp->eventbox31[2]);
			gtk_widget_hide (pp->sbutton[2]);
		}
	}
	else if (GROUP_VAL_POS( grp , SizingCurves.mode_pos) == 1)  /* MODE 为Edit 时候 */
	{
		if( GROUP_VAL_POS( grp , SizingCurves.curve_pos) == 1|| GROUP_VAL_POS( grp , SizingCurves.curve_pos)==3)
			/* Curve选项为 DAC或TCG 时 */
		{
			/* 当前步进 */
			switch (pp -> p_tmp_config -> position_reg)
			{
				case 0:	tmpf = 0.01; break;
				case 1:	tmpf = 0.1; break;
				case 2:	tmpf = 1.0; break;
				case 3:	tmpf = 10.0; break;
				default:break;
			}

			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
			{
				if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
				{
					if (UNIT_MM == get_unit(pp->p_config))
					{
						cur_value = (GROUP_VAL_POS( grp , SizingCurves.position[_nBeamNo][GROUP_VAL_POS( grp , SizingCurves.point_pos)]) / 1000.0) *
							(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
						cur_value *= cos(TMP(current_angle[grp])) ;
						if(GROUP_VAL_POS( grp , SizingCurves.point_pos)){
							lower = (GROUP_VAL_POS( grp , SizingCurves.position[_nBeamNo][GROUP_VAL_POS( grp , SizingCurves.point_pos) - 1]) / 1000.0) *
								(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
							lower *= cos(TMP(current_angle[grp])) ;
                        } else {
                            lower = 0.02;
                        }
                        if (cur_value < lower) {
                            cur_value = lower;
                        }
						upper = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/100.0 * 4.95;
						step = tmpf;
						digit = 2;
						pos = 2;
						unit = UNIT_MM;
					}
					else
					{
						cur_value = (GROUP_VAL_POS( grp , SizingCurves.position[_nBeamNo][GROUP_VAL_POS( grp , SizingCurves.point_pos)]) / 1000.0) *
							(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
						cur_value *= cos(TMP(current_angle[grp])) * 0.03937 ;
						if(GROUP_VAL_POS( grp , SizingCurves.point_pos)){
							lower = (GROUP_VAL_POS( grp , SizingCurves.position[_nBeamNo][GROUP_VAL_POS( grp , SizingCurves.point_pos) - 1]) / 1000.0) *
								(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
							lower *= cos(TMP(current_angle[grp])) * 0.03937 ;
						}
						else
							lower = 0.0;
						upper = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/100.0 * 4.95 * 0.03937;
						step = tmpf/2.0;
						digit = 3;
						pos = 2;
						unit = UNIT_INCH;
					}
				}
				else
				{
					cur_value = GROUP_VAL_POS( grp , SizingCurves.position[_nBeamNo][GROUP_VAL_POS( grp , SizingCurves.point_pos)]) / 1000.0 ;
					if(GROUP_VAL_POS( grp , SizingCurves.point_pos))
						lower = GROUP_VAL_POS( grp , SizingCurves.position[_nBeamNo][GROUP_VAL_POS( grp , SizingCurves.point_pos) - 1]) / 1000.0 ;
					else
						lower = 0.0;
					upper = 9900.80;
					step = tmpf*5.0;
					digit = 2;
					pos = 2;
					unit = UNIT_US;
				}
				draw3_digit_pressed (data_2321, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 11);
			}
			else
			{
				if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
				{
					if (UNIT_MM == get_unit(pp->p_config))
					{
						cur_value = (GROUP_VAL_POS( grp , SizingCurves.position[_nBeamNo][GROUP_VAL_POS( grp , SizingCurves.point_pos)]) / 1000.0) *
							(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
						cur_value *= cos(TMP(current_angle[grp]))  ;
						digit = 2;
						pos = 2;
						unit = UNIT_MM;
					}
					else
					{
						cur_value = (GROUP_VAL_POS( grp , SizingCurves.position[_nBeamNo][GROUP_VAL_POS( grp , SizingCurves.point_pos)]) / 1000.0) *
							(get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
						cur_value *= cos(TMP(current_angle[grp])) * 0.03937 ;
						digit = 3;
						pos = 2;
						unit = UNIT_INCH;
					}
				}
				else
				{
					cur_value = GROUP_VAL_POS( grp , SizingCurves.position[_nBeamNo][GROUP_VAL_POS( grp , SizingCurves.point_pos)]) / 1000.0 ;
					digit = 2;
					pos = 2;
					unit = UNIT_US;
				}
				draw3_digit_stop (cur_value, units[unit], digit, pos, 11);
			}
		}
		else if (GROUP_VAL_POS( grp , SizingCurves.curve_pos == 2)) /* Curve 为 linear DAC */
		{
			/* 当前步进 */
			switch (pp->p_tmp_config->delay_reg)
			{
				case 0:	tmpf = 10.0; break;
				case 1:	tmpf = 1.0; break;
				case 2:	tmpf = 0.1; break;
				case 3: tmpf = 0.01; break;
				default:break;
			}

			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
			{
				if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
				{
					if (UNIT_MM == get_unit(pp->p_config))
					{
						cur_value = GROUP_VAL_POS( grp , SizingCurves.delay)/1000.0;
						cur_value *= (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
						cur_value *= cos(TMP(current_angle[grp])) ;
						lower = 0.0;
						upper = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 2000.0;
						step = tmpf;
						digit = 2;
						pos = 2;
						unit = UNIT_MM;
					}
					else
					{
						cur_value =GROUP_VAL_POS( grp , SizingCurves.delay)/1000.0*0.03937;
						cur_value *= (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
						cur_value *= cos(TMP(current_angle[grp])) ;
						lower = 0.0;
						upper = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 2000.0*0.03937;
						step = tmpf/10.0;
						digit = 3;
						pos = 2;
						unit = UNIT_INCH;
					}
				}
				else
				{
					cur_value =GROUP_VAL_POS( grp , SizingCurves.delay)/1000.0/2.945;
					lower = 0.0;
					upper = 100.0;
					step = tmpf/10.0;
					digit = 2;
					pos = 2;
					unit = UNIT_US;

				}

				draw3_digit_pressed (data_2322, units[unit], cur_value , lower,
						upper, step, digit, NULL, pos, 16);

			}
			else
			{
				if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
				{
					if (UNIT_MM == get_unit(pp->p_config))
					{
						cur_value =GROUP_VAL_POS( grp , SizingCurves.delay)/1000.0;
						cur_value *= (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
						cur_value *= cos(TMP(current_angle[grp])) ;
						digit = 2;
						pos = 2;
						unit = UNIT_MM;
					}
					else
					{
						cur_value =GROUP_VAL_POS( grp , SizingCurves.delay)/1000.0*0.03937;
						cur_value *= (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
						cur_value *= cos(TMP(current_angle[grp])) ;
						digit = 3;
						pos = 2;
						unit = UNIT_INCH;
					}
				}
				else
				{
					cur_value =GROUP_VAL_POS( grp , SizingCurves.delay)/1000.0/2.945;
					digit = 2;
					pos = 2;
					unit = UNIT_US;
				}
				draw3_digit_stop (cur_value, units[unit], digit, pos, 16);
			}

		}
		else
		{
			gtk_widget_hide (pp->eventbox30[2]);
			gtk_widget_hide (pp->eventbox31[2]);
			gtk_widget_hide (pp->sbutton[2]);
		}
	}
}
void DrawMenu242()
{
	if ( !con2_p[2][4][2] )
		gtk_widget_hide (pp->eventbox30[2]);
	gtk_widget_hide (pp->eventbox31[2]);
}

void DrawMenu302()
{
	pp->x_pos = 200, pp->y_pos = 0;
	int feildNum = GetMesureFeildNum();
	const char* pSimpleStr[feildNum];
	const char* pStr[feildNum];
	GetMesureFeildString(pSimpleStr ,pStr);

	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
		draw3_pop_tt_shen (data_302, NULL,
				pSimpleStr[get_reading_field1(pp->p_config)],
				pStr, feildNum, 2, get_reading_field1(pp->p_config), 0);
	else
		draw3_popdown (pSimpleStr[get_reading_field1(pp->p_config)], 2, 0);
}

void DrawMenu312()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower=0, upper=0, step=0 ;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);
	int _nSelection = GROUP_VAL_POS( grp , selection)  ;

	if(_nSelection == 0)
	{
		/* 当前步进 */
		switch (TMP(per_measure_reg))
		{
			case 0:	tmpf = 1; break;
			case 1:	tmpf = 10; break;
			case 2:	tmpf = 0.1; break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
		{
			cur_value = GROUP_VAL_POS( grp , per_measure)/100.0;
			lower = 0.0;
			upper = 100.0;
			step = tmpf;
			digit = 1;
			pos = 2;
			unit = UNIT_BFH;
			draw3_digit_pressed (data_312, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 0);
		}
		else
		{
			cur_value = GROUP_VAL_POS( grp , per_measure)/100.0;
			digit = 1;
			pos = 2;
			unit = UNIT_BFH;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		}
	}
	else if (_nSelection == 1 || _nSelection == 2 )
	{
		/* 当前步进 */
		switch (pp->p_tmp_config->s_measure_reg)
		{
			case 0:	tmpf = 1; break;
			case 1:	tmpf = 5; break;
			case 2:	tmpf = 0.1; break;
			case 3:	tmpf = 0.5; break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
		{
			if(get_inspec_source (pp->p_config) == 0)
			{
				cur_value = GROUP_VAL_POS( grp , s_measure)/100.0;
				double rate =  GROUP_VAL_POS(0 , prf1) * get_area_scanresolution (pp->p_config) / 10000.0;
				cur_value /= rate ;
				lower = -1000.0 ;
				upper =  1000.0;
				step = tmpf;
				digit = 1;
				pos = 2;
				unit = UNIT_S;
			}
			else
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					cur_value = GROUP_VAL_POS( grp , s_measure)/100.0;
					lower = -10000.0;
					upper =  10000.0;
					step = tmpf;
					digit = 1;
					pos = 2;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = GROUP_VAL_POS( grp , s_measure)/100.0*0.03937;
					lower = -10000.0*0.03937;
					upper =  10000.0*0.03937;
					step = tmpf/500.0;
					digit = 3;
					pos = 2;
					unit = UNIT_INCH;
				}
			}

			draw3_digit_pressed (data_3121, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 7);
		}
		else
		{
			if(get_inspec_source (pp->p_config) == 0)
			{
				cur_value = GROUP_VAL_POS( grp , s_measure)/100.0;
				double rate =  GROUP_VAL_POS(0 , prf1) * get_area_scanresolution (pp->p_config) / 10000.0;
				cur_value /= rate ;
				digit = 1;
				pos = 2;
				unit = UNIT_S;
			}
			else
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					cur_value = GROUP_VAL_POS( grp , s_measure)/100.0;
					digit = 1;
					pos = 2;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = GROUP_VAL_POS( grp , s_measure)/100.0*0.03937;
					digit = 3;
					pos = 2;
					unit = UNIT_INCH;
				}
			}
			draw3_digit_stop (cur_value, units[unit], digit, pos, 7);
		}

		//if(_nSelection == 1)
		//{
		//	gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
		//	gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
		//}

	}
	else if (_nSelection == 3)
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
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
					pos = 2;
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
					pos = 2;
					unit = UNIT_INCH;
				}
			}
			else	/* us */
			{
				cur_value = GROUP_VAL_POS( grp , u_reference)/1000.0;
				//lower = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START) / 1000.0;
				//upper = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_START) / 1000.0 + get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0 ;
				lower = 0 ;
				upper = 1000 ;
				step = tmpf;
				digit = 2;
				pos = 2;
				unit = UNIT_US;
			}
			draw3_digit_pressed (data_313, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 3);
		}
		else
		{
			if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					cur_value = GROUP_VAL_POS( grp , u_reference)/1000.0 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
					digit = 2;
					pos = 2;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = GROUP_VAL_POS( grp , u_reference)/1000.0 * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY)/200000.0);
					digit = 3;
					pos = 2;
					unit = UNIT_INCH;
				}
			}
			else
			{
				cur_value = GROUP_VAL_POS( grp , u_reference)/1000.0;
				digit = 2;
				pos = 2;
				unit = UNIT_US;
			}
			draw3_digit_stop (cur_value, units[unit], digit, pos, 3);
		}

	}
	else if (_nSelection == 4|| _nSelection == 5||_nSelection == 6)
	{
		/* 当前步进 */
		switch (TMP(cursors_amplitude_reg))
		{
			case 0:	tmpf = 0.1; break;
			case 1:	tmpf = 1.0; break;
			case 2:	tmpf = 10.0; break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
		{
			cur_value = GROUP_CURSORS_POS(amplitude)/100.0;
			lower = 0.0;
			upper = 100.0;
			step = tmpf;
			digit = 1;
			pos = 2;
			unit = UNIT_BFH;
			draw3_digit_pressed (data_3122, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 13);
		}
		else
		{
			cur_value = GROUP_CURSORS_POS(amplitude)/100.0;
			digit = 1;
			pos = 2;
			unit = UNIT_BFH;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 13);
		}
	}
	else  if (_nSelection == 7)
	{
		/* 当前步进 */
		switch (TMP(cursors_scan_reg))
		{
			case 0:	tmpf = 0.5; break;
			case 1:	tmpf = 2.5; break;
			case 2:	tmpf = 5.0; break;
			case 3:	tmpf = 7.5; break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
		{

			if(get_inspec_source(pp->p_config))
			{
				lower = pp->p_config->inspection_scan_start / 1000.0;
				upper = pp->p_config->inspection_scan_end / 1000.0;
			}
			else
			{
				lower = 0;
				upper = pp->p_config->inspection_scan_end / 1000.0  ;
				upper *= pp->p_config->inspection_scan_resolution / 1000.0 ;
			}

			if(get_unit(pp->p_config) == UNIT_MM)
			{
				cur_value = TMP(scan)/100.0;
				step = tmpf;
				digit = 1;
				pos = 2;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = TMP(scan)/100.0*0.03937;
				lower *= 0.03937;
				upper *= 0.03937;
				step = tmpf;
				digit = 3;
				pos = 2;
				unit = UNIT_INCH;
			}
			draw3_digit_pressed (data_3142, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 15);
		}
		else
		{
			if(get_unit(pp->p_config) == UNIT_MM)
			{
				cur_value = TMP(scan)/100.0;
				digit = 1;
				pos = 2;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = TMP(scan)/100.0*0.03937;
				digit = 3;
				pos = 2;
				unit = UNIT_INCH;
			}
			draw3_digit_stop (cur_value, units[unit], digit, pos, 15);
		}
	}
	//gtk_widget_queue_draw(pp->draw_area->drawing_area);
}

void DrawMenu322()
{
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
		tableAddEntry();
	draw3_popdown(NULL,2,1);
	if(get_display_table(pp->p_config))
	{
		gtk_widget_set_sensitive(pp->eventbox30[2] ,FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[2] ,FALSE);
	}
}

void DrawMenu332()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower=0, upper=0, step=0 ;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group(pp->p_config);
	/* 当前步进 */
	switch (TMP(max_thickness_reg))
	{
		case 0:	tmpf = 1; break;
		case 1:	tmpf = 10; break;
		case 2:	tmpf = 0.1; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
	{
		if(UNIT_MM == get_unit(pp->p_config))
		{
			cur_value = GROUP_VAL_POS(grp , max_thickness) /1000.0 ;
			lower = GROUP_VAL_POS(grp , min_thickness) /1000.0  + 0.01;
			upper = 99999.00;
			step = tmpf;
			digit = 2;
			pos = 2;
			unit = UNIT_MM;
		}
		else
		{
			cur_value = (GROUP_VAL_POS(grp , max_thickness) /1000.0 ) * 0.03937;
			lower = (GROUP_VAL_POS(grp , min_thickness) /1000.0 ) * 0.03937 + 0.005;
			upper = 99999.00 * 0.03937;
			step = tmpf/2.0;
			digit = 3;
			pos = 2;
			unit = UNIT_INCH;
		}
		draw3_digit_pressed (data_332, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 0);
	}
	else
	{
		if(UNIT_MM == get_unit(pp->p_config))
		{
			cur_value = GROUP_VAL_POS(grp , max_thickness) /1000.0 ;
			digit = 2;
			pos = 2;
			unit = UNIT_MM;
		}
		else
		{
			cur_value = ( GROUP_VAL_POS(grp , max_thickness) /1000.0 ) * 0.03937;
			digit = 3;
			pos = 2;
			unit = UNIT_INCH;
		}
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
	}
}
void DrawMenu342()
{
	if ( !con2_p[3][4][2] )
		gtk_widget_hide (pp->eventbox30[2]);
	gtk_widget_hide (pp->eventbox31[2]);
}

void DrawMenu402()
{
	char* str ;
	int menu_status = 0;

	pp->x_pos = 580, pp->y_pos = 330;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))/*选中402这个位置*/
	{
		if(get_display_pos(pp->p_config) == A_C_CC_SCAN)/*Display 为 A-C-[C]*/
		{
			draw3_pop_tt (data_402, NULL,
					menu_content[C_SCAN1 + get_cscan_source(pp->p_config, 1)],
					menu_content + C_SCAN1, 5, 2, get_cscan_source(pp->p_config, 1), 0);
			str = g_strdup_printf ("%s", con2_p[4][0][7]);
			gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
			gtk_widget_queue_draw(pp->vboxtable);

		}
		else if(get_display_pos(pp->p_config)==A_S_CC_SCAN)
			/*Display 为 A-S-[C]*/
		{
			if (get_group_qty(pp->p_config) == 1)
				menu_status = 0x01;
			draw3_pop_tt (data_401, NULL,
					menu_content[GROUP_POS + get_display_group(pp->p_config)],
					menu_content + GROUP_POS, 2, 2, get_display_group(pp->p_config), menu_status);
			str = g_strdup_printf ("%s", con2_p[4][0][1]);
			gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);

		}
		else if(get_display_pos(pp->p_config)==Strip_Chart_AA)
			/*Display 为 Strip Chart-[A]*/
		{
			draw3_pop_tt (data_4021, NULL,
					menu_content[DATA2+get_stripscan_data2(pp->p_config)],
					menu_content + DATA2, 4, 2, get_stripscan_data2(pp->p_config), 0);
			str = g_strdup_printf ("%s", con2_p[4][0][9]);
			gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);

		}
		else
		{
			gtk_widget_hide (pp->eventbox30[2]);
			gtk_widget_hide (pp->eventbox31[2]);
		}

	}
	else
	{
		if(get_display_pos(pp->p_config)==A_C_CC_SCAN)/* Display 为 A-C-[C] */
		{
			draw3_popdown (menu_content[C_SCAN1+get_cscan_source(pp->p_config, 1)], 2, 0);
			str = g_strdup_printf ("%s", con2_p[4][0][7]);
			gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
		}

		else if(get_display_pos(pp->p_config)==A_S_CC_SCAN)/* Display 为 A-S-[C] */
		{
			draw3_popdown (menu_content[GROUP_POS+get_display_group(pp->p_config)], 2, 0);
			str = g_strdup_printf ("%s", con2_p[4][0][1]);
			gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
		}

		else if(get_display_pos(pp->p_config)==Strip_Chart_AA)/* Display 为 Strip Chart-[A] */
		{
			draw3_popdown (menu_content[DATA2+get_stripscan_data2(pp->p_config)], 2, 0);
			str = g_strdup_printf ("%s", con2_p[4][0][9]);
			gtk_label_set_text (GTK_LABEL (pp->label3[2]), str);
		}
		else
		{
			gtk_widget_hide (pp->eventbox30[2]);
			gtk_widget_hide (pp->eventbox31[2]);
		}
	}
}

void DrawMenu412()
{
	draw3_popdown(menu_content[OFF_ON + get_overlay_sizing_curves(pp->p_config)],2,0);
}

void DrawMenu422()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower=0, upper=0, step=0;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);

	if(GROUP_VAL_POS( grp , col_select_pos) == 0)
	{
		/* 当前步进 */
		switch (TMP(color_end_reg))
		{
			case 0:	tmpf = 1.0; break;
			case 1:	tmpf = 10.0; break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
		{
			cur_value = GROUP_VAL_POS( grp , col_end);
			lower = GROUP_VAL_POS( grp , col_start)+1;
			upper = 100.0;
			step = tmpf;
			digit = 0;
			pos = 2;
			unit = UNIT_BFH;
			draw3_digit_pressed (data_432, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 0);
		}
		else
		{
			cur_value = GROUP_VAL_POS( grp , col_end);
			digit = 0;
			pos = 2;
			unit = UNIT_BFH;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		}
	}
	else if(GROUP_VAL_POS( grp , col_select_pos) == 1)
	{
		/* 当前步进 */
		switch (TMP(brightness_reg))
		{
			case 0:	tmpf = 1.0; break;
			case 1:	tmpf = 10.0; break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
		{
			cur_value = GROUP_VAL_POS( grp , col_brightness);
			lower = (int)((100.0-GROUP_VAL_POS( grp , col_contrast))/2.0);
			upper = 100.0-lower;
			step = tmpf;
			digit = 0;
			pos = 2;
			unit = UNIT_BFH;
			draw3_digit_pressed (data_4321, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 7);
		}
		else
		{
			cur_value = GROUP_VAL_POS( grp , col_brightness);
			digit = 0;
			pos = 2;
			unit = UNIT_BFH;
			draw3_digit_stop (cur_value, units[unit], digit, pos,7);
		}
	}
	else if (GROUP_VAL_POS( grp , col_select_pos) == 2)
	{
		/* 当前步进 */
		switch (TMP(max_reg))
		{
			case 0:	tmpf = 0.01; break;
			case 1:	tmpf =  1.0; break;
			case 2:	tmpf = 10.0; break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
		{
			cur_value = GROUP_VAL_POS( grp , col_max)/1000.0;
			lower = GROUP_VAL_POS( grp , col_min)/1000.0 + 0.01;
			upper = 99999.0;
			step = tmpf;
			digit = 2;
			pos = 2;
			unit = UNIT_MM;
			draw3_digit_pressed (data_4322, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 9);
		}
		else
		{
			cur_value = GROUP_VAL_POS( grp , col_max)/1000.0;
			digit = 2;
			pos = 2;
			unit = UNIT_MM;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 9);
		}
	}
	gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
	gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
}

void DrawMenu432()
{
	//char temp[52];
	int grp = get_current_group (pp->p_config);

	switch(get_dis_prop_scan(pp->p_config))
	{
		case 0: /* Ascan 时候的包络 */
			pp->x_pos = 596, pp->y_pos = 320-26;
			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
				draw3_pop_tt (data_442, NULL,
						menu_content[ENVELOPE + GROUP_VAL_POS( grp , ascan_envelope)],
						menu_content+ENVELOPE, 2, 2, GROUP_VAL_POS( grp , ascan_envelope), 0);
			else
				draw3_popdown (menu_content[ENVELOPE + GROUP_VAL_POS( grp , ascan_envelope)], 2, 0);
			break;
		case 1: /* Bscan 时候是optimum */
			//draw3_popdown (menu_content[OFF_ON + get_dis_prop_boptimum (pp->p_config)], 2, 0);
			//g_sprintf (temp,"%s", con2_p[4][3][7]);
			//gtk_label_set_text (GTK_LABEL (pp->label3[2]), temp);
			//break;
		case 2:
		case 3:
			gtk_widget_hide(pp->eventbox30[2]);
			gtk_widget_hide(pp->eventbox31[2]);
			break;
		default:break;
	}
}

void DrawMenu442()
{
	if ( !con2_p[4][4][2] )
		gtk_widget_hide (pp->eventbox30[2]);
	gtk_widget_hide (pp->eventbox31[2]);
}

void DrawMenu502()
{
	pp->x_pos = 562, pp->y_pos = 288-26;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
		draw3_pop_tt (data_502, NULL,
				menu_content[PROB_SELECT + get_probe_select (pp->p_config)],
				menu_content + PROB_SELECT, 2, 2, get_probe_select (pp->p_config), 0);
	else
		draw3_popdown (menu_content[PROB_SELECT + get_probe_select (pp->p_config)], 2, 0);
}

void DrawMenu512()
{
	double cur_value=0.0 ;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);

	pp->x_pos = 588, pp->y_pos = 285-26;

	if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 2))
	{
		draw3_pop_tt (data_512, NULL,
				menu_content[PROB_SKEW + GROUP_VAL_POS( grp , skew_pos)], menu_content+PROB_SKEW, 4, 2,
				GROUP_VAL_POS( grp , skew_pos), 0);
	}
	else
	{
		cur_value = GROUP_VAL_POS( grp , skew)/100.0 ;
		unit = UNIT_NULL;
		pos = 2;
		digit = 1;
		draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
	}
}

void DrawMenu522()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower=0, upper=0, step=0 ;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);

	switch (TMP(agate_start_reg))
	{
		case 0:	tmpf = 0.01; break;
		case 1:	tmpf = 0.1; break;
		case 2:	tmpf = 1; break;
		case 3:	tmpf = 10; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
	{
		if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
				cur_value = (GROUP_VAL_POS( grp , gate[0].start) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的起位数值mm */
				lower = 0;
				upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
				step = tmpf;// * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);
				digit = 2;
				pos = 2;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = (GROUP_VAL_POS( grp , gate[0].start) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
				lower = 0;
				upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0 ) * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
				step = tmpf;// * 0.03937 * get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0;
				digit = 3;
				pos = 2;
				unit = UNIT_INCH;
			}
		}
		else
		{
			cur_value = GROUP_VAL_POS( grp , gate[0].start) / 1000.0 ;
			lower =	0;
			upper =	(MAX_RANGE_US - GROUP_GATE_POS(width) / 1000.0);
			step = tmpf;
			digit = 2;
			pos = 2;
			unit = UNIT_US;
		}
		draw3_digit_pressed (data_202, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 0);
	}
	else
	{
		if ((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
				cur_value = (GROUP_VAL_POS( grp , gate[0].start) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
				unit = UNIT_MM;
				digit = 2;
				pos = 2;
			}
			else
			{
				cur_value = (GROUP_VAL_POS( grp , gate[0].start) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
				unit = UNIT_INCH;
				digit = 3;
				pos = 2;
			}
		}
		else
		{
			cur_value = GROUP_VAL_POS( grp , gate[0].start)/1000.0;
			unit = UNIT_US;
			digit = 2;
			pos = 2;
		}
		draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
	}
}

void DrawMenu532()
{
	double tmpf = 0.0, tmp;
	double cur_value=0.0, lower=0, upper=0, step=0 ;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group(pp->p_config);

    if (get_part_geometry(grp) == 0) {
        gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
        return;
    }
    switch (pp->p_tmp_config->diameter_reg) {
        case 0:	tmpf = 0.1; break;
        case 1:	tmpf = 1.0; break;
        case 2:	tmpf = 10.0; break;
        case 3:	tmpf = 100.0; break;
        default:break;
    }

    if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2)) {
        if(UNIT_MM == get_unit(pp->p_config)) {
            cur_value = get_part_diameter(grp) / 1000.0;
            lower = 2.0 * GROUP_VAL_POS(grp , part.Thickness) / 1000.0;
            tmp = GROUP_VAL_POS(grp , wedge.length)/1000.0;
	        if (lower < tmp) {
	            lower = tmp;
            }
            upper = 1000000.00;
            step = tmpf;
            digit = 2;
            pos = 2;
            unit = UNIT_MM;
        } else {
            cur_value = get_part_diameter(grp) / 1000.0 * 0.03937;
            lower = 2.0 * GROUP_VAL_POS(grp , part.Thickness) / 1000.0*0.03937;
            tmp = GROUP_VAL_POS(grp , wedge.length)/1000.0*0.03937;
	        if (lower < tmp) {
	            lower = tmp;
            }
            upper = 1000000.00 * 0.03937;
            step = tmpf;
            digit = 3;
            pos = 2;
            unit = UNIT_INCH;
        }
        draw3_digit_pressed (data_532, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 0);
    } else {
        if(UNIT_MM == get_unit(pp->p_config)) {
            cur_value = get_part_diameter(grp) / 1000.0;
            digit = 2;
            pos = 2;
            unit = UNIT_MM;
        } else {
            cur_value = get_part_diameter(grp) / 1000.0 * 0.03937;
            digit = 3;
            pos = 2;
            unit = UNIT_INCH;
        }
        draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
    }
#if 0

	switch(get_part_geometry(grp))
	{
		case 0:
			if(UNIT_MM == get_unit(pp->p_config))
			{
				cur_value = get_part_diameter(grp) / 1000.0;
				digit = 2;
				pos = 2;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = get_part_diameter (grp) / 1000.0 * 0.03937;
				digit = 3;
				pos = 2;
				unit = UNIT_INCH;
			}
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
			gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
			gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
			break;
		case 1:
			/* 当前步进 */
			switch (pp->p_tmp_config->diameter_reg)
			{
				case 0:	tmpf = 0.01; break;
				case 1:	tmpf = 0.1; break;
				case 2:	tmpf = 1.0; break;
				case 3:	tmpf = 10.0; break;
				default:break;
			}
			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
			{
				if(UNIT_MM == get_unit(pp->p_config))
				{
					cur_value = get_part_diameter(grp) / 1000.0;
					lower = 2.0 * GROUP_VAL_POS(grp , part.Thickness) / 1000.0;
					upper = 1000000.00;
					step = tmpf;
					digit = 2;
					pos = 2;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = get_part_diameter(grp) / 1000.0 * 0.03937;
					lower = 2.0 * GROUP_VAL_POS(grp , part.Thickness) / 1000.0*0.03937;
					upper = 1000000.00 * 0.03937;
					step = tmpf;
					digit = 3;
					pos = 2;
					unit = UNIT_INCH;
				}
				draw3_digit_pressed (data_532, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 0);
			}
			else
			{
				if(UNIT_MM == get_unit(pp->p_config))
				{
					cur_value = get_part_diameter(grp) / 1000.0;
					digit = 2;
					pos = 2;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = get_part_diameter(grp) / 1000.0 * 0.03937;
					digit = 3;
					pos = 2;
					unit = UNIT_INCH;
				}
				draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
			}
			break;
		case 2:
			/* 当前步进 */
			switch (pp->p_tmp_config->diameter_reg)
			{
				case 0:	tmpf = 0.01; break;
				case 1:	tmpf = 0.1; break;
				case 2:	tmpf = 1.0; break;
				case 3:	tmpf = 10.0; break;
				default:break;
			}
			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
			{
				if(UNIT_MM == get_unit (pp->p_config))
				{
					cur_value = get_part_diameter(grp) / 1000.0;
					lower = 2.0 * GROUP_VAL_POS(grp , part.Thickness) / 1000.0 + 10.0;
					upper = 1000000.00;
					step = tmpf;
					digit = 2;
					pos = 2;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = get_part_diameter(grp) / 1000.0 * 0.03937;
					lower =(2.0 * GROUP_VAL_POS(grp , part.Thickness) / 1000.0 + 10.0)*0.03937;
					upper = 1000000.00 * 0.03937;
					step = tmpf;
					digit = 3;
					pos = 2;
					unit = UNIT_INCH;
				}
				draw3_digit_pressed (data_532, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 0);
			}
			else
			{
				if(UNIT_MM == get_unit (pp->p_config))
				{
					cur_value = get_part_diameter(grp) / 1000.0;
					digit = 2;
					pos = 2;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = get_part_diameter(grp) / 1000.0 * 0.03937;
					digit = 3;
					pos = 2;
					unit = UNIT_INCH;
				}
				draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
			}
			break;
		default:break;
	}
#endif
}
void DrawMenu542()
{
	/*
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
	{
		pp->pos_pos = MENU3_STOP;
		draw3_popdown(NULL,2,1);
	}
	else
		draw3_popdown(NULL,2,1);
	*/
	//if ( !con2_p[5][4][2] )
/*
	char* str ;
	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit;

	switch (TMP(prf_reg))
	{
		case 0:	tmpf = 1.0; break;
		case 1:	tmpf = 10.0; break;
		case 2:	tmpf = 100.0; break;
		default:break;
	}
	if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 5))
	{
		cur_value = get_prf() / 10.0;
		lower =	1.0;
		upper = MultiGroupGetMaxPrf() / 10.0;
		step = tmpf;
		digit = 0;
		pos = 5;
		unit = UNIT_NONE;
		draw3_digit_pressed (data_1151, units[unit], cur_value,
				lower, upper, step, digit, pp, pos, 0);
	}
	else
	{
		cur_value = GROUP_VAL_POS(0 , prf1) / 10.0;
		unit = UNIT_NULL;
		pos = 5;
		digit = 0;
		draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
	}
*/
	gtk_widget_hide (pp->eventbox30[2]);
	gtk_widget_hide (pp->eventbox31[2]);
}

void DrawMenu602()
{
	double tmpf = 0.0;
	char *str = NULL;
	double cur_value=0.0, lower=0, upper=0, step=0;
	int digit = 0, pos;
	int grp = get_current_group (pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	switch (TMP(receiver_reg))
	{
		case 0:	tmpf = 1.0; break;
		case 1:	tmpf = 10.0; break;
		case 2:	tmpf = 100.0; break;
		default:break;
	}
    if (group_get_rx_tx_mode(grp) == PULSE_ECHO)	/* 脉冲回波模式不可以调节 */
	{
		gtk_widget_set_sensitive (pp->eventbox30[2], FALSE);
		gtk_widget_set_sensitive (pp->eventbox31[2], FALSE);
	}
	//upper = (double) (128 + 1 - LAW_VAL_POS(grp , Elem_qty));
	upper = (double)getPulserReceiverRange(grp);
    if ((MENU_STATUS == MENU3_PRESSED)
            && (CUR_POS == 2)
            && !TMP(freeze)) {
		cur_value = (double) (get_group_val (&pp->p_config->group[get_current_group(pp->p_config)], GROUP_RECEIVER));
		lower = 1.0;
		step = tmpf;
		digit = 0;
		pos = 2;
		str = g_strdup_printf ("( 1 to %d)", (int)(upper));
		draw3_digit_pressed (data_120, str, cur_value , lower, upper, step, digit, NULL, pos, 0);
	}
	else
	{
		cur_value = (double) (get_group_val (&pp->p_config->group[get_current_group(pp->p_config)], GROUP_RECEIVER));
		digit = 0;
		pos = 2;
		str = g_strdup_printf ("( 1 to %d)", (int)(upper));
		draw3_digit_stop (cur_value, str, digit, pos, 0);
	}
	g_free(str);
}
void DrawMenu612()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower=0, upper=0, step=0 ;// temp_beam;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);

	switch (pp->p_tmp_config->angle_step_reg)
	{
		case 0:	tmpf = 2.0; break;
		case 1:	tmpf = 1.0; break;
		case 2:	tmpf = 0.1; break;
		default:break;
	}
	if ((LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN) &&
			(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON))
		/* 角度扫查时开始自动计算聚焦法则时候可以调节 */
	{
        if ((pp->pos_pos == MENU3_PRESSED)
                && (CUR_POS == 2)
                && !TMP(freeze)) {
			cur_value = LAW_VAL_POS(grp , Angle_step) / 100.0;
			lower = 0.1 ;
			upper = 10  ;
			step = tmpf;
			digit = 1;
			pos = 2;
			unit = UNIT_DEG;
			draw3_digit_pressed (data_612, units[unit], cur_value,
					lower, upper, step, digit, NULL, pos, 0);
		}
		else
		{
			cur_value = LAW_VAL_POS(grp , Angle_step) / 100.0;
			digit = 1;
			pos = 2;
			unit = UNIT_DEG;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		}
	}
	else
	{
		cur_value = LAW_VAL_POS(grp , Angle_step) / 100.0;
		digit = 1;
		pos = 2;
		unit = UNIT_DEG;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
	}
}
void DrawMenu622()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower=0, upper=0, step=0 ;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);
	float multiplier = 1.0;
	if(UNIT_MM == get_unit(pp->p_config))
	{
		multiplier = 1.0;
	}else
	{
		multiplier = 0.03937;
	}

	if(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON)
		/* 聚焦法则自动计算开启时, Max Angle 才可调节 */
	{
        if ((pp->pos_pos == MENU3_PRESSED)
                && (CUR_POS == 2)
                && !TMP(freeze)) {
			//if((LAW_VAL_POS(grp , Focal_point_type)==HALFPATH_P)||(LAW_VAL_POS(grp , Focal_point_type)==DEPTH_P))
			if(LAW_VAL_POS(grp , Focal_type) == 2)
				/*law type 为 depth sector 时*/
			{
				switch (TMP(positione_reg))
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
				cur_value = LAW_VAL_POS(grp , Position_end) / 1000.0 * multiplier;
				lower = 0.1 * multiplier;
				upper = 10000.0 * multiplier;
				step = tmpf;
				pos = 2;

				draw3_digit_pressed (data_622, units[unit], cur_value , lower, upper,
						step, digit, NULL, pos, 0);
			}
			else if(LAW_VAL_POS(grp , Focal_point_type)==FOCALPLANE_P)
				/*type 为 focal plane 时*/
			{
				switch (TMP(offsete_reg))
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
				cur_value = LAW_VAL_POS(grp , Offset_end) / 1000.0 * multiplier;
				lower = -10000 * multiplier;
				upper = 10000.0 * multiplier;
				step = tmpf;
				pos = 2;

				draw3_digit_pressed (data_6221, units[unit], cur_value , lower, upper,
						step, digit, NULL, pos, 7);
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
				cur_value = LAW_VAL_POS(grp , Position_end) / 1000.0 * multiplier;
				pos = 2;
				draw3_digit_stop (cur_value, units[unit], digit, pos, 0);

				gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
				gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
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
				cur_value = LAW_VAL_POS(grp , Position_end) / 1000.0 * multiplier;
				pos = 2;
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
				cur_value = LAW_VAL_POS(grp , Offset_end) / 1000.0 * multiplier;
				pos = 2;
				draw3_digit_stop (cur_value, units[unit], digit, pos, 7);
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
				cur_value = LAW_VAL_POS(grp , Position_end) / 1000.0 * multiplier;
				pos = 2;
				draw3_digit_stop (cur_value, units[unit], digit, pos, 0);

				gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
				gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
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
		cur_value = LAW_VAL_POS(grp , Position_end) / 1000.0 * multiplier;
		pos = 2;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
	}
}

void DrawMenu632()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower=0, upper=0, step=0;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group (pp->p_config);
	switch (TMP(last_element_reg))
	{
		case 0:	tmpf = 1.0; break;
		case 1:	tmpf = 10.0; break;
		case 2:	tmpf = 100.0; break;
		default:break;
	}
	/* 聚焦法则自动计算开启时  */
	if ((get_auto_focal (pp->p_config) == AUTO_FOCAL_ON) &&
			(LAW_VAL_POS(grp , Focal_type) == LINEAR_SCAN))
	{
        if ((pp->pos_pos == MENU3_PRESSED)
                && (CUR_POS == 2)
                && !TMP(freeze)) {
			cur_value = LAW_VAL_POS(grp , Last_tx_elem);
			lower = LAW_VAL_POS(grp , First_tx_elem) + LAW_VAL_POS(grp , Elem_qty) - 1;
			upper = MIN(gMaxElementRecieveQty , GROUP_VAL_POS (grp , probe.Elem_qty)) ;
			step  = tmpf;
			digit = 0;
			pos   = 2;
			unit  = UNIT_NONE;
			draw3_digit_pressed (data_632, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 0);
		}
		else
		{
			cur_value = LAW_VAL_POS(grp , Last_tx_elem);
			digit = 0;
			pos = 2;
			unit = UNIT_NONE;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		}
	}
	else /*Law Config 不为 Linear 时，Last Element  Unsensitive*/
	{
		cur_value = LAW_VAL_POS(grp , Last_tx_elem);
		digit = 0;
		pos = 2;
		unit = UNIT_NONE;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
	}
}

void DrawMenu642()
{
	//if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
	//	draw_dialog_all (DIALOG_LAW_SAVE);
	//else
		draw3_popdown(NULL, 2, 1);
	gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
	gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
}

void DrawMenu702()
{
	unsigned char _nCurrentEcoderType ;
	unsigned char _nInspecSource = (int)get_inspec_source (pp->p_config) - 1;
	if(_nInspecSource != 0 && _nInspecSource != 1)
	{
		_nCurrentEcoderType = 3 ;
	}
	else
	{
		_nCurrentEcoderType  = get_enc_type (pp->p_config,  _nInspecSource );
		_nCurrentEcoderType = (_nCurrentEcoderType & 0x3 ) ;
		_nCurrentEcoderType = 3 - _nCurrentEcoderType ;
	}

	pp->x_pos = 540, pp->y_pos = 288-26;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
		draw3_pop_tt (data_702, NULL,
				menu_content[E_TYPE + _nCurrentEcoderType],
				menu_content + E_TYPE, 3, 2, _nCurrentEcoderType, 0);
	else
		draw3_popdown (menu_content[E_TYPE + _nCurrentEcoderType], 2, 0);

	if(!get_inspec_source (pp->p_config))
	{
		gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
	}

}
void DrawMenu712()
{
	int menu_status = 0;
	pp->x_pos = 570, pp->y_pos = 305;
	if(get_inspec_type (pp->p_config)==0)
	{
		draw3_popdown (menu_content[I_INDEX + 4], 2, 0);

		gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
	}

	else if(get_inspec_type (pp->p_config)==1)
	{
		if(get_inspec_source (pp->p_config)==0)
			set_inspec_source (pp->p_config, SCAN_ENCODER1);
		/*if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
		  draw3_pop_tt (data_712, NULL,
		  menu_content[I_INDEX + get_inspec_index (pp->p_config)],
		  menu_content + I_INDEX, 5, 2, get_inspec_index (pp->p_config), 0);
		  else */
		if(get_inspec_source (pp->p_config)==1)
			draw3_popdown (menu_content[I_INDEX + 2], 2, 0);
		else
			draw3_popdown (menu_content[I_INDEX + 1], 2, 0);

		gtk_widget_set_sensitive(pp->eventbox30[2],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[2],FALSE);
	}
	else
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
		{
			if(get_inspec_source (pp->p_config)==1)
				menu_status = 0x12;
			else if(get_inspec_source (pp->p_config)==2)
				menu_status = 0x14;
			else
				menu_status = 0x10;

			draw3_pop_tt (data_712, NULL,
					menu_content[I_INDEX + get_inspec_index (pp->p_config)],
					menu_content + I_INDEX, 5, 2, get_inspec_index (pp->p_config), menu_status);
		}
		else
			draw3_popdown (menu_content[I_INDEX + get_inspec_index (pp->p_config)], 2, 0);
	}
}
void DrawMenu722()
{
	double tmpf = 0.0;
	double cur_value=0.0, lower=0, upper=0, step=0 ;
	int digit = 0, pos, unit = 0;

	/* 当前步进 */
	switch (TMP(scan_resolution_reg))
	{
		case 0:	tmpf = 0.1; break;
		case 1:	tmpf = 0.5; break;
		case 2:	tmpf = 1.0; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
	{
		lower = get_enc_resolution (pp->p_config, get_cur_encoder (pp->p_config))/1000.0;
		lower = 2 / lower ;
		lower = MAX(0.08 , lower);
		upper = 100.0;
		cur_value = get_area_scanresolution (pp->p_config)/1000.0;
		step = tmpf;
		pos = 2;
		if(get_unit(pp->p_config) == UNIT_MM)
		{
			digit = 2;
			unit = UNIT_MM;
		}
		else
		{
			cur_value *= 0.03937;
			lower *= 0.03937 ;
			upper *= 0.03937;
			step   = tmpf/10.0;
			digit  = 3;
			unit = UNIT_INCH;
		}
		draw3_digit_pressed (data_722, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 0);
	}
	else
	{
		if(get_unit(pp->p_config) == UNIT_MM)
		{
			cur_value = get_area_scanresolution (pp->p_config)/1000.0;
			digit = 2;
			pos = 2;
			unit = UNIT_MM;
		}
		else
		{
			cur_value = get_area_scanresolution (pp->p_config)/1000.0*0.03937;
			digit = 3;
			pos = 2;
			unit = UNIT_INCH;
		}
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
	}
}
void DrawMenu732()
{
	if (get_start_pause (pp->p_config)==0)
		draw3_popdown(NULL,2,1);
	else
	{
		draw3_popdown(NULL,2,1);
		gtk_widget_set_sensitive (pp->eventbox30[2],FALSE);
		gtk_widget_set_sensitive (pp->eventbox31[2],FALSE);
	}
}
void DrawMenu742()
{
	if ( !con2_p[7][4][2] )
		gtk_widget_hide (pp->eventbox30[2]);
	gtk_widget_hide (pp->eventbox31[2]);
}

extern GtkWidget* saveSetupAsNew(GtkWidget* fatherWidget);
void DrawMenu802()
{
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
	{
		//	draw_dialog_all (DIALOG_SAVE_SETUP_AS);
		GtkWidget* dialog = saveSetupAsNew(pp->window);
		gtk_widget_show(dialog);
	}
	else
		draw3_popdown(NULL,2,1);
}
void DrawMenu812()
{
	pp->x_pos = 605, pp->y_pos = 287-26;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
		draw3_pop_tt (data_812, NULL,
				menu_content[PAPER_SIZE+get_report_paper_size (pp->p_config)],
				menu_content+PAPER_SIZE, 4, 2,get_report_paper_size (pp->p_config), 0x0b);
	else
		draw3_popdown (menu_content[PAPER_SIZE+get_report_paper_size (pp->p_config)], 2, 0);
}

void DrawMenu822()
{
	draw3_popdown (menu_content[OFF_ON + get_report_format_setup (pp->p_config)], 2, 0);
}
void DrawMenu832()
{
	unsigned char userFieldSelect = get_report_userfield_select(pp->p_config);
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
	{
	//	draw_dialog_all (DIALOG_USERFIELD_LABEL);
		char* str = NULL;
		str = dialogGetAString( pp->window
							    ,con2_p[8][3][2]
		                        ,get_report_userfield_label(pp->p_config ,userFieldSelect));
		if(str)
		{
			set_report_userfield_label(pp->p_config ,str ,userFieldSelect);
			free(str);
		}
		draw3_popdown(get_report_userfield_label(pp->p_config ,userFieldSelect) ,2 ,0);
	}
	else
	{
		draw3_popdown(get_report_userfield_label(pp->p_config ,userFieldSelect) ,2 ,0);
	}
}
void DrawMenu842()
{
	if ( !con2_p[8][4][2] )
		gtk_widget_hide (pp->eventbox30[2]);
	gtk_widget_hide (pp->eventbox31[2]);
}

void DrawMenu902()
{
    static gchar *menuContent[3] = {
        "1~6 MHz",
        "3~12 MHz",
        "0.5~15 MHz"
    };

    pp->x_pos = 470;
    pp->y_pos = 288-26;
    if (pp->pos_pos == MENU3_PRESSED && CUR_POS == 2) {
        draw3_pop_tt( data_902, NULL,
                      menuContent[0],
                      menuContent+0, 3, 2, 0, 0);
    } else {
        draw3_popdown(menuContent[0], 2, 0);
    }
}
void DrawMenu912()
{
#if HIGH_POWER
	draw3_popdown (menu_content[LANGUAGE+get_language (pp->p_config)], 2, 0);
#else
    pp->x_pos = 605, pp->y_pos = 287-26;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2))
		draw3_pop_tt (data_912, NULL,
				menu_content[LANGUAGE+get_language (pp->p_config)],
				menu_content+LANGUAGE, 2, 2,get_language (pp->p_config), 0);
	else
		draw3_popdown (menu_content[LANGUAGE+get_language (pp->p_config)], 2, 0);
#endif
}
void DrawMenu922()
{
//	if ( !con2_p[9][2][2] )
//		gtk_widget_hide (pp->eventbox30[2]);
//	gtk_widget_hide (pp->eventbox31[2]);
    pp->x_pos = 590, pp->y_pos = 287-26;
    if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 2)) {
        draw3_pop_tt(data_922, NULL, NULL, menu_content+CERT_IMPORT, 2, 2, 0, 2);
    } else {
        draw3_popdown(NULL, 2, 1);
    }

}
void DrawMenu932()
{
	if ( !con2_p[9][3][2] )
		gtk_widget_hide (pp->eventbox30[2]);
	gtk_widget_hide (pp->eventbox31[2]);
}
void DrawMenu942()
{
	if ( !con2_p[9][4][2] )
		gtk_widget_hide (pp->eventbox30[2]);
	gtk_widget_hide (pp->eventbox31[2]);
}

/*
 * drawmenu4.c
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
#include <glib/gprintf.h>
#include "menu.h"
#include "../../string/_string.h"


void DrawSampleStart()
{
	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit;
	int grp = get_current_group (pp->p_config);
	double _nStart    = GROUP_VAL_POS(grp , start) / 1000.0  ;
	double _nRange    = GROUP_VAL_POS(grp , range) / 1000.0  ;
	double _nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0 ;
	int _nPointQty = GROUP_VAL_POS(grp , point_qty)     ;
	double _nCosCurrentAngle = cos(TMP(current_angle[grp]));
	double _nLimitRange    =  PrfLimitToGroupSampleRange(grp)/ 100.0 ;

	switch (TMP(start_reg))
	{
		case 0:	tmpf = _nRange / _nPointQty ; break;
		case 1:	tmpf = _nRange / 20.0 ; break;
		case 2:	tmpf = _nRange / 10.0 ; break;
		default:break;
	}
	pos = 4 ;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
	{
		if(UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit))
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
				cur_value = _nCosCurrentAngle * _nStart * _nVelocity / 2000.0 ;   /* 当前显示的范围数值mm */
				lower = 0;
				upper =	_nCosCurrentAngle * ( _nLimitRange - _nRange ) * (_nVelocity / 2000.0);
				step = _nCosCurrentAngle * tmpf * (_nVelocity / 2000.0);
				if (step < 0.01) step = 0.01 ;
				digit = 2;
				unit = UNIT_MM;
			}
			else
			{
				cur_value =0.03937 * _nCosCurrentAngle * _nStart * _nVelocity / 2000.0 ;   /* 当前显示的范围数值mm */
				lower = 0;
				upper =	0.03937 * _nCosCurrentAngle * ( _nLimitRange - _nRange ) * (_nVelocity / 2000.0);
				step  = 0.03937 * _nCosCurrentAngle * tmpf * (_nVelocity / 2000.0);
				if (step < 0.01) step = 0.01 ;
				digit = 3;
				unit = UNIT_INCH;
			}
		}
		else if  (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit))
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
				cur_value =  _nStart * _nVelocity / 2000.0 ;   /* 当前显示的范围数值mm */
				lower = 0;
				upper =	 ( _nLimitRange - _nRange ) * (_nVelocity / 2000.0);
				step  =  tmpf * (_nVelocity / 2000.0);
				if (step < 0.01) step = 0.01 ;
				digit = 2;
				unit = UNIT_MM;
			}
			else
			{
				cur_value =0.03937 * _nStart * _nVelocity / 2000.0 ;   /* 当前显示的范围数值mm */
				lower = 0;
				upper =	0.03937 * ( _nLimitRange - _nRange ) * (_nVelocity / 2000.0);
				step  = 0.03937 * tmpf * (_nVelocity / 2000.0);
				if (step < 0.01) step = 0.01 ;
				digit = 3;
				unit = UNIT_INCH;
			}
		}
		else
		{
			cur_value = _nStart ;
			lower =	0;
			upper =	_nLimitRange - _nRange;
			step = tmpf;
			if (step < 0.01)  (step = 0.01) ;
			digit = 2;
			unit = UNIT_US;
		}
		draw3_digit_pressed (data_101, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 10);
	}
	else
	{
		if(UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit))
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
				cur_value = _nCosCurrentAngle * _nStart * _nVelocity / 2000.0 ;
				unit = UNIT_MM;
				digit = 2;
			}
			else
			{
				cur_value =0.03937 * _nCosCurrentAngle * _nStart * _nVelocity / 2000.0 ;
				unit = UNIT_INCH;
				digit = 3;
			}
		}
		else if(UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit))
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
				cur_value =  _nStart * _nVelocity / 2000.0 ;
				unit = UNIT_MM;
				digit = 2;
			}
			else
			{
				cur_value =0.03937 * _nStart * _nVelocity / 2000.0 ;
				unit = UNIT_INCH;
				digit = 3;
			}
		}
		else
		{
			cur_value = _nStart / 1000.0 ;
			unit = UNIT_US;
			digit = 2;
		}
		draw3_digit_stop (cur_value , units[unit], digit, pos, 10);
	}
}

void DrawGateAHeight()
{
	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit;
	int grp = get_current_group (pp->p_config);

	switch (TMP(cheight_reg))
	{
		case 0:	tmpf = 1.0; break;
		case 1:	tmpf = 10.0; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
	{
		cur_value = GROUP_VAL_POS( grp , gate[0].height);
		lower = 0.0;
		upper = 98.0;
		step = tmpf;
		digit = 0;
		pos = 4;
		unit = UNIT_BFH;
		draw3_digit_pressed (data_204, units[unit], cur_value,
				lower, upper, step, digit, NULL, pos, 21);
	}
	else
	{
		cur_value =GROUP_VAL_POS( grp , gate[0].height);
		digit = 0;
		pos = 4;
		unit = UNIT_BFH;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 21);
	}
}

void DrawMenu004()
{

}

void DrawMenu014()
{

}


void DrawMenu024Encoder()
{
	gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu024Step1()
{
	//else if ((pp->ctype_pos == 1)&&(pp->cmode_pos == 3))
	//{
	draw3_popdown_offset(NULL, 4,1,5);
	//}
	//gtk_widget_hide (pp->eventbox30[4]);
	//gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu024UltrasoundVelocityStep2()
{
    DrawSampleStart();
}
void DrawMenu024UltrasoundVelocityStep3()
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
		{
			cur_value = pp->radius1 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 4;
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
			pos = 4;
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
		{
			cur_value = pp->depth1 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 4;
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
			pos = 4;
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
		{
			cur_value = pp->thickness1 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 4;
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
			pos = 4;
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

void DrawMenu024UltrasoundVelocityStep4()
{
	DrawGateAHeight() ;
}
void DrawMenu024UltrasoundVelocityStep5()
{
	DrawGateAHeight();
}

void DrawMenu024UltrasoundVelocityStep6()
{
	gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu024UltrasoundWedgeDelayStep2()
{
	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit;
	int grp = get_current_group (pp->p_config);

	switch(TMP(tolerance_reg))
	{
		case 0:	tmpf = 1; break;
		case 1:	tmpf = 10; break;
		case 2:	tmpf = 0.1; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
	{
		cur_value = pp->tolerance / 100.0 ;
		lower = 0.0;
		upper = 1000.0;
		step = tmpf;
		digit = 2;
		pos = 4;
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
		draw3_digit_pressed (data_0243, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 26);
	}
	else
	{
		cur_value = pp->tolerance / 100.0 ;
		digit = 2;
		pos = 4;
		unit = UNIT_MM;
		if(get_unit(pp->p_config) == UNIT_INCH)
		{
			cur_value *= 0.03937;
			digit = 3;
			unit = UNIT_INCH;
		}
		draw3_digit_stop (cur_value, units[unit], digit, pos, 26);
	}
	if(PA_SCAN != GROUP_VAL_POS(grp , group_mode))
	{
		gtk_widget_hide (pp->eventbox30[4]);
		gtk_widget_hide (pp->eventbox31[4]);
	}
}

void DrawMenu024UltrasoundWedgeDelayStep3()
{
	gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu024UltrasoundWedgeDelayStep4()
{
	DrawGateAHeight() ;
}

void DrawMenu024UltrasoundWedgeDelayStep5()
{
	int grp = get_current_group(pp->p_config);
	if(GROUP_VAL_POS(grp , group_mode) == PA_SCAN)
	{
		draw3_popdown_offset(NULL, 4,1,30);
	}
	else
	{
		gtk_widget_hide (pp->eventbox30[4]);
		gtk_widget_hide (pp->eventbox31[4]);
	}
}

void DrawMenu024UltrasoundWedgeDelayStep6()
{
	gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu024UltrasoundSensationStep2()
{
	gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu024UltrasoundSensationStep3()
{
	gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu024UltrasoundSensationStep4()
{
	DrawGateAHeight() ;
}

void DrawMenu024UltrasoundSensationStep5()
{
	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit;
	int grp = get_current_group(pp->p_config);
    int _nGain = group_get_gain(grp);
    int _nGainR= group_get_gainrf(grp);

	switch (TMP(db_reg))
	{
		case 0:	tmpf = 0.1; break;
		case 1:	tmpf = 0.5; break;
		case 2:	tmpf = 1.0; break;
		case 3:	tmpf = 2.0; break;
		case 4:	tmpf = 6.0; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
	{
		cur_value = (_nGain - _nGainR * get_group_db_ref (pp->p_config, grp)) / 100.0;
		lower = 0.0 - _nGainR * get_group_db_ref (pp->p_config, grp) / 100.0;
		upper = GAIN_MAX - _nGainR * get_group_db_ref (pp->p_config, grp) / 100.0;
		step = tmpf;
		digit = 1;
		pos = 4;
		unit = UNIT_DB;
		draw3_digit_pressed (data_100, units[unit], cur_value ,
				lower, upper, step, digit, NULL, pos, 9);
	}
	else
	{
		cur_value = (_nGain - _nGainR * get_group_db_ref (pp->p_config, grp)) / 100.0;
		digit = 1;
		pos = 4;
		unit = UNIT_DB;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 9);
	}
}

void DrawMenu024UltrasoundSensationStep6()
{
	draw3_popdown_offset(NULL, 4,1, 30);
}

void DrawMenu024UltrasoundUtWedgedelayAndVelocityStep2()
{
	DrawSampleStart();
}

void DrawMenu024UltrasoundUtWedgedelayAndVelocityStep3()
{
	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit;

	if(pp->echotype_pos == 0)
	{
		switch (TMP(clb_reg))
		{
			case 0:	tmpf = 1; break;
			case 1:	tmpf = 10; break;
			case 2:	tmpf = 0.1; break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
		{
			cur_value = pp->radius2 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 4;
			unit = UNIT_MM;
			draw3_digit_pressed (data_024, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 14);
		}
		else
		{
			cur_value = pp->radius2 / 1000.0;
			digit = 2;
			pos = 4;
			unit = UNIT_MM;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 14);
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
		{
			cur_value = pp->depth2 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 4;
			unit = UNIT_MM;
			draw3_digit_pressed (data_0241, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 16);
		}
		else
		{
			cur_value = pp->depth2 / 1000.0;
			digit = 2;
			pos = 4;
			unit = UNIT_MM;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 16);
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
		{
			cur_value = pp->thickness2 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 4;
			unit = UNIT_MM;
			draw3_digit_pressed (data_0242, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 18);
		}
		else
		{
			cur_value = pp->thickness2 / 1000.0;
			digit = 2;
			pos = 4;
			unit = UNIT_MM;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 18);
		}
	}
}

void DrawMenu024UltrasoundUtWedgedelayAndVelocityStep4()
{
	DrawGateAHeight();
}

void DrawMenu024UltrasoundUtWedgedelayAndVelocityStep5()
{
	DrawGateAHeight();
}

void DrawMenu024UltrasoundUtWedgedelayAndVelocityStep6()
{
	draw3_popdown_offset(NULL, 4,1,31);
}

void DrawMenu024CodeAWSStep2()
{
	gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}
void DrawMenu024CodeAWSStep3()
{
	DrawGateAHeight();
}
void DrawMenu024CodeAWSStep4()
{
	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit;
	int grp = get_current_group(pp->p_config);
	switch (TMP(clb_reg))
	{
		case 0:	tmpf = 1; break;
		case 1:	tmpf = 10; break;
		case 2:	tmpf = 0.1; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
	{
		cur_value = GROUP_VAL_POS(grp , part.Thickness) / 1000;
		lower = 0.0;
		upper = 1000.0;
		step = tmpf;
		digit = 2;
		pos = 4;
		unit = UNIT_MM;
		if(get_unit(pp->p_config) == UNIT_INCH)
		{
			cur_value *= 0.03937 ;
			upper *= 0.03937 ;
			step  *= 0.03937 ;
			unit = UNIT_INCH;
		}
		draw3_digit_pressed (data_531, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 39);
	}
	else
	{
		cur_value = GROUP_VAL_POS(grp , part.Thickness) / 1000.0;
		digit = 2;
		pos = 4;
		unit = UNIT_MM;
		if(get_unit(pp->p_config) == UNIT_INCH)
		{
			cur_value *= 0.03937 ;
			unit = UNIT_INCH;
		}
		draw3_digit_stop (cur_value, units[unit], digit, pos, 39);
	}
}
void DrawMenu024CodeAWSStep5()
{
	gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}
void DrawMenu024CodeAWSStep6()
{
	gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}


void DrawMenu024UltrasoundVelocity()
{
	switch(pp->cstart_qty)
	{
	case 0:
	case 1:
		DrawMenu024Step1();
		break;
	case 2:
		DrawMenu024UltrasoundVelocityStep2();
		break;
	case 3:
		DrawMenu024UltrasoundVelocityStep3();
		break;
	case 4:
		DrawMenu024UltrasoundVelocityStep4();
		break;
	case 5:
		DrawMenu024UltrasoundVelocityStep5();
		break;
	case 6:
		DrawMenu024UltrasoundVelocityStep6();
		break;
	default:
		break;

	}
}

void DrawMenu024UltrasoundWedgeDelay()
{
	switch(pp->cstart_qty)
	{
	case 0:
	case 1:
		DrawMenu024Step1();
		break;
	case 2:
		DrawMenu024UltrasoundWedgeDelayStep2();
		break;
	case 3:
		DrawMenu024UltrasoundWedgeDelayStep3();
		break;
	case 4:
		DrawMenu024UltrasoundWedgeDelayStep4();
		break;
	case 5:
		DrawMenu024UltrasoundWedgeDelayStep5();
		break;
	case 6:
		DrawMenu024UltrasoundWedgeDelayStep6();
		break;
	default:
		break;

	}
}

void DrawMenu024UltrasoundSensation()
{
	int _nGroupId = get_current_group(pp->p_config);
	if(PA_SCAN == GROUP_VAL_POS(_nGroupId , group_mode))
	{
		switch(pp->cstart_qty)
		{
		case 0:
		case 1:
		case 2:
			DrawMenu024Step1();
			break;
		case 3:
			DrawMenu024UltrasoundSensationStep3();
			break;
		case 4:
			DrawMenu024UltrasoundSensationStep4();
			break;
		case 5:
			DrawMenu024UltrasoundSensationStep5();
			break;
		case 6:
			DrawMenu024UltrasoundSensationStep6();
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
			DrawMenu024Step1();
			break;
		case 2:
			DrawMenu024UltrasoundUtWedgedelayAndVelocityStep2();
			break;
		case 3:
			DrawMenu024UltrasoundUtWedgedelayAndVelocityStep3();
			break;
		case 4:
			DrawMenu024UltrasoundUtWedgedelayAndVelocityStep4();
			break;
		case 5:
			DrawMenu024UltrasoundUtWedgedelayAndVelocityStep5();
			break;
		case 6:
			DrawMenu024UltrasoundUtWedgedelayAndVelocityStep6();
			break;
		default:
			break;
		}
	}
}


void DrawMenu024UltrasoundTCGStep4()
{
	DrawGateAHeight() ;
}

void DrawMenu024UltrasoundTCGStep6()
{
	//draw3_popdown_offset(NULL, 4, 1 , 59);
	int grp = get_current_group(pp->p_config);
	if(TMP_CBA(bNewPoint))
	{
		draw3_popdown_offset(NULL, 4, 1 , 31);
	}
	else
	{
		if(GROUP_VAL_POS(grp , SizingCurves.point_pos) >= (GROUP_VAL_POS(grp , SizingCurves.dac_point_qty) - 1))
		{
			draw3_popdown_offset(NULL, 4, 1 , 60);
		}
		else
		{
			draw3_popdown_offset(NULL, 4, 1 , 59);
		}

		if(GROUP_VAL_POS(get_current_group(pp->p_config) , SizingCurves.dac_point_qty) == 16)
		{
			gtk_widget_set_sensitive(pp->eventbox30[4],FALSE);
			gtk_widget_set_sensitive(pp->eventbox31[4],FALSE);
		}

	}
}

void CalibrationFirstLawEaglet()
{
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos = 4, unit = 0;
	int grp = get_current_group (pp->p_config);
	cur_value = TMP_CBA(clb_start) / 100.0 ;
	if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
	{
		if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 4))
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
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

void DrawMenu024UltrasoundTCG()
{
	switch(pp->cstart_qty)
	{
	case 0:
	case 1:
	    draw3_popdown_offset(NULL, 4,1,5);
		break;
	case 2:
        CalibrationFirstLawEaglet();
		break;
	case 3:
		DrawMenu024UltrasoundTCGStep4();
		break;
	case 4:
		gtk_widget_hide (pp->eventbox30[4]);
		gtk_widget_hide (pp->eventbox31[4]);
		break;
	case 5:
		draw3_popdown_offset (NULL, 4, 1, 62 );
		break;
	case 6:
		//DrawMenu024UltrasoundTCGStep6();
		break;
	default:
		break;

	}
}


void DrawMenu024TOFDStep3()
{
	DrawSampleStart();
}

void DrawMenu024TOFDStep2()
{
	gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu024TOFDStep4()
{
	gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu024TOFDStep5()
{
	DrawGateAHeight();
}

void DrawMenu024TOFDStep6()
{
	double cur_value=0.0;
	int digit = 0, pos, unit = 0;
	int grp = get_current_group(pp->p_config);
	if (get_unit(pp->p_config) == 0)
	{
		cur_value = GROUP_VAL_POS(grp , field_distance[253]);
		digit = 1;
		unit = UNIT_MM;
	}
	else
	{
		cur_value = GROUP_VAL_POS(grp , field_distance[253]) *  0.03937;
		digit = 4;
		unit = UNIT_INCH;
	}
	pos = 4;
	draw3_digit_stop (cur_value, units[unit], digit, pos, 55);
	gtk_widget_set_sensitive(pp->eventbox30[4],FALSE);
	gtk_widget_set_sensitive(pp->eventbox31[4],FALSE);
}

void DrawMenu024TOFD()
{
	switch(pp->cstart_qty)
	{
	case 0:
	case 1:
		DrawMenu024Step1();
		break;
	case 2:
		DrawMenu024TOFDStep2();
		break;
	case 3:
		DrawMenu024TOFDStep3();
		break;
	case 4:
		DrawMenu024TOFDStep4();
		break;
	case 5:
		//DrawMenu024TOFDStep5();
		DrawMenu024TOFDStep6() ;
		break;
	case 6:
		//DrawMenu024TOFDStep6();
		break;
	default:
		break;
	}
}

void DrawMenu024Ultrasound()
{
	int grp = get_current_group(pp->p_config);
	if(GROUP_VAL_POS(grp , tx_rxmode1) == TOFD)
	{
		DrawMenu024TOFD();
	}
	else
	{
		switch(pp->cmode_pos)
		{
		case 0:
			DrawMenu024UltrasoundVelocity();
			break;
		case 1:
			DrawMenu024UltrasoundWedgeDelay();
			break;
		case 2:
			DrawMenu024UltrasoundSensation();
			break;
		case 3:
			DrawMenu024UltrasoundTCG();
			break;
		default:
			break;
		}
	}
}

void DrawMenu024CodeAPI()
{
	gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu024CodeAWS()
{
	switch(pp->cstart_qty)
	{
	case 0:
	case 1:
		DrawMenu024Step1();
		break;
	case 2:
		DrawMenu024CodeAWSStep2();
		break;
	case 3:
		DrawMenu024CodeAWSStep3();
		break;
	case 4:
		DrawMenu024CodeAWSStep4();
		break;
	case 5:
		DrawMenu024CodeAWSStep5();
		break;
	case 6:
		DrawMenu024CodeAWSStep6();
		break;
	default:
		break;
	}
}

void DrawMenu024Code()
{
	switch(pp->scode_pos)
	{
	case 0:
		DrawMenu024CodeAPI();
		break;
	case 1:
		DrawMenu024CodeAWS();
		break;
	default:
		break;
	}
}

void DrawMenu024()
{
	switch (pp->ctype_pos)
	{
	case 0:// encoder calibration
		DrawMenu024Encoder();
		break;
	case 1:
		DrawMenu024Ultrasound();
		break;
	case 2:
		DrawMenu024Code();
		break;
	default:
		break;
	}
}

void DrawMenu034()
{
	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit;
	int grp = get_current_group(pp->p_config);
	if(((get_part_weld(grp)==3)&&(pp->wstart_qty == 4)) || ((get_part_weld(grp)==5)&&(pp->wstart_qty == 4)))
	{
		/* 当前步进 */
		switch (pp->p_tmp_config->fizone_radius_reg)
		{
			case 0:	tmpf = 0.1; break;
			case 1:	tmpf = 1.0; break;
			case 2:	tmpf = 10.0; break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
		{

			cur_value = get_fizone_radius(grp) / 1000.0;
			lower = 0;
			upper = 89.9;
			step = tmpf;
			digit = 2;
			pos = 4;
			unit = UNIT_MM;
			draw3_digit_pressed (data_034, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 12);
		}
		else
		{
			cur_value = get_fizone_radius(grp) / 1000.0;
			digit = 2;
			pos = 4;
			unit = UNIT_MM;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 12);
		}
	}
        else if((get_part_weld(grp)==4)&&(pp->wstart_qty == 4))//xrz
        {
                switch (pp->p_tmp_config->fizone_radius_reg)
                {
                        case 0: tmpf = 0.1; break;
                        case 1: tmpf = 1.0; break;
                        case 2: tmpf = 10.0; break;
                        default:break;
                }
                if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
                {

                        cur_value = get_fizone_radius(grp) / 1000.0;
                        lower = 0;
                        upper = 89.9;
                        step = tmpf;
                        digit = 2;
                        pos = 4;
                        unit = UNIT_DEG;
                        draw3_digit_pressed (data_034, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 17);
                }
                else
                {
                        cur_value = get_fizone_radius(grp) / 1000.0;
                        digit = 2;
                        pos = 4;
                        unit = UNIT_DEG;
                        draw3_digit_stop (cur_value, units[unit], digit, pos, 17);
                }
        }
	else if((get_part_weld(grp) == 2) && (pp->wstart_qty == 3))
	{
		switch (pp->p_tmp_config->fizone_angle_reg)
		{						
				case 0: tmpf = 0.1; break;
				case 1: tmpf = 1.0; break;
				case 2: tmpf = 10.0; break;
				default:break;
		}				
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
		{
		
				cur_value = get_fizone_angle(grp) / 1000.0;
				lower = 0;
				upper = 89.9;
				step = tmpf;
				digit = 2;		
				pos = 4;
				unit = UNIT_DEG;
				draw3_digit_pressed (data_0332, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 18);
		}
		else
		{
				cur_value = get_fizone_angle(grp) / 1000.0;
				digit = 2;
				pos = 4;
				unit = UNIT_DEG;
				draw3_digit_stop (cur_value, units[unit], digit, pos, 18);
		}

	}

	else
	{
		gtk_widget_hide (pp->eventbox30[4]);
		gtk_widget_hide (pp->eventbox31[4]);
	}
}

void DrawMenu044()
{
	if ( !con2_p[0][4][4] )
		gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu104()
{
	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit;
	int grp = get_current_group (pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);


	switch (pp->p_tmp_config->velocity_reg)
	{
		//case 0:	tmpf = 0.1; break;
	    case 0: tmpf = 1000 ; break;
		case 1:	tmpf = 1.0;   break;
		case 2:	tmpf = 10.0;  break;
		case 3:	tmpf = 100.0; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
	{
		if (get_unit(pp->p_config) == 0)
		{
			cur_value = get_group_val (p_grp, GROUP_VELOCITY) / 100.0;
			lower = 635.0;
			upper = 15240.0;
			step = tmpf;
			digit = 1;
			unit = UNIT_M_S;
		}
		else
		{
			cur_value = get_group_val (p_grp, GROUP_VELOCITY) / (100.0 * 25400 );
			lower = 0.025;
			upper = 0.6;
			step = tmpf / 1000.0;
			digit = 4;
			unit = UNIT_IN_US;
		}
		pos = 4;
		draw3_digit_pressed (data_104, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 0);
	}
	else
	{
		if (get_unit(pp->p_config) == 0)
		{
			cur_value = get_group_val (p_grp, GROUP_VELOCITY) / 100.0;
			digit = 1;
			unit = UNIT_M_S;
		}
		else
		{
			cur_value = get_group_val (p_grp, GROUP_VELOCITY) * 0.000000394;
			digit = 4;
			unit = UNIT_IN_US;
		}
		pos = 4;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
	}
}

void DrawMenu114()
{
	char* str ;
	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit;
	int grp = get_current_group (pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);

	pp->x_pos = 588, pp->y_pos = 510 - 26;
	switch (TMP(pulser_width_reg))
	{
		case 0:	tmpf = 2.5; break;
		case 1:	tmpf = 10.0; break;
		case 2:	tmpf = 25.0; break;
		default:break;
	}
	if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 4))
	{
		if (pp->mark_pop_change)
		{
			cur_value = get_group_val (p_grp, GROUP_PW_VAL) / (double)PW_DIV;
#if HIGH_POWER
			lower =	200.0;
			upper =	2350.0;
#else
			lower =	30.0;
			upper =	500.0;
#endif
			step = tmpf;
			digit = 1;
			pos = 4;
			unit = UNIT_NONE;

			draw3_digit_pressed (data_1141, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 0);
		}
		else
		{
			if (!get_group_val (p_grp, GROUP_PW_POS))
			{
				/* 更新当前增益值显示 */
				str = g_strdup_printf ("%s %0.1f",
						menu_content[PULSER_WIDTH + get_group_val (p_grp, GROUP_PW_POS)], get_group_val (p_grp, GROUP_PW_VAL) / (double)PW_DIV);
				draw3_pop_tt (data_114, NULL,
						str, menu_content + PULSER_WIDTH, 2, 4, get_group_val (p_grp, GROUP_PW_POS), 0);
				g_free(str);
			}
			else
			{
				str = g_strdup_printf ("%0.1f", get_group_val (p_grp, GROUP_PW_VAL) / (double)PW_DIV);
				draw3_pop_tt (data_114, NULL,
						str, menu_content + PULSER_WIDTH, 2, 4, get_group_val (p_grp, GROUP_PW_POS), 0);
				g_free(str);
			}
		}
	}
	else
	{
		/* 这个选中Auto 时候显示 Auto + 数值 */
		if (get_group_val (p_grp, GROUP_PW_POS))
		{
			/* Auto 时候计算脉冲宽度 */
			str = g_strdup_printf ("%s %0.1f",
					menu_content[PULSER_WIDTH + get_group_val (p_grp, GROUP_PW_POS)], get_group_val (p_grp, GROUP_PW_VAL) / (double)PW_DIV);
			draw3_popdown (str, 4, 0);
			g_free(str);
		}
		else
		{
			cur_value = GROUP_VAL_POS(grp , pulser_width1) / (double)PW_DIV;
			unit = UNIT_NULL;
			pos = 4;
			digit = 1;
			draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
		}
	}
}

void DrawMenu124()
{
	pp->x_pos = 627, pp->y_pos = 458-26;
	if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 4))
		draw3_pop_tt (data_124, NULL,
				menu_content[AVERAGING + get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_AVERAGING)],
				menu_content + AVERAGING, 5, 4, get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_AVERAGING), 0);
	else
		draw3_popdown (menu_content[AVERAGING + get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_AVERAGING)], 4, 0);

}

void DrawMenu134()
{
	double tmpf = 0.0;
	int offset;
	double cur_value, lower, upper, step;
	int digit, pos, unit;
	int grp = get_current_group (pp->p_config);

	offset = TMP(beam_num[grp]) ;

	if (GROUP_VAL_POS( grp , group_mode) != PA_SCAN)
	{
		gtk_widget_hide (pp->eventbox30[4]);
		gtk_widget_hide (pp->eventbox31[4]);
	}
	else if (GROUP_VAL_POS( grp , group_mode) == PA_SCAN)
	{
		switch (TMP(beam_delay_reg))
		{
			case 0:	tmpf = 1; break;
			case 1:	tmpf = 10; break;
			case 2:	tmpf = 0.1; break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
		{
			cur_value = GROUP_VAL_POS(grp , beam_delay[offset])/1000.0;
			lower = 0.0;
			upper = 100.0;
			step = tmpf;
			digit = 2;
			pos = 4;
			unit = UNIT_US;
			draw3_digit_pressed (data_134, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 0);
		}
		else
		{
			cur_value = GROUP_VAL_POS(grp , beam_delay[offset])/1000.0;
			digit = 2;
			pos = 4;
			unit = UNIT_US;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		}
	}
}

void DrawMenu144()
{
	double cur_value ;
	int digit, pos, unit;
	int grp = get_current_group (pp->p_config);

	gtk_widget_set_sensitive (pp->eventbox30[4], FALSE);
	gtk_widget_set_sensitive (pp->eventbox31[4], FALSE);

	cur_value = (GROUP_VAL_POS(grp , range)/ 10) / GROUP_VAL_POS( grp , point_qty);
	digit = 0;
	pos = 4;
	unit = UNIT_NULL;
	draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
}

void DrawMenu204()
{
	double tmpf = 0.0;
	char* str = NULL;
	double cur_value, lower, upper, step;
	int digit, pos, unit;

	pp->x_pos = 590, pp->y_pos = 470;
	if (GROUP_GATE_POS(parameters) == 0)
	{
		switch (TMP(agate_height_reg))
		{
			case 0:	tmpf = 1.0; break;
			case 1:	tmpf = 10.0; break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
		{
			cur_value = GROUP_GATE_POS(height);
			lower = 0.0;
			upper = 98.0;
			step = tmpf;
			digit = 0;
			pos = 4;
			unit = UNIT_BFH;
			draw3_digit_pressed (data_204, units[unit], cur_value,
					lower, upper, step, digit, NULL, pos, 0);
		}
		else
		{
			cur_value = GROUP_GATE_POS(height);
			digit = 0;
			pos = 4;
			unit = UNIT_BFH;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		}
	}
	else
	{
		if (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RECTIFIER) != RF_WAVE)
		{
			gtk_widget_set_sensitive(pp->eventbox30[4],FALSE);
			gtk_widget_set_sensitive(pp->eventbox31[4],FALSE);
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
		{
			draw3_pop_tt (data_2041, NULL, menu_content[RECTIFIER_FREQ + GROUP_GATE_POS(rectifier_freq)],menu_content + RECTIFIER_FREQ, 3, 4, GROUP_GATE_POS(rectifier_freq), 0);
			str = g_strdup_printf ("%s", con2_p[2][0][8]);
			gtk_label_set_text (GTK_LABEL (pp->label3[4]), str);
		}
		else
		{
			draw3_popdown (menu_content[RECTIFIER_FREQ + GROUP_GATE_POS(rectifier_freq)], 4, 0);
			str = g_strdup_printf ("%s", con2_p[2][0][8]);
			gtk_label_set_text (GTK_LABEL (pp->label3[4]), str);
		}
		if (str) {
			g_free(str);
        }
	}
}

void DrawMenu214(){}
void DrawMenu224(){}

void DrawMenu234()
{
	double tmpf = 0.0;
	char *str;
	double cur_value, lower, upper, step;
	int digit, pos, unit;
	int grp = get_current_group (pp->p_config);

	if(GROUP_VAL_POS( grp , SizingCurves.mode_pos)==0)
	{
		if(GROUP_VAL_POS( grp , SizingCurves.curve_pos)==1 || GROUP_VAL_POS( grp , SizingCurves.curve_pos)==2)
		{
			/* 当前步进 */
			switch (TMP(curve_step_reg))
			{
				case 0:	tmpf = 0.1; break;
				case 1:	tmpf = 0.5; break;
				case 2:	tmpf = 1.0; break;
				default:break;
			}
			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
			{
				cur_value =GROUP_VAL_POS( grp , SizingCurves.curve_step)/100.0;
				lower = 0.0;
				upper = 12.0;
				step = tmpf;
				digit = 1;
				pos = 4;
				unit = UNIT_DB;
				draw3_digit_pressed (data_234, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 8);


			}
			else
			{
				cur_value = GROUP_VAL_POS( grp , SizingCurves.curve_step)/100.0;
				digit = 1;
				pos = 4;
				unit = UNIT_DB;
				draw3_digit_stop (cur_value, units[unit], digit, pos, 8);
			}

		}
		else
		{
			gtk_widget_hide (pp->eventbox30[4]);
			gtk_widget_hide (pp->eventbox31[4]);
			gtk_widget_hide (pp->sbutton[4]);

		}
	}
	else if(GROUP_VAL_POS( grp , SizingCurves.mode_pos)==1)
	{
		if(GROUP_VAL_POS( grp , SizingCurves.curve_pos)==1 || GROUP_VAL_POS( grp , SizingCurves.curve_pos)==3)
		{
			draw3_popdown(NULL,4,1);
			str = g_strdup_printf ("%s", con2_p[2][3][13]);
			gtk_label_set_text (GTK_LABEL (pp->label3[4]), str);
			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
				data_234_add_point(pp);
		}
		else
		{
			gtk_widget_hide (pp->eventbox30[4]);
			gtk_widget_hide (pp->eventbox31[4]);
			gtk_widget_hide (pp->sbutton[4]);
		}
	}
}

void DrawMenu244()
{
	if ( !con2_p[2][4][4] )
		gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu304()
{
	pp->x_pos = 200, pp->y_pos = 0;
	int feildNum = GetMesureFeildNum();
	const char* pSimpleStr[feildNum];
	const char* pStr[feildNum];
	GetMesureFeildString(pSimpleStr ,pStr);
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
		draw3_pop_tt_shen (data_304, NULL,
				pSimpleStr[get_reading_field3(pp->p_config)],
				pStr, feildNum, 4, get_reading_field3(pp->p_config), 0);
	else
		draw3_popdown (pSimpleStr[get_reading_field3(pp->p_config)], 4, 0);
}

void DrawMenu314()
{
	char temp[52];
	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit;
	int grp = get_current_group (pp->p_config);
	GROUP *p_grp = get_group_by_id (pp->p_config, grp);
	int  _nSelection = GROUP_VAL_POS(grp , selection)  ;

	if(_nSelection == 0 || _nSelection == 1)
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
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
					pos = 4;
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
					pos = 4;
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
				pos = 4;
				unit = UNIT_US;
			}
			draw3_digit_pressed (data_314, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 0);
		}
		else
		{
			if((UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit)) || (UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit)))
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					cur_value = GROUP_VAL_POS( grp , u_measure)/1000.0 * (GROUP_VAL_POS(grp , velocity)/200000.0);
					digit = 2;
					pos = 4;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = 0.03937 * GROUP_VAL_POS( grp , u_measure)/1000.0 * (GROUP_VAL_POS(grp , velocity)/200000.0);
					digit = 3;
					pos = 4;
					unit = UNIT_INCH;
				}
			}
			else	/*????*/
			{
				cur_value = GROUP_VAL_POS( grp , u_measure)/1000.0;
				digit = 2;
				pos = 4;
				unit = UNIT_US;
			}
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		}
	}
	else if(_nSelection == 2)
	{
		/* 当前步进 */
		switch (TMP(i_measure_reg))
		{
			case 0:	tmpf = 0.1; break;
			case 1:	tmpf = 0.5;  break;
			case 2:	tmpf = 1.0;  break;
			case 3:	tmpf = 10;  break;
			default:break;
		}
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
		{
			if(get_unit(pp->p_config) == UNIT_MM)
			{
				cur_value = GROUP_VAL_POS( grp , i_measure)/100.0;
				lower = -9999.0;
				upper =  9999.0;
				step = tmpf;
				digit = 1;
				pos = 4;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = GROUP_VAL_POS( grp , i_measure)/100.0*0.03937;
				lower = -9999.0*0.03937;
				upper = 9999.0*0.03937;
				step = tmpf;
				digit = 3;
				pos = 4;
				unit = UNIT_INCH;
			}
			draw3_digit_pressed (data_3141, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 10);
		}
		else
		{
			if(get_unit(pp->p_config) == UNIT_MM)
			{
				cur_value = GROUP_VAL_POS( grp , i_measure)/100.0;
				digit = 1;
				pos = 4;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = GROUP_VAL_POS( grp , i_measure)/100.0*0.03937;
				digit = 3;
				pos = 4;
				unit = UNIT_INCH;
			}
			draw3_digit_stop (cur_value, units[unit], digit, pos, 10);
		}
	}
	else if (_nSelection == 3)
	{
		/* 当前步进 */
		if(GROUP_VAL_POS(grp , skew_pos) == 1 || GROUP_VAL_POS(grp , skew_pos) == 3)
		{
			switch (TMP(i_reference_reg))
			{
				case 0:	tmpf = 0.1;  break;
				case 1:	tmpf = 0.5;  break;
				case 2:	tmpf = 1.0;  break;
				case 3:	tmpf =  10;  break;
				default:break;
			}
			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					cur_value = GROUP_VAL_POS( grp , i_reference)/100.0;
					lower = -9999.0;
					upper = 9999.0;
					step = tmpf;
					digit = 1;
					pos = 4;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = GROUP_VAL_POS( grp , i_reference)/100.0*0.03937;
					lower = -9999.0*0.03937;
					upper = 9999.0*0.03937;
					step = tmpf;
					digit = 3;
					pos = 4;
					unit = UNIT_INCH;
				}
				draw3_digit_pressed (data_3131, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 9);
			}
			else
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					cur_value = GROUP_VAL_POS( grp , i_reference)/100.0;
					digit = 1;
					pos = 4;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = GROUP_VAL_POS( grp , i_reference)/100.0*0.03937;
					digit = 3;
					pos = 4;
					unit = UNIT_INCH;
				}
				draw3_digit_stop (cur_value, units[unit], digit, pos, 9);
			}
		}
		else
		{
			switch (TMP(i_reference_reg))
			{
				case 0:	tmpf = 0.1; break;
				case 1:	tmpf = 0.5;  break;
				case 2:	tmpf = 1.0;  break;
				case 3:	tmpf = 10;  break;
				default:break;
			}
			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					cur_value = GROUP_VAL_POS( grp , s_reference)/100.0;
					lower = -9999.0;
					upper = 9999.0;
					step = tmpf;
					digit = 1;
					pos = 4;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = GROUP_VAL_POS( grp , s_reference)/100.0*0.03937;
					lower = -9999.0*0.03937;
					upper = 9999.0*0.03937;
					step = tmpf;
					digit = 3;
					pos = 4;
					unit = UNIT_INCH;
				}
				draw3_digit_pressed (data_3132, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 6);
			}
			else
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					cur_value = GROUP_VAL_POS( grp , s_reference)/100.0;
					digit = 1;
					pos = 4;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = GROUP_VAL_POS( grp , s_reference)/100.0*0.03937;
					digit = 3;
					pos = 4;
					unit = UNIT_INCH;
				}
				draw3_digit_stop (cur_value, units[unit], digit, pos, 6);
			}
		}

	}
	else if (_nSelection == 4|| _nSelection == 5||  _nSelection==6)
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
		{
			if(get_unit(pp->p_config) == UNIT_MM)
			{
				cur_value = TMP(scan)/100.0;
				lower = -100000.0;
				upper = 100000;
				step = tmpf;
				digit = 1;
				pos = 4;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = TMP(scan)/100.0*0.03937;
				lower = -100000.0*0.03937;
				upper = 100000*0.03937;
				step = tmpf;
				digit = 3;
				pos = 4;
				unit = UNIT_MM;
			}
			draw3_digit_pressed (data_3142, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 15);
		}
		else
		{
			if(get_unit(pp->p_config) == UNIT_MM)
			{
				cur_value = TMP(scan)/100.0;
				digit = 1;
				pos = 4;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = TMP(scan)/100.0*0.03937;
				digit = 3;
				pos = 4;
				unit = UNIT_MM;
			}
			draw3_digit_stop (cur_value, units[unit], digit, pos, 15);
		}
	}
	else if (_nSelection == 7)
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
			tableAddEntry();
		draw3_popdown(NULL,4,1);
		g_sprintf (temp,"\n\n%s", con2_p[3][1][17]);
		gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
		if(get_display_table(pp->p_config))
		{
			gtk_widget_set_sensitive(pp->eventbox30[4] ,FALSE);
			gtk_widget_set_sensitive(pp->eventbox31[4] ,FALSE);
		}
	}
	gtk_widget_queue_draw(pp->draw_area->drawing_area);

}
void DrawMenu324()
{
	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit;

	switch (pp->p_tmp_config->entry_reg)
	{
		case 0:	tmpf = 1.0; break;
		case 1:	tmpf = 10.0; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
	{
		cur_value = get_cur_entry (pp->p_config);
		lower = 1.0;
		upper = get_entry_qty (pp->p_config);
		step = tmpf;
		digit = 0;
		pos = 4;
		unit = UNIT_NONE;
		draw3_digit_pressed (data_324, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 0);
	}
	else
	{
		cur_value = get_cur_entry (pp->p_config);
		digit = 0;
		pos = 4;
		unit = UNIT_NONE;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
	}
	if(!get_display_table(pp->p_config))
	{
		gtk_widget_set_sensitive(pp->eventbox30[4] ,FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[4] ,FALSE);
	}
}
void DrawMenu334()
{
	if ( !con2_p[3][3][4] )
		gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu344()
{
	if ( !con2_p[3][4][4] )
		gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu404()
{
	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit;

	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))/*选中404这个位置*/
	{
		if(get_display_pos(pp->p_config)==Strip_Chart_AA)
			/*Display 为 Strip Chart-[A]*/
		{
			/* 当前步进 */
			switch (TMP(dis_range_reg))
			{
				case 0:	tmpf = 0.1; break;
				case 1:	tmpf = 1.0; break;
				case 2:	tmpf = 10.0; break;
				default:break;
			}

			cur_value = get_stripscan_disrange(pp->p_config)/100.0;
			lower = 0.51;
			upper = 46.0;
			step = tmpf;
			digit = 2;
			pos = 4;
			unit = UNIT_S;
			draw3_digit_pressed (data_404, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 11);

		}
		else
		{
			gtk_widget_hide (pp->eventbox30[4]);
			gtk_widget_hide (pp->eventbox31[4]);
		}

	}
	else
	{
		if(get_display_pos(pp->p_config)==Strip_Chart_AA)
			/*Display 为 Strip Chart-[A]*/
		{
			cur_value = get_stripscan_disrange(pp->p_config)/100.0;
			digit = 2;
			pos = 4;
			unit = UNIT_S;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 11);
		}

		else
		{
			gtk_widget_hide (pp->eventbox30[4]);
			gtk_widget_hide (pp->eventbox31[4]);
		}
	}
}

void DrawMenu414()
{
	draw3_popdown(menu_content[OFF_ON + get_overlay_cursor (pp->p_config)],4,0);
}

void DrawMenu424()
{
    int grp = get_current_group(pp->p_config);
	pp->x_pos = 560, pp->y_pos = 500;
	if (GROUP_VAL_POS( grp , col_select_pos) == 2)
	{

		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
			draw3_pop_tt (data_434, NULL,
					menu_content[COL_MODE+GROUP_VAL_POS( grp , col_mode)],
					menu_content+COL_MODE, 2, 4, GROUP_VAL_POS( grp , col_mode), 0);
		else
			draw3_popdown (menu_content[COL_MODE+GROUP_VAL_POS( grp , col_mode)], 4, 0);

		gtk_widget_set_sensitive(pp->eventbox30[4],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[4],FALSE);
	}
	else
	{
		gtk_widget_hide (pp->eventbox30[4]);
		gtk_widget_hide (pp->eventbox31[4]);
	}

}

void DrawMenu434()
{
    int grp = get_current_group(pp->p_config);
	pp->x_pos = 536, pp->y_pos = 455-26;
	switch(get_dis_prop_scan(pp->p_config))
	{
		case 0:	/* Ascan 时候的 appearance */
			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
				draw3_pop_tt (data_445, NULL,

						menu_content[PROP_OVERLAY + GROUP_VAL_POS( grp , ascan_overlay)],
						menu_content+PROP_OVERLAY, 3, 4, GROUP_VAL_POS( grp , ascan_overlay), 0);
			else
				draw3_popdown (menu_content[PROP_OVERLAY + GROUP_VAL_POS( grp , ascan_overlay)], 4, 0);
			break;
		case 1:
		case 2:
		case 3:
			gtk_widget_hide(pp->eventbox30[4]);
			gtk_widget_hide(pp->eventbox31[4]);
			break;
		default:break;

	}
}

void DrawMenu444()
{
	if ( !con2_p[4][4][4] )
		gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu504()
{
	char temp[52];

	int grp = get_current_group (pp->p_config);

	if (get_probe_select (pp->p_config)==0)
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
			draw_dialog_all (DIALOG_WEDGE);
		else
			draw3_popdown(GROUP_VAL_POS( grp , wedge.Model), 4, 0);

		g_sprintf (temp,"%s", con2_p[5][0][4]);
		gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
	}
	else if(get_probe_select (pp->p_config)==1)
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
		{
			draw_dialog_all (DIALOG_DEFINE_WEDGE);
		}
		else
		{
			draw3_popdown(GROUP_VAL_POS( grp , wedge.Model), 4, 0);
		}

		g_sprintf (temp,"%s", con2_p[5][0][7]);
		gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
	}
}

void DrawMenu514()
{
	if ( !con2_p[5][1][4] )
		gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}
void DrawMenu524()
{
	gtk_widget_hide(pp->eventbox30[4]);
	gtk_widget_hide(pp->eventbox31[4]);
#if 0
	char temp[52];
	/* 格式化字符串 */
	g_sprintf (temp,"%s", con2_p[5][2][4]);

	/* 设置label */
	gtk_label_set_text (GTK_LABEL (pp->label3[4]), temp);
	gtk_label_set_text (GTK_LABEL (pp->data3[4]), "Undefined");

	/* 显示和隐藏控件 */
	gtk_widget_show (pp->eventbox30[4]);
	gtk_widget_show (pp->eventbox31[4]);
	gtk_widget_show (pp->data3[4]);

	gtk_widget_set_sensitive(pp->eventbox30[4],FALSE);
	gtk_widget_set_sensitive(pp->eventbox31[4],FALSE);
#endif
}

extern GtkWidget* PartFileOpenNew(GtkWidget* fatherWidget) ;
void DrawMenu534()
{
	gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);

	//	焊缝加载
	//if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
	//{
		//draw_dialog_all (DIALOG_FILE_OPEN);
	//	GtkWidget* w = PartFileOpenNew(pp->window);
	//	gtk_widget_show(w);
	//}
	//else
	//	draw3_popdown(NULL,4,1);
	/*
	pp->x_pos = 536, pp->y_pos = 420;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
		draw3_pop_tt (data_534, NULL,
				menu_content[WELD + get_part_weld(pp->p_config)],
				menu_content+WELD, 4, 4, get_part_weld(pp->p_config), 0);
	else
		draw3_popdown (menu_content[WELD + get_part_weld(pp->p_config)], 4, 0);
	*/
}

void DrawMenu544()
{
	if ( !con2_p[5][4][4] )
			gtk_widget_hide (pp->eventbox30[4]);
		gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu604()
{
	//if ( !con2_p[6][0][4] )
	gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu614()
{
	gtk_widget_hide (pp->eventbox30[4]);
    gtk_widget_hide (pp->eventbox31[4]);
    return ;
    //*******************************************************

	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit, temp_beam;
	int grp = get_current_group (pp->p_config);

	switch (TMP(beam_skew_max_angle_reg))
	{
		case 0:	tmpf = 0.1; break;
		case 1:	tmpf = 1.0; break;
		case 2:	tmpf = 10.0; break;
		default:break;
	}
	if ((LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN) &&
			(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON)
			&& (GROUP_VAL_POS( grp , probe.PA_probe_type) == 9))
	{/* 聚焦法则自动计算开启时, Min Angle 才可调节 */
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
		{
			/* 最大不能超过最大Angle_beam_skew_max */
			cur_value = LAW_VAL_POS(grp , Angle_beam_skew_max) / 100.0;
			/* 计算lower为妙 */
			temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[get_current_group(pp->p_config)]);
			TMP(angle_num)	= (LAW_VAL_POS(grp , Angle_max) - LAW_VAL_POS(grp , Angle_min)) /
				LAW_VAL_POS(grp , Angle_step) + 1;
			temp_beam = temp_beam / TMP(angle_num);
			lower = LAW_VAL_POS(grp , Angle_beam_skew_min) / 100.0;
			upper = MIN (((int)(LAW_VAL_POS(grp , Angle_beam_skew_min)) +
						(int)(temp_beam * LAW_VAL_POS(grp , Angle_beam_skew_step))) / 100.0, 89.9);
			step = tmpf;
			digit = 1;
			pos = 4;
			unit = UNIT_DEG;
			draw3_digit_pressed (data_614, units[unit], cur_value , lower,
					upper, step, digit, NULL, pos, 0);
		}
		else
		{
			cur_value = LAW_VAL_POS(grp , Angle_beam_skew_max) / 100.0;
			digit = 1;
			pos = 4;
			unit = UNIT_DEG;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		}
	}
	else /* 聚焦法则自动计算为OFF, Max.Angle 不可调节 */
	{
		cur_value = LAW_VAL_POS(grp , Angle_beam_skew_min) / 100.0;
		digit = 1;
		pos = 4;
		unit = UNIT_DEG;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		gtk_widget_set_sensitive(pp->eventbox30[4],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[4],FALSE);
	}
}

void DrawMenu624()
{
	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit;
	int grp = get_current_group (pp->p_config);

	float multiplier;
	if(UNIT_MM == get_unit(pp->p_config))
	{
		multiplier = 1.0;
	}
	else
	{
		multiplier = 0.03937;
	}
	switch (TMP(depthe_reg))
	{
		case 0:	tmpf = 0.5; break;
		case 1:	tmpf = 5.0; break;
		case 2:	tmpf = 50.0; break;
		default:break;
	}
	if((LAW_VAL_POS(grp , Focal_point_type)==HALFPATH_P)||(LAW_VAL_POS(grp , Focal_point_type)==DEPTH_P))
	{
		gtk_widget_hide(pp->eventbox30[4]);
		gtk_widget_hide(pp->eventbox31[4]);
	}
	else
	{
		if(UNIT_MM == get_unit(pp->p_config))
		{
			unit = UNIT_MM;
			digit = 2;
		}
		else
		{
			unit = UNIT_INCH;
			digit = 3;
		}
		if ((get_auto_focal (pp->p_config) == AUTO_FOCAL_ON)&&(LAW_VAL_POS(grp , Focal_point_type)==FOCALPLANE_P))
		{
			if((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
			{
				cur_value = LAW_VAL_POS(grp , Depth_end) / 1000.0 * multiplier;
				lower = 0.1 * multiplier;
				upper = 10000.0 * multiplier;
				step = tmpf;
				pos = 4;

				draw3_digit_pressed (data_624, units[unit], cur_value , lower, upper,
						step, digit, NULL, pos, 0);
			}
			else
			{
				cur_value = LAW_VAL_POS(grp , Depth_end) / 1000.0 * multiplier;
				pos = 4;
				draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
			}
		}
		else
		{
			cur_value = LAW_VAL_POS(grp , Depth_end) / 1000.0 * multiplier;
			pos = 4;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
			gtk_widget_set_sensitive(pp->eventbox30[4],FALSE);
			gtk_widget_set_sensitive(pp->eventbox31[4],FALSE);
		}

	}
}

void DrawMenu634()
{
	int menu_status = 0, temp_qty =0, temp_pos = 0;
	int grp = get_current_group(pp->p_config);
	int material = get_part_material(grp);
	pp->x_pos = 555, pp->y_pos = 400;
	g_sprintf (TMP (velocity_data[0]), "LW:%dm/s", get_material_lw(material) / 100);
	g_sprintf (TMP (velocity_data[1]), "SW:%dm/s", get_material_sw(material) / 100);
	g_sprintf (TMP (velocity_data[2]), "%d m/s", GROUP_VAL_POS(grp , velocity) / 100);

	TMP(velocity_data_p[0]) =  TMP(velocity_data[0]);
	TMP(velocity_data_p[1]) =  TMP(velocity_data[1]);
	TMP(velocity_data_p[2]) =  TMP(velocity_data[2]);

	char **tp = TMP(velocity_data_p);
	if (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) == get_material_lw(material))
	{
		temp_pos = 0;
		temp_qty = 2;
	}
	else if (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) == get_material_sw(material))
	{
		temp_pos = 1;
		temp_qty = 2;
	}
	else
	{
		temp_pos = 2;
		temp_qty = 3;
	}

	if (get_auto_focal (pp->p_config) == AUTO_FOCAL_ON)
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
		{

			if (get_material_sw(material) == 0)
				menu_status = 0x02;

			draw3_pop_tt (data_634, NULL,
					tp[temp_pos],	(const char **)tp,	temp_qty, 4, temp_pos, menu_status);
		}
		else
		{
			draw3_popdown (TMP(velocity_data[temp_pos]), 4, 0);
		}
	}
	else
	{
		draw3_popdown ( TMP(velocity_data[temp_pos]), 4, 0);
		gtk_widget_set_sensitive(pp->eventbox30[4],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[4],FALSE);
	}
}

void DrawMenu644()
{
	if ( !con2_p[6][4][4] )
		gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu704()
{
	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit;
	/* 当前步进 */
	switch (TMP(origin_reg))
	{
		case 0:	tmpf = 0.01; break;
		case 1:	tmpf = 0.1; break;
		case 2:	tmpf = 1.0; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
	{
		cur_value = get_enc_origin (pp->p_config, get_cur_encoder (pp->p_config))/1000.0;
		lower = -1000;
		upper = 1000.00;
		step = tmpf;
		digit = 2;
		pos = 4;
		unit = UNIT_MM;
		draw3_digit_pressed (data_704, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 0);
	}
	else
	{
		cur_value = get_enc_origin (pp->p_config, get_cur_encoder (pp->p_config))/1000.0;
		digit = 2;
		pos = 4;
		unit = UNIT_MM;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
	}
	if( ((get_inspec_type (pp->p_config)==0) && (get_inspec_source (pp->p_config)==0)) || ((get_inspec_type (pp->p_config)==2)&&(get_inspec_source (pp->p_config)==0)&& ((get_inspec_index (pp->p_config)==0)||(get_inspec_index (pp->p_config)==3))) )
	{
		gtk_widget_set_sensitive(pp->eventbox30[4],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[4],FALSE);
	}
}

void DrawMenu714()
{
	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit, content_pos;

	if(get_inspec_type (pp->p_config)==2)
	{
		/* 当前步进 */
		switch (TMP(scanspeed_rpm_reg))
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

		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
		{
			cur_value = get_inspec_rpmspeed (pp->p_config)/10.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 1;
			pos = 4;
			unit = UNIT_RPM;
			draw3_digit_pressed (data_714, units[unit], cur_value , lower, upper, step, digit, NULL, pos, content_pos);
		}
		else
		{
			cur_value = get_inspec_rpmspeed (pp->p_config)/10.0;
			digit = 1;
			pos = 4;
			unit = UNIT_RPM;
			draw3_digit_stop (cur_value, units[unit], digit, pos, content_pos);
		}
	}
	else
	{
		gtk_widget_hide (pp->eventbox30[4]);
		gtk_widget_hide (pp->eventbox31[4]);
	}
}

void DrawMenu724()
{
	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit;

	switch (pp->p_tmp_config->index_end_reg)
	{
		case 0:	tmpf = 0.001 * get_area_indexresolution (pp->p_config); break;
		case 1:	tmpf = 0.01 * get_area_indexresolution (pp->p_config); break;
		case 2:	tmpf = 0.1 * get_area_indexresolution (pp->p_config); break;
		default:break;
	}

	if(get_inspec_type (pp->p_config)==1 || get_inspec_type (pp->p_config)==2)
		/* Inspection -> Type 选择 Helicoidal Scan \ Raster Scan  时 */
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
		{
			if( get_unit(pp->p_config) == UNIT_MM )
			{
				cur_value = get_area_indexend (pp->p_config)/1000.0;
				lower = get_area_indexstart (pp->p_config)/1000.0;
				upper = 100000.0;
				step = tmpf;
				digit = 2;
				pos = 4;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = get_area_indexend (pp->p_config)/1000.0*0.03937;
				lower = get_area_indexstart (pp->p_config)/1000.0*0.03937;
				upper = 100000.0*0.03937;
				step = tmpf*0.03937;
				digit = 3;
				pos = 4;
				unit = UNIT_INCH;
			}
			draw3_digit_pressed (data_724, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 0);
		}
		else
		{
			if( get_unit(pp->p_config) == UNIT_MM )
			{
				cur_value = get_area_indexend (pp->p_config)/1000.0;
				digit = 2;
				pos = 4;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = get_area_indexend (pp->p_config)/1000.0*0.03937;
				digit = 3;
				pos = 4;
				unit = UNIT_INCH;
			}
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		}
	}

	else
		/* Inspection -> Type 选择 One-Line Scan 时 */
	{
		if( get_unit(pp->p_config) == UNIT_MM )
		{
			cur_value = get_area_indexend (pp->p_config)/1000.0;
			digit = 2;
			pos = 4;
			unit = UNIT_MM;
		}
		else
		{
			cur_value = get_area_indexend (pp->p_config)/1000.0*0.03937;
			digit = 3;
			pos = 4;
			unit = UNIT_INCH;
		}
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);

		gtk_widget_set_sensitive (pp->eventbox30[4], FALSE);
		gtk_widget_set_sensitive (pp->eventbox31[4], FALSE);
	}
}

void DrawMenu734()
{
	if ( !con2_p[7][3][4] )
		gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu744()
{
	if ( !con2_p[7][4][4] )
		gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu804()
{
	pp->x_pos = 540, pp->y_pos = 456-26;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
		draw3_pop_tt (data_804, NULL,
				menu_content[SAVE_MODE+get_file_save_mode (pp->p_config)],
				menu_content+SAVE_MODE, 4, 4, get_file_save_mode (pp->p_config), 0);
	else
		draw3_popdown (menu_content[SAVE_MODE+get_file_save_mode (pp->p_config)], 4, 0);
}

void DrawMenu814()
{
	if ( !con2_p[8][1][4] )
		gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu824()
{
	pp->x_pos = 547, pp->y_pos = 456-26;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 4))
		draw3_pop_tt (data_824, NULL,
				menu_content[VIEW+get_report_format_view (pp->p_config)],
				menu_content+VIEW, 3, 4, get_report_format_view (pp->p_config), 0);
	else
		draw3_popdown (menu_content[VIEW+get_report_format_view (pp->p_config)], 4, 0);
}

void DrawMenu834()
{
	if ( !con2_p[8][3][4] )
		gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu844()
{
	if ( !con2_p[8][4][4] )
		gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu904()
{
	if ( !con2_p[9][0][4] )
		gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu914()
{
	if ( !con2_p[9][1][4] )
		gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu924()
{
	if ( !con2_p[9][2][4] )
		gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu934()
{
	if ( !con2_p[9][3][4] )
		gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}

void DrawMenu944()
{
	if ( !con2_p[9][4][4] )
		gtk_widget_hide (pp->eventbox30[4]);
	gtk_widget_hide (pp->eventbox31[4]);
}

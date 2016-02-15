/*
 * drawmenu5.c
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
#include "../../calculation/limit.h"
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

#include "../ui.h"
#include "../../string/_string.h"

void DrawSampleRange()
{
	double tmpf = 0.0, tmpfm;
	double max_tmp = 0.0,  max_tmp1 = 0.0, cur_value, lower, upper, step;
	int digit, pos, unit;
	int grp = get_current_group (pp->p_config);
	int _nPointQty = GROUP_VAL_POS(grp , point_qty);
    double _nStart = group_get_start(grp) / 1000.0   ;
	double _nRange = GROUP_VAL_POS(grp , range) / 1000.0   ;
	double _nVelocity = GROUP_VAL_POS(grp , velocity) / 100.0  ;
	double _nLimitRange  = PrfLimitToGroupSampleRange(grp) / 100.0 ;
	double _nCosCurrentAngle  = cos(TMP(current_angle[grp]))  ;

	switch (TMP(range_reg))
	{
		case 0:	tmpf = _nPointQty / 100.0; break;
		case 1:	tmpf = _nPointQty / 20.0; break;
		case 2:	tmpf = _nPointQty / 10.0; break;
		default:break;
	}
	tmpfm = GROUP_VAL_POS( grp , point_qty) / 100.0;
	pos = 5 ;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
	{
		max_tmp = (_nLimitRange  - _nStart);
		max_tmp1 = _nPointQty * 20.0;

		if(UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit))
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
				cur_value = _nCosCurrentAngle * _nRange * _nVelocity / 2000.0;   /* 当前显示的范围数值mm */
				if(GROUP_VAL_POS( grp , point_qty_pos)== 0)
				{
					lower = ( 32 / 100.0) * _nVelocity / 2000.0;
				}
				else
				{
					lower = (_nPointQty / 100.0) * _nVelocity / 2000.0;
				}
				lower = lower * _nCosCurrentAngle;
				upper = MIN(max_tmp, max_tmp1) * _nVelocity / 2000.0 ;
				upper = upper * _nCosCurrentAngle ;
				if(GROUP_VAL_POS( grp , point_qty_pos)== 0)
				{
					step = tmpf ;
				}
				else
				{
					step = tmpf * _nVelocity / 2000.0;
				}
				step = step * _nCosCurrentAngle;
				tmpfm = tmpfm * _nVelocity / 2000.0;
				digit = 2;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = 0.0397 * _nCosCurrentAngle * _nRange * _nVelocity / 2000.0;   /* 当前显示的范围数值mm */
				if(GROUP_VAL_POS( grp , point_qty_pos)== 0)
				{
					lower = ( 32 / 100.0) * _nVelocity / 2000.0;
				}
				else
				{
					lower = (_nPointQty / 100.0) * _nVelocity / 2000.0;
				}
				lower = 0.0397 * lower * _nCosCurrentAngle;
				upper = MIN(max_tmp, max_tmp1) * _nVelocity / 2000.0 ;
				upper = 0.0397 * upper * _nCosCurrentAngle ;
				if(GROUP_VAL_POS( grp , point_qty_pos)== 0)
				{
					step = tmpf ;
				}
				else
				{
					step = tmpf * _nVelocity / 2000.0;
				}
				step = 0.0397 * step * _nCosCurrentAngle;
				tmpfm = 0.0397 * tmpfm * _nVelocity / 2000.0;

				digit = 3;
				unit = UNIT_INCH;
			}
		}
		else if(UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit))
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
				cur_value = _nRange * _nVelocity / 2000.0;   /* 当前显示的范围数值mm */
				if(GROUP_VAL_POS( grp , point_qty_pos)== 0)
				{
					lower = ( 32 / 100.0) * _nVelocity / 2000.0;
				}
				else
				{
					lower = (_nPointQty / 100.0) * _nVelocity / 2000.0;
				}
				upper = MIN(max_tmp, max_tmp1) * _nVelocity / 2000.0 ;
				if(GROUP_VAL_POS( grp , point_qty_pos)== 0)
				{
					step = tmpf ;
				}
				else
				{
					step = tmpf * _nVelocity / 2000.0;
				}
				tmpfm = tmpfm * _nVelocity / 2000.0;
				digit = 2;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = 0.0397 * _nRange * _nVelocity / 2000.0;   /* 当前显示的范围数值mm */
				if(GROUP_VAL_POS( grp , point_qty_pos)== 0)
				{
					lower = ( 32 / 100.0) * _nVelocity / 2000.0;
				}
				else
				{
					lower = (_nPointQty / 100.0) * _nVelocity / 2000.0;
				}
				lower = 0.0397 * lower ;
				upper = MIN(max_tmp, max_tmp1) * _nVelocity / 2000.0 ;
				upper = 0.0397 * upper  ;
				if(GROUP_VAL_POS( grp , point_qty_pos)== 0)
				{
					step = tmpf ;
				}
				else
				{
					step = tmpf * _nVelocity / 2000.0;
				}
				step = 0.0397 * step ;
				tmpfm = 0.0397 * tmpfm * _nVelocity / 2000.0;
				digit = 3;
				unit = UNIT_INCH;
			}
		}
		else
		{
			cur_value = _nRange ;
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
				step = tmpf ;
			}
			else
			{
				step = tmpf;
			}
			digit = 2;
			unit = UNIT_US;
		}
		draw3_digit_pressed (data_102, units[unit], cur_value , lower, upper, step, digit, pp, pos, 11);
	}
	else
	{
		if(UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit))
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
				cur_value = _nCosCurrentAngle * _nRange * _nVelocity / 2000.0;
				unit = UNIT_MM;
				digit = 2;
			}
			else
			{
				cur_value = 0.0397 * _nCosCurrentAngle * _nRange * _nVelocity / 2000.0;
				unit = UNIT_INCH;
				digit = 3;
			}
		}
		else if(UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit))
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
				cur_value = _nRange * _nVelocity / 2000.0;
				unit = UNIT_MM;
				digit = 2;
			}
			else
			{
				cur_value = 0.0397 * _nRange * _nVelocity / 2000.0;
				unit = UNIT_INCH;
				digit = 3;
			}
		}
		else
		{
			cur_value = _nRange ;
			unit = UNIT_US;
			digit = 2;
		}
		draw3_digit_stop (cur_value , units[unit], digit, pos, 11);
	}
}

void DrawMenu005(){}
void DrawMenu015(){}



void DrawMenu025Encoder()
{
	gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu025Step1()
{
	int grp = get_current_group(pp->p_config);
	if(PA_SCAN != GROUP_VAL_POS(grp , group_mode))
	{
		gtk_widget_hide (pp->eventbox30[5]);
		gtk_widget_hide (pp->eventbox31[5]);
	}
	//else if ((pp->ctype_pos == 1)&&(pp->cmode_pos == 2))
	//{
	//	draw3_popdown(NULL, 5,1);
	//}
	else if ((pp->ctype_pos == 1)&&(pp->cmode_pos == 3))
	{
		draw3_popdown_offset(NULL, 5,1,7);
	}
	else
	{
		gtk_widget_hide (pp->eventbox30[5]);
		gtk_widget_hide (pp->eventbox31[5]);
	}
}

void DrawMenu025UltrasoundVelocityStep2()
{
	DrawSampleRange();
}

void DrawMenu025UltrasoundVelocityStep3()
{
	if(pp->cstart_qty == 4)
	     draw3_popdown_offset(NULL, 5,1,31);
	else if(pp->cstart_qty == 3)
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
		{
			cur_value = pp->radius2 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 5;
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
			draw3_digit_pressed (data_024, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 14);
		}
		else
		{
			cur_value = pp->radius2 / 1000.0;
			digit = 2;
			pos = 5;
			unit = UNIT_MM;
			if(get_unit(pp->p_config) == UNIT_INCH)
			{
				cur_value *= 0.03937;
				digit = 3;
				unit = UNIT_INCH;
			}
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
		{
			cur_value = pp->depth2 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 5;
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
			draw3_digit_pressed (data_0241, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 16);
		}
		else
		{
			cur_value = pp->depth2 / 1000.0;
			digit = 2;
			pos = 5;
			unit = UNIT_MM;
			if(get_unit(pp->p_config) == UNIT_INCH)
			{
				cur_value *= 0.03937;
				digit = 3;
				unit = UNIT_INCH;
			}
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
		{
			cur_value = pp->thickness2 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 5;
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
			draw3_digit_pressed (data_0242, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 18);
		}
		else
		{
			cur_value = pp->thickness2 / 1000.0;
			digit = 2;
			pos = 5;
			unit = UNIT_MM;
			if(get_unit(pp->p_config) == UNIT_INCH)
			{
				cur_value *= 0.03937;
				digit = 3;
				unit = UNIT_INCH;
			}
			draw3_digit_stop (cur_value, units[unit], digit, pos, 18);
		}
	}
	}
	else
	{
		gtk_widget_hide (pp->eventbox30[5]);
		gtk_widget_hide (pp->eventbox31[5]);
	}
}

void DrawMenu025UltrasoundVelocityStep4()
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
		{
			cur_value = pp->radius1 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 5;
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
			pos = 5;
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
		{
			cur_value = pp->depth1 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 5;
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
			pos = 5;
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
		{
			cur_value = pp->thickness1 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 5;
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
			pos = 5;
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

void DrawMenu025UltrasoundVelocityStep5()
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
		{
			cur_value = pp->radius2 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 5;
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
			draw3_digit_pressed (data_024, units[unit], cur_value , lower, upper, step, digit, pp, pos, 14);
		}
		else
		{
			cur_value = pp->radius2 / 1000.0;
			digit = 2;
			pos = 5;
			unit = UNIT_MM;
			if(get_unit(pp->p_config) == UNIT_INCH)
			{
				cur_value *= 0.03937;
				digit = 3;
				unit = UNIT_INCH;
			}
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
		{
			cur_value = pp->depth2 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 5;
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
			draw3_digit_pressed (data_0241, units[unit], cur_value , lower, upper, step, digit, pp, pos, 16);
		}
		else
		{
			cur_value = pp->depth2 / 1000.0;
			digit = 2;
			pos = 5;
			unit = UNIT_MM;
			if(get_unit(pp->p_config) == UNIT_INCH)
			{
				cur_value *= 0.03937;
				digit = 3;
				unit = UNIT_INCH;
			}
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
		{
			cur_value = pp->thickness2 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 5;
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
			draw3_digit_pressed (data_0242, units[unit], cur_value , lower, upper, step, digit, pp, pos, 18);
		}
		else
		{
			cur_value = pp->thickness2 / 1000.0;
			digit = 2;
			pos = 5;
			unit = UNIT_MM;
			if(get_unit(pp->p_config) == UNIT_INCH)
			{
				cur_value *= 0.03937;
				digit = 3;
				unit = UNIT_INCH;
			}
			draw3_digit_stop (cur_value, units[unit], digit, pos, 18);
		}
	}
}

void DrawMenu025UltrasoundVelocityStep6()
{
	gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu025UltrasoundWedgeDelayStep2()
{
	gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu025UltrasoundWedgeDelayStep3()
{
	gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu025UltrasoundWedgeDelayStep4()
{
	double tmpf = 0.0 ;
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
		{
			cur_value = pp->radius1 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 5;
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
			pos = 5;
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
		{
			cur_value = pp->depth1 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 5;
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
			pos = 5;
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
		{
			cur_value = pp->thickness1 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 5;
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
			pos = 5;
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

void DrawMenu025UltrasoundWedgeDelayStep5()
{
	int grp = get_current_group(pp->p_config);
	if(GROUP_VAL_POS(grp , group_mode) == PA_SCAN)
	{
		draw3_popdown_offset(NULL, 5,1,31);
	}
	else
	{
		gtk_widget_hide (pp->eventbox30[5]);
		gtk_widget_hide (pp->eventbox31[5]);
	}
}


void DrawMenu025Step6()
{
	int grp = get_current_group(pp->p_config);
	if(GROUP_VAL_POS(grp , group_mode) == PA_SCAN )
	{
		draw3_popdown_offset(NULL, 5,1,31);
	}
	else
	{
		if((pp->cmode_pos == 0))
		{
			draw3_popdown_offset(NULL, 5,1,31);
		}
		else
		{
			gtk_widget_hide (pp->eventbox30[5]);
			gtk_widget_hide (pp->eventbox31[5]);
		}
	}
}

void DrawMenu025UltrasoundSensationStep2()
{
	draw3_popdown(NULL, 5,1);
}
void DrawMenu025UltrasoundSensationStep3()
{
	gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}
void DrawMenu025UltrasoundSensationStep4()
{
	gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}
void DrawMenu025UltrasoundSensationStep5()
{
	draw3_popdown_offset(NULL, 5,1,29);
}
void DrawMenu025UltrasoundUtWedgedelayAndVelocityStep2()
{
	DrawSampleRange();
}
void DrawMenu025UltrasoundUtWedgedelayAndVelocityStep3()
{
	gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}
void DrawMenu025UltrasoundUtWedgedelayAndVelocityStep4()
{
	double tmpf = 0.0 ;
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
		{
			cur_value = pp->radius1 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 5;
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
			pos = 5;
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
		{
			cur_value = pp->depth1 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 5;
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
			pos = 5;
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
		{
			cur_value = pp->thickness1 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 5;
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
			pos = 5;
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
void DrawMenu025UltrasoundUtWedgedelayAndVelocityStep5()
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
		{
			cur_value = pp->radius2 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 5;
			unit = UNIT_MM;
			draw3_digit_pressed (data_024, units[unit], cur_value , lower, upper, step, digit, pp, pos, 14);
		}
		else
		{
			cur_value = pp->radius2 / 1000.0;
			digit = 2;
			pos = 5;
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
		{
			cur_value = pp->depth2 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 5;
			unit = UNIT_MM;
			draw3_digit_pressed (data_0241, units[unit], cur_value , lower, upper, step, digit, pp, pos, 16);
		}
		else
		{
			cur_value = pp->depth2 / 1000.0;
			digit = 2;
			pos = 5;
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
		{
			cur_value = pp->thickness2 / 1000.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 2;
			pos = 5;
			unit = UNIT_MM;
			draw3_digit_pressed (data_0242, units[unit], cur_value , lower, upper, step, digit, pp, pos, 18);
		}
		else
		{
			cur_value = pp->thickness2 / 1000.0;
			digit = 2;
			pos = 5;
			unit = UNIT_MM;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 18);
		}
	}
}

void DrawMenu025CodeAWSStep5()
{
	draw3_popdown_offset(NULL, 5,1,31);
}

void DrawMenu025UltrasoundVelocity()
{
	switch(pp->cstart_qty)
	{
	case 0:
	case 1:
		DrawMenu025Step1();
		break;
	case 2:
		DrawMenu025UltrasoundVelocityStep2();
		break;
	case 3:
		DrawMenu025UltrasoundVelocityStep3();
		break;
	case 4:
		DrawMenu025UltrasoundVelocityStep4();
		break;
	case 5:
		DrawMenu025UltrasoundVelocityStep5();
		break;
	case 6:
		DrawMenu025UltrasoundVelocityStep6();
		break;
	default:
		break;
	}
}

void DrawMenu025UltrasoundWedgeDelay()
{
	switch(pp->cstart_qty)
	{
	case 0:
	case 1:
		DrawMenu025Step1();
		break;
	case 2:
		DrawMenu025UltrasoundWedgeDelayStep2();
		break;
	case 3:
		DrawMenu025UltrasoundWedgeDelayStep3();
		break;
	case 4:
		DrawMenu025UltrasoundWedgeDelayStep4();
		break;
	case 5:
		DrawMenu025UltrasoundWedgeDelayStep5();
		break;
	case 6:
		DrawMenu025Step6();
		break;
	default:
		break;

	}
}

void DrawMenu025UltrasoundSensation()
{
	int _nGroupId = get_current_group(pp->p_config);
	if(PA_SCAN == GROUP_VAL_POS(_nGroupId , group_mode))
	{
		switch(pp->cstart_qty)
		{
		case 0:
		case 1:
		case 2:
			DrawMenu025Step1();
			break;
		case 3:
			DrawMenu025UltrasoundSensationStep3();
			break;
		case 4:
			DrawMenu025UltrasoundSensationStep4();
			break;
		case 5:
			DrawMenu025UltrasoundSensationStep5();
			break;
		case 6:
			DrawMenu025Step6();
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
			DrawMenu025Step1();
			break;
		case 2:
			DrawMenu025UltrasoundUtWedgedelayAndVelocityStep2();
			break;
		case 3:
			DrawMenu025UltrasoundUtWedgedelayAndVelocityStep3();
			break;
		case 4:
			DrawMenu025UltrasoundUtWedgedelayAndVelocityStep4();
			break;
		case 5:
			DrawMenu025UltrasoundUtWedgedelayAndVelocityStep5();
			break;
		case 6:
			DrawMenu025Step6();
			break;
		default:
			break;
		}
	}
}

void DrawMenu025UltrasoundTCGStep6()
{
	draw3_popdown_offset(NULL, 5, 1 , 61);
	int grp = get_current_group(pp->p_config);
	if(!GROUP_VAL_POS(grp , SizingCurves.point_pos))
	{
		gtk_widget_set_sensitive(pp->eventbox30[5] , FALSE );
		gtk_widget_set_sensitive(pp->eventbox31[5] , FALSE );
	}
}

void CalibrationLastLawEaglet()
{
	double cur_value=0.0, lower = 0, upper = 0, step = 0;
	int digit = 0, pos=5, unit = 0;
	int grp = get_current_group (pp->p_config);
	if (LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN)
	{
		if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 5))
		{
			cur_value = TMP_CBA(clb_end) / 100.0 ;
			lower =	TMP_CBA(clb_start) / 100.0  ;
			upper =	LAW_VAL_POS(grp , Angle_max) / 100.0;
			step  = LAW_VAL_POS(grp , Angle_step) / 100.0;
			digit = 1;
			unit = UNIT_DEG;
			draw3_digit_pressed (data_0238, units[unit], cur_value,
					lower, upper, step, digit, pp, pos, 28);
		}
		else
		{
			cur_value = TMP_CBA(clb_end)/100.0 ;
			unit = UNIT_DEG;
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
			unit = UNIT_VPA;
			draw3_digit_pressed (data_0238, units[unit], cur_value , lower, upper, step, digit, pp, pos, 46);
		}
		else
		{
			cur_value = TMP_CBA(clb_end) / 100.0;
			digit = 0;
			unit = UNIT_VPA;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 46);
		}
	}
}

void DrawMenu102Eaglet()
{
	double tmpf = 0.0 , tmpfm;
	double cur_value=0.0, lower=0, upper=0, step=0 , max_tmp=0, max_tmp1=0;
	int digit = 0, pos=5, unit = 0;
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
			cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
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
			unit = UNIT_MM;
		}
		else
		{
			cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
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
			unit = UNIT_INCH;
		}
	}
	else if(UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit))
	{
		if (UNIT_MM == get_unit(pp->p_config))
		{
			cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */

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
			unit = UNIT_MM;
		}
		else
		{
			cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
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
			unit = UNIT_INCH;
		}
	}
	else
	{
		cur_value = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0 ;
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
		unit = UNIT_US;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
	{//edit by lzk，条件放到这的目的是为了else状态时也计算lower
		draw3_digit_pressed (data_102, units[unit], cur_value , lower, upper, step, digit, pp, pos, 11);
	}
	else
	{
		if(UT_UNIT_TRUE_DEPTH == GROUP_VAL_POS( grp , ut_unit))
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
				cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
				cur_value = cur_value * cos(TMP(current_angle[grp]));
				unit = UNIT_MM;
				digit = 2;
			}
			else
			{
				cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
				cur_value = cur_value * cos(TMP(current_angle[grp]));
				unit = UNIT_INCH;
				digit = 3;
			}
		}
		else if(UT_UNIT_SOUNDPATH == GROUP_VAL_POS( grp , ut_unit))
		{
			if (UNIT_MM == get_unit(pp->p_config))
			{
				cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0);   /* 当前显示的范围数值mm */
				unit = UNIT_MM;
				digit = 2;
			}
			else
			{
				cur_value = (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0) * 0.03937 * (get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 200000.0); /* 当前显示的范围inch */
				unit = UNIT_INCH;
				digit = 3;
			}
		}
		else
		{
			cur_value = get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_RANGE) / 1000.0 ;
			unit = UNIT_US;
			digit = 2;
		}
		draw3_digit_stop (cur_value , units[unit], digit, pos, 11);
	}
}

void DrawMenu025UltrasoundTCG()
{
	switch(pp->cstart_qty)
	{
	case 0:
	case 1:
	case 4:
	case 6:
		gtk_widget_hide (pp->eventbox30[5]);
		gtk_widget_hide (pp->eventbox31[5]);
		break;
	case 2:
        CalibrationLastLawEaglet();
		break;
	case 3:
        DrawMenu102Eaglet();
		break;
	case 5:
		DrawMenu025UltrasoundTCGStep6();
		break;
	default:
		break;

	}
}


void DrawMenu025TOFDStep3()
{
	DrawSampleRange();
}

void DrawMenu025TOFDStep6()
{
	draw3_popdown_offset(NULL, 5,1,31);
}

void DrawMenu025TOFD()
{
	switch(pp->cstart_qty)
	{
	case 0:
	case 1:
		DrawMenu025Step1();
		break;
	case 3:
		DrawMenu025TOFDStep3();
		break;
	case 2:
	case 4:
		DrawMenu025Step1();
		break;
	case 5:
		//DrawMenu025Step1();
		DrawMenu025TOFDStep6();
		break;
	default:
		break;
	}
}

void DrawMenu025Ultrasound()
{
	int grp = get_current_group(pp->p_config);
	if(GROUP_VAL_POS(grp , tx_rxmode1) == TOFD)
	{
		DrawMenu025TOFD();
	}
	else
	{
		switch(pp->cmode_pos)
		{
		case 0:
			DrawMenu025UltrasoundVelocity();
			break;
		case 1:
			DrawMenu025UltrasoundWedgeDelay();
			break;
		case 2:
			DrawMenu025UltrasoundSensation();
			break;
		case 3:
			DrawMenu025UltrasoundTCG();
			break;
		default:
			break;
		}
	}
}


void DrawMenu025CodeAPI()
{
	gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu025CodeAWS()
{
	switch(pp->cstart_qty)
	{
	case 0:
	case 1:
		DrawMenu025Step1();
		break;
	case 2:
	case 3:
	case 4:
		gtk_widget_hide (pp->eventbox30[5]);
		gtk_widget_hide (pp->eventbox31[5]);
		break;
	case 5:
		gtk_widget_hide (pp->eventbox30[5]);
		gtk_widget_hide (pp->eventbox31[5]);
		//DrawMenu025CodeAWSStep5();
		break;
	case 6:
		gtk_widget_hide (pp->eventbox30[5]);
		gtk_widget_hide (pp->eventbox31[5]);
		//DrawMenu025Step6();
		break;
	default:
		break;
	}
}

void DrawMenu025Code()
{
	switch(pp->scode_pos)
	{
	case 0:
		DrawMenu025CodeAPI();
		break;
	case 1:
		DrawMenu025CodeAWS();
		break;
	default:
		break;
	}
}

void DrawMenu025()
{
	switch (pp->ctype_pos)
	{
	case 0:// encoder calibration
		DrawMenu025Encoder();
		break;
	case 1:
		DrawMenu025Ultrasound();
		break;
	case 2:
		DrawMenu025Code();
		break;
	default:
		break;
	}
}

void DrawMenu035()
{
	if ( !con2_p[0][3][5] )
		gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu045()
{
	if ( !con2_p[0][4][5] )
		gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu105()
{
	int menu_status =  0 ;
	int grp = get_current_group(pp->p_config);
	int material = get_part_material(grp);
	if ( GROUP_VAL_POS( grp , group_mode) == PA_SCAN )
	{
		gtk_widget_hide (pp->eventbox30[5]);
		gtk_widget_hide (pp->eventbox31[5]);
	}
	else
	{
		int temp_pos ;
		int temp_qty ;
		pp->x_pos = 578, pp->y_pos = 543-26;

		g_sprintf (TMP (velocity_data[0]), "LW:%dm/s", get_material_lw(material) / 100);
		g_sprintf (TMP (velocity_data[1]), "SW:%dm/s", get_material_sw(material) / 100);
		g_sprintf (TMP (velocity_data[2]), "%d m/s", get_group_val (get_group_by_id (pp->p_config, get_current_group(pp->p_config)), GROUP_VELOCITY) / 100);

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

		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
		{

			if (get_material_sw(material) == 0)
				menu_status = 0x02;

			draw3_pop_tt (data_634, NULL,
					tp[temp_pos],	(const char **)tp,	temp_qty, 5, temp_pos, menu_status);
		}
		else
		{
			draw3_popdown (TMP(velocity_data[temp_pos]), 5, 0);
		}

	}
}

void DrawMenu115()
{
	char* str ;
	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit;

	pp->x_pos = 578, pp->y_pos = 500-26;
	switch (TMP(prf_reg))
	{
		case 0:	tmpf = 1.0; break;
		case 1:	tmpf = 10.0; break;
		case 2:	tmpf = 100.0; break;
		default:break;
	}
	if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 5))
	{
		if (pp->mark_pop_change)
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
			if (GROUP_VAL_POS(0 , prf_pos1) == 3)	/* 自定义数值时候按下显示数值 */
			{
				/* 更新当前增益值显示 */
				str = g_strdup_printf ("%d", get_prf() / 10);
				draw3_pop_tt (data_115, NULL,
						str, menu_content + PRF, 4, 5, GROUP_VAL_POS(0 , prf_pos1), 0);
				g_free(str);
			}
			else	/* Auto ==时候 显示当前选项 */
				draw3_pop_tt (data_115, NULL, menu_content[PRF + GROUP_VAL_POS(0 , prf_pos1)],
						menu_content + PRF, 4, 5, GROUP_VAL_POS(0 , prf_pos1), 0);
		}
	}
	else
	{
		cur_value = GROUP_VAL_POS(0 , prf1) / 10.0;
		unit = UNIT_NULL;
		pos = 5;
		digit = 0;
		draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
	}
}

void DrawMenu125()
{
	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit;

	switch (TMP(reject_reg))
	{
		case 0:	tmpf = 1.0; break;
		case 1:	tmpf = 10.0; break;
		case 2:	tmpf = 100.0; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
	{
		cur_value = get_reject(pp->p_config);
		lower = 0.0;
		upper = 100.0;
		step = tmpf;
		digit = 0;
		pos = 5;
		unit = UNIT_BFH;
		draw3_digit_pressed (data_125, units[unit], cur_value , lower,
				upper, step, digit, pp, pos, 0);
	}
	else
	{
		cur_value = get_reject(pp->p_config);
		digit = 0;
		pos = 5;
		unit = UNIT_BFH;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
	}
}

void DrawMenu135()
{
	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit;
	int grp = get_current_group (pp->p_config);

	if (GROUP_VAL_POS( grp , group_mode) != PA_SCAN)
	{
		gtk_widget_hide (pp->eventbox30[5]);
		gtk_widget_hide (pp->eventbox31[5]);
		return ;
	}
	else if (GROUP_VAL_POS( grp , group_mode) == PA_SCAN)
	{
		switch (TMP(gain_offset_reg))
		{
			case 0:	tmpf = 0.1; break;
			case 1:	tmpf = 0.5; break;
			case 2:	tmpf = 1.0; break;
			case 3:	tmpf = 2.0; break;
			case 4:	tmpf = 6.0; break;
			default:break;
		}

		cur_value = GROUP_VAL_POS( grp , gain_offset[TMP(beam_num[grp])])/10.0;
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
		{
			lower = 0.0;
			upper = GetGainOffsetLimit(grp) ;
			step = tmpf;
			digit = 1;
			pos = 5;
			unit = UNIT_DB;
			draw3_digit_pressed (data_135, units[unit], cur_value , lower, upper, step, digit, pp, pos, 0);
		}
		else
		{
			digit = 1;
			pos = 5;
			unit = UNIT_DB;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		}
	}
}

void DrawMenu145()
{
	char *str ;
	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit;
	int grp = get_current_group (pp->p_config);

	pp->x_pos = 587, pp->y_pos = 543-26;
	if(GROUP_VAL_POS( grp , group_mode)==1)
	{
		switch (TMP(sum_gain_reg))
		{
			case 0:	tmpf = 0.1; break;
			case 1:	tmpf = 0.5; break;
			case 2:	tmpf = 1.0; break;
			case 3:	tmpf = 2.0; break;
			case 4:	tmpf = 6.0; break;
			default:break;
		}
		if (LAW_VAL_POS(grp, Elem_qty) == 1)
			cur_value = 48.0  ;
		else
			cur_value = 20 * log10(GROUP_VAL_POS(grp , sum_gain) / 16.0);

		if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 5))
		{
			if (pp->mark_pop_change)
			{
				lower =	20.0;
				upper =	48.0;
				step = tmpf;
				digit = 1;
				pos = 5;
				unit = UNIT_NONE;
				draw3_digit_pressed (data_1451, units[unit], cur_value , lower, upper, step, digit, pp, pos, 0);
			}
			else
			{
				if (!GROUP_VAL_POS( grp , sum_gain_pos))
				{
					/* 更新当前增益值显示 */
					str = g_strdup_printf ("%s %0.1f",
							menu_content[SUM_GAIN + GROUP_VAL_POS( grp , sum_gain_pos)], cur_value);
					draw3_pop_tt (data_145, NULL,
							str, menu_content + SUM_GAIN, 2, 5, GROUP_VAL_POS( grp , sum_gain_pos), 0);
					g_free(str);
				}
				else
				{
					str = g_strdup_printf ("%0.1f", cur_value);
					draw3_pop_tt (data_145, NULL,
							str, menu_content + SUM_GAIN, 2, 5, GROUP_VAL_POS( grp , sum_gain_pos), 0);
					g_free(str);
				}
			}
		}
		else
		{
			if (!GROUP_VAL_POS( grp , sum_gain_pos))
			{
				str = g_strdup_printf ("%s %0.1f",
						menu_content[SUM_GAIN + GROUP_VAL_POS( grp , sum_gain_pos)], cur_value);
				draw3_popdown (str, 5, 0);
				g_free(str);
			}
			else
			{
				unit = UNIT_NULL;
				pos = 5;
				digit = 1;
				draw3_digit_stop (cur_value , units[unit], digit, pos, 0);
			}
		}
	}
	else
	{
		gtk_widget_hide (pp->eventbox30[5]);
		gtk_widget_hide (pp->eventbox31[5]);
	}
}

void DrawMenu205()
{
	int grp = get_current_group(pp->p_config) ;
    unsigned char mask = 1 << grp;
    if (TMP(gateShow)[GROUP_VAL_POS(grp , gate_pos)] & mask) {
        mask = 1;
    } else {
        mask = 0;
    }
	//draw3_popdown(menu_content[OFF_ON + TMP(gateShow)[GROUP_VAL_POS(grp , gate_pos)]],5,0);
	draw3_popdown(menu_content[OFF_ON + mask],5,0);
	gtk_widget_queue_draw(pp->vboxtable);
#if 0
	if ( !con2_p[2][0][5] )
	gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
#endif
}

void DrawMenu215(){}
void DrawMenu225(){}
void DrawMenu235()
{
	char* str ;
	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit;
	int grp = get_current_group (pp->p_config);
	GROUP* p_grp = get_group_by_id(pp->p_config , grp);

	if(GROUP_VAL_POS( grp , SizingCurves.mode_pos)==0)
	{
		if( GROUP_VAL_POS( grp , SizingCurves.curve_pos)==1 || GROUP_VAL_POS( grp , SizingCurves.curve_pos)==2 )
		{
			/* 当前步进 */
			switch (TMP(ref_gain_reg))
			{
				case 0:	tmpf = 0.1; break;
				case 1:	tmpf = 0.5; break;
				case 2:	tmpf = 1.0; break;
				case 3:	tmpf = 2.0; break;
				case 4:	tmpf = 6.0; break;
				default:break;
			}
			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
			{
                cur_value =group_get_refgain(grp)/100.0;
                lower = (group_get_refgain(grp)- group_get_gain(grp))/100.0;
                if (PA_SCAN == GROUP_VAL_POS(grp, group_mode)
                             || UT_SCAN == GROUP_VAL_POS(grp, group_mode)) {
                    upper = PA_MAX_GAIN + lower;
                } else {
                    upper = UT_MAX_GAIN + lower;
                }
				step = tmpf;
				digit = 1;
				pos = 5;
				unit = UNIT_DB;
				draw3_digit_pressed (data_235, units[unit], cur_value , lower, upper, step, digit, pp, pos, 9);
			}
			else
			{
                cur_value = group_get_refgain(grp)/100.0;
				digit = 1;
				pos = 5;
				unit = UNIT_DB;
				draw3_digit_stop (cur_value, units[unit], digit, pos, 9);
			}
		}
		else
		{
			gtk_widget_hide (pp->eventbox30[5]);
			gtk_widget_hide (pp->eventbox31[5]);
			gtk_widget_hide (pp->sbutton[5]);
		}
	}
	else if(GROUP_VAL_POS( grp , SizingCurves.mode_pos)==1)
	{
		if(GROUP_VAL_POS( grp , SizingCurves.curve_pos)==1 || GROUP_VAL_POS( grp , SizingCurves.curve_pos)==3)
		{
			draw3_popdown(NULL,5,1);
			str = g_strdup_printf ("%s", con2_p[2][3][14]);
			gtk_label_set_text (GTK_LABEL (pp->label3[5]), str);
			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
				data_235_del_point(pp);///////////////////////////////////////////////////
		}
		else
		{
			gtk_widget_hide (pp->eventbox30[5]);
			gtk_widget_hide (pp->eventbox31[5]);
			gtk_widget_hide (pp->sbutton[5]);

		}
	}
}

void DrawMenu245()
{
	if ( !con2_p[2][4][5] )
		gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu305()
{
	pp->x_pos = 200, pp->y_pos = 0;
	int feildNum = GetMesureFeildNum();
	const char* pSimpleStr[feildNum];
	const char* pStr[feildNum];
	GetMesureFeildString(pSimpleStr ,pStr);
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
		draw3_pop_tt_shen (data_305, NULL,
				pSimpleStr[get_reading_field4(pp->p_config)],
				pStr, feildNum, 5, get_reading_field4(pp->p_config), 0);
	else
		draw3_popdown (pSimpleStr[get_reading_field4(pp->p_config)], 5, 0);
}

void DrawMenu315()
{
	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit;
	int grp = get_current_group (pp->p_config);

	int  _nSelection  =  GROUP_VAL_POS(grp , selection) ;
	if(_nSelection == 0 || _nSelection == 2 || _nSelection == 7)
	{
		gtk_widget_hide (pp->eventbox30[5]);
		gtk_widget_hide (pp->eventbox31[5]);
	}
	else if(_nSelection == 1) {
		MENU_UNION menu_union = {{0 ,0 ,5 ,0}};
		
		menu_union.pos.menu1 = 3;
		menu_union.pos.menu2 = 1;
		menu_union.pos.menu3 = 5;
		menu3FuncTrans(menu_union);
    } else if(_nSelection == 3)
	{
		if(GROUP_VAL_POS(grp , skew_pos) == 1 || GROUP_VAL_POS(grp , skew_pos) == 3)
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
			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					cur_value = GROUP_VAL_POS( grp , i_measure)/100.0;
					lower = -9999.0;
					upper = 9999.0;
					step = tmpf;
					digit = 1;
					pos = 5;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = GROUP_VAL_POS( grp , i_measure)/100.0*0.03937;
					lower = -9999.0*0.03937;
					upper = 9999.0*0.03937;
					step = tmpf;
					digit = 3;
					pos = 5;
					unit = UNIT_INCH;
				}
				draw3_digit_pressed (data_3141, units[unit], cur_value , lower, upper, step, digit, pp, pos, 10);
			}
			else
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					cur_value = GROUP_VAL_POS( grp , i_measure)/100.0;
					digit = 1;
					pos = 5;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = GROUP_VAL_POS( grp , i_measure)/100.0*0.03937;
					digit = 3;
					pos = 5;
					unit = UNIT_INCH;
				}
				draw3_digit_stop (cur_value, units[unit], digit, pos, 10);
			}
		}
		else
		{
			/* 当前步进 */
			switch (pp->p_tmp_config->i_measure_reg)
			{
				case 0:	tmpf = 0.1; break;
				case 1:	tmpf = 0.5; break;
				case 2:	tmpf = 1.0; break;
				case 3:	tmpf = 10 ; break;
				default:break;
			}
			pos = 5 ;
			if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					cur_value = GROUP_VAL_POS( grp , s_measure)/100.0;
					lower = -9999.0;
					upper =  9999.0;
					step = tmpf;
					digit = 1;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = GROUP_VAL_POS( grp , s_measure)/100.0*0.03937;
					lower = -9999 * 0.03937;
					upper =  9999 * 0.03937;
					step  =  tmpf / 500.0  ;
					digit =  3;
					unit = UNIT_INCH;
				}
				draw3_digit_pressed (data_3143, units[unit], cur_value , lower, upper, step, digit, NULL, pos, 7);
			}
			else
			{
				if(get_unit(pp->p_config) == UNIT_MM)
				{
					cur_value = GROUP_VAL_POS( grp , s_measure)/100.0;
					digit = 1;
					unit = UNIT_MM;
				}
				else
				{
					cur_value = GROUP_VAL_POS( grp , s_measure)/100.0*0.03937;
					digit = 3;
					unit = UNIT_INCH;
				}
				draw3_digit_stop (cur_value, units[unit], digit, pos, 7);
			}
		}

	}
	else if ( _nSelection == 4||_nSelection==5||_nSelection==6)
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
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
		{
			if(get_unit(pp->p_config)==UNIT_MM)
			{
				cur_value = TMP(index)/100.0;
				lower = -100000.0;
				upper = 100000;
				step = tmpf;
				digit = 1;
				pos = 5;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = TMP(index)/100.0*0.03937;
				lower = -100000.0*0.03937;
				upper = 100000*0.03937;
				step = tmpf;
				digit = 3;
				pos = 5;
				unit = UNIT_INCH;
			}
			draw3_digit_pressed (data_315, units[unit], cur_value , lower, upper, step, digit, pp, pos, 16);
		}
		else
		{
			if(get_unit(pp->p_config)==UNIT_MM)
			{
				cur_value = TMP(index)/100.0;
				digit = 1;
				pos = 5;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = TMP(index)/100.0*0.03937;
				digit = 3;
				pos = 5;
				unit = UNIT_INCH;
			}
			draw3_digit_stop (cur_value, units[unit], digit, pos, 16);
		}
	}
}

void getTableComments();
void DrawMenu325()
{
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
	{
		getTableComments();
	}
	draw3_popdown(NULL,5,1);
	if(get_display_table(pp->p_config))
	{
		gtk_widget_set_sensitive(pp->eventbox30[5] ,FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[5] ,FALSE);
	}
	else
	{
	}
}

void DrawMenu335()
{
	if ( !con2_p[3][3][5] )
		gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu345()
{
	if ( !con2_p[3][4][5] )
		gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu405()
{
	double cur_value;
	int digit, pos, unit;
	if(get_display_pos(pp->p_config)==Strip_Chart_AA)
		/*Display 为 Strip Chart-[A]*/
	{
		if(UNIT_MM == get_unit(pp->p_config))
		{
			cur_value = GROUP_VAL_POS(0 , prf1) / 10.0;
			digit = 2;
			pos = 5;
			unit = UNIT_MM_S;
		}
		else
		{
			cur_value = GROUP_VAL_POS(0 , prf1) / 10.0 * 0.03937;
			digit = 3;
			pos = 5;
			unit = UNIT_INCH_S;
		}
		draw3_digit_stop (cur_value, units[unit], digit, pos, 12);
		gtk_widget_set_sensitive (pp->eventbox30[5],FALSE);
		gtk_widget_set_sensitive (pp->eventbox31[5],FALSE);
	}
	else
	{
		gtk_widget_hide (pp->eventbox30[5]);
		gtk_widget_hide (pp->eventbox31[5]);
	}
}

void DrawMenu415()
{
	draw3_popdown(menu_content[OFF_ON + get_overlay_overlay(pp->p_config)],5,0);
}

void DrawMenu425()
{
	if ( !con2_p[4][2][5] )
			gtk_widget_hide (pp->eventbox30[5]);
		gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu435()
{
	//int grp = get_current_group(pp->p_config);
	pp->x_pos = 575, pp->y_pos = 541-26;
	switch(get_dis_prop_scan(pp->p_config))
	{
		case 0:	/* Ascan 时候的 overlay */
			gtk_widget_hide(pp->eventbox30[5]);
			gtk_widget_hide(pp->eventbox31[5]);
			break;
		case 1:
			gtk_widget_hide(pp->eventbox30[5]);
			gtk_widget_hide(pp->eventbox31[5]);
			break;
		case 2:
			gtk_widget_hide(pp->eventbox30[5]);
			gtk_widget_hide(pp->eventbox31[5]);
			break;
		case 3:
			gtk_widget_hide(pp->eventbox30[5]);
			gtk_widget_hide(pp->eventbox31[5]);
			break;
		default:break;

	}
}

void DrawMenu445()
{
	if ( !con2_p[4][4][5] )
		gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu505()
{
	int grp = get_current_group(pp->p_config);
	if(GROUP_VAL_POS(grp , group_mode) == PA_SCAN || GROUP_VAL_POS(grp , group_mode) == UT_SCAN)
	{
		draw3_popdown (menu_content[OFF_ON + get_auto_detect (pp->p_config)], 5, 0);
	}
	else
	{
		set_auto_detect (pp->p_config , 0) ;
		draw3_popdown (menu_content[OFF_ON], 5, 0);
		gtk_widget_set_sensitive(pp->eventbox30[5], FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[5], FALSE);
	}
}

void DrawMenu515()
{
	if ( !con2_p[5][1][5] )
		gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu525()
{
	gtk_widget_hide(pp->eventbox30[5]);
	gtk_widget_hide(pp->eventbox31[5]);
#if 0
	return ;
    char temp[52] ;
	/* 格式化字符串 */
	g_sprintf (temp,"%s", con2_p[5][2][5]);
	/* 设置label */
	gtk_label_set_text (GTK_LABEL (pp->label3[5]), temp);
	gtk_label_set_text (GTK_LABEL (pp->data3[5]), "Undefined");
	/* 显示和隐藏控件 */
	gtk_widget_show (pp->eventbox30[5]);
	gtk_widget_show (pp->eventbox31[5]);
	gtk_widget_show (pp->data3[5]);

	gtk_widget_set_sensitive(pp->eventbox30[5],FALSE);
	gtk_widget_set_sensitive(pp->eventbox31[5],FALSE);
#endif
}

void DrawMenu535()
{
	if ( !con2_p[5][3][5] )
		gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu545()
{
	if ( !con2_p[5][4][5] )
		gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu605()
{
	//if ( !con2_p[6][0][5] )
	gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu615()
{
	gtk_widget_hide (pp->eventbox30[5]);
    gtk_widget_hide (pp->eventbox31[5]);
    return ;
    //*******************************************************

	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit, temp_beam;
	int grp = get_current_group (pp->p_config);

	switch (TMP(beam_skew_angle_step_reg))
	{
		case 0:	tmpf = 0.1; break;
		case 1:	tmpf = 1.0; break;
		case 2:	tmpf = 10.0; break;
		default:break;
	}
	if ((LAW_VAL_POS(grp , Focal_type) == AZIMUTHAL_SCAN) &&
			(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON)
			&& (GROUP_VAL_POS( grp , probe.PA_probe_type) == 9))
		/* 聚焦法则自动计算开启时, Min Angle 才可调节 */
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 3))
		{
			/* 最大不能超过最大Angle_beam_skew_max */
			cur_value = LAW_VAL_POS(grp , Angle_beam_skew_step) / 100.0;
			/* 计算lower为妙 */
			temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[get_current_group(pp->p_config)]);
			TMP(angle_num)	= (LAW_VAL_POS(grp , Angle_max) - LAW_VAL_POS(grp , Angle_min)) /
				LAW_VAL_POS(grp , Angle_step) + 1;
			temp_beam = temp_beam / TMP(angle_num);
			lower = MAX (((int)(LAW_VAL_POS(grp , Angle_beam_skew_max)) -
						(int)(LAW_VAL_POS(grp , Angle_beam_skew_min)))
					/ (100.0 * temp_beam), 0.1);
			upper = 89.9;
			step = tmpf;
			digit = 1;
			pos = 5;
			unit = UNIT_DEG;
			draw3_digit_pressed (data_614, units[unit], cur_value , lower,
					upper, step, digit, pp, pos, 0);
		}
		else
		{
			cur_value = LAW_VAL_POS(grp , Angle_beam_skew_max) / 100.0;
			digit = 1;
			pos = 5;
			unit = UNIT_DEG;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		}
	}
	else /* 聚焦法则自动计算为OFF, Max.Angle 不可调节 */
	{
		cur_value = LAW_VAL_POS(grp , Angle_beam_skew_min) / 100.0;
		digit = 1;
		pos = 5;
		unit = UNIT_DEG;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		gtk_widget_set_sensitive(pp->eventbox30[5],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[5],FALSE);
	}
}

void DrawMenu625()
{
	if ( !con2_p[6][2][5] )
		gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu635()
{
	if ( !con2_p[6][3][5] )
		gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu645()
{
	if ( !con2_p[6][4][5] )
		gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu705()
{
	draw3_popdown(NULL,5,1);
	if( ((get_inspec_type (pp->p_config)==0) && (get_inspec_source (pp->p_config)==0)) || ((get_inspec_type (pp->p_config)==2)&&(get_inspec_source (pp->p_config)==0)&& ((get_inspec_index (pp->p_config)==0)||(get_inspec_index (pp->p_config)==3))) )
	{
		gtk_widget_set_sensitive(pp->eventbox30[5],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[5],FALSE);
	}
}

void DrawMenu715()
{
	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit, content_pos;

	if(get_inspec_type (pp->p_config)==2)
	{
		/* 当前步进 */
		switch (TMP(indexspeed_reg))
		{
			case 0:	tmpf = 1.0; break;
			case 1:	tmpf = 10.0; break;
			case 2:	tmpf = 100.0; break;
			default:break;
		}

		if(get_inspec_source (pp->p_config)==0)
			content_pos=0;
		else
			content_pos=7;

		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
		{
			cur_value = get_inspec_indexspeed (pp->p_config)/10.0;
			lower = 0.0;
			upper = 1000.0;
			step = tmpf;
			digit = 1;
			pos = 5;
			unit = UNIT_RPM;
			draw3_digit_pressed (data_715, units[unit], cur_value , lower, upper, step, digit, pp, pos, content_pos);
		}
		else
		{
			cur_value = get_inspec_indexspeed (pp->p_config)/10.0;
			digit = 1;
			pos = 5;
			unit = UNIT_RPM;
			draw3_digit_stop (cur_value, units[unit], digit, pos, content_pos);
		}

		gtk_widget_set_sensitive (pp->eventbox30[5], FALSE);
		gtk_widget_set_sensitive (pp->eventbox31[5], FALSE);
	}
	else
	{
		gtk_widget_hide (pp->eventbox30[5]);
		gtk_widget_hide (pp->eventbox31[5]);
	}
}

void DrawMenu725()
{
	double tmpf = 0.0;
	double cur_value, lower, upper, step;
	int digit, pos, unit;

	switch (TMP(index_resolution_reg))
	{
		case 0:	tmpf = 0.1; break;
		case 1:	tmpf = 0.5; break;
		case 2:	tmpf = 1.0; break;
		default:break;
	}

	if(get_inspec_type (pp->p_config)==1 || get_inspec_type (pp->p_config)==2)
		/* Inspection -> Type 选择 Helicoidal Scan \ Raster Scan  时 */
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
		{
			if( get_unit(pp->p_config) == UNIT_MM )
			{
				cur_value = get_area_indexresolution (pp->p_config)/1000.0;
				lower = 0.01;
				upper = 99999.0;
				step = tmpf;
				digit = 2;
				pos = 5;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = get_area_indexresolution (pp->p_config)/1000.0*0.03937;
				lower = 0.001;
				upper = 99999.0 * 0.03937;
				step = tmpf/10.0;
				digit = 3;
				pos = 5;
				unit = UNIT_INCH;
			}
			draw3_digit_pressed (data_725, units[unit], cur_value , lower, upper, step, digit, pp, pos, 0);
		}
		else
		{
			if( get_unit(pp->p_config) == UNIT_MM )
			{
				cur_value = get_area_indexresolution (pp->p_config)/1000.0;
				digit = 2;
				pos = 5;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = get_area_indexresolution (pp->p_config)/1000.0 * 0.03937;
				digit = 3;
				pos = 5;
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
			cur_value = get_area_indexresolution (pp->p_config)/1000.0;
			digit = 2;
			pos = 5;
			unit = UNIT_MM;
		}
		else
		{
			cur_value = get_area_indexresolution (pp->p_config)/1000.0 * 0.03937;
			digit = 3;
			pos = 5;
			unit = UNIT_INCH;
		}
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);

		gtk_widget_set_sensitive (pp->eventbox30[5], FALSE);
		gtk_widget_set_sensitive (pp->eventbox31[5], FALSE);
	}
}

void DrawMenu735()
{
	if ( !con2_p[7][3][5] )
		gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu745()
{
	if ( !con2_p[7][4][5] )
		gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu805()
{
	char _str[100];
    unsigned int len, len2;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 5))
	{
		char* str = NULL;
		char* ptr = NULL;
		str = dialogGetAString( pp->window
							    ,getDictString(_STRING_File_Name)
		                        ,get_file_name_all (pp->p_config));

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
			set_file_name_all(pp->p_config ,str);
			free(str);
		}
		strcpy(_str , get_file_name_all (pp->p_config));
		strcat(_str , "###") ;
		draw3_popdown ( _str, 5, 0);
	}
	else
	{
		strcpy(_str , get_file_name_all (pp->p_config));
		strcat(_str , "###") ;
		draw3_popdown ( _str, 5, 0);
	}
}

void DrawMenu815()
{
	if ( !con2_p[8][1][5] )
		gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu825()
{
	if ( !con2_p[8][2][5] )
		gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu835()
{
	if ( !con2_p[8][3][5] )
		gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu845()
{
	if ( !con2_p[8][4][5] )
		gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu905()
{
	if ( !con2_p[9][0][5] )
		gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu915()
{
	if ( !con2_p[9][1][5] )
		gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu925()
{
	if ( !con2_p[9][2][5] )
		gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu935()
{
	if ( !con2_p[9][3][5] )
		gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

void DrawMenu945()
{
	if ( !con2_p[9][4][5] )
		gtk_widget_hide (pp->eventbox30[5]);
	gtk_widget_hide (pp->eventbox31[5]);
}

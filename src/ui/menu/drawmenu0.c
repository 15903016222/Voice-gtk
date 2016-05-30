/*
 * drawmenu.c
 *
 *  Created on: 2012-5-9
 *      Author: shensheng
 */

#include "../core/core.h"
#include "../../drawui.h"
#include "../../file_op.h"
#include "../../drawfb.h"
#include "../../draw_dialog.h"
#include "../../callback.h"
#include "../../main.h"
#include "../dialog/reportPreview.h"
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
#include "menu.h"
#include "../../string/_string.h"
#include "../../lzk/fileHandler.h"
#include "../../calculation/limit.h"
#include "../authorization.h"
#include "../../globalData.h"

#include <libgen.h>

void DrawMenu000()
{

}
void DrawMenu010()
{

}
void DrawMenu020()
{
	draw3_popdown (NULL, 0, 1);
	if ((pp->cstart_qty == 0)||(pp->cstart_qty == 1))
	{
		gtk_widget_set_sensitive(pp->eventbox30[0], FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[0], FALSE);
	} else if((pp->cstart_qty == 2) && (pp->cmode_pos == 3) && (pp->ctype_pos == 1)) {
		gtk_widget_set_sensitive(pp->eventbox30[0], FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[0], FALSE);
    }
/*	else if((pp->cstart_qty == 2))
	{
		if(!pp->clb_flag)
		{
			gtk_widget_set_sensitive(pp->eventbox30[0], TRUE);
			gtk_widget_set_sensitive(pp->eventbox31[0], TRUE);
		}
		else
		{
			gtk_widget_set_sensitive(pp->eventbox30[0], FALSE);
			gtk_widget_set_sensitive(pp->eventbox31[0], FALSE);
		}
	}*/
	else
	{
		gtk_widget_set_sensitive(pp->eventbox30[0], TRUE);
		gtk_widget_set_sensitive(pp->eventbox31[0], TRUE);
	}
}
void DrawMenu030()
{
	draw3_popdown (NULL, 0, 1);
	if ((pp->wstart_qty == 0)||(pp->wstart_qty == 1))
	{
		gtk_widget_set_sensitive(pp->eventbox30[0], FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[0], FALSE);
	}
	else
	{
		gtk_widget_set_sensitive(pp->eventbox30[0], TRUE);
		gtk_widget_set_sensitive(pp->eventbox31[0], TRUE);
	}
}
void DrawMenu040()
{
	if ( !con2_p[0][4][0] )
		gtk_widget_hide (pp->eventbox30[0]);
	gtk_widget_hide (pp->eventbox31[0]);
}


void DrawMenu100()
{
	double tmpf ;
	double curValue=0.0, lower, upper, step;
	int digit, pos, unit, content_pos ;
    int group = get_current_group(pp->p_config);
    int _bDbRef = get_group_db_ref (pp->p_config, group);


	switch (TMP(db_reg))
	{
		case 0:	tmpf = 0.1; break;
		case 1:	tmpf = 0.5; break;
		case 2:	tmpf = 1.0; break;
		case 3:	tmpf = 2.0; break;
		case 4:	tmpf = 6.0; break;
		default:break;
	}
	if (get_group_db_ref (pp->p_config, get_current_group (pp->p_config)))
		content_pos = 6;
	else
		content_pos = 0;
    if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0)) {
        if(_bDbRef) {
            curValue = (group_get_gain(group) - group_get_refgain(group))/100.0;
            lower = 0.0 - group_get_refgain(group)/ 100.0 ;
            upper = GetGainLimit(group) -  group_get_refgain(group)/ 100.0;
        } else {
            curValue = group_get_gain(group) / 100.0 ;
			lower = 0.0;
            upper = GetGainLimit(group);
		}

		step = tmpf;
		digit = 1;
		pos = 0;
		unit = UNIT_DB;
        draw3_digit_pressed (data_100, units[unit], curValue ,
				lower, upper, step, digit, pp, pos, content_pos);
    } else {
        if(_bDbRef) {
            curValue = (group_get_gain(group) - group_get_refgain(group))/100.0;
        } else {
            curValue = group_get_gain(group) / 100.0;
		}
		digit = 1;
		pos = 0;
		unit = UNIT_DB;
        draw3_digit_stop (curValue, units[unit], digit, pos, content_pos);
	}
}

void DrawMenu110()
{
	double tmpf ;
	double cur_value=0.0, lower, upper, step;
	int digit , pos ;
	char* str ;
	int _nGroupId = get_current_group(pp->p_config) ;
	switch (TMP(pulser_reg))
	{
		case 0:	tmpf = 1.0; break;
		case 1:	tmpf = 10.0; break;
		case 2:	tmpf = 100.0; break;
		default:break;
	}
	upper = (double)getPulserReceiverRange(_nGroupId);
	//upper = (double) (128 + 1 - LAW_VAL_POS(_nGroupId , Elem_qty));
	if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 0))
	{
		cur_value = (double) (get_group_val (&pp->p_config->group[get_current_group(pp->p_config)], GROUP_PULSER));
		lower = 1.0;
		step = tmpf;
		digit = 0;
		pos = 0;
		str = g_strdup_printf ("( 1 to %d)", (unsigned int)(upper));
		draw3_digit_pressed (data_110, str, cur_value , lower, upper, step, digit, pp, pos, 0);
	}
	else
	{
		cur_value = (double) (get_group_val (&pp->p_config->group[get_current_group(pp->p_config)], GROUP_PULSER));
		digit = 0;
		pos = 0;
		str = g_strdup_printf ("( 1 to %d)", (unsigned int)(upper));
		draw3_digit_stop (cur_value, str, digit, pos, 0);
	}
	g_free(str);
}

void DrawMenu120()
{
	double tmpf ;
	double cur_value=0.0, lower, upper, step;
	int digit , pos ;
	char* str ;
	int grp = get_current_group(pp->p_config) ;

	if((GROUP_VAL_POS(grp , group_mode)==2)||(GROUP_VAL_POS(grp , group_mode)==3))/*group_mode选择UT1 或 UT2*/
	{
		pp->x_pos = 600, pp->y_pos = 150;
		if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 0))
			draw3_pop_tt (data_1201, NULL,
					menu_content[DAMPING + get_damping_pos(pp->p_config)],
					menu_content + DAMPING, 2, 0, get_damping_pos(pp->p_config), 0);
		else
			draw3_popdown (menu_content[DAMPING + get_damping_pos(pp->p_config)], 0, 0);
		str = g_strdup_printf ("%s", con2_p[1][2][6]);
		gtk_label_set_text (GTK_LABEL (pp->label3[0]), str);
	}
	else
	{
		switch (TMP(receiver_reg))
		{
			case 0:	tmpf = 1.0; break;
			case 1:	tmpf = 10.0; break;
			case 2:	tmpf = 100.0; break;
			default:break;
		}
        if (group_get_rx_tx_mode(grp) == PULSE_ECHO )
			/* 脉冲回波模式不可以调节 */
		{
			gtk_widget_set_sensitive (pp->eventbox30[0], FALSE);
			gtk_widget_set_sensitive (pp->eventbox31[0], FALSE);
		}
		upper = (double)getPulserReceiverRange(grp);
		//upper = (double) (128 + 1 - LAW_VAL_POS(grp , Elem_qty));
		if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 0))
		{
			cur_value = (double) (get_group_val (&pp->p_config->group[get_current_group(pp->p_config)], GROUP_RECEIVER));
			lower = 1.0;
			step = tmpf;
			digit = 0;
			pos = 0;
			str = g_strdup_printf ("( 1 to %d)", (unsigned int)(upper));
			draw3_digit_pressed (data_120, str, cur_value , lower, upper, step, digit, pp, pos, 0);
		}
		else
		{
			cur_value = (double) (get_group_val (&pp->p_config->group[get_current_group(pp->p_config)], GROUP_RECEIVER));
			digit = 0;
			pos = 0;
			str = g_strdup_printf ("( 1 to %d)", (unsigned int)(upper));
			draw3_digit_stop (cur_value, str, digit, pos, 0);
		}
	}

	g_free(str);
}
void DrawMenu130()
{
	double tmpf ;
	double cur_value=0.0, lower, upper, step;
	int digit , pos , unit;
	int grp = get_current_group(pp->p_config) ;

	if (GROUP_VAL_POS( grp , group_mode) == PA_SCAN)	/* 脉冲回波模式不可以调节 */
	{
		gtk_widget_set_sensitive (pp->eventbox30[0], FALSE);
		gtk_widget_set_sensitive (pp->eventbox31[0], FALSE);
	}
	switch (TMP(scanoffset_reg))
	{
		case 0:	tmpf = 1; break;
		case 1:	tmpf = 5; break;
		case 2:	tmpf = 0.1; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
	{
		cur_value = GROUP_VAL_POS( grp , scan_offset)/10.0;
		lower = -10000.0;
		upper = 9999.0;
		step = tmpf;
		digit = 1;
		pos = 0;
		unit = UNIT_MM;
		draw3_digit_pressed (data_510, units[unit], cur_value , lower, upper, step, digit, pp, pos, 0);
	}
	else
	{
		cur_value = GROUP_VAL_POS( grp , scan_offset)/10.0;
		digit = 1;
		pos = 0;
		unit = UNIT_MM;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
	}
}
void DrawMenu140()
{
	draw3_popdown (NULL, 0, 1);
}

void DrawMenu200()
{
	int grp = get_current_group(pp->p_config) ;

	pp->x_pos = 633, pp->y_pos = 142;
	if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 0))
		draw3_pop_tt (data_200, NULL,
				menu_content[GATE_POS + GROUP_VAL_POS( grp , gate_pos)],
				menu_content + GATE_POS, 3, 0, GROUP_VAL_POS( grp , gate_pos), 0);
	else
		draw3_popdown (menu_content[GATE_POS + GROUP_VAL_POS( grp , gate_pos)], 0, 0);
}

void DrawMenu210()
{
/*	MENU_UNION menu_union = {{2 ,1 ,0 ,0}};
	if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 0))
		menuCommonPopPress(menu_union ,-1);
	else
		menuCommonPopUpdate(menu_union);//*/
}

void DrawMenu220()
{
/*	MENU_UNION menu_union = {{2 ,2 ,0 ,0}};//获取当前菜单的索引
	if ((MENU_STATUS == MENU3_PRESSED) && (CUR_POS == 0))
	{
		const popDataStruct* pPopData = NULL;
		if(popGetData(menu_union ,pPopData))
		{
			if(pPopData)
			{

				int num_of_item = OUTPUT_MAX;
			//	const char** current_label = malloc(num_of_item * sizeof(const char*));
				const char* current_label[num_of_item];
				int i;
				for(i = 0 ;i < OUTPUT_MAX ;++i)
				{
					if(0x01 == output_get_parameter(i ,OUTPUT_COMMAND_SWITCH))//
					{
						current_label[i] = menu3_get_para_label(pPopData->stringIndex)[OUTPUT_MAX + i];
					}
					else
					{
						current_label[i] = menu3_get_para_label(pPopData->stringIndex)[i];
					}
				}
				menuMallocPopPress(menu_union ,current_label ,num_of_item);
			//	free(current_label);
			}
		}
	}
	else
	{
		menuPopSetDataWithMap(menu_union ,MENU_STATUS_RELEASE);
	}//*/
}

void DrawMenu230()
{
	int grp = get_current_group(pp->p_config) ;
	int menu_status = 0 ;
	pp->x_pos = 605, pp->y_pos = 149;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
	{
		if (GROUP_VAL_POS( grp , SizingCurves.curve_pos))
			menu_status = 0;
		else
			menu_status = 0x02;

		draw3_pop_tt (data_230, NULL,
				menu_content[MODE_POS+GROUP_VAL_POS( grp , SizingCurves.mode_pos)],
				menu_content + MODE_POS, 2, 0, GROUP_VAL_POS( grp , SizingCurves.mode_pos), menu_status);
	}
	else
		draw3_popdown (menu_content[MODE_POS+GROUP_VAL_POS( grp , SizingCurves.mode_pos)], 0, 0);
}

void DrawMenu240()
{
	if ( !con2_p[2][4][0] )
		gtk_widget_hide (pp->eventbox30[0]);
	gtk_widget_hide (pp->eventbox31[0]);
}

void DrawMenu300()
{
	pp->x_pos = 398, pp->y_pos = 118-26;

	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
	{
		char listField[2][100];
		int fieldNO;
		int listNO;
		memset(listField ,0 ,200);
		for(listNO = 0 ;listNO < 2 ;++listNO)
		{
			char str[50];
			memset(str ,0 ,50);
			sprintf(str ,"%d %s : " ,listNO + 1 ,getMainDictString(MAINSTRINGDICT_WELD ,listNO));
			strcpy(listField[listNO] ,str);
			for(fieldNO = 0;fieldNO < 4 ;++fieldNO)
			{
				strcat(listField[listNO] ," ");
				strcat(listField[listNO] ,field[getFieldIndex(listNO ,fieldNO)]);
			}
		}
		const char* listField_[2];
		listField_[0] = listField[0];
		listField_[1] = listField[1];

		draw3_pop_tt (data_300, NULL,
				list1[get_reading_list(pp->p_config)],
				listField_, 2, 0, get_reading_list(pp->p_config), 0);
	}
	else
		draw3_popdown (list1[get_reading_list(pp->p_config)], 0, 0);
}
void DrawMenu310()
{
	int grp = get_current_group(pp->p_config) ;
	pp->x_pos = 569, pp->y_pos = 116-26;

	int menu_status = 0 ;
	menu_status = 0x70;
	if(GROUP_VAL_POS( grp , selection) == 4 || GROUP_VAL_POS( grp , selection) == 5 || GROUP_VAL_POS( grp , selection) == 6)
	{
			GROUP_VAL_POS( grp , selection) = 0 ;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
		draw3_pop_tt (data_310, NULL,
				menu_content[SELECTION + GROUP_VAL_POS( grp , selection)],
				menu_content + SELECTION, 8, 0, GROUP_VAL_POS( grp , selection),  menu_status );
	else
		draw3_popdown (menu_content[SELECTION + GROUP_VAL_POS( grp , selection)], 0, 0);

}
void DrawMenu320()
{
	//display table
	int show = get_display_table(pp->p_config);
	tableDisplay(show);
	//disable/enable menu
	show = !show;
	gtk_widget_set_sensitive(pp->eventbox2[0], show);
	gtk_widget_set_sensitive(pp->eventbox2[1], show);
	gtk_widget_set_sensitive(pp->eventbox2[3], show);
	gtk_widget_set_sensitive(pp->menubar, show);

	draw3_popdown (menu_content[OFF_ON + get_display_table(pp->p_config)], 0, 0);
}
void DrawMenu330()
{
	pp->x_pos = 602, pp->y_pos = 117-26;

	int grp = get_current_group(pp->p_config) ;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
		draw3_pop_tt (data_330, NULL,
				menu_content[SOURCE + GROUP_VAL_POS( grp , source)],
				menu_content + SOURCE, 9, 0, GROUP_VAL_POS( grp , source), 0);
	else
		draw3_popdown (menu_content[SOURCE + GROUP_VAL_POS( grp , source)], 0, 0);
}
void DrawMenu340()
{
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
	{
		draw_dialog_all (DIALOG_REMARK);
		if(tableHasEntry())
		{
			tableExport();
		}
	}
	else
		draw3_popdown(NULL,0,1);
	if(!tableHasEntry())
	{
		gtk_widget_set_sensitive(pp->eventbox30[0] ,FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[0] ,FALSE);
	}
}


void DrawMenu400()
{
	pp->x_pos = 415, pp->y_pos = 90;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
	{
		draw3_pop_tt (data_400, NULL,
				menu_content[DISPL + get_display_pos(pp->p_config)],
				menu_content + DISPLAY, 11, 0, get_display_pos(pp->p_config), DisplayModeValidateFlag());
	}
	else
		draw3_popdown (menu_content[DISPL + get_display_pos(pp->p_config)], 0, 0);
}
void DrawMenu410()
{
	pp->x_pos = 566, pp->y_pos = 120-26;
	int grp = get_current_group(pp->p_config);
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
		draw3_pop_tt (data_410, NULL,
				menu_content[UTUNIT + GROUP_VAL_POS( grp , ut_unit)],
				menu_content + UTUNIT, 3, 0, GROUP_VAL_POS( grp , ut_unit), 0);
	else
		draw3_popdown (menu_content[UTUNIT + GROUP_VAL_POS( grp , ut_unit)], 0, 0);
	gtk_widget_queue_draw(pp->draw_area->drawing_area);
}

void DrawMenu420()
{
	pp->x_pos = 516, pp->y_pos = 118-26;
	int grp = get_current_group(pp->p_config);
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
		draw3_pop_tt (data_430, NULL,
				menu_content[COL_SELECT + GROUP_VAL_POS( grp , col_select_pos)],
				menu_content +COL_SELECT, 3, 0, GROUP_VAL_POS( grp , col_select_pos), 0);
	else
		draw3_popdown (menu_content[COL_SELECT + GROUP_VAL_POS( grp , col_select_pos)], 0, 0);
}
void DrawMenu430()
{
	pp->x_pos = 570, pp->y_pos = 118-26;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
		draw3_pop_tt (data_440, NULL,
				menu_content[PROP_SCAN + get_dis_prop_scan(pp->p_config)],
				menu_content + PROP_SCAN, 4, 0, get_dis_prop_scan(pp->p_config), 0); // 取消掉 STRIPCHART
	else
		draw3_popdown (menu_content[PROP_SCAN + get_dis_prop_scan(pp->p_config)], 0, 0);
}
void DrawMenu440()
{
	if ( !con2_p[4][4][0] )
		gtk_widget_hide (pp->eventbox30[0]);
	gtk_widget_hide (pp->eventbox31[0]);
}

void DrawMenu500()
{
#if HIGH_POWER
	draw3_popdown (menu_content[GROUP_P + get_current_group(pp->p_config) + 1], 0, 0);
#else
	int menu_status = 0 ;
	pp->x_pos = 590, pp->y_pos = 90;	/* 位置ok */
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
	{
		switch (get_group_qty(pp->p_config))
		{
			case 1:	menu_status = 0x03fc;break;
			case 2: menu_status = 0x01f8;break;
			case 3: menu_status = 0x01f0;break;
			case 4: menu_status = 0x01e0;break;
			case 5:	menu_status = 0x01c0;break;
			case 6: menu_status = 0x0180;break;
			case 7: menu_status = 0x0100;break;
			case 8: menu_status = 0x0001;break;
			default:break;
		}
		draw3_pop_tt (data_500, NULL,
				menu_content[GROUP_P + get_current_group(pp->p_config) + 1],
				menu_content + GROUP_P, 10, 0, get_current_group(pp->p_config) + 1, menu_status);
	}
	else
		draw3_popdown (menu_content[GROUP_P + get_current_group(pp->p_config) + 1], 0, 0);
#endif
}
void DrawMenu510()
{
	double tmpf ;
	double cur_value=0.0, lower, upper, step;
	int digit , pos , unit;
	int grp = get_current_group(pp->p_config) ;

	switch (TMP(scanoffset_reg))
	{
		case 0:	tmpf = 1  ; break;
		case 1:	tmpf = 5  ; break;
		case 2:	tmpf = 0.1; break;
		default:break;
	}
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
	{
		if(UNIT_MM == get_unit(pp->p_config))
		{
			cur_value = GROUP_VAL_POS( grp , scan_offset)/10.0;
			lower = -10000.0;
			upper = 9999.0;
			step = tmpf;
			digit = 1;
			pos = 0;
			unit = UNIT_MM;
		}
		else
		{
			cur_value = GROUP_VAL_POS( grp , scan_offset)/10.0*0.03937;
			lower = -10000.0*0.03937;
			upper = 9999.0*0.03937;
			step = tmpf/100.0;
			digit = 1;
			pos = 0;
			unit = UNIT_INCH;
		}
		draw3_digit_pressed (data_510, units[unit], cur_value , lower, upper, step, digit, pp, pos, 0);
	}
	else
	{
		if(UNIT_MM == get_unit(pp->p_config))
		{
			cur_value = GROUP_VAL_POS( grp , scan_offset)/10.0;
			digit = 1;
			pos = 0;
			unit = UNIT_MM;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		}
		else
		{
			cur_value = GROUP_VAL_POS( grp , scan_offset)/10.0*0.03937;
			digit = 1;
			pos = 0;
			unit = UNIT_INCH;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		}
	}
}
void DrawMenu520()
{
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
	{
		data_520();
	}else{
	//	draw3_popdown (menu_content[OFF_ON + get_probe_fft (pp->p_config)], 0, 0);
		int group = get_current_group(pp->p_config);
		draw3_popdown (menu_content[OFF_ON + gData->fft[group].enableFft], 0, 0);
	}
}
void DrawMenu530()
{
	pp->x_pos = 608, pp->y_pos = 118-26;
	int grp = get_current_group(pp->p_config);
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
	{
		//if(get_inspec_type (pp->p_config)==2)
		//{
		//	draw3_pop_tt (data_530, NULL,
		//			menu_content[GEOMETRY + get_part_geometry(grp)],
		//			menu_content+GEOMETRY, 3, 0, get_part_geometry(grp), 0x01);
		//}
		//else
		draw3_pop_tt (data_530, NULL,
					menu_content[GEOMETRY + get_part_geometry(grp)],
					menu_content+GEOMETRY, 3, 0, get_part_geometry(grp), 0x8);
	}

	else
		draw3_popdown (menu_content[GEOMETRY + get_part_geometry(grp)], 0, 0);
}

extern GtkWidget* PartFileOpenNew(GtkWidget* fatherWidget) ;
void DrawMenu540()
{
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
	{
		//draw_dialog_all (DIALOG_FILE_OPEN);
		GtkWidget* w = PartFileOpenNew(pp->window);
        if (NULL != w) {
            gtk_widget_show(w);
            pp->pos_pos = MENU3_STOP;
        }
		draw3_popdown(NULL,0,1);
	}
	else
		draw3_popdown(NULL,0,1);
	//return ;
	//if ( !con2_p[5][4][0] )
	//	gtk_widget_hide (pp->eventbox30[0]);
	//gtk_widget_hide (pp->eventbox31[0]);
}


void DrawMenu600()
{
	int grp = get_current_group(pp->p_config) ;
	if(get_language(pp->p_config))
	{
		pp->x_pos = 570 ;
	}
	else
	{
		pp->x_pos = 490 ;
	}
	pp->y_pos = 90;
    if ((pp->pos_pos == MENU3_PRESSED)
            && (CUR_POS == 0)
            && !TMP(freeze))
		draw3_pop_tt (data_600, NULL,
				menu_content[L_CONFIG + LAW_VAL_POS(grp , Focal_type)],
				menu_content+LAW_CONFIG, 2, 0, LAW_VAL_POS(grp , Focal_type), 0xc);
	else
		draw3_popdown (menu_content[L_CONFIG + LAW_VAL_POS(grp , Focal_type)], 0, 0);
}
void DrawMenu610()
{
	double tmpf ;
	double cur_value=0.0, lower, upper, step;
	int digit , pos , unit;
	int grp = get_current_group(pp->p_config) ;
	int temp_beam ;
	switch (TMP(min_angle_reg))
	{
		case 0:	tmpf = 1; break;
		case 1:	tmpf = 10; break;
		case 2:	tmpf = 0.1; break;
		default:break;
	}
	if(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON)/* 聚焦法则自动计算开启时, Min Angle 才可调节 */
	{
        if ((pp->pos_pos == MENU3_PRESSED)
                && (CUR_POS == 0)
                && !TMP(freeze)) {
			/* 最大不能超过最大Angle_max */
			cur_value = LAW_VAL_POS(grp , Angle_min) / 100.0;
			/* 计算lower为妙 */
			temp_beam = LAW_MAX_QTY - get_beam_qty() + TMP(beam_qty[get_current_group(pp->p_config)]);
			TMP(beam_skew_num)	= (LAW_VAL_POS(grp , Angle_beam_skew_max) -
					LAW_VAL_POS(grp , Angle_beam_skew_min)) /
				LAW_VAL_POS(grp , Angle_beam_skew_step) + 1;
			temp_beam = temp_beam / TMP(beam_skew_num) - 1;
            RefractAngleRange(&lower, &upper, get_group_by_id (pp->p_config, grp), 0);
			step = tmpf;
			digit = 1;
			pos = 0;
			unit = UNIT_DEG;
			draw3_digit_pressed (data_610, units[unit], cur_value , lower,
					upper, step, digit, pp, pos, 0);
		}
		else
		{
			cur_value = LAW_VAL_POS(grp , Angle_min) / 100.0;
			digit = 1;
			pos = 0;
			unit = UNIT_DEG;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		}
	}
	else /* 聚焦法则自动计算为OFF, Min.Angle 不可调节 */
	{
		cur_value = LAW_VAL_POS(grp , Angle_min) / 100.0;
		digit = 1;
		pos = 0;
		unit = UNIT_DEG;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		gtk_widget_set_sensitive(pp->eventbox30[0],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[0],FALSE);
	}
}
void DrawMenu620()
{
	int grp = get_current_group(pp->p_config) ;
	int menu_status = 0 ;

	pp->x_pos = 555, pp->y_pos = 116 - 26;
	if(get_auto_focal (pp->p_config) == AUTO_FOCAL_ON)
	{
        if ((pp->pos_pos == MENU3_PRESSED)
                && (CUR_POS == 0)
                && !TMP(freeze)) {
            if (group_get_rx_tx_mode(grp) == PITCH_CATCH )
			{
				menu_status = 0x0c;
			}
			else if ((LAW_VAL_POS(grp , Focal_type) == DEPTH_SCAN) || (LAW_VAL_POS(grp , Focal_type) == STATIC_SCAN))
			{
				menu_status = 0x1d;//0x1c;
			}
			else
			{
				menu_status = 0x10;
			}
            if (get_part_geometry(grp) != 0) {
			    menu_status = 0x1d;
            }

			draw3_pop_tt (data_620, NULL,
					menu_content[FOCAL_POINT_TYPE1 + LAW_VAL_POS(grp , Focal_point_type)],
					menu_content+ FOCAL_POINT_TYPE, 4, 0, LAW_VAL_POS(grp , Focal_point_type), menu_status);
		}
		else
		{
			draw3_popdown (menu_content[FOCAL_POINT_TYPE1 + LAW_VAL_POS(grp , Focal_point_type)], 0, 0);
		}
	}
	else
	{
		draw3_popdown (menu_content[FOCAL_POINT_TYPE1 + LAW_VAL_POS(grp , Focal_point_type)], 0, 0);
		gtk_widget_set_sensitive(pp->eventbox30[0],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[0],FALSE);
	}
}

void DrawMenu630()
{
	double tmpf ;
	double cur_value=0.0, lower, upper, step;
	int digit , pos , unit;
	int grp = get_current_group(pp->p_config) ;

	switch (pp->p_tmp_config->element_qty_reg)
	{
		case 0:	tmpf = 1.0; break;
		case 1:	tmpf = 10.0; break;
		case 2:	tmpf = 100.0; break;
		default:break;
	}
	if (get_auto_focal (pp->p_config) == AUTO_FOCAL_ON) /* 聚焦法则自动计算为 on 时，Element Qty 才可调节 */
	{
        if ((pp->pos_pos == MENU3_PRESSED)
                && (CUR_POS == 0)
                && !TMP(freeze)) {
			cur_value = LAW_VAL_POS(grp , Elem_qty);
			lower = 1.0;
			/* 计算最大激发阵元数 */
			upper =  MIN(gMaxElementTriggerQty, GROUP_VAL_POS( grp , probe.Elem_qty));
			upper =  MIN(upper , MIN(gMaxElementRecieveQty , GROUP_VAL_POS( grp , probe.Elem_qty)) - LAW_VAL_POS(grp , First_tx_elem) + 1);
			step  = tmpf;
			digit = 0;
			pos   = 0;
			unit  = UNIT_NONE;
			draw3_digit_pressed (data_630, units[unit], cur_value , lower,
					upper, step, digit, pp, pos, 0);
		}
		else
		{
			cur_value = LAW_VAL_POS(grp , Elem_qty);
			digit = 0;
			pos = 0;
			unit = UNIT_NONE;
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		}
	}
	else /* 聚焦法则自动计算为off时，Element Qty 不可以调节 */
	{
		cur_value = LAW_VAL_POS(grp , Elem_qty);
		digit = 0;
		pos = 0;
		unit = UNIT_NONE;
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		gtk_widget_set_sensitive(pp->eventbox30[0],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[0],FALSE);
	}
}

void DrawMenu640()
{
	draw3_popdown (menu_content[OFF_ON + get_auto_focal (pp->p_config)], 0, 0);
	gtk_widget_set_sensitive(pp->eventbox30[0],FALSE);
	gtk_widget_set_sensitive(pp->eventbox31[0],FALSE);
}

void DrawMenu700()
{
	pp->x_pos = 638, pp->y_pos = 104;
	//if ((get_inspec_type (pp->p_config) == 1) || (get_inspec_type (pp->p_config) == 2))
	//{
	//	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
	//		draw3_pop_tt (data_700, NULL,
	//				menu_content[ENCODER + get_cur_encoder (pp->p_config)],
	//				menu_content+ENCODER, 2, 0, get_cur_encoder (pp->p_config), 0);
	//	else
	//		draw3_popdown (menu_content[ENCODER+get_cur_encoder (pp->p_config)], 0, 0);
	//}
	//else
	//{
		if ((get_inspec_source (pp->p_config) == 1) || (get_inspec_source (pp->p_config) == 2))
			draw3_popdown (menu_content[ENCODER+(get_inspec_source (pp->p_config)-1)], 0, 0);
		else
			draw3_popdown (menu_content[ENCODER+ 0 ], 0, 0);
		gtk_widget_set_sensitive(pp->eventbox30[0],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[0],FALSE);
	//}
}

void DrawMenu710()
{
	pp->x_pos = 546, pp->y_pos = 118-26;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
		draw3_pop_tt (data_710, NULL,
				menu_content[I_TYPE+get_inspec_type (pp->p_config)],
				menu_content+I_TYPE, 3, 0,get_inspec_type (pp->p_config), 0);
	else
		draw3_popdown (menu_content[I_TYPE+get_inspec_type (pp->p_config)], 0, 0);

	gtk_widget_set_sensitive(pp->eventbox30[0],FALSE);
	gtk_widget_set_sensitive(pp->eventbox31[0],FALSE);
}
void DrawMenu720()
{
	double tmpf ;
	double cur_value=0.0, lower, upper, step;
	int digit , pos , unit;

	switch (TMP(scan_start_reg))
	{
		case 0:	tmpf = 0.01 * get_area_scanresolution (pp->p_config); break;
		case 1:	tmpf = 0.1 * get_area_scanresolution (pp->p_config); break;
		case 2:	tmpf = 0.001 * get_area_scanresolution (pp->p_config); break;
		default:break;
	}
	if(get_inspec_type (pp->p_config)==0 || get_inspec_type (pp->p_config)==1)
		/* Inspection -> Type 选择 One-Line Scan \ Raster Scan  时 */
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
		{
			if(get_unit(pp->p_config) == UNIT_MM)
			{
				cur_value = get_area_scanstart (pp->p_config)/1000.0;
				lower = -9999.0;
                upper  = get_area_scanend (pp->p_config)/1000.0-1;
                step = tmpf;
				digit = 2;
				pos = 0;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = get_area_scanstart (pp->p_config)/1000.0*0.03937;
				lower = -9999.0*0.03937;
                upper  = get_area_scanend (pp->p_config)/1000.0 * 0.03937-0.03937;
                step = tmpf*0.03937;
				digit = 3;
				pos = 0;
				unit = UNIT_INCH;
			}
			draw3_digit_pressed (data_720, units[unit], cur_value , lower, upper, step, digit, pp, pos, 0);
		}
		else
		{
			if(get_unit(pp->p_config) == UNIT_MM)
			{
				cur_value = get_area_scanstart (pp->p_config)/1000.0;
				digit = 2;
				pos = 0;
				unit = UNIT_MM;
			}
			else
			{
				cur_value = get_area_scanstart (pp->p_config)/1000.0*0.03937;
				digit = 3;
				pos = 0;
				unit = UNIT_INCH;
			}
			draw3_digit_stop (cur_value, units[unit], digit, pos, 0);
		}
	}
	else  /* Inspection -> Type 选择 Helicoidal Scan  时 */
	{
		if(get_unit(pp->p_config) == UNIT_MM)
		{
			set_area_scanstart (pp->p_config, 0);
			digit = 2;
			pos = 0;
			unit = UNIT_MM;
		}
		else
		{
			set_area_scanstart (pp->p_config, 0);
			digit = 2;
			pos = 0;
			unit = UNIT_INCH;
		}
		draw3_digit_stop (cur_value, units[unit], digit, pos, 0);

		gtk_widget_set_sensitive(pp->eventbox30[0],FALSE);
		gtk_widget_set_sensitive(pp->eventbox31[0],FALSE);
	}
}
void DrawMenu730()
{
	pp->x_pos = 550, pp->y_pos = 118-26;
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
			draw3_pop_tt (data_730, NULL,
					menu_content[START_MODE+get_start_mode (pp->p_config)],
					menu_content+START_MODE, 3, 0,get_start_mode (pp->p_config), 0);
		else
			draw3_popdown (menu_content[START_MODE+get_start_mode (pp->p_config)], 0, 0);
}
void DrawMenu740()
{
	pp->x_pos = 550, pp->y_pos = 118-26;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
		draw3_pop_tt (data_740, NULL,
				menu_content[STORAGE+get_data_storage (pp->p_config)],
				menu_content+STORAGE, 4, 0,get_data_storage (pp->p_config), 0);
	else
		draw3_popdown (menu_content[STORAGE+get_data_storage (pp->p_config)], 0, 0);
}

void DrawMenu800()
{
	pp->x_pos = 442, pp->y_pos = 118-26;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
	{
		if(fileDetectUSB())
		{
			draw3_pop_tt (data_800, NULL,
					menu_content[F_STORAGE_P+get_file_storage (pp->p_config)],
					menu_content+F_STORAGE, 2, 0,get_file_storage (pp->p_config), 0);
		}
		else
		{
			draw3_pop_tt (data_800, NULL,
					menu_content[F_STORAGE_P+get_file_storage (pp->p_config)],
					menu_content+F_STORAGE, 3, 0,get_file_storage (pp->p_config), 0);
		}
	}
	else
		draw3_popdown (menu_content[F_STORAGE_P+get_file_storage (pp->p_config)], 0, 0);
}

extern GtkWidget *report_file_dialog(GtkWidget *parent);

void DrawMenu810()
{
	pp->x_pos = 502, pp->y_pos = 100;
    if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0)) {
        GtkWidget* w = report_file_dialog(pp->window);
        gtk_dialog_run(GTK_DIALOG(w));

        pp->pos_pos = MENU3_STOP;
        if (report_preview_get_tmpl()) {
            draw3_popdown (basename(report_preview_get_tmpl()), 0, 0);
        } else {
            draw3_popdown (NULL, 0, 0);
        }
//		draw3_pop_tt (data_810, NULL,
//				menu_content[TEMPLA_P+get_report_template (pp->p_config)],
//				menu_content+TEMPLA, 1, 0,get_report_template (pp->p_config), 0);
    } else {
//        draw3_popdown (menu_content[TEMPLA_P+get_report_template (pp->p_config)], 0, 0);
        if (report_preview_get_tmpl()) {
            draw3_popdown (basename(report_preview_get_tmpl()), 0, 0);
        } else {
            draw3_popdown (NULL, 0, 0);
        }
    }
}

void DrawMenu820()
{
	draw3_popdown (menu_content[OFF_ON + get_report_format_userfield (pp->p_config)], 0, 0);
}

void DrawMenu830()
{
	pp->x_pos = 630, pp->y_pos = 118-26;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
		draw3_pop_tt (data_830, NULL,
				menu_content[F_SELECT+get_report_userfield_select (pp->p_config)],
				menu_content+F_SELECT, 10, 0,get_report_userfield_select (pp->p_config), 0);
	else
		draw3_popdown (menu_content[F_SELECT+get_report_userfield_select (pp->p_config)], 0, 0);
}

void DrawMenu840()
{
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
	{
		//draw_dialog_all (DIALOG_EDIT_NOTES);
		char* str = NULL;
		str = dialogGetAString( pp->window
							    ,con2_p[8][4][0]
		                        ,get_edit_notes_info(pp->p_config));
		if(str)
		{
			set_edit_notes_info(pp->p_config, str);
			free(str);
		}
	}
	else
		draw3_popdown(NULL,0,1);
}

void DrawMenu900()
{
	pp->x_pos = 570, pp->y_pos = 118-26;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
		draw3_pop_tt (data_900, NULL,
				menu_content[P_UNITS+get_unit(pp->p_config)],
				menu_content+P_UNITS, 2, 0,get_unit(pp->p_config), 0);
	else
		draw3_popdown (menu_content[P_UNITS+get_unit(pp->p_config)], 0, 0);
}
void DrawMenu910()
{
    char buffer[32] = {0};
    time_t curtime = core_time();
    struct tm *loctime = localtime(&curtime);
	strftime (buffer, 32, "%F %T", loctime);

    if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0)) {
		draw_dialog_all (DIALOG_TIME);
    } else {
		draw3_popdown (buffer + 11, 0, 0);
    }
}
void DrawMenu920()
{
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
		draw_dialog_all(DIALOG_SYSTEM_INFO);
	else
		draw3_popdown(NULL,0,1);
}
void DrawMenu930()
{
	struct ifreq ifr;
	char ip_temp[256];
	int inet_sock;
	char temp[52];
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == 0))
	{
		draw_dialog_all (DIALOG_IP);
	}
	else
	{
		inet_sock = socket(AF_INET, SOCK_DGRAM, 0);
		strcpy(ifr.ifr_name, "usb0");
		ioctl(inet_sock, SIOCGIFADDR, &ifr);
		sprintf(ip_temp,"%s\n", inet_ntoa(((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr));
		gtk_label_set_text (GTK_LABEL (pp->data3[0]), ip_temp);
	}

	g_sprintf (temp,"%s", con2_p[9][3][0]);
	/* 设置label */
	gtk_label_set_text (GTK_LABEL (pp->label3[0]), temp);
	/* 显示和隐藏控件 */
	//gtk_widget_set_sensitive(pp->eventbox30[0] , FALSE );
	//gtk_widget_set_sensitive(pp->eventbox31[0] , FALSE );
	gtk_widget_show (pp->eventbox30[0]);
	gtk_widget_show (pp->eventbox31[0]);
	gtk_widget_show (pp->data3[0]);
}
void DrawMenu940()
{
}





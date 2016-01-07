/*
 * keyHandler.c
 *
 *  Created on: 2012-9-18
 *      Author: wolflord
 */
#include "../drawui.h"
#include "../base_config.h"
#include "../base.h"
#include "../main.h"
#include "../calibration.h"
#include "../callback.h"
#include <math.h>
#include <gdk/gdkkeysyms.h>
#include <stdio.h>
#include "../ui/menu/menuFunc.h"
#include "../ui/ui.h"
#include "../ui/menu/drawmenu0.h"

extern	gboolean data_function0 (GtkWidget *widget,	GdkEventButton *event,	gpointer data);
#define B3Fun(x) data_function0(NULL ,NULL ,x)
void B3func(int num);

#define menu_wizard_restart() \
	{ \
		wizardGroupHandler_setStep(0); \
		wizardFocallawHandler_setStep(0); \
		gCurrentMenuPos.pos.menu4 = 0; \
		menu3Update(); \
		pp->help_yn = FALSE; \
		showHelp(gCurrentMenuPos ,pp->help_yn); \
	}

void ReturnCallback(guint keyval, gpointer data)
{
	unsigned char tmp = pp->pos_pos;
	unsigned char tmp1 = pp->mark_pop_change;
	guint	data1 = (GPOINTER_TO_UINT (data));

	//add by lzk, do not spin step
	gReturnButtonRelease = FALSE;

	if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
	{
		main_menu_pop(MENU_ENTER);/*选中当前主菜单，并收回主菜单条*/
	}
	else if((MENU31_PRESS == data1)||(MENU32_PRESS == data1)||(MENU33_PRESS == data1)||(MENU34_PRESS == data1)||(MENU35_PRESS == data1)||(MENU36_PRESS == data1))/*当三级菜单中有弹出菜单选项时*/
	{
		menu3_pop(MENU3_ENTER);/*选中当前三级菜单，并收回三级菜单条*/
	}
	else/*当主菜单条 三级菜单条 都处于收回状态时*/
	{
		switch (pp->pos_pos)
		{
			case MENU2_STOP:
				pp->pos_pos = MENU2_PRESSED;
				pp->pos_pos = MENU3_STOP;
				switch (pp->pos1[pp->pos])
				{
					case 0:
						b2_fun0(pp,0);
						break;
					case 1:
						b2_fun0(pp,1);
						break;
					case 2:
						b2_fun0(pp,2);
						break;
					case 3:
						b2_fun0(pp,3);
						break;
					case 4:
						b2_fun0(pp,4);
						break;
					default:break;
				}
				break;
			case MENU2_PRESSED:
				pp->pos_pos = MENU3_STOP;
				break;
			case MENU3_STOP:
				if(!pp->help_yn)
				{
					if(gtk_widget_get_sensitive(pp->eventbox30[CUR_POS]))
					{
						pp->pos_pos = MENU3_PRESSED;
					}
					else
						pp->pos_pos = MENU3_STOP;
					/* 按下的动作在这里实现 */
				}
				else
				{
					if(gCurrentMenuPos.pos.menu1 == 0)
					{
						if((gCurrentMenuPos.pos.menu2 == 0)//wizard group
						||(gCurrentMenuPos.pos.menu2 == 1))//wizard law
						{
							if(gtk_widget_get_sensitive(pp->eventbox30[CUR_POS]))
							{
								pp->pos_pos = MENU3_PRESSED;
							}
							else
								pp->pos_pos = MENU3_STOP;
						}
					}
				}
				break;
			case MENU3_PRESSED:
					pp->pos_pos = MENU3_STOP;
					int cur_pos = CUR_POS;
					B3func(cur_pos);
					pp->pos_pos = MENU3_STOP;
				break;
			default:break;
		}
		if ((tmp != pp->pos_pos) || (tmp1 != pp->mark_pop_change))
		{
			draw_menu2(0);
			draw_menu3(0, NULL);
			tttmp = gtk_spin_button_get_value (GTK_SPIN_BUTTON (pp->sbutton[CUR_POS]));
		}
	}
}


void KeyResponseForEsc(int data)
{
	unsigned char tmp  = pp->pos_pos;
	unsigned char tmp1 = pp->mark_pop_change;

	if (pp->win_keyboard)
	{
		gtk_widget_destroy (pp->win_keyboard);
		pp->win_keyboard = NULL;
		return;
	}

	if (MAIN_MENU_PRESS == data)/*当主菜单条处于被弹出状态时*/
	{
		main_menu_pop(MENU_HIDE);/*收回主菜单条*/
		return ;
	}
	else if((MENU31_PRESS == data)||(MENU32_PRESS == data)||(MENU33_PRESS == data)||(MENU34_PRESS == data)||(MENU35_PRESS == data)||(MENU36_PRESS == data))/*当三级菜单中有弹出菜单选项时*/
	{
		menu3_pop(MENU3_HIDE);/*隐藏三级菜单的弹出菜单*/
		return ;
	}
	else/*当主菜单条处于收回状态时*/
	{
		if ( pp->mark_pop_change == 1)
			pp->mark_pop_change = 0;
		switch (pp->pos_pos)
		{
			case MENU2_STOP:
				break;
			case MENU2_PRESSED:
				pp->pos_pos = MENU2_STOP;
				break;
			case MENU3_STOP:
				pp->pos_pos = MENU2_STOP;
				break;
			case MENU3_PRESSED:
				gtk_spin_button_set_value (GTK_SPIN_BUTTON (pp->sbutton[CUR_POS]), tttmp);
				pp->pos_pos = MENU3_STOP;
				draw_menu2(0);
				draw_menu3(0, NULL);
				return ;
				break;
			default:break;
		}
		if ((tmp != pp->pos_pos) || (tmp1 != pp->mark_pop_change))
		{
			draw_menu2(0);
			draw_menu3(0, NULL);
		}
	}
	if((pp->pos==0)&&(pp->pos1[pp->pos]==0))
	{
			pp->pos1[pp->pos]=0;
			pp->start_qty = 0;
			menu_wizard_restart();
			gtk_widget_set_sensitive(pp->eventbox2[0],TRUE);
			gtk_widget_set_sensitive(pp->eventbox2[1],TRUE);
			gtk_widget_set_sensitive(pp->eventbox2[2],TRUE);
			gtk_widget_set_sensitive(pp->eventbox2[3],TRUE);
			gtk_widget_set_sensitive(pp->menubar,TRUE);
			draw_menu3(0, NULL);
	}
	else if((pp->pos==0)&&(pp->pos1[pp->pos]==1))
	{
			pp->pos1[pp->pos]=1;
			pp->fstart_qty = 0;
			menu_wizard_restart();
			gtk_widget_set_sensitive(pp->eventbox2[0],TRUE);
			gtk_widget_set_sensitive(pp->eventbox2[1],TRUE);
			gtk_widget_set_sensitive(pp->eventbox2[2],TRUE);
			gtk_widget_set_sensitive(pp->eventbox2[3],TRUE);
			gtk_widget_set_sensitive(pp->menubar,TRUE);
			draw_menu3(0, NULL);
	}
	else if((pp->pos==0)&&(pp->pos1[pp->pos]==3))
	{
			menu_wizard_restart();
			gtk_widget_set_sensitive(pp->eventbox2[1],TRUE);
			pp->pos1[pp->pos]=3;
			pp->wstart_qty = 1;
			gtk_widget_set_sensitive(pp->eventbox2[0],TRUE);
			gtk_widget_set_sensitive(pp->eventbox2[2],TRUE);
			gtk_widget_set_sensitive(pp->eventbox2[3],TRUE);
			gtk_widget_set_sensitive(pp->menubar,TRUE);
			draw_menu3(0, NULL);
	}
	else if (pp->help_yn)
	{
		pp->help_yn = !pp->help_yn;
		showHelp(gCurrentMenuPos ,pp->help_yn);
		return;
	}

	if(pp->clb_flag)
	{
			esc_calibration();
			draw_menu3(0, NULL);
	}
	if(get_display_table(pp->p_config))
	{
			set_display_table (pp->p_config, 0); /* P320 */
			DrawMenu320();
	}
}

#include "../lzk/output_spi.h"
void StartPause()
{
    gint enc;
    guchar encType;

    TMP(freeze) = !TMP(freeze);

    if(TMP(freeze))
    {
        FreezingFPGA(TRUE) ;
        update_widget_bg(pp->event[19], /*backpic[12]*/ 15);
        setKeyInvalidateWhenDataRecalling(FALSE);
    }
    else
    {
        if(TMP(dataRecalling))
        {// if is recalling data , set recalling end
            setKeyInvalidateWhenDataRecalling(TRUE);
            if (TMP(loadData)) {
                TMP(loadData) = 0;
                RefreshScanInfor();
                draw_area_all();
                pp->bRefreshDraw = TRUE ;
            }
        }
        update_widget_bg(pp->event[19], /*backpic[12]*/ 16);
        FreezingFPGA(FALSE) ;
    }


    enc = (gint)get_inspec_source(pp->p_config);
    if (enc != 0) {
        encType = get_enc_type(pp->p_config, enc-1);
        if (TMP(freeze)) {
            encType |= 4;
        } else {
            encType &= 11;
        }
        set_enc_type(pp->p_config, encType, enc-1);
        if (enc == 1) {
            output_set_parameter(0, OUTPUT_OTHER_COMMAND_ENCODE_X, encType, 0);
        } else if ( enc == 2) {
            output_set_parameter(0, OUTPUT_OTHER_COMMAND_ENCODE_Y, encType, 0);
        }
        output_write_one_reg_to_spi(0, OUTPUT_OTHER_COMMAND_ENCODE_Y);
    }


    if((pp->pos == 7 && pp->pos1[7] == 3)|| (pp->pos == 9 && pp->pos1[7] == 4)
            || (pp->pos == 3 && pp->pos1[3] == 1)) {
		draw_menu3(0 , NULL) ;
	}
}


int isTableInShowing(int keyval)
{
	int ret = FALSE;
	if(gtk_widget_get_sensitive(pp->menubar)) return ret;
	switch(keyval)
	{
	case GDK_KP_0:	/* 选中 P310 cursors 这个位置 */
	case GDK_KP_1:	/* 冻结 */
	case GDK_KP_2:	/* 选中 Calibration 这个位置 */
	case GDK_KP_3:	/* 选中 P100 Gain 这个位置 */
	case GDK_KP_4:	/* 选中 P801 Open 这个位置 */
	case GDK_KP_5:	/* 选中 P200 Gate 这个位置 */
	case GDK_KP_6:	/* scan 开始 (stop is not available)*/
	case GDK_KP_7:	/* 保存 */
	case GDK_KP_8:	/* 选中 P140 Set 80% 这个位置 */
	case GDK_KP_9:	/* 选中 P400 Display 这个位置 */
	case GDK_KP_Subtract:  /* +/- */
	//case GDK_KP_Decimal: //GDK_KP_Divide:	/* 擦除 */
	case GDK_Super_L:
	case GDK_F1:
	case GDK_F2:
	case GDK_F3:
	case GDK_F5:
		ret = TRUE ;
        break;
	default:
		break;
	}

	return ret;
}

int isHelpInShowing(int nKeyValue_)
{
	int ret = FALSE ;
	if(!pp->help_yn) return ret;

	switch (nKeyValue_)
	{
		case GDK_KP_0:	/* 选中 P310 cursors 这个位置 */
		case GDK_KP_1:	/* 冻结 */
		case GDK_KP_2:	/* 选中 Calibration 这个位置 */
		case GDK_KP_3:	/* 选中 P100 Gain 这个位置 */
		case GDK_KP_4:	/* 选中 P801 Open 这个位置 */
		case GDK_KP_5:	/* 选中 P200 Gate 这个位置 */
		case GDK_KP_6:	/* scan 开始 (stop is not available)*/
		case GDK_KP_7:	/* 保存 */
		case GDK_KP_8:	/* 选中 P140 Set 80% 这个位置 */
		case GDK_KP_9:	/* 选中 P400 Display 这个位置 */
		case GDK_KP_Subtract:  /* +/- */
		case GDK_KP_Decimal: //GDK_KP_Divide:	/* 擦除 */
		//case GDK_Super_L:
//		case GDK_Escape:
//		case GDK_Return:	/*回车键*/
			ret =  TRUE;
			break;
		default:
			break;
	}
	return ret ;
}

int isCalibrationInProcessing(int nKeyValue_)
{
	int ret = FALSE ;
	if(!pp->clb_flag) return ret;

	switch (nKeyValue_)
	{
		case GDK_KP_0:	/* 选中 P310 cursors 这个位置 */
		case GDK_KP_1:	/* 冻结 */
		case GDK_KP_2:	/* 选中 Calibration 这个位置 */
		case GDK_KP_3:	/* 选中 P100 Gain 这个位置 */
		case GDK_KP_4:	/* 选中 P801 Open 这个位置 */
		case GDK_KP_5:	/* 选中 P200 Gate 这个位置 */
		case GDK_KP_6:	/* scan 开始 (stop is not available)*/
		case GDK_KP_7:	/* 保存 */
		case GDK_KP_8:	/* 选中 P140 Set 80% 这个位置 */
		case GDK_KP_9:	/* 选中 P400 Display 这个位置 */
		case GDK_Super_L:
		case GDK_F1:
		case GDK_F2:
		case GDK_F3:
		case GDK_F4:
		case GDK_F5:
		case GDK_F6:
			ret =  TRUE;
			break;
		case GDK_KP_Subtract:  /* +/- */
			if(pp->ctype_pos != 2) ret = TRUE ;
			break;
		default:
			break;
	}
	return ret ;
}

int isDataRecallInProcessing(int nKeyValue_)
{
	int ret = FALSE ;
	if(!TMP(dataRecalling)) return ret;

	switch (nKeyValue_)
	{
	case GDK_KP_2:	/* 选中 Calibration 这个位置 */
	case GDK_KP_3:	/* 选中 P100 Gain 这个位置 */
	case GDK_KP_4:	/* 选中 P801 Open 这个位置 */
	case GDK_KP_5:	/* 选中 P200 Gate 这个位置 */
	case GDK_KP_6:	/* scan 开始 (stop is not available)*/
	case GDK_KP_7:	/* 保存 */
	case GDK_KP_8:	/* 选中 P140 Set 80% 这个位置 */
	case GDK_KP_Decimal: //GDK_KP_Divide:	/* 擦除 */
	case GDK_F1:
		ret = TRUE ;
        break;
	default:
		break;
	}
	return ret ;
}

static int checkKeyval(int keyval)
{

	if(isHelpInShowing(keyval))
	{
		return TRUE;
	}
	if(isCalibrationInProcessing(keyval))
	{
		return TRUE;
	}
	if(isDataRecallInProcessing(keyval))
	{
		return TRUE;
	}
	if(isTableInShowing(keyval))
	{
		return TRUE;
	}
	return FALSE ;
}
int handler_key(int keyval, void* data)
{
	unsigned char tmp  = pp->pos_pos;
	unsigned char tmp1 = pp->mark_pop_change;
	guint	data1 = (GPOINTER_TO_UINT (data));

	guint group = get_current_group(pp->p_config);
	int BeamNo ;
	double current_angle;

	if(checkKeyval(keyval))
	{
		return FALSE;
	}

	switch (keyval)
	{
		case GDK_KP_0:	/* 选中 P310 cursors 这个位置 */
			if(get_overlay_cursor (pp->p_config))
			{
				set_overlay_cursor (pp->p_config, 0);
				gtk_widget_queue_draw (pp->vboxtable);
			}
			else
			{
				if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
				{
					main_menu_pop(MENU_HIDE);/*收回主菜单条*/
				}
				pp->pos = 3;
				pp->pos1[pp->pos] = 1;
				CUR_POS = 0;
				pp->pos_pos = MENU3_STOP ;
				set_overlay_cursor (pp->p_config, 1);
				gtk_widget_queue_draw (pp->vboxtable);
				draw_menu1();
				draw_menu2(0);
				draw_menu3(0, NULL);
			}
			break;
		case GDK_KP_1:	/* 冻结 */
			StartPause();
			break;
		case GDK_KP_2:	/* 选中 Calibration 这个位置 */
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_HIDE);/*收回主菜单条*/
			}
			pp->pos = 0;
			pp->pos1[pp->pos] = 2;
			//CUR_POS = 0;
			pp->pos_pos = MENU2_PRESSED;
			draw_menu1();
			draw_menu2(0);
			draw_menu3(0, NULL);
			break;
		case GDK_KP_3:	/* 选中 P100 Gain 这个位置 */
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_HIDE);/*收回主菜单条*/
			}
			else if((MENU31_PRESS == data1)||(MENU32_PRESS == data1)
					||(MENU33_PRESS == data1)||(MENU34_PRESS == data1)
					||(MENU35_PRESS == data1)||(MENU36_PRESS == data1))/*当三级菜单中有弹出菜单选项时*/
			{
				menu3_pop(MENU3_HIDE);/*隐藏三级菜单的弹出菜单*/
			}
			pp->pos = 1;
			pp->pos1[pp->pos] = 0;
			CUR_POS = 0;
			pp->pos_pos = MENU3_PRESSED;
			draw_menu1();
			draw_menu2(0);
			draw_menu3(0, NULL);
			break;
		case GDK_KP_4:	/* 选中 P801 Open 这个位置 */
			pp->pos = 8;
			pp->pos1[pp->pos] = 0;
			CUR_POS = 1;
			pp->pos_pos = MENU3_PRESSED;
			draw_menu1();
			draw_menu2(0);
			draw_menu3(0, NULL);
			break;
		case GDK_KP_5:	/* 选中 P200 Gate 这个位置 */
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_HIDE);/*收回主菜单条*/
			}
			pp->pos = 2;
			pp->pos1[pp->pos] = 0;
			CUR_POS = 2;
			pp->pos_pos = MENU3_PRESSED;
			draw_menu1();
			draw_menu2(0);
			draw_menu3(0, NULL);
			break;
		case GDK_KP_6:	/* scan 开始 (stop is not available)*/
			data_732 (NULL, NULL) ;
			break;
		case GDK_KP_7:	/* 保存 */
			SaveDataProcess("Saving Data!") ;
			break;
		case GDK_KP_8:	/* 选中 P140 Set 80% 这个位置 */
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_HIDE);/*收回主菜单条*/
			}
			pp->pos = 1;
			pp->pos1[pp->pos] = 4;
			CUR_POS = 0;
			pp->pos_pos = MENU3_STOP;
			draw_menu1();
			draw_menu2(0);
			draw_menu3(0, NULL);
			data_140 (NULL, (gpointer)pp);
			break;
		case GDK_KP_9:	/* 选中 P400 Display 这个位置 */
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_HIDE);/*收回主菜单条*/
			}
			pp->pos = 4;
			pp->pos1[pp->pos] = 0;
			CUR_POS = 0;
			pp->pos_pos = MENU3_PRESSED;
			draw_menu1();
			draw_menu2(0);
			draw_menu3(0, NULL);
			break;
		case GDK_KP_Subtract:  /* +/- */
#if 0
			(TMP(beam_num[group]) < (TMP(beam_qty[group]) - 1))	?
				(TMP(beam_num[group]) += 1)	: (TMP(beam_num[group]) = 0);
			if(LAW_VAL_POS(group , Focal_type) == 0)
			{
				BeamNo = pp->p_tmp_config->beam_num[group];
				current_angle = LAW_VAL_POS(group , Angle_min)/100.0 + BeamNo * LAW_VAL_POS(group , Angle_step)/100.0 ;
				TMP(current_angle[group]) = current_angle * G_PI / 180.0 ;
			}
			pp->pos_pos = MENU3_STOP;
			draw_menu3(0, NULL);
			DrawDisplayWindowFrame() ;
#endif
            GROUP_VAL_POS(group, selection) = 3;
            pp->pos = 3;
            pp->pos1[pp->pos] = 1;
            CUR_POS = 1;
            pp->pos_pos = MENU3_PRESSED;
            draw_menu1();
            draw_menu2(0);
            draw_menu3(0, NULL);
			break;
		case GDK_KP_Decimal: //GDK_KP_Divide:	/* 擦除 */
			ascanResetEnvelope(get_current_group(pp->p_config)) ;
			ascanResetPeak(get_current_group(pp->p_config)) ;
			RefreshScanInfor();
			pp->bRefreshDraw = TRUE ;
			break;

		case GDK_Super_L:
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_CHANGE);/*被选中的主菜单轮流切换*/
			}
			else/*当主菜单条处于收回状态时*/
			{
				menu3_pop(MENU3_HIDE);/*隐藏三级菜单的弹出菜单*/
				main_menu_pop(MENU_POP);/*弹出主菜单条并选中当前选项*/
				if ((tmp != pp->pos_pos) || (tmp1 != pp->mark_pop_change))
				{
					draw_menu2(0);
					draw_menu3(0, NULL);
				}
			}
			break;
		case GDK_Escape:
             KeyResponseForEsc(data1);
			break;
		case GDK_Return:	/*回车键*/
			ReturnCallback(keyval, data) ;
			break;
		case GDK_F1:
			pp->help_yn = !pp->help_yn;
			showHelp(gCurrentMenuPos ,pp->help_yn);
			break;
		case GDK_F2:
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_HIDE);/*收回主菜单条*/
			}
			if(gtk_widget_get_sensitive(pp->eventbox2[0]))
			{
				menu3_pop(MENU3_HIDE);
				if (pp->pos_pos != MENU2_PRESSED)
					pp->pos_pos = MENU2_PRESSED;
				b2_fun0(pp,0);
			}
			break;
		case GDK_F3:
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_HIDE);/*收回主菜单条*/
			}
			if(gtk_widget_get_sensitive(pp->eventbox2[1]))
			{
				menu3_pop(MENU3_HIDE);
				if (pp->pos_pos != MENU2_PRESSED)
					pp->pos_pos = MENU2_PRESSED;
				b2_fun0(pp,1);
			}
			break;
		case GDK_F4:
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_HIDE);/*收回主菜单条*/
			}
			if(gtk_widget_get_sensitive(pp->eventbox2[2]))
			{
				menu3_pop(MENU3_HIDE);
				if (pp->pos_pos != MENU2_PRESSED)
					pp->pos_pos = MENU2_PRESSED;
				b2_fun0(pp,2);
			}
			break;
		case GDK_F5:
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_HIDE);/*收回主菜单条*/
			}
			if(gtk_widget_get_sensitive(pp->eventbox2[3]))
			{
				menu3_pop(MENU3_HIDE);
				if (pp->pos_pos != MENU2_PRESSED)
					pp->pos_pos = MENU2_PRESSED;
				b2_fun0(pp,3);
			}
			break;
		case GDK_F6:
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_HIDE);/*收回主菜单条*/
			}
			if(gtk_widget_get_sensitive(pp->eventbox2[4]))
			{
				menu3_pop(MENU3_HIDE);
				if (pp->pos_pos != MENU2_PRESSED)
					pp->pos_pos = MENU2_PRESSED;
				b2_fun0(pp,4);
			}
			break;
		case GDK_F7:
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_HIDE);/*收回主菜单条*/
			}
			if(gtk_widget_get_sensitive(pp->eventbox30[5]))
			{
				if (MENU36_PRESS == data1)
					menu3_pop(MENU3_TURN);
				else
				{
					menu3_pop(MENU3_HIDE);
				//	b3_fun5(NULL);
					B3Fun((gpointer)5);
				}
			}
			break;
		case GDK_F8:
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_HIDE);/*收回主菜单条*/
			}
			if(gtk_widget_get_sensitive(pp->eventbox30[4]))
			{
				if (MENU35_PRESS == data1)
					menu3_pop(MENU3_TURN);
				else
				{
					menu3_pop(MENU3_HIDE);
				//	b3_fun4(NULL);
					B3Fun((gpointer)4);
				}
			}
			break;
		case GDK_F9:
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_HIDE);/*收回主菜单条*/
			}
			if(gtk_widget_get_sensitive(pp->eventbox30[3]))
			{
				if (MENU34_PRESS == data1)
					menu3_pop(MENU3_TURN);
				else
				{
					menu3_pop(MENU3_HIDE);
				//	b3_fun3(NULL);
					B3Fun((gpointer)3);
				}
			}
			break;
		case GDK_F10:
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_HIDE);/*收回主菜单条*/
			}
			if(gtk_widget_get_sensitive(pp->eventbox30[2]))
			{
				if (MENU33_PRESS == data1)
				{
					menu3_pop(MENU3_TURN);
				}
				else
				{
					menu3_pop(MENU3_HIDE);
				//	b3_fun2(NULL);
					B3Fun((gpointer)2);
				}
			}
			break;
		case GDK_F11:
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_HIDE);/*收回主菜单条*/
			}
			if(gtk_widget_get_sensitive(pp->eventbox30[1]))
			{
				if (MENU32_PRESS == data1)
					menu3_pop(MENU3_TURN);
				else
				{
					menu3_pop(MENU3_HIDE);
				//	b3_fun1(NULL);
					B3Fun((gpointer)1);
				}
			}
			break;
		case GDK_F12:
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_HIDE);/*收回主菜单条*/
			}
			if(gtk_widget_get_sensitive(pp->eventbox30[0]))
			{
				if (MENU31_PRESS == data1)
					menu3_pop(MENU3_TURN);
				else
				{
					menu3_pop(MENU3_HIDE);
				//	b3_fun0(pp);
					B3Fun((gpointer)0);
				}
			}
			break;
		case GDK_Left:
		case GDK_Up:
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_UP);/*被选中的主菜单向上切换*/
			}
			else if((MENU31_PRESS == data1)||(MENU32_PRESS == data1)||(MENU33_PRESS == data1)||(MENU34_PRESS == data1)||(MENU35_PRESS == data1)||(MENU36_PRESS == data1))/*当三级菜单中有弹出菜单选项时*/
			{
				menu3_pop(MENU3_UP);/*被选中的三级菜单向上切换选项*/
			}
			else/*当主菜单条处于收回状态 且 没有三级菜单弹出选项 时*/
			{
				switch (pp->pos_pos)
				{
					case MENU2_STOP:
						pp->pos_last1 = pp->pos1[pp->pos];
						pp->pos1[pp->pos] > 0 ? pp->pos1[pp->pos]-- :  (pp->pos1[pp->pos] = (pp->menu2_qty - 1));
						draw_menu2(0);
						draw_menu3(1, NULL);
						break;
					case MENU2_PRESSED:
						break;
					case MENU3_STOP:	/*向上轮流切换三级菜单*/
						if(pp->menu3_amount != 0)  /*当三级菜单不全为黑时*/
						{
							pp->menu3_start = 0;
							while(!(gtk_widget_get_sensitive(pp->eventbox30[pp->menu3_start])))
								pp->menu3_start++;	/*找出第一个可选的三级菜单位置*/

							if(CUR_POS > pp->menu3_start)
							{
								CUR_POS--;
								while(!(gtk_widget_get_sensitive(pp->eventbox30[CUR_POS])))
									CUR_POS--;
							}

							else if(CUR_POS == pp->menu3_start)
							{
								CUR_POS = pp->menu3_amount-1;
							}
							draw_menu3(1, NULL);
						}
						else if(pp->menu3_amount == 0)	/*当三级菜单全部为黑掉时*/
							break;
						break;

					case MENU3_PRESSED:
						break;
				}
			}
			break;
		case GDK_Right:
		case GDK_Down:
			if (MAIN_MENU_PRESS == data1)/*当主菜单条处于被弹出状态时*/
			{
				main_menu_pop(MENU_DOWN);/*被选中的主菜单向下切换*/
			}
			else if((MENU31_PRESS == data1)||(MENU32_PRESS == data1)||(MENU33_PRESS == data1)||(MENU34_PRESS == data1)||(MENU35_PRESS == data1)||(MENU36_PRESS == data1))/*当三级菜单中有弹出菜单选项时*/
			{
				menu3_pop(MENU3_TURN);/*被选中的三级菜单向下切换*/
			}
			else/*当主菜单条处于收回状态时*/
			{
				switch (pp->pos_pos)
				{
					case MENU2_STOP:
						pp->pos_last1 = pp->pos1[pp->pos];
						pp->pos1[pp->pos] < (pp->menu2_qty - 1) ? pp->pos1[pp->pos]++ :  (pp->pos1[pp->pos] = 0);
						draw_menu2(0);
						draw_menu3(1, NULL);
						break;
					case MENU2_PRESSED:
						break;
					case MENU3_STOP:	/*向下轮流切换三级菜单*/
						if(pp->menu3_amount != 0)  /*当三级菜单不全为黑时*/
						{
							pp->menu3_start = 0;
							while(!(gtk_widget_get_sensitive(pp->eventbox30[pp->menu3_start])))
								pp->menu3_start++;	/*找出第一个可选的三级菜单位置*/

							if(CUR_POS < pp->menu3_amount - 1)
							{
								CUR_POS++;
								while(!(gtk_widget_get_sensitive(pp->eventbox30[CUR_POS])))
									CUR_POS++;
							}

							else if(CUR_POS == pp->menu3_amount - 1)
							{
								CUR_POS = pp->menu3_start;
							}
							draw_menu3(1, NULL);
						}
						else if (pp->menu3_amount == 0)	/*当三级菜单全部为黑掉时*/
							break;
						break;

					case MENU3_PRESSED:
						break;
				}
			}
			break;
		default:break;
	}
	return 0;
}

/*
 * update.c
 *
 *  Created on: 2012-10-17
 *      Author: lzk
 */


#include <gtk/gtk.h>
#include "../../drawui.h"
#include "menuFunc.h"
#include "../../string/_string.h"
#include "../../map/menuMap.h"

/*
void menu940(MENU_UNION menu_union)//9 ,4 ,0 ,0
{
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
	{
		gCurrentMenuPos.key = menu_union.key;
		menuSetDataWithMap(menu_union ,LABELBUTTON_STATUS_PRESS);

		if(menu4FuncTrans(menu_union ,NULL))
		{
			pp->pos_pos = MENU3_STOP;
			draw_menu2(0);
			draw_menu3(0, NULL);
		}
	}
	else
	{
		menuSetDataWithMap(menu_union ,LABELBUTTON_STATUS_RELEASE);
	}
}

void menu941(MENU_UNION menu_union)//9 ,4 ,1 ,0
{
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
	{
		gCurrentMenuPos.key = menu_union.key;
		menuSetDataWithMap(menu_union ,LABELBUTTON_STATUS_PRESS);

		if(menu4FuncTrans(menu_union ,NULL))
		{
			pp->pos_pos = MENU3_STOP;
			draw_menu2(0);
			draw_menu3(0, NULL);
		}
	}
	else
	{
		menuSetDataWithMap(menu_union ,LABELBUTTON_STATUS_RELEASE);
	}
}//*/

void menu942(MENU_UNION menu_union)//9 ,4 ,2 ,0
{
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
	{
		gCurrentMenuPos.key = menu_union.key;
		menuSetDataWithMap(menu_union ,LABELBUTTON_STATUS_PRESS);

		if(menu4FuncTrans(menu_union ,NULL))
		{
			char* markup ;
			markup = g_markup_printf_escaped ("<span foreground='white' font_desc='10'>%s</span>" ,"default.cfg");
				gtk_label_set_markup (GTK_LABEL (pp->label[2]), markup);
			g_free(markup);

			pp->pos_pos = MENU3_STOP;
			draw_menu2(0);
			draw_menu3(0, NULL);
		}
	}
	else
	{
		menuSetDataWithMap(menu_union ,LABELBUTTON_STATUS_RELEASE);
	}
	if(TMP(dataRecalling))
	{
		gtk_widget_set_sensitive (pp->eventbox30[2] , FALSE);
		gtk_widget_set_sensitive (pp->eventbox31[2] , FALSE);
	}

}

const static gchar *authors[] = {
    "Jake Yang <yanghuanjie@cndoppler.cn>",
    NULL
};

static void phascan_about()
{
    GtkWidget *dialog = gtk_about_dialog_new();

    gtk_window_set_decorated(GTK_WINDOW(dialog), FALSE);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

    gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "About Phascan");

    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), APP_NAME);

    gchar *version = g_strdup_printf("%d.%d.%d", APP_MAJOR, APP_MINOR, APP_MICRO);
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), version);
    g_free(version);

    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "CopyRight©2010-2016 Doppler All rights reserved");
    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "PHASCAN has a powerful detection capability, can achieve a variety of scanning mode and focus mode, which greatly improves the detection reliability.");

    gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "http://www.cndoppler.cn");
    gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(dialog), authors);

    GdkPixbuf *logo = gdk_pixbuf_new_from_file(LOGO_FILE, NULL);
    gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), logo);

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void menu943(MENU_UNION menu_union)//9 ,4 ,3 ,0
{
    if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3)) {
        gCurrentMenuPos.key = menu_union.key;

        phascan_about();

    } else {
        menuSetDataWithMap(menu_union, LABELBUTTON_STATUS_RELEASE);
    }
}

void menuCommon(MENU_UNION menu_union)
{
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
	{
		gCurrentMenuPos.key = menu_union.key;
		menuSetDataWithMap(menu_union ,LABELBUTTON_STATUS_PRESS);

		if(menu4FuncTrans(menu_union ,NULL))
		{
			pp->pos_pos = MENU3_STOP;
			draw_menu2(0);
			draw_menu3(0, NULL);
		}
	}
	else
	{
		menuSetDataWithMap(menu_union ,LABELBUTTON_STATUS_RELEASE);
	}
}

#include "../../lzk/tofd.h"

void menu315(MENU_UNION menu_union)
{
    int grp = get_current_group(pp->p_config);
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
	{
		gCurrentMenuPos.key = menu_union.key;

		spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
		if(pMenuData)
		{
			if(menu3Repeat())
			{
				if(fabs(1.0 - pMenuData->step) < 0.0001)   			pMenuData->step = 5.0;
				else if(fabs(5.0 - pMenuData->step) < 0.0001)		pMenuData->step = 0.1;
				else if(fabs(0.1 - pMenuData->step) < 0.0001)		pMenuData->step = 0.5;
				else				    							pMenuData->step = 1.0;
			}
			if(get_inspec_source (pp->p_config) == 0)
			{
                pMenuData->current_value = gTofdS[grp].refLine;
				double rate =  GROUP_VAL_POS(0 , prf1) * get_area_scanresolution (pp->p_config) / 10000.0;
				pMenuData->current_value /= rate ;
				pMenuData->lower = -1000;
				pMenuData->upper = 1000;
				pMenuData->unit = UNITS_S;
				pMenuData->digit = 1;
			}
			else
			{
                pMenuData->current_value = gTofdS[grp].refLine;
				pMenuData->lower = -10000;
				pMenuData->upper = 10000;
				pMenuData->unit = UNITS_MM;
				pMenuData->digit = 3;
			}
			menuSpinPress(menu_union);
		}
		else
		{
			menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_PRESS);
		}

	}
	else
	{
		spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
		if(pMenuData)
		{
			if(get_inspec_source (pp->p_config) == 0)
			{
                pMenuData->current_value = gTofdS[grp].refLine;
				double rate =  GROUP_VAL_POS(0 , prf1) * get_area_scanresolution (pp->p_config) / 10000.0;
				pMenuData->current_value /= rate ;
				pMenuData->unit = UNITS_S;
			}
			else
			{
                pMenuData->current_value = gTofdS[grp].refLine;
				pMenuData->unit = UNITS_MM;
			}
			menuSpinRelease(menu_union);
		}
		else
		{
			menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
		}
	}
}

void menu341(MENU_UNION menu_union)//3 ,4 ,1 ,0
{
	menuDataStruct* pMenuData = (menuDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)//用于找数据,函数
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
		{
			gCurrentMenuPos.key = menu_union.key;
			menuSetDataWithMap(menu_union ,LABELBUTTON_STATUS_PRESS);

			if(menu4FuncTrans(menu_union ,NULL))
			{
				pp->pos_pos = MENU3_STOP;
				draw_menu2(0);
				draw_menu3(0, NULL);
			}
		}
		else
        {
            int grp = get_current_group(pp->p_config);

            pMenuData->data = gTofdS[grp].straighteningEnable;//更新需要读取数据
			menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE
					,getMainLabelString(menu_union)
					,getMainDictString(MAINSTRINGDICT_SWITCH ,pMenuData->data));
		}
	}
}

void menu342(MENU_UNION menu_union)//3 ,4 ,2 ,0
{
	menuDataStruct* pMenuData = (menuDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)//用于找数据,函数
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
		{
			gCurrentMenuPos.key = menu_union.key;
			menuSetDataWithMap(menu_union ,LABELBUTTON_STATUS_PRESS);

			if(menu4FuncTrans(menu_union ,NULL))
			{
				pp->pos_pos = MENU3_STOP;
				draw_menu2(0);
				draw_menu3(0, NULL);
			}
		}
		else
		{
            int grp = get_current_group(pp->p_config);
            pMenuData->data = gTofdS[grp].removeLateralWaveEnable;//更新需要读取数据
			menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE
					,getMainLabelString(menu_union)
					,getMainDictString(MAINSTRINGDICT_SWITCH ,pMenuData->data));
		}
	}
}

void menu343(MENU_UNION menu_union)//3 ,4 ,3 ,0
{
	int grp = get_current_group(pp->p_config);
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
	{
		gCurrentMenuPos.key = menu_union.key;
		spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
		if(pMenuData)
		{
			if(menu3Repeat())
			{
				if(fabs(1.0 - pMenuData->step) < 0.0001)
					pMenuData->step = 5.0;
				else if(fabs(5.0 - pMenuData->step) < 0.0001)
					pMenuData->step = 0.1;
				else if(fabs(0.1 - pMenuData->step) < 0.0001)
					pMenuData->step = 0.5;
				else
					pMenuData->step = 1.0;
			}
            if(TOFD == group_get_rx_tx_mode(grp)) {
    			pMenuData->current_value = GROUP_VAL_POS(grp , field_distance[250]);
            } else {
    			pMenuData->current_value = 0;
            }
			pMenuData->lower = 0;
			pMenuData->upper = 10000;
			pMenuData->unit = UNITS_MM;
			pMenuData->digit = 1;
			menuSpinPress(menu_union);
		}
		else
		{
			menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_PRESS);
		}
	}
	else
	{
		spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
		if(pMenuData)
		{
			pMenuData->unit = UNITS_MM;
            if(TOFD == group_get_rx_tx_mode(grp)) {
    			pMenuData->current_value = GROUP_VAL_POS(grp , field_distance[250]);
            } else {
    			pMenuData->current_value = 0;
            }
			menuSpinRelease(menu_union);
		}
		else
		{
			menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
		}
	}
}

void menu344(MENU_UNION menu_union)//3 ,4 ,4 ,0
{
	int group = get_current_group(pp->p_config);
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
	{
		gCurrentMenuPos.key = menu_union.key;
		menuSetDataWithMap(menu_union ,LABELBUTTON_STATUS_PRESS);

		if(menu4FuncTrans(menu_union, NULL))
		{
			pp->pos_pos = MENU3_STOP;
			draw_menu2(0);
			draw_menu3(0, NULL);
		}
	} else {
	    menuSetLabel(menu_union.pos.menu3, LABELBUTTON_MODE_DATA3,
                LABELBUTTON_STATUS_RELEASE, getMainLabelString(menu_union),
                getMainDictString(MAINSTRINGDICT_SWITCH,
                    (int)GROUP_VAL_POS(group , field_distance[252])));
	}
}

void menu345(MENU_UNION menu_union)//3 ,4 ,5 ,0
{
	int grp = get_current_group(pp->p_config);
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
	{
		gCurrentMenuPos.key = menu_union.key;
		spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
		if(pMenuData)
		{
			if(menu3Repeat())
			{
				//if(pMenuData->step == 1.0) 			pMenuData->step = 10.0;
				//else if(pMenuData->step == 10.0)	pMenuData->step = 0.01;
				//else if(pMenuData->step == 0.01)	pMenuData->step = 0.1;
				//else								pMenuData->step = 1.0;
				if(1.0 == pMenuData->step)   	pMenuData->step = 5.0;
				else if(5.0 == pMenuData->step)	pMenuData->step = 0.1;
				else if(0.1 == pMenuData->step)	pMenuData->step = 0.5;
				else				    		pMenuData->step = 1.0;
			}
			pMenuData->current_value = GROUP_VAL_POS(grp , field_distance[255]);
			pMenuData->lower = 0;
			pMenuData->upper = 10000;
			pMenuData->unit = UNITS_MM;
			pMenuData->digit = 1;
			menuSpinPress(menu_union);
		}
		else
		{
			menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_PRESS);
		}
	}
	else
	{
		spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
		if(pMenuData)
		{
			pMenuData->unit = UNITS_MM;
			pMenuData->current_value = GROUP_VAL_POS(grp , field_distance[255]);
			menuSpinRelease(menu_union);
		}
		else
		{
			menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
		}
	}
}

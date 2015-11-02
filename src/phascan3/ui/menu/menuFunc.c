/*
 * menuFunc.c
 *
 *  Created on: 2012-5-4
 *      Author: lzk
 */
#include <gtk/gtk.h>
#include "../../drawui.h"
#include "../../uihandler/menu/menuHandler.h"
#include "../labelbutton.h"
#include "../../string/_string.h"
#include "../../map/menuMap.h"

#include <pango/pango-types.h>
#include <pango/pango.h>
#include <assert.h>
static GtkWidget* menu4;
MENU_UNION gCurrentMenuPos = {{1 ,0 ,0 ,0}};//保存点击的位置和次数等菜单3按键信息
MENU_UNION gLastMenuPos = {{0 ,0 ,0 ,0}};//保存点击的位置和次数等菜单3按键信息
int gReturnButtonRelease = TRUE;
void menuLabelInit()
{
//	PangoFontDescription *font_desc = pango_font_description_new();
	PangoFontDescription *font_desc = pango_font_description_from_string("Sans 9");
	pango_font_description_set_size (font_desc, 9 * PANGO_SCALE);
	gtk_widget_modify_font (pp->data3[0], font_desc);
	gtk_widget_modify_font (pp->data3[1], font_desc);
	gtk_widget_modify_font (pp->data3[2], font_desc);
	gtk_widget_modify_font (pp->data3[3], font_desc);
	gtk_widget_modify_font (pp->data3[4], font_desc);
	gtk_widget_modify_font (pp->data3[5], font_desc);

	pango_font_description_set_size (font_desc, 10 * PANGO_SCALE);
	gtk_widget_modify_font (pp->label3[0], font_desc);
	gtk_widget_modify_font (pp->label3[1], font_desc);
	gtk_widget_modify_font (pp->label3[2], font_desc);
	gtk_widget_modify_font (pp->label3[3], font_desc);
	gtk_widget_modify_font (pp->label3[4], font_desc);
	gtk_widget_modify_font (pp->label3[5], font_desc);

	pango_font_description_free(font_desc);

}

void menuSetBackground(int menu3_pos ,LABELBUTTON_MODE_ENUM menu3_mode ,LABELBUTTON_STATUS_ENUM menu_status)
{
	switch(menu_status)
	{
	case LABELBUTTON_STATUS_PRESS:
		update_widget_bg (pp->eventbox30[menu3_pos], /*backpic[8]*/ 6);
		if(LABELBUTTON_MODE_BIGMENU == menu3_mode)
		{
			update_widget_bg (pp->eventbox31[menu3_pos], /*backpic[14]*/ 14);
		}else{
			update_widget_bg (pp->eventbox31[menu3_pos], /*backpic[14]*/ 11);
		}
		break;
	case LABELBUTTON_STATUS_RELEASE:
		update_widget_bg (pp->eventbox30[menu3_pos], /*backpic[8]*/ 8);
		if(LABELBUTTON_MODE_BIGMENU == menu3_mode)
		{
			update_widget_bg (pp->eventbox31[menu3_pos], /*backpic[14]*/ 14);
		}else{
			update_widget_bg (pp->eventbox31[menu3_pos], /*backpic[14]*/ 11);
		}
		break;
	case LABELBUTTON_STATUS_NORMAL:
	default:
		update_widget_bg (pp->eventbox30[menu3_pos], /*backpic[7]*/ 7);
		if(LABELBUTTON_MODE_BIGMENU == menu3_mode)
		{
			update_widget_bg (pp->eventbox31[menu3_pos], /*backpic[13]*/ 13);
		}else{
			update_widget_bg (pp->eventbox31[menu3_pos], /*backpic[13]*/ 10);
		}
		break;
	}
}

void menuModeSetBigMenu(int menu3_pos ,const char* label3)
{
	char label[256];
	sprintf(label ,"\n\n%s" ,label3);
	gtk_label_set_label(GTK_LABEL (pp->label3[menu3_pos]), label);
//	gtk_label_set_label(GTK_LABEL (pp->data3[menu3_pos]), "");
	/* 显示和隐藏控件 */
	gtk_widget_show (pp->eventbox30[menu3_pos]);
	gtk_widget_show (pp->scale_drawarea);

	gtk_widget_show (pp->eventbox31[menu3_pos]);

	gtk_widget_hide (pp->data3[menu3_pos]);
	gtk_widget_hide (pp->sbutton[menu3_pos]);
	gtk_widget_hide (pp->button_add);
	gtk_widget_hide (pp->button_sub);
	gtk_widget_hide (pp->vscale);
}

void menuModeSetData(int menu3_pos ,const char* label3 ,const char* data3)
{
	gtk_label_set_label(GTK_LABEL (pp->label3[menu3_pos]), label3);
	gtk_label_set_label(GTK_LABEL (pp->data3[menu3_pos]), data3);
	/* 显示和隐藏控件 */
	gtk_widget_show (pp->eventbox30[menu3_pos]);
	gtk_widget_show (pp->eventbox31[menu3_pos]);
	gtk_widget_show (pp->scale_drawarea);
	gtk_widget_show (pp->data3[menu3_pos]);

	gtk_widget_hide (pp->sbutton[menu3_pos]);
	gtk_widget_hide (pp->button_add);
	gtk_widget_hide (pp->button_sub);
	gtk_widget_hide (pp->vscale);
}

void menuModeSetSpin(int menu3_pos ,const char* label3)
{
	gtk_label_set_label(GTK_LABEL (pp->label3[menu3_pos]), label3);
	/* 显示和隐藏控件 */
	gtk_widget_show (pp->eventbox30[menu3_pos]);
	gtk_widget_show (pp->sbutton[menu3_pos]);
	gtk_widget_show (pp->button_add);
	gtk_widget_show (pp->button_sub);
	gtk_widget_show (pp->vscale);

	gtk_widget_hide (pp->eventbox31[menu3_pos]);
	gtk_widget_hide (pp->scale_drawarea);

	gtk_widget_grab_focus (pp->sbutton[menu3_pos]);
}

void menuModeSetHide(int menu3_pos)
{
	gtk_widget_show (pp->scale_drawarea);

	gtk_widget_hide(pp->eventbox30[menu3_pos]);
	gtk_widget_hide(pp->eventbox31[menu3_pos]);
//	gtk_widget_hide (pp->data3[menu3_pos]);
	gtk_widget_hide (pp->sbutton[menu3_pos]);
	gtk_widget_hide (pp->button_add);
	gtk_widget_hide (pp->button_sub);
	gtk_widget_hide (pp->vscale);
}

void menuModeSetDisable(int menu3_pos)
{
	gtk_widget_show (pp->scale_drawarea);

	gtk_widget_set_sensitive(pp->eventbox30[menu3_pos] ,FALSE);
	gtk_widget_set_sensitive(pp->eventbox31[menu3_pos] ,FALSE);
	gtk_widget_show(pp->eventbox30[menu3_pos]);
	gtk_widget_show(pp->eventbox31[menu3_pos]);
//	gtk_widget_hide (pp->data3[menu3_pos]);
	gtk_widget_hide (pp->sbutton[menu3_pos]);
	gtk_widget_hide (pp->button_add);
	gtk_widget_hide (pp->button_sub);
	gtk_widget_hide (pp->vscale);
}

//menu3 4种显示模式，大图标，带参数的图标，设置时的图标，和隐藏
void menuSpinCallback(GtkSpinButton *spinbutton, gpointer data);
void menuSetLabel(int menu3_pos ,LABELBUTTON_MODE_ENUM menu3_mode
						,LABELBUTTON_STATUS_ENUM menu_status ,const char* label3 ,const char* data3)
{
	assert(menu3_mode < LABELBUTTON_MODE_MAX);
	g_signal_handlers_disconnect_by_func(G_OBJECT(pp->sbutton[menu3_pos]) , menuSpinCallback ,NULL);
	switch(menu_status)
	{
	case LABELBUTTON_STATUS_HIDE:
		menuModeSetHide(menu3_pos);
		return;

	case LABELBUTTON_STATUS_RELEASE:
		if ((CUR_POS != menu3_pos) || (pp->pos_pos != MENU3_STOP))
		{
			menu_status = LABELBUTTON_STATUS_NORMAL;
		}
		break;

	default:
		break;
	}
	switch(menu3_mode)
	{
	case LABELBUTTON_MODE_BIGMENU:
		menuModeSetBigMenu(menu3_pos ,label3);
		menuSetBackground(menu3_pos ,menu3_mode ,menu_status);
		break;
	case LABELBUTTON_MODE_DATA3:
		menuModeSetData(menu3_pos ,label3 ,data3);
		menuSetBackground(menu3_pos ,menu3_mode ,menu_status);
		break;
	case LABELBUTTON_MODE_SPINBUTTON:
		g_signal_connect(G_OBJECT(pp->sbutton[menu3_pos]), "value-changed", G_CALLBACK(menuSpinCallback), NULL);
		menuSetBackground(menu3_pos ,menu3_mode ,menu_status);
		menuModeSetSpin(menu3_pos ,label3);
		break;
	case LABELBUTTON_MODE_DISABLE:
		menuModeSetDisable(menu3_pos);
		break;
	case LABELBUTTON_MODE_NONE:
	case LABELBUTTON_MODE_HIDE:
	default:
		menuModeSetHide(menu3_pos);
		break;
	}
}

void popCallback(GtkMenuItem *menuitem, gpointer data)
{
	popDataStruct* ppopData = (popDataStruct*)getMenuDataAddress(gCurrentMenuPos);
	if(ppopData)
	{
		ppopData->lastPopPos = GPOINTER_TO_INT(data);
	}
	menu4FuncTrans(gCurrentMenuPos ,&data);
	if(NULL != menu4)
	{
//		gtk_menu_popdown(GTK_MENU(menu4));
		gtk_widget_destroy(menu4);
		menu4 = NULL;
	}
	pp->pos_pos = MENU3_STOP;
	draw_menu2(0);
	draw_menu3(0, NULL);
}

void popPosFunc(GtkMenu   *menu,
				 gint      *x,
				 gint      *y,
				 gboolean  *push_in,
				 gpointer	user_data)
 {
	popStruct* para = (popStruct*) user_data;
	*x = ((GtkAllocation)para->allocation).x;
	*y = ((GtkAllocation)para->allocation).y;
	*push_in = para->push_in;
 }

#include <gdk/gdkkeysyms.h>
gboolean popKeyhandler (GtkWidget* pWidget ,GdkEventKey* pEvent ,gpointer data)
{
	GtkWidget* menu = GTK_WIDGET(data);
	unsigned int key = (int)g_object_get_data(G_OBJECT(menu) ,"key");
	if(key == pEvent->keyval)
	{
		if(NULL != menu4)
		{
			gtk_widget_destroy(menu4);
			menu4 = NULL;
			return TRUE;
		}
	}
	switch(pEvent->keyval)
	{
	case GDK_Left:
		fakekey_press_keysym(pp->fk, XK_Up, 0);
		fakekey_release(pp->fk);
		return TRUE;

	case GDK_Right:
		fakekey_press_keysym(pp->fk, XK_Down, 0);
		fakekey_release(pp->fk);
		return TRUE;
	}
	return FALSE;
}

static void popup(popStruct* para)
{
	int i;
	GtkWidget* menu = menu4;
	GtkWidget* menu_item ,*current_menu_item = NULL;
	//根据主父窗口调整坐标
	int x,y;
	gtk_window_get_position((GtkWindow *) pp->window, &x, &y);
	para->allocation.x += x;
	para->allocation.y += y;

	GtkRequisition requisition;
	if(NULL != menu4)
	{
		gtk_widget_destroy(menu4);
		menu4 = NULL;
	}
	menu = gtk_menu_new();
	menu4 = menu;
	g_object_set_data(G_OBJECT(menu4) ,"key" ,(gpointer)para->popKey);
//生成菜单项
	for (i = 0 ;i < para->num_of_item ;++i)
	{
		menu_item = gtk_menu_item_new_with_label (para->label[i]);
		gtk_menu_shell_append (GTK_MENU_SHELL(menu), menu_item);
		g_signal_connect (menu_item, "activate",
				G_CALLBACK(popCallback), (GUINT_TO_POINTER (i)));
		if (0x01 & (para->item_status >> i))
			gtk_widget_set_sensitive (menu_item ,FALSE);
		gtk_widget_show (menu_item);
		if(i == para->current_pos)
		{
			current_menu_item = menu_item;
		}

	}
//调整位置
	gtk_widget_size_request(menu ,&requisition);
	para->allocation.x -= requisition.width;//pop up to the left
//弹出菜单
	gtk_menu_popup (GTK_MENU(menu), NULL, NULL,
			(GtkMenuPositionFunc)popPosFunc,
			para,
			0,
			gtk_get_current_event_time());
//默认选中位置
	if(NULL != current_menu_item)
	{
		gtk_menu_shell_select_item(GTK_MENU_SHELL(menu),current_menu_item);
	}
//额外按键处理
	g_signal_connect (menu, "key-press-event",
			G_CALLBACK(popKeyhandler) ,(gpointer)menu4);
}

void menuPopdownWithMap(MENU_UNION menu_union ,	int lastPopPos)
{
	popDataStruct* pPopData = (popDataStruct*)getMenuDataAddress(menu_union);
	if(pPopData)
	{
		const char* label3 = getMainLabelString(menu_union);
		const char* data3 = NULL;

		if(pPopData->stringIndex2)
		{
			if(lastPopPos >= 0)
				pPopData->lastPopPos = lastPopPos;
			data3 = getMainDictString(pPopData->stringIndex2 ,pPopData->lastPopPos);
		}//*/

		menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE ,label3 ,data3);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

void menuPopupWithMap(MENU_UNION menu_union ,int y)
{
	popStruct popmenu_para;//弹出菜单参数
	//获取pop数据
	popDataStruct* pPopData = (popDataStruct*)getMenuDataAddress(menu_union);
	if(pPopData)
	{
		//弹出菜单
		int size = getMainDictSize(pPopData->stringIndex1);
		const char* str[size];
		getMainDictStrings(pPopData->stringIndex1 ,str ,size);

		popmenu_para.current_pos = pPopData->lastPopPos;//上次弹出菜单显示的位置
		popmenu_para.item_status = pPopData->itemStatus;
		popmenu_para.label = str;
		popmenu_para.num_of_item = size;
		popmenu_para.push_in = FALSE;
		gtk_widget_get_allocation(pp->eventbox30[menu_union.pos.menu3] ,&popmenu_para.allocation);
		//个别弹出菜单的位置调整
		if(	y >= 0)
			popmenu_para.allocation.y = y;
		switch(menu_union.pos.menu3)
		{
		case 0:
			popmenu_para.popKey = GDK_F12;
			break;
		case 1:
			popmenu_para.popKey = GDK_F11;
			break;
		case 2:
			popmenu_para.popKey = GDK_F10;
			break;
		case 3:
			popmenu_para.popKey = GDK_F9;
			break;
		case 4:
			popmenu_para.popKey = GDK_F8;
			break;
		case 5:
			popmenu_para.popKey = GDK_F7;
			break;
		}
		popup(&popmenu_para);
		//显示label
		const char* label3 = getMainLabelString(menu_union);
		const char*	data3 = NULL;

		if(pPopData->stringIndex2)
		{
			data3 = getMainDictString(pPopData->stringIndex2 ,pPopData->lastPopPos);
		}//*/

		menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_PRESS ,label3 ,data3);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_PRESS);
	}
}

//显示具体字符串数组中的图标
void menuPopdownWithMalloc(MENU_UNION menu_union ,const char** label ,int num_of_item)
{
	//获取pop数据
	popDataStruct* pPopData = (popDataStruct*)getMenuDataAddress(menu_union);
	if(pPopData)
	{
		const char* label3 = getMainLabelString(menu_union);
		const char*	data3 = label[pPopData->lastPopPos];
		menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE ,label3 ,data3);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

//显示具体字符串数组中的菜单
void menuPopupWithMalloc(MENU_UNION menu_union ,const char** label ,int num_of_item)
{
	//获取pop数据
	popDataStruct* pPopData = (popDataStruct*)getMenuDataAddress(menu_union);
	if(pPopData)
	{
		//弹出菜单
		popStruct popmenu_para;//弹出菜单参数
		assert(pPopData->lastPopPos < num_of_item);
		popmenu_para.current_pos = pPopData->lastPopPos;//上次弹出菜单显示的位置
		popmenu_para.item_status = pPopData->itemStatus;
		popmenu_para.label = label;
		popmenu_para.num_of_item = num_of_item;
		popmenu_para.push_in = FALSE;
		gtk_widget_get_allocation(pp->eventbox30[menu_union.pos.menu3] ,&popmenu_para.allocation);
		switch(menu_union.pos.menu3)
		{
		case 0:
			popmenu_para.popKey = GDK_F12;
			break;
		case 1:
			popmenu_para.popKey = GDK_F11;
			break;
		case 2:
			popmenu_para.popKey = GDK_F10;
			break;
		case 3:
			popmenu_para.popKey = GDK_F9;
			break;
		case 4:
			popmenu_para.popKey = GDK_F8;
			break;
		case 5:
			popmenu_para.popKey = GDK_F7;
			break;
		}
		popup(&popmenu_para);
		//显示label
	//	const char* label3 = getMainLabelString(menu_union);
	//	const char*	data3 = label[pPopData->lastPopPos];
	//	menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_PRESS ,label3 ,data3);
	}
	menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_PRESS);
}

void menuSetDataWithMap(MENU_UNION menu_union ,LABELBUTTON_STATUS_ENUM menu_status)
{
	menuDataStruct* pMenuData = (menuDataStruct*)getMenuDataAddress(menu_union);
	LABELBUTTON_MODE_ENUM labelMode = LABELBUTTON_MODE_BIGMENU;
	const char* label3 = getMainLabelString(menu_union);
	const char*	data3 = NULL;
	if(pMenuData)
	{
		if(pMenuData->stringIndex1)
		{
			data3 = getMainDictString(pMenuData->stringIndex1 ,pMenuData->data);
			labelMode = LABELBUTTON_MODE_DATA3;
		}//*/
	}
	menuSetLabel(menu_union.pos.menu3 ,labelMode ,menu_status ,label3 ,data3);
}

void menuSpinCallback(GtkSpinButton *spinbutton, gpointer data)
{
	float value = gtk_spin_button_get_value(spinbutton);
	spinDataStruct* pSpinData = (spinDataStruct*)getMenuDataAddress(gCurrentMenuPos);
	if(pSpinData)
	{
		pSpinData->current_value = value;
	}
	menu4FuncTrans(gCurrentMenuPos ,&value);
//	pp->pos_pos = MENU3_STOP;
//	draw_menu2(0);
//	draw_menu3(0, NULL);
}

void menuSpinConfig(MENU_UNION menu_union ,const spinDataStruct* pSpinData)
{
	int menu3_pos = menu_union.pos.menu3;
	float multiplier = 1.0;
	if( (UNITS_MM == pSpinData->unit)
	|| (UNITS_INCH == pSpinData->unit)
	)
	{
		if(UNIT_INCH == get_unit(pp->p_config))
		{
			multiplier = 0.03937;
		}
	}
//	g_signal_handlers_disconnect_by_func(G_OBJECT(pp->sbutton[menu3_pos]) ,menuSpinCallback ,NULL);
//	g_signal_connect(G_OBJECT(pp->sbutton[menu3_pos]), "value-changed", G_CALLBACK(menuSpinCallback), NULL);
	/* 设置值的范围 */
	pp->adj = gtk_spin_button_get_adjustment (GTK_SPIN_BUTTON (pp->sbutton[menu3_pos]));
	gtk_adjustment_configure (pp->adj, pSpinData->current_value * multiplier, pSpinData->lower * multiplier
							,pSpinData->upper * multiplier ,pSpinData->step * multiplier ,10.0 ,0.0);
	gtk_spin_button_set_digits (GTK_SPIN_BUTTON (pp->sbutton[menu3_pos]), pSpinData->digit);
	/* 设置scale */
	gtk_range_set_adjustment (GTK_RANGE (pp->vscale), pp->adj);
	gtk_range_set_inverted (GTK_RANGE (pp->vscale), TRUE);
	gtk_scale_set_draw_value (GTK_SCALE (pp->vscale), FALSE);
}

//单独更新特定的data的图标，该图标用来显示spin的数值
void menuSpinSetData(MENU_UNION menu_union)
{
	spinDataStruct* pSpinData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pSpinData)
	{
		float multiplier = 1.0;
		if( (UNITS_MM == pSpinData->unit)
		|| (UNITS_INCH == pSpinData->unit)
		)
		{
			if(UNIT_INCH == get_unit(pp->p_config))
			{
				multiplier = 0.03937;
			}
		}
		char* data3 = NULL;
		if(pSpinData->digit)//带小数点
		{
			data3 = g_strdup_printf("%0.*f"
					,pSpinData->digit
					,pSpinData->current_value * multiplier);
		}else{//不带小数点显示
			data3 = g_strdup_printf("%0.*f"
					,pSpinData->digit
					,pSpinData->current_value * multiplier);
		}
		gtk_label_set_label(GTK_LABEL (pp->data3[menu_union.pos.menu3]), data3);
		if(data3)
		{
			free(data3);
		}
	}
}

void menuSpinPress(MENU_UNION menu_union)
{
	gCurrentMenuPos.key = menu_union.key;

	//按下时先更新，因为spin按下后不会更新
//	pp->pos_pos = MENU3_STOP;
//	draw_menu2(0);
//	draw_menu3(0, NULL);
//	pp->pos_pos = MENU3_PRESSED;

	spinDataStruct* pSpinData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pSpinData)
	{
		char* label3 = NULL;
		const char* unit = "";
		float multiplier = 1.0;
		if( (UNITS_MM == pSpinData->unit)
		|| (UNITS_INCH == pSpinData->unit)
		)
		{
			if(UNIT_MM == get_unit(pp->p_config))
			{
				unit = getUnit(UNITS_MM);
			}
			else
			{
				unit = getUnit(UNITS_INCH);
				multiplier = 0.03937;
			}
		}
		else
		{
			unit = getUnit(pSpinData->unit);
		}

		menuSpinConfig(menu_union ,pSpinData);
		if(pSpinData->digit)//显示带小数点的情况
		{
			label3 = g_strdup_printf ("%s\n%s Δ%0.*f"//label3
					,getMainLabelString(menu_union)
					,unit
					,pSpinData->digit
					,pSpinData->step * multiplier);
		}
		else
		{//不带小数点的情况
			label3 = g_strdup_printf ("%s\n(%d - %d) Δ%0.*f"//label3
					,getMainLabelString(menu_union)
					,(uint)(pSpinData->lower * multiplier)
					,(uint)(pSpinData->upper * multiplier)
					,pSpinData->digit
					,pSpinData->step * multiplier);
		}
		menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_SPINBUTTON ,LABELBUTTON_STATUS_PRESS ,label3 ,"");
		change_keypress_event (KEYPRESS_MAIN_SPINBUTTON);
		if(label3)
		{
			free(label3);
		}
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_SPINBUTTON ,LABELBUTTON_STATUS_PRESS);
	}
}

void menuSpinRelease(MENU_UNION menu_union)
{
	spinDataStruct* pSpinData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pSpinData)
	{
		const char* unit = "";
		float multiplier = 1.0;
		if( (UNITS_MM == pSpinData->unit)
		|| (UNITS_INCH == pSpinData->unit)
		)
		{
			if(UNIT_MM == get_unit(pp->p_config))
			{
				unit = getUnit(UNITS_MM);
			}
			else
			{
				unit = getUnit(UNITS_INCH);
				multiplier = 0.03937;
			}
		}
		else
		{
			unit = getUnit(pSpinData->unit);
		}


		char* label3 = NULL;
		char* data3 = NULL;
		if(pSpinData->digit)//显示带小数点的情况
		{
			label3 = g_strdup_printf("%s\n%s"//label3
						,getMainLabelString(menu_union)
						,unit);
			data3 = g_strdup_printf("%0.*f"
					,pSpinData->digit
					,pSpinData->current_value * multiplier);
		}else{//不带小数点的情况
			label3 = g_strdup_printf ("%s\n(%d - %d)"//label3
					,getMainLabelString(menu_union)
					,(uint)(pSpinData->lower * multiplier)
					,(uint)(pSpinData->upper * multiplier)
					);
			data3 = g_strdup_printf("%0.*f"
					,pSpinData->digit
					,pSpinData->current_value * multiplier);
		}
		menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE ,label3 ,data3);
		change_keypress_event (KEYPRESS_MAIN);
		if(data3)
		{
			free(data3);
		}
		if(label3)
		{
			free(label3);
		}
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_SPINBUTTON ,LABELBUTTON_STATUS_RELEASE);
	}
}

void showHelp(MENU_UNION menu_union ,int enable);
void menu2Update()
{
	pp->pos_pos = MENU3_STOP;
	draw_menu2(0);
	draw_menu3(0, NULL);
	showHelp(gCurrentMenuPos ,pp->help_yn);
}

void menu3Update()
{
	pp->pos_pos = MENU3_STOP;
//	draw_menu2(0);
	draw_menu3(0, NULL);
	showHelp(gCurrentMenuPos ,pp->help_yn);
}


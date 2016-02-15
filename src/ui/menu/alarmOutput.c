/*
 * alarmOutput.c
 *
 *  Created on: 2012-2-24
 *      Author: lzk
 */

#include "../../map/menuMap.h"
#include "../../lzk/common.h"

#include <gtk/gtk.h>
#include <assert.h>
#include "../../base.h"
#include "../../drawui.h"
#include "../../main.h"
#include "../../lzk/output_spi.h"
#include "menuFunc.h"
#include "../../string/_string.h"
//alarm
void menu210(MENU_UNION menu_union)//2 ,1 ,0 ,0
{
	popDataStruct* pMenuData = (popDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)//用于找数据,函数
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
		{
			gCurrentMenuPos.key = menu_union.key;

			int num_of_item = OUTPUT_ALARM_MAX;

			const char* current_label[num_of_item];
			getMainDictStrings(pMenuData->stringIndex1 ,current_label ,num_of_item);

			menuPopupWithMalloc(menu_union ,current_label ,num_of_item);
		}
		else
		{
			menuPopdownWithMap(menu_union ,-1);
		}
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

//group select
void menu211(MENU_UNION menu_union)//2 ,1 ,1 ,0
{
	MENU_UNION menu_alarm = {{2 ,1 ,0 ,0}};//获取alarm菜单的索引
	int alarm = popGetLastPos(menu_alarm);
	menu_union.pos.menu4 = alarm;

	popDataStruct* pMenuData = (popDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)//用于找数据,函数
	{
		int enabled_group = output_get_parameter(alarm ,OUTPUT_ALARM_COMMAND_ENABLED_GROUP);//更新需要读取数据
		int num_of_group = get_group_qty(pp->p_config);
		int i;
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
		{
			gCurrentMenuPos.key = menu_union.key;

			//获取设置过的组
			int num_of_item = 3 + num_of_group;
		//	const char** current_label = malloc(num_of_item * sizeof(const char*));//使能的组个数加3个label标签
			const char* current_label[num_of_item];
			for(i = 0 ;i < num_of_group ;++i)
			{
				if(0x01 == (0x01 & (enabled_group >> i)))//查找打开的组
				{
					current_label[i] = getMainDictString(pMenuData->stringIndex1 ,i + 16);//constLabel1[i + 16];
				}else{
					current_label[i] = getMainDictString(pMenuData->stringIndex1 ,i);//constLabel1[i];
				}
			}
			current_label[num_of_group] = getMainDictString(pMenuData->stringIndex2 ,0);//constLabel2[0];   //-----
			current_label[num_of_group + 1] = getMainDictString(pMenuData->stringIndex2 ,1);//constLabel2[1];//all
			current_label[num_of_group + 2] = getMainDictString(pMenuData->stringIndex2 ,2);//constLabel2[2];//none
			pMenuData->itemStatus = 1 << num_of_group;//-------

			if(pMenuData->lastPopPos >= num_of_item)//如果其他地方组的个数有变动，有可能造成上次选中的位置大于组个数
			{
				assert(num_of_item > 0);
				pMenuData->lastPopPos = 0;
			}

			menuPopupWithMalloc(menu_union ,current_label ,num_of_item);
		//	free(current_label);
		}
		else
		{
			//获取设置过的组
			int multi = 0;
			const char* last_label;
			for(i = 0 ;i < num_of_group ;++i)
			{
				if(0x01 == (0x01 & (enabled_group >> i)))//查找打开的组
				{
					++multi;
					last_label = getMainDictString(pMenuData->stringIndex1 ,i);//constLabel1[i];//单选
				}
			}
			if(0 == multi)
			{
				last_label = getMainDictString(pMenuData->stringIndex2 ,2);//constLabel2[2];//none
			}
			else if(1 == multi)
			{
				//单选,只有1个组时挡住显示ALL，显示group1
			}
			else if(num_of_group == multi)
			{
				last_label = getMainDictString(pMenuData->stringIndex2 ,1);//constLabel2[1];//all
			}
			else
			{
				last_label = getMainDictString(pMenuData->stringIndex2 ,3);//constLabel2[3];//multi
			}
			menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE
					,getMainLabelString(menu_union) ,last_label);

			if(output_get_parameter(alarm ,OUTPUT_COMMAND_SWITCH))
			{
				menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DISABLE ,LABELBUTTON_STATUS_RELEASE ,NULL ,NULL);
			}
		}
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

//condition1
void menu212(MENU_UNION menu_union)//2 ,1 ,2 ,0
{
	MENU_UNION menu_alarm = {{2 ,1 ,0 ,0}};//获取alarm菜单的索引
	int alarm = popGetLastPos(menu_alarm);
	menu_union.pos.menu4 = alarm;

	popDataStruct* pMenuData = (popDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)//用于找数据,函数
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
		{
			gCurrentMenuPos.key = menu_union.key;

			int condition = output_get_parameter(alarm ,OUTPUT_ALARM_COMMAND_CONDITION2);
			pMenuData->itemStatus = 0;
			if(ALARM_CONDITION_NONE != condition)
			{
				pMenuData->itemStatus = 1 << condition;
			}

			menuPopupWithMap(menu_union ,-1);
		}
		else
		{
			menuPopdownWithMap(menu_union ,output_get_parameter(alarm ,OUTPUT_ALARM_COMMAND_CONDITION1));

			if(output_get_parameter(alarm ,OUTPUT_COMMAND_SWITCH))
			{
				menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DISABLE ,LABELBUTTON_STATUS_RELEASE ,NULL ,NULL);
			}
		}
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

void menu213(MENU_UNION menu_union)//2 ,1 ,3 ,0
{
	MENU_UNION menu_alarm = {{2 ,1 ,0 ,0}};//获取alarm菜单的索引
	int alarm = popGetLastPos(menu_alarm);
	menu_union.pos.menu4 = alarm;

	popDataStruct* pMenuData = (popDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)//用于找数据,函数
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
		{
			gCurrentMenuPos.key = menu_union.key;

			menuPopupWithMap(menu_union ,-1);
		}
		else
		{
			menuPopdownWithMap(menu_union ,output_get_parameter(alarm ,OUTPUT_ALARM_COMMAND_OPERATOR));

			if(	(0 == output_get_parameter(alarm ,OUTPUT_ALARM_COMMAND_CONDITION1))
			||	(0 == output_get_parameter(alarm ,OUTPUT_ALARM_COMMAND_CONDITION2))
			||	(output_get_parameter(alarm ,OUTPUT_COMMAND_SWITCH)))
			{
				menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DISABLE ,LABELBUTTON_STATUS_RELEASE ,NULL ,NULL);
			}
		}
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}


void menu214(MENU_UNION menu_union)//2 ,1 ,4 ,0
{
	MENU_UNION menu_alarm = {{2 ,1 ,0 ,0}};//获取alarm菜单的索引
	int alarm = popGetLastPos(menu_alarm);
	menu_union.pos.menu4 = alarm;

	popDataStruct* pMenuData = (popDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)//用于找数据,函数
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
		{
			gCurrentMenuPos.key = menu_union.key;

			int condition = output_get_parameter(alarm ,OUTPUT_ALARM_COMMAND_CONDITION1);
			pMenuData->itemStatus = 0;
			if(ALARM_CONDITION_NONE != condition)
			{
				pMenuData->itemStatus = 1 << condition;
			}

			menuPopupWithMap(menu_union ,-1);
		}
		else
		{
			menuPopdownWithMap(menu_union ,output_get_parameter(alarm ,OUTPUT_ALARM_COMMAND_CONDITION2));

			if(output_get_parameter(alarm ,OUTPUT_COMMAND_SWITCH))
			{
				menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DISABLE ,LABELBUTTON_STATUS_RELEASE ,NULL ,NULL);
			}
		}
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

void menu215(MENU_UNION menu_union)//2 ,1 ,5 ,0
{
	menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_NONE ,LABELBUTTON_STATUS_HIDE ,NULL ,NULL);
}

void menu220(MENU_UNION menu_union)//2 ,2 ,0 ,0
{
	popDataStruct* pMenuData = (popDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)//用于找数据,函数
	{
		if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
		{
			gCurrentMenuPos.key = menu_union.key;

			int num_of_item = OUTPUT_MAX;
		//	const char** current_label = malloc(num_of_item * sizeof(const char*));
			const char* current_label[num_of_item];
			int i;
			for(i = 0 ;i < OUTPUT_MAX ;++i)
			{
				if(0x01 == output_get_parameter(i ,OUTPUT_COMMAND_SWITCH))//
				{
					current_label[i] = getMainDictString(pMenuData->stringIndex1 ,OUTPUT_MAX + i);
				}else{
					current_label[i] = getMainDictString(pMenuData->stringIndex1 ,i);
				}
			}

			menuPopupWithMalloc(menu_union ,current_label ,num_of_item);
		//	free(current_label);
		}
		else
		{
			menuPopdownWithMap(menu_union ,-1);
		}
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}


void menu221(MENU_UNION menu_union)//2 ,2 ,1 ,0
{
	MENU_UNION menu_output = {{2 ,2 ,0 ,0}};//获取output菜单的索引
	int output = popGetLastPos(menu_output);
	menu_union.pos.menu4 = output;

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
			pMenuData->data = output_get_parameter(output ,OUTPUT_COMMAND_SWITCH);//更新需要读取数据
		//	menuSetDataWithMap(menu_union ,LABELBUTTON_STATUS_RELEASE);
			menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE
					,getMainLabelString(menu_union)
					,getMainDictString(MAINSTRINGDICT_SWITCH ,pMenuData->data));
		}
	}
}

static void output_count_press(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		if(menu3Repeat())
		{
			if(1.0 == pMenuData->step)	pMenuData->step = 10.0;
			else						pMenuData->step = 1.0;
		}
		menuSpinPress(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_PRESS);
	}
}

static void output_count_update(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		int output = menu_union.pos.menu4;
		pMenuData->current_value = (float)output_get_parameter(output ,OUTPUT_ALARM_COMMAND_COUNT);
		menuSpinRelease(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

static void output_analog_group_press(MENU_UNION menu_union)
{
	popDataStruct* pMenuData = (popDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)//用于找数据,函数
	{
		int num_of_group = get_group_qty(pp->p_config);
		int num_of_item = num_of_group;
		const char* current_label[num_of_item];
		getMainDictStrings(pMenuData->stringIndex1 ,current_label ,num_of_item);

		menuPopupWithMalloc(menu_union ,current_label ,num_of_item);

	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_PRESS);
	}
}

static void output_analog_group_update(MENU_UNION menu_union)
{
	popDataStruct* pMenuData = (popDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)//用于找数据,函数
	{
		int num_of_group = get_group_qty(pp->p_config);
		int num_of_item = num_of_group;
		const char* current_label[num_of_item];
		getMainDictStrings(pMenuData->stringIndex1 ,current_label ,num_of_item);

		int output = menu_union.pos.menu4;
		pMenuData->lastPopPos = output_get_parameter(output ,OUTPUT_ANALOG_COMMAND_ENABLED_GROUP);//更新需要读取数据
		if(pMenuData->lastPopPos >= num_of_item)//如果其他地方组的个数有变动，有可能造成上次选中的位置大于组个数
		{
			assert(num_of_item > 0);
			pMenuData->lastPopPos = 0;
		}

		menuPopdownWithMalloc(menu_union ,current_label ,num_of_item);

	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

void menu222(MENU_UNION menu_union)//2 ,2 ,2 ,0
{
	MENU_UNION menu_output = {{2 ,2 ,0 ,0}};//获取output菜单的索引
	int output = popGetLastPos(menu_output);
	menu_union.pos.menu4 = output;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
	{
		gCurrentMenuPos.key = menu_union.key;

		if(output < OUTPUT_ALARM_MAX)
		{//alarm
			output_count_press(menu_union);
		}
		else
		{//analog
			output_analog_group_press(menu_union);
		}
	}
	else
	{
		if(output < OUTPUT_ALARM_MAX)
		{//alarm count
			output_count_update(menu_union);
		}
		else
		{//analog group
			output_analog_group_update(menu_union);
		}

		if(output_get_parameter(output ,OUTPUT_COMMAND_SWITCH))
		{
			menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DISABLE ,LABELBUTTON_STATUS_RELEASE ,NULL ,NULL);
		}
	}
}

#define output_beep_press(x)  menuPopupWithMap((x) ,-1)
#define output_beep_update(x) menuPopdownWithMap((x) ,output_get_parameter(0 ,OUTPUT_COMMAND_BEEP_FREQUENCY))

#define output_analog_dataSource_press(x)  menuPopupWithMap((x) ,-1)
static void output_analog_dataSource_update(MENU_UNION menu_union)
{
	popDataStruct* pMenuData = (popDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)//用于找数据,函数
	{
		int output = menu_union.pos.menu4;
		int value = output_get_parameter(output ,OUTPUT_ANALOG_COMMAND_SOURCE);//更新需要读取数据
		int i;
		if(value)
		{
			for(i = ANALOG_SOURCE_PEAK_A; i < ANALOG_SOURCE_MAX ;++i)
			{
				if(value & (1 << (i - 1)))//source来源于哪个选项
				{
					pMenuData->lastPopPos = i;
					break;
				}
			}
		}
		else
		{
			pMenuData->lastPopPos = value;
		}

		menuPopdownWithMap(menu_union ,-1);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

void menu223(MENU_UNION menu_union)//2 ,2 ,3 ,0
{
	MENU_UNION menu_output = {{2 ,2 ,0 ,0}};//获取output菜单的索引
	int output = popGetLastPos(menu_output);
	menu_union.pos.menu4 = output;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
	{
		gCurrentMenuPos.key = menu_union.key;

		if(output < OUTPUT_ALARM_MAX)
		{//alarm
			output_beep_press(menu_union);
		}
		else
		{//analog
			output_analog_dataSource_press(menu_union);
		}
	}
	else
	{
		if(output < OUTPUT_ALARM_MAX)
		{//alarm count
			output_beep_update(menu_union);
		}
		else
		{//analog group
			output_analog_dataSource_update(menu_union);
		}

		if(output_get_parameter(output ,OUTPUT_COMMAND_SWITCH))
		{
			menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DISABLE ,LABELBUTTON_STATUS_RELEASE ,NULL ,NULL);
		}
	}
}

static void output_delay_press(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		if(menu3Repeat())
		{
			if(1.0 == pMenuData->step)			pMenuData->step = 10.0;
			else if(10.0 == pMenuData->step)	pMenuData->step = 0.1;
			else if(0.1 == pMenuData->step)		pMenuData->step = 0.01;
			else								pMenuData->step = 1.0;
		}
		menuSpinPress(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_PRESS);
	}
}

static void output_delay_update(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		int output = menu_union.pos.menu4;
		pMenuData->current_value = output_get_parameter(output ,OUTPUT_ALARM_COMMAND_DELAY) / 100.0;
		menuSpinRelease(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

void menu224(MENU_UNION menu_union)//2 ,2 ,4 ,0
{
	MENU_UNION menu_output = {{2 ,2 ,0 ,0}};//获取output菜单的索引
	int output = popGetLastPos(menu_output);
	menu_union.pos.menu4 = output;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
	{
		gCurrentMenuPos.key = menu_union.key;

		if(output < OUTPUT_ALARM_MAX)
		{//alarm
			output_delay_press(menu_union);
		}
		else
		{//analog

		}
	}
	else
	{
		if(output < OUTPUT_ALARM_MAX)
		{//alarm count
			output_delay_update(menu_union);
			if(output_get_parameter(output ,OUTPUT_COMMAND_SWITCH))
			{
				menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DISABLE ,LABELBUTTON_STATUS_RELEASE ,NULL ,NULL);
			}
		}
		else
		{//analog group
			menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_NONE ,LABELBUTTON_STATUS_RELEASE ,NULL ,NULL);
		}

	}
}

#define output_holdtime_press(x) output_delay_press(x)
static void output_holdtime_update(MENU_UNION menu_union)
{
	spinDataStruct* pMenuData = (spinDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		int output = menu_union.pos.menu4;
		pMenuData->current_value = output_get_parameter(output ,OUTPUT_ALARM_COMMAND_HOLD_TIME) / 100.0;
		menuSpinRelease(menu_union);
	}
	else
	{
		menuSetBackground(menu_union.pos.menu3 ,LABELBUTTON_MODE_DATA3 ,LABELBUTTON_STATUS_RELEASE);
	}
}

void menu225(MENU_UNION menu_union)//2 ,2 ,5 ,0
{
	MENU_UNION menu_output = {{2 ,2 ,0 ,0}};//获取output菜单的索引
	int output = popGetLastPos(menu_output);
	menu_union.pos.menu4 = output;
	if ((pp->pos_pos == MENU3_PRESSED) && (CUR_POS == menu_union.pos.menu3))
	{
		gCurrentMenuPos.key = menu_union.key;

		if(output < OUTPUT_ALARM_MAX)
		{//alarm
			output_holdtime_press(menu_union);
		}
		else
		{//analog

		}
	}
	else
	{
		if(output < OUTPUT_ALARM_MAX)
		{//alarm count
			output_holdtime_update(menu_union);

			if(output_get_parameter(output ,OUTPUT_COMMAND_SWITCH))
			{
				menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_DISABLE ,LABELBUTTON_STATUS_RELEASE ,NULL ,NULL);
			}
		}
		else
		{//analog group
			menuSetLabel(menu_union.pos.menu3 ,LABELBUTTON_MODE_NONE ,LABELBUTTON_STATUS_RELEASE ,NULL ,NULL);
		}
	}
}

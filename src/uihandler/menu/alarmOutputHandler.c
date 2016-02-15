/*
 * alarmOutputHandler.c
 *
 *  Created on: 2012-2-24
 *      Author: lzk
 */

#include "menuHandler.h"
#include "../../lzk/common.h"
#include "../../map/menuMap.h"

#include "../../drawui.h"
#include <assert.h>
#include "../../lzk/output_spi.h"
int outputHandler_alarmSelect(void* p_para)
{
	if(NULL != p_para)
	{
		int para = *(int*)p_para;
		set_alarm_pos (pp->p_config, (guchar)para);
	}
	return TRUE;
}

int outputHandler_alarmGroupSelect(void* p_para)
{
	if(NULL != p_para)
	{
		int para = *(int*)p_para;
		int command;
		set_alarm_groupa (pp->p_config,	(guchar)para);
		/* 发送给硬件 */
		MENU_UNION menu_alarm = {{2 ,1 ,0 ,0}};//获取alarm菜单的索引
		int alarm = popGetLastPos(menu_alarm);
		g_debug("alarm:%d" ,alarm);
		int enabled_group = output_get_parameter(alarm ,OUTPUT_ALARM_COMMAND_ENABLED_GROUP);//更新需要读取数据
		int num_of_group = get_group_qty(pp->p_config);
		if(para < num_of_group)
		{
			if(0x01 == (0x01 & (enabled_group >> para)))//获取group的状态
			{
				command = ALARM_GROUP_COMMAND_OFF;
			}
			else
			{
				command = ALARM_GROUP_COMMAND_ON;
			}
		}
		else if(para == 1 + num_of_group)//ALL
		{
			command = ALARM_GROUP_COMMAND_ALL;
			para = num_of_group;//总的group数
		}
		else if(para == 2 + num_of_group)//NONE
		{
			command = ALARM_GROUP_COMMAND_NONE;
		}
		else
		{
			return TRUE;
		}
		output_set_parameter(alarm ,OUTPUT_ALARM_COMMAND_ENABLED_GROUP ,command ,para);
		output_write_one_reg_to_spi(alarm ,OUTPUT_ALARM_COMMAND_ENABLED_GROUP);
	}
	return TRUE;
}

int outputHandler_alarmCondition1(void* p_para)
{
	if(NULL != p_para)
	{
		int para = *(int*)p_para;
		set_alarm_conditiona (pp->p_config,	(guchar) para);
		// 发送给硬件
		MENU_UNION menu_alarm = {{2 ,1 ,0 ,0}};//获取alarm菜单的索引
		int alarm = popGetLastPos(menu_alarm);
		output_set_parameter(alarm ,OUTPUT_ALARM_COMMAND_CONDITION1 ,para ,0);
		if(0 == para)//NONE
		{
			output_set_parameter(alarm ,OUTPUT_ALARM_COMMAND_OPERATOR ,1 ,0);
		//	output_write_one_reg_to_spi(alarm ,OUTPUT_ALARM_COMMAND_OPERATOR);//发1次就可以了
		}
		output_write_one_reg_to_spi(alarm ,OUTPUT_ALARM_COMMAND_CONDITION1);
	}
	return TRUE;
}

int outputHandler_alarmOperator(void* p_para)
{
	if(NULL != p_para)
	{
		int para = *(int*)p_para;
		set_alarm_operator (pp->p_config, (guchar)para);
		// 发送给硬件
		MENU_UNION menu_alarm = {{2 ,1 ,0 ,0}};//获取alarm菜单的索引
		int alarm = popGetLastPos(menu_alarm);
		output_set_parameter(alarm ,OUTPUT_ALARM_COMMAND_OPERATOR ,para ,0);
		output_write_one_reg_to_spi(alarm ,OUTPUT_ALARM_COMMAND_OPERATOR);
	}
	return TRUE;
}

int outputHandler_alarmCondition2(void* p_para)
{

	if(NULL != p_para)
	{
		int para = *(int*)p_para;
		MENU_UNION menu_alarm = {{2 ,1 ,0 ,0}};//获取alarm菜单的索引
		int alarm = popGetLastPos(menu_alarm);
		output_set_parameter(alarm ,OUTPUT_ALARM_COMMAND_CONDITION2 ,para ,0);
		if(0 == para)//NONE
		{
			output_set_parameter(alarm ,OUTPUT_ALARM_COMMAND_OPERATOR ,1 ,0);
		//	output_write_one_reg_to_spi(alarm ,OUTPUT_ALARM_COMMAND_OPERATOR);//发一次就可以了
		}
		output_write_one_reg_to_spi(alarm ,OUTPUT_ALARM_COMMAND_CONDITION2);
	}
	return TRUE;
}


int outputHandler_outputSelect(void* p_para)
{

	if(NULL != p_para)
	{
		int para = *(int*)p_para;
		set_output_pos (pp->p_config, (guchar)para);
	}
	return TRUE;
}

int outputHandler_switch(void* p_para)
{
	MENU_UNION menu_output = {{2 ,2 ,0 ,0}};//获取alarm菜单的索引
	int output = popGetLastPos(menu_output);
	int switch_state = output_get_parameter(output ,OUTPUT_COMMAND_SWITCH);
	if(0x01 ==  switch_state)
	{
		output_set_parameter(output ,OUTPUT_COMMAND_SWITCH ,0 ,0);
	}
	else
	{
		output_set_parameter(output ,OUTPUT_COMMAND_SWITCH ,1 ,0);
	}
	output_write_one_reg_to_spi(output ,OUTPUT_COMMAND_SWITCH);

	return TRUE;
}

int outputHandler_alarmCount(void* p_para)
{
	if(NULL != p_para)
	{
		MENU_UNION menu_output = {{2 ,2 ,0 ,0}};//获取alarm菜单的索引
		int output = popGetLastPos(menu_output);
		if(output < OUTPUT_ALARM_MAX)
		{
			float para = *(float*)p_para;
			int count = (int)para;
			set_output_count (pp->p_config, (unsigned char)para);
			output_set_parameter(output ,OUTPUT_ALARM_COMMAND_COUNT , count ,0);
			output_write_one_reg_to_spi(output ,OUTPUT_ALARM_COMMAND_COUNT);
		}
	}
	return TRUE;
}

int outputHandler_analogGroup(void* p_para)
{
	if(NULL != p_para)
	{
		MENU_UNION menu_output = {{2 ,2 ,0 ,0}};//获取alarm菜单的索引
		int output = popGetLastPos(menu_output);
		if(output >= OUTPUT_ALARM_MAX)
		{
			int para = *(int*)p_para;
			set_output_group (pp->p_config, (guchar)para);
			output_set_parameter(output ,OUTPUT_ANALOG_COMMAND_ENABLED_GROUP ,para ,0);
			//send to spi
			output_write_one_reg_to_spi(output ,OUTPUT_ANALOG_COMMAND_ENABLED_GROUP);
		}
	}
	return TRUE;
}

int outputHandler_alarmBeep(void* p_para)
{
	if(NULL != p_para)
	{
		MENU_UNION menu_output = {{2 ,2 ,0 ,0}};//获取alarm菜单的索引
		int output = popGetLastPos(menu_output);
		if(output < OUTPUT_ALARM_MAX)
		{
			int para = *(int*)p_para;
			set_output_sound (pp->p_config,	(guchar)para);
			output_set_parameter(0 ,OUTPUT_COMMAND_BEEP_FREQUENCY ,para ,0);
			output_write_one_reg_to_spi(0 ,OUTPUT_COMMAND_BEEP_FREQUENCY);
		}
	}
	return TRUE;
}

int outputHandler_analogDataSource(void* p_para)
{

	if(NULL != p_para)
	{
		int para = *(int*)p_para;
		MENU_UNION menu_output = {{2 ,2 ,0 ,0}};//获取alarm菜单的索引
		int output = popGetLastPos(menu_output);
		if(output >= OUTPUT_ALARM_MAX)
		{
			set_output_data (pp->p_config, (guchar)para);
			output_set_parameter(output ,OUTPUT_ANALOG_COMMAND_SOURCE ,para ,0);
			output_write_one_reg_to_spi(output ,OUTPUT_ANALOG_COMMAND_SOURCE);
		}
	}
	return TRUE;
}

int outputHandler_alarmDelay(void* p_para)
{

	if(NULL != p_para)
	{
		MENU_UNION menu_output = {{2 ,2 ,0 ,0}};//获取alarm菜单的索引
		int output = popGetLastPos(menu_output);
		if(output < OUTPUT_ALARM_MAX)
		{
			float para = *(float*)p_para;
			unsigned int value = (unsigned int)(para * 100.0);//ms->10us
			set_output_delay (pp->p_config,	(guint) (para * 1000.0));
			output_set_parameter(output ,OUTPUT_ALARM_COMMAND_DELAY ,value ,0);
			output_write_one_reg_to_spi(output ,OUTPUT_ALARM_COMMAND_DELAY);
		}
	}
	return TRUE;
}

int outputHandler_alarmHoleTime(void* p_para)
{

	if(NULL != p_para)
	{
		MENU_UNION menu_output = {{2 ,2 ,0 ,0}};//获取alarm菜单的索引
		int output = popGetLastPos(menu_output);
		if(output < OUTPUT_ALARM_MAX)
		{
			float para = *(float*)p_para;
			unsigned int value = (unsigned int)(para * 100.0);//ms->10us
			set_output_holdtime (pp->p_config,	(guint) (para * 1000.0));
			output_set_parameter(output ,OUTPUT_ALARM_COMMAND_HOLD_TIME ,value ,0);
			output_write_one_reg_to_spi(output ,OUTPUT_ALARM_COMMAND_HOLD_TIME);
		}
	}
	return TRUE;
}


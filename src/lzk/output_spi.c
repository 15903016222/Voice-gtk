/*
 * output_spi.c
 *
 *  Created on: 2012-2-24
 *      Author: lzk
 */


#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "output_spi.h"
#include "../spi_d.h"
#include "../base.h"
#include "../drawui.h"
#include "../callback.h"
#include "../main.h"
#include "../globalData.h"
//alarm
//start 寄存器起始地址
//end 寄存器结束地址 ,建议用start + n写n个寄存器，小于总的寄存器 + 1

void write_output_to_spi(output_to_spi_t* p_alarm ,unsigned int start ,unsigned int end)
{
	unsigned int* p_offset;
	int size_to_write;
	unsigned int num_of_register;//总寄存器数
	unsigned int num_of_register_to_write;
	int i;
	output_to_spi_t alarm;
	memcpy(&alarm ,p_alarm ,sizeof(output_to_spi_t));
	num_of_register = sizeof(output_to_spi_t) / 4 - 1;//去掉-1寄存器
	assert(start <= end);
	assert(end <= num_of_register + 1);
	//要写入的大小
	num_of_register_to_write = end - start;

	//写入
	if(num_of_register_to_write > 0)
	{
		//转换数据格式
		alarm.offset = start;//首地址
		//alarm.addr = 1 << 3;//alarm地址
		p_offset = (unsigned int*)&alarm;
		little_to_big (p_offset, num_of_register + 1);//包含-1寄存器
		size_to_write = 4 * (num_of_register_to_write + 1);//包含-1寄存器
		if(num_of_register_to_write == num_of_register)
		{
#if ARM
			write (fd_array, p_offset, size_to_write);
#endif
		}
		else
		{
		//	unsigned int* data_to_write;
		//	data_to_write = (unsigned int*)malloc(size_to_write);
			unsigned int data_to_write[size_to_write];
			data_to_write[0] = p_offset[0];
			for(i = start ;i < end ;++i)
			{
				data_to_write[i - start + 1] = p_offset[i + 1];//包含-1寄存器
			}

#if ARM
			write (fd_array ,data_to_write ,size_to_write);
#endif
		//	free(data_to_write);
		}
	}

}

output_to_spi_t output_data;
unsigned int output_get_parameter(OUTPUT_ENUM output ,OUTPUT_COMMAND_ENUM command)
{
	int analog;
	assert(command < OUTPUT_COMMAND_MAX);
	switch(command)
	{
		case OUTPUT_ALARM_COMMAND_ENABLED_GROUP:	assert(output < OUTPUT_ALARM_MAX);	return output_data.alarm[output].enabled_group;
		case OUTPUT_ALARM_COMMAND_CONDITION1:		assert(output < OUTPUT_ALARM_MAX);	return output_data.alarm[output].logic_condition1;
		case OUTPUT_ALARM_COMMAND_OPERATOR:			assert(output < OUTPUT_ALARM_MAX);	return output_data.alarm[output].operator;
		case OUTPUT_ALARM_COMMAND_CONDITION2:		assert(output < OUTPUT_ALARM_MAX);	return output_data.alarm[output].logic_condition2;
		case OUTPUT_ALARM_COMMAND_COUNT:			assert(output < OUTPUT_ALARM_MAX);	return output_data.alarm[output].count;
		case OUTPUT_ALARM_COMMAND_DELAY:			assert(output < OUTPUT_ALARM_MAX);	return output_data.alarm[output].delay;
		case OUTPUT_ALARM_COMMAND_HOLD_TIME:		assert(output < OUTPUT_ALARM_MAX);	return output_data.alarm[output].hold_time;

		case OUTPUT_COMMAND_BEEP_FREQUENCY:												return output_data.freqency;
		case OUTPUT_COMMAND_SWITCH:					assert(output < OUTPUT_MAX); 		return (0x01 & (output_data.output_switch >> output));

		case OUTPUT_OTHER_COMMAND_LAW_QTY:          return output_data.law_qty  ;
		case OUTPUT_OTHER_COMMAND_LAW_QTY_VIRTUAL:  return output_data.law_qty_virtual  ;
		case OUTPUT_OTHER_COMMAND_VOLTAGE_UT:		return output_data.voltage_ut;//发射电压
		case OUTPUT_OTHER_COMMAND_VOLTAGE_PA:		return output_data.voltage_pa;//发射电压
		case OUTPUT_OTHER_COMMAND_DAMPING_UT1:		return output_data.damping_ut1;//阻尼
		case OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT1:	return output_data.twin_on_off_ut1;//双晶
		case OUTPUT_OTHER_COMMAND_DAMPING_UT2:		return output_data.damping_ut2;//阻尼
		case OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT2:	return output_data.twin_on_off_ut1;//双晶
		//	case OUTPUT_OTHER_COMMAND_TWIN_UT:			return output_data.twin_ut;//UT
		case OUTPUT_OTHER_COMMAND_ENCODE_X:			return output_data.enc_x;
		case OUTPUT_OTHER_COMMAND_ENCODE_Y:			return output_data.enc_y;

		case OUTPUT_ANALOG_COMMAND_ENABLED_GROUP:   analog = output - OUTPUT_ALARM_MAX; assert(analog < OUTPUT_ANALOG_MAX);  return output_data.analog[analog].enabled_group;
		case OUTPUT_ANALOG_COMMAND_SOURCE:          analog = output - OUTPUT_ALARM_MAX; assert(analog < OUTPUT_ANALOG_MAX);  return output_data.analog[analog].source;

		default:		break;
	}
	return 0;
}

void output_set_parameter(OUTPUT_ENUM output ,OUTPUT_COMMAND_ENUM command ,unsigned int value ,unsigned int value2)
{
	int analog;
	int factor;
	int i;
	assert(command < OUTPUT_COMMAND_MAX);
	switch(command)
	{
	case OUTPUT_ALARM_COMMAND_ENABLED_GROUP:
		assert(output < OUTPUT_ALARM_MAX);
		assert(value < ALARM_GROUP_COMMAND_MAX);
		assert(value2 < 16);
		switch(value)
		{
		case ALARM_GROUP_COMMAND_ALL:
			for(i = 0 ;i < value2 ;++i)//value2表示当前组的数量
			{
				output_data.alarm[output].enabled_group |= (1 << i);
			}
			break;

		case ALARM_GROUP_COMMAND_NONE:
			output_data.alarm[output].enabled_group = 0;
			break;

		case ALARM_GROUP_COMMAND_ON:
			output_data.alarm[output].enabled_group |= (1 << value2);//value2表示使能的组
			break;

		case ALARM_GROUP_COMMAND_OFF:
			output_data.alarm[output].enabled_group &= ~(1 << value2);//value2表示清除使能的组
			break;

		default:
			output_data.alarm[output].enabled_group = 0;
			break;
		}
		break;

	case OUTPUT_ALARM_COMMAND_CONDITION1:
		assert(output < OUTPUT_ALARM_MAX);
		assert(value < ALARM_CONDITION_MAX);
		output_data.alarm[output].logic_condition1 = value;
		break;

	case OUTPUT_ALARM_COMMAND_OPERATOR:
		assert(output < OUTPUT_ALARM_MAX);
		assert(value < ALARM_OPERATOR_MAX);
		output_data.alarm[output].operator = value;
		break;

	case OUTPUT_ALARM_COMMAND_CONDITION2:
		assert(output < OUTPUT_ALARM_MAX);
		assert(value < ALARM_CONDITION_MAX);
		output_data.alarm[output].logic_condition2 = value;
		break;

	case OUTPUT_COMMAND_BEEP_FREQUENCY:
	//	assert(output < OUTPUT_ALARM_MAX);
		assert(value < ALARM_FREQ_MAX);
		output_data.freqency = value; 		//0-4
		break;

	case OUTPUT_ALARM_COMMAND_COUNT:
		assert(output < OUTPUT_ALARM_MAX);
		assert(value <= 100);
		output_data.alarm[output].count = value;
		break;

	case OUTPUT_ALARM_COMMAND_DELAY:
		assert(output < OUTPUT_ALARM_MAX);
		assert(value <= 5000000);//5s 5*1000ms 5000*1000us
		output_data.alarm[output].delay = value; 		//0-2
		break;

	case OUTPUT_ALARM_COMMAND_HOLD_TIME:
		assert(output < OUTPUT_ALARM_MAX);
		assert(value <= 5000000);//5s
		output_data.alarm[output].hold_time = value; 		//0-2
		break;

	case OUTPUT_COMMAND_SWITCH:
		assert(output < OUTPUT_MAX);
		if(value != 0)
		{
			output_data.output_switch |= (1 << output);
		}
		else
		{
			output_data.output_switch &= ~(1 << output);
		}
		break;

	case OUTPUT_ANALOG_COMMAND_ENABLED_GROUP:
		assert((output == OUTPUT_4) || (output == OUTPUT_5));
		analog = output - OUTPUT_ALARM_MAX;
		assert(analog < OUTPUT_ANALOG_MAX);
		assert(value < 16);
		output_data.analog[analog].enabled_group = value;
		break;

	case OUTPUT_ANALOG_COMMAND_SOURCE:
		assert((output == OUTPUT_4) || (output == OUTPUT_5));
		analog = output - OUTPUT_ALARM_MAX;
		assert(analog < OUTPUT_ANALOG_MAX);
		assert(value < ANALOG_SOURCE_MAX);
		if(ANALOG_SOURCE_OFF != value)
		{
			output_data.analog[analog].source = 1 << (value - 1);
		}
		else
		{
			output_data.analog[analog].source = 0;
		}
		break;

	case OUTPUT_ANALOG_COMMAND_FACTOR_ECHO:
		assert(value > 0);
		assert(value < 11);//回波数
		factor = 0x0fff / value;
		output_data.factor_echo = factor;
		break;

	case OUTPUT_OTHER_COMMAND_LAW_QTY:
		output_data.law_qty  = value ;
		output_data.law_qty_virtual = value ;
		break;
	case OUTPUT_OTHER_COMMAND_LAW_QTY_VIRTUAL:
		output_data.law_qty_virtual = value ;
		break;
	case OUTPUT_OTHER_COMMAND_VOLTAGE_UT:		//常规发射电压
		output_data.voltage_ut = value;
		break;
	case OUTPUT_OTHER_COMMAND_VOLTAGE_PA:		//相控阵发射电压
		assert(value < VOLTAGE_PA_MAX);
		output_data.voltage_pa = value;
		break;

	case OUTPUT_OTHER_COMMAND_DAMPING_UT1://阻尼
		output_data.damping_ut1 = value;
		break;

	case OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT1:	//双晶
		output_data.twin_on_off_ut1 = value;
		break;

	case OUTPUT_OTHER_COMMAND_DAMPING_UT2://阻尼
		output_data.damping_ut2 = value;
		break;

	case OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT2:	//双晶
		output_data.twin_on_off_ut2 = value;
		break;
//	case OUTPUT_OTHER_COMMAND_TWIN_UT:	//UT
//		output_data.twin_ut = value;
//		break;

	case OUTPUT_OTHER_COMMAND_ENCODE_X:
		output_data.enc_x = value;
		break;

	case OUTPUT_OTHER_COMMAND_ENCODE_Y:
		output_data.enc_y = value;
		break;

	case OUTPUT_OTHER_COMMAND_CHANNEL_ENABLE:
		output_data.channel_enable = value;
		break ;
	case OUTPUT_OTHER_COMMAND_FPGA_FROZEN:
		output_data.fpga_freeze   = value;
		break ;

	default:
		break;
	}
}

void output_write_one_reg_to_spi(OUTPUT_ENUM output ,OUTPUT_COMMAND_ENUM command)
{
	int start ,analog;
	assert(command < OUTPUT_COMMAND_MAX);
	switch(command)
	{
	case OUTPUT_ALARM_COMMAND_ENABLED_GROUP:
	case OUTPUT_ALARM_COMMAND_CONDITION1:
	case OUTPUT_ALARM_COMMAND_OPERATOR:
	case OUTPUT_ALARM_COMMAND_CONDITION2:
	case OUTPUT_ALARM_COMMAND_COUNT: 			assert(output < OUTPUT_ALARM_MAX);	start = 3 * output + 17;	break;
	case OUTPUT_ALARM_COMMAND_DELAY: 			assert(output < OUTPUT_ALARM_MAX);	start = 3 * output + 18;	break;
	case OUTPUT_ALARM_COMMAND_HOLD_TIME: 		assert(output < OUTPUT_ALARM_MAX);	start = 3 * output + 19; 	break;

	case OUTPUT_COMMAND_SWITCH: 													start = 16;					break;
	case OUTPUT_COMMAND_BEEP_FREQUENCY: 										start = 16;					break;

	case OUTPUT_ANALOG_COMMAND_ENABLED_GROUP:
	case OUTPUT_ANALOG_COMMAND_SOURCE:
													analog = output - OUTPUT_ALARM_MAX;
													assert(analog < OUTPUT_ANALOG_MAX);
													start = analog + 26; 	break;

	case OUTPUT_ANALOG_COMMAND_FACTOR_ECHO:    		start = 28; 	break;

	case OUTPUT_OTHER_COMMAND_LAW_QTY:
	case OUTPUT_OTHER_COMMAND_LAW_QTY_VIRTUAL:
		start = 0 ;
		break;
	case OUTPUT_OTHER_COMMAND_VOLTAGE_UT:    								//UT发射电压
	case OUTPUT_OTHER_COMMAND_VOLTAGE_PA:   								//PA发射电压
	case OUTPUT_OTHER_COMMAND_DAMPING_UT1:    									//阻尼
	case OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT1:    								//双晶
	case OUTPUT_OTHER_COMMAND_DAMPING_UT2:    									//阻尼
	case OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT2:    							    //双晶
	//case OUTPUT_OTHER_COMMAND_TWIN_UT:    			//start = 1; 	break;		//UT

	case OUTPUT_OTHER_COMMAND_ENCODE_X:
	case OUTPUT_OTHER_COMMAND_ENCODE_Y:
		start = 1 ;
		break;
	case OUTPUT_OTHER_COMMAND_CHANNEL_ENABLE:
		start = 2 ;
		break ;
	case OUTPUT_OTHER_COMMAND_FPGA_FROZEN:
		start = 3 ;
		break ;
	default:
		return;
	}
	write_output_to_spi(&output_data ,start ,start + 1);
}

void output_write_all_to_spi()
{
	write_output_to_spi(&output_data ,0 ,sizeof(output_to_spi_t) / 4 - 1);//写全部寄存器
}

void SetChannelEnableFlag(unsigned int nChannelEnableFlag_)
{
	output_data.channel_enable = nChannelEnableFlag_ ;
	output_write_one_reg_to_spi(0 ,OUTPUT_OTHER_COMMAND_CHANNEL_ENABLE);
}

void OutputSave()
{
	memcpy(&(SYSTEM(output)) ,&output_data ,sizeof(output_to_spi_t));
}

static void inline OutputResume()
{
	//恢复reg16-reg28，4*13字节
	memcpy(((int*)&output_data.alarm - 1) ,((int*)&SYSTEM(output).alarm - 1) ,52);
}


void output_init(unsigned int nChannelEnableFlag_)
{
	int i;
	memset(&output_data ,0 ,sizeof(output_data));
	output_data.addr = 1 << 3;
	output_data.channel_enable = nChannelEnableFlag_ ;

	int _nBeamQty = get_beam_qty();
	output_data.law_qty    = _nBeamQty ;
	output_data.law_qty_virtual = _nBeamQty ;

	output_data.voltage_ut = pp->p_config->voltage_ut ;
	output_data.voltage_pa = pp->p_config->voltage_pa ;
	output_data.reserved1 = 1;

	output_data.twin_on_off_ut2 = 0 ;
	output_data.damping_ut2     = 0 ;
	output_data.twin_on_off_ut1 = 0 ;
	output_data.damping_ut1     = 0 ;
	for(i = 0 ; i < get_group_qty(pp->p_config) ; i++)
	{
		if(GROUP_VAL_POS(i , group_mode) == UT1_SCAN)
		{
			if(pp->p_config->damping_pos_ut1)
			{
				output_data.damping_ut1     = 3 ;
			}
            if(group_get_rx_tx_mode(i) != PULSE_ECHO) // PE
			{
				output_data.twin_on_off_ut1 = 1 ;
			}
		}
		else if(GROUP_VAL_POS(i , group_mode) == UT2_SCAN)
		{
			if(pp->p_config->damping_pos_ut2)
			{
				output_data.damping_ut2     = 3 ;
			}
            if(group_get_rx_tx_mode(i) != PULSE_ECHO) // PE
			{
				output_data.twin_on_off_ut2= 1 ;
			}
		}
	}
//报警相关设置
	if(gData->file.loadFail)
	{

		for(i = 0 ;i < OUTPUT_ALARM_MAX ;++i)
		{
			output_data.alarm[i].count = 1;
			output_data.alarm[i].hold_time = 10000;
			output_data.alarm[i].enabled_group = 1;
			output_data.alarm[i].operator = 1;
		}
		//reg28
		output_data.factor_echo = 0x0fff;//0x0fff / num_of_echo;
	}
	else
	{
		OutputResume();
	}

	output_write_all_to_spi();
}

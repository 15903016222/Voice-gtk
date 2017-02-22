/*
 * output_spi.h
 *
 *  Created on: 2012-2-24
 *      Author: lzk
 */

#ifndef OUTPUT_SPI_H_
#define OUTPUT_SPI_H_

typedef struct{
	//register17 20 23
	unsigned int count:7; 				//0-6 低位无用
	unsigned int logic_condition2:4;	//7-10 设置count值
	unsigned int operator:1; 			//11 与或操作
	unsigned int logic_condition1:4;	//12-15
	unsigned int enabled_group:16;		//16-31使能的组

    //register18 21 24
	unsigned int delay:20; 		//0-19 *10us
	unsigned int reserved1:12; 	//20-31

    //register19 22 25
	unsigned int hold_time:20;	//0-19 *10us
	unsigned int reserved2:12; 	//20-31
}
	output_alarm_t;

typedef struct{
	//register26 27
    unsigned int source:3; 					//0-2 A% B%或thickness
    unsigned int enabled_group:4; 			//3-6 源组
    unsigned int reserved1:25; 				//7-31

}
output_analog_t;

typedef struct{
	// s_group_reg (-1) 地址
	unsigned int offset:28;			// bit:0-27 地址的偏移
	unsigned int addr:4;			// bit:28-31 片选 Group取值1000
	// s_group_reg (0) 地址
	//unsigned int reserved1:32;		//0-31
	unsigned int law_qty:16 ;
	unsigned int law_qty_virtual:16 ;
/****************************************************************
	// s_group_reg (1) 地址           // 1XXX inverse logic
	unsigned int enc_x:4;            //bit:0-2 编码器x编码器逻辑，	011:双向增减，010:正向增，001:负向递增
	unsigned int enc_y:4;            //bit:3-5 编码器y 			000 : 关掉编码器  1xx:暂停编码器
	//unsigned int reserved2:2;		 //6-7
	unsigned int voltage_ut:8;			// bit:8-15 发射电压
	unsigned int damping:2;			// bit:16-17 阻尼
	unsigned int twin_on_off:1;		// bit:18 双晶
	//unsigned int twin_ut:1;		// bit:11 0:UT1 1:UT2
	unsigned int voltage_pa:2; 		//20-19
	unsigned int reserved3:11;      //31-21
***************************************************************/
	// s_group_reg (1) 地址
	unsigned int enc_x:4;            //bit:0-2 编码器x编码器逻辑，	011:双向增减，010:正向增，001:负向递增
	unsigned int enc_y:4;            //bit:3-5 编码器y 			000 : 关掉编码器  1xx:暂停编码器
	unsigned int reserved1:1;
	unsigned int twin_on_off_ut2:1;
	unsigned int damping_ut2:2;
	unsigned int twin_on_off_ut1:1;
	unsigned int damping_ut1:2;
	unsigned int voltage_ut:6;
	unsigned int power_control:5;
	unsigned int voltage_pa:6;
	// s_group_reg (2) 地址
	// channel enable
	unsigned int channel_enable ;
	// s_group_reg (3) 地址
	// fpga freeze and enable
	unsigned int reserved3:31   ;
	unsigned int fpga_freeze:1  ;
	// s_group_reg (4-15) 地址
    unsigned int reserved4[11];  	//4-15寄存器
    /* reg(15) 地址 */
    unsigned int band:3;            //0-2
    unsigned int res15:29;             //3-31
	// s_group_reg (16) 地址
    unsigned int freqency:3; 		//0-2
    unsigned int output_switch:5; 	//3-7
    unsigned int reserved16:24; 		//8-31
	// s_group_reg (17-19) 地址
	// s_group_reg (20-22) 地址
	// s_group_reg (23-25) 地址
    output_alarm_t alarm[3];

	// s_group_reg (26-27) 地址
    output_analog_t analog[2];

	// s_group_reg (28) 地址
    unsigned int reserved28:20; 			//0-19
    unsigned int factor_echo:12; 		//20-32  4095/回波数

	// s_group_reg (29 31) 地址
    unsigned int reserved29[3];
}
	output_to_spi_t;

typedef enum{//输出端口
	OUTPUT_ALARM1 = 0,
	OUTPUT_ALARM2 = 1,
	OUTPUT_ALARM3 = 2,
	OUTPUT_ALARM_MAX = 3,
	OUTPUT_ANALOG1 = 0,
	OUTPUT_ANALOG2 = 1,
	OUTPUT_ANALOG_MAX = 2,
	OUTPUT_1 = 0,
	OUTPUT_2 = 1,
	OUTPUT_3 = 2,
	OUTPUT_4 = 3,
	OUTPUT_5 = 4,
	OUTPUT_MAX = 5,
}
	OUTPUT_ENUM;

typedef enum{//命令

	OUTPUT_COMMAND_SWITCH,
	OUTPUT_COMMAND_BEEP_FREQUENCY,

	OUTPUT_ALARM_COMMAND_ENABLED_GROUP,
	OUTPUT_ALARM_COMMAND_CONDITION1,
	OUTPUT_ALARM_COMMAND_OPERATOR,
	OUTPUT_ALARM_COMMAND_CONDITION2,
	OUTPUT_ALARM_COMMAND_COUNT,
	OUTPUT_ALARM_COMMAND_DELAY,
	OUTPUT_ALARM_COMMAND_HOLD_TIME,

	OUTPUT_ANALOG_COMMAND_ENABLED_GROUP,
	OUTPUT_ANALOG_COMMAND_SOURCE,
	OUTPUT_ANALOG_COMMAND_FACTOR_ECHO,
/*************************************************************
	OUTPUT_OTHER_COMMAND_VOLTAGE_UT,//发射电压
	OUTPUT_OTHER_COMMAND_VOLTAGE_PA,//发射电压
	OUTPUT_OTHER_COMMAND_DAMPING,//阻尼
	OUTPUT_OTHER_COMMAND_TWIN_SWITCH,//双晶
	OUTPUT_OTHER_COMMAND_TWIN_UT,//UT

	OUTPUT_OTHER_COMMAND_ENCODE_X,
	OUTPUT_OTHER_COMMAND_ENCODE_Y,
**************************************************************/
	OUTPUT_OTHER_COMMAND_LAW_QTY,
	OUTPUT_OTHER_COMMAND_LAW_QTY_VIRTUAL ,
	OUTPUT_OTHER_COMMAND_VOLTAGE_UT,//发射电压
	OUTPUT_OTHER_COMMAND_VOLTAGE_PA,//发射电压
	OUTPUT_OTHER_COMMAND_DAMPING_UT1,//阻尼
	OUTPUT_OTHER_COMMAND_DAMPING_UT2,//阻尼
	OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT1,//双晶
	OUTPUT_OTHER_COMMAND_TWIN_SWITCH_UT2,//双晶
	OUTPUT_OTHER_COMMAND_POWER_CONTROL,
	//OUTPUT_OTHER_COMMAND_TWIN_UT,//UT
	OUTPUT_OTHER_COMMAND_ENCODE_X,
	OUTPUT_OTHER_COMMAND_ENCODE_Y,

	OUTPUT_OTHER_COMMAND_CHANNEL_ENABLE ,
	OUTPUT_OTHER_COMMAND_FPGA_FROZEN ,

    OUTPUT_OTHER_COMMAND_BAND,

	OUTPUT_COMMAND_MAX
}
OUTPUT_COMMAND_ENUM;//command命令列表

extern void output_init(unsigned int nChannelEnableFlag_);//spi alarm数据初始化
extern void SetChannelEnableFlag(unsigned int nChannelEnableFlag_) ;
extern void output_write_all_to_spi();//写入全部数据到alarm spi寄存器
extern void output_write_one_reg_to_spi(OUTPUT_ENUM output ,OUTPUT_COMMAND_ENUM command);//写入到单个spi寄存器，部分参数的命令写入的寄存器相同，写一次即可
extern unsigned int output_get_parameter(OUTPUT_ENUM output ,OUTPUT_COMMAND_ENUM command);//获取参数的值
extern void output_set_parameter(OUTPUT_ENUM output ,OUTPUT_COMMAND_ENUM command ,unsigned int value ,unsigned int value2);//设置参数的值

enum{
	ALARM_FREQ_SHUTDOWN,
	ALARM_FREQ_300HZ,
	ALARM_FREQ_600HZ,
	ALARM_FREQ_1000HZ,
	ALARM_FREQ_5000HZ,
	ALARM_FREQ_MAX
};

enum{
	ALARM_GROUP_COMMAND_OFF,
	ALARM_GROUP_COMMAND_ON,
	ALARM_GROUP_COMMAND_ALL,
	ALARM_GROUP_COMMAND_NONE,
	ALARM_GROUP_COMMAND_MAX,
};

enum{
	ALARM_CONDITION_NONE,
	ALARM_CONDITION_GATE_A,
	ALARM_CONDITION_GATE_B,
	ALARM_CONDITION_GATE_I,
	ALARM_CONDITION_NOT_GATE_A,
	ALARM_CONDITION_NOT_GATE_B,
	ALARM_CONDITION_NOT_GATE_I,
	ALARM_CONDITION_MAX_THICKNESS,
	ALARM_CONDITION_MIN_THICKNESS,
	ALARM_CONDITION_MAX
};

enum{
	ALARM_OPERATOR_AND,
	ALARM_OPERATOR_OR,
	ALARM_OPERATOR_MAX
};

enum{
	SOURCE_THICKNESS_PEAK_A,
	SOURCE_THICKNESS_PEAK_A_I,
	SOURCE_THICKNESS_PEAK_A_GATE_I,
	SOURCE_THICKNESS_PEAK_B,
	SOURCE_THICKNESS_PEAK_B_I,
	SOURCE_THICKNESS_PEAK_B_GATE_I,
	SOURCE_THICKNESS_PEAK_B_A,
	SOURCE_THICKNESS_PEAK_I,
	SOURCE_THICKNESS_MAX
};

enum{
	ANALOG_SOURCE_OFF,
	ANALOG_SOURCE_PEAK_A,
	ANALOG_SOURCE_PEAK_B,
	ANALOG_SOURCE_THICKNESS,
	ANALOG_SOURCE_MAX
};

enum{
	VOLTAGE_PA_50V,
	VOLTAGE_PA_100V,
	VOLTAGE_PA_200V,
	VOLTAGE_PA_400V,
	VOLTAGE_PA_MAX,
};
/*
typedef enum{//参数，只含alarm的
	ALARM_PARAMETER_ENABLED_GROUP,
	ALARM_PARAMETER_CONDITION1,
	ALARM_PARAMETER_OPERATOR,
	ALARM_PARAMETER_CONDITION2,
	ALARM_PARAMETER_COUNT,
	ALARM_PARAMETER_DELAY,
	ALARM_PARAMETER_HOLD_TIME,
	ALARM_PARAMETER_MAX,
}
	ALARM_PARAMETER_ENUM;

typedef enum{//参数，只含ANALOG的
	ANALOG_PARAMETER_ENABLED_GROUP,
	ANALOG_PARAMETER_SOURCE,
	ANALOG_PARAMETER_MAX
}
	ANALOG_PARAMETER_ENUM;

enum{//参数，只含ANALOG的
	ANALOG_COMMAND_ENABLED_GROUP,
	ANALOG_COMMAND_SOURCE_THICKNESS,
	ANALOG_COMMAND_SOURCE,
	ANALOG_COMMAND_FACTOR,
	ANALOG_COMMAND_FACTOR_ECHO,
	ANALOG_COMMAND_MAX
};
//*/

void OutputSave();
#endif /* OUTPUT_SPI_H_ */

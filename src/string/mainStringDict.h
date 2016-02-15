/*
 * stringDict.h
 *
 *  Created on: 2012-7-12
 *      Author: lzk
 */

#ifndef MAINSTRINGDICT_H_
#define MAINSTRINGDICT_H_

#include "language.h"
static _STRING_ENUM group_dict[] =//0
{
		_STRING_Group_1  ,_STRING_Group_2  ,_STRING_Group_3  ,_STRING_Group_4 ,
		_STRING_Group_5  ,_STRING_Group_6  ,_STRING_Group_7  ,_STRING_Group_8 ,
		_STRING_Group_9  ,_STRING_Group_10  ,_STRING_Group_11  ,_STRING_Group_12 ,
		_STRING_Group_13  ,_STRING_Group_14  ,_STRING_Group_15  ,_STRING_Group_16 ,

		_STRING_Group_1_ ,_STRING_Group_2_ ,_STRING_Group_3_ ,_STRING_Group_4_ ,
		_STRING_Group_5_ ,_STRING_Group_6_ ,_STRING_Group_7_ ,_STRING_Group_8_ ,
		_STRING_Group_9_ ,_STRING_Group_10_ ,_STRING_Group_11_ ,_STRING_Group_12_ ,
		_STRING_Group_13_ ,_STRING_Group_14_ ,_STRING_Group_15_ ,_STRING_Group_16_ ,
};

static _STRING_ENUM add_dict[] = {_STRING_Add ,_STRING_Remove};//1

static _STRING_ENUM geometry_dict[] ={_STRING_Plate ,_STRING_OD ,_STRING_ID};//2

static _STRING_ENUM group_mode_dict[] =//3
{
		_STRING_UT__PA_Connector_ ,_STRING_PA_Phassed_Array ,_STRING_UT1_Conventional_UT_ ,_STRING_UT2_Conventional_UT_
};

static _STRING_ENUM simple_group_mode_dict[] =//4
{
		_STRING_UT ,_STRING_PA ,_STRING_UT1 ,_STRING_UT2
};

static _STRING_ENUM tx_rx_mode_dict[] =//5
{
		_STRING_PC_Pitch_and_Catch ,_STRING_PE_Pulse_Echo ,_STRING_TT_Through_Transmission ,_STRING_TOFD_Time_of_Flight_Diffraction
};

static _STRING_ENUM simple_tx_rx_mode_dict[] =//6
{
		_STRING_PC ,_STRING_PE ,_STRING_TT ,_STRING_TOFD
};

static _STRING_ENUM select_pe_probe_dict[] = //7
{
		_STRING_Select_Tx_Rx ,_STRING_Defines
};

static _STRING_ENUM select_probe_dict[] =//8
{
		_STRING_Select_Tx	,_STRING_Select_Rx	,_STRING_Defines
};

static _STRING_ENUM material_dict[] =//10
{
		/*	_STRING_AL_OXIDE, "ALUMINUM", _STRING_BERYLIUM, "BRASS", "CADMIUM", "COPPER", "STEEL.MILD",	*/		/* 399 Material */
		_STRING_Aluminum ,_STRING_Steel_common ,_STRING_Steel_stainless ,_STRING_Brass ,_STRING_Copper ,
		_STRING_Iron ,_STRING_Lead ,_STRING_Nylon,_STRING_Silver ,_STRING_Gold ,_STRING_Zinc ,_STRING_Titanium ,
		_STRING_Tin ,_STRING_Epoxy_resin ,_STRING_Ice ,_STRING_Nickel ,_STRING_Plexiglass ,_STRING_Polystyrene ,
		_STRING_Porcelain ,_STRING_PVC ,_STRING_Quartz_glass ,_STRING_Rubber_vulcanized ,_STRING_Teflon ,_STRING_Water ,	/* 24 个材料 */
};

static _STRING_ENUM law_type_dict[] =//13
{
		_STRING_Azimuthal_sector, _STRING_Linear_sector_,
		_STRING_Depth_sector, _STRING_Static_sector_,
};

static _STRING_ENUM simple_law_type_dict[] =//14
{
		_STRING_Azimuthal, _STRING_Linear, _STRING_Depth, _STRING_Static,
};


static _STRING_ENUM focal_type_dict[] =//15
{
		_STRING_Half_Path, _STRING_True_Depth, _STRING_Projection, _STRING_Focal_Plane , _STRING_Automatic,
};

static _STRING_ENUM wave_type_dict[] = //16
{
		_STRING_LW__5890_0m_s ,_STRING_SW__3240_0m_s
};

static _STRING_ENUM simple_wave_type_dict[] = //17
{
		_STRING_LW ,_STRING_SW
};


static _STRING_ENUM group_select_dict[] = //18
{
		_STRING_STICK ,
		_STRING_All_ ,_STRING_None ,_STRING_Multi ,
};

static _STRING_ENUM output_dict[] = //19
{
		_STRING_Alarm__1 			,_STRING_Alarm__2		,_STRING_Alarm__3			,_STRING_Analog_1 			,_STRING_Analog_2,
		_STRING_Alarm__1___On_ 	,_STRING_Alarm__2___On_ 	,_STRING_Alarm__3___On_ 	,_STRING_Analog_1___On_ 	,_STRING_Analog_2___On_,
};

static _STRING_ENUM condition_dict[] =//20
{
		_STRING_None ,_STRING_Gate_A ,_STRING_Gate_B, _STRING_Gate_I,
		_STRING_Not_Gate_A ,_STRING_Not_Gate_B ,_STRING_Not_Gate_I ,_STRING_Max_Thickness_ ,_STRING_Min_Thickness_
};

static _STRING_ENUM operator_dict[] = //21
{
		_STRING_AND ,_STRING_OR
};

static _STRING_ENUM sound_dict[] = //22
{
		_STRING_Off__No_Sound, _STRING_300Hz__Audio_output_at_300_Hz ,_STRING_600Hz__Audio_output_at_600_Hz,
		_STRING_1000Hz__Audio_output_at_1000_Hz ,_STRING_5000Hz__Audio_output_at_5000_Hz
};

static _STRING_ENUM simple_sound_dict[] = //23
{
		_STRING_Off, _STRING_300Hz ,_STRING_600Hz ,_STRING_1000Hz, _STRING_5000Hz,
};

static _STRING_ENUM analog_data_dict[] = //24
{
		_STRING_Off, _STRING_A_33, _STRING_B_, _STRING_Thickness_,
};

static _STRING_ENUM switch_dict[] = //25
{
		_STRING_Off ,_STRING_On ,
};

static _STRING_ENUM cmode_ut_dict[] =
{
		_STRING_Velocity_ ,_STRING_Wedge_Delay ,_STRING_Velocity_And_Delay
};

static _STRING_ENUM group_step_dict[] = //9
{
		_STRING_Start_s ,_STRING_Group_Select ,_STRING_Add ,_STRING_Material ,_STRING_Group_Mode
		,_STRING_Tx_Rx_Mode ,_STRING_Probe ,_STRING_Wedge ,_STRING_Position ,_STRING_Finish
};

static _STRING_ENUM foclaw_step_dict[] =//12
{
		_STRING_Start_s ,_STRING_Group_Select ,_STRING_Law_Config ,_STRING_Focal_Type ,
		_STRING_Wave_Type ,_STRING_Probe_Elements ,_STRING_Beam_Angle ,_STRING_Generate ,
};

static _STRING_ENUM weld_dict[] =
{
		_STRING_Manual_Weld ,_STRING_Auto_Weld ,
};

static _STRING_ENUM fileType_dict[] =
{
        _STRING_Setup ,_STRING_Data ,_STRING_Report ,_STRING_Image_ , _STRING_CAD, _STRING_All_ ,
};

static _STRING_ENUM fileList_dict[] =
{
		_STRING_User ,_STRING_Sample
};

static _STRING_ENUM tableTitle_dict[] =
{
		_STRING_Entry ,_STRING_Scan_t ,_STRING_Index_t ,_STRING_Group ,_STRING_Channel ,_STRING_Comments,
};

static _STRING_ENUM copy_src_dict[] = {_STRING_No_CopySrc,};

static const stringDictStruct mainStringDict[] =
{
	/*MAINSTRINGDICT_NONE*/				{NULL 						,0},
	/*MAINSTRINGDICT_GROUP*/			{group_dict 				,NUM_OF_STRING_INDEXS(group_dict)} ,
	/*MAINSTRINGDICT_ADD*/				{add_dict 					,NUM_OF_STRING_INDEXS(add_dict)} ,
	/*MAINSTRINGDICT_NOCOPY*/			{copy_src_dict 				,NUM_OF_STRING_INDEXS(copy_src_dict)} ,
	/*MAINSTRINGDICT_GEOMETRY*/			{geometry_dict 				,NUM_OF_STRING_INDEXS(geometry_dict)},
	/*MAINSTRINGDICT_GROUP_MODE*/		{group_mode_dict 			,NUM_OF_STRING_INDEXS(group_mode_dict)},
	/*MAINSTRINGDICT_S_GROUP_MODE*/		{simple_group_mode_dict 	,NUM_OF_STRING_INDEXS(simple_group_mode_dict)},
	/*MAINSTRINGDICT_TR_MODE*/			{tx_rx_mode_dict 			,NUM_OF_STRING_INDEXS(tx_rx_mode_dict)},
	/*MAINSTRINGDICT_S_TR_MODE*/		{simple_tx_rx_mode_dict 	,NUM_OF_STRING_INDEXS(simple_tx_rx_mode_dict)},
	/*MAINSTRINGDICT_PE_SELECT_PROBE*/	{select_pe_probe_dict 		,NUM_OF_STRING_INDEXS(select_pe_probe_dict)},
	/*MAINSTRINGDICT_SELECT_PROBE*/		{select_probe_dict 			,NUM_OF_STRING_INDEXS(select_probe_dict)},
	/*MAINSTRINGDICT_MATERIAL*/			{material_dict 				,NUM_OF_STRING_INDEXS(material_dict)},
									//wizard focallaw 010
	/*MAINSTRINGDICT_LAW_TYPE*/			{law_type_dict 				,  2 /*NUM_OF_STRING_INDEXS(law_type_dict)*/},
	/*MAINSTRINGDICT_S_LAW_TYPE*/		{simple_law_type_dict		,NUM_OF_STRING_INDEXS(simple_law_type_dict)},
	/*MAINSTRINGDICT_FOCAL_TYPE*/		{focal_type_dict			, 4 /*NUM_OF_STRING_INDEXS(focal_type_dict)*/},
	/*MAINSTRINGDICT_WAVE_TYPE*/		{wave_type_dict				,NUM_OF_STRING_INDEXS(wave_type_dict)},
	/*MAINSTRINGDICT_S_WAVE_TYPE*/		{simple_wave_type_dict		,NUM_OF_STRING_INDEXS(simple_wave_type_dict)},
									//output menu210 220
	/*MAINSTRINGDICT_GROUP_SELECT*/		{group_select_dict			,NUM_OF_STRING_INDEXS(group_select_dict)},
	/*MAINSTRINGDICT_OUTPUT*/			{output_dict				,NUM_OF_STRING_INDEXS(output_dict)},
	/*MAINSTRINGDICT_CONDITION*/		{condition_dict				,NUM_OF_STRING_INDEXS(condition_dict)},
	/*MAINSTRINGDICT_OPERATOR*/			{operator_dict				,NUM_OF_STRING_INDEXS(operator_dict)},
	/*MAINSTRINGDICT_SOUND*/			{sound_dict					,NUM_OF_STRING_INDEXS(sound_dict)},
	/*MAINSTRINGDICT_S_SOUND*/			{simple_sound_dict			,NUM_OF_STRING_INDEXS(simple_sound_dict)},
	/*MAINSTRINGDICT_ANALOG_DATA*/		{analog_data_dict			,NUM_OF_STRING_INDEXS(analog_data_dict)},
	/*MAINSTRINGDICT_SWITCH*/			{switch_dict				,NUM_OF_STRING_INDEXS(switch_dict)},
	/*MAINSTRINGDICT_CMODE_UT*/			{cmode_ut_dict				,NUM_OF_STRING_INDEXS(cmode_ut_dict)},

	/*MAINSTRINGDICT_WELD*/				{weld_dict					,NUM_OF_STRING_INDEXS(weld_dict)},

	/*MAINSTRINGDICT_STEP_GROUP*/		{group_step_dict			,NUM_OF_STRING_INDEXS(group_step_dict)},
	/*MAINSTRINGDICT_STEP_FOCALLAW*/	{foclaw_step_dict			,NUM_OF_STRING_INDEXS(foclaw_step_dict)},

	/*MAINSTRINGDICT_FILE_TYPE*/		{fileType_dict				,NUM_OF_STRING_INDEXS(fileType_dict)},
	/*MAINSTRINGDICT_FILE_LIST*/		{fileList_dict				,NUM_OF_STRING_INDEXS(fileList_dict)},
	/*MAINSTRINGDICT_TABLE_TITLE*/		{tableTitle_dict			,NUM_OF_STRING_INDEXS(tableTitle_dict)},

	/*MAINSTRINGDICT_MAX*/
};
#endif /* MAINSTRINGDICT_H_ */

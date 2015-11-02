/*
 * string_d.h
 *
 *  Created on: 2012-7-12
 *      Author: lzk
 */

#ifndef STRING_D_H_
#define STRING_D_H_

typedef enum
{
	MAINSTRINGDICT_NONE,//				{NULL 						,0},

	MAINSTRINGDICT_GROUP,//	{group_dict ,NUM_OF_STRING_INDEXS(group_dict)} ,
	MAINSTRINGDICT_ADD,//		{add_dict ,NUM_OF_STRING_INDEXS(add_dict)} ,
	MAINSTRINGDICT_NOCOPY,//			{copy_src_dict 				,NUM_OF_STRING_INDEXS(copy_src_dict)} ,
	MAINSTRINGDICT_GEOMETRY,//			{geometry_dict 				,NUM_OF_STRING_INDEXS(geometry_dict)},
	MAINSTRINGDICT_GROUP_MODE,//		{group_mode_dict 			,NUM_OF_STRING_INDEXS(group_mode_dict)},
	MAINSTRINGDICT_S_GROUP_MODE,//		{simple_group_mode_dict 	,NUM_OF_STRING_INDEXS(simple_group_mode_dict)},
	MAINSTRINGDICT_TR_MODE,//			{tx_rx_mode_dict 			,NUM_OF_STRING_INDEXS(tx_rx_mode_dict)},
	MAINSTRINGDICT_S_TR_MODE,//		{simple_tx_rx_mode_dict 	,NUM_OF_STRING_INDEXS(simple_tx_rx_mode_dict)},
	MAINSTRINGDICT_PE_SELECT_PROBE,//	{select_pe_probe_dict 		,NUM_OF_STRING_INDEXS(select_pe_probe_dict)},
	MAINSTRINGDICT_SELECT_PROBE,//		{select_probe_dict 			,NUM_OF_STRING_INDEXS(select_probe_dict)},
	MAINSTRINGDICT_MATERIAL,//			{material_dict 				,NUM_OF_STRING_INDEXS(material_dict)},
									//wizard focallaw 010
	MAINSTRINGDICT_LAW_TYPE,//			{law_type_dict 				,NUM_OF_STRING_INDEXS(law_type_dict)},
	MAINSTRINGDICT_S_LAW_TYPE,//		{simple_law_type_dict		,NUM_OF_STRING_INDEXS(simple_law_type_dict)},
	MAINSTRINGDICT_FOCAL_TYPE,//		{focal_type_dict			,NUM_OF_STRING_INDEXS(focal_type_dict)},
	MAINSTRINGDICT_WAVE_TYPE,//		{wave_type_dict				,NUM_OF_STRING_INDEXS(wave_type_dict)},
	MAINSTRINGDICT_S_WAVE_TYPE,//		{simple_wave_type_dict		,NUM_OF_STRING_INDEXS(simple_wave_type_dict)},
									//output menu210 220
	MAINSTRINGDICT_GROUP_SELECT,//		{group_select_dict			,NUM_OF_STRING_INDEXS(group_select_dict)},
	MAINSTRINGDICT_OUTPUT,//			{output_dict				,NUM_OF_STRING_INDEXS(output_dict)},
	MAINSTRINGDICT_CONDITION,//		{condition_dict				,NUM_OF_STRING_INDEXS(condition_dict)},
	MAINSTRINGDICT_OPERATOR,//			{operator_dict				,NUM_OF_STRING_INDEXS(operator_dict)},
	MAINSTRINGDICT_SOUND,//			{sound_dict					,NUM_OF_STRING_INDEXS(sound_dict)},
	MAINSTRINGDICT_S_SOUND,//			{simple_sound_dict			,NUM_OF_STRING_INDEXS(simple_sound_dict)},
	MAINSTRINGDICT_ANALOG_DATA,//		{analog_data_dict			,NUM_OF_STRING_INDEXS(analog_data_dict)},
	MAINSTRINGDICT_SWITCH,//			{switch_dict				,NUM_OF_STRING_INDEXS(switch_dict)},
	MAINSTRINGDICT_CMODE_UT,//			{cmode_ut_dict				,NUM_OF_STRING_INDEXS(cmode_ut_dict)},

	MAINSTRINGDICT_WELD,//				{weld_dict				,NUM_OF_STRING_INDEXS(weld_dict)},

	MAINSTRINGDICT_STEP_GROUP,//		{group_step_dict			,NUM_OF_STRING_INDEXS(group_step_dict)},
	MAINSTRINGDICT_STEP_FOCALLAW,//	{foclaw_step_dict			,NUM_OF_STRING_INDEXS(foclaw_step_dict)},

	MAINSTRINGDICT_FILE_TYPE,//		{fileType_dict				,NUM_OF_STRING_INDEXS(fileType_dict)},
	MAINSTRINGDICT_FILE_LIST,//		{fileList_dict				,NUM_OF_STRING_INDEXS(fileList_dict)},
	MAINSTRINGDICT_TABLE_TITLE,//		{tableTitle_dict			,NUM_OF_STRING_INDEXS(tableTitle_dict)},

	MAINSTRINGDICT_MAX,//
}
	MAINSTRINGDICT_ENUM;

typedef enum
{
	REPORTSTRINGDICT_HEAD1,//				{head1 					,NUM_OF_STRING_INDEXS(head1)},
	REPORTSTRINGDICT_HEAD2,//				{head2 					,NUM_OF_STRING_INDEXS(head2)},
	REPORTSTRINGDICT_PROBE1,//				{probe1 				,NUM_OF_STRING_INDEXS(probe1)},
	REPORTSTRINGDICT_PROBE2,//				{probe2 				,NUM_OF_STRING_INDEXS(probe2)},
	REPORTSTRINGDICT_PROBE3,//				{probe3 				,NUM_OF_STRING_INDEXS(probe3)},
	REPORTSTRINGDICT_PROBE4,//				{probe4 				,NUM_OF_STRING_INDEXS(probe4)},
	REPORTSTRINGDICT_SETUP1,//				{setup1 				,NUM_OF_STRING_INDEXS(setup1)},
	REPORTSTRINGDICT_SETUP2,//				{setup2 				,NUM_OF_STRING_INDEXS(setup2)},
	REPORTSTRINGDICT_SETUP3,//				{setup3 				,NUM_OF_STRING_INDEXS(setup3)},
	REPORTSTRINGDICT_SETUP4,//				{setup4 				,NUM_OF_STRING_INDEXS(setup4)},
	REPORTSTRINGDICT_SETUP5,//				{setup5 				,NUM_OF_STRING_INDEXS(setup5)},
	REPORTSTRINGDICT_CALCULATOR1,//		{calculator1 			,NUM_OF_STRING_INDEXS(calculator1)},
	REPORTSTRINGDICT_CALCULATOR2,//		{calculator2 			,NUM_OF_STRING_INDEXS(calculator2)},
	REPORTSTRINGDICT_PART,//				{part 					,NUM_OF_STRING_INDEXS(part)},
	REPORTSTRINGDICT_SCANAREA1,//			{scanArea1 				,NUM_OF_STRING_INDEXS(scanArea1)},
	REPORTSTRINGDICT_SCANAREA2,//			{scanArea2 				,NUM_OF_STRING_INDEXS(scanArea2)},
	REPORTSTRINGDICT_SCANAREA3,//			{scanArea3 				,NUM_OF_STRING_INDEXS(scanArea3)},
	REPORTSTRINGDICT_SIGNATURE,//			{signature 				,NUM_OF_STRING_INDEXS(signature)},
	REPORTSTRINGDICT_TITLE,//				{title 					,NUM_OF_STRING_INDEXS(title)},
	REPORTSTRINGDICT_FOCALTYPE1,//			{focalType1 			,NUM_OF_STRING_INDEXS(focalType1)},
	REPORTSTRINGDICT_FOCALTYPE2,//			{focalType2 			,NUM_OF_STRING_INDEXS(focalType2)},
	REPORTSTRINGDICT_FOCALTYPE3,//			{focalType3 			,NUM_OF_STRING_INDEXS(focalType3)},

	REPORTSTRINGDICT_MAX,//
}
	REPORTSTRINGDICT_ENUM;


typedef enum
{
	COMMONSTRING_TMP_HTML,//				{tmpHtmlDict 					,NUM_OF_STRING(tmpHtmlDict)},
	COMMONSTRING_UNIT,//					{unit_dict				,NUM_OF_STRING(unit_dict)},
	COMMONSTRING_DATA_PATH,//				{dataPath_dict				,NUM_OF_STRING(dataPath_dict)},
	COMMONSTRING_DATA_SUFFIX,//			{dataSuffix_dict 				,NUM_OF_STRING(dataSuffix_dict)},

	COMMONSTRING_MAX,//
}
	COMMONSTRING_ENUM;

#endif /* STRING_D_H_ */

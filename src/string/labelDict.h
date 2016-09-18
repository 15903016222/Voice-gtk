/*
 * labelDict.h
 *
 *  Created on: 2012-7-12
 *      Author: lzk
 */

#ifndef LABELDICT_H_
#define LABELDICT_H_
#include "language.h"

static CONST_STRING_ENUM menu000_dict[] =  {_STRING_Back 		,_STRING_Start_s    ,_STRING_NULL    	,_STRING_NULL 			,_STRING_NULL 			,_STRING_NULL };
static CONST_STRING_ENUM menu001_dict[] =  {_STRING_Back 		,_STRING_Next 		,_STRING_Group_S 	,_STRING_Delete 		,_STRING_NULL 			,_STRING_NULL };
static CONST_STRING_ENUM menu002_dict[] =  {_STRING_Back 		,_STRING_Finish 	,_STRING_Source__ 	,_STRING_NULL 			,_STRING_NULL 			,_STRING_NULL };
static CONST_STRING_ENUM menu003_dict[] =  {_STRING_Back 		,_STRING_Next 		,_STRING_Geometry_ 	,_STRING_Thickness_ 	,_STRING_Diameter 		,_STRING_Material};
static CONST_STRING_ENUM menu004_dict[] =  {_STRING_Back 		,_STRING_Next 		,_STRING_Group_Mode ,_STRING_NULL 			,_STRING_NULL 			,_STRING_NULL };
static CONST_STRING_ENUM menu0051_dict[] = {_STRING_Back 		,_STRING_Next 		,_STRING_Tx_Rx_Mode	,_STRING_Pulser_		,_STRING_Receiver_ 		,_STRING_NULL };
static CONST_STRING_ENUM menu0052_dict[] = {_STRING_Back 		,_STRING_Next 		,_STRING_Tx_Rx_Mode	,_STRING_Connection_P	,_STRING_Connection_R 	,_STRING_NULL };
static CONST_STRING_ENUM menu0061_dict[] =  {_STRING_Back 	,_STRING_Next 		,_STRING_Select 	,_STRING_Select_Probe	,_STRING_Auto_Detect	,_STRING_NULL };
static CONST_STRING_ENUM menu0062_dict[] =  {_STRING_Back 	,_STRING_Next 		,_STRING_Select		,_STRING_Define_Probe	,_STRING_Auto_Detect	,_STRING_NULL };
static CONST_STRING_ENUM menu0071_dict[] =  {_STRING_Back 	,_STRING_Next 		,_STRING_Select 	,_STRING_Select_Wedge	,_STRING_NULL 			,_STRING_NULL };
static CONST_STRING_ENUM menu0072_dict[] =  {_STRING_Back 	,_STRING_Next 		,_STRING_Select 	,_STRING_Define_Wedge	,_STRING_NULL 			,_STRING_NULL };
static CONST_STRING_ENUM menu008_dict[] =  {_STRING_Back 		,_STRING_Next 		,_STRING_Scan_Offset,_STRING_Index_Offset	,_STRING_Skew 			,_STRING_NULL };
static CONST_STRING_ENUM menu009_dict[] =  {_STRING_Back 		,_STRING_Finish   	,_STRING_Continue 	,_STRING_NULL 			,_STRING_NULL 			,_STRING_NULL };

static CONST_STRING_ENUM* menu00_dict[] =
{
		menu000_dict,//STEP_START
		menu001_dict,//STEP_GROUP_SELECT
		menu002_dict,//STEP_ADD
		menu003_dict,//STEP_MATERIAL
		menu004_dict,//STEP_GROUP_MODE
		menu0051_dict,//STEP_TX_RX_MODE
		menu0061_dict,//STEP_PROBE PE
		menu0071_dict,//STEP_WEDGE PE
		menu008_dict,//STEP_POSITION
		menu009_dict,//STEP_FINISH
		menu0052_dict,//STEP_TX_RX_MODE_PA
		menu0061_dict,//STEP_PROBE
		menu0071_dict,//STEP_WEDGE
		menu0062_dict,//WIZARD_GROUP_STEP_PROBE_DEFINE
		menu0072_dict,//WIZARD_GROUP_STEP_WEDGE_DEFINE

};

static CONST_STRING_ENUM menu010_dict[] =  {_STRING_Back 		,_STRING_Start_s    ,_STRING_NULL    		,_STRING_NULL 			,_STRING_NULL 			,_STRING_NULL };
static CONST_STRING_ENUM menu011_dict[] =  {_STRING_Back 		,_STRING_Next 		,_STRING_Group 			,_STRING_NULL 			,_STRING_NULL 			,_STRING_NULL };
static CONST_STRING_ENUM menu012_dict[] =  {_STRING_Back 		,_STRING_Next 		,_STRING_Law_Type		,_STRING_Focal_Type_	,_STRING_Wave_Type  ,_STRING_NULL };

static CONST_STRING_ENUM menu0130_dict[] =  {_STRING_Back 	,_STRING_Next 		,_STRING_Position_Start	,_STRING_Position_End	,_STRING_Position_Step 	,_STRING_NULL };
static CONST_STRING_ENUM menu0131_dict[] =  {_STRING_Back 	,_STRING_Next 		,_STRING_Offset_Start	,_STRING_Position_End	,_STRING_Position_Step 	,_STRING_Depth_End};
static CONST_STRING_ENUM menu0132_dict[] =  {_STRING_Back 	,_STRING_Next 		,_STRING_Offset_Start	,_STRING_Offset_End		,_STRING_Depth_Start  	,_STRING_Depth_End};
static CONST_STRING_ENUM menu0133_dict[] =  {_STRING_Back 	,_STRING_Next 		,_STRING_Focus_Half     ,_STRING_Position_End	,_STRING_Position_Step 	,_STRING_NULL };

static CONST_STRING_ENUM menu014_dict[] =  {_STRING_Back 		,_STRING_Next 		,_STRING_Wave_Type 		,_STRING_Material		,_STRING_NULL 			,_STRING_NULL };
static CONST_STRING_ENUM menu015_dict[] =  {_STRING_Back 		,_STRING_Next 		,_STRING_Element_Qty_	,_STRING_First_Element	,_STRING_Last_Element_	,_STRING_Element_Step};
static CONST_STRING_ENUM menu016_dict[] =  {_STRING_Back 		,_STRING_Next 		,_STRING_Min_Angle		,_STRING_Max_Angle		,_STRING_Angle_Step		,_STRING_Focus_Depth };
static CONST_STRING_ENUM menu017_dict[] =  {_STRING_Back 		,_STRING_Generate_ 	,_STRING_NULL			,_STRING_NULL 			,_STRING_NULL 			,_STRING_NULL };

static CONST_STRING_ENUM* menu01_dict[] =
{
		menu010_dict,
		menu011_dict,
		menu012_dict,
		menu0130_dict,
		menu014_dict,
		menu015_dict,
		menu016_dict,
		menu017_dict,
		menu0130_dict,
		menu0131_dict,
		menu0132_dict,
		menu0133_dict,
};

static CONST_STRING_ENUM** menu0_dict[] =
{
		menu00_dict,
		menu01_dict,
};

static CONST_STRING_ENUM** menu1_dict[] =
{

};

static CONST_STRING_ENUM menu200_dict[] =  {_STRING_NULL ,_STRING_NULL ,_STRING_NULL ,_STRING_NULL ,_STRING_NULL ,_STRING_NULL};
static CONST_STRING_ENUM* menu20_dict[] =  { menu200_dict };

static CONST_STRING_ENUM menu210_dict[] =  {_STRING_Alarm 	,_STRING_Group_S 	,_STRING_Condition1	,_STRING_Operator	,_STRING_Condition2	,_STRING_NULL};
static CONST_STRING_ENUM* menu21_dict[] =
{
		menu210_dict ,
		menu210_dict ,
		menu210_dict ,
};

static CONST_STRING_ENUM menu220_dict[] =  {_STRING_Output 	,_STRING_Switch 	,_STRING_Count		,_STRING_Sound	,_STRING_Delay_	,_STRING_Hold_Time};
static CONST_STRING_ENUM menu223_dict[] =  {_STRING_Output 	,_STRING_Switch 	,_STRING_Group_S	,_STRING_Data_	,_STRING_NULL			,_STRING_NULL};
static CONST_STRING_ENUM* menu22_dict[] =
{
		menu220_dict,
		menu220_dict,
		menu220_dict,
		menu223_dict,
		menu223_dict,
};

static CONST_STRING_ENUM** menu2_dict[] =
{
		menu20_dict,
		menu21_dict,
		menu22_dict,
};

static CONST_STRING_ENUM menu310_dict[] =  {_STRING_NULL ,_STRING_NULL ,_STRING_NULL,_STRING_NULL   ,_STRING_NULL, _STRING_RefLine};
static CONST_STRING_ENUM* menu31_dict[] =
{
    menu310_dict,
};

static CONST_STRING_ENUM menu320_dict[] =  {_STRING_Display_Table ,_STRING_Entry_Image ,_STRING_Add_Entry	,_STRING_Delete_Entry ,_STRING_Select_Entry	,_STRING_Edit_Comments};
static CONST_STRING_ENUM* menu32_dict[] =
{
		menu320_dict,
};

static CONST_STRING_ENUM menu340_dict[] =  {
    _STRING_NULL ,_STRING_Straightening ,_STRING_Remove_Lateral_Wave	, _STRING_RefLine_Depth, _STRING_DepthCal,_STRING_WedgeSeparation//_STRING_PCS//
};

static CONST_STRING_ENUM* menu34_dict[] =
{
		menu340_dict,
};
static CONST_STRING_ENUM** menu3_dict[] =
{
		NULL,
		menu31_dict,
		menu32_dict,
		NULL,
		menu34_dict,
};

static CONST_STRING_ENUM** menu4_dict[] =
{

};

static CONST_STRING_ENUM** menu5_dict[] =
{

};

static CONST_STRING_ENUM** menu6_dict[] =
{

};

static CONST_STRING_ENUM** menu7_dict[] =
{

};

static CONST_STRING_ENUM** menu8_dict[] =
{

};

static CONST_STRING_ENUM menu940_dict[] = {	/*_STRING_Software_Update*/_STRING_Calibrate_Touch_Screen ,_STRING_Hardware_Update ,_STRING_Reset_Config ,_STRING_ABOUT ,_STRING_NULL ,_STRING_NULL    };

static CONST_STRING_ENUM* menu94_dict[] = {		menu940_dict, };

static CONST_STRING_ENUM** menu9_dict[] =
{
		menu94_dict,
		menu94_dict,
		menu94_dict,
		menu94_dict,
		menu94_dict,
};

static CONST_STRING_ENUM*** menuLabelDict[] =
{
		menu0_dict,
		menu1_dict,
		menu2_dict,
		menu3_dict,
		menu4_dict,
		menu5_dict,
		menu6_dict,
		menu7_dict,
		menu8_dict,
		menu9_dict,
};


#endif /* LABELDICT_H_ */

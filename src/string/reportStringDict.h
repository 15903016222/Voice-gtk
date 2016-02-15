/*
 * reportString.h
 *
 *  Created on: 2012-7-12
 *      Author: lzk
 */

#ifndef REPORTSTRINGDICT_H_
#define REPORTSTRINGDICT_H_

#include "language.h"
static _STRING_ENUM head1[] =
{
		_STRING_Report_Date, _STRING_Report_Version ,_STRING_Setup_File_Name ,
		_STRING_Inspection_Date ,_STRING_Inspection_Version ,_STRING_Save_Mode
};

static _STRING_ENUM head2[] =
{
		_STRING_Doppler_Type ,_STRING_Data_File_Name  ,_STRING_HTML_SPACE//,"Doppler Serial" ,"Module Type" ,"Module Serial #" ,"Cailbration Due"
};

static _STRING_ENUM probe1[] =
{
		_STRING_Probe_Model ,_STRING_Probe_Serial__ ,_STRING_HTML_SPACE
};

static _STRING_ENUM probe2[] =
{
		_STRING_Probe_Frequency ,_STRING_Peak_Frequency ,_STRING_Wedge_Model ,_STRING_Wedge_Angle ,_STRING_Probe_Aperture ,_STRING_HTML_SPACE
};

static _STRING_ENUM probe3[] =
{
		_STRING_HTML_SPACE ,_STRING_Lower_Frequency ,_STRING_Higher_Frequency ,
		_STRING_Center_Frequency ,_STRING_Bandwidth__MHz_ ,_STRING_Bandwidth____
};

static _STRING_ENUM probe4[] =
{
		_STRING_Date ,_STRING_Time ,_STRING_Procedure ,_STRING_Calibration_Block ,_STRING_Characterization_Gain ,_STRING_A_33
};

static _STRING_ENUM setup1[] =
{
		_STRING_Beam_Delay ,_STRING_Start__Half_Path_ ,_STRING_Range__Half_Path_ ,_STRING_PRF ,_STRING_Type_m ,_STRING_Averaging_Factor
};

static _STRING_ENUM setup2[] =
{
		_STRING_Scale_Factor ,_STRING_Video_Filter ,_STRING_Rectification_ ,_STRING_Band_Pass_Filter ,_STRING_HTML_SPACE//"Scale Type" ,"Pretrig."
};

static _STRING_ENUM setup3[] =
{
		_STRING_Voltage ,_STRING_Gain ,_STRING_Mode ,_STRING_Wave_Type ,_STRING_Sound_Velocity ,_STRING_Pulse_Width
};

static _STRING_ENUM setup4[] =
{
		_STRING_Scan_Offset ,_STRING_Index_Offset ,_STRING_Skew ,_STRING_HTML_SPACE
};

static _STRING_ENUM setup5[] =
{
		_STRING_Gate ,_STRING_Start___ ,_STRING_Width_ ,_STRING_Threshold ,_STRING_Synchro ,_STRING_HTML_SPACE
};

static _STRING_ENUM calculator1[] =
{
		_STRING_Used_Element_Qty_ ,_STRING_First_TX_Element ,_STRING_Last_TX_Element ,
		_STRING_First_RX_Element ,_STRING_Last_RX_Element ,_STRING_Resolution
};

static _STRING_ENUM calculator2[] =
{
		_STRING_Wave_Type ,_STRING_Start_Angle ,_STRING_Stop_Angle ,_STRING_Angle_Resolution ,_STRING_Law_Configuration ,_STRING_HTML_SPACE
};

static _STRING_ENUM part[] =
{
		_STRING_Material ,_STRING_Geometry ,_STRING_Thickness_ ,_STRING_HTML_SPACE
};

static _STRING_ENUM scanArea1[] =
{
		_STRING_Scan_Start ,_STRING_Scan_Length ,_STRING_Scan_Resolution ,_STRING_Index_Start ,_STRING_Index_Length ,_STRING_Index_Resolution
};

static _STRING_ENUM scanArea2[] =
{
		_STRING_Synchro ,_STRING_Scan_Speed_ ,_STRING_HTML_SPACE
};

static _STRING_ENUM scanArea3[] =
{
		_STRING_Axis ,_STRING_Encoder ,_STRING_Encoder_Type ,_STRING_Encoder_Resolution ,_STRING_Polarity ,_STRING_HTML_SPACE
};

static _STRING_ENUM signature[] =
{
		_STRING_Technician_Name ,_STRING_Technician_Signature ,_STRING_Contractor ,_STRING_Date
};

static _STRING_ENUM title[] =
{
		_STRING_PhaScan_Report ,_STRING_Probe_Characterization ,_STRING_Setup_ ,_STRING_Calculator ,
		_STRING_Part ,_STRING_Scan_Area ,_STRING_Note ,_STRING_Table ,_STRING_Current_View
};

static _STRING_ENUM focalType1[] =
{
		_STRING_Focal_Type ,_STRING_Position_Start	,_STRING_Position_End	,
		_STRING_Position_Step 	,_STRING_HTML_SPACE
};

static _STRING_ENUM focalType2[] =
{
		_STRING_Focal_Type ,_STRING_Offset_Start		,_STRING_Position_End	,
		_STRING_Position_Step	,_STRING_Depth_End	,_STRING_HTML_SPACE
};

static _STRING_ENUM focalType3[] =
{
		_STRING_Focal_Type ,_STRING_Offset_Start		,_STRING_Offset_End	,
		_STRING_Depth_Start  	,_STRING_Depth_End	,_STRING_HTML_SPACE
};

static const stringDictStruct reportStringDict[] =
{
/*REPORTSTRINGDICT_HEAD1*/				{head1 					,NUM_OF_STRING_INDEXS(head1)},
/*REPORTSTRINGDICT_HEAD2*/				{head2 					,NUM_OF_STRING_INDEXS(head2)},
/*REPORTSTRINGDICT_PROBE1*/				{probe1 				,NUM_OF_STRING_INDEXS(probe1)},
/*REPORTSTRINGDICT_PROBE2*/				{probe2 				,NUM_OF_STRING_INDEXS(probe2)},
/*REPORTSTRINGDICT_PROBE3*/				{probe3 				,NUM_OF_STRING_INDEXS(probe3)},
/*REPORTSTRINGDICT_PROBE4*/				{probe4 				,NUM_OF_STRING_INDEXS(probe4)},
/*REPORTSTRINGDICT_SETUP1*/				{setup1 				,NUM_OF_STRING_INDEXS(setup1)},
/*REPORTSTRINGDICT_SETUP2*/				{setup2 				,NUM_OF_STRING_INDEXS(setup2)},
/*REPORTSTRINGDICT_SETUP3*/				{setup3 				,NUM_OF_STRING_INDEXS(setup3)},
/*REPORTSTRINGDICT_SETUP4*/				{setup4 				,NUM_OF_STRING_INDEXS(setup4)},
/*REPORTSTRINGDICT_SETUP5*/				{setup5 				,NUM_OF_STRING_INDEXS(setup5)},
/*REPORTSTRINGDICT_CALCULATOR1*/		{calculator1 			,NUM_OF_STRING_INDEXS(calculator1)},
/*REPORTSTRINGDICT_CALCULATOR2*/		{calculator2 			,NUM_OF_STRING_INDEXS(calculator2)},
/*REPORTSTRINGDICT_PART*/				{part 					,NUM_OF_STRING_INDEXS(part)},
/*REPORTSTRINGDICT_SCANAREA1*/			{scanArea1 				,NUM_OF_STRING_INDEXS(scanArea1)},
/*REPORTSTRINGDICT_SCANAREA2*/			{scanArea2 				,NUM_OF_STRING_INDEXS(scanArea2)},
/*REPORTSTRINGDICT_SCANAREA3*/			{scanArea3 				,NUM_OF_STRING_INDEXS(scanArea3)},
/*REPORTSTRINGDICT_SIGNATURE*/			{signature 				,NUM_OF_STRING_INDEXS(signature)},
/*REPORTSTRINGDICT_TITLE*/				{title 					,NUM_OF_STRING_INDEXS(title)},
/*REPORTSTRINGDICT_FOCALTYPE1*/			{focalType1 			,NUM_OF_STRING_INDEXS(focalType1)},
/*REPORTSTRINGDICT_FOCALTYPE2*/			{focalType2 			,NUM_OF_STRING_INDEXS(focalType2)},
/*REPORTSTRINGDICT_FOCALTYPE3*/			{focalType3 			,NUM_OF_STRING_INDEXS(focalType3)},

/*REPORTSTRINGDICT_MAX*/
};


#endif /* REPORTSTRINGDICT_H_ */

/*
 * menu4DataMap.c
 *
 *  Created on: 2012-5-24
 *      Author: lzk
 */

#include "menuDataMap.h"
#include "popDataMap.h"
#include "spinDataMap.h"

#include "../uihandler/handler.h"
//WIZARD_GROUP_STEP_START,
static const menuDataDictStruct menu0000_dict = {menuDataAddress(MENUDATA_GROUP_STEP) ,wizardGroupHandler_back};
static const menuDataDictStruct menu0010_dict = {menuDataAddress(MENUDATA_GROUP_STEP) ,wizardGroupHandler_next};
static const menuDataDictStruct* menu000_dict[] =
{		&menu0000_dict ,&menu0010_dict ,NULL ,NULL ,NULL ,NULL		};

//WIZARD_GROUP_STEP_GROUP_SELECT,
#define menu0001_dict menu0000_dict
#define menu0011_dict menu0010_dict
//static const menuDataDictStruct menu0011_dict = {menuDataAddress(MENUDATA_GROUP_STEP) ,wizardGroupHandler_group_select};
static const menuDataDictStruct menu0021_dict = {popDataAddress(POPDATA_GROUP_SELECT) ,NULL};
static const menuDataDictStruct menu0031_dict = {menuDataAddress(MENUDATA_GROUP_STEP) ,wizardGroupHandler_DeleteGroup};
static const menuDataDictStruct* menu001_dict[] =
{		&menu0001_dict ,&menu0011_dict ,&menu0021_dict, &menu0031_dict, NULL ,NULL};

//WIZARD_GROUP_STEP_ADD,
#define menu0002_dict menu0000_dict
#define menu0012_dict menu0010_dict
//static const menuDataDictStruct menu0012_dict = {menuDataAddress(MENUDATA_GROUP_STEP) ,wizardGroupHandler_addGroup};
static const menuDataDictStruct menu0022_dict = {popDataAddress(POPDATA_SOURCE) ,NULL};
static const menuDataDictStruct* menu002_dict[] =
{		&menu0002_dict ,&menu0012_dict ,&menu0022_dict ,NULL ,NULL ,NULL};

//WIZARD_GROUP_STEP_MATERIAL,
#define menu0003_dict menu0000_dict
#define menu0013_dict menu0011_dict
static const menuDataDictStruct menu0023_dict = {popDataAddress(POPDATA_GEOMETRY) ,wizardHandlerGroup_geometry};
static const menuDataDictStruct menu0033_dict = {spinDataAddress(SPINDATA_THICKNESS) ,wizardHandlerGroup_thickness};
static const menuDataDictStruct menu0043_dict = {spinDataAddress(SPINDATA_DIAMETER) ,wizardHandlerGroup_diameter};
static const menuDataDictStruct menu0053_dict = {popDataAddress(POPDATA_MATERIAL) ,wizardHandlerGroup_material};
static const menuDataDictStruct* menu003_dict[] =
{		&menu0003_dict ,&menu0013_dict ,&menu0023_dict	,&menu0033_dict ,&menu0043_dict ,&menu0053_dict	};

//WIZARD_GROUP_STEP_GROUP_MODE,
#define menu0004_dict menu0000_dict
#define menu0014_dict menu0010_dict
static const menuDataDictStruct menu0024_dict = {popDataAddress(POPDATA_GROUP_MODE) ,wizardHandlerGroup_group_mode};
static const menuDataDictStruct* menu004_dict[] ={&menu0004_dict ,&menu0014_dict ,&menu0024_dict ,NULL ,NULL ,NULL};

//WIZARD_GROUP_STEP_TX_RX_MODE,
#define menu0005_1_dict menu0000_dict
#define menu0015_1_dict menu0010_dict
static const menuDataDictStruct menu0025_1_dict = {popDataAddress(POPDATA_TR_MODE) ,wizardHandlerGroup_tx_rx_mode};
static const menuDataDictStruct menu0035_1_dict = {spinDataAddress(SPINDATA_PULSE) ,wizardHandlerGroup_pulse};
static const menuDataDictStruct menu0045_1_dict = {spinDataAddress(SPINDATA_RECEIVE) ,wizardHandlerGroup_receive};
static const menuDataDictStruct* menu005_1_dict[] ={&menu0005_1_dict ,&menu0015_1_dict ,&menu0025_1_dict ,&menu0035_1_dict ,&menu0045_1_dict ,NULL};

//WIZARD_GROUP_STEP_TX_RX_MODE_PA
#define menu0005_2_dict menu0000_dict
#define menu0015_2_dict menu0010_dict
#define menu0025_2_dict menu0025_1_dict
static const menuDataDictStruct menu0035_2_dict = {spinDataAddress(SPINDATA_CONNECTION_P) ,wizardHandlerGroup_pulse};
static const menuDataDictStruct menu0045_2_dict = {spinDataAddress(SPINDATA_CONNECTION_R) ,wizardHandlerGroup_receive};
static const menuDataDictStruct* menu005_2_dict[] ={&menu0005_2_dict ,&menu0015_2_dict ,&menu0025_2_dict ,&menu0035_2_dict ,&menu0045_2_dict ,NULL};

//WIZARD_GROUP_STEP_PROBE_PE,
#define menu0006_1_dict menu0000_dict
#define menu0016_1_dict menu0010_dict
static const menuDataDictStruct menu0026_1_dict = {popDataAddress(POPDATA_PE_SELECT_PROBE) ,wizardHandlerGroup_select_probe};
static const menuDataDictStruct menu0036_1_dict = {NULL ,wizardHandlerGroup_define_probe};
static const menuDataDictStruct menu0046_1_dict = {menuDataAddress(MENUDATA_AUTO_DETECT) ,wizardHandlerGroup_probe_auto_detect};
static const menuDataDictStruct* menu006_1_dict[] ={&menu0006_1_dict ,&menu0016_1_dict ,&menu0026_1_dict ,&menu0036_1_dict ,&menu0046_1_dict ,NULL};

//WIZARD_GROUP_STEP_PROBE,//WIZARD_GROUP_STEP_PROBE_DEFINE,
#define menu0006_2_dict menu0000_dict
#define menu0016_2_dict menu0010_dict
static const menuDataDictStruct menu0026_2_dict = {popDataAddress(POPDATA_SELECT_PROBE) ,wizardHandlerGroup_select_probe};
#define menu0036_2_dict menu0036_1_dict
#define menu0046_2_dict menu0046_1_dict
static const menuDataDictStruct* menu006_2_dict[] ={&menu0006_2_dict ,&menu0016_2_dict ,&menu0026_2_dict ,&menu0036_2_dict ,&menu0046_2_dict ,NULL};

//WIZARD_GROUP_STEP_WEDGE_PE,
#define menu0007_1_dict menu0000_dict
#define menu0017_1_dict menu0010_dict
static const menuDataDictStruct menu0027_1_dict = {popDataAddress(POPDATA_PE_SELECT_WEDGE) ,wizardHandlerGroup_select_wedge};
static const menuDataDictStruct menu0037_1_dict = {NULL ,wizardHandlerGroup_define_wedge};
static const menuDataDictStruct* menu007_1_dict[] ={&menu0007_1_dict ,&menu0017_1_dict ,&menu0027_1_dict ,&menu0037_1_dict ,NULL ,NULL};


//WIZARD_GROUP_STEP_WEDGE,//WIZARD_GROUP_STEP_WEDGE_DEFINE,
#define menu0007_2_dict menu0000_dict
#define menu0017_2_dict menu0010_dict
static const menuDataDictStruct menu0027_2_dict = {popDataAddress(POPDATA_SELECT_WEDGE) ,wizardHandlerGroup_select_wedge};
#define menu0037_2_dict menu0037_1_dict
static const menuDataDictStruct* menu007_2_dict[] ={&menu0007_2_dict ,&menu0017_2_dict ,&menu0027_2_dict ,&menu0037_2_dict ,NULL ,NULL};


//WIZARD_GROUP_STEP_POSITION,
#define menu0008_dict menu0000_dict
#define menu0018_dict menu0010_dict
static const menuDataDictStruct menu0028_dict = {spinDataAddress(SPINDATA_SCAN_OFFSET) ,wizardHandlerGroup_scan_off};
static const menuDataDictStruct menu0038_dict = {spinDataAddress(SPINDATA_INDEX_OFFSET) ,wizardHandlerGroup_index_offset};
static const menuDataDictStruct menu0048_dict = {spinDataAddress(SPINDATA_SKEW) ,wizardHandlerGroup_skew};
static const menuDataDictStruct* menu008_dict[] ={&menu0008_dict ,&menu0018_dict ,&menu0028_dict ,&menu0038_dict ,&menu0048_dict ,NULL};


//WIZARD_GROUP_STEP_FINISH,
#define menu0009_dict menu0000_dict
#define menu0019_dict menu0010_dict
//static const menuDataDictStruct menu0019_dict = {menuDataAddress(MENUDATA_GROUP_STEP) ,wizardGroupHandler_finish};
static const menuDataDictStruct menu0029_dict = {NULL ,wizardHandlerGroup_continue};
static const menuDataDictStruct* menu009_dict[] ={&menu0009_dict ,&menu0019_dict ,&menu0029_dict ,NULL ,NULL ,NULL};

static const menuDataDictStruct** menu00_dict[] =
{
		menu000_dict,//WIZARD_GROUP_STEP_START
		menu001_dict,//WIZARD_GROUP_STEP_GROUP_SELECT
		menu002_dict,//WIZARD_GROUP_STEP_ADD
		menu003_dict,//WIZARD_GROUP_STEP_MATERIAL
		menu004_dict,//WIZARD_GROUP_STEP_GROUP_MODE
		menu005_1_dict,//WIZARD_GROUP_STEP_TX_RX_MODE
		menu006_1_dict,//WIZARD_GROUP_STEP_PROBE_PE
		menu007_1_dict,//WIZARD_GROUP_STEP_WEDGE_PE
		menu008_dict,//WIZARD_GROUP_STEP_POSITION
		menu009_dict,//WIZARD_GROUP_STEP_FINISH

		menu005_2_dict,//WIZARD_GROUP_STEP_TX_RX_MODE_PA
		menu006_2_dict,//WIZARD_GROUP_STEP_PROBE
		menu007_2_dict,//WIZARD_GROUP_STEP_WEDGE
		NULL,//WIZARD_GROUP_STEP_PROBE_DEFINE //只有label
		NULL,//WIZARD_GROUP_STEP_WEDGE_DEFINE //只有label
};


//WIZARD_FOCLAW_STEP_START,
static const menuDataDictStruct menu0100_dict = {menuDataAddress(MENUDATA_FOCALLAW_STEP) ,wizardFocallawHandler_back};
static const menuDataDictStruct menu0110_dict = {menuDataAddress(MENUDATA_FOCALLAW_STEP) ,wizardFocallawHandler_next};
static const menuDataDictStruct* menu010_dict[] ={&menu0100_dict ,&menu0110_dict ,NULL ,NULL ,NULL ,NULL};

//WIZARD_FOCLAW_STEP_GROUP_SELECT,
#define menu0101_dict menu0100_dict
#define menu0111_dict menu0110_dict
//static const menuDataDictStruct menu0111_dict = {menuDataAddress(MENUDATA_FOCALLAW_STEP) ,wizardFocallawHandler_group_select};
static const menuDataDictStruct menu0121_dict = {popDataAddress(POPDATA_FOCAL_GROUP_SELECT) ,NULL};
static const menuDataDictStruct* menu011_dict[] ={&menu0101_dict ,&menu0111_dict ,&menu0121_dict ,NULL ,NULL ,NULL};

//WIZARD_FOCLAW_STEP_LAW_CONFIGURATION,
static const menuDataDictStruct menu0124_dict = {popDataAddress(POPDATA_WAVE_TYPE) ,wizardFocallawHandler_wave_type};
#define menu0102_dict menu0100_dict
#define menu0112_dict menu0110_dict
#define menu0142_dict menu0124_dict
static const menuDataDictStruct menu0122_dict = {popDataAddress(POPDATA_LAW_TYPE) ,wizardFocallawHandler_law_type};
static const menuDataDictStruct menu0132_dict = {popDataAddress(POPDATA_FOCAL_TYPE) ,wizardFocallawHandler_focal_type};
static const menuDataDictStruct* menu012_dict[] ={&menu0102_dict ,&menu0112_dict ,&menu0122_dict ,&menu0132_dict , &menu0142_dict, NULL};

//WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE,//WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE1,
#define menu0103_0_dict menu0100_dict
#define menu0113_0_dict menu0110_dict
static const menuDataDictStruct menu0123_0_dict = {spinDataAddress(SPINDATA_POSITION_START) ,wizardFocallawHandler_position_start};
static const menuDataDictStruct menu0133_0_dict = {spinDataAddress(SPINDATA_POSITION_END) ,wizardFocallawHandler_position_end};
static const menuDataDictStruct menu0143_0_dict = {spinDataAddress(SPINDATA_POSITION_STEP) ,wizardFocallawHandler_position_step};
static const menuDataDictStruct* menu013_0_dict[] ={&menu0103_0_dict ,&menu0113_0_dict ,&menu0123_0_dict ,&menu0133_0_dict ,&menu0143_0_dict ,NULL};

//WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE2,
#define menu0103_1_dict menu0100_dict
#define menu0113_1_dict menu0110_dict
static const menuDataDictStruct menu0123_1_dict = {spinDataAddress(SPINDATA_OFFSET_START) ,wizardFocallawHandler_offset_start};
#define menu0133_1_dict menu0133_0_dict
//static const menuDataDictStruct menu0133_1_dict = {spinDataAddress(SPINDATA_POSITION_END) ,wizardFocallawHandler_position_end};
#define menu0143_1_dict menu0143_0_dict
//static const menuDataDictStruct menu0143_1_dict = {spinDataAddress(SPINDATA_POSITION_STEP) ,wizardFocallawHandler_position_step};
static const menuDataDictStruct menu0153_1_dict = {spinDataAddress(SPINDATA_DEPTH_END) ,wizardFocallawHandler_depth_end};
static const menuDataDictStruct* menu013_1_dict[] ={&menu0103_1_dict ,&menu0113_1_dict ,&menu0123_1_dict ,&menu0133_1_dict ,&menu0143_1_dict ,&menu0153_1_dict};

//WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE3,
#define menu0103_2_dict menu0100_dict
#define menu0113_2_dict menu0110_dict
#define menu0123_2_dict menu0123_1_dict
//static const menuDataDictStruct menu0123_2_dict = {spinDataAddress(SPINDATA_OFFSET_START) ,wizardFocallawHandler_offset_start};
static const menuDataDictStruct menu0133_2_dict = {spinDataAddress(SPINDATA_OFFSET_END) ,wizardFocallawHandler_offset_end};
static const menuDataDictStruct menu0143_2_dict = {spinDataAddress(SPINDATA_DEPTH_START) ,wizardFocallawHandler_depth_start};
#define menu0153_2_dict menu0153_1_dict
//static const menuDataDictStruct menu0153_2_dict = {spinDataAddress(SPINDATA_DEPTH_END) ,wizardFocallawHandler_depth_end};
static const menuDataDictStruct* menu013_2_dict[] ={&menu0103_2_dict ,&menu0113_2_dict ,&menu0123_2_dict ,&menu0133_2_dict ,&menu0143_2_dict ,&menu0153_2_dict};

//WIZARD_FOCLAW_STEP_WAVE_TYPE,
#define menu0104_dict menu0100_dict
#define menu0114_dict menu0110_dict
//static const menuDataDictStruct menu0134_dict = {popDataAddress(POPDATA_MATERIAL) ,NULL};
#define menu0134_dict menu0053_dict
static const menuDataDictStruct* menu014_dict[] ={&menu0104_dict ,&menu0114_dict ,&menu0124_dict ,&menu0134_dict ,NULL ,NULL};

//WIZARD_FOCLAW_STEP_PROBE_ELEMENTS,
#define menu0105_dict menu0100_dict
#define menu0115_dict menu0110_dict
static const menuDataDictStruct menu0125_dict = {spinDataAddress(SPINDATA_ELEMENT_QTY) ,wizardFocallawHandler_element_qty};
static const menuDataDictStruct menu0135_dict = {spinDataAddress(SPINDATA_FIRST_ELEMENT) ,wizardFocallawHandler_first_element};
static const menuDataDictStruct menu0145_dict = {spinDataAddress(SPINDATA_LAST_ELEMENT) ,wizardFocallawHandler_last_element};
static const menuDataDictStruct menu0155_dict = {spinDataAddress(SPINDATA_ELEMENT_STEP) ,wizardFocallawHandler_element_step};
static const menuDataDictStruct* menu015_dict[] ={&menu0105_dict ,&menu0115_dict ,&menu0125_dict ,&menu0135_dict ,&menu0145_dict ,&menu0155_dict};

//WIZARD_FOCLAW_STEP_BEAM_ANGLE,
#define menu0106_dict menu0100_dict
#define menu0116_dict menu0110_dict
static const menuDataDictStruct menu0126_dict = {spinDataAddress(SPINDATA_MIN_ANGLE) ,wizardFocallawHandler_min_angle};
static const menuDataDictStruct menu0136_dict = {spinDataAddress(SPINDATA_MAX_ANGLE) ,wizardFocallawHandler_max_angle};
static const menuDataDictStruct menu0146_dict = {spinDataAddress(SPINDATA_ANGLE_STEP) ,wizardFocallawHandler_angle_step};
static const menuDataDictStruct menu0156_dict = {spinDataAddress(SPINDATA_FOCUS_DEPTH) ,wizardFocallawHandler_focus_depth};
static const menuDataDictStruct* menu016_dict[] ={&menu0106_dict ,&menu0116_dict ,&menu0126_dict ,&menu0136_dict ,&menu0146_dict ,&menu0156_dict};

//WIZARD_FOCLAW_STEP_GENERATE,
#define menu0107_dict menu0100_dict
#define menu0117_dict menu0110_dict
//static const menuDataDictStruct menu0117_dict = {menuDataAddress(MENUDATA_FOCALLAW_STEP) ,wizardFocallawHandler_generate};
static const menuDataDictStruct* menu017_dict[] ={&menu0107_dict ,&menu0117_dict ,NULL ,NULL ,NULL ,NULL};

static const menuDataDictStruct** menu01_dict[] =
{
		menu010_dict,
		menu011_dict,
		menu012_dict,
		menu013_0_dict,
		menu014_dict,
		menu015_dict,
		menu016_dict,
		menu017_dict,
		menu013_0_dict,
		menu013_1_dict,
		menu013_2_dict,
		menu013_0_dict,
};

static const menuDataDictStruct** menu02_dict[] =
{

};

static const menuDataDictStruct*** menu0_dict[] =
{
		menu00_dict,
		menu01_dict,
		menu02_dict,
};

static const menuDataDictStruct*** menu1_dict[] =
{

};


static const menuDataDictStruct menu2100_dict = {popDataAddress(POPDATA_ALARM) ,outputHandler_alarmSelect};
static const menuDataDictStruct menu2110_dict = {popDataAddress(POPDATA_ALARM_GROUP_SELECT1) ,outputHandler_alarmGroupSelect};
static const menuDataDictStruct menu2120_dict = {popDataAddress(POPDATA_CONDITION11) ,outputHandler_alarmCondition1};
static const menuDataDictStruct menu2130_dict = {popDataAddress(POPDATA_OPERATOR1) ,outputHandler_alarmOperator};
static const menuDataDictStruct menu2140_dict = {popDataAddress(POPDATA_CONDITION21) ,outputHandler_alarmCondition2};
static const menuDataDictStruct* menu210_dict[] ={&menu2100_dict ,&menu2110_dict ,&menu2120_dict ,&menu2130_dict ,&menu2140_dict ,NULL};

#define menu2101_dict menu2100_dict
static const menuDataDictStruct menu2111_dict = {popDataAddress(POPDATA_ALARM_GROUP_SELECT2) ,outputHandler_alarmGroupSelect};
static const menuDataDictStruct menu2121_dict = {popDataAddress(POPDATA_CONDITION12) ,outputHandler_alarmCondition1};
static const menuDataDictStruct menu2131_dict = {popDataAddress(POPDATA_OPERATOR2) ,outputHandler_alarmOperator};
static const menuDataDictStruct menu2141_dict = {popDataAddress(POPDATA_CONDITION22) ,outputHandler_alarmCondition2};
static const menuDataDictStruct* menu211_dict[] ={&menu2101_dict ,&menu2111_dict ,&menu2121_dict ,&menu2131_dict ,&menu2141_dict ,NULL};

#define menu2102_dict menu2100_dict
static const menuDataDictStruct menu2112_dict = {popDataAddress(POPDATA_ALARM_GROUP_SELECT3) ,outputHandler_alarmGroupSelect};
static const menuDataDictStruct menu2122_dict = {popDataAddress(POPDATA_CONDITION13) ,outputHandler_alarmCondition1};
static const menuDataDictStruct menu2132_dict = {popDataAddress(POPDATA_OPERATOR3) ,outputHandler_alarmOperator};
static const menuDataDictStruct menu2142_dict = {popDataAddress(POPDATA_CONDITION23) ,outputHandler_alarmCondition2};
static const menuDataDictStruct* menu212_dict[] ={&menu2102_dict ,&menu2112_dict ,&menu2122_dict ,&menu2132_dict ,&menu2142_dict ,NULL};

static const menuDataDictStruct** menu21_dict[] ={menu210_dict ,menu211_dict ,menu212_dict };

static const menuDataDictStruct menu2200_dict = {popDataAddress(POPDATA_OUTPUT) ,outputHandler_outputSelect};
static const menuDataDictStruct menu2210_dict = {menuDataAddress(MENUDATA_SWITCH1) ,outputHandler_switch};
static const menuDataDictStruct menu2220_dict = {spinDataAddress(SPINDATA_COUNT1) ,outputHandler_alarmCount};
static const menuDataDictStruct menu2230_dict = {popDataAddress(POPDATA_SOUND) ,outputHandler_alarmBeep};
static const menuDataDictStruct menu2240_dict = {spinDataAddress(SPINDATA_DELAY1) ,outputHandler_alarmDelay};
static const menuDataDictStruct menu2250_dict = {spinDataAddress(SPINDATA_HOLD_TIME1) ,outputHandler_alarmHoleTime};
static const menuDataDictStruct* menu220_dict[] ={&menu2200_dict ,&menu2210_dict ,&menu2220_dict ,&menu2230_dict ,&menu2240_dict ,&menu2250_dict};

#define menu2201_dict menu2200_dict
static const menuDataDictStruct menu2211_dict = {menuDataAddress(MENUDATA_SWITCH2) ,outputHandler_switch};
static const menuDataDictStruct menu2221_dict = {spinDataAddress(SPINDATA_COUNT2) ,outputHandler_alarmCount};
#define menu2231_dict menu2230_dict
static const menuDataDictStruct menu2241_dict = {spinDataAddress(SPINDATA_DELAY2) ,outputHandler_alarmDelay};
static const menuDataDictStruct menu2251_dict = {spinDataAddress(SPINDATA_HOLD_TIME2) ,outputHandler_alarmHoleTime};
static const menuDataDictStruct* menu221_dict[] ={&menu2201_dict ,&menu2211_dict ,&menu2221_dict ,&menu2231_dict ,&menu2241_dict ,&menu2251_dict};

#define menu2202_dict menu2200_dict
static const menuDataDictStruct menu2212_dict = {menuDataAddress(MENUDATA_SWITCH3) ,outputHandler_switch};
static const menuDataDictStruct menu2222_dict = {spinDataAddress(SPINDATA_COUNT3) ,outputHandler_alarmCount};
#define menu2232_dict menu2230_dict
static const menuDataDictStruct menu2242_dict = {spinDataAddress(SPINDATA_DELAY3) ,outputHandler_alarmDelay};
static const menuDataDictStruct menu2252_dict = {spinDataAddress(SPINDATA_HOLD_TIME3) ,outputHandler_alarmHoleTime};
static const menuDataDictStruct* menu222_dict[] ={&menu2202_dict ,&menu2212_dict ,&menu2222_dict ,&menu2232_dict ,&menu2242_dict ,&menu2252_dict};

#define menu2203_dict menu2200_dict
static const menuDataDictStruct menu2213_dict = {menuDataAddress(MENUDATA_SWITCH4) ,outputHandler_switch};
static const menuDataDictStruct menu2223_dict = {popDataAddress(POPDATA_ANALOG_GROUP1) ,outputHandler_analogGroup};
static const menuDataDictStruct menu2233_dict = {popDataAddress(POPDATA_ANALOG_DATA1) ,outputHandler_analogDataSource};
static const menuDataDictStruct* menu223_dict[] ={&menu2203_dict ,&menu2213_dict ,&menu2223_dict ,&menu2233_dict ,NULL ,NULL};

#define menu2204_dict menu2200_dict
static const menuDataDictStruct menu2214_dict = {menuDataAddress(MENUDATA_SWITCH5) ,outputHandler_switch};
static const menuDataDictStruct menu2224_dict = {popDataAddress(POPDATA_ANALOG_GROUP2) ,outputHandler_analogGroup};
static const menuDataDictStruct menu2234_dict = {popDataAddress(POPDATA_ANALOG_DATA2) ,outputHandler_analogDataSource};
static const menuDataDictStruct* menu224_dict[] ={&menu2204_dict ,&menu2214_dict ,&menu2224_dict ,&menu2234_dict ,NULL ,NULL};

static const menuDataDictStruct** menu22_dict[] =
{
		menu220_dict,
		menu221_dict,
		menu222_dict,
		menu223_dict,
		menu224_dict,
};

static const menuDataDictStruct*** menu2_dict[] =
{
		NULL,
		menu21_dict,
		menu22_dict,
};

extern int MenuHandler_TofdStraightening(void* p_para);
extern int MenuHandler_TofdRemoveLateralWave(void* p_para);
extern int MenuHandler_TofdCancel(void* p_para);
extern int MenuHandler_TofdRefLine(void* p_para);
extern int MenuHandler_TofdRefLineDepth(void* p_para);
extern int MenuHandler_TofdDepthCal(void* p_para);
extern int MenuHandler_TofdPcs(void* p_para);
static const menuDataDictStruct menu3150_dict = {spinDataAddress(SPINDATA_TOFD_REFLINE) ,MenuHandler_TofdRefLine};
static const menuDataDictStruct* menu310_dict[] ={NULL,NULL,NULL,NULL,NULL,&menu3150_dict};
static const menuDataDictStruct** menu31_dict[] = {menu310_dict};
static const menuDataDictStruct menu3410_dict = {menuDataAddress(MENUDATA_TOFDSTRAIGHTENING) ,MenuHandler_TofdStraightening};
static const menuDataDictStruct menu3420_dict = {menuDataAddress(MENUDATA_TOFDREMOVELATERALWAVE) ,MenuHandler_TofdRemoveLateralWave};
//static const menuDataDictStruct menu3430_dict = {NULL ,MenuHandler_TofdCancel};
static const menuDataDictStruct menu3430_dict = {spinDataAddress(SPINDATA_TOFD_REFLINE_DEPTH) ,MenuHandler_TofdRefLineDepth};
static const menuDataDictStruct menu3440_dict = {NULL, MenuHandler_TofdDepthCal};
static const menuDataDictStruct menu3450_dict = {spinDataAddress(SPINDATA_TOFD_PCS) ,MenuHandler_TofdPcs};
static const menuDataDictStruct* menu340_dict[] ={NULL ,&menu3410_dict ,&menu3420_dict ,&menu3430_dict ,&menu3440_dict ,&menu3450_dict};
static const menuDataDictStruct** menu34_dict[] = {menu340_dict};
static const menuDataDictStruct*** menu3_dict[] =
{
		NULL,
        menu31_dict,
		NULL,
		NULL,
		menu34_dict,
		NULL,
};

static const menuDataDictStruct*** menu4_dict[] =
{

};

static const menuDataDictStruct*** menu5_dict[] =
{

};

static const menuDataDictStruct*** menu6_dict[] =
{

};

static const menuDataDictStruct*** menu7_dict[] =
{

};

static const menuDataDictStruct*** menu8_dict[] =
{

};

static const menuDataDictStruct menu9400_dict = {NULL ,NULL};
//static const menuDataDictStruct menu9410_dict = {NULL ,NULL};
#define menu9410_dict menu9400_dict
static const menuDataDictStruct menu9420_dict = {NULL ,UpdateHandler_ResetCFG};
#define menu9430_dict menu9400_dict
static const menuDataDictStruct* menu940_dict[] ={&menu9400_dict ,&menu9410_dict ,&menu9420_dict ,&menu9430_dict ,NULL ,NULL};

static const menuDataDictStruct** menu94_dict[] = {menu940_dict};

static const menuDataDictStruct*** menu9_dict[] =
{
		NULL,
		NULL,
		NULL,
		NULL,
		menu94_dict,
};

const menuDataDictStruct**** menu4DataDict[] =
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


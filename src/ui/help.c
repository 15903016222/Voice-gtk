/*
 * help.c
 *
 *  Created on: 2012-5-31
 *      Author: lzk
 */
#include <assert.h>
#include "../uihandler/menu/menuDefine.h"
#include "../drawui.h"
#include "string.h"
const char* localHelpAddress = "http://127.0.0.1/home/tt/TT/source/system/Help/";
/*帮助文档路径*/
#define _HELP_WIZARD_PATH 			"Contextual/Help_Wizard_Menu.html"
#define _HELP_UT_SETTING_PATH	 	"Contextual/Help_UT_Settings_Menu.html"
#define _HELP_GATE_ALARM_PATH	 	"Contextual/Help_GateAlarm_Menu.html"
#define _HELP_MEASUREMENTS_PATH	 	"Contextual/Help_Measurements_Menu.html"
#define _HELP_DISPLAY_PATH		 	"Contextual/Help_Display_Menu.html"
#define _HELP_PROBE_PART_PATH	 	"Contextual/Help_ProbePart_Menu.html"
#define _HELP_FOCAL_LAW_PATH 	 	"Contextual/Help_Focal_Law_Menu.html"
#define _HELP_SCAN_PATH 		 	"Contextual/Help_Scan_Menu.html"
#define _HELP_FILE_PATH 			"Contextual/Help_File_Menu.html"
#define _HELP_PREFERENCES_PATH  	"Contextual/Help_Preferences_Menu.html"

#define HELP_WIZARD_LAW1 			"Wizard/LawWizard_1.html"
#define HELP_WIZARD_LAW2 			"Wizard/LawWizard_2.html"
#define HELP_WIZARD_LAW3 			"Wizard/LawWizard_3.html"
#define HELP_WIZARD_LAW4 			"Wizard/LawWizard_4.html"
#define HELP_WIZARD_LAW5 			"Wizard/LawWizard_5.html"
#define HELP_WIZARD_LAW6 			"Wizard/LawWizard_6.html"
#define HELP_WIZARD_LAW7 			"Wizard/LawWizard_7.html"

#define HELP_WIZARD_GROUP1 			"Wizard/PA_GroupWizard_1.html"
#define HELP_WIZARD_GROUP2 			"Wizard/PA_GroupWizard_2.html"
#define HELP_WIZARD_GROUP3 			"Wizard/PA_GroupWizard_3.html"
#define HELP_WIZARD_GROUP4 			"Wizard/PA_GroupWizard_4.html"
#define HELP_WIZARD_GROUP5 			"Wizard/PA_GroupWizard_5.html"
#define HELP_WIZARD_GROUP6 			"Wizard/PA_GroupWizard_6.html"
#define HELP_WIZARD_GROUP7 			"Wizard/PA_GroupWizard_7.html"
#define HELP_WIZARD_GROUP8 			"Wizard/PA_GroupWizard_8.html"
#define HELP_WIZARD_GROUP9 			"Wizard/PA_GroupWizard_9.html"
#define HELP_WIZARD_GROUP10 		"Wizard/PA_GroupWizard_10.html"
/*帮助文档 标签信息*/
#define HELP_WIZARD_GROUP 			"#Group"
#define HELP_WIZARD_FOCALLAW		"#Focal Law"
#define HELP_WIZARD_CALIBRATION 	"#Calibration"
#define HELP_WIZARD_WELD 			"#Weld"

#define HELP_UTSETTING_GENERAL 		"#General"
#define HELP_UTSETTING_PULSE 		"#Pulser"
#define HELP_UTSETTING_RECEIVE 		"#Receiver"
#define HELP_UTSETTING_BEAM 		"#Beam"
#define HELP_UTSETTING_ADVANCED 	"#Advanced"

#define HELP_GATE_GATE 				"#Gate"
#define HELP_GATE_ALARM 			"#Alarm"
#define HELP_GATE_OUTPUT 			"#Output"
#define HELP_GATE_CURVES 			"#Sizing Curves"

#define HELP_MEASUREMENTS_READING 	"#Reading"
#define HELP_MEASUREMENTS_CURSORS 	"#Cursors"
#define HELP_MEASUREMENTS_TABLE 	"#Table"
#define HELP_MEASUREMENTS_THICKNESS "#Thickness"
#define HELP_MEASUREMENTS_EXPORT 	"#Export"

#define HELP_DISPLAY_SELECTION 		"#Selection"
#define HELP_DISPLAY_OVERLAY 		"#Overlay"
#define HELP_DISPLAY_COLOR 			"#Color"
#define HELP_DISPLAY_PROPERTIES 	"#Properties"

#define HELP_PROBE_SELECT 			"#Select"
#define HELP_PROBE_POSITION 		"#Position"
#define HELP_PROBE_CHARACTERIZE 	"#Characterize"
#define HELP_PROBE_PARTS 			"#Parts"

#define HELP_FOCALLAW_CONFIGURATION "#Configuration"
#define HELP_FOCALLAW_BEAM 	 		"#Beam Angle"
#define HELP_FOCALLAW_FOCAL 		"#Focal Point"
#define HELP_FOCALLAW_APERTURE 		"#Aperture"
#define HELP_FOCALLAW_LAWS 			"#Laws"

#define HELP_SCAN_ENCODER 			"#Encoder"
#define HELP_SCAN_INSPECTION		"#Inspection"
#define HELP_SCAN_AREA 				"#Area"
#define HELP_SCAN_START 			"#Start"
#define HELP_SCAN_DATA 				"#Data"

#define HELP_FILE_FILE				"#File"
#define HELP_FILE_REPORT			"#Report"
#define HELP_FILE_FORMAT			"#Format"
#define HELP_FILE_USER				"#User Field"
#define HELP_FILE_NOTE				"#Notes"

#define HELP_PREFERENCES_PREF		"#Pref."
#define HELP_PREFERENCES_SYSTEM		"#System"
#define HELP_PREFERENCES_SERVICE	"#Service"
#define HELP_PREFERENCES_NETWORK	"#Network"
#define HELP_PREFERENCES_UPDATE		"#Update"

#define _LABEL_00   HELP_WIZARD_GROUP
#define _LABEL_01   HELP_WIZARD_FOCALLAW
#define _LABEL_02   HELP_WIZARD_CALIBRATION
#define _LABEL_03   HELP_WIZARD_WELD

#define _LABEL_10   HELP_UTSETTING_GENERAL
#define _LABEL_11   HELP_UTSETTING_PULSE
#define _LABEL_12   HELP_UTSETTING_RECEIVE
#define _LABEL_13   HELP_UTSETTING_BEAM
#define _LABEL_14   HELP_UTSETTING_ADVANCED

#define _LABEL_20   HELP_GATE_GATE
#define _LABEL_21   HELP_GATE_ALARM
#define _LABEL_22   HELP_GATE_OUTPUT
#define _LABEL_23   HELP_GATE_CURVES

#define _LABEL_30   HELP_MEASUREMENTS_READING
#define _LABEL_31   HELP_MEASUREMENTS_CURSORS
#define _LABEL_32   HELP_MEASUREMENTS_TABLE
#define _LABEL_33   HELP_MEASUREMENTS_THICKNESS
#define _LABEL_34   HELP_MEASUREMENTS_EXPORT

#define _LABEL_40   HELP_DISPLAY_SELECTION
#define _LABEL_41   HELP_DISPLAY_OVERLAY
#define _LABEL_42   HELP_DISPLAY_COLOR
#define _LABEL_43   HELP_DISPLAY_PROPERTIES

#define _LABEL_50   HELP_PROBE_SELECT
#define _LABEL_51   HELP_PROBE_POSITION
#define _LABEL_52   HELP_PROBE_CHARACTERIZE
#define _LABEL_53   HELP_PROBE_PARTS

#define _LABEL_60   HELP_FOCALLAW_CONFIGURATION
#define _LABEL_61   HELP_FOCALLAW_BEAM
#define _LABEL_62   HELP_FOCALLAW_FOCAL
#define _LABEL_63   HELP_FOCALLAW_APERTURE
#define _LABEL_64   HELP_FOCALLAW_LAWS

#define _LABEL_70   HELP_SCAN_ENCODER
#define _LABEL_71   HELP_SCAN_INSPECTION
#define _LABEL_72   HELP_SCAN_AREA
#define _LABEL_73   HELP_SCAN_START
#define _LABEL_74   HELP_SCAN_DATA

#define _LABEL_80   HELP_FILE_FILE
#define _LABEL_81   HELP_FILE_REPORT
#define _LABEL_82   HELP_FILE_FORMAT
#define _LABEL_83   HELP_FILE_USER
#define _LABEL_84   HELP_FILE_NOTE

#define _LABEL_90   HELP_PREFERENCES_PREF
#define _LABEL_91   HELP_PREFERENCES_SYSTEM
#define _LABEL_92   HELP_PREFERENCES_SERVICE
#define _LABEL_93   HELP_PREFERENCES_NETWORK
#define _LABEL_94   HELP_PREFERENCES_UPDATE

const char* menu1HelpAdress[] =
{
	_HELP_WIZARD_PATH 		,//	"Contextual/Help_Wizard_Menu.html"
	_HELP_UT_SETTING_PATH	,// "Contextual/Help_UT_Settings_Menu.html"
	_HELP_GATE_ALARM_PATH	,//	"Contextual/Help_GateAlarm_Menu.html"
	_HELP_MEASUREMENTS_PATH	,// "Contextual/Help_Measurements_Menu.html"
	_HELP_DISPLAY_PATH		,// "Contextual/Help_Display_Menu.html"
	_HELP_PROBE_PART_PATH	,//	"Contextual/Help_ProbePart_Menu.html"
	_HELP_FOCAL_LAW_PATH 	,//	"Contextual/Help_Focal_Law_Menu.html"
	_HELP_SCAN_PATH 		,//	"Contextual/Help_Scan_Menu.html"
	_HELP_FILE_PATH 		,//	"Contextual/Help_File_Menu.html"
	_HELP_PREFERENCES_PATH  ,//	"Contextual/Help_Preferences_Menu.html"
};

const char* menu2HelpLabel[][5] =
{
		{_LABEL_00 ,_LABEL_01 ,_LABEL_02 ,_LABEL_03 ,NULL},
		{_LABEL_10 ,_LABEL_11 ,_LABEL_12 ,_LABEL_13 ,_LABEL_14},
		{_LABEL_20 ,_LABEL_21 ,_LABEL_22 ,_LABEL_23 ,NULL},
		{_LABEL_30 ,_LABEL_31 ,_LABEL_32 ,_LABEL_33 ,_LABEL_34},
		{_LABEL_40 ,_LABEL_41 ,_LABEL_42 ,_LABEL_43 ,NULL},
		{_LABEL_50 ,_LABEL_51 ,_LABEL_52 ,_LABEL_53 ,NULL},
		{_LABEL_60 ,_LABEL_61 ,_LABEL_62 ,_LABEL_63 ,_LABEL_64},
		{_LABEL_70 ,_LABEL_71 ,_LABEL_72 ,_LABEL_73 ,_LABEL_74},
		{_LABEL_80 ,_LABEL_81 ,_LABEL_82 ,_LABEL_83 ,_LABEL_84},
		{_LABEL_90 ,_LABEL_91 ,_LABEL_92 ,_LABEL_93 ,_LABEL_94},
};

static void getHtmlAdress(MENU_UNION menu_union ,char* htmlAddress)
{
	strcat(htmlAddress ,menu1HelpAdress[menu_union.pos.menu1]);
	assert(menu_union.pos.menu2 < 5);
	if(menu2HelpLabel[menu_union.pos.menu1][menu_union.pos.menu2])
	{
		strcat(htmlAddress ,menu2HelpLabel[menu_union.pos.menu1][menu_union.pos.menu2]);
	}
}

void showHelp(MENU_UNION menu_union ,int enable)
{
	if(enable)
	{
		char htmlAddress[256];
		memset(htmlAddress ,0 ,256);

		strcpy(htmlAddress ,localHelpAddress);
		if(ENGLISH_ == get_language(pp->p_config))
		{
			strcat(htmlAddress ,"English/");
		}
		else
		{
			strcat(htmlAddress ,"Chinese/");
		}
		switch(menu_union.pos.menu1)
		{
		case 0:
			if(0 == menu_union.pos.menu4)
			{
				getHtmlAdress(menu_union ,htmlAddress);
				break;
			}
			switch(menu_union.pos.menu2)
			{
			case 0:
				switch(menu_union.pos.menu4)
				{
			//	case WIZARD_GROUP_STEP_START:
				case WIZARD_GROUP_STEP_GROUP_SELECT:
					strcat(htmlAddress ,HELP_WIZARD_GROUP1);
					break;
				case WIZARD_GROUP_STEP_ADD:
					strcat(htmlAddress ,HELP_WIZARD_GROUP10);
					break;
				case WIZARD_GROUP_STEP_MATERIAL:
					strcat(htmlAddress ,HELP_WIZARD_GROUP2);
					break;
				case WIZARD_GROUP_STEP_GROUP_MODE:
					strcat(htmlAddress ,HELP_WIZARD_GROUP3);
					break;
				case WIZARD_GROUP_STEP_TX_RX_MODE:
					strcat(htmlAddress ,HELP_WIZARD_GROUP4);
					break;
				case WIZARD_GROUP_STEP_PROBE_PE:
					strcat(htmlAddress ,HELP_WIZARD_GROUP5);
					break;
				case WIZARD_GROUP_STEP_WEDGE_PE:
					strcat(htmlAddress ,HELP_WIZARD_GROUP6);
					break;
				case WIZARD_GROUP_STEP_POSITION:
					strcat(htmlAddress ,HELP_WIZARD_GROUP7);
					break;
				case WIZARD_GROUP_STEP_FINISH:
					strcat(htmlAddress ,HELP_WIZARD_GROUP8);
					break;
				case WIZARD_GROUP_STEP_TX_RX_MODE_PA:
					strcat(htmlAddress ,HELP_WIZARD_GROUP4);
					break;
				case WIZARD_GROUP_STEP_PROBE:
					strcat(htmlAddress ,HELP_WIZARD_GROUP5);
					break;
				case WIZARD_GROUP_STEP_WEDGE:
					strcat(htmlAddress ,HELP_WIZARD_GROUP6);
					break;
				case WIZARD_GROUP_STEP_PROBE_DEFINE:
					strcat(htmlAddress ,HELP_WIZARD_GROUP5);
					break;
				case WIZARD_GROUP_STEP_WEDGE_DEFINE:
					strcat(htmlAddress ,HELP_WIZARD_GROUP6);
					break;
				default:
					return;
				}
				break;
			case 1:
				switch(menu_union.pos.menu4)
				{
			//	case WIZARD_FOCLAW_STEP_START:
				case WIZARD_FOCLAW_STEP_GROUP_SELECT:
					strcat(htmlAddress ,HELP_WIZARD_LAW1);
					break;
				case WIZARD_FOCLAW_STEP_LAW_CONFIGURATION:
					strcat(htmlAddress ,HELP_WIZARD_LAW2);
					break;
				case WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE:
					strcat(htmlAddress ,HELP_WIZARD_LAW3);
					break;
				case WIZARD_FOCLAW_STEP_WAVE_TYPE:
					strcat(htmlAddress ,HELP_WIZARD_LAW4);
					break;
				case WIZARD_FOCLAW_STEP_PROBE_ELEMENTS:
					strcat(htmlAddress ,HELP_WIZARD_LAW5);
					break;
				case WIZARD_FOCLAW_STEP_BEAM_ANGLE:
					strcat(htmlAddress ,HELP_WIZARD_LAW6);
					break;
				case WIZARD_FOCLAW_STEP_GENERATE:
					strcat(htmlAddress ,HELP_WIZARD_LAW7);
					break;
				case WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE1:
					strcat(htmlAddress ,HELP_WIZARD_LAW3);
					break;
				case WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE2:
					strcat(htmlAddress ,HELP_WIZARD_LAW3);
					break;
				case WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE3:
					strcat(htmlAddress ,HELP_WIZARD_LAW3);
					break;
				default:
					return;
				}
				break;
			case 2:
			case 3:
			default:
				return;
			}
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			getHtmlAdress(menu_union ,htmlAddress);
			break;
		default:
			return;
		}
		gtk_widget_hide(pp->hbox211);
		gtk_widget_show(pp->sw);
		webkit_web_view_load_uri (pp->web_view ,htmlAddress);
	}
	else
	{
		gtk_widget_hide(pp->sw);
		gtk_widget_show(pp->hbox211);
	}
}

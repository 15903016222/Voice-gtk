/*
 * menuDataMap.h
 *
 *  Created on: 2012-5-24
 *      Author: gener
 */

#ifndef MENUDATAMAP_H_
#define MENUDATAMAP_H_

#include "../uihandler/menu/menuDefine.h"
#include "../string/_string.h"

	int straighteningEnable;
	int removeLateralWaveEnable;
enum
{
	MENUDATA_GROUP_STEP,//		{0 ,0 },
	MENUDATA_FOCALLAW_STEP,//	{0 ,0 },
	MENUDATA_SWITCH1,//		{0 ,0 },
	MENUDATA_SWITCH2,//		{0 ,0 },
	MENUDATA_SWITCH3,//		{0 ,0 },
	MENUDATA_SWITCH4,//		{0 ,0 },
	MENUDATA_SWITCH5,//		{0 ,0 },
	MENUDATA_AUTO_DETECT,//	{0 ,0 },

	MENUDATA_TOFDSTRAIGHTENING,//
	MENUDATA_TOFDREMOVELATERALWAVE,//
	MENUDATA_MAX,//
};
#define menuDataAddress(index) (menuDataDict + (index))

static menuDataStruct menuDataDict[] =
{
/*MENUDATA_GROUP_STEP*/		{MAINSTRINGDICT_NONE 	,0 },
/*MENUDATA_FOCALLAW_STEP*/	{MAINSTRINGDICT_NONE 	,0 },
/*MENUDATA_SWITCH1*/		{MAINSTRINGDICT_SWITCH 	,0 },
/*MENUDATA_SWITCH2*/		{MAINSTRINGDICT_SWITCH 	,0 },
/*MENUDATA_SWITCH3*/		{MAINSTRINGDICT_SWITCH 	,0 },
/*MENUDATA_SWITCH4*/		{MAINSTRINGDICT_SWITCH 	,0 },
/*MENUDATA_SWITCH5*/		{MAINSTRINGDICT_SWITCH 	,0 },
/*MENUDATA_AUTO_DETECT*/	{MAINSTRINGDICT_SWITCH 	,0 },

/*MENUDATA_TOFDSTRAIGHTENING,*/	{MAINSTRINGDICT_SWITCH 	,0 },
/*MENUDATA_TOFDREMOVELATERALWAVE,*/	{MAINSTRINGDICT_SWITCH 	,0 },
/*MENUDATA_MAX*/
};

#endif /* MENUDATAMAP_H_ */

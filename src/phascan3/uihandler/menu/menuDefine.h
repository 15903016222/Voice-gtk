/*
 * menuDefine.h
 *
 *  Created on: 2012-5-4
 *      Author: lzk
 */

#ifndef MENUDEFINE_H_
#define MENUDEFINE_H_


#include <gtk/gtk.h>
typedef struct{
	unsigned char menu1;//菜单2的位置
	unsigned char menu2;//菜单2的位置
	unsigned char menu3;//菜单3的位置
	unsigned char menu4;//附加参数，前3个参数确定不下来时用于索引
}
	MENU_POS_STRUCT;

typedef union{
	MENU_POS_STRUCT pos;
	unsigned int key;
}
	MENU_UNION;//索引号

enum{
	WIZARD_GROUP_STEP_START,
	WIZARD_GROUP_STEP_GROUP_SELECT,
	WIZARD_GROUP_STEP_ADD,
	WIZARD_GROUP_STEP_MATERIAL,
	WIZARD_GROUP_STEP_GROUP_MODE,
	WIZARD_GROUP_STEP_TX_RX_MODE,
	WIZARD_GROUP_STEP_PROBE_PE,
	WIZARD_GROUP_STEP_WEDGE_PE,
	WIZARD_GROUP_STEP_POSITION,
	WIZARD_GROUP_STEP_FINISH,
	WIZARD_GROUP_STEP_MAX,
	WIZARD_GROUP_STEP_TX_RX_MODE_PA = WIZARD_GROUP_STEP_MAX,
	WIZARD_GROUP_STEP_PROBE,
	WIZARD_GROUP_STEP_WEDGE,
	WIZARD_GROUP_STEP_PROBE_DEFINE,
	WIZARD_GROUP_STEP_WEDGE_DEFINE,
};

enum{
	WIZARD_FOCLAW_STEP_START,
	WIZARD_FOCLAW_STEP_GROUP_SELECT,
	WIZARD_FOCLAW_STEP_LAW_CONFIGURATION,
	WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE,
	WIZARD_FOCLAW_STEP_WAVE_TYPE,
	WIZARD_FOCLAW_STEP_PROBE_ELEMENTS,
	WIZARD_FOCLAW_STEP_BEAM_ANGLE,
	WIZARD_FOCLAW_STEP_GENERATE,
	WIZARD_FOCLAW_STEP_MAX,
	WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE1 = WIZARD_FOCLAW_STEP_MAX,
	WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE2,
	WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE3,
	WIZARD_FOCLAW_STEP_LAW_FOCAL_TYPE_HALF,
};


typedef struct{
	void* pParaObject;
	int popPos;
}
	popParaStruct;

typedef struct{
	const int stringIndex1;
	const int stringIndex2;
	int itemStatus;
	int lastPopPos;
}
	popDataStruct;

typedef struct{
	float current_value;
	float lower;
	float upper;
	float step;
	int digit;
	int unit;
}
	spinDataStruct;

typedef struct{
	const int stringIndex1;
	int data;
}
	menuDataStruct;
/*
typedef struct{
	MENU_UNION menu;
	void* pPara;
	void (*func)(void*);
}
	menuDictStruct;//*/

typedef struct{
	void* pData;
	int (*func)(void*);
}
	menuDataDictStruct;


typedef void(*menuFuncType)(MENU_UNION menu_union);
#endif /* MENUDEFINE_H_ */

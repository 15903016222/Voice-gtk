/*
 * menuMap.c
 *
 *  Created on: 2012-5-24
 *      Author: lzk
 */

#include "../uihandler/menu/menuDefine.h"
#include "../base.h"
extern const menuDataDictStruct**** menu4DataDict[];
void* getMenuDataAddress(MENU_UNION menu_union)
{
	if(menu4DataDict[menu_union.pos.menu1][menu_union.pos.menu2][menu_union.pos.menu4][menu_union.pos.menu3])
		return menu4DataDict[menu_union.pos.menu1][menu_union.pos.menu2][menu_union.pos.menu4][menu_union.pos.menu3]->pData;
	else
		return 0;
}

int menu4FuncTrans(MENU_UNION menu_union ,void* pPara)
{
	int retvar = FALSE;

	if(menu4DataDict[menu_union.pos.menu1][menu_union.pos.menu2][menu_union.pos.menu4][menu_union.pos.menu3]
	 && menu4DataDict[menu_union.pos.menu1][menu_union.pos.menu2][menu_union.pos.menu4][menu_union.pos.menu3]->func)
	{
		retvar = menu4DataDict[menu_union.pos.menu1][menu_union.pos.menu2][menu_union.pos.menu4][menu_union.pos.menu3]->func(pPara);
	}
	return retvar;
}

int popGetLastPos(MENU_UNION menu_union)
{
	int retVar;
	popDataStruct* pPopData = (popDataStruct*)getMenuDataAddress(menu_union);
	if(pPopData)
	{
		retVar = pPopData->lastPopPos;
	}
	else
	{
		retVar = 0;
	}
	return retVar;
}

void popSetLastPos(MENU_UNION menu_union ,int lastPos)
{
	popDataStruct* pPopData = (popDataStruct*)getMenuDataAddress(menu_union);
	if(pPopData)
	{
		pPopData->lastPopPos = lastPos;
	}
}

int menuGetData(MENU_UNION menu_union)
{
	int retVar;
	menuDataStruct* pMenuData = (menuDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		retVar = pMenuData->data;
	}
	else
	{
		retVar = 0;
	}
	return retVar;
}

void menuSetData(MENU_UNION menu_union ,int para)
{
	menuDataStruct* pMenuData = (menuDataStruct*)getMenuDataAddress(menu_union);
	if(pMenuData)
	{
		pMenuData->data = para;
	}
}

extern const menuFuncType** menu3FuncDict[];
int menu3FuncTrans(MENU_UNION menu_union)
{
	int retvar = FALSE;
	if(NULL != menu3FuncDict[menu_union.pos.menu1][menu_union.pos.menu2][menu_union.pos.menu3])
	{
		menu3FuncDict[menu_union.pos.menu1][menu_union.pos.menu2][menu_union.pos.menu3](menu_union);
		retvar = TRUE;
	}
	return retvar;
}

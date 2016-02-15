/*
 * menuMap.h
 *
 *  Created on: 2012-5-24
 *      Author: gener
 */

#ifndef MENUMAP_H_
#define MENUMAP_H_

#include "../uihandler/menu/menuDefine.h"
void* getMenuDataAddress(MENU_UNION menu_union);
int menu4FuncTrans(MENU_UNION menu_union ,void* pPara);
void popSetLastPos(MENU_UNION menu_union ,int lastPos);
int popGetLastPos(MENU_UNION menu_union);
void menuSetData(MENU_UNION menu_union ,int para);
int menuGetData(MENU_UNION menu_union);

int menu3FuncTrans(MENU_UNION menu_union);

#endif /* MENUMAP_H_ */

/*
 * menuFunc.h
 *
 *  Created on: 2012-5-7
 *      Author: lzk
 */

#ifndef MENUFUNC_H_
#define MENUFUNC_H_

#include "../../uihandler/menu/menuHandler.h"
#include "../labelbutton.h"
void menuLabelInit();
void menuSetBackground(int menu3_pos ,LABELBUTTON_MODE_ENUM menu3_mode ,LABELBUTTON_STATUS_ENUM menu_status);
void menuSetLabel(int menu3_pos ,LABELBUTTON_MODE_ENUM menu3_mode
						,LABELBUTTON_STATUS_ENUM menu_status ,const char* label3 ,const char* data3);


void menuPopdownWithMap(MENU_UNION menu_union ,	int lastPopPos);
void menuPopupWithMap(MENU_UNION menu_union ,int y);
void menuPopdownWithMalloc(MENU_UNION menu_union ,const char** label ,int num_of_item);
void menuPopupWithMalloc(MENU_UNION menu_union ,const char** label ,int num_of_item);
void menuSetDataWithMap(MENU_UNION menu_union ,LABELBUTTON_STATUS_ENUM menu_status);

void menuSpinPress(MENU_UNION menu_union);
void menuSpinRelease(MENU_UNION menu_union);
void menuSpinSetData(MENU_UNION menu_union);

void menu2Update();
void menu3Update();




extern MENU_UNION gCurrentMenuPos;
extern MENU_UNION gLastMenuPos;
extern int gReturnButtonRelease;
#define menu3Repeat() ((gLastMenuPos.pos.menu3 == gCurrentMenuPos.pos.menu3) && gReturnButtonRelease)
#endif /* MENUFUNC_H_ */

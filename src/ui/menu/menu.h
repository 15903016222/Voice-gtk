/*
 * menu.h
 *
 *  Created on: 2012-5-11
 *      Author: gener
 */

#ifndef MENU_H_
#define MENU_H_
#include "../../uihandler/menu/menuHandler.h"
//table
#include "table.h"

void menuCommon(MENU_UNION menu_union);

void menu000(MENU_UNION menu_union);
void menu001(MENU_UNION menu_union);
void menu002(MENU_UNION menu_union);
void menu003(MENU_UNION menu_union);
void menu004(MENU_UNION menu_union);
void menu005(MENU_UNION menu_union);

void menu010(MENU_UNION menu_union);
void menu011(MENU_UNION menu_union);
void menu012(MENU_UNION menu_union);
void menu013(MENU_UNION menu_union);
void menu014(MENU_UNION menu_union);
void menu015(MENU_UNION menu_union);

void menu210(MENU_UNION menu_union);
void menu211(MENU_UNION menu_union);
void menu212(MENU_UNION menu_union);
void menu213(MENU_UNION menu_union);
void menu214(MENU_UNION menu_union);
void menu215(MENU_UNION menu_union);
void menu220(MENU_UNION menu_union);
void menu221(MENU_UNION menu_union);
void menu222(MENU_UNION menu_union);
void menu223(MENU_UNION menu_union);
void menu224(MENU_UNION menu_union);
void menu225(MENU_UNION menu_union);

void menu315(MENU_UNION menu_union);

#define menu340 menuCommon
void menu341(MENU_UNION menu_union);
void menu342(MENU_UNION menu_union);
void menu343(MENU_UNION menu_union);
void menu344(MENU_UNION menu_union);
void menu345(MENU_UNION menu_union);

#define menu940 menuCommon
#define menu941 menuCommon
//void menu940(MENU_UNION menu_union);
//void menu941(MENU_UNION menu_union);
void menu942(MENU_UNION menu_union);
void menu943(MENU_UNION menu_union);
#endif /* MENU_H_ */

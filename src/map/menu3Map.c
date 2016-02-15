/*
 * menu3Map.c
 *
 *  Created on: 2012-5-22
 *      Author: lzk
 */

#include <stddef.h>
#include "../drawui.h"
#include "../ui/menu/menuFunc.h"
#include "../ui/menu/menu.h"

static const menuFuncType menu00_dict[] ={	menu000 ,menu001 ,menu002 ,menu003 ,menu004 ,menu005,};

static const menuFuncType menu01_dict[] ={	menu010 ,menu011 ,menu012 ,menu013 ,menu014 ,menu015,};


static const menuFuncType* menu0_dict[] =
{
		menu00_dict,		menu01_dict,		NULL,
		NULL,		NULL,		NULL,
};


static const menuFuncType* menu1_dict[] =
{
		NULL,		NULL,		NULL,
		NULL,		NULL,		NULL,
};

static const menuFuncType menu21_dict[] ={	menu210 ,menu211 ,menu212 ,menu213 ,menu214 ,menu215,};
static const menuFuncType menu22_dict[] ={	menu220 ,menu221 ,menu222 ,menu223 ,menu224 ,menu225,};
static const menuFuncType* menu2_dict[] =
{
		NULL,		menu21_dict,		menu22_dict,
		NULL,		NULL,		NULL,
};

static const menuFuncType menu31_dict[] ={	NULL ,NULL ,NULL ,NULL,NULL,menu315,};

static const menuFuncType menu34_dict[] ={	menu340 ,menu341 ,menu342 ,menu343, menu344, menu345,};
static const menuFuncType* menu3_dict[] =
{
		NULL,		menu31_dict,		NULL,
		NULL,		menu34_dict,		NULL,
};

static const menuFuncType* menu4_dict[] =
{
		NULL,		NULL,		NULL,
		NULL,		NULL,		NULL,
};

static const menuFuncType* menu5_dict[] =
{
		NULL,		NULL,		NULL,
		NULL,		NULL,		NULL,
};

static const menuFuncType* menu6_dict[] =
{
		NULL,		NULL,		NULL,
		NULL,		NULL,		NULL,
};

static const menuFuncType* menu7_dict[] =
{
		NULL,		NULL,		NULL,
		NULL,		NULL,		NULL,
};

static const menuFuncType* menu8_dict[] =
{
		NULL,		NULL,		NULL,
		NULL,		NULL,		NULL,
};

static const menuFuncType menu94_dict[] ={	menu940 ,menu941 ,menu942 ,menu943 ,NULL ,NULL,};
static const menuFuncType* menu9_dict[] =
{
		NULL,		NULL,		NULL,
		NULL,		menu94_dict,		NULL,
};

const menuFuncType** menu3FuncDict[] =
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


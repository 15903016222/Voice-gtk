/*
 * _string.c
 *
 *  Created on: 2012-7-11
 *      Author: lzk
 */

#include "languageDict.h"
#include "mainStringDict.h"
#include "reportStringDict.h"
#include "commonStringDict.h"
#include "labelDict.h"

#include "../globalData.h"
#include <assert.h>

const char* gThttpdPath = "/";
const char* gReportPath = "/home/tt/TT/data/Report/";
const char* gTmpReportPath = "/tmp/paReport/";
const char* gTmpReportImagePath = "/tmp/paReport/image/";
const char* gLogoFile = "/home/tt/TT/data/Report/image/logo.png";
const char* gTmpTableFileName = "/tmp/paReport/tmpTable.html";
const char* gTmpReportFileName = "/tmp/paReport/tmpReport.html";
const char* tdSpace = "&nbsp;";

const char* getDictString(_STRING_ENUM _index)
{
	assert(_index < _STRING_MAX);
	return stringDict[_index][SYSTEM(language)];
}

#include "../uihandler/menu/menuDefine.h"
const char* getMainLabelString(MENU_UNION menu_union)
{
	_STRING_ENUM _index = menuLabelDict[menu_union.pos.menu1][menu_union.pos.menu2][menu_union.pos.menu4][menu_union.pos.menu3];
	return getDictString(_index);
}

const char* getMainDictString(MAINSTRINGDICT_ENUM dictIndex ,int _index)
{
	assert(dictIndex < MAINSTRINGDICT_MAX);
	assert(_index < mainStringDict[dictIndex].size);
	assert(mainStringDict[dictIndex].index[_index] < _STRING_MAX);
	return getDictString(mainStringDict[dictIndex].index[_index]);
}

int getMainDictSize(MAINSTRINGDICT_ENUM dictIndex)
{
	assert(dictIndex < MAINSTRINGDICT_MAX);
	return mainStringDict[dictIndex].size;
}

void getMainDictStrings(MAINSTRINGDICT_ENUM dictIndex ,const char** pStrings ,int size)
{
	assert(dictIndex < MAINSTRINGDICT_MAX);
	assert(size <= mainStringDict[dictIndex].size);
	int i;
	for(i = 0 ;i < size ;i++)
	{
		pStrings[i] = getDictString(mainStringDict[dictIndex].index[i]);
	}
}


const char* getReportDictString(REPORTSTRINGDICT_ENUM dictIndex ,int _index)
{
	assert(dictIndex < REPORTSTRINGDICT_MAX);
	assert(_index < reportStringDict[dictIndex].size);
	assert(reportStringDict[dictIndex].index[_index] < _STRING_MAX);
	return getDictString(reportStringDict[dictIndex].index[_index]);
}

int getReportDictSize(REPORTSTRINGDICT_ENUM dictIndex)
{
	assert(dictIndex < REPORTSTRINGDICT_MAX);
	return reportStringDict[dictIndex].size;
}


const stringStruct getCommonString(COMMONSTRING_ENUM _index)
{
	assert(_index < COMMONSTRING_MAX);
	return commonString[_index];
}

const char* getUnit(UNITS_ENUM unit)
{
	assert(unit < UNITS_MAX);
	assert(unit < commonString[COMMONSTRING_UNIT].numofstring);
	return commonString[COMMONSTRING_UNIT].stringDict[unit];
}


const char* getDataPath(DATA_PATH_ENUM dataPath)
{
	assert(dataPath < DATA_PATH_MAX);
	assert(dataPath < commonString[COMMONSTRING_DATA_PATH].numofstring);
	return commonString[COMMONSTRING_DATA_PATH].stringDict[dataPath];
}

const char* getDataSuffix(DATA_SUFFIX_ENUM dataSuffix)
{
	assert(dataSuffix < DATA_SUFFIX_MAX);
	assert(dataSuffix < commonString[COMMONSTRING_DATA_SUFFIX].numofstring);
	return commonString[COMMONSTRING_DATA_SUFFIX].stringDict[dataSuffix];
}


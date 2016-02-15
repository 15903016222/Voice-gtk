/*
 * _string.h
 *
 *  Created on: 2012-7-11
 *      Author: gener
 */

#ifndef STRING_H_
#define STRING_H_

#include "language.h"
#include "getFieldString.h"

const char* getDictString(_STRING_ENUM index);

const char* getMainDictString(MAINSTRINGDICT_ENUM dictIndex ,int _index);
int getMainDictSize(MAINSTRINGDICT_ENUM dictIndex);
//需先给pStrings分配内存
void getMainDictStrings(MAINSTRINGDICT_ENUM dictIndex ,const char** pStrings ,int size);

const char* getReportDictString(REPORTSTRINGDICT_ENUM dictIndex ,int _index);
int getReportDictSize(REPORTSTRINGDICT_ENUM dictIndex);


const stringStruct getCommonString(COMMONSTRING_ENUM _index);
const char* getUnit(UNITS_ENUM unit);
const char* getDataPath(DATA_PATH_ENUM dataPath);
const char* getDataSuffix(DATA_PATH_ENUM dataSuffix);
extern const char* gThttpdPath;
extern const char* gReportPath;
extern const char* gTmpReportPath;
extern const char* gTmpReportImagePath;
extern const char* gLogoFile;
extern const char* gTmpTableFileName;
extern const char* gTmpReportFileName;
extern const char* tdSpace;
#define WEB_PREFIX "http://127.0.0.1/"


//linshi
#include "../uihandler/menu/menuDefine.h"
const char* getMainLabelString(MENU_UNION menu_union);
#endif /* STRING_H_ */

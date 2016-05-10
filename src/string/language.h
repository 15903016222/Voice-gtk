/*
 * language.h
 *
 *  Created on: 2012-7-11
 *      Author: lzk
 */

#ifndef ___LANGUAGE_H_
#define ___LANGUAGE_H_

#include <stddef.h>
#include "language_d.h"
#include "string_d.h"
typedef enum
{
	LANGUAGE_ENGLISH,
	LANGUAGE_CHINESE,
	LANGUAGE_MAX
}
	LANGUAGE_ENUM;


typedef struct stringDictStruct_
{
	_STRING_ENUM* index;
	int size;
}
	stringDictStruct;

#define NUM_OF_STRING_INDEXS(a) sizeof(a) / sizeof(_STRING_ENUM)


typedef struct{
	const char** stringDict;
	int numofstring;
}
	stringStruct;

#define NUM_OF_STRING(a) (sizeof(a) / sizeof(const char*))

typedef enum
{
	UNITS_NONE,
	UNITS_MM,
	UNITS_INCH,
	UNITS_ANGLE,
	UNITS_MS,
	UNITS_US,
	UNITS_NS,
	UNITS_S,
	UNITS_MAX
}
	UNITS_ENUM;

typedef enum{
	DATA_PATH_SETUP,//				"/home/tt/TT/source/user/setup/", //setup
	DATA_PATH_DATA,//				"/home/tt/TT/data/InspectData/", //data
	DATA_PATH_REPORT,//				"/home/tt/TT/data/Report/", //report
	DATA_PATH_PICTURE,//			"/home/tt/TT/data/Image/",//picture
    DATA_PATH_CAD,//				"/home/tt/TT/data/Part/",
    DATA_PATH_TMPL,                 //"/home/tt/TT/source/system/Template/Report/", // report template
	DATA_PATH_USB,//				"/opt/usbStorage/",//usb
//	DATA_PATH_SETUP_SAMPLE,//		"/home/tt/",//setup sample path
	DATA_PATH_DATA_SAMPLE,//		"/home/tt/TT/",//data sample path

	DATA_PATH_MAX,//
}
	DATA_PATH_ENUM;

typedef enum{
	DATA_SUFFIX_SETUP,//		".cfg", //setup
	DATA_SUFFIX_DATA,//		".data", //data
	DATA_SUFFIX_REPORT,//		".html", //report
	DATA_SUFFIX_PICTURE,//	".jpg", //picture
    DATA_SUFFIX_CAD,//      ".dxf", //dxf
	DATA_SUFFIX_ALL,//		"",//all

	DATA_SUFFIX_MAX,//
}
DATA_SUFFIX_ENUM;
#endif /* ___LANGUAGE_H_ */

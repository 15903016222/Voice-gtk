/*
 * spinDataMap.h
 *
 *  Created on: 2012-5-24
 *      Author: gener
 */

#ifndef SPINDATAMAP_H_
#define SPINDATAMAP_H_

#include "../string/_string.h"
#include "../base.h"
enum
{

	SPINDATA_CONNECTION_P,//		{1.0      ,1.0  		,128.0 		,1.0   ,0 ,UNITS_NONE},//0 //connection P
	SPINDATA_SCAN_OFFSET,//		{0.0      ,-1000000.0 	,9999999.0	,0.1   ,1 ,UNITS_MM},//1 //scan offset
	SPINDATA_THICKNESS,//			{0.05	  ,0.05         ,500000.0   ,0.1   ,2 ,UNITS_MM},//2 //thickness
	SPINDATA_DIAMETER,//			{0.10	  ,0.10         ,1000000.00 ,0.1   ,2 ,UNITS_MM},//3 //diameter
	SPINDATA_CONNECTION_R,//		{1.0      ,1.0  		,128.0 		,1.0   ,0 ,UNITS_NONE},//4 //connection R
	SPINDATA_RECEIVE_PA,//			{1.0      ,1.0  		,128.0 		,1.0   ,0 ,UNITS_NONE},//5 //receive_pa
	SPINDATA_PULSE,//				{1.0      ,1.0  		,128.0 		,1.0   ,0 ,UNITS_NONE},//6 //pulse
	SPINDATA_RECEIVE,//			{1.0      ,1.0  		,128.0 		,1.0   ,0 ,UNITS_NONE},//7 //receive
	SPINDATA_INDEX_OFFSET,//		{0.0	  ,-1000000.0   ,9999999.0  ,0.1   ,1 ,UNITS_MM},//8 //index offset
	SPINDATA_SKEW,//				{0.0	  ,0.0          ,359.9      ,0.1   ,1 ,UNITS_ANGLE},//9 //skew
									//focal law向导
	SPINDATA_ELEMENT_QTY,//		{1.0      ,1.0  		,32.0		,1.0   ,0 ,UNITS_NONE},//10 //element qty.
	SPINDATA_FIRST_ELEMENT,//		{1.0      ,1.0  		,32.0 		,1.0   ,0 ,UNITS_NONE},//11 //first element
	SPINDATA_LAST_ELEMENT,//		{1.0      ,1.0  		,32.0 		,1.0   ,0 ,UNITS_NONE},//12 //last element
	SPINDATA_ELEMENT_STEP,//		{1.0      ,1.0  		,32.0 		,1.0   ,0 ,UNITS_NONE},//13 //element step
	SPINDATA_MIN_ANGLE,//			{0.0	  ,-89.9        ,89.9       ,0.1   ,1 ,UNITS_ANGLE},//14 //min angle
	SPINDATA_MAX_ANGLE,//			{0.0	  ,-89.9        ,89.9       ,0.1   ,1 ,UNITS_ANGLE},//15 //max angle
	SPINDATA_ANGLE_STEP,//			{0.0	  ,0.0          ,89.9       ,0.1   ,1 ,UNITS_ANGLE},//16 //angle step
	SPINDATA_FOCUS_DEPTH,//		{0.05	  ,0.05         ,1000.0     ,0.1   ,2 ,UNITS_NONE},//17 //focus depth
									//output 220
	SPINDATA_COUNT1,//				{1.0	  ,1.0          ,100.0      ,1.0   ,0 ,UNITS_NONE},//18 //count
	SPINDATA_COUNT2,//				{1.0	  ,1.0          ,100.0      ,1.0   ,0 ,UNITS_NONE},//19 //count
	SPINDATA_COUNT3,//				{1.0	  ,1.0          ,100.0      ,1.0   ,0 ,UNITS_NONE},//20 //count
	SPINDATA_DELAY1,//				{0.0	  ,0.0          ,5000.0     ,1.0   ,2 ,UNITS_MS},//21 //delay
	SPINDATA_DELAY2,//				{0.0	  ,0.0          ,5000.0     ,1.0   ,2 ,UNITS_MS},//22 //delay
	SPINDATA_DELAY3,//				{0.0	  ,0.0          ,5000.0     ,1.0   ,2 ,UNITS_MS},//23 //delay
	SPINDATA_HOLD_TIME1,//			{100.0	  ,0.0          ,5000.0     ,1.0   ,2 ,UNITS_MS},//24 //hold time
	SPINDATA_HOLD_TIME2,//			{100.0	  ,0.0          ,5000.0     ,1.0   ,2 ,UNITS_MS},//25 //hold time
	SPINDATA_HOLD_TIME3,//			{100.0	  ,0.0          ,5000.0     ,1.0   ,2 ,UNITS_MS},//26 //hold time
									//focal type
	SPINDATA_POSITION_START,//		{1.0	  ,0.01         ,10000.0    ,0.1   ,1 ,UNITS_MM},//27 position start
	SPINDATA_POSITION_END,//		{1.0	  ,0.01         ,10000.0    ,0.1   ,2 ,UNITS_MM},//28 position end
	SPINDATA_POSITION_STEP,//		{1.0	  ,0.01         ,10000.0    ,0.1   ,2 ,UNITS_MM},//29 position step
	SPINDATA_OFFSET_START,//		{1.0	  ,0.01         ,10000.0    ,0.5   ,2 ,UNITS_MM},//30 offset start
	SPINDATA_OFFSET_END,//			{1.0	  ,0.01         ,10000.0    ,0.5   ,2 ,UNITS_MM},//31 offset end
	SPINDATA_DEPTH_START,//		{1.0	  ,0.01         ,10000.0    ,0.5   ,2 ,UNITS_MM},//32 depth start
	SPINDATA_DEPTH_END,//			{1.0	  ,0.01         ,10000.0    ,0.5   ,2 ,UNITS_MM},//33 depth end

	SPINDATA_TOFD_REFLINE,//       {1.0	  ,1            ,100        ,1     ,2 ,UNITS_MM},
	SPINDATA_TOFD_PCS,//       	{0.0	  ,0            ,1000        ,1     ,2 ,UNITS_MM},
    SPINDATA_TOFD_REFLINE_DEPTH,//           {0.0      ,0            ,1000        ,1     ,2 ,UNITS_MM},

	SPINDATA_MAX,//
};
#define spinDataAddress(index) (spinDataDict + (index))

static spinDataStruct spinDataDict[] =
{
								//group向导
								//value    lower        upper		step   digit  unit
/*SPINDATA_CONNECTION_P*/		{1.0      ,1.0  		,128 		,1.0   ,0 ,UNITS_NONE},//0 //connection P
/*SPINDATA_SCAN_OFFSET*/		{0.0      ,-1000000.0 	,9999999.0	,0.1   ,2 ,UNITS_MM},//1 //scan offset
/*SPINDATA_THICKNESS*/			{0.05	  ,0.05         ,500000.0   ,0.1   ,2 ,UNITS_MM},//2 //thickness
/*SPINDATA_DIAMETER*/			{0.10	  ,0.10         ,1000000.00 ,0.1   ,2 ,UNITS_MM},//3 //diameter
/*SPINDATA_CONNECTION_R*/		{1.0      ,1.0  		,128 		,1.0   ,0 ,UNITS_NONE},//4 //connection R
/*SPINDATA_RECEIVE_PA*/			{1.0      ,1.0  		,128 		,1.0   ,0 ,UNITS_NONE},//5 //receive_pa
/*SPINDATA_PULSE*/				{1.0      ,1.0  		,128 		,1.0   ,0 ,UNITS_NONE},//6 //pulse
/*SPINDATA_RECEIVE*/			{1.0      ,1.0  		,128 		,1.0   ,0 ,UNITS_NONE},//7 //receive
/*SPINDATA_INDEX_OFFSET*/		{0.0	  ,-1000000.0   ,9999999.0  ,0.1   ,2 ,UNITS_MM},//8 //index offset
/*SPINDATA_SKEW*/				{0.0	  ,0.0          ,359.9      ,0.1   ,1 ,UNITS_ANGLE},//9 //skew
								//focal law向导
/*SPINDATA_ELEMENT_QTY*/		{1.0      ,1.0  		,32		,1.0   ,0 ,UNITS_NONE},//10 //element qty.
/*SPINDATA_FIRST_ELEMENT*/		{1.0      ,1.0  		,32 		,1.0   ,0 ,UNITS_NONE},//11 //first element
/*SPINDATA_LAST_ELEMENT*/		{1.0      ,1.0  		,32 		,1.0   ,0 ,UNITS_NONE},//12 //last element
/*SPINDATA_ELEMENT_STEP*/		{1.0      ,1.0  		,32 		,1.0   ,0 ,UNITS_NONE},//13 //element step
/*SPINDATA_MIN_ANGLE*/			{0.0	  ,-89.9        ,89.9       ,0.1   ,1 ,UNITS_ANGLE},//14 //min angle
/*SPINDATA_MAX_ANGLE*/			{0.0	  ,-89.9        ,89.9       ,0.1   ,1 ,UNITS_ANGLE},//15 //max angle
/*SPINDATA_ANGLE_STEP*/			{0.0	  ,0.0          ,89.9       ,0.1   ,1 ,UNITS_ANGLE},//16 //angle step
/*SPINDATA_FOCUS_DEPTH*/		{0.05	  ,0.05         ,1000.0     ,0.1   ,2 ,UNITS_NONE},//17 //focus depth
								//output 220
/*SPINDATA_COUNT1*/				{1.0	  ,1.0          ,100.0      ,1.0   ,0 ,UNITS_NONE},//18 //count
/*SPINDATA_COUNT2*/				{1.0	  ,1.0          ,100.0      ,1.0   ,0 ,UNITS_NONE},//19 //count
/*SPINDATA_COUNT3*/				{1.0	  ,1.0          ,100.0      ,1.0   ,0 ,UNITS_NONE},//20 //count
/*SPINDATA_DELAY1*/				{0.0	  ,0.0          ,5000.0     ,1.0   ,2 ,UNITS_MS},//21 //delay
/*SPINDATA_DELAY2*/				{0.0	  ,0.0          ,5000.0     ,1.0   ,2 ,UNITS_MS},//22 //delay
/*SPINDATA_DELAY3*/				{0.0	  ,0.0          ,5000.0     ,1.0   ,2 ,UNITS_MS},//23 //delay
/*SPINDATA_HOLD_TIME1*/			{100.0	  ,0.0          ,5000.0     ,1.0   ,2 ,UNITS_MS},//24 //hold time
/*SPINDATA_HOLD_TIME2*/			{100.0	  ,0.0          ,5000.0     ,1.0   ,2 ,UNITS_MS},//25 //hold time
/*SPINDATA_HOLD_TIME3*/			{100.0	  ,0.0          ,5000.0     ,1.0   ,2 ,UNITS_MS},//26 //hold time
								//focal type
/*SPINDATA_POSITION_START*/		{1.0	  ,0.01         ,10000.0    ,0.1   ,2 ,UNITS_MM},//27 position start
/*SPINDATA_POSITION_END*/		{1.0	  ,0.01         ,10000.0    ,0.1   ,2 ,UNITS_MM},//28 position end
/*SPINDATA_POSITION_STEP*/		{1.0	  ,0.01         ,10000.0    ,0.1   ,2 ,UNITS_MM},//29 position step
/*SPINDATA_OFFSET_START*/		{1.0	  ,0.01         ,10000.0    ,0.5   ,2 ,UNITS_MM},//30 offset start
/*SPINDATA_OFFSET_END*/			{1.0	  ,0.01         ,10000.0    ,0.5   ,2 ,UNITS_MM},//31 offset end
/*SPINDATA_DEPTH_START*/		{1.0	  ,0.01         ,10000.0    ,0.5   ,2 ,UNITS_MM},//32 depth start
/*SPINDATA_DEPTH_END*/			{1.0	  ,0.01         ,10000.0    ,0.5   ,2 ,UNITS_MM},//33 depth end

/*SPINDATA_TOFD_REFLINE*/       {0.0	  ,1            ,100        ,1     ,2 ,UNITS_MM},
/*SPINDATA_TOFD_PCS*/       	{0.0	  ,0            ,1000        ,1     ,2 ,UNITS_MM},
/*SPINDATA_TOFD_REFLINE_DEPTH*/ {0.0   ,0            ,1000        ,1     ,2 ,UNITS_MM},

/*SPINDATA_MAX*/
};


#endif /* SPINDATAMAP_H_ */

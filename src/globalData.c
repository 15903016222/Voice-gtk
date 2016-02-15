/*
 * globalData.c
 *
 *  Created on: 2012-6-20
 *      Author: lzk
 */

#include "globalData.h"
#include <string.h>
#include "drawui.h"
extern void ascanDataInit();

globalDataStruct globalData;
globalDataStruct* gData;

void fftInit()
{
	int group;
	for(group = 0 ;group < 8 ;group++)
	{
		gData->fft[group].hzPerPoint = 50.0 / 1024;
//		gData->fft[group].peakPoint = 0;
//		gData->fft[group].db6minPoint = 0;
//		gData->fft[group].db6maxPoint = 0;
//		gData->fft[group].db20minPoint = 0;
//		gData->fft[group].db20maxPoint = 0;
	}
}


void globalDataInit()
{
	gData = &globalData;
	memset(gData ,0 ,sizeof(globalDataStruct));

	//file
	strcpy(gData->file.setupfile ,"default.cfg");

	//fft data //默认都为0
	fftInit();

	ascanDataInit();

}

void globalDataInitAfterReadCfg()
{
	//读取default.cfg文件后
	SYSTEM(language) = get_language(pp->p_config);
	//硬件相关，留后处理
//	output_init(GetChannelEnable()) ;
}

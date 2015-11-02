/*
 * globalData.h
 *
 *  Created on: 2012-6-20
 *      Author: lzk
 */

#ifndef GLOBALDATA_H_
#define GLOBALDATA_H_
#include "string/language.h"
#include "lzk/Ascan.h"
#include "lzk/output_spi.h"
#include "lzk/fft.h"

#define FILE_SIZE 100

#define VERSION_SETUPFILE 1
#define VERSION_DATAFILE 1


struct fileStruct
{
	char setupfile[FILE_SIZE];
	int loadFail;
};

struct groupStruct
{
	char probeType[32];
	char probeFileName[32];
	char wedgeType[32];
	char wedgeFileName[32];
};

struct systemStruct
{
	//int language;
	LANGUAGE_ENUM language;
	struct groupStruct groupData[8];
	output_to_spi_t output;
};

typedef struct
{
	struct fileStruct file;
	struct fftStruct fft[8];
	struct systemStruct system;
	ascanStruct ascan[8];
	struct groupStruct tmpGroupData;
}
	globalDataStruct;

extern globalDataStruct* gData;

#define SYSTEM(a) gData->system.a

#define gGroupData gData->system.groupData
#define gAscanData gData->ascan

void globalDataInit();
void globalDataInitAfterReadCfg();
#endif /* GLOBALDATA_H_ */

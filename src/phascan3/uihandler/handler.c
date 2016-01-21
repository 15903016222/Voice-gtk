/*
 * fileHandler1.c
 *
 *  Created on: 2012-5-3
 *      Author: lzk
 */

#include "../drawui.h"
#include "../callback.h"
#include "../lzk/fileHandler.h"
#include "../main.h"
#include "../globalData.h"
#include "../map/popDataMap.h"

int handlerOpenSetupFile(const char* filename)
{
	int retvar;
	retvar = readConfigFile(filename)  ;
	if(0 == retvar)
	{
		request_refresh(REFRESH_SETUP_CONFIG_LOAD) ;
	}
	return retvar;
}


int HandlerOpenDataFile(const char* filename)
{
	int retvar;


    retvar = ReadDataFileSetup(filename);
    if(0 ==  retvar) {
		retvar = ReadDataFileData(filename);
	}

	return retvar;
}

extern const menuDataDictStruct**** menu4DataDict[];
void HandlerResetPopData()
{
    ((popDataStruct*)menu4DataDict[0][0][1][2]->pData)->lastPopPos = popDataAddress(POPDATA_GROUP_SELECT)->lastPopPos;
    ((popDataStruct*)menu4DataDict[0][0][1][2]->pData)->itemStatus = popDataAddress(POPDATA_GROUP_SELECT)->itemStatus;

    ((popDataStruct*)menu4DataDict[0][0][2][2]->pData)->lastPopPos = popDataAddress(POPDATA_SOURCE)->lastPopPos;
    ((popDataStruct*)menu4DataDict[0][0][2][2]->pData)->itemStatus = popDataAddress(POPDATA_SOURCE)->itemStatus;

    ((popDataStruct*)menu4DataDict[0][0][3][2]->pData)->lastPopPos = popDataAddress(POPDATA_GEOMETRY)->lastPopPos;
    ((popDataStruct*)menu4DataDict[0][0][3][2]->pData)->itemStatus = popDataAddress(POPDATA_GEOMETRY)->itemStatus;

    ((popDataStruct*)menu4DataDict[0][0][3][5]->pData)->lastPopPos = popDataAddress(POPDATA_MATERIAL)->lastPopPos;
    ((popDataStruct*)menu4DataDict[0][0][3][5]->pData)->itemStatus = popDataAddress(POPDATA_MATERIAL)->itemStatus;

    ((popDataStruct*)menu4DataDict[0][0][4][2]->pData)->lastPopPos = popDataAddress(POPDATA_GROUP_MODE)->lastPopPos;
    ((popDataStruct*)menu4DataDict[0][0][4][2]->pData)->itemStatus = popDataAddress(POPDATA_GROUP_MODE)->itemStatus;

    ((popDataStruct*)menu4DataDict[0][0][5][2]->pData)->lastPopPos = popDataAddress(POPDATA_TR_MODE)->lastPopPos;
    ((popDataStruct*)menu4DataDict[0][0][5][2]->pData)->itemStatus = popDataAddress(POPDATA_TR_MODE)->itemStatus;

    ((popDataStruct*)menu4DataDict[0][0][6][2]->pData)->lastPopPos = popDataAddress(POPDATA_PE_SELECT_PROBE)->lastPopPos;
    ((popDataStruct*)menu4DataDict[0][0][6][2]->pData)->itemStatus = popDataAddress(POPDATA_PE_SELECT_PROBE)->itemStatus;

    ((popDataStruct*)menu4DataDict[0][0][11][2]->pData)->lastPopPos = popDataAddress(POPDATA_SELECT_PROBE)->lastPopPos;
    ((popDataStruct*)menu4DataDict[0][0][11][2]->pData)->itemStatus = popDataAddress(POPDATA_SELECT_PROBE)->itemStatus;

    ((popDataStruct*)menu4DataDict[0][0][7][2]->pData)->lastPopPos = popDataAddress(POPDATA_PE_SELECT_WEDGE)->lastPopPos;
    ((popDataStruct*)menu4DataDict[0][0][7][2]->pData)->itemStatus = popDataAddress(POPDATA_PE_SELECT_WEDGE)->itemStatus;

    ((popDataStruct*)menu4DataDict[0][0][12][2]->pData)->lastPopPos = popDataAddress(POPDATA_SELECT_WEDGE)->lastPopPos;
    ((popDataStruct*)menu4DataDict[0][0][12][2]->pData)->itemStatus = popDataAddress(POPDATA_SELECT_WEDGE)->itemStatus;

    ((popDataStruct*)menu4DataDict[0][1][1][2]->pData)->lastPopPos = popDataAddress(POPDATA_FOCAL_GROUP_SELECT)->lastPopPos;
    ((popDataStruct*)menu4DataDict[0][1][1][2]->pData)->itemStatus = popDataAddress(POPDATA_FOCAL_GROUP_SELECT)->itemStatus;

    ((popDataStruct*)menu4DataDict[0][1][4][2]->pData)->lastPopPos = popDataAddress(POPDATA_WAVE_TYPE)->lastPopPos;
    ((popDataStruct*)menu4DataDict[0][1][4][2]->pData)->itemStatus = popDataAddress(POPDATA_WAVE_TYPE)->itemStatus;

    ((popDataStruct*)menu4DataDict[0][1][2][2]->pData)->lastPopPos = popDataAddress(POPDATA_LAW_TYPE)->lastPopPos;
    ((popDataStruct*)menu4DataDict[0][1][2][2]->pData)->itemStatus = popDataAddress(POPDATA_LAW_TYPE)->itemStatus;

    ((popDataStruct*)menu4DataDict[0][1][2][3]->pData)->lastPopPos = popDataAddress(POPDATA_FOCAL_TYPE)->lastPopPos;
    ((popDataStruct*)menu4DataDict[0][1][2][3]->pData)->itemStatus = popDataAddress(POPDATA_FOCAL_TYPE)->itemStatus;

    ((popDataStruct*)menu4DataDict[2][1][0][0]->pData)->lastPopPos = popDataAddress(POPDATA_ALARM)->lastPopPos;
    ((popDataStruct*)menu4DataDict[2][1][0][0]->pData)->itemStatus = popDataAddress(POPDATA_ALARM)->itemStatus;

    ((popDataStruct*)menu4DataDict[2][1][0][1]->pData)->lastPopPos = popDataAddress(POPDATA_ALARM_GROUP_SELECT1)->lastPopPos;
    ((popDataStruct*)menu4DataDict[2][1][0][1]->pData)->itemStatus = popDataAddress(POPDATA_ALARM_GROUP_SELECT1)->itemStatus;

    ((popDataStruct*)menu4DataDict[2][2][3][2]->pData)->lastPopPos = popDataAddress(POPDATA_ANALOG_GROUP1)->lastPopPos;
    ((popDataStruct*)menu4DataDict[2][2][3][2]->pData)->itemStatus = popDataAddress(POPDATA_ANALOG_GROUP1)->itemStatus;

    ((popDataStruct*)menu4DataDict[2][2][3][2]->pData)->lastPopPos = popDataAddress(POPDATA_ANALOG_DATA1)->lastPopPos;
    ((popDataStruct*)menu4DataDict[2][2][3][2]->pData)->itemStatus = popDataAddress(POPDATA_ANALOG_DATA1)->itemStatus;

    ((popDataStruct*)menu4DataDict[2][2][4][2]->pData)->lastPopPos = popDataAddress(POPDATA_ANALOG_GROUP2)->lastPopPos;
    ((popDataStruct*)menu4DataDict[2][2][4][2]->pData)->itemStatus = popDataAddress(POPDATA_ANALOG_GROUP2)->itemStatus;

    ((popDataStruct*)menu4DataDict[2][2][4][3]->pData)->lastPopPos = popDataAddress(POPDATA_ANALOG_DATA2)->lastPopPos;
    ((popDataStruct*)menu4DataDict[2][2][4][3]->pData)->itemStatus = popDataAddress(POPDATA_ANALOG_DATA2)->itemStatus;
}

void HandlerResetCFG()
{
	memset((void*)(pp->p_config) , 0 , sizeof(CONFIG)) ;
	set_config(0);
	//add by lzk
	globalDataInitAfterReadCfg();
	gData->file.loadFail = TRUE;//重设alarm值
	request_refresh(REFRESH_SETUP_CONFIG_LOAD) ;
}

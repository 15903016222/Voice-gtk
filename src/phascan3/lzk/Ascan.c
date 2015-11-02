/*
 * Ascan.c
 *
 *  Created on: 2012-5-8
 *      Author: lzk
 */
#include <assert.h>
#include <string.h>
#include "../drawfb.h"
#include "../base.h"
#include "../main.h"
#include "Ascan.h"
#include "../globalData.h"

static inline void compressData(const unsigned char* const sourceData ,unsigned char* targetData ,
		int sourceQty, int targetQty ,unsigned int rfMode)
{
	int i,j;
	int lo,hi;
	for(i = 0 ;i < targetQty ;i++)
	{
		lo = i * sourceQty / targetQty;
		hi = (i + 1) * sourceQty / targetQty;
		targetData[i] = sourceData[lo];
		if(rfMode)
		{
			unsigned int max ,temp;
			max = abs(targetData[i] - 128);
			for(j = lo + 1 ;j < hi ;j++)
			{
				temp = abs(targetData[i] - 128);
				if(temp > max)
				{
					max = temp;
					targetData[i] = sourceData[j];
				}
			}
		}
		else
		{
			for(j = lo + 1 ;j < hi ;j++)
			{
				if(sourceData[j] > targetData[i])
				{
					targetData[i] = sourceData[j];
				}
			}
		}
	}
}

static inline void interpolationData(const unsigned char* const sourceData, unsigned char* targetData,
						int sourceQty, int targetQty)
{
	int	i, j;
	int lo,hi;
	signed int temp;
	for(i = 0 ;i < sourceQty - 1 ;i++)
	{
		lo = i * targetQty / (sourceQty - 1);
		hi = (i + 1) * targetQty / (sourceQty - 1);
		temp = sourceData[i + 1] - sourceData[i];
		for(j = lo ;j < hi ;j++)
		{
			targetData[j] = sourceData[i] + temp * (j - lo) / (hi - lo);
		}
	}
}


void getDisplayData(const unsigned char* const sourceData ,unsigned char* targetData ,
		int sourceQty, int targetQty ,unsigned int rfMode)
{
	if(sourceQty == targetQty)
	{
		memcpy ((void *)(targetData), (void *)(sourceData), sourceQty);
	}
	else if(sourceQty > targetQty)
	{
		compressData(sourceData ,targetData ,sourceQty ,targetQty ,rfMode);
	}
	else
	{
		interpolationData(sourceData ,targetData ,sourceQty ,targetQty);
	}
}

void ascanDataInit()
{
	int i;
	for(i = 0 ;i < 8 ;++i)
	{
//		memset(gAscanData + i ,0 ,sizeof(ascanStruct));
		gAscanData[i].group = 0xff;//无效组
	}
	//printf("size of ascanStruct:%d\n" ,sizeof(ascanStruct));
}

void ascanResetEnvelope(int group)
{
	int i;
	for(i = 0 ;i < 8 ;++i)
	{
		if(gAscanData[i].group == group)
		{
			gAscanData[i].envelope.bReset = TRUE;
		}
	}
}

void ascanResetRef(int group)
{
	int i;
	for(i = 0 ;i < 8 ;++i)
	{
		if(gAscanData[i].group == group)
		{
			gAscanData[i].ref.bReset = TRUE;//更新数据
//			gAscanData[i].ref.bRedraw = TRUE;//重新压缩或放大
			gAscanData[i].bClear = TRUE;
		}
	}
}

void ascanResetPeak(int group)
{
	int i;
	for(i = 0 ;i < 8 ;++i)
	{
		if(gAscanData[i].group == group)
		{
			gAscanData[i].peak.amplitude = 0;//比0大都会造成更新数据
			gAscanData[i].peak.bRedraw = TRUE;//重压缩或放大
		}
	}
}

void ascanResetCurvesWhenStartOrRangeChanged(int group)
{
	int i;
	for(i = 0 ;i < 8 ;++i)
	{
		if(gAscanData[i].group == group)
		{
			gAscanData[i].ref.bRedraw = TRUE;//重画

			gAscanData[i].envelope.bReset = TRUE;//重置

			gAscanData[i].peak.amplitude = 0;//重置

		}
	}
}

void ascanClearDisp(int group)
{
	int i;
	for(i = 0 ;i < 8 ;++i)
	{
		if(gAscanData[i].group == group)
		{
			gAscanData[i].bClear = TRUE;
		}
	}
}

void ascanClearAndRedraw(int group)
{
	int i;
	for(i = 0 ;i < 8 ;++i)
	{
	//	if(gAscanData[i].group == group)
		{
			gAscanData[i].bClear = TRUE;

			gAscanData[i].peak.amplitude = 0;//比0大都会造成更新数据
			gAscanData[i].peak.bRedraw = TRUE;//重压缩或放大

			gAscanData[i].ref.bReset = TRUE;//更新数据

			gAscanData[i].envelope.bReset = TRUE;
		}
	}
}

void ascanInitRefData(ascanStruct* pAscanData)
{
	pAscanData->ref.dataPoints = 0;
	memset(pAscanData->ref.originalData ,0 ,WAVE_MAX_POINT);
}

void ascanInitPeakData(ascanStruct* pAscanData)
{
	pAscanData->peak.amplitude = 0;
	pAscanData->peak.dataPoints = 0;
	memset(pAscanData->peak.originalData ,0 ,WAVE_MAX_POINT);
}

void ascanResetEnvelopeData(ascanStruct* pAscanData)
{
	pAscanData->envelope.bReset = FALSE;
	memcpy(pAscanData->envelope.dispData[0] ,pAscanData->wave.dispData ,pAscanData->wave.dispDataWidth);
	memcpy(pAscanData->envelope.dispData[1] ,pAscanData->wave.dispData ,pAscanData->wave.dispDataWidth);
}

void ascanSetRefDispData(ascanStruct* pAscanData)
{
	if(pAscanData->ref.range)
	{
		pAscanData->ref.width = pAscanData->wave.dispDataWidth * (pAscanData->ref.range)
								/ pAscanData->range;
		pAscanData->ref.offset = pAscanData->wave.dispDataWidth * (int)(pAscanData->ref.start - pAscanData->start)
								/ pAscanData->ref.range;
	}

	getDisplayData(pAscanData->ref.originalData,
			pAscanData->ref.dispData,
			pAscanData->ref.dataPoints,
			pAscanData->ref.width,
			pAscanData->wave.rfMode) ;
}

void ascanResetRefData(ascanStruct* pAscanData)
{
	pAscanData->ref.bReset = FALSE;

	pAscanData->ref.start = pAscanData->start;
	pAscanData->ref.range = pAscanData->range;

	pAscanData->ref.dataPoints = pAscanData->wave.dataPoints;
	memcpy(pAscanData->ref.originalData ,pAscanData->wave.pData ,pAscanData->ref.dataPoints);

	int group = pAscanData->group;
	int i;
	int offset ;
	for (offset = 0, i = 0 ; i < group; i++)
		offset += TMP(beam_qty[i]);
	int _nIndex = offset + TMP(beam_num[group]);
	memcpy ((void*)(GROUP_VAL_POS(group , nReferenceData)) ,(void*)TMP(measure_data[_nIndex]), 32);

	pAscanData->ref.bRedraw = TRUE;
}

void ascanSetPeakDispData(ascanStruct* pAscanData)
{
	getDisplayData(pAscanData->peak.originalData,
					pAscanData->peak.dispData,
					pAscanData->peak.dataPoints,
					pAscanData->wave.dispDataWidth,
					pAscanData->wave.rfMode) ;
}

void ascanResetPeakData(ascanStruct* pAscanData)
{
	pAscanData->peak.dataPoints = pAscanData->wave.dataPoints;
//	memset(pAscanData->peak.originalData ,0 ,pAscanData->peak.dataPoints);
	memcpy(pAscanData->peak.originalData ,pAscanData->wave.pData ,pAscanData->peak.dataPoints);
	pAscanData->peak.bRedraw = TRUE;
}

void ascanSetEnvelope(ascanStruct* pAscanData)
{
	int i;
	for(i = 0 ;i < pAscanData->wave.dispDataWidth ;++i)
	{
		if(pAscanData->envelope.dispData[0][i] < pAscanData->wave.dispData[i])
		{
			pAscanData->envelope.dispData[0][i] = pAscanData->wave.dispData[i];
		}
		if(pAscanData->envelope.dispData[1][i] > pAscanData->wave.dispData[i])
		{
			pAscanData->envelope.dispData[1][i] = pAscanData->wave.dispData[i];
		}
	}
}

void ascanResetDispData(ascanStruct* pAscanData)
{
	ascanResetEnvelopeData(pAscanData);
	ascanSetRefDispData(pAscanData);
	ascanSetPeakDispData(pAscanData);
}

static inline int ascanGetPointHorizontalAxis(ascanStruct* pAscanData ,int i ,int j)
{
	int x;
	if(pAscanData->axis.direction == DIRECTION_HORIZONTAL)
		x = (j + pAscanData->axis.y) * FB_WIDTH  + i  + pAscanData->axis.x ;
	else
		x = (i + pAscanData->axis.y) * FB_WIDTH  + j  + pAscanData->axis.x ;
	return x;
}

static inline int ascanGetPointVerticalAxis(const ascanStruct* pAscanData ,const unsigned char dispData)
{
	int y;
	if(pAscanData->axis.direction == DIRECTION_HORIZONTAL)
	{
		y = (pAscanData->axis.vertical - 1) * (255 - dispData) / 255;
		if(0 == y)
		{
			y = -1;
		}
		if(pAscanData->wave.rfMode)
		{
			if(pAscanData->axis.vertical - 1 == y)
			{
				y = pAscanData->axis.vertical;
			}
		}
	}
	else
	{
		y = ((pAscanData->axis.vertical - 1) * dispData) / 255;
		if(pAscanData->axis.vertical - 1 == y)
		{
			y = pAscanData->axis.vertical;
		}
		if(pAscanData->wave.rfMode)
		{
			if(0 == y)
			{
				y = -1;
			}
		}
	}

	return y;
}

static inline int ascanDrawCrossGate(const ascanStruct* pAscanData ,int i ,int j)
{
	int CrossGate = FALSE;
	if((i >= pAscanData->gateDataA.start)
	&&(i <= pAscanData->gateDataA.end)//闸门内
	)
	{
		if(pAscanData->wave.rfMode)
		{
			if(pAscanData->axis.direction == DIRECTION_HORIZONTAL)
			{
				if(j < pAscanData->gateDataA.height)
					CrossGate = TRUE;
				else if(j > pAscanData->axis.vertical - pAscanData->gateDataA.height)
					CrossGate = TRUE;
			}
			else
			{
				if(j > pAscanData->gateDataA.height)
					CrossGate = TRUE;
				else if(j < pAscanData->axis.vertical - pAscanData->gateDataA.height)
					CrossGate = TRUE;
			}
		}
		else
		{
			if(pAscanData->axis.direction == DIRECTION_HORIZONTAL)
			{
				if(j < pAscanData->gateDataA.height)
					CrossGate = TRUE;
			}
			else
			{
				if(j > pAscanData->gateDataA.height)
					CrossGate = TRUE;
			}
		}
	}
	return CrossGate;
}

typedef enum proiority_enum
{
	PRIORITY_CLEAR,
	PRIORITY_ASCAN,
	PRIORITY_ENVELOPE,
	PRIORITY_PEAK_REF,
	PRIORITY_PEAK,
	PRIORITY_REF,
	PRIORITY_MAX
}
	PRIORITY_ENUM;

static inline int outOfRange(ascanStruct* pAscanData ,PRIORITY_ENUM priority ,int j)
{
	if(priority < PRIORITY_PEAK_REF)
	{
        if(REFMODE_PEAK == pAscanData->ref.mode)
        {
        	if((j >= pAscanData->peak.currentLine.lo)
        	&& (j <= pAscanData->peak.currentLine.hi))
        	{
        		return PRIORITY_PEAK;//peak点
        	}
        }
        else if(REFMODE_REF == pAscanData->ref.mode)//ref模式
        {
        	if((j >= pAscanData->ref.currentLine.lo)
        	&& (j <= pAscanData->ref.currentLine.hi))
        	{
        		return PRIORITY_REF;//ref点
        	}
        }
	}
	if(priority < PRIORITY_ENVELOPE)
	{
        //包络
        if(pAscanData->envelope.mode)
        {
        	if((j >= pAscanData->envelope.currentLine[0].lo)
        	&& (j <= pAscanData->envelope.currentLine[0].hi))
        	{
        		return PRIORITY_ENVELOPE;//上包络点
        	}
        	if((j >= pAscanData->envelope.currentLine[1].lo)
        	&& (j <= pAscanData->envelope.currentLine[1].hi))
        	{
        		return PRIORITY_ENVELOPE;//下包络点
        	}
        }
	}
	if(priority < PRIORITY_ASCAN)
	{
        //ascan
		if((j >= pAscanData->wave.currentLine.lo)
		&& (j <= pAscanData->wave.currentLine.hi))
		{
			return PRIORITY_ASCAN;//ascan点
		}
	}
	return PRIORITY_CLEAR;
}

static inline int getRangeColor(ascanStruct* pAscanData ,int i ,int j)
{
	int color;
	switch(outOfRange(pAscanData ,PRIORITY_CLEAR ,j))
	{
	case PRIORITY_ASCAN:
		if(ascanDrawCrossGate(pAscanData ,i ,j))//闸门内，超过闸门
			color = pAscanData->wave.colorCrossAGate;
		else
			color = pAscanData->wave.color;
		break;

	case PRIORITY_ENVELOPE:
		color = COLOR_ENVELOPE;
		break;

	case PRIORITY_PEAK:
		color = COLOR_PEAK;
		break;

	case PRIORITY_REF:
		color = COLOR_REF;
		break;

	case PRIORITY_CLEAR:
	default:
		color = COLOR_BACKGROUD;
		break;
	}
	return color;
}

static inline singleLineStruct ascanGetLine(ascanStruct* pAscanData ,unsigned char* dispData ,int i)
{
	singleLineStruct line;

	unsigned char data1 ,data2;
	signed int tmp;

	if(0 == i)//第一个点
	{
		data1 = dispData[i];

		tmp = dispData[i + 1] - dispData[i];
		tmp = tmp >= 0 ?tmp + 1 :tmp - 1;
		data2 = dispData[i] + tmp / 2 ;
	}
	else if(i >= pAscanData->axis.horizontal - 1)//最后一个点
	{
		data1 = dispData[i];

		tmp = dispData[i] - dispData[i - 1];
		data2 = dispData[i] - tmp / 2;
	}
	else
	{
		tmp = dispData[i] - dispData[i - 1];
		data1 = dispData[i] - tmp / 2;

		tmp = dispData[i + 1] - dispData[i];
		tmp = tmp >= 0 ?tmp + 1 :tmp - 1;
		data2 = dispData[i] + tmp / 2;

		if(data1 < data2)
		{
			data1 = data1 < dispData[i] ? data1 :dispData[i];
			data2 = data2 > dispData[i] ? data2 :dispData[i];
		}
		else
		{
			data2 = data2 < dispData[i] ? data2 :dispData[i];
			data1 = data1 > dispData[i] ? data1 :dispData[i];
		}
	}

	int tmp1,tmp2;
	tmp1 = ascanGetPointVerticalAxis(pAscanData ,data1);
	tmp2 = ascanGetPointVerticalAxis(pAscanData ,data2);

	if(tmp1 < tmp2)
	{
		line.lo = tmp1;
		line.hi = tmp2;
	}
	else
	{
		line.lo = tmp2;
		line.hi = tmp1;
	}
	return line;
}

static inline void ascanDrawPeak(unsigned short* pFrameBuffer_ ,ascanStruct* pAscanData ,int i)
{
	int j;
	int _nPointer;
	int color = COLOR_PEAK;
	//获取轨迹
	pAscanData->peak.currentLine = ascanGetLine(pAscanData ,pAscanData->peak.dispData ,i);

	if(pAscanData->peak.line[i].hi)
	{
		for(j = pAscanData->peak.currentLine.lo ; j < pAscanData->peak.line[i].lo ;j++)
		{
			if(PRIORITY_CLEAR == outOfRange(pAscanData ,PRIORITY_PEAK_REF ,j))
			{
				_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
				pFrameBuffer_[_nPointer] = color;
			}
		}

		for(j = pAscanData->peak.line[i].hi + 1; j <= pAscanData->peak.currentLine.hi ;j++)
		{
			if(PRIORITY_CLEAR == outOfRange(pAscanData ,PRIORITY_PEAK_REF ,j))
			{
				_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
				pFrameBuffer_[_nPointer] = color;
			}
		}
	}
	else
	{
		for(j = pAscanData->peak.currentLine.lo; j <= pAscanData->peak.currentLine.hi ;j++)
		{
			if(PRIORITY_CLEAR == outOfRange(pAscanData ,PRIORITY_PEAK_REF ,j))
			{
				_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
				pFrameBuffer_[_nPointer] = color;
			}
		}
	}

}

static inline void ascanDrawRef(unsigned short* pFrameBuffer_ ,ascanStruct* pAscanData ,int i)
{
	int j;
	int _nPointer;

	int color = COLOR_REF;
	//获取轨迹
	if((i - pAscanData->ref.offset >= 0)
	&& (i - pAscanData->ref.offset + 1 < pAscanData->wave.dispDataWidth)
	&& (i <= pAscanData->ref.width + pAscanData->ref.offset)
	)
	{
		pAscanData->ref.currentLine = ascanGetLine(pAscanData ,pAscanData->ref.dispData ,i - pAscanData->ref.offset);
	}
	else
	{
		pAscanData->ref.currentLine.lo = 0;
		pAscanData->ref.currentLine.hi = 0;
	}
	if(pAscanData->ref.line[i].hi)
	{
		for(j = pAscanData->ref.currentLine.lo ; j < pAscanData->ref.line[i].lo ;j++)
		{
			if(PRIORITY_CLEAR == outOfRange(pAscanData ,PRIORITY_PEAK_REF ,j))
			{
				_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
				pFrameBuffer_[_nPointer] = color;
			}
		}

		for(j = pAscanData->ref.line[i].hi + 1; j <= pAscanData->ref.currentLine.hi ;j++)
		{
			if(PRIORITY_CLEAR == outOfRange(pAscanData ,PRIORITY_PEAK_REF ,j))
			{
				_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
				pFrameBuffer_[_nPointer] = color;
			}
		}
	}
	else
	{
		for(j = pAscanData->ref.currentLine.lo; j <= pAscanData->ref.currentLine.hi ;j++)
		{
			if(PRIORITY_CLEAR == outOfRange(pAscanData ,PRIORITY_PEAK_REF ,j))
			{
				_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
				pFrameBuffer_[_nPointer] = color;
			}
		}
	}
}

static inline void ascanDrawEnvelope(unsigned short* pFrameBuffer_ ,ascanStruct* pAscanData ,int i)
{
	int j;
	int _nPointer;
	int color = COLOR_ENVELOPE;
	//画上包络
	pAscanData->envelope.currentLine[0] = ascanGetLine(pAscanData ,pAscanData->envelope.dispData[0] ,i);

	if(pAscanData->envelope.line[0][i].hi)
	{
	//
		for(j = pAscanData->envelope.currentLine[0].lo ; j < pAscanData->envelope.line[0][i].lo ; j++)
		{
			if(PRIORITY_CLEAR == outOfRange(pAscanData ,PRIORITY_ENVELOPE ,j))
			{
				_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
				pFrameBuffer_[_nPointer] = color;
			}
		}

		for(j = pAscanData->envelope.line[0][i].hi + 1; j <= pAscanData->envelope.currentLine[0].hi ; j++)
		{
			if(PRIORITY_CLEAR == outOfRange(pAscanData ,PRIORITY_ENVELOPE ,j))
			{
				_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
				pFrameBuffer_[_nPointer] = color;
			}
		}
	}
	else
	{
		for(j = pAscanData->envelope.currentLine[0].lo; j <= pAscanData->envelope.currentLine[0].hi ; j++)
		{
			if(PRIORITY_CLEAR == outOfRange(pAscanData ,PRIORITY_ENVELOPE ,j))
			{
				_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
				pFrameBuffer_[_nPointer] = color;
			}
		}
	}

    //画下包络
	pAscanData->envelope.currentLine[1] = ascanGetLine(pAscanData ,pAscanData->envelope.dispData[1] ,i);

	if((pAscanData->envelope.currentLine[1].lo < pAscanData->envelope.currentLine[0].lo)
	||(pAscanData->envelope.currentLine[1].hi > pAscanData->envelope.currentLine[0].hi)	)	//不包含在上包络内
	{

		if(pAscanData->envelope.line[1][i].hi)
		{
			for(j = pAscanData->envelope.currentLine[1].lo ; j < pAscanData->envelope.line[1][i].lo ; j++)
			{
				if(PRIORITY_CLEAR == outOfRange(pAscanData ,PRIORITY_ENVELOPE ,j))
				{
					_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
					pFrameBuffer_[_nPointer] = color;
				}
			}

			for(j = pAscanData->envelope.line[1][i].hi + 1; j <= pAscanData->envelope.currentLine[1].hi ; j++)
			{
				if(PRIORITY_CLEAR == outOfRange(pAscanData ,PRIORITY_ENVELOPE ,j))
				{
					_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
					pFrameBuffer_[_nPointer] = color;
				}
			}
		}
		else
		{
			for(j = pAscanData->envelope.currentLine[1].lo; j <= pAscanData->envelope.currentLine[1].hi ; j++)
			{
				if(PRIORITY_CLEAR == outOfRange(pAscanData ,PRIORITY_ENVELOPE ,j))
				{
					_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
					pFrameBuffer_[_nPointer] = color;
				}
			}
		}
	}
}

static inline void ascanDrawAscan(unsigned short* pFrameBuffer_ ,ascanStruct* pAscanData ,int i)
{
	int j;
	int _nPointer;
	//画图
	//当前列的ascan着色点范围
	if((i < pAscanData->wave.start) || (i > pAscanData->wave.end))
	{
		pAscanData->wave.currentLine.lo = pAscanData->axis.vertical;//不画出来
		pAscanData->wave.currentLine.hi = pAscanData->axis.vertical;
	}
	else
	{
		pAscanData->wave.currentLine = ascanGetLine(pAscanData ,pAscanData->wave.dispData ,i - pAscanData->wave.start);
		//if fill mode ,line is diffrent
		switch(pAscanData->wave.fillMode)
		{
		case FILLMODE_HOLLOW://hollow
		case FILLMODE_HOLLOW2COLOR://hollow 2 color
			break;
		case FILLMODE_FILL://fill
		case FILLMODE_FILL2COLOR://fill 2 color
			if(pAscanData->wave.currentLine.lo > pAscanData->wave.fillOffset)
				pAscanData->wave.currentLine.lo = pAscanData->wave.fillOffset;
			if(pAscanData->wave.currentLine.hi < pAscanData->wave.fillOffset)
				pAscanData->wave.currentLine.hi = pAscanData->wave.fillOffset;
			break;
		default:
			break;
		}
	}

	if(pAscanData->wave.line[i].hi)
	{
		for(j = pAscanData->wave.currentLine.lo ; j < pAscanData->wave.line[i].lo ; j++)
		{
			if(PRIORITY_CLEAR == outOfRange(pAscanData ,PRIORITY_ASCAN ,j))
			{
				_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
				if(ascanDrawCrossGate(pAscanData ,i ,j))//闸门内，超过闸门
					pFrameBuffer_[_nPointer] = pAscanData->wave.colorCrossAGate;
				else
					pFrameBuffer_[_nPointer] = pAscanData->wave.color;
			}
		}

		for(j = pAscanData->wave.line[i].hi + 1; j <= pAscanData->wave.currentLine.hi ; j++)
		{
			if(PRIORITY_CLEAR == outOfRange(pAscanData ,PRIORITY_ASCAN ,j))
			{
				_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
				if(ascanDrawCrossGate(pAscanData ,i ,j))//闸门内，超过闸门
				{
					pFrameBuffer_[_nPointer] = pAscanData->wave.colorCrossAGate;
				}
				else
					pFrameBuffer_[_nPointer] = pAscanData->wave.color;
			}
		}
	}
	else
	{
		for(j = pAscanData->wave.currentLine.lo; j <= pAscanData->wave.currentLine.hi ; j++)
		{
			if(PRIORITY_CLEAR == outOfRange(pAscanData ,PRIORITY_ASCAN ,j))
			{
				_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
				if(ascanDrawCrossGate(pAscanData ,i ,j))//闸门内，超过闸门
				{
					pFrameBuffer_[_nPointer] = pAscanData->wave.colorCrossAGate;
				}
				else
					pFrameBuffer_[_nPointer] = pAscanData->wave.color;
			}
		}
	}
}

static inline int verifyLine(singleLineStruct* pLine ,int* pNumOfLines ,singleLineStruct* pNewLine)
{
	int i;
	int numOfNewLines = 0;
	singleLineStruct newLine = pNewLine[0];
	if(pNewLine[0].lo < pNewLine[0].hi)
	{
		if(0 == *pNumOfLines)
		{
			pLine[0] = newLine;
			*pNumOfLines = 1;
			numOfNewLines = 1;
		}
		else
		{
			int numOfLo = 0;
			int numOfHi = 0;
			//计算插入线段两端点的位置，（左边有几个端点）
			for(i = 0 ;i < *pNumOfLines ;i++)
			{
				//pline第一个点lo点
				if(newLine.lo >= pLine[i].lo)
				{
					numOfLo ++;
					numOfHi ++;
					//第二个点hi点
					if(newLine.lo > pLine[i].hi)
					{
						numOfLo ++;
						numOfHi ++;
					//	continue;
					}
					else if(newLine.hi > pLine[i].hi)
					{
						numOfHi ++;
					//	continue;
					}
					else
					{
						break;
					}
				}
				//lo点已经计算完了，hi点继续
				else if(newLine.hi >= pLine[i].lo)
				{
					numOfHi ++;
					if(newLine.hi > pLine[i].hi)
					{
						numOfHi ++;
						//continue;
					}
					else
					{
						break;
					}
				}
			}
			int num = numOfHi - numOfLo;
			int loSite = numOfLo % 2;
			int hiSite = numOfHi % 2;
			int loPos = numOfLo / 2;
			int hiPos = (numOfHi + 1) / 2;//<= *pNumOfLines
			int tmpNum = *pNumOfLines - hiPos + 1;

			*pNumOfLines += 1 + loPos - hiPos;
			numOfNewLines =  num  + 1 + loPos - hiPos;
			//不需要增加线段的情况
			if(1 == hiPos - loPos)
			{
				if(1 == num)
				{
					if(0 == loSite)
					{
						pNewLine[0].hi = pLine[loPos].lo;
						pLine[loPos].lo = newLine.lo;
					}
					else
					{
						pNewLine[0].lo = pLine[loPos].hi;
						pLine[loPos].hi = newLine.hi;
					}
					numOfNewLines = 1;
				}
				else if(2 == num)
				{
					pNewLine[0].hi = pLine[loPos].lo;

					pNewLine[1].lo = pLine[loPos].hi;
					pNewLine[1].hi = newLine.hi;

					pLine[loPos] = newLine;

					numOfNewLines = 2;
				}
				else//0 == num
				{
					//do nothing
				}
			}
			else
			{//线段有增减，用临时线段来倒
				singleLineStruct tmpLines[tmpNum];
				tmpLines[0].lo = loSite ?pLine[loPos].lo :newLine.lo;
				tmpLines[0].hi = hiSite ?pLine[hiPos - 1].hi :newLine.hi;


				if(0 == loSite)
				{
					pNewLine[0].lo = newLine.lo;
					pNewLine[0].hi = pLine[loPos].lo;

					for(i = 1 ;i < numOfNewLines - 1 ;i ++)
					{
						pNewLine[i].lo = pLine[loPos + i - 1].hi;
						pNewLine[i].hi = pLine[loPos + i].lo;
					}
				}
				else
				{
					for(i = 0 ;i < numOfNewLines - 1 ;i ++)
					{
						pNewLine[i].lo = pLine[loPos + i].hi;
						pNewLine[i].hi = pLine[loPos + i + 1].lo;
					}
				}

				if(0 == hiSite)
				{
					pNewLine[numOfNewLines - 1].hi = pLine[hiPos - 1].hi;
					pNewLine[numOfNewLines - 1].hi = newLine.hi;
				}
				else
				{
					pNewLine[numOfNewLines - 1].hi = pLine[hiPos - 2].hi;
					pNewLine[numOfNewLines - 1].hi = pLine[hiPos - 1].lo;
				}

				memcpy(tmpLines + 1 ,pLine + hiPos ,(tmpNum - 1) * sizeof(singleLineStruct));
				memcpy(pLine + loPos ,tmpLines ,tmpNum * sizeof(singleLineStruct));
			}

		}
	}
	return numOfNewLines;
}
#if 0
static inline void ascanDrawClear(unsigned short* pFrameBuffer_ ,ascanStruct* pAscanData ,int i)
{
	int j;
	int _nPointer;
	//擦Ascan
	for(j = pAscanData->wave.line[i].lo ; j < pAscanData->wave.currentLine.lo ;j++)
	{
		_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
		pFrameBuffer_[ _nPointer ]    = getRangeColor(pAscanData ,i ,j);
	}

    if(0 == pAscanData->wave.line[i].hi)
    {
    	pAscanData->wave.line[i].hi = pAscanData->axis.vertical;//用于清屏
    }
    for(j = pAscanData->wave.currentLine.hi + 1; j <= pAscanData->wave.line[i].hi ;j++)
    {
		_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
		pFrameBuffer_[ _nPointer ]    = getRangeColor(pAscanData ,i ,j);
    }

	pAscanData->wave.line[i].lo  = pAscanData->wave.currentLine.lo;
	pAscanData->wave.line[i].hi    = pAscanData->wave.currentLine.hi;

	//擦包络
    if(pAscanData->envelope.mode)
    {
		for(j = pAscanData->envelope.line[0][i].lo ; j < pAscanData->envelope.currentLine[0].lo ;j++)
		{
			_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
			pFrameBuffer_[ _nPointer ]    = getRangeColor(pAscanData ,i ,j);
		}

	    if(0 == pAscanData->envelope.line[0][i].hi)
	    {
	    	pAscanData->envelope.line[0][i].hi = pAscanData->axis.vertical;//用于清屏
	    }
		for(j = pAscanData->envelope.currentLine[0].hi + 1; j <= pAscanData->envelope.line[0][i].hi ;j++)
		{
			_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
			pFrameBuffer_[ _nPointer ]    = getRangeColor(pAscanData ,i ,j);
		}

		pAscanData->envelope.line[0][i].lo  = pAscanData->envelope.currentLine[0].lo;
		pAscanData->envelope.line[0][i].hi    = pAscanData->envelope.currentLine[0].hi;

		for(j = pAscanData->envelope.line[1][i].lo ;j < pAscanData->envelope.currentLine[1].lo ;j++)
		{
			_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
			pFrameBuffer_[ _nPointer ]    = getRangeColor(pAscanData ,i ,j);
		}

	    if(0 == pAscanData->envelope.line[1][i].hi)
	    {
	    	pAscanData->envelope.line[1][i].hi = pAscanData->axis.vertical;//用于清屏
	    }
		for(j = pAscanData->envelope.currentLine[1].hi + 1; j <= pAscanData->envelope.line[1][i].hi ;j++)
		{
			_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
			pFrameBuffer_[ _nPointer ]    = getRangeColor(pAscanData ,i ,j);
		}

		pAscanData->envelope.line[1][i].lo  = pAscanData->envelope.currentLine[1].lo;
		pAscanData->envelope.line[1][i].hi    = pAscanData->envelope.currentLine[1].hi;
    }

    if(REFMODE_PEAK == pAscanData->ref.mode)
    {//擦peak
        for(j = pAscanData->peak.line[i].lo ;j < pAscanData->peak.currentLine.lo ;j++)
        {
			_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
			pFrameBuffer_[ _nPointer ]    = getRangeColor(pAscanData ,i ,j);
        }

        if(0 == pAscanData->peak.line[i].hi)
        {
        	pAscanData->peak.line[i].hi = pAscanData->axis.vertical;//用于清屏
        }
        for(j = pAscanData->peak.currentLine.hi + 1; j <= pAscanData->peak.line[i].hi ;j++)
        {
			_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
			pFrameBuffer_[ _nPointer ]    = getRangeColor(pAscanData ,i ,j);
        }

    	pAscanData->peak.line[i].lo  = pAscanData->peak.currentLine.lo;
    	pAscanData->peak.line[i].hi    = pAscanData->peak.currentLine.hi;
    }
    else if(REFMODE_REF == pAscanData->ref.mode)//ref模式
    {//擦ref
        for(j = pAscanData->ref.line[i].lo ;j < pAscanData->ref.currentLine.lo ;j++)
        {
			_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
			pFrameBuffer_[ _nPointer ]    = getRangeColor(pAscanData ,i ,j);
        }

        if(0 == pAscanData->ref.line[i].hi)
        {
        	pAscanData->ref.line[i].hi = pAscanData->axis.vertical;//用于清屏
        }
        for(j = pAscanData->ref.currentLine.hi + 1 ;j <= pAscanData->ref.line[i].hi ;j++)
        {
			_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
			pFrameBuffer_[ _nPointer ]    = getRangeColor(pAscanData ,i ,j);
        }

    	pAscanData->ref.line[i].lo  = pAscanData->ref.currentLine.lo;
    	pAscanData->ref.line[i].hi    = pAscanData->ref.currentLine.hi;
    }
}
#else
static inline void ascanDrawClear(unsigned short* pFrameBuffer_ ,ascanStruct* pAscanData ,int i)
{
	int j;
	int _nPointer;
    int k;
	int numOfClearLine = 0;
	int numOfNewLine = 0;
	singleLineStruct clearLine[10];
	singleLineStruct line[10];
	//Ascan
	line[0].lo = pAscanData->wave.line[i].lo;
	line[0].hi = pAscanData->wave.currentLine.lo;
	numOfNewLine = verifyLine(clearLine ,&numOfClearLine ,line);
/*    	if(numOfNewLine)
	{
		for(k = 0 ;k < numOfNewLine ;k++)
		{
			for(j = line[k].lo ; j < line[k].hi ;j++)
			{
				_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
				pFrameBuffer_[ _nPointer ] = getRangeColor(pAscanData ,i ,j);
			}
		}
	}//*/

    if(0 == pAscanData->wave.line[i].hi)
    {
    	pAscanData->wave.line[i].hi = pAscanData->axis.vertical;//用于清屏
    }
	line[0].lo = pAscanData->wave.currentLine.hi + 1;
	line[0].hi = pAscanData->wave.line[i].hi + 1;
	numOfNewLine = verifyLine(clearLine ,&numOfClearLine ,line);
/*    	if(numOfNewLine)
	{
		for(k = 0 ;k < numOfNewLine ;k++)
		{
			for(j = line[k].lo ; j < line[k].hi ;j++)
			{
				_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
				pFrameBuffer_[ _nPointer ] = getRangeColor(pAscanData ,i ,j);
			}
		}
	}//*/

	pAscanData->wave.line[i].lo  = pAscanData->wave.currentLine.lo;
	pAscanData->wave.line[i].hi  = pAscanData->wave.currentLine.hi;

	//擦包络
    if(pAscanData->envelope.mode)
    {
    	line[0].lo = pAscanData->envelope.line[0][i].lo;
    	line[0].hi = pAscanData->envelope.currentLine[0].lo;
    	numOfNewLine = verifyLine(clearLine ,&numOfClearLine ,line);
/*    	if(numOfNewLine)
		{
			for(k = 0 ;k < numOfNewLine ;k++)
			{
				for(j = line[k].lo ; j < line[k].hi ;j++)
				{
					_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
					pFrameBuffer_[ _nPointer ] = getRangeColor(pAscanData ,i ,j);
				}
			}
		}//*/

        if(0 == pAscanData->envelope.line[0][i].hi)
        {
        	pAscanData->envelope.line[0][i].hi = pAscanData->axis.vertical;//用于清屏
        }
    	line[0].lo = pAscanData->envelope.currentLine[0].hi + 1;
    	line[0].hi = pAscanData->envelope.line[0][i].hi + 1;
    	numOfNewLine = verifyLine(clearLine ,&numOfClearLine ,line);
/*    	if(numOfNewLine)
		{
			for(k = 0 ;k < numOfNewLine ;k++)
			{
				for(j = line[k].lo ; j < line[k].hi ;j++)
				{
					_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
					pFrameBuffer_[ _nPointer ] = getRangeColor(pAscanData ,i ,j);
				}
			}
		}//*/

		pAscanData->envelope.line[0][i].lo  = pAscanData->envelope.currentLine[0].lo;
		pAscanData->envelope.line[0][i].hi  = pAscanData->envelope.currentLine[0].hi;

    	line[0].lo = pAscanData->envelope.line[1][i].lo;
    	line[0].hi = pAscanData->envelope.currentLine[1].lo;
    	numOfNewLine = verifyLine(clearLine ,&numOfClearLine ,line);
/*    	if(numOfNewLine)
		{
			for(k = 0 ;k < numOfNewLine ;k++)
			{
				for(j = line[k].lo ; j < line[k].hi ;j++)
				{
					_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
					pFrameBuffer_[ _nPointer ] = getRangeColor(pAscanData ,i ,j);
				}
			}
		}//*/

        if(0 == pAscanData->envelope.line[1][i].hi)
        {
        	pAscanData->envelope.line[1][i].hi = pAscanData->axis.vertical;//用于清屏
        }
    	line[0].lo = pAscanData->envelope.currentLine[1].hi + 1;
    	line[0].hi = pAscanData->envelope.line[1][i].hi + 1;
    	numOfNewLine = verifyLine(clearLine ,&numOfClearLine ,line);
/*    	if(numOfNewLine)
		{
			for(k = 0 ;k < numOfNewLine ;k++)
			{
				for(j = line[k].lo ; j < line[k].hi ;j++)
				{
					_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
					pFrameBuffer_[ _nPointer ] = getRangeColor(pAscanData ,i ,j);
				}
			}
		}//*/

		pAscanData->envelope.line[1][i].lo  = pAscanData->envelope.currentLine[1].lo;
		pAscanData->envelope.line[1][i].hi  = pAscanData->envelope.currentLine[1].hi;

    }

    if(REFMODE_PEAK == pAscanData->ref.mode)
    {//擦peak
    	line[0].lo = pAscanData->peak.line[i].lo;
    	line[0].hi = pAscanData->peak.currentLine.lo;
    	numOfNewLine = verifyLine(clearLine ,&numOfClearLine ,line);
/*    	if(numOfNewLine)
		{
			for(k = 0 ;k < numOfNewLine ;k++)
			{
				for(j = line[k].lo ; j < line[k].hi ;j++)
				{
					_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
					pFrameBuffer_[ _nPointer ] = getRangeColor(pAscanData ,i ,j);
				}
			}
		}//*/

        if(0 == pAscanData->peak.line[i].hi)
        {
        	pAscanData->peak.line[i].hi = pAscanData->axis.vertical;//用于清屏
        }
    	line[0].lo = pAscanData->peak.currentLine.hi + 1;
    	line[0].hi = pAscanData->peak.line[i].hi + 1;
    	numOfNewLine = verifyLine(clearLine ,&numOfClearLine ,line);
/*    	if(numOfNewLine)
		{
			for(k = 0 ;k < numOfNewLine ;k++)
			{
				for(j = line[k].lo ; j < line[k].hi ;j++)
				{
					_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
					pFrameBuffer_[ _nPointer ] = getRangeColor(pAscanData ,i ,j);
				}
			}
		}//*/

    	pAscanData->peak.line[i].lo  = pAscanData->peak.currentLine.lo;
    	pAscanData->peak.line[i].hi  = pAscanData->peak.currentLine.hi;
    }
    else if(REFMODE_REF == pAscanData->ref.mode)//ref模式
    {//擦ref
    	line[0].lo = pAscanData->ref.line[i].lo;
    	line[0].hi = pAscanData->ref.currentLine.lo;
    	numOfNewLine = verifyLine(clearLine ,&numOfClearLine ,line);
/*    	if(numOfNewLine)
		{
			for(k = 0 ;k < numOfNewLine ;k++)
			{
				for(j = line[k].lo ; j < line[k].hi ;j++)
				{
					_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
					pFrameBuffer_[ _nPointer ] = getRangeColor(pAscanData ,i ,j);
				}
			}
		}//*/

        if(0 == pAscanData->ref.line[i].hi)
        {
        	pAscanData->ref.line[i].hi = pAscanData->axis.vertical;//用于清屏
        }
    	line[0].lo = pAscanData->ref.currentLine.hi + 1;
    	line[0].hi = pAscanData->ref.line[i].hi + 1;
    	numOfNewLine = verifyLine(clearLine ,&numOfClearLine ,line);
/*    	if(numOfNewLine)
    	{
    	    for(k = 0 ;k < numOfNewLine ;k++)
    	    {
    	    	for(j = line[k].lo ; j < line[k].hi ;j++)
    	    	{
    	    		_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
    	    		pFrameBuffer_[ _nPointer ] = getRangeColor(pAscanData ,i ,j);
    	    	}
    	    }
    	}//*/

    	pAscanData->ref.line[i].lo  = pAscanData->ref.currentLine.lo;
    	pAscanData->ref.line[i].hi  = pAscanData->ref.currentLine.hi;
    }
    for(k = 0 ;k < numOfClearLine ;k++)
    {
    	for(j = clearLine[k].lo ; j < clearLine[k].hi ;j++)
    	{
    		_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
    		pFrameBuffer_[ _nPointer ] = getRangeColor(pAscanData ,i ,j);
    	}
    }//*/
}
#endif
singleLineStruct oldLines[50] ,newLines[50];
int numOfOldLine = 0;

static inline void _ascanDrawPeak(unsigned short* pFrameBuffer_ ,ascanStruct* pAscanData ,int i)
{
	int j;
	int _nPointer;
	int k;
	int numOfNewLine;
	int color = COLOR_PEAK;
	//获取轨迹
	pAscanData->peak.currentLine = ascanGetLine(pAscanData ,pAscanData->peak.dispData ,i);

	if(pAscanData->peak.line[i].hi)
	{
		//Ascan
		newLines[0].lo = pAscanData->peak.currentLine.lo;
		newLines[0].hi = pAscanData->peak.line[i].lo;
		numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
		for(k = 0 ;k < numOfNewLine ;k++)
		{
			for(j = newLines[k].lo ; j < newLines[k].hi ;j++)
			{
				_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
				pFrameBuffer_[ _nPointer ] = color;
			}
		}//*/

		newLines[0].lo = pAscanData->peak.line[i].hi + 1;
		newLines[0].hi = pAscanData->peak.currentLine.hi + 1;
		numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
		for(k = 0 ;k < numOfNewLine ;k++)
		{
			for(j = newLines[k].lo ; j < newLines[k].hi ;j++)
			{
				_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
				pFrameBuffer_[ _nPointer ] = color;
			}
		}//*/

		newLines[0].lo = pAscanData->peak.currentLine.lo;
		newLines[0].hi = pAscanData->peak.currentLine.hi + 1;
		numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
	}
	else
	{
		newLines[0].lo = pAscanData->peak.currentLine.lo;
		newLines[0].hi = pAscanData->peak.currentLine.hi + 1;
		numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
		for(k = 0 ;k < numOfNewLine ;k++)
		{
			for(j = newLines[k].lo ; j < newLines[k].hi ;j++)
			{
				_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
				pFrameBuffer_[ _nPointer ] = color;
			}
		}//*/
	}
}

static inline void _ascanDrawRef(unsigned short* pFrameBuffer_ ,ascanStruct* pAscanData ,int i)
{
	int j;
	int _nPointer;
	int color = COLOR_REF;
	int k;
	int numOfNewLine;
	//获取轨迹
	if((i - pAscanData->ref.offset >= 0)
	&& (i - pAscanData->ref.offset + 1 < pAscanData->wave.dispDataWidth)
	&& (i <= pAscanData->ref.width + pAscanData->ref.offset)
	)
	{
		pAscanData->ref.currentLine = ascanGetLine(pAscanData ,pAscanData->ref.dispData ,i - pAscanData->ref.offset);
	}
	else
	{
		pAscanData->ref.currentLine.lo = 0;
		pAscanData->ref.currentLine.hi = 0;
	}

	if(pAscanData->ref.line[i].hi)
	{
		//Ascan
		newLines[0].lo = pAscanData->ref.currentLine.lo;
		newLines[0].hi = pAscanData->ref.line[i].lo;
		numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
		for(k = 0 ;k < numOfNewLine ;k++)
		{
			for(j = newLines[k].lo ; j < newLines[k].hi ;j++)
			{
				_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
				pFrameBuffer_[ _nPointer ] = color;
			}
		}//*/

		newLines[0].lo = pAscanData->ref.line[i].hi + 1;
		newLines[0].hi = pAscanData->ref.currentLine.hi + 1;
		numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
		for(k = 0 ;k < numOfNewLine ;k++)
		{
			for(j = newLines[k].lo ; j < newLines[k].hi ;j++)
			{
				_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
				pFrameBuffer_[ _nPointer ] = color;
			}
		}//*/

		newLines[0].lo = pAscanData->ref.currentLine.lo;
		newLines[0].hi = pAscanData->ref.currentLine.hi + 1;
		numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
	}
	else
	{
		newLines[0].lo = pAscanData->ref.currentLine.lo;
		newLines[0].hi = pAscanData->ref.currentLine.hi + 1;
		numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
		for(k = 0 ;k < numOfNewLine ;k++)
		{
			for(j = newLines[k].lo ; j < newLines[k].hi ;j++)
			{
				_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
				pFrameBuffer_[ _nPointer ] = color;
			}
		}//*/
	}
}

static inline void _ascanDrawEnvelope(unsigned short* pFrameBuffer_ ,ascanStruct* pAscanData ,int i)
{
	int j;
	int _nPointer;
	int color = COLOR_ENVELOPE;
	int k;
	int numOfNewLine;
	//画上包络
	pAscanData->envelope.currentLine[0] = ascanGetLine(pAscanData ,pAscanData->envelope.dispData[0] ,i);

	if(pAscanData->envelope.line[0][i].hi)
	{
	//
		newLines[0].lo = pAscanData->envelope.currentLine[0].lo;
		newLines[0].hi = pAscanData->envelope.line[0][i].lo;
		numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
		for(k = 0 ;k < numOfNewLine ;k++)
		{
			for(j = newLines[k].lo ; j < newLines[k].hi ;j++)
			{
				_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
				pFrameBuffer_[ _nPointer ] = color;
			}
		}//*/

		newLines[0].lo = pAscanData->envelope.line[0][i].hi + 1;
		newLines[0].hi = pAscanData->envelope.currentLine[0].hi + 1;
		numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
		for(k = 0 ;k < numOfNewLine ;k++)
		{
			for(j = newLines[k].lo ; j < newLines[k].hi ;j++)
			{
				_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
				pFrameBuffer_[ _nPointer ] = color;
			}
		}//*/

		newLines[0].lo = pAscanData->envelope.currentLine[0].lo;
		newLines[0].hi = pAscanData->envelope.currentLine[0].hi + 1;
		numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
	}
	else
	{
		newLines[0].lo = pAscanData->envelope.currentLine[0].lo;
		newLines[0].hi = pAscanData->envelope.currentLine[0].hi + 1;
		numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
		for(k = 0 ;k < numOfNewLine ;k++)
		{
			for(j = newLines[k].lo ; j < newLines[k].hi ;j++)
			{
				_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
				pFrameBuffer_[ _nPointer ] = color;
			}
		}//*/
	}

    //画下包络
	pAscanData->envelope.currentLine[1] = ascanGetLine(pAscanData ,pAscanData->envelope.dispData[1] ,i);

	if((pAscanData->envelope.currentLine[1].lo < pAscanData->envelope.currentLine[0].lo)
	||(pAscanData->envelope.currentLine[1].hi > pAscanData->envelope.currentLine[0].hi)	)	//不包含在上包络内
	{

		if(pAscanData->envelope.line[1][i].hi)
		{
			//
				newLines[0].lo = pAscanData->envelope.currentLine[1].lo;
				newLines[0].hi = pAscanData->envelope.line[1][i].lo;
				numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
				for(k = 0 ;k < numOfNewLine ;k++)
				{
					for(j = newLines[k].lo ; j < newLines[k].hi ;j++)
					{
						_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
						pFrameBuffer_[ _nPointer ] = color;
					}
				}//*/

				newLines[0].lo = pAscanData->envelope.line[1][i].hi + 1;
				newLines[0].hi = pAscanData->envelope.currentLine[1].hi + 1;
				numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
				for(k = 0 ;k < numOfNewLine ;k++)
				{
					for(j = newLines[k].lo ; j < newLines[k].hi ;j++)
					{
						_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
						pFrameBuffer_[ _nPointer ] = color;
					}
				}//*/

				newLines[0].lo = pAscanData->envelope.currentLine[1].lo;
				newLines[0].hi = pAscanData->envelope.currentLine[1].hi + 1;
				numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
		}
		else
		{
			newLines[0].lo = pAscanData->envelope.currentLine[1].lo;
			newLines[0].hi = pAscanData->envelope.currentLine[1].hi + 1;
			numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
			for(k = 0 ;k < numOfNewLine ;k++)
			{
				for(j = newLines[k].lo ; j < newLines[k].hi ;j++)
				{
					_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
					pFrameBuffer_[ _nPointer ] = color;
				}
			}//*/
		}
	}
}

static inline void _ascanDrawAscan(unsigned short* pFrameBuffer_ ,ascanStruct* pAscanData ,int i)
{
	int j;
	int _nPointer;
	int k;
	int numOfNewLine;
	//画图
	//当前列的ascan着色点范围
	if((i < pAscanData->wave.start) || (i > pAscanData->wave.end))
	{
		pAscanData->wave.currentLine.lo = 0;
		pAscanData->wave.currentLine.hi = 0;
	}
	else
	{
		pAscanData->wave.currentLine = ascanGetLine(pAscanData ,pAscanData->wave.dispData ,i - pAscanData->wave.start);
	}
	//if fill mode ,line is diffrent
	switch(pAscanData->wave.fillMode)
	{
	case FILLMODE_HOLLOW://hollow
	case FILLMODE_HOLLOW2COLOR://hollow 2 color
		break;
	case FILLMODE_FILL://fill
	case FILLMODE_FILL2COLOR://fill 2 color
		if(pAscanData->wave.currentLine.lo > pAscanData->wave.fillOffset)
			pAscanData->wave.currentLine.lo = pAscanData->wave.fillOffset;
		if(pAscanData->wave.currentLine.hi < pAscanData->wave.fillOffset)
			pAscanData->wave.currentLine.hi = pAscanData->wave.fillOffset;
		break;
	default:
		break;
	}

	if(pAscanData->wave.line[i].hi)
	{
		newLines[0].lo = pAscanData->wave.currentLine.lo;
		newLines[0].hi = pAscanData->wave.line[i].lo;
		numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
		for(k = 0 ;k < numOfNewLine ;k++)
		{
			for(j = newLines[k].lo ; j < newLines[k].hi ;j++)
			{
				_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
				if(ascanDrawCrossGate(pAscanData ,i ,j))//闸门内，超过闸门
					pFrameBuffer_[_nPointer] = pAscanData->wave.colorCrossAGate;
				else
					pFrameBuffer_[_nPointer] = pAscanData->wave.color;
			}
		}//*/

		newLines[0].lo = pAscanData->wave.line[i].hi + 1;
		newLines[0].hi = pAscanData->wave.currentLine.hi + 1;
		numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
		for(k = 0 ;k < numOfNewLine ;k++)
		{
			for(j = newLines[k].lo ; j < newLines[k].hi ;j++)
			{
				_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
				if(ascanDrawCrossGate(pAscanData ,i ,j))//闸门内，超过闸门
					pFrameBuffer_[_nPointer] = pAscanData->wave.colorCrossAGate;
				else
					pFrameBuffer_[_nPointer] = pAscanData->wave.color;
			}
		}//*/

		newLines[0].lo = pAscanData->wave.currentLine.lo;
		newLines[0].hi = pAscanData->wave.currentLine.hi + 1;
		numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
	}
	else
	{
		newLines[0].lo = pAscanData->wave.currentLine.lo;
		newLines[0].hi = pAscanData->wave.currentLine.hi + 1;
		numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
		for(k = 0 ;k < numOfNewLine ;k++)
		{
			for(j = newLines[k].lo ; j < newLines[k].hi ;j++)
			{
				_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
				if(ascanDrawCrossGate(pAscanData ,i ,j))//闸门内，超过闸门
					pFrameBuffer_[_nPointer] = pAscanData->wave.colorCrossAGate;
				else
					pFrameBuffer_[_nPointer] = pAscanData->wave.color;
			}
		}//*/
	}
}

static inline void _ascanDrawClear(unsigned short* pFrameBuffer_ ,ascanStruct* pAscanData ,int i)
{
	int j;
	int _nPointer;
	int color = 0;
    int k;
	int numOfNewLine = 0;
	//Ascan
	newLines[0].lo = pAscanData->wave.line[i].lo;
	newLines[0].hi = pAscanData->wave.currentLine.lo;
	numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
	for(k = 0 ;k < numOfNewLine ;k++)
	{
		for(j = newLines[k].lo ; j < newLines[k].hi ;j++)
		{
			_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
			pFrameBuffer_[ _nPointer ] = color;
		}
	}//*/

    if(0 == pAscanData->wave.line[i].hi)
    {
    	pAscanData->wave.line[i].hi = pAscanData->axis.vertical;//用于清屏
    }
	newLines[0].lo = pAscanData->wave.currentLine.hi + 1;
	newLines[0].hi = pAscanData->wave.line[i].hi + 1;
	numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
	for(k = 0 ;k < numOfNewLine ;k++)
	{
		for(j = newLines[k].lo ; j < newLines[k].hi ;j++)
		{
			_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
			pFrameBuffer_[ _nPointer ] = color;
		}
	}//*/

	pAscanData->wave.line[i].lo  = pAscanData->wave.currentLine.lo;
	pAscanData->wave.line[i].hi  = pAscanData->wave.currentLine.hi;

	//擦包络
    if(pAscanData->envelope.mode)
    {
    	newLines[0].lo = pAscanData->envelope.line[0][i].lo;
    	newLines[0].hi = pAscanData->envelope.currentLine[0].lo;
    	numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
    	for(k = 0 ;k < numOfNewLine ;k++)
    	{
    		for(j = newLines[k].lo ; j < newLines[k].hi ;j++)
    		{
    			_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
    			pFrameBuffer_[ _nPointer ] = color;
    		}
    	}//*/

        if(0 == pAscanData->envelope.line[0][i].hi)
        {
        	pAscanData->envelope.line[0][i].hi = pAscanData->axis.vertical;//用于清屏
        }
    	newLines[0].lo = pAscanData->envelope.currentLine[0].hi + 1;
    	newLines[0].hi = pAscanData->envelope.line[0][i].hi + 1;
    	numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
    	for(k = 0 ;k < numOfNewLine ;k++)
    	{
    		for(j = newLines[k].lo ; j < newLines[k].hi ;j++)
    		{
    			_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
    			pFrameBuffer_[ _nPointer ] = color;
    		}
    	}//*/

		pAscanData->envelope.line[0][i].lo  = pAscanData->envelope.currentLine[0].lo;
		pAscanData->envelope.line[0][i].hi  = pAscanData->envelope.currentLine[0].hi;

    	newLines[0].lo = pAscanData->envelope.line[1][i].lo;
    	newLines[0].hi = pAscanData->envelope.currentLine[1].lo;
    	numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
    	for(k = 0 ;k < numOfNewLine ;k++)
    	{
    		for(j = newLines[k].lo ; j < newLines[k].hi ;j++)
    		{
    			_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
    			pFrameBuffer_[ _nPointer ] = color;
    		}
    	}//*/

        if(0 == pAscanData->envelope.line[1][i].hi)
        {
        	pAscanData->envelope.line[1][i].hi = pAscanData->axis.vertical;//用于清屏
        }
    	newLines[0].lo = pAscanData->envelope.currentLine[1].hi + 1;
    	newLines[0].hi = pAscanData->envelope.line[1][i].hi + 1;
    	numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
    	for(k = 0 ;k < numOfNewLine ;k++)
    	{
    		for(j = newLines[k].lo ; j < newLines[k].hi ;j++)
    		{
    			_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
    			pFrameBuffer_[ _nPointer ] = color;
    		}
    	}//*/

		pAscanData->envelope.line[1][i].lo  = pAscanData->envelope.currentLine[1].lo;
		pAscanData->envelope.line[1][i].hi  = pAscanData->envelope.currentLine[1].hi;

    }

    if(REFMODE_PEAK == pAscanData->ref.mode)
    {//擦peak
    	newLines[0].lo = pAscanData->peak.line[i].lo;
    	newLines[0].hi = pAscanData->peak.currentLine.lo;
    	numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
    	for(k = 0 ;k < numOfNewLine ;k++)
    	{
    		for(j = newLines[k].lo ; j < newLines[k].hi ;j++)
    		{
    			_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
    			pFrameBuffer_[ _nPointer ] = color;
    		}
    	}//*/

        if(0 == pAscanData->peak.line[i].hi)
        {
        	pAscanData->peak.line[i].hi = pAscanData->axis.vertical;//用于清屏
        }
    	newLines[0].lo = pAscanData->peak.currentLine.hi + 1;
    	newLines[0].hi = pAscanData->peak.line[i].hi + 1;
    	numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
    	for(k = 0 ;k < numOfNewLine ;k++)
    	{
    		for(j = newLines[k].lo ; j < newLines[k].hi ;j++)
    		{
    			_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
    			pFrameBuffer_[ _nPointer ] = color;
    		}
    	}//*/

    	pAscanData->peak.line[i].lo  = pAscanData->peak.currentLine.lo;
    	pAscanData->peak.line[i].hi  = pAscanData->peak.currentLine.hi;
    }
    else if(REFMODE_REF == pAscanData->ref.mode)//ref模式
    {//擦ref
    	newLines[0].lo = pAscanData->ref.line[i].lo;
    	newLines[0].hi = pAscanData->ref.currentLine.lo;
    	numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
    	for(k = 0 ;k < numOfNewLine ;k++)
    	{
    		for(j = newLines[k].lo ; j < newLines[k].hi ;j++)
    		{
    			_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
    			pFrameBuffer_[ _nPointer ] = color;
    		}
    	}//*/

        if(0 == pAscanData->ref.line[i].hi)
        {
        	pAscanData->ref.line[i].hi = pAscanData->axis.vertical;//用于清屏
        }
    	newLines[0].lo = pAscanData->ref.currentLine.hi + 1;
    	newLines[0].hi = pAscanData->ref.line[i].hi + 1;
    	numOfNewLine = verifyLine(oldLines ,&numOfOldLine ,newLines);
    	for(k = 0 ;k < numOfNewLine ;k++)
    	{
    		for(j = newLines[k].lo ; j < newLines[k].hi ;j++)
    		{
    			_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
    			pFrameBuffer_[ _nPointer ] = color;
    		}
    	}//*/

    	pAscanData->ref.line[i].lo  = pAscanData->ref.currentLine.lo;
    	pAscanData->ref.line[i].hi  = pAscanData->ref.currentLine.hi;
    }
/*    for(k = 0 ;k < numOfOldLine ;k++)
    {
    	for(j = oldLines[k].lo ; j < oldLines[k].hi ;j++)
    	{
    		_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
    		pFrameBuffer_[ _nPointer ] = getRangeColor(pAscanData ,i ,j);
    	}
    }//*/
}

static void ascanClear(ascanStruct* pAscanData)
{
	int size = WAVE_MAX_POINT * sizeof(singleLineStruct);

	memset(pAscanData->wave.line ,0 ,size);

    if(REFMODE_PEAK == pAscanData->ref.mode)
    {
    	memset(pAscanData->peak.line ,0 ,size);
    }
    else if(REFMODE_REF == pAscanData->ref.mode)//ref模式
    {
    	memset(pAscanData->ref.line ,0 ,size);
    }
    //包络
    if(pAscanData->envelope.mode)
    {
    	memset(pAscanData->envelope.line[0] ,0 ,size);

    	memset(pAscanData->envelope.line[1] ,0 ,size);
    }
}

#include "../drawui.h"
extern void TofdHandler(int i ,int grp ,int width ,int _nDataOffset ,int _nDataSize);
void ascanDrawConfig(ascanStruct* pAscanData)
{
	int group = pAscanData->group;
	GROUP* pGroup = get_group_by_id (pp->p_config, group);
	if(TMP(dataRecalling) && (TOFD == get_group_val (pGroup, GROUP_TX_RX_MODE)))
	{
		int _nTempValue = TMP(scan);
		int _nIndex ;
		if(get_inspec_source(pp->p_config))
		{
			_nIndex = (_nTempValue * 10 - pp->p_config->inspection_scan_start ) / pp->p_config->inspection_scan_resolution ;
		}
		else
		{
			_nIndex =    _nTempValue * 10  / pp->p_config->inspection_scan_resolution ;
		}

		int k ;
		int _nDataOffset = DMA_DATA_OFFSET ;
		for ( k = 0 ; k < group ; k++)
		{
			_nDataOffset += (pp->nPointQty[k] + 32) * TMP(beam_qty[k]);
		}

		SCAN_INFO_P _pScanInfor;
		_pScanInfor = &pp->ScanInfor ;
		int _nDataSize   =   _pScanInfor->DataSize;
		int width = pAscanData->wave.end - pAscanData->wave.start;

		TofdHandler(_nIndex ,group ,width ,_nDataOffset ,_nDataSize);
		memcpy(pAscanData->wave.dispData ,TMP(scan_data[group]) ,width);
	}
	else
	{
		//*/
		getDisplayData(pAscanData->wave.pData,
				pAscanData->wave.dispData,
				pAscanData->wave.dataPoints,
				pAscanData->wave.end - pAscanData->wave.start,
				pAscanData->wave.rfMode) ;
	}

	if(pAscanData->bClear)
	{
		pAscanData->bClear = FALSE;
		ascanClear(pAscanData);
	}

	if(pAscanData->wave.dispDataWidth != pAscanData->axis.horizontal)
	{
		pAscanData->wave.dispDataWidth = pAscanData->axis.horizontal;
		//数据重置
		ascanResetDispData(pAscanData);
	}

    if(REFMODE_PEAK == pAscanData->ref.mode)
    {
    	//条件达到更新数据//
    	if(abs(pAscanData->peak.amplitude) < abs(pAscanData->gateAmplitude))//A% 穿过闸门A且幅度峰值大更新
    	{
    		pAscanData->peak.amplitude = pAscanData->gateAmplitude;
    		ascanResetPeakData(pAscanData);
    	}
    	if(pAscanData->peak.bRedraw)
    	{
    		pAscanData->peak.bRedraw = FALSE;
    		ascanSetPeakDispData(pAscanData);
    	}
    }
    else if(REFMODE_REF == pAscanData->ref.mode)
    {
    	if(pAscanData->ref.bReset)
    	{
    		ascanResetRefData(pAscanData);
    	}
    	if(pAscanData->ref.bRedraw)
    	{
    		pAscanData->ref.bRedraw = FALSE;
			ascanSetRefDispData(pAscanData);
    	}
    }
//*/
	//分色显示
	if(pAscanData->wave.fillMode > FILLMODE_FILL)
	{
		pAscanData->wave.colorCrossAGate = COLOR_CROSS_GATEA;
	}
	else
	{
		pAscanData->wave.colorCrossAGate = pAscanData->wave.color;
	}

	if(DIRECTION_HORIZONTAL == pAscanData->axis.direction)
	{
		//fill模式offset
		if(pAscanData->wave.rfMode)
		{
			pAscanData->wave.fillOffset = pAscanData->axis.vertical / 2;
		}
		else
		{
			pAscanData->wave.fillOffset = pAscanData->axis.vertical;
		}
	}
	else
	{
		if(pAscanData->wave.rfMode)
		{
			pAscanData->wave.fillOffset = pAscanData->axis.vertical / 2;
		}
		else
		{
			pAscanData->wave.fillOffset = 0;
		}
	}
	//包络实时更新
    if(pAscanData->envelope.mode)
    {
    	//更新数据
    	if(pAscanData->envelope.bReset)
    	{
    		ascanResetEnvelopeData(pAscanData);
    	}
    	ascanSetEnvelope(pAscanData);
    }

}

void ascanDrawWave(unsigned short* pFrameBuffer_ ,ascanStruct* pAscanData)
{
	int i;
	assert(pAscanData->wave.fillMode < FILLMODE_MAX);

	ascanDrawConfig(pAscanData);

	for(i = 0 ;i < pAscanData->axis.horizontal ;++i)
	{
		numOfOldLine = 0;
        //peak模式
        if(REFMODE_PEAK == pAscanData->ref.mode)
        {
        	ascanDrawPeak(pFrameBuffer_ ,pAscanData ,i);
        }
        else if(REFMODE_REF == pAscanData->ref.mode)//ref模式
        {
        	ascanDrawRef(pFrameBuffer_ ,pAscanData ,i);
        }
        //包络
        if(pAscanData->envelope.mode)
        {
        	ascanDrawEnvelope(pFrameBuffer_ ,pAscanData ,i);
        }
        //ascan
		ascanDrawAscan(pFrameBuffer_ ,pAscanData ,i);

		ascanDrawClear(pFrameBuffer_ ,pAscanData ,i);
	}
}












#if 0
//链表实现
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ascanConfigLines(ascanStruct* pAscanData ,int i)
{
	linesStruct* pLine;
	if(NULL == pAscanData->pNewLine[i])
	{
		pAscanData->pNewLine[i] = malloc(sizeof(linesStruct));
	}
	pLine = pAscanData->pNewLine[i];

	int tmp1,tmp2;
	if((i < pAscanData->wave.start) || (i > pAscanData->wave.end))
	{
		tmp1 = 0;
		tmp2 = 0;
	}
	else
	{
		tmp1 = ascanGetPointVerticalAxis(pAscanData ,pAscanData->wave.dispData[i - pAscanData->wave.start]);
		tmp2 = ascanGetPointVerticalAxis(pAscanData ,pAscanData->wave.dispData[i + 1 - pAscanData->wave.start]);
	}
	if(tmp1 < tmp2)
	{
		pLine->lo = tmp1;
		pLine->hi = tmp2;
	}
	else
	{
		pLine->lo = tmp2;
		pLine->hi = tmp1;
	}

	//hollow模式
	if((FILLMODE_HOLLOW == pAscanData->wave.fillMode)
	||(FILLMODE_HOLLOW2COLOR == pAscanData->wave.fillMode)
	)
	{

	}
	else//填充模式
	{
		if(pLine->lo > pAscanData->wave.fillOffset)
		{
			pLine->lo = pAscanData->wave.fillOffset;
		}
		else if(pLine->hi < pAscanData->wave.fillOffset)
		{
			pLine->hi = pAscanData->wave.fillOffset;
		}
	}

    //包络
    if(pAscanData->envelope.mode)
    {
    	if(NULL == pLine->pNext)
    	{

    	}
    	pLine = pLine->pNext;
    }


    //peak模式
    if(REFMODE_PEAK == pAscanData->ref.mode)
    {

    }
    else if(REFMODE_REF == pAscanData->ref.mode)//ref模式
    {

    }

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static inline void insertPoint(linesStruct* pOldLine ,int point)
{
	linesStruct* p = pOldLine->pNext;//原来的下一个，推后

	pOldLine->pNext = malloc(sizeof(linesStruct));//插入的下一个

	pOldLine->pNext->lo = point;
	pOldLine->pNext->hi = pOldLine->hi;
	pOldLine->pNext->pNext = p;//指向原来的下一个

	pOldLine->hi = point;//从point处一分为二
}

//1条彩线中插入一条线
void insertLine(linesStruct* pOldLine ,const linesStruct newLines)
{
	while(pOldLine)
	{
		//insert a line if other color and other point
		if((newLines.lo > pOldLine->lo)
		&&(newLines.lo < pOldLine->hi))
		{
			if(newLines.color != pOldLine->color)//other color
			{
				insertPoint(pOldLine ,newLines.lo);
				pOldLine->pNext->color = pOldLine->color;
			}
			pOldLine = pOldLine->pNext; //next to new line
			break;
		}
		else if(newLines.lo == pOldLine->lo)//the same point with lo ,break
		{
		//	pOldLine = pOldLine->pNext; //can't
			break;
		}
		else if(newLines.lo == pOldLine->hi)//the same point with hi ,break too
		{
			pOldLine = pOldLine->pNext; //next
			break;
		}//*/
		else//still low to lo ,continue, untill up to lo
		{
			pOldLine = pOldLine->pNext;
			continue;
		}
	}
//pOldLine.lo == newLines.lo
//find hi and insert a line if need
//change color up to newLines.hi
	while(pOldLine)
	{
		if((newLines.hi > pOldLine->lo)
		&&(newLines.hi < pOldLine->hi))
		{
			//hi is other point
			//if other color ,insert a line
			if(newLines.color != pOldLine->color)
			{
				insertPoint(pOldLine ,newLines.hi);
				//change line's color
				pOldLine->pNext->color = pOldLine->color;
				pOldLine->color = newLines.color;
			}
			break;//hi found
		}
		else if(newLines.hi == pOldLine->hi)//the same point with hi ,no need insert a new line
		{
			//change color up to newLines.hi
			pOldLine->color = newLines.color;
			break;//hi found
		}
		else if(newLines.hi == pOldLine->lo)//the same point with lo ,no need insert a new line
		{
			//never here
			break;//hi found
		}//*/
		else//new hi < old lo ,continue find
		{
			//change color till newLines.hi
			pOldLine->color = newLines.color;
			//find next
			pOldLine = pOldLine->pNext;
			continue;
		}
	}
}

//delete all lines
void deleteLines(linesStruct* pOldLine)
{
	linesStruct* p;
	while(pOldLine)
	{
		p = pOldLine->pNext;
		free(pOldLine);
		pOldLine = p;
	}
}

//copy all lines
linesStruct* copyLines(linesStruct* pOldLine)
{
	linesStruct* pNewLine = NULL;
	linesStruct* pLine;
	if(pOldLine)
	{
		//first line
		pLine = malloc(sizeof(linesStruct));
		memcpy(pLine ,pOldLine ,sizeof(linesStruct));
		pLine->pNext = NULL;

		pNewLine = pLine;
		//next
		pOldLine = pOldLine->pNext;
		while(pOldLine)
		{
			pLine->pNext = malloc(sizeof(linesStruct));
			pLine = pLine->pNext;
			memcpy(pLine ,pOldLine ,sizeof(linesStruct));
			pLine->pNext = NULL;
			//next
			pOldLine = pOldLine->pNext;
		}
	}
	return pNewLine;
}

//ascan
static inline void getAscanLines(linesStruct* pOldLine ,ascanStruct* pAscanData ,int i)
{
	int tmp1,tmp2;

	//first line
	linesStruct line;
	linesStruct* pLine;
	pLine = &line;

	//get new ascan line
	if((i < pAscanData->wave.start) || (i > pAscanData->wave.end))
	{
		tmp1 = 0;
		tmp2 = 0;
	}
	else
	{
	//	tmp1 = ascanGetPointVerticalAxis(pAscanData ,0);
	//	tmp2 = ascanGetPointVerticalAxis(pAscanData ,0);
		tmp1 = ascanGetPointVerticalAxis(pAscanData ,pAscanData->wave.dispData[i - pAscanData->wave.start]);
		tmp2 = ascanGetPointVerticalAxis(pAscanData ,pAscanData->wave.dispData[i + 1 - pAscanData->wave.start]);
	}

	if(tmp1 < tmp2)
	{
		pLine->lo = tmp1;
		pLine->hi = (tmp2 == pAscanData->axis.vertical) ?tmp2 :tmp2 + 1;
	}
	else
	{
		pLine->lo = tmp2;
		pLine->hi = (tmp1 == pAscanData->axis.vertical) ?tmp1 :tmp1 + 1;
	}
	pLine->color = pAscanData->wave.color;
	pLine->pNext = NULL;

	//if fill mode ,line is diffrent
	switch(pAscanData->wave.fillMode)
	{
	case FILLMODE_HOLLOW://hollow
	case FILLMODE_HOLLOW2COLOR://hollow 2 color
		break;
	case FILLMODE_FILL://fill
	case FILLMODE_FILL2COLOR://fill 2 color
		pLine->lo = pLine->lo < pAscanData->wave.fillOffset ?pLine->lo :pAscanData->wave.fillOffset;
		pLine->hi = pLine->hi > pAscanData->wave.fillOffset ?pLine->hi :pAscanData->wave.fillOffset;
		break;
	default:
		break;
	}
	insertLine(pOldLine ,*pLine);

	//if 2 color ,insert a new line to change 1 color
	switch(pAscanData->wave.fillMode)
	{
	case FILLMODE_HOLLOW:
	case FILLMODE_FILL:
		break;
	case FILLMODE_HOLLOW2COLOR:
	case FILLMODE_FILL2COLOR:
	{
		//horizontal , < gate height change color
		if(pAscanData->axis.direction == DIRECTION_HORIZONTAL)
		{
			// in gate
			if((i >= pAscanData->gateDataA.start)
					&&(i <= pAscanData->gateDataA.end))
			{
				if(pLine->lo < pAscanData->gateDataA.height)
				{
					pLine->color = pAscanData->wave.colorCrossAGate;
					if(pAscanData->gateDataA.height < pLine->hi)
					{
						pLine->hi = pAscanData->gateDataA.height;
					}
					insertLine(pOldLine ,*pLine);
				}

			}
		}
		else//vertical ,>gate height change color
		{//*/
			//in gate
			if((i >= pAscanData->gateDataA.start)
					&&(i <= pAscanData->gateDataA.end))
			{
				if(pLine->hi > pAscanData->gateDataA.height)
				{
					pLine->color = pAscanData->wave.colorCrossAGate;
					if(pAscanData->gateDataA.height > pLine->lo)
					{
						pLine->lo = pAscanData->gateDataA.height;
					}
					insertLine(pOldLine ,*pLine);
				}
			}
		}
	}
		break;
	default:
		break;
	}
}

//envelope
static inline void getEnvelopeLines(linesStruct* pOldLine ,ascanStruct* pAscanData ,int i)
{
	//first line
	linesStruct line;
	linesStruct* pLine;
	pLine = &line;

	int tmp1,tmp2;
	tmp1 = ascanGetPointVerticalAxis(pAscanData ,pAscanData->envelope.dispData[0][i]);
	tmp2 = ascanGetPointVerticalAxis(pAscanData ,pAscanData->envelope.dispData[0][i + 1]);

	if(tmp1 < tmp2)
	{
		pLine->lo = tmp1;
		pLine->hi = (tmp2 == pAscanData->axis.vertical) ?tmp2 :tmp2 + 1;
	}
	else
	{
		pLine->lo = tmp2;
		pLine->hi = (tmp1 == pAscanData->axis.vertical) ?tmp1 :tmp1 + 1;
	}
	pLine->color = ENVELOPE_COLOR;
	pLine->pNext = NULL;

	insertLine(pOldLine ,*pLine);


	//second line
	if((pAscanData->envelope.dispData[0][i] != pAscanData->envelope.dispData[1][i])
	||(pAscanData->envelope.dispData[0][i + 1] != pAscanData->envelope.dispData[1][i + 1])
	)
	{
		tmp1 = ascanGetPointVerticalAxis(pAscanData ,pAscanData->envelope.dispData[1][i]);
		tmp2 = ascanGetPointVerticalAxis(pAscanData ,pAscanData->envelope.dispData[1][i + 1]);

		if(tmp1 < tmp2)
		{
			pLine->lo = tmp1;
			pLine->hi = (tmp2 == pAscanData->axis.vertical) ?tmp2 :tmp2 + 1;
		}
		else
		{
			pLine->lo = tmp2;
			pLine->hi = (tmp1 == pAscanData->axis.vertical) ?tmp1 :tmp1 + 1;
		}
		insertLine(pOldLine ,*pLine);
	}
}

//peak
static inline void getPeakLines(linesStruct* pOldLine ,ascanStruct* pAscanData ,int i)
{
	linesStruct line;
	linesStruct* pLine;
	pLine = &line;

	int tmp1,tmp2;
	tmp1 = ascanGetPointVerticalAxis(pAscanData ,pAscanData->peak.dispData[i]);
	tmp2 = ascanGetPointVerticalAxis(pAscanData ,pAscanData->peak.dispData[i + 1]);

	if(tmp1 < tmp2)
	{
		pLine->lo = tmp1;
		pLine->hi = (tmp2 == pAscanData->axis.vertical) ?tmp2 :tmp2 + 1;
	}
	else
	{
		pLine->lo = tmp2;
		pLine->hi = (tmp1 == pAscanData->axis.vertical) ?tmp1 :tmp1 + 1;
	}
	pLine->color = PEAK_COLOR;
	pLine->pNext = NULL;

	insertLine(pOldLine ,*pLine);
}

//Ref
static inline void getRefLines(linesStruct* pOldLine ,ascanStruct* pAscanData ,int i)
{
	linesStruct line;
	linesStruct* pLine;
	pLine = &line;

	int tmp1,tmp2;
	if((i - pAscanData->ref.offset >= 0)
	&& (i - pAscanData->ref.offset + 1 < pAscanData->wave.dispDataWidth)
	&& (i <= pAscanData->ref.width + pAscanData->ref.offset)
	)
	{
		tmp1 = ascanGetPointVerticalAxis(pAscanData ,pAscanData->ref.dispData[i - pAscanData->ref.offset]);
		tmp2 = ascanGetPointVerticalAxis(pAscanData ,pAscanData->ref.dispData[i + 1 - pAscanData->ref.offset]);
		if(tmp1 < tmp2)
		{
			pLine->lo = tmp1;
			pLine->hi = (tmp2 == pAscanData->axis.vertical) ?tmp2 :tmp2 + 1;
		}
		else
		{
			pLine->lo = tmp2;
			pLine->hi = (tmp1 == pAscanData->axis.vertical) ?tmp1 :tmp1 + 1;
		}
	}
	else
	{
		pLine->lo = pLine->hi = 0;//no line
	}

	pLine->color = REF_COLOR;
	pLine->pNext = NULL;

	insertLine(pOldLine ,*pLine);
}

static linesStruct* getNewLines(ascanStruct* pAscanData ,int i)
{
	assert(i >= 0);
	assert(i < WAVE_MAX_POINT);

	//first line ,background
	linesStruct* pLine;
	pLine = malloc(sizeof(linesStruct));
	pLine->color = COLOR_BACKGROUD;
	pLine->lo = 0;
	pLine->hi = pAscanData->axis.vertical;
	pLine->pNext = NULL;

	//AScan wave
	getAscanLines(pLine ,pAscanData ,i);

	//envelope
    if(pAscanData->envelope.mode)
    {
		getEnvelopeLines(pLine ,pAscanData ,i);
    }


    if(REFMODE_PEAK == pAscanData->ref.mode)//peak
    {
    	getPeakLines(pLine ,pAscanData ,i);
    }
    else if(REFMODE_REF == pAscanData->ref.mode)//ref
    {
    	getRefLines(pLine ,pAscanData ,i);
    }
    else //NONE
    {
    	//nothing
    }

	return pLine;
}

void _ascanDrawWave(unsigned short* pFrameBuffer_ ,ascanStruct* pAscanData)
{
	int i ,j;
	int _nPointer;
	linesStruct* pLine;
	linesStruct* pDrawLine;
	linesStruct* pOldLine;
	linesStruct* pNewLine;
	linesStruct* p;

	int lo ,hi;
	//config
	ascanDrawConfig(pAscanData);
	//every horizontal point
	for(i = 0 ;i < pAscanData->axis.horizontal ;++i)
	{
		//init if null
		if(NULL == pAscanData->pOldLine[i])
		{
			pAscanData->pOldLine[i] = malloc(sizeof(linesStruct));
			pAscanData->pOldLine[i]->color = COLOR_BACKGROUD;
			pAscanData->pOldLine[i]->lo = 0;
			pAscanData->pOldLine[i]->hi = pAscanData->axis.vertical;
			pAscanData->pOldLine[i]->pNext = NULL;
		}
		//old line
		pOldLine = pAscanData->pOldLine[i];
		//draw line ,init copy old line
		pDrawLine = copyLines(pOldLine);
		//new line
		pNewLine = getNewLines(pAscanData ,i);

		//get draw lines from new line 遮蔽分析
		p = pNewLine;
		pLine = pDrawLine;
		while(p)
		{
			insertLine(pLine ,*p);
			p = p->pNext;
		}

		//draw
		//every old line
		pLine = pDrawLine;
		while(pOldLine)
		{
			//every draw lines in old line area
			while(pLine)
			{
				//out of old line area ,break to next old line
				if(pLine->lo == pOldLine->hi)
				{
					break;
				}
				//the same color no need draw
				if(pLine->color != pOldLine->color)
				{
					lo = pLine->lo;//ascanGetPointVerticalAxis(pAscanData ,pLine->lo);
					hi = pLine->hi;//ascanGetPointVerticalAxis(pAscanData ,pLine->hi);
					for(j = lo ;j < hi ;++j)
					{
						_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j) ;
						pFrameBuffer_[_nPointer] = pLine->color;
					}
					//pAscanData->axis.vertical显示不了，用pAscanData->axis.vertical-1 代替
					if(hi == pAscanData->axis.vertical)
					{
						_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,hi - 1) ;
						pFrameBuffer_[_nPointer] = pLine->color;
					}
				}
				//next draw line
				pLine = pLine->pNext;
			}
			//next old line
			pOldLine = pOldLine->pNext;
		}

		//delete draw lines
		deleteLines(pDrawLine);
		//delete old lines
		deleteLines(pAscanData->pOldLine[i]);
		//change new lines to old lines
		pAscanData->pOldLine[i] = pNewLine;
	}
}

void _ascanClear(unsigned short* pFrameBuffer_ ,ascanStruct* pAscanData)
{
	int i ,j;
	int _nPointer;
	//clear
	for(i = 0 ;i < pAscanData->axis.horizontal ;++i)
	{
		for(j = 0 ;j < pAscanData->axis.vertical ;++j)
		{
			_nPointer = ascanGetPointHorizontalAxis(pAscanData ,i ,j);
			pFrameBuffer_[_nPointer] = COLOR_BACKGROUD;
		}
	}
	//init old lines
	memset(pAscanData->pOldLine ,0 ,WAVE_MAX_POINT * sizeof(linesStruct*));
}
#endif

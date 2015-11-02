/*
 * Ascan.h
 *
 *  Created on: 2012-5-9
 *      Author: lzk
 */

#ifndef ASCAN_H_
#define ASCAN_H_

#define WAVE_MAX_POINT 			640
#define ORIGIN_WAVE_MAX_POINT 	8192

#define COLOR_BACKGROUD 	0x0000
#define COLOR_ENVELOPE 		0x07ff /*青色*/
#define COLOR_REF 			0x001f /*blue*/
#define COLOR_PEAK 			0xf800 /*red*/
#define COLOR_CROSS_GATEA 	0xf81f
enum{
	REFMODE_NONE,
	REFMODE_PEAK,
	REFMODE_REF,
	REFMODE_MAX,
};

enum{
	FILLMODE_HOLLOW,
	FILLMODE_FILL,
	FILLMODE_HOLLOW2COLOR,
	FILLMODE_FILL2COLOR,
	FILLMODE_MAX
};

typedef struct
{
	int x;
	int y;
	int width;
	int height;
}
	rectangleStruct;

typedef struct
{
	int start;
	int end;
	int height;
}
	gateDataStruct;

typedef struct{
	//rectangleStruct waveWidget;//fb1的窗口
	gateDataStruct gateDataA;//fb1上的闸门

}
	scanWidgetStruct;
//////////////////////////////////////////////////////

typedef struct lines_struct
{
	struct lines_struct* pNext;//end with NULL
	int lo;//min:0
	int hi;//max:height + 1
	unsigned short color;
}
	linesStruct;//line:[lo ,hi) 或 [lo ,hi - 1]

typedef struct singleLine_struct
{
	int lo;
	int hi;
}
	singleLineStruct;

typedef struct{
	unsigned char mode;//包络模式
	unsigned char bReset;
	//unsigned short color;
	//unsigned char bRedraw;
	singleLineStruct currentLine[2];
	singleLineStruct line[2][WAVE_MAX_POINT];
	unsigned char dispData[2][WAVE_MAX_POINT];//包络数据
}
	ascanEnvelopeStruct;

typedef struct{
	unsigned char mode;
	//unsigned short color;
	unsigned char bRedraw;
	signed short amplitude;//12位,measure【1】高12位
	unsigned int dataPoints;
	singleLineStruct currentLine;
	singleLineStruct line[WAVE_MAX_POINT];
	unsigned char dispData[WAVE_MAX_POINT];//显示数据
	unsigned char originalData[ORIGIN_WAVE_MAX_POINT];//原始数据
}
	ascanPeakStruct;

typedef struct{
	unsigned char mode;
	unsigned char bReset;
	unsigned char bRedraw;
	//unsigned short color;
	unsigned int width;//波形宽度
	unsigned int dataPoints;
	int start;//10ns
	int range;//10ns
	int offset;//波形起始点
	singleLineStruct currentLine;
	singleLineStruct line[WAVE_MAX_POINT];
	unsigned char dispData[ORIGIN_WAVE_MAX_POINT];//显示数据
	unsigned char originalData[ORIGIN_WAVE_MAX_POINT];//原始数据

}
	ascanRefStruct;

typedef struct{
	unsigned char fillMode;
	unsigned char rfMode;
	unsigned short color;//ascan颜色，填充颜色
	unsigned short colorCrossAGate;//超过gateA的AScan或填充颜色
	unsigned short fillOffset;
	unsigned short dispDataWidth;
	unsigned short start;
	unsigned short end;
	unsigned int dataPoints;
	singleLineStruct currentLine;
	singleLineStruct line[WAVE_MAX_POINT];
	unsigned char dispData[WAVE_MAX_POINT];//显示数据
//	unsigned char originalData[ORIGIN_WAVE_MAX_POINT];//原始数据
	unsigned char* pData;//ascan显示原始数据beam数据地址
}
	ascanWaveStruct;

enum{
	DIRECTION_HORIZONTAL = 0,
	DIRECTION_VERTICAL = 90,
	DIRECTION_MAX = 360,
};
typedef struct{
	unsigned short direction;
	unsigned short horizontal;//坐标轴横向宽度
	unsigned short vertical;//坐标轴纵向宽度
	unsigned int x;
	unsigned int y;
}
	axisStruct;

typedef struct{
	unsigned char group;
	unsigned char bClear;
	signed short gateAmplitude;//与%A成正比
	unsigned int start;//10ns
	unsigned int range;//10ns
	axisStruct axis;
	gateDataStruct gateDataA;
	ascanEnvelopeStruct envelope;
	ascanPeakStruct peak;
	ascanRefStruct ref;
	ascanWaveStruct wave;
//	linesStruct* pOldLine[WAVE_MAX_POINT];//链表实现
//	linesStruct* pNewLine[WAVE_MAX_POINT];//链表实现
}
	ascanStruct;

void getDisplayData(const unsigned char* const sourceData ,unsigned char* targetData ,
		int sourceQty, int targetQty ,unsigned int rfMode);

void ascanDataInit();
void ascanResetPeak(int group);
void ascanResetCurvesWhenStartOrRangeChanged(int group);
void ascanResetRef(int group);
void ascanResetEnvelope(int group);

void ascanClearDisp(int group);
void ascanClearAndRedraw(int group);
//常规优化
void ascanDrawWave(unsigned short* pFrameBuffer_ ,ascanStruct* pAscanData);

//链表实现
//void _ascanDrawWave(unsigned short* pFrameBuffer_ ,ascanStruct* pAscanData);

#endif /* ASCAN_H_ */

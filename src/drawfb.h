
/*
 *345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#ifndef	__DRAW_FB_H_
#define	__DRAW_FB_H_

#include "base_const.h"

#define FB_DEVICE "/dev/fb1"

#define FB_WIDTH 768
#include <pthread.h>
#define MAX_BEAM_QTY    256
pthread_mutex_t ttylock;
typedef struct _Point
{
	int x;
	int y;
}POINT, *P_POINT;
typedef struct _VERTICAL_LINE
{
	int start;
	int end  ;
}VERTICAL_LINE, *P_VERTICAL_LINE;


typedef struct _BSCAN_DRAW_INFO
{
	int nGroupId ;
	int nOffsetX ;
	int nOffsetY ;
	int nWidth   ;
	int nHeight  ;
	int nShowStart    ;
	int nShowStop     ;
	int nZeroOffset   ;
	int nTotalSteps   ;
	int nDataOffset   ;
	int nDataSize     ;
	int nMaxStoreIndex;
	int nShowBeamQty  ;
	int nWndType      ;
	int nEncoderResolution ;
	unsigned short* pColor  ;

	int nPosRecorderStart  ;
	int nPosRecorderStop   ;
}BSCAN_DRAW_INFO , *BSCAN_DRAW_INFO_P ;

typedef struct _CSCAN_DRAW_INFO
{
	int nGroupId ;
	int nOffsetX ;
	int nOffsetY ;
	int nWidth   ;
	int nHeight  ;
	int nShowStart    ;
	int nShowStop     ;
	int nZeroOffset   ;
	int nTotalSteps   ;
	int nDataOffset   ;
	int nDataSize     ;
	int nMaxStoreIndex;
	int nShowBeamQty  ;
	int nWndType      ;
	int nSourceType    ;
	int nDataType     ;
	int bRFmode       ;
	int nEncoderResolution ;
	unsigned short* pColor  ;

	int nPosRecorderStart  ;
	int nPosRecorderStop   ;
	float nCosAngleValue[setup_MAX_GROUP_LAW_QTY];
}CSCAN_DRAW_INFO , *CSCAN_DRAW_INFO_P ;

typedef struct _TOFDB_DRAW_INFO
{
	int nGroupId ;
	int nOffsetX ;
	int nOffsetY ;
	int nWidth   ;
	int nHeight  ;
	int nShowStart    ;
	int nShowStop     ;
	int nZeroOffset   ;
	int nTotalSteps   ;
	int nDataOffset   ;
	int nDataSize     ;
	int nMaxStoreIndex;
	int nShowBeamQty  ;
	int nWndType      ;
	int nEncoderResolution ;
	unsigned short* pColor  ;

	int nPosRecorderStart  ;
	int nPosRecorderStop   ;
}TOFDB_DRAW_INFO , *TOFDB_DRAW_INFO_P ;


typedef struct fan_scan_draw_info
{
    double   nStartAngle;
    double   nStopAngle ;
    double   nStepAngle ;
    double   nSampleStart ;
    double   nSampleRange ;
    float*  pExitPoint  ;
    int    nPointQty    ;
    int    width ;
    int    height ;
    int    group  ;
    int    direction  ;
}FAN_SCAN_INFO, *FAN_SCAN_INFO_P  ;

typedef struct linear_scan_draw_info
{
	float  fAngle    ;
	float  fRange    ;
	int    nLawQty   ;
    int    nPointQty ;
    int    nWidth    ;
    int    nHeight   ;
    int    nGroupId  ;
	float*  pExitPoint  ;
    int     direction   ;
}LINEAR_SCAN_INFO, *LINEAR_SCAN_INFO_P  ;



extern void init_fb ();
extern void init_mem ();
extern void init_serial ();
extern void fbdot(unsigned short *p, int x, int y, unsigned short col);
extern void fbliney(unsigned short *p, int x, int y1, int y2, unsigned short col);
extern void fblinex(unsigned short *p, int y, int x1, int x2, unsigned short col);
extern void fbline(unsigned short *p, int x1, int y1, int x2, int y2, unsigned short col);
extern void draw_scan(unsigned char WinIndex, unsigned short *pFrameBuffer , unsigned int pBeamData)  ;
extern void RefreshDisplayMatrix(unsigned char WinIndex)  ;
extern int  getCurrentBscanStart();
extern int  getCurrentBscanStop();
extern int  getCurrentTofdBscanStart(int nWndIndex_);
extern int  getCurrentTofdBscanStop(int nWndIndex_);
extern int  getCurrentCscanStart(int nIndex_);
extern int  getCurrentCscanStop(int nIndex_);

extern POINT CurrentLine_top [setup_MAX_GROUP_QTY][MAX_BEAM_QTY];
extern POINT CurrentLine_bottom [setup_MAX_GROUP_QTY][MAX_BEAM_QTY];
extern VERTICAL_LINE AScanDrawRange[setup_MAX_GROUP_QTY][MAX_BEAM_QTY];


extern int g_nScanTimmerCounter ;
extern int g_bScanTimmerCircled ;

extern void bscan_set_refresh(int flag);

#endif

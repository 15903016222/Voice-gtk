
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include "base.h"
#include "drawui.h"
#include "drawfb.h"
#include "callback.h"
#include <termios.h>	//串口头文件 
#include <fcntl.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <gtk/gtk.h>
#include <pthread.h>
#include "globalData.h"
#include "lzk/Ascan.h"
#include "calibration.h"

#define _PI        3.1415926

#define MEM_DEVICE "/dev/mem"
#define TTY_DEVICE "/dev/ttyS1"
#define TTY_DEVICE1 "/dev/ttyS0"

#define COLOR_STEP 32     /*    4  8  16  32  64*/
#define COLOR_SHIFT 5     /*    2  3   4   5   6*/
#define MIN_DRAW_FAN_ANGLE_STEP    3

#define MAX_DB_12_BIT   0x7ff

POINT CurrentLine_top [setup_MAX_GROUP_QTY][MAX_BEAM_QTY];
POINT CurrentLine_bottom [setup_MAX_GROUP_QTY][MAX_BEAM_QTY];
VERTICAL_LINE AScanDrawRange[setup_MAX_GROUP_QTY][MAX_BEAM_QTY];

static unsigned char	pAngleZoom[setup_MAX_GROUP_QTY][605 *  390] ; // = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}; // 处于哪个角度区间
static char	            pDraw[setup_MAX_GROUP_QTY][605 *  390] ;      //     = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}; // 是否扇形区域
static unsigned char	pDrawRate[setup_MAX_GROUP_QTY][605 *  390] ;  //  = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}; // 填充比例
static int  	        pDataNo[setup_MAX_GROUP_QTY][605 *  390] ;    //    = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}; // 数据在数组中的列号

static int fd_fb;
struct fb_var_screeninfo vinfo;


int g_nScanTimmerCounter ;
int g_bScanTimmerCircled ;



static unsigned short all_col_16[] =
{
	0x001f,				/* 蓝色 */
	0x03e0,				/* 绿色 */
	0xf800,				/* 红色 */
	0xffe0,				/* 黄色 */
	0x0000,				/* 黑色 */
	0xffff				/* 白色 */		
};

static double HEIGHT_TABLE[256]=
{
	1.000000,0.996078,0.992157,0.988235,0.984314,0.980392,0.976471,0.972549,
	0.968627,0.964706,0.960784,0.956863,0.952941,0.949020,0.945098,0.941176,
	0.937255,0.933333,0.929412,0.925490,0.921569,0.917647,0.913725,0.909804,
	0.905882,0.901961,0.898039,0.894118,0.890196,0.886275,0.882353,0.878431,
	0.874510,0.870588,0.866667,0.862745,0.858824,0.854902,0.850980,0.847059,
	0.843137,0.839216,0.835294,0.831373,0.827451,0.823529,0.819608,0.815686,
	0.811765,0.807843,0.803922,0.800000,0.796078,0.792157,0.788235,0.784314,
	0.780392,0.776471,0.772549,0.768627,0.764706,0.760784,0.756863,0.752941,
	0.749020,0.745098,0.741176,0.737255,0.733333,0.729412,0.725490,0.721569,
	0.717647,0.713725,0.709804,0.705882,0.701961,0.698039,0.694118,0.690196,
	0.686275,0.682353,0.678431,0.674510,0.670588,0.666667,0.662745,0.658824,
	0.654902,0.650980,0.647059,0.643137,0.639216,0.635294,0.631373,0.627451,
	0.623529,0.619608,0.615686,0.611765,0.607843,0.603922,0.600000,0.596078,
	0.592157,0.588235,0.584314,0.580392,0.576471,0.572549,0.568627,0.564706,
	0.560784,0.556863,0.552941,0.549020,0.545098,0.541176,0.537255,0.533333,
	0.529412,0.525490,0.521569,0.517647,0.513725,0.509804,0.505882,0.501961,
	0.498039,0.494118,0.490196,0.486275,0.482353,0.478431,0.474510,0.470588,
	0.466667,0.462745,0.458824,0.454902,0.450980,0.447059,0.443137,0.439216,
	0.435294,0.431373,0.427451,0.423529,0.419608,0.415686,0.411765,0.407843,
	0.403922,0.400000,0.396078,0.392157,0.388235,0.384314,0.380392,0.376471,
	0.372549,0.368627,0.364706,0.360784,0.356863,0.352941,0.349020,0.345098,
	0.341176,0.337255,0.333333,0.329412,0.325490,0.321569,0.317647,0.313725,
	0.309804,0.305882,0.301961,0.298039,0.294118,0.290196,0.286275,0.282353,
	0.278431,0.274510,0.270588,0.266667,0.262745,0.258824,0.254902,0.250980,
	0.247059,0.243137,0.239216,0.235294,0.231373,0.227451,0.223529,0.219608,
	0.215686,0.211765,0.207843,0.203922,0.200000,0.196078,0.192157,0.188235,
	0.184314,0.180392,0.176471,0.172549,0.168627,0.164706,0.160784,0.156863,
	0.152941,0.149020,0.145098,0.141176,0.137255,0.133333,0.129412,0.125490,
	0.121569,0.117647,0.113725,0.109804,0.105882,0.101961,0.098039,0.094118,
	0.090196,0.086275,0.082353,0.078431,0.074510,0.070588,0.066667,0.062745,
	0.058824,0.054902,0.050980,0.047059,0.043137,0.039216,0.035294,0.031373,
	0.027451,0.023529,0.019608,0.015686,0.011765,0.007843,0.003922,0.000000
};

void init_fb ()
{
	if ((fd_fb = open(FB_DEVICE, O_RDWR)) == -1)
	{
		perror (FB_DEVICE);
		return ;
	}

	TMP(fb1_addr) = (unsigned short *)
		mmap(NULL, 2 * 1024 * 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);
	//TMP(fb1_addr) += 768 * 400 ;
	g_print ("fb1 video addr:%p %p", TMP(fb1_addr), TMP(virtual_add));

	if (ioctl(fd_fb, FBIOGET_VSCREENINFO, &vinfo))
	{
		g_debug("Error reading variable information\n");
		exit(3);
	}

	return ;
}



void init_mem ()
{
	int fd_mem;

	if ((fd_mem = open(MEM_DEVICE, O_RDWR | O_SYNC)) == -1) 
	{
		perror (MEM_DEVICE);
		return ;
	}
	
	TMP(dma_data_add) = (unsigned long int)
		mmap (0,  16 * 1024 * 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd_mem, 0x8f000000);
	TMP(StoreBuffAddr) = (unsigned long int)
		mmap (0, 256 * 1024 * 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd_mem, 0x90000000);

	//TMP(fb1_addr) = (unsigned short*) (TMP(dma_data_add) + 0x100000);
	g_debug ("mem init \n");
	g_debug("virtual addr dma_data %x , buffer_addr %x \n" , TMP(dma_data_add) , TMP(StoreBuffAddr) );
	return ;
}

void init_serial ()
{
	struct termios newtermios;
	pp->fd_key = open(TTY_DEVICE, O_RDWR | O_NOCTTY );
	if (pp->fd_key < 0) {
		perror(TTY_DEVICE); 
		return ;
	}
	bzero(&newtermios, sizeof(newtermios)); /* 清除结构体以放入新的序列埠设定值 */
	tcgetattr(pp->fd_key, &newtermios);
	newtermios.c_cc[VMIN] = 1;
	newtermios.c_iflag = 0;
	newtermios.c_oflag = 0;
	newtermios.c_lflag = 0; 
	newtermios.c_cflag	= B115200 | CS8 | PARENB | CLOCAL | CREAD;
	if (tcsetattr(pp->fd_key, TCSANOW, &newtermios)) {
		perror("tcsetattr   error");  
		exit(1);  
	}

	pp->fd_key1 = open(TTY_DEVICE1, O_RDWR | O_NOCTTY );
	if (pp->fd_key1 < 0) {
		perror(TTY_DEVICE1); 
		return ;
	}
	bzero(&newtermios, sizeof(newtermios)); /* 清除结构体以放入新的序列埠设定值 */
	tcgetattr(pp->fd_key1, &newtermios);
	//newtermios.c_cc[VTIME] = 10;
	newtermios.c_cc[VMIN] = 0;
	newtermios.c_iflag = 0;
	newtermios.c_oflag = 0;
	newtermios.c_lflag = 0; 
	newtermios.c_cflag	= B115200 | CS8 | PARENB | CLOCAL | CREAD;
	if (tcsetattr(pp->fd_key1, TCSANOW, &newtermios)) {
		perror("tcsetattr   error");  
		exit(1);  
	}
	return ;
}


/* draw a dot on coordinate (x,y) 在 (x,y) 画点 */
inline void fbdot(unsigned short *p, int x, int y, unsigned short col)
{
	*(p + FB_WIDTH*y + x) = col;
}

/*draw a line from coordinate(x,y1) to coordinate (x,y2) 画线(x,y1) 到 (x,y2)*/
void fbliney(unsigned short *p, int x, int y1, int y2, unsigned short col)
{
	int yy1, yy2;
	yy1 = MAX(y1, y2);
	yy2 = MIN(y1, y2);
	for (;yy2 < yy1 + 1; yy2++)
		fbdot(p, x, yy2, col);
}

/*draw a line from coordinate(x1,y) to coordinate (x2,y) 画线(x1,y) 到 (x2,y)*/
void fblinex(unsigned short *p, int y, int x1, int x2, unsigned short col)
{
	int xx1, xx2;
	xx1 = MAX(x1, x2);
	xx2 = MIN(x1, x2);
	for (;xx2 < xx1 + 1; xx2++)
		fbdot(p, xx2, y, col);
}

/*draw a line from coordinate(x1,y1) ro coordinate(x2,y2)  画线(x1,y1)到(x2,y2)*/
void fbline(unsigned short *p, int x1, int y1, int x2, int y2, unsigned short col)
{
	int M, N, n, r, s, t;

	if (x1 > x2)
	{
		M = x1 ; x1 = x2; x2 = M;
		M = y1 ; y1 = y2; y2 = M;
	}

	/* 算出,两个点之间的x坐标的差值.*/
	M = ABS(x1 - x2);
	/* 算出,两个点之间的y坐标的差值.*/
	N = ABS(y1 - y2);
	/*如果两个点之间的x坐标间差值是0的话,则调用画竖线函数fbliney来完成*/
	if (M == 0)
		fbliney(p, x1, y1, y2, col);
	/*如果两个点之间的y坐标间差值是0的话,则调用画横线函数fblinex来完成*/
	if (N == 0)
		fblinex(p, y1, x1, x2, col);

	/*如果两个点之间的斜率是1的话*/
	if ((M == N) && (M != 0))
	{
		for (t = x1; t <= x2; t++)
		{
			s = (t - x1) * (y2 - y1)/M+y1;
			fbdot(p, t, s, col);
		}
	}
	if ((M < N) && (M != 0))
	{
		for (t = x1, r = y1; t <= x2; t++)
		{
			s = (t - x1) * (y2 - y1)/M+y1;
			fbliney(p, t, r, s, col);
			r = s;
		}
	}
	if ((M > N) && (N != 0))
	{
		if (y1 > y2)
		{
			n = x1; x1 = x2;x2 = n;
			n = y1; y1 = y2;y2 = n;
		}
		for (t = y1, r = x1; t <= y2; t++)
		{
			s = (t - y1) * (x2 - x1)/N+x1;
			fblinex(p, t, r, s, col);
			r=s;
		}
	}
}

void draw_a_scan_clb (unsigned short *p, int width, int height,
		DOT_TYPE *data,int xoffset, int yoffset, unsigned short point_qty, unsigned char nGroupId_, int start, int end)
{
	int	   i;
	int range   ;
	range = end - start + 1 ;
	/* 清空这块显示区 背景暂定黑色 可以全部一起清空 */
	for (i = 0; i < height; i++)
		memset (p + FB_WIDTH * (i + yoffset) + xoffset, 0x0, width * 2 );
	/* 画回波 */
	for (i = 0; i < range - 1 ; i++)
	{

		fbline (p,
				xoffset + start + i,
				yoffset + height * HEIGHT_TABLE[data[(int)(0.5 + i * point_qty / range)]],
				xoffset + start + i + 1,
				yoffset + height * HEIGHT_TABLE[data[(int)(0.5 + (i + 1) * point_qty / range)]],
				all_col_16[GROUP_VAL_POS(nGroupId_, ascan_color)]);
	}

}


//*****************************************************
//*****
//*****    B SCAN
//*****
//*****************************************************
// init the B scan display information
#include "lzk/tofd.h"

BSCAN_DRAW_INFO g_BscanDrawInfo ;

void DrawBscanInit(int nWndIndex_)
{
	int _nGroupId ;
	int _nHeight , _nWidth , _nOffsetX , _nOffsetY;
	int _nShowStart , _nShowStop   , _nShowBeamQty;
	unsigned short* _pColor  ;
	_nGroupId  = TMP(_scan_group[nWndIndex_])     ;
	_nOffsetX  = TMP(_scan_xpos[nWndIndex_])      ;
	_nOffsetY  = TMP(_scan_ypos[nWndIndex_])      ;
	_nWidth    = TMP(_scan_width[nWndIndex_])     ;
	_nHeight   = TMP(_scan_height[nWndIndex_])    ;

	SCAN_INFO_P _pScanInfor;
	_pScanInfor = &pp->ScanInfor ;

	_nShowStart =  0 ;
	// each beam should display at least one pixel line
	if(pp->nEncoderType)
	{
		_nShowStop  =  MIN(_nHeight , _pScanInfor->ScanStepQty)  - 1 ;
		if(_pScanInfor->ScanZeroOffset > _nShowStop / 2)
		{
			_nShowStart = _pScanInfor->ScanZeroOffset - _nShowStop / 2 ;
			_nShowStop  = _nShowStart  + _nShowStop     ;
		}
	}
	else
	{
		_nShowStop  =  _nHeight  - 1 ;
	}
	_nShowBeamQty  =  _nShowStop - _nShowStart + 1  ;

	int k ;
	int _nOffsetGroup = DMA_DATA_OFFSET ;
	for ( k = 0 ; k < _nGroupId ; k++)
	{
		_nOffsetGroup += (pp->nPointQty[k] + 32) * TMP(beam_qty[k]);
	}
	//int _nDataOffset =  _nOffsetGroup + TMP(beam_num[_nGroupId]) * (pp->nPointQty[_nGroupId] + 32) ;

    if(pp->p_config->group[_nGroupId].rectifier1)
	        _pColor = TMP(color_amp)  ;
    else
    	    _pColor = TMP(color_tofd) ;

	g_BscanDrawInfo.nGroupId       = _nGroupId ;
	g_BscanDrawInfo.nDataOffset    = _nOffsetGroup ; //_nDataOffset ;
	g_BscanDrawInfo.nDataSize      = _pScanInfor->DataSize ;
	g_BscanDrawInfo.nMaxStoreIndex = _pScanInfor->MaxStoreIndex  ;
	g_BscanDrawInfo.nTotalSteps    = _pScanInfor->ScanStepQty    ;
	g_BscanDrawInfo.nZeroOffset    = _pScanInfor->ScanZeroOffset ;
	g_BscanDrawInfo.nShowBeamQty   = _nShowBeamQty  ;
	g_BscanDrawInfo.nOffsetX       = _nOffsetX  ;
	g_BscanDrawInfo.nOffsetY       = _nOffsetY  ;
	g_BscanDrawInfo.nWidth         = _nWidth    ;
	g_BscanDrawInfo.nHeight        = _nHeight   ;
	g_BscanDrawInfo.nShowStart     = _nShowStart;
	g_BscanDrawInfo.nShowStop      = _nShowStop ;
	g_BscanDrawInfo.pColor         = _pColor    ;

	g_BscanDrawInfo.nPosRecorderStart  = 0  ;
	g_BscanDrawInfo.nPosRecorderStop   = g_BscanDrawInfo.nTotalSteps - 1  ;

	g_BscanDrawInfo.nEncoderResolution = _pScanInfor->StepsPerResolution  ;

	if(_nHeight < g_BscanDrawInfo.nTotalSteps)
	{
		g_BscanDrawInfo.nWndType   = 1  ;
	}
	else
	{
		g_BscanDrawInfo.nWndType   = 0  ;
	}

}

TOFDB_DRAW_INFO  g_TofdBscanDrawInfo[16] ;
void DrawBscanTOFDInit(int nWndIndex_)
{
	int _nGroupId ;
	int _nHeight , _nWidth , _nOffsetX , _nOffsetY, _nRef;
	int _nShowStart , _nShowStop   , _nShowBeamQty;
	unsigned short* _pColor  ;
	_nGroupId  = TMP(_scan_group[nWndIndex_])     ;
	_nOffsetX  = TMP(_scan_xpos[nWndIndex_])      ;
	_nOffsetY  = TMP(_scan_ypos[nWndIndex_])      ;
	_nWidth    = TMP(_scan_width[nWndIndex_])     ;
	_nHeight   = TMP(_scan_height[nWndIndex_])    ;

	SCAN_INFO_P _pScanInfor;
	_pScanInfor = &pp->ScanInfor ;

	_nShowStart =  0 ;
	// each beam should display at least one pixel line
    if (TMP(_scan_type[nWndIndex_]) == B_SCAN_VERT) {
        _nRef = _nHeight;
    } else {
        _nRef = _nWidth;
    }

	if(pp->nEncoderType)
	{
        _nShowStop  =  MIN(_nRef, _pScanInfor->ScanStepQty)  - 1 ;
		if(_pScanInfor->ScanZeroOffset > _nShowStop / 2)
		{
			_nShowStart = _pScanInfor->ScanZeroOffset - _nShowStop / 2 ;
			_nShowStop  = _nShowStart  + _nShowStop     ;
		}
	}
	else
	{
    	_nShowStop  =  _nRef  - 1 ;
	}
	_nShowBeamQty  =  _nShowStop - _nShowStart + 1  ;

	int k ;
	int _nOffsetGroup = DMA_DATA_OFFSET ;
	for ( k = 0 ; k < _nGroupId ; k++)
	{
		_nOffsetGroup += (pp->nPointQty[k] + 32) * TMP(beam_qty[k]);
	}
	int _nDataOffset =  _nOffsetGroup + TMP(beam_num[_nGroupId]) * (pp->nPointQty[_nGroupId] + 32) ;

    if(pp->p_config->group[_nGroupId].rectifier1)
	        _pColor = TMP(color_amp)  ;
    else
    	    _pColor = TMP(color_tofd) ;

	//_pColor = TMP(color_tofd) ;

	TOFDB_DRAW_INFO_P pTofdBscanDrawInfo = &(g_TofdBscanDrawInfo[nWndIndex_]);
	pTofdBscanDrawInfo->nGroupId       = _nGroupId ;
	pTofdBscanDrawInfo->nDataOffset    = _nDataOffset ;
	pTofdBscanDrawInfo->nDataSize      = _pScanInfor->DataSize ;
	pTofdBscanDrawInfo->nMaxStoreIndex = _pScanInfor->MaxStoreIndex  ;
	pTofdBscanDrawInfo->nTotalSteps    = _pScanInfor->ScanStepQty    ;
	pTofdBscanDrawInfo->nZeroOffset    = _pScanInfor->ScanZeroOffset ;
	pTofdBscanDrawInfo->nShowBeamQty   = _nShowBeamQty  ;
	pTofdBscanDrawInfo->nOffsetX       = _nOffsetX  ;
	pTofdBscanDrawInfo->nOffsetY       = _nOffsetY  ;
	pTofdBscanDrawInfo->nWidth         = _nWidth    ;
	pTofdBscanDrawInfo->nHeight        = _nHeight   ;
	pTofdBscanDrawInfo->nShowStart     = _nShowStart;
	pTofdBscanDrawInfo->nShowStop      = _nShowStop ;
	pTofdBscanDrawInfo->pColor         = _pColor    ;


	pTofdBscanDrawInfo->nPosRecorderStart  = 0  ;
	pTofdBscanDrawInfo->nPosRecorderStop   = pTofdBscanDrawInfo->nTotalSteps - 1 ;
	pTofdBscanDrawInfo->nEncoderResolution = _pScanInfor->StepsPerResolution  ;

	if(_nRef < pTofdBscanDrawInfo->nTotalSteps)
	{
		pTofdBscanDrawInfo->nWndType   = 1  ;
	}
	else
	{
		pTofdBscanDrawInfo->nWndType   = 0  ;
	}
}

/* 画B扫描 */
// encoder type is timer
#define DATA_HANDLER_LZK(i,x) \
	{ \
        if(TMP(dataRecalling) && (TOFD == group_get_rx_tx_mode(_nGroupId))) \
		{ \
			TofdHandler(i ,_nGroupId ,x ,_nDataOffset ,_nDataSize);\
		}\
		else\
		{\
			_pData = (unsigned char*)(pStoreBuffer + i * _nDataSize + _nDataOffset)  ;\
			getDisplayData( _pData,\
				TMP(scan_data[_nGroupId]),\
				pp->nPointQty[_nGroupId],\
				x,\
				GROUP_VAL_POS(_nGroupId , rectifier1) == RF_WAVE);\
		}\
    }

void DrawBScanTimer (unsigned short *pFrame)
{
	int i ; int j ;
	int _nPosY   , _nOffset;
	unsigned char*   _pData ;

	int _nGroupId    = g_BscanDrawInfo.nGroupId   ;
	int _nOffsetX    = g_BscanDrawInfo.nOffsetX   ;
	int _nOffsetY    = g_BscanDrawInfo.nOffsetY   ;
	int _nWidth      = g_BscanDrawInfo.nWidth     ;
	int _nHeight     = g_BscanDrawInfo.nHeight    ;
	int _nIndexStart = g_BscanDrawInfo.nShowStart ;
	int _nIndexEnd   = g_BscanDrawInfo.nShowStop  ;
	int _nDataSize   = g_BscanDrawInfo.nDataSize  ;
	int _nMaxIndex   = g_BscanDrawInfo.nMaxStoreIndex ;
	int _nBeamShowQty= g_BscanDrawInfo.nShowBeamQty   ;
	int _nDataOffset = g_BscanDrawInfo.nDataOffset    ;
	_nDataOffset += TMP(beam_num[_nGroupId]) * (pp->nPointQty[_nGroupId] + 32) ;
	unsigned short* pColor =  g_BscanDrawInfo.pColor  ;
	unsigned char* pStoreBuffer = (unsigned char*)TMP(StoreBuffAddr);

	GROUP* pGroup = get_group_by_id (pp->p_config, _nGroupId);

	int _nIndex ;
//	printf("nShowStart:%d\n" ,g_BscanDrawInfo.nShowStart);
//	printf("nShowStop:%d\n" ,g_BscanDrawInfo.nShowStop);
//	printf("nShowBeamQty:%d\n" ,g_BscanDrawInfo.nShowBeamQty);
//	printf("g_nScanTimmerCounter:%d\n" ,g_nScanTimmerCounter);
//	printf("g_bScanTimmerCircled:%d\n" ,g_bScanTimmerCircled);
	if ( g_nScanTimmerCounter >= _nIndexEnd )
	{
		g_BscanDrawInfo.nShowStop  = g_nScanTimmerCounter ;
		g_BscanDrawInfo.nShowStart = g_nScanTimmerCounter - _nBeamShowQty + 1 ;
		_nIndexStart = g_BscanDrawInfo.nShowStart ;
		_nIndexEnd   = g_BscanDrawInfo.nShowStop  ;

		_nOffset = _nOffsetY + _nHeight + _nIndexStart - 1 ;
		for(i = _nIndexStart ; i <= _nIndexEnd ; i++ )
		{
			if(ScanDataMark[i])
			{
				DATA_HANDLER_LZK(i ,_nWidth);

				_nPosY = _nOffset  - i  ;
				_pData = TMP(scan_data[_nGroupId])   ;
				for (j = 0; j < _nWidth; j++)
				{
					fbdot (pFrame, _nOffsetX + j, _nPosY, pColor[_pData[j]]);
				}
			}
			else
			{
				_nPosY = _nOffset  - i  ;
				for (j = 0; j < _nWidth; j++)
				{
					fbdot (pFrame, _nOffsetX + j, _nPosY, 0 );
				}
			}
		}

	}
	else if(g_bScanTimmerCircled)
	{
		_nIndexStart =  g_nScanTimmerCounter +  _nMaxIndex - _nBeamShowQty;
		_nIndexEnd   =  g_nScanTimmerCounter +  _nMaxIndex;

		g_BscanDrawInfo.nShowStart = 0;
		g_BscanDrawInfo.nShowStop  = _nBeamShowQty - 1;

		_nOffset = _nOffsetY + _nHeight  + _nIndexStart -1 ;
		for(i = _nIndexStart ; i <= _nIndexEnd ; i++ )
		{
			_nIndex = i % _nMaxIndex ;
			if(ScanDataMark[_nIndex])
			{
				DATA_HANDLER_LZK(_nIndex ,_nWidth);

				_nPosY = _nOffset - i  ;
				_pData = TMP(scan_data[_nGroupId])   ;
				for (j = 0; j < _nWidth; j++)
				{
					fbdot (pFrame, _nOffsetX + j, _nPosY, pColor[_pData[j]]);
				}

			}
			else
			{
				_nPosY = _nOffset - i  ;
				for (j = 0; j < _nWidth; j++)
				{
					fbdot (pFrame, _nOffsetX + j, _nPosY, 0 );
				}
			}
		}
	}
	else if ( g_nScanTimmerCounter <= _nIndexStart )
	{
		g_BscanDrawInfo.nShowStart  = g_nScanTimmerCounter ;
		g_BscanDrawInfo.nShowStop   = g_nScanTimmerCounter + _nBeamShowQty - 1 ;

		_nIndexStart = g_BscanDrawInfo.nShowStart ;
		_nIndexEnd   = g_BscanDrawInfo.nShowStop  ;

		_nOffset = _nOffsetY + _nHeight + _nIndexStart - 1 ;
		for(i = _nIndexStart ; i <= _nIndexEnd ; i++ )
		{
			if(ScanDataMark[i])
			{
				DATA_HANDLER_LZK(i ,_nWidth);

				_nPosY = _nOffset  - i  ;
				_pData = TMP(scan_data[_nGroupId])   ;
				for (j = 0; j < _nWidth; j++)
				{
					fbdot (pFrame, _nOffsetX + j, _nPosY, pColor[_pData[j]]);
				}
			}
			else
			{
				_nPosY = _nOffset  - i  ;
				for (j = 0; j < _nWidth; j++)
				{
					fbdot (pFrame, _nOffsetX + j, _nPosY, 0 );
				}
			}
		}
	}
	else
	{
		_nIndexEnd   = g_BscanDrawInfo.nShowStop  ;
		_nIndexStart = g_BscanDrawInfo.nShowStart ;

		_nOffset = _nOffsetY + _nHeight  + _nIndexStart -1 ;
		for(i = _nIndexStart ; i <= _nIndexEnd ; i++ )
		{
			if(ScanDataMark[i])
			{
				DATA_HANDLER_LZK(i ,_nWidth);

				_nPosY = _nOffset - i  ;
				_pData = TMP(scan_data[_nGroupId])   ;
				for (j = 0; j < _nWidth; j++)
				{
					fbdot (pFrame, _nOffsetX + j, _nPosY, pColor[_pData[j]]);
				}

			}
			else
			{
				_nPosY = _nOffset - i  ;
				for (j = 0; j < _nWidth; j++)
				{
					fbdot (pFrame, _nOffsetX + j, _nPosY, 0 );
				}
			}
		}
	}
}
// encoder type is timer
void DrawBScanTOFDTimer (unsigned short *pFrame , int nWndIndex_)
{
	int i ; int j ;
	int _nPosX   ;
	unsigned char*   _pData ;
	TOFDB_DRAW_INFO_P pTofdBscanDrawInfo = &(g_TofdBscanDrawInfo[nWndIndex_]);
	int _nGroupId    = pTofdBscanDrawInfo->nGroupId   ;
	int _nOffsetX    = pTofdBscanDrawInfo->nOffsetX   ;
	int _nOffsetY    = pTofdBscanDrawInfo->nOffsetY   ;
	//int _nWidth      = pTofdBscanDrawInfo->nWidth     ;
	int _nHeight     = pTofdBscanDrawInfo->nHeight    ;
	int _nIndexStart = pTofdBscanDrawInfo->nShowStart ;
	int _nIndexEnd   = pTofdBscanDrawInfo->nShowStop  ;
	int _nDataSize   = pTofdBscanDrawInfo->nDataSize  ;
	int _nMaxIndex   = pTofdBscanDrawInfo->nMaxStoreIndex ;
	int _nBeamShowQty= pTofdBscanDrawInfo->nShowBeamQty   ;
	int _nDataOffset = pTofdBscanDrawInfo->nDataOffset    ;
	unsigned short* pColor =  pTofdBscanDrawInfo->pColor  ;
	unsigned char* pStoreBuffer = (unsigned char*)TMP(StoreBuffAddr);

	GROUP* pGroup = get_group_by_id (pp->p_config, _nGroupId);

	int _nIndex ;

	if ( g_nScanTimmerCounter >=_nIndexEnd )
	{
		pTofdBscanDrawInfo->nShowStop  = g_nScanTimmerCounter ;
		pTofdBscanDrawInfo->nShowStart = g_nScanTimmerCounter - _nBeamShowQty + 1 ;
		_nIndexStart = pTofdBscanDrawInfo->nShowStart ;
		_nIndexEnd   = pTofdBscanDrawInfo->nShowStop  ;

		for(i = _nIndexStart ; i <= _nIndexEnd ; i++ )
		{
			if(ScanDataMark[i])
			{
				DATA_HANDLER_LZK(i ,_nHeight);

				_nPosX = _nOffsetX  + i - _nIndexStart ;
				_pData = TMP(scan_data[_nGroupId])   ;
				for (j = 0; j < _nHeight; j++)
				{
					fbdot (pFrame, _nPosX , j + _nOffsetY , pColor[_pData[j]]);
				}
			}
			else
			{
				_nPosX = _nOffsetX  + i - _nIndexStart ;
				for (j = 0; j < _nHeight; j++)
				{
					fbdot (pFrame, _nPosX , j + _nOffsetY , 0);
				}
			}
		}

	}
	else if(g_bScanTimmerCircled)
	{
		_nIndexStart =  g_nScanTimmerCounter +  _nMaxIndex - _nBeamShowQty;
		_nIndexEnd   =  g_nScanTimmerCounter +  _nMaxIndex;

		pTofdBscanDrawInfo->nShowStart = 0;
		pTofdBscanDrawInfo->nShowStop  = _nBeamShowQty - 1;
		for(i = _nIndexStart ; i <= _nIndexEnd ; i++ )
		{
			_nIndex = i % _nMaxIndex ;
			if(ScanDataMark[_nIndex])
			{
				DATA_HANDLER_LZK(_nIndex ,_nHeight);

				_nPosX = _nOffsetX  + i - _nIndexStart ;
				_pData = TMP(scan_data[_nGroupId])   ;
				for (j = 0; j < _nHeight; j++)
				{
					fbdot (pFrame, _nPosX , j + _nOffsetY , pColor[_pData[j]]);
				}

			}
			else
			{
				_nPosX = _nOffsetX  + i - _nIndexStart ;
				for (j = 0; j < _nHeight; j++)
				{
					fbdot (pFrame, _nPosX , j + _nOffsetY , 0);
				}
			}
		}
	}
	else
	{
		_nIndexEnd   = pTofdBscanDrawInfo->nShowStop  ;
		_nIndexStart = pTofdBscanDrawInfo->nShowStart ;

		for(i = _nIndexStart ; i <= _nIndexEnd ; i++ )
		{
			if(ScanDataMark[i])
			{
				DATA_HANDLER_LZK(i ,_nHeight);

				_nPosX = _nOffsetX  + i - _nIndexStart ;
				_pData = TMP(scan_data[_nGroupId])   ;
				for (j = 0; j < _nHeight; j++)
				{
					fbdot (pFrame, _nPosX , j + _nOffsetY , pColor[_pData[j]]);
				}

			}
			else
			{
				_nPosX = _nOffsetX  + i - _nIndexStart ;
				for (j = 0; j < _nHeight; j++)
				{
					fbdot (pFrame, _nPosX , j + _nOffsetY , 0);
				}
			}
		}
	}
}

void DrawBScanTOFDTimerVertical(unsigned short *pFrame , int nWndIndex_)
{
	int i, j, k;
	int _nPosX, _nPosY;
	unsigned char*   _pData ;
	TOFDB_DRAW_INFO_P pTofdBscanDrawInfo = &(g_TofdBscanDrawInfo[nWndIndex_]);
	int _nGroupId    = pTofdBscanDrawInfo->nGroupId   ;
	int _nOffsetX    = pTofdBscanDrawInfo->nOffsetX   ;
	int _nOffsetY    = pTofdBscanDrawInfo->nOffsetY   ;
	int _nWidth      = pTofdBscanDrawInfo->nWidth     ;
	int _nHeight     = pTofdBscanDrawInfo->nHeight    ;
	int _nIndexStart = pTofdBscanDrawInfo->nShowStart ;
	int _nIndexEnd   = pTofdBscanDrawInfo->nShowStop  ;
	int _nDataSize   = pTofdBscanDrawInfo->nDataSize  ;
	int _nMaxIndex   = pTofdBscanDrawInfo->nMaxStoreIndex ;
	int _nBeamShowQty= pTofdBscanDrawInfo->nShowBeamQty   ;
	int _nDataOffset = pTofdBscanDrawInfo->nDataOffset    ;
    int _nWaveRange  = pTofdBscanDrawInfo->nWidth     ;
    int _nWaveGrow;

	unsigned short* pColor =  pTofdBscanDrawInfo->pColor  ;
	unsigned char* pStoreBuffer = (unsigned char*)TMP(StoreBuffAddr);

	GROUP* pGroup = get_group_by_id (pp->p_config, _nGroupId);

	int _nIndex ;

	if ( g_nScanTimmerCounter >=_nIndexEnd )
	{
		pTofdBscanDrawInfo->nShowStop  = g_nScanTimmerCounter ;
		pTofdBscanDrawInfo->nShowStart = g_nScanTimmerCounter - _nBeamShowQty + 1 ;
		_nIndexStart = pTofdBscanDrawInfo->nShowStart ;
		_nIndexEnd   = pTofdBscanDrawInfo->nShowStop  ;

		for(i = _nIndexStart ; i <= _nIndexEnd ; i++ )
		{
			if(ScanDataMark[i])
			{
				DATA_HANDLER_LZK(i ,_nWaveRange);
				_nPosY = _nOffsetY + _nHeight - i + _nIndexStart;
				_pData = TMP(scan_data[_nGroupId]);
				for (j = 0; j < _nWaveRange; j++)
				{
					fbdot (pFrame, _nOffsetX+j, _nPosY, pColor[_pData[j]]);
				}
			} else {
				_nPosY = _nOffsetY + _nHeight - i + _nIndexStart;
				for (j = 0; j < _nWaveRange; j++)
				{
					fbdot(pFrame, _nOffsetX+j, _nOffsetY, 0);
				}
			}
		}

	} else if(g_bScanTimmerCircled) {
		_nIndexStart =  g_nScanTimmerCounter +  _nMaxIndex - _nBeamShowQty;
		_nIndexEnd   =  g_nScanTimmerCounter +  _nMaxIndex;

		pTofdBscanDrawInfo->nShowStart = 0;
		pTofdBscanDrawInfo->nShowStop  = _nBeamShowQty - 1;
		for(i = _nIndexStart ; i <= _nIndexEnd ; i++ )
		{
			_nIndex = i % _nMaxIndex ;
			_nPosY = _nOffsetY + _nHeight - i + _nIndexStart;
			if(ScanDataMark[_nIndex])
			{
				DATA_HANDLER_LZK(_nIndex ,_nWaveRange);

				_pData = TMP(scan_data[_nGroupId])   ;
				for (j = 0; j < _nWaveRange; j++)
				{
					fbdot (pFrame, _nOffsetX+j, _nPosY, pColor[_pData[j]]);
				}

			}
			else
			{
				for (j = 0; j < _nHeight; j++)
				{
					fbdot(pFrame, _nOffsetX+j, _nOffsetY, 0);
				}
			}
		}
	} else {
		for(i = _nIndexStart, k=0; i <= _nIndexEnd ; i++, k++ )
		{
			if(ScanDataMark[i]) {
				DATA_HANDLER_LZK(i ,_nWaveRange);
			    _nPosY = _nOffsetY + _nHeight - k;
				_pData = TMP(scan_data[_nGroupId])   ;
				for (j = 0; j < _nWaveRange; j++)
				{
					fbdot (pFrame, _nOffsetX+j, _nPosY, pColor[_pData[j]]);
				}
			} else {
			    _nPosY = _nOffsetY + _nHeight - k;
				for (j = 0; j < _nWaveRange; j++)
				{
					fbdot (pFrame, _nOffsetX+j, _nPosY, 0);
				}
			}
		}
	}
}

// when beam quantity is bigger than display height
// the window should scroll sometime
void DrawBscanEncoderScrollWindow( unsigned short *p , int nDataIndex_)
{
	int i ; int j ;
	int _nPosY  , _nOffset ;
	int _nGroupId    = g_BscanDrawInfo.nGroupId   ;
	int _nOffsetX    = g_BscanDrawInfo.nOffsetX   ;
	int _nOffsetY    = g_BscanDrawInfo.nOffsetY   ;
	int _nWidth      = g_BscanDrawInfo.nWidth     ;
	int _nHeight     = g_BscanDrawInfo.nHeight    ;
	int _nIndexStart = g_BscanDrawInfo.nShowStart ;
	int _nIndexEnd   = g_BscanDrawInfo.nShowStop  ;
	int _nDataSize   = g_BscanDrawInfo.nDataSize  ;
	int _nBeamShowQty= g_BscanDrawInfo.nShowBeamQty   ;
	int _nDataOffset = g_BscanDrawInfo.nDataOffset    ;
	_nDataOffset += TMP(beam_num[_nGroupId]) * (pp->nPointQty[_nGroupId] + 32) ;
	unsigned short* pColor =  g_BscanDrawInfo.pColor  ;

	GROUP* pGroup = get_group_by_id (pp->p_config, _nGroupId);

	unsigned char* _pData ;
	unsigned char* pStoreBuffer = (unsigned char*)TMP(StoreBuffAddr);

	if ( nDataIndex_ > _nIndexEnd )
	{
		g_BscanDrawInfo.nShowStop  = nDataIndex_ ;
		g_BscanDrawInfo.nShowStart = nDataIndex_ - _nBeamShowQty  + 1 ;
	}
	else if ( nDataIndex_ < _nIndexStart )
	{
		g_BscanDrawInfo.nShowStart  = nDataIndex_ ;
		g_BscanDrawInfo.nShowStop   = nDataIndex_ + _nBeamShowQty - 1 ;
	}
	_nIndexStart = g_BscanDrawInfo.nShowStart ;
	_nIndexEnd   = g_BscanDrawInfo.nShowStop  ;

	_nOffset = _nOffsetY + _nHeight - 1 + _nIndexStart  ;
	for(i = _nIndexStart ; i <= _nIndexEnd ; i++ )
	{
		if(ScanDataMark[i])
		{
			DATA_HANDLER_LZK(i ,_nWidth);

			_nPosY =  _nOffset - i  ;
			_pData = TMP(scan_data[_nGroupId])   ;
    		for (j = 0; j < _nWidth ; j++)
    		{
    			fbdot (p, _nOffsetX + j, _nPosY, pColor[_pData[j]]);
    		}
		}
		else
		{
			_nPosY = _nOffset - i  ;
    		for (j = 0; j < _nWidth ; j++)
    		{
    			fbdot (p, _nOffsetX + j, _nPosY, 0);
    		}
		}
	}
}

void DrawBscanTOFDEncoderScrollWindow( unsigned short *pFrame , int nDataIndex_ , int nWndIndex_)
{
	int i ; int j ;
	int _nPosX   , _nOffset;
	TOFDB_DRAW_INFO_P pTofdBscanDrawInfo = &(g_TofdBscanDrawInfo[nWndIndex_]);
	int _nGroupId    = pTofdBscanDrawInfo->nGroupId   ;
	int _nOffsetX    = pTofdBscanDrawInfo->nOffsetX   ;
	int _nOffsetY    = pTofdBscanDrawInfo->nOffsetY   ;
	//int _nWidth      = pTofdBscanDrawInfo->nWidth     ;
	int _nHeight     = pTofdBscanDrawInfo->nHeight    ;
	int _nIndexStart = pTofdBscanDrawInfo->nShowStart ;
	int _nIndexEnd   = pTofdBscanDrawInfo->nShowStop  ;
	int _nDataSize   = pTofdBscanDrawInfo->nDataSize  ;
	int _nBeamShowQty= pTofdBscanDrawInfo->nShowBeamQty   ;
	int _nDataOffset = pTofdBscanDrawInfo->nDataOffset    ;
	unsigned short* pColor =  pTofdBscanDrawInfo->pColor  ;

	GROUP* pGroup = get_group_by_id (pp->p_config, _nGroupId);

	unsigned char* _pData ;
	unsigned char* pStoreBuffer = (unsigned char*)TMP(StoreBuffAddr);

	if ( nDataIndex_ > _nIndexEnd )
	{
		pTofdBscanDrawInfo->nShowStop  = nDataIndex_ ;
		pTofdBscanDrawInfo->nShowStart = nDataIndex_ - _nBeamShowQty  + 1 ;
	}
	else if ( nDataIndex_ < _nIndexStart )
	{
		pTofdBscanDrawInfo->nShowStart  = nDataIndex_ ;
		pTofdBscanDrawInfo->nShowStop   = nDataIndex_ + _nBeamShowQty - 1 ;
	}
	_nIndexStart = pTofdBscanDrawInfo->nShowStart ;
	_nIndexEnd   = pTofdBscanDrawInfo->nShowStop  ;

	_nOffset = _nOffsetX  - _nIndexStart ;
	for(i = _nIndexStart ; i <= _nIndexEnd ; i++ )
	{
		if(ScanDataMark[i])
		{
			DATA_HANDLER_LZK(i ,_nHeight);

			_nPosX = _nOffset  + i ;
			_pData = TMP(scan_data[_nGroupId])   ;
			for (j = 0; j < _nHeight; j++)
			{
				fbdot (pFrame, _nPosX , j + _nOffsetY , pColor[_pData[j]]);
			}
		}
		else
		{
			_nPosX = _nOffset  + i  ;
			for (j = 0; j < _nHeight; j++)
			{
				fbdot (pFrame, _nPosX , j + _nOffsetY , 0);
			}
		}
	}

}

void DrawBscanTOFDEncoderScrollWindowVer( unsigned short *pFrame , int nDataIndex_ , int nWndIndex_)
{

	int i, j, k;
	int _nPosX, _nPosY, _nOffset;
	TOFDB_DRAW_INFO_P pTofdBscanDrawInfo = &(g_TofdBscanDrawInfo[nWndIndex_]);
	int _nGroupId    = pTofdBscanDrawInfo->nGroupId   ;
	int _nOffsetX    = pTofdBscanDrawInfo->nOffsetX   ;
	int _nOffsetY    = pTofdBscanDrawInfo->nOffsetY   ;
	int _nWidth      = pTofdBscanDrawInfo->nWidth     ;
	int _nHeight     = pTofdBscanDrawInfo->nHeight    ;
	int _nIndexStart = pTofdBscanDrawInfo->nShowStart ;
	int _nIndexEnd   = pTofdBscanDrawInfo->nShowStop  ;
	int _nDataSize   = pTofdBscanDrawInfo->nDataSize  ;
	int _nBeamShowQty= pTofdBscanDrawInfo->nShowBeamQty   ;
	int _nDataOffset = pTofdBscanDrawInfo->nDataOffset    ;
	unsigned short* pColor =  pTofdBscanDrawInfo->pColor  ;

	GROUP* pGroup = get_group_by_id (pp->p_config, _nGroupId);

	unsigned char* _pData ;
	unsigned char* pStoreBuffer = (unsigned char*)TMP(StoreBuffAddr);

	if ( nDataIndex_ > _nIndexEnd )
	{
		pTofdBscanDrawInfo->nShowStop  = nDataIndex_ ;
		pTofdBscanDrawInfo->nShowStart = nDataIndex_ - _nBeamShowQty  + 1 ;
	}
	else if ( nDataIndex_ < _nIndexStart )
	{
		pTofdBscanDrawInfo->nShowStart  = nDataIndex_ ;
		pTofdBscanDrawInfo->nShowStop   = nDataIndex_ + _nBeamShowQty - 1 ;
	}
	_nIndexStart = pTofdBscanDrawInfo->nShowStart ;
	_nIndexEnd   = pTofdBscanDrawInfo->nShowStop  ;

	_nOffset = _nOffsetY + _nHeight ;
	for(i = _nIndexStart, k=0; i <= _nIndexEnd ; i++, k++)
	{
        _nPosY = _nOffset - k ;
		if(ScanDataMark[i])
		{
			DATA_HANDLER_LZK(i ,_nWidth);

			_pData = TMP(scan_data[_nGroupId])   ;
			for (j = 0; j < _nWidth; j++)
			{
				fbdot (pFrame, _nOffsetX + j , _nPosY , pColor[_pData[j]]);
			}
		} else {
			for (j = 0; j < _nWidth; j++)
			{
				fbdot (pFrame, _nOffsetX + j , _nPosY , 0);
			}
		}
	}
}

// when beam quantity is smaller than display height
// the specify beam is displayed in static zoom
void DrawBscanEncoderStaticWindow(unsigned short *p , int nDataIndex_)
{
	int i , j , k;
	int yyy      ;
	unsigned char* _pData ;

	unsigned char* pStoreBuffer = (unsigned char*)TMP(StoreBuffAddr);
	int _nGroupId    = g_BscanDrawInfo.nGroupId   ;
	int _nOffsetX    = g_BscanDrawInfo.nOffsetX   ;
	int _nOffsetY    = g_BscanDrawInfo.nOffsetY   ;
	int _nWidth      = g_BscanDrawInfo.nWidth     ;
	int _nHeight     = g_BscanDrawInfo.nHeight    ;
	int _nIndexStart = g_BscanDrawInfo.nShowStart ;
	int _nIndexEnd   = g_BscanDrawInfo.nShowStop  ;
	int _nDataSize   = g_BscanDrawInfo.nDataSize  ;
	int _nBeamShowQty= g_BscanDrawInfo.nShowBeamQty   ;
	int _nDataOffset = g_BscanDrawInfo.nDataOffset    ;
	_nDataOffset += TMP(beam_num[_nGroupId]) * (pp->nPointQty[_nGroupId] + 32) ;
	unsigned short* pColor =  g_BscanDrawInfo.pColor  ;

	GROUP* pGroup = get_group_by_id (pp->p_config, _nGroupId);

	_nOffsetY -= 1 ;
	for(k = 0 ; k < _nBeamShowQty ; k++)
	{
	    // draw stored beam to mapped lines
		if(ScanDataMark[k])
		{
			DATA_HANDLER_LZK(k ,_nWidth);

			_nIndexStart = k * _nHeight / _nBeamShowQty ;
			_nIndexEnd   = (k + 1) * _nHeight / _nBeamShowQty ;
			_pData = TMP(scan_data[_nGroupId]) ;
			for(i = _nIndexStart ;i < _nIndexEnd ; i++)
			{
				yyy = _nOffsetY + _nHeight - i ;
				for (j = 0; j < _nWidth ; j++)
					fbdot (p, _nOffsetX + j, yyy , pColor[_pData[j]]);
			}
		}
	}
}

void DrawBscanTOFDEncoderStaticWindow(unsigned short *p , int nDataIndex_ , int nWndIndex_)
{
    int i , j , k;
    int _nPosX      ;
    unsigned char* _pData ;
    
    unsigned char* pStoreBuffer = (unsigned char*)TMP(StoreBuffAddr);
    TOFDB_DRAW_INFO_P pTofdBscanDrawInfo = &(g_TofdBscanDrawInfo[nWndIndex_]);
    int _nGroupId    = pTofdBscanDrawInfo->nGroupId   ;
    int _nOffsetX    = pTofdBscanDrawInfo->nOffsetX   ;
    int _nOffsetY    = pTofdBscanDrawInfo->nOffsetY   ;
    int _nWidth      = pTofdBscanDrawInfo->nWidth     ;
    int _nHeight     = pTofdBscanDrawInfo->nHeight    ;
    int _nIndexStart = pTofdBscanDrawInfo->nShowStart ;
    int _nIndexEnd   = pTofdBscanDrawInfo->nShowStop  ;
    int _nDataSize   = pTofdBscanDrawInfo->nDataSize  ;
    int _nBeamShowQty= pTofdBscanDrawInfo->nShowBeamQty   ;
    int _nDataOffset = pTofdBscanDrawInfo->nDataOffset    ;
    unsigned short* pColor =  pTofdBscanDrawInfo->pColor  ;
    float drawStart=0.5, scale = ((float) _nWidth)/_nBeamShowQty, drawEnd=0.5+scale;

    GROUP* pGroup = get_group_by_id (pp->p_config, _nGroupId);
    
    for(k = 0 ; k <= _nIndexEnd; k++)
    {
        if(ScanDataMark[k])
        {
            DATA_HANDLER_LZK(k ,_nHeight);
            _pData = TMP(scan_data[_nGroupId]) ;
            for(i = drawStart; i < drawEnd; i++)
            {
                _nPosX = _nOffsetX + i;
                for (j = 0; j < _nHeight ; j++)
                    fbdot (p, _nPosX, _nOffsetY + j , pColor[_pData[j]]);
            }
        }
        drawStart += scale;
        drawEnd += scale;
    }
}

void DrawBscanTOFDEncoderVerStaticWindow(unsigned short *p , int nDataIndex_ , int nWndIndex_)
{
    int i , j , k;
    int _nPos, _nPosY;
    unsigned char* _pData ;
    
    unsigned char* pStoreBuffer = (unsigned char*)TMP(StoreBuffAddr);
    TOFDB_DRAW_INFO_P pTofdBscanDrawInfo = &(g_TofdBscanDrawInfo[nWndIndex_]);
    int _nGroupId    = pTofdBscanDrawInfo->nGroupId   ;
    int _nOffsetX    = pTofdBscanDrawInfo->nOffsetX   ;
    int _nOffsetY    = pTofdBscanDrawInfo->nOffsetY   ;
    int _nWidth      = pTofdBscanDrawInfo->nWidth     ;
    int _nHeight     = pTofdBscanDrawInfo->nHeight    ;
    int _nIndexStart = pTofdBscanDrawInfo->nShowStart ;
    int _nIndexEnd   = pTofdBscanDrawInfo->nShowStop  ;
    int _nDataSize   = pTofdBscanDrawInfo->nDataSize  ;
    int _nBeamShowQty= pTofdBscanDrawInfo->nShowBeamQty   ;
    int _nDataOffset = pTofdBscanDrawInfo->nDataOffset    ;
    unsigned short* pColor =  pTofdBscanDrawInfo->pColor  ;
    float scale = ((float) _nHeight)/_nBeamShowQty;
    float drawStart=0.5, drawEnd=0.5+scale;

    GROUP* pGroup = get_group_by_id (pp->p_config, _nGroupId);
    
    _nPosY = _nOffsetY+_nHeight;
    for(k = 0 ; k <= _nIndexEnd; k++)
    {
        if(ScanDataMark[k])
        {
            DATA_HANDLER_LZK(k ,_nWidth);
            _pData = TMP(scan_data[_nGroupId]) ;
            for(i = drawStart; i < drawEnd; i++)
            {
                _nPos = _nPosY - i;
                for (j = 0; j < _nWidth ; j++)
                    fbdot (p, _nOffsetX+j, _nPos, pColor[_pData[j]]);
            }
        }
        drawStart += scale;
        drawEnd += scale;
    }
}


//*****************************************************
//*****
//*****    C SCAN
//*****
//*****************************************************
CSCAN_DRAW_INFO  g_CscanDrawInfo[16]  ;

// initialize c can display parameters
void CScanDisplayInit(int nWndIndex_ , int nSouceType_)
{

	int _nGroupId ;
	int _nHeight , _nWidth , _nOffsetX , _nOffsetY , _bRFmode;
	int _nShowStart , _nShowStop   , _nShowBeamQty;
	unsigned short* _pColor  ;
	_nGroupId  = TMP(_scan_group[nWndIndex_])     ;
	_nOffsetX  = TMP(_scan_xpos[nWndIndex_])      ;
	_nOffsetY  = TMP(_scan_ypos[nWndIndex_])      ;
	_nWidth    = TMP(_scan_width[nWndIndex_])     ;
	_nHeight   = TMP(_scan_height[nWndIndex_])    ;

	SCAN_INFO_P _pScanInfor;
	_pScanInfor = &pp->ScanInfor ;

	_nShowStart =  0 ;
	// each beam should display at least one pixel line
	if(pp->nEncoderType)
	{
		_nShowStop  =  MIN(_nWidth , _pScanInfor->ScanStepQty)  - 1 ;
		if(_pScanInfor->ScanZeroOffset > _nShowStop / 2)
		{
			_nShowStart = _pScanInfor->ScanZeroOffset - _nShowStop / 2 ;
			_nShowStop  = _nShowStart  + _nShowStop     ;
		}
	}
	else
	{
		_nShowStop  =  _nWidth  - 1 ;
	}
	_nShowBeamQty  =  _nShowStop - _nShowStart + 1  ;

	int k ;
	int _nOffsetGroup = DMA_DATA_OFFSET ;
	for ( k = 0 ; k < _nGroupId ; k++)
	{
		_nOffsetGroup += (pp->nPointQty[k] + 32) * TMP(beam_qty[k]);
	}

	if(nSouceType_ < 2)
	{
	    if(pp->p_config->group[_nGroupId].rectifier1)
	    {
	    	 _pColor = TMP(color_amp)  ;
	    }
	    else
	    {
	    	 _pColor = TMP(color_tofd) ;
	    }
	}
	else
	{
		_pColor = TMP(color_depth) ;
	}

    if(pp->p_config->group[_nGroupId].rectifier1)
    {
    	 _bRFmode = FALSE  ;
    }
    else
    {
    	 _bRFmode = TRUE  ;
    }

    g_CscanDrawInfo[nWndIndex_].nGroupId       = _nGroupId ;
    g_CscanDrawInfo[nWndIndex_].nDataOffset    = _nOffsetGroup ;
    g_CscanDrawInfo[nWndIndex_].nDataSize      = _pScanInfor->DataSize ;
    g_CscanDrawInfo[nWndIndex_].nMaxStoreIndex = _pScanInfor->MaxStoreIndex  ;
    g_CscanDrawInfo[nWndIndex_].nTotalSteps    = _pScanInfor->ScanStepQty    ;
    g_CscanDrawInfo[nWndIndex_].nZeroOffset    = _pScanInfor->ScanZeroOffset ;
    g_CscanDrawInfo[nWndIndex_].nShowBeamQty   = _nShowBeamQty  ;
    g_CscanDrawInfo[nWndIndex_].nOffsetX       = _nOffsetX  ;
    g_CscanDrawInfo[nWndIndex_].nOffsetY       = _nOffsetY  ;
    g_CscanDrawInfo[nWndIndex_].nWidth         = _nWidth    ;
    g_CscanDrawInfo[nWndIndex_].nHeight        = _nHeight   ;
    g_CscanDrawInfo[nWndIndex_].nShowStart     = _nShowStart;
    g_CscanDrawInfo[nWndIndex_].nShowStop      = _nShowStop ;
    g_CscanDrawInfo[nWndIndex_].pColor         = _pColor    ;
    g_CscanDrawInfo[nWndIndex_].nSourceType    = nSouceType_;
    g_CscanDrawInfo[nWndIndex_].nDataType      = GROUP_VAL_POS(_nGroupId, source);
    g_CscanDrawInfo[nWndIndex_].bRFmode        = _bRFmode  ;

    g_CscanDrawInfo[nWndIndex_].nPosRecorderStart  = 0  ;
    g_CscanDrawInfo[nWndIndex_].nPosRecorderStop   = _pScanInfor->ScanStepQty - 1 ;

    g_CscanDrawInfo[nWndIndex_].nEncoderResolution = _pScanInfor->StepsPerResolution  ;

	if(_nWidth < g_CscanDrawInfo[nWndIndex_].nTotalSteps)
	{
		g_CscanDrawInfo[nWndIndex_].nWndType   = 1  ;
	}
	else
	{
		g_CscanDrawInfo[nWndIndex_].nWndType   = 0  ;
	}

	int i ; double _nCurrentAngle ;
	int _nBeamQty  = TMP(beam_qty[_nGroupId]) ;
	if(LAW_VAL_POS(_nGroupId , Focal_type) == AZIMUTHAL_SCAN)
	{
		for(i = 0 ; i< _nBeamQty ; i++)
		{
			_nCurrentAngle = LAW_VAL_POS(_nGroupId , Angle_min)/100.0 + i * LAW_VAL_POS(_nGroupId , Angle_step)/100.0 ;
			_nCurrentAngle = _nCurrentAngle * G_PI / 180.0 ;
			g_CscanDrawInfo[nWndIndex_].nCosAngleValue[i] = cos(_nCurrentAngle);
		}
	}
	else // LINEAR_SCAN
	{
		_nCurrentAngle = LAW_VAL_POS(_nGroupId , Angle_min)/100.0 ;
	    _nCurrentAngle = _nCurrentAngle * G_PI / 180.0 ;
	    _nCurrentAngle = cos(_nCurrentAngle);
		for(i = 0 ; i< _nBeamQty ; i++)
		{
			g_CscanDrawInfo[nWndIndex_].nCosAngleValue[i] = _nCurrentAngle ;
		}
	}
}

void CScanVerDisplayInit(int nWndIndex_ , int nSouceType_)
{

	int _nGroupId ;
	int _nHeight , _nWidth , _nOffsetX , _nOffsetY , _bRFmode;
	int _nShowStart , _nShowStop   , _nShowBeamQty;
	unsigned short* _pColor  ;
	_nGroupId  = TMP(_scan_group[nWndIndex_])     ;
	_nOffsetX  = TMP(_scan_xpos[nWndIndex_])      ;
	_nOffsetY  = TMP(_scan_ypos[nWndIndex_])      ;
	_nWidth    = TMP(_scan_width[nWndIndex_])     ;
	_nHeight   = TMP(_scan_height[nWndIndex_])    ;

	SCAN_INFO_P _pScanInfor;
	_pScanInfor = &pp->ScanInfor ;

	_nShowStart =  0 ;
	// each beam should display at least one pixel line
	if(pp->nEncoderType)
	{
		_nShowStop  =  MIN(_nHeight, _pScanInfor->ScanStepQty)  - 1 ;
		if(_pScanInfor->ScanZeroOffset > _nShowStop / 2)
		{
			_nShowStart = _pScanInfor->ScanZeroOffset - _nShowStop / 2 ;
			_nShowStop  = _nShowStart  + _nShowStop     ;
		}
	}
	else
	{
		_nShowStop  =  _nHeight  - 1 ;
	}
	_nShowBeamQty  =  _nShowStop - _nShowStart + 1  ;

	int k ;
	int _nOffsetGroup = DMA_DATA_OFFSET ;
	for ( k = 0 ; k < _nGroupId ; k++)
	{
		_nOffsetGroup += (pp->nPointQty[k] + 32) * TMP(beam_qty[k]);
	}

	if(nSouceType_ < 2)
	{
	    if(pp->p_config->group[_nGroupId].rectifier1)
	    {
	    	 _pColor = TMP(color_amp)  ;
	    }
	    else
	    {
	    	 _pColor = TMP(color_tofd) ;
	    }
	}
	else
	{
		_pColor = TMP(color_depth) ;
	}

    if(pp->p_config->group[_nGroupId].rectifier1)
    {
    	 _bRFmode = FALSE  ;
    }
    else
    {
    	 _bRFmode = TRUE  ;
    }

    g_CscanDrawInfo[nWndIndex_].nGroupId       = _nGroupId ;
    g_CscanDrawInfo[nWndIndex_].nDataOffset    = _nOffsetGroup ;
    g_CscanDrawInfo[nWndIndex_].nDataSize      = _pScanInfor->DataSize ;
    g_CscanDrawInfo[nWndIndex_].nMaxStoreIndex = _pScanInfor->MaxStoreIndex  ;
    g_CscanDrawInfo[nWndIndex_].nTotalSteps    = _pScanInfor->ScanStepQty    ;
    g_CscanDrawInfo[nWndIndex_].nZeroOffset    = _pScanInfor->ScanZeroOffset ;
    g_CscanDrawInfo[nWndIndex_].nShowBeamQty   = _nShowBeamQty  ;
    g_CscanDrawInfo[nWndIndex_].nOffsetX       = _nOffsetX  ;
    g_CscanDrawInfo[nWndIndex_].nOffsetY       = _nOffsetY  ;
    g_CscanDrawInfo[nWndIndex_].nWidth         = _nWidth    ;
    g_CscanDrawInfo[nWndIndex_].nHeight        = _nHeight   ;
    g_CscanDrawInfo[nWndIndex_].nShowStart     = _nShowStart;
    g_CscanDrawInfo[nWndIndex_].nShowStop      = _nShowStop ;
    g_CscanDrawInfo[nWndIndex_].pColor         = _pColor    ;
    g_CscanDrawInfo[nWndIndex_].nSourceType    = nSouceType_;
    g_CscanDrawInfo[nWndIndex_].nDataType      = GROUP_VAL_POS(_nGroupId, source);
    g_CscanDrawInfo[nWndIndex_].bRFmode        = _bRFmode  ;

    g_CscanDrawInfo[nWndIndex_].nPosRecorderStart  = 0  ;
    g_CscanDrawInfo[nWndIndex_].nPosRecorderStop   = _pScanInfor->ScanStepQty - 1 ;

    g_CscanDrawInfo[nWndIndex_].nEncoderResolution = _pScanInfor->StepsPerResolution  ;

	if(_nHeight < g_CscanDrawInfo[nWndIndex_].nTotalSteps)
	{
		g_CscanDrawInfo[nWndIndex_].nWndType   = 1  ;
	}
	else
	{
		g_CscanDrawInfo[nWndIndex_].nWndType   = 0  ;
	}

	int i ; double _nCurrentAngle ;
	int _nBeamQty  = TMP(beam_qty[_nGroupId]) ;
	if(LAW_VAL_POS(_nGroupId , Focal_type) == AZIMUTHAL_SCAN)
	{
		for(i = 0 ; i< _nBeamQty ; i++)
		{
			_nCurrentAngle = LAW_VAL_POS(_nGroupId , Angle_min)/100.0 + i * LAW_VAL_POS(_nGroupId , Angle_step)/100.0 ;
			_nCurrentAngle = _nCurrentAngle * G_PI / 180.0 ;
			g_CscanDrawInfo[nWndIndex_].nCosAngleValue[i] = cos(_nCurrentAngle);
		}
	}
	else // LINEAR_SCAN
	{
		_nCurrentAngle = LAW_VAL_POS(_nGroupId , Angle_min)/100.0 ;
	    _nCurrentAngle = _nCurrentAngle * G_PI / 180.0 ;
	    _nCurrentAngle = cos(_nCurrentAngle);
		for(i = 0 ; i< _nBeamQty ; i++)
		{
			g_CscanDrawInfo[nWndIndex_].nCosAngleValue[i] = _nCurrentAngle ;
		}
	}
}

void DrawGateAmplitude( unsigned short *pFrame, int nWndIndex_ , int nPosX_ , int nIndex_ , int nGateType_)
{
	int i , j ;
	unsigned char* pData ;
	unsigned int*  pBuff ; short _nPixValue     ;
	int _nGroupId      = g_CscanDrawInfo[nWndIndex_].nGroupId       ;
	int _nDataSize     = g_CscanDrawInfo[nWndIndex_].nDataSize      ;
	int _nGroupOffset  = g_CscanDrawInfo[nWndIndex_].nDataOffset    ;
	int _nOffsetY      = g_CscanDrawInfo[nWndIndex_].nOffsetY       ;
	int _nHeight       = g_CscanDrawInfo[nWndIndex_].nHeight        ;
	int _bRFmode       = g_CscanDrawInfo[nWndIndex_].bRFmode        ;
	unsigned short* _pColor  = g_CscanDrawInfo[nWndIndex_].pColor   ;
	int _nPosX   =   nPosX_ + g_CscanDrawInfo[nWndIndex_].nOffsetX  ;
	unsigned char* pStoreBuffer = (unsigned char*)TMP(StoreBuffAddr);
	int _nBeamQty      = TMP(beam_qty[_nGroupId])                   ;
	int _nPointQty     = pp->nPointQty[_nGroupId]                   ;
	int _nBeamDataSize = _nPointQty + 32                            ;

	pData = (unsigned char*)(pStoreBuffer +  nIndex_ * _nDataSize  + _nGroupOffset);
	for	(j = 0 ; j < _nBeamQty; j++)
	{
		pBuff = (unsigned int*)(TMP(measure_data[j])) ;
		memcpy ((void*)pBuff , (void *)(pData + _nBeamDataSize * j + _nPointQty), 16);
	}
	pBuff = (unsigned int*)(TMP(measure_data)) ;

	nGateType_ += 1 ;
	if(_bRFmode)
	{
		for (i = 0 ; i < _nHeight ; i++)
		{
			_nPixValue = pBuff[(i * _nBeamQty / _nHeight) * 8 + nGateType_] >> 16 ;
			_nPixValue = _nPixValue / 128 + 128;
			if(_nPixValue < 0) _nPixValue = 0 ;
			else if(_nPixValue > 255)    _nPixValue = 255 ;
			fbdot (pFrame, _nPosX, _nOffsetY + i , _pColor[_nPixValue]);
		}
	}
	else
	{
		for (i = 0 ; i < _nHeight ; i++)
		{
			_nPixValue = pBuff[(i * _nBeamQty / _nHeight) * 8 + nGateType_] >> 23 ;
			_nPixValue = MIN(255 , _nPixValue);
			fbdot (pFrame, _nPosX, _nOffsetY + i , _pColor[_nPixValue]);
		}
	}

}

void DrawGateHorizontalAmplitude( unsigned short *pFrame, int nWndIndex_ , int nPosX_ , int nIndex_ , int nGateType_)
{
	int i , j ;
	unsigned char* pData ;
	unsigned int*  pBuff ; short _nPixValue     ;
	int _nGroupId      = g_CscanDrawInfo[nWndIndex_].nGroupId       ;
	int _nDataSize     = g_CscanDrawInfo[nWndIndex_].nDataSize      ;
	int _nGroupOffset  = g_CscanDrawInfo[nWndIndex_].nDataOffset    ;
	int _nOffsetY      = g_CscanDrawInfo[nWndIndex_].nOffsetY       ;
	int _nOffsetX      = g_CscanDrawInfo[nWndIndex_].nOffsetX       ;
	int _nHeight       = g_CscanDrawInfo[nWndIndex_].nHeight        ;
	int _nWidth       = g_CscanDrawInfo[nWndIndex_].nWidth        ;
	int _bRFmode       = g_CscanDrawInfo[nWndIndex_].bRFmode        ;
	unsigned short* _pColor  = g_CscanDrawInfo[nWndIndex_].pColor   ;
	unsigned char* pStoreBuffer = (unsigned char*)TMP(StoreBuffAddr);
	int _nBeamQty      = TMP(beam_qty[_nGroupId])                   ;
	int _nPointQty     = pp->nPointQty[_nGroupId]                   ;
	int _nBeamDataSize = _nPointQty + 32                            ;
    float step, index;
    int _nPosY = g_CscanDrawInfo[nWndIndex_].nOffsetY + g_CscanDrawInfo[nWndIndex_].nHeight - nPosX_;

	pData = (unsigned char*)(pStoreBuffer +  nIndex_ * _nDataSize  + _nGroupOffset);
	for	(j = 0 ; j < _nBeamQty; j++)
	{
		pBuff = (unsigned int*)(TMP(measure_data[j])) ;
		memcpy ((void*)pBuff , (void *)(pData + _nBeamDataSize * j + _nPointQty), 16);
	}
	pBuff = (unsigned int*)(TMP(measure_data)) ;

	nGateType_ += 1 ;
	if(_bRFmode)
	{
		for (i = 0 ; i < _nWidth ; i++)
		{
			_nPixValue = pBuff[(i * _nBeamQty / _nWidth) * 8 + nGateType_] >> 16 ;
			_nPixValue = _nPixValue / 128 + 128;
			if(_nPixValue < 0) _nPixValue = 0 ;
			else if(_nPixValue > 255)    _nPixValue = 255 ;
			fbdot (pFrame, _nOffsetX+i, _nHeight+_nOffsetY-nPosX_, _pColor[_nPixValue]);
		}
	}
	else
	{
#if 0
		for (i = 0 ; i < _nWidth ; i++)
		{
			_nPixValue = pBuff[(i * _nBeamQty / _nWidth) * 8 + nGateType_] >> 23 ;
			_nPixValue = MIN(255 , _nPixValue);
			//fbdot (pFrame, _nPosX, _nOffsetY + i , _pColor[_nPixValue]);
			fbdot (pFrame, _nOffsetX+i, _nPosY, _pColor[_nPixValue]);
		}
#else
        step = ((float) _nBeamQty)/_nWidth;
		for (i = 0, index = 0; i < _nWidth; i++, index += step)
		{
			_nPixValue = pBuff[((int)index) * 8 + nGateType_] >> 23 ;
			_nPixValue = MIN(255 , _nPixValue);
			fbdot (pFrame, _nOffsetX+i, _nPosY, _pColor[_nPixValue]);
		}
#endif
	}
}

void DrawGatePosition( unsigned short *pFrame, int nWndIndex_ , int nPosX_ , int nIndex_ , int nGateType_)
{
	int i , j ;
	unsigned char* pData ;
	unsigned int*  pBuff ; short _nPixValue     ;
	double _nVelocity    ; double _nThicknessMin  ;  double _nThicknessMax  ;
	int _nWedgeDelay ; int _nGateHeight ;
	int _nTmpBeamNo  , _nTmpValue  ; double _nGateAmplitude   , _nResult;
	int _nGroupId      = g_CscanDrawInfo[nWndIndex_].nGroupId       ;
	int _nDataSize     = g_CscanDrawInfo[nWndIndex_].nDataSize      ;
	int _nGroupOffset  = g_CscanDrawInfo[nWndIndex_].nDataOffset    ;
	int _nOffsetY      = g_CscanDrawInfo[nWndIndex_].nOffsetY       ;
	int _nHeight       = g_CscanDrawInfo[nWndIndex_].nHeight        ;
	int _bRFmode       = g_CscanDrawInfo[nWndIndex_].bRFmode        ;
	unsigned short* _pColor  = g_CscanDrawInfo[nWndIndex_].pColor   ;
	int _nPosX   =   nPosX_ + g_CscanDrawInfo[nWndIndex_].nOffsetX  ;
	unsigned char* pStoreBuffer = (unsigned char*)TMP(StoreBuffAddr);
	int _nBeamQty      = TMP(beam_qty[_nGroupId])                   ;
	int _nPointQty     = pp->nPointQty[_nGroupId]                   ;
	int _nBeamDataSize = _nPointQty + 32                            ;

	pData = (unsigned char*)(pStoreBuffer +  nIndex_ * _nDataSize  + _nGroupOffset);
	for	(j = 0 ; j < _nBeamQty; j++)
	{
		pBuff = (unsigned int*)(TMP(measure_data[j])) ;
		memcpy ((void*)pBuff , (void *)(pData + _nBeamDataSize * j + _nPointQty), 16);
	}
	pBuff = (unsigned int*)(TMP(measure_data)) ;

	_nVelocity = GROUP_VAL_POS( _nGroupId , velocity ) / 100.0 ;
	_nThicknessMin = GROUP_VAL_POS(_nGroupId , min_thickness) / 1000.0 ;
	_nThicknessMax = GROUP_VAL_POS(_nGroupId , max_thickness) / 1000.0 ;
	_nWedgeDelay   = GROUP_VAL_POS(_nGroupId , wedge_delay) / 10 + GROUP_VAL_POS(_nGroupId , pulser_width1)/(10 * PW_DIV);

	_nGateHeight   = GROUP_VAL_POS(_nGroupId , gate[nGateType_].height) ;
	nGateType_ += 1 ;
	_nThicknessMin = 200000 * _nThicknessMin / _nVelocity   ;
	_nThicknessMax = 200000 * _nThicknessMax / _nVelocity   ;
	double _nThicknessSpace = _nThicknessMax - _nThicknessMin  ;
    for (i = 0 ; i < _nHeight ; i++)
	{
    	_nTmpBeamNo = i * _nBeamQty / _nHeight ;
    	_nTmpValue  = pBuff[_nTmpBeamNo * 8 + nGateType_]  ;
    	if (_bRFmode)
    	{
    		_nGateAmplitude = (signed short)(_nTmpValue >> 16) ;
    		_nGateAmplitude = _nGateAmplitude / (10.24 * 16);
    		_nGateAmplitude = fabs(_nGateAmplitude);
    	}
    	else
    	{
    		_nGateAmplitude  = ((_nTmpValue >> 20 ) & 0x00000fff) / 20.47;
    	}
    	if(_nGateAmplitude < _nGateHeight)
    	{
    		_nPixValue = 0 ;
    	}
    	else
    	{
    		_nResult = (_nTmpValue & 0xfffff) - GROUP_VAL_POS(_nGroupId , beam_delay[_nTmpBeamNo]) / 10 - _nWedgeDelay ;
    		//************  补偿角度
    		_nResult *= g_CscanDrawInfo[nWndIndex_].nCosAngleValue[_nTmpBeamNo] ;
    		if(_nResult < _nThicknessMin)
				_nResult = 0 ;
            else if(_nResult > _nThicknessMax)
				_nPixValue = 255 ;
            else
				_nPixValue = 255 * (_nResult - _nThicknessMin) / _nThicknessSpace ;
    	}
		fbdot (pFrame, _nPosX, _nOffsetY + i , _pColor[_nPixValue]);
	}
}

void DrawGatePositionHorizontal( unsigned short *pFrame, int nWndIndex_ , int nPosX_ , int nIndex_ , int nGateType_)
{
	int i , j ;
	unsigned char* pData ;
	unsigned int*  pBuff ; short _nPixValue     ;
	double _nVelocity    ; double _nThicknessMin  ;  double _nThicknessMax  ;
	int _nWedgeDelay ; int _nGateHeight ;
	int _nTmpBeamNo  , _nTmpValue  ; double _nGateAmplitude   , _nResult;
	int _nGroupId      = g_CscanDrawInfo[nWndIndex_].nGroupId       ;
	int _nDataSize     = g_CscanDrawInfo[nWndIndex_].nDataSize      ;
	int _nGroupOffset  = g_CscanDrawInfo[nWndIndex_].nDataOffset    ;
	int _nOffsetX      = g_CscanDrawInfo[nWndIndex_].nOffsetX       ;
	int _nOffsetY      = g_CscanDrawInfo[nWndIndex_].nOffsetY       ;
	int _nWidth        = g_CscanDrawInfo[nWndIndex_].nWidth        ;
	int _bRFmode       = g_CscanDrawInfo[nWndIndex_].bRFmode        ;
	unsigned short* _pColor  = g_CscanDrawInfo[nWndIndex_].pColor   ;
	int _nPosY   = g_CscanDrawInfo[nWndIndex_].nOffsetY +
        g_CscanDrawInfo[nWndIndex_].nHeight -nPosX_  ;
	unsigned char* pStoreBuffer = (unsigned char*)TMP(StoreBuffAddr);
	int _nBeamQty      = TMP(beam_qty[_nGroupId])                   ;
	int _nPointQty     = pp->nPointQty[_nGroupId]                   ;
	int _nBeamDataSize = _nPointQty + 32                            ;

	pData = (unsigned char*)(pStoreBuffer +  nIndex_ * _nDataSize  + _nGroupOffset);
	for	(j = 0 ; j < _nBeamQty; j++)
	{
		pBuff = (unsigned int*)(TMP(measure_data[j])) ;
		memcpy ((void*)pBuff , (void *)(pData + _nBeamDataSize * j + _nPointQty), 16);
	}
	pBuff = (unsigned int*)(TMP(measure_data)) ;

	_nVelocity = GROUP_VAL_POS( _nGroupId , velocity ) / 100.0 ;
	_nThicknessMin = GROUP_VAL_POS(_nGroupId , min_thickness) / 1000.0 ;
	_nThicknessMax = GROUP_VAL_POS(_nGroupId , max_thickness) / 1000.0 ;
	_nWedgeDelay   = GROUP_VAL_POS(_nGroupId , wedge_delay) / 10 + GROUP_VAL_POS(_nGroupId , pulser_width1)/(10 * PW_DIV);

	_nGateHeight   = GROUP_VAL_POS(_nGroupId , gate[nGateType_].height) ;
	nGateType_ += 1 ;
	_nThicknessMin = 200000 * _nThicknessMin / _nVelocity   ;
	_nThicknessMax = 200000 * _nThicknessMax / _nVelocity   ;
	double _nThicknessSpace = _nThicknessMax - _nThicknessMin  ;
    for (i = 0 ; i < _nWidth ; i++)
	{
    	_nTmpBeamNo = i * _nBeamQty / _nWidth ;
    	_nTmpValue  = pBuff[_nTmpBeamNo * 8 + nGateType_]  ;
    	if(_bRFmode)
    	{
    		_nGateAmplitude = (signed short)(_nTmpValue >> 16) ;
    		_nGateAmplitude = _nGateAmplitude / (10.24 * 16);
    		_nGateAmplitude = fabs(_nGateAmplitude);
    	}
    	else
    	{
    		_nGateAmplitude  = ((_nTmpValue >> 20 ) & 0x00000fff) / 20.47;
    	}
    	if(_nGateAmplitude < _nGateHeight)
    	{
    		_nPixValue = 0 ;
    	}
    	else
    	{
    		_nResult = (_nTmpValue & 0xfffff) - GROUP_VAL_POS(_nGroupId , beam_delay[_nTmpBeamNo]) / 10 - _nWedgeDelay ;
    		//************  补偿角度
    		_nResult *= g_CscanDrawInfo[nWndIndex_].nCosAngleValue[_nTmpBeamNo] ;
    		if(_nResult < _nThicknessMin)
				_nResult = 0 ;
			else if(_nResult > _nThicknessMax)
				_nPixValue = 255 ;
			else
				_nPixValue = 255 * (_nResult - _nThicknessMin) / _nThicknessSpace ;
    	}
		fbdot (pFrame, _nOffsetX+i, _nPosY, _pColor[_nPixValue]);
	}
}

void DrawGatePositionDistance( unsigned short *pFrame, int nWndIndex_ , int nPosX_ , int nIndex_ , int nGateType_1 , int nGateType_2 )
{
	int i , j ;
	unsigned char* pData ;
	unsigned int*  pBuff ; short _nPixValue     ;
	double _nVelocity  ; double _nThicknessMin  ;  double _nThicknessMax  ;
	int _nWedgeDelay ; int _nGateHeight1 , _nGateHeight2 ;
	int _nTmpBeamNo  , _nTmpValue1 , _nTmpValue2  ; double _nGateAmplitude1 , _nGateAmplitude2   , _nResult1 , _nResult2;
	int _nGroupId      = g_CscanDrawInfo[nWndIndex_].nGroupId       ;
	int _nDataSize     = g_CscanDrawInfo[nWndIndex_].nDataSize      ;
	int _nGroupOffset  = g_CscanDrawInfo[nWndIndex_].nDataOffset    ;
	int _nOffsetY      = g_CscanDrawInfo[nWndIndex_].nOffsetY       ;
	int _nHeight       = g_CscanDrawInfo[nWndIndex_].nHeight        ;
	int _bRFmode       = g_CscanDrawInfo[nWndIndex_].bRFmode        ;
	unsigned short* _pColor  = g_CscanDrawInfo[nWndIndex_].pColor   ;
	int _nPosX   =   nPosX_ + g_CscanDrawInfo[nWndIndex_].nOffsetX  ;
	unsigned char* pStoreBuffer = (unsigned char*)TMP(StoreBuffAddr);
	int _nBeamQty      = TMP(beam_qty[_nGroupId])                   ;
	int _nPointQty     = pp->nPointQty[_nGroupId]                   ;
	int _nBeamDataSize = _nPointQty + 32                            ;

	pData = (unsigned char*)(pStoreBuffer +  nIndex_ * _nDataSize  + _nGroupOffset);
	for	(j = 0 ; j < _nBeamQty; j++)
	{
		pBuff = (unsigned int*)(TMP(measure_data[j])) ;
		memcpy ((void*)pBuff , (void *)(pData + _nBeamDataSize * j + _nPointQty), 16);
	}
	pBuff = (unsigned int*)(TMP(measure_data)) ;

	_nVelocity = GROUP_VAL_POS( _nGroupId , velocity ) / 100.0 ;
	_nThicknessMin = GROUP_VAL_POS(_nGroupId , min_thickness) / 1000.0 ;
	_nThicknessMax = GROUP_VAL_POS(_nGroupId , max_thickness) / 1000.0 ;
	_nWedgeDelay   = GROUP_VAL_POS(_nGroupId , wedge_delay) / 10 + GROUP_VAL_POS(_nGroupId , pulser_width1)/(10 * PW_DIV);

	_nGateHeight1   = GROUP_VAL_POS(_nGroupId , gate[nGateType_1].height) ;
	_nGateHeight2   = GROUP_VAL_POS(_nGroupId , gate[nGateType_2].height) ;
	nGateType_1 += 1 ;
	nGateType_2 += 1 ;
	_nThicknessMin = 200000 * _nThicknessMin / _nVelocity   ;
	_nThicknessMax = 200000 * _nThicknessMax / _nVelocity   ;
	double _nThicknessSpace = _nThicknessMax - _nThicknessMin  ;
    for (i = 0 ; i < _nHeight ; i++)
	{
    	_nTmpBeamNo = i * _nBeamQty / _nHeight ;
    	_nTmpValue1  = pBuff[_nTmpBeamNo * 8 + nGateType_1]  ;
    	_nTmpValue2  = pBuff[_nTmpBeamNo * 8 + nGateType_2]  ;
    	if(_bRFmode)
    	{
    		_nGateAmplitude1 = (signed short)(_nTmpValue1 >> 16) ;
    		_nGateAmplitude1 = _nGateAmplitude1 / (10.24 * 16);
    		_nGateAmplitude1 = fabs(_nGateAmplitude1);
    		_nGateAmplitude2 = (signed short)(_nTmpValue2 >> 16) ;
    		_nGateAmplitude2 = _nGateAmplitude2 / (10.24 * 16);
    		_nGateAmplitude2 = fabs(_nGateAmplitude2);
    	}
    	else
    	{
    		_nGateAmplitude1  = ((_nTmpValue1 >> 20 ) & 0x00000fff) / 20.47;
    		_nGateAmplitude2  = ((_nTmpValue2 >> 20 ) & 0x00000fff) / 20.47;
    	}
    	if((_nGateAmplitude1 < _nGateHeight1) || (_nGateAmplitude2 < _nGateHeight2))
    	{
    		_nPixValue = 0 ;
    	}
    	else
    	{
    		_nResult1 = (_nTmpValue1 & 0xfffff) - GROUP_VAL_POS(_nGroupId , beam_delay[_nTmpBeamNo]) / 10 - _nWedgeDelay ;
    		_nResult2 = (_nTmpValue2 & 0xfffff) - GROUP_VAL_POS(_nGroupId , beam_delay[_nTmpBeamNo]) / 10 - _nWedgeDelay ;
    		_nResult1 = fabs(_nResult1 - _nResult2);
    		//************  补偿角度
    		_nResult1 *= g_CscanDrawInfo[nWndIndex_].nCosAngleValue[_nTmpBeamNo] ;
    		if(_nResult1 < _nThicknessMin)
				_nPixValue = 0 ;
			else if(_nResult1 > _nThicknessMax)
				_nPixValue = 255 ;
			else
				_nPixValue = 255 * (_nResult1 - _nThicknessMin) / _nThicknessSpace ;
    	}
		fbdot (pFrame, _nPosX, _nOffsetY + i , _pColor[_nPixValue]);
	}

}

void DrawGatePositionHorizontalDistance( unsigned short *pFrame, int nWndIndex_ , int nPosX_ , int nIndex_ , int nGateType_1 , int nGateType_2 )
{
	int i , j ;
	unsigned char* pData ;
	unsigned int*  pBuff ; short _nPixValue     ;
	double _nVelocity  ; double _nThicknessMin  ;  double _nThicknessMax  ;
	int _nWedgeDelay ; int _nGateHeight1 , _nGateHeight2 ;
	int _nTmpBeamNo  , _nTmpValue1 , _nTmpValue2  ; double _nGateAmplitude1 , _nGateAmplitude2   , _nResult1 , _nResult2;
	int _nGroupId      = g_CscanDrawInfo[nWndIndex_].nGroupId       ;
	int _nDataSize     = g_CscanDrawInfo[nWndIndex_].nDataSize      ;
	int _nGroupOffset  = g_CscanDrawInfo[nWndIndex_].nDataOffset    ;
	int _nOffsetX      = g_CscanDrawInfo[nWndIndex_].nOffsetX       ;
	int _nOffsetY      = g_CscanDrawInfo[nWndIndex_].nOffsetY       ;
	int _nHeight       = g_CscanDrawInfo[nWndIndex_].nHeight        ;
	int _nWidth        = g_CscanDrawInfo[nWndIndex_].nWidth         ;
	int _bRFmode       = g_CscanDrawInfo[nWndIndex_].bRFmode        ;
	unsigned short* _pColor  = g_CscanDrawInfo[nWndIndex_].pColor   ;
	int _nPosY   = g_CscanDrawInfo[nWndIndex_].nOffsetY + g_CscanDrawInfo[nWndIndex_].nHeight - nPosX_;
	unsigned char* pStoreBuffer = (unsigned char*)TMP(StoreBuffAddr);
	int _nBeamQty      = TMP(beam_qty[_nGroupId])                   ;
	int _nPointQty     = pp->nPointQty[_nGroupId]                   ;
	int _nBeamDataSize = _nPointQty + 32                            ;

	pData = (unsigned char*)(pStoreBuffer +  nIndex_ * _nDataSize  + _nGroupOffset);
	for	(j = 0 ; j < _nBeamQty; j++)
	{
		pBuff = (unsigned int*)(TMP(measure_data[j])) ;
		memcpy ((void*)pBuff , (void *)(pData + _nBeamDataSize * j + _nPointQty), 16);
	}
	pBuff = (unsigned int*)(TMP(measure_data)) ;

	_nVelocity = GROUP_VAL_POS( _nGroupId , velocity ) / 100.0 ;
	_nThicknessMin = GROUP_VAL_POS(_nGroupId , min_thickness) / 1000.0 ;
	_nThicknessMax = GROUP_VAL_POS(_nGroupId , max_thickness) / 1000.0 ;
	_nWedgeDelay   = GROUP_VAL_POS(_nGroupId , wedge_delay) / 10 + GROUP_VAL_POS(_nGroupId , pulser_width1)/(10 * PW_DIV);

	_nGateHeight1   = GROUP_VAL_POS(_nGroupId , gate[nGateType_1].height) ;
	_nGateHeight2   = GROUP_VAL_POS(_nGroupId , gate[nGateType_2].height) ;
	nGateType_1 += 1 ;
	nGateType_2 += 1 ;
	_nThicknessMin = 200000 * _nThicknessMin / _nVelocity   ;
	_nThicknessMax = 200000 * _nThicknessMax / _nVelocity   ;
	double _nThicknessSpace = _nThicknessMax - _nThicknessMin  ;
    for (i = 0 ; i < _nWidth ; i++)
	{
    	_nTmpBeamNo = i * _nBeamQty / _nWidth ;
    	_nTmpValue1  = pBuff[_nTmpBeamNo * 8 + nGateType_1]  ;
    	_nTmpValue2  = pBuff[_nTmpBeamNo * 8 + nGateType_2]  ;
    	if(_bRFmode)
    	{
    		_nGateAmplitude1 = (signed short)(_nTmpValue1 >> 16) ;
    		_nGateAmplitude1 = _nGateAmplitude1 / (10.24 * 16);
    		_nGateAmplitude1 = fabs(_nGateAmplitude1);
    		_nGateAmplitude2 = (signed short)(_nTmpValue2 >> 16) ;
    		_nGateAmplitude2 = _nGateAmplitude2 / (10.24 * 16);
    		_nGateAmplitude2 = fabs(_nGateAmplitude2);
    	}
    	else
    	{
    		_nGateAmplitude1  = ((_nTmpValue1 >> 20 ) & 0x00000fff) / 20.47;
    		_nGateAmplitude2  = ((_nTmpValue2 >> 20 ) & 0x00000fff) / 20.47;
    	}
    	if((_nGateAmplitude1 < _nGateHeight1) || (_nGateAmplitude2 < _nGateHeight2))
    	{
    		_nPixValue = 0 ;
    	}
    	else
    	{
    		_nResult1 = (_nTmpValue1 & 0xfffff) - GROUP_VAL_POS(_nGroupId , beam_delay[_nTmpBeamNo]) / 10 - _nWedgeDelay ;
    		_nResult2 = (_nTmpValue2 & 0xfffff) - GROUP_VAL_POS(_nGroupId , beam_delay[_nTmpBeamNo]) / 10 - _nWedgeDelay ;
    		_nResult1 = fabs(_nResult1 - _nResult2);
    		//************  补偿角度
    		_nResult1 *= g_CscanDrawInfo[nWndIndex_].nCosAngleValue[_nTmpBeamNo] ;
    		if(_nResult1 < _nThicknessMin)
				_nPixValue = 0 ;
			else if(_nResult1 > _nThicknessMax)
				_nPixValue = 255 ;
			else
				_nPixValue = 255 * (_nResult1 - _nThicknessMin) / _nThicknessSpace ;
    	}
		//fbdot (pFrame, _nPosX, _nOffsetY + i , _pColor[_nPixValue]);
		fbdot (pFrame, _nOffsetX+i, _nPosY, _pColor[_nPixValue]);
	}

}

void DrawVerticalLineCscan( unsigned short *pFrame, int nWndIndex_ , int nPosX_ , int nIndex_)
{
	if(ScanDataMark[nIndex_])
	{
	    switch (g_CscanDrawInfo[nWndIndex_].nSourceType)
	    {
			case 0: //%a
				DrawGateAmplitude( pFrame, nWndIndex_ ,  nPosX_ ,  nIndex_ ,  GATE_A);
				break;
			case 1: //%b
				DrawGateAmplitude( pFrame, nWndIndex_ ,  nPosX_ ,  nIndex_ ,  GATE_B);
				break;
			case 2: //thickness
				switch(g_CscanDrawInfo[nWndIndex_].nDataType)
				{
				case 0:  //a^
					DrawGatePosition( pFrame, nWndIndex_ , nPosX_ , nIndex_ , GATE_A) ;
					break;

				case 3: // b^
					DrawGatePosition( pFrame, nWndIndex_ , nPosX_ , nIndex_ , GATE_B) ;
					break;
				case 7: // i^
				case 8: // i/
					DrawGatePosition( pFrame, nWndIndex_ , nPosX_ , nIndex_ , GATE_I) ;
					break;
				case 1: // a^ - i^
				case 2: // a^ - i/
					DrawGatePositionDistance( pFrame, nWndIndex_ , nPosX_ , nIndex_ , GATE_A , GATE_I) ;
					break;
				case 4: // b^ - i^
				case 5: // b^ - i/
					DrawGatePositionDistance( pFrame, nWndIndex_ , nPosX_ , nIndex_ , GATE_B , GATE_I) ;
					break;
				case 6: // b^ - a^
					DrawGatePositionDistance( pFrame, nWndIndex_ , nPosX_ , nIndex_ , GATE_B , GATE_A) ;
					break;
				default:
					break;
				}
				break;
			case 3:
				DrawGatePosition( pFrame, nWndIndex_ , nPosX_ , nIndex_ , GATE_I) ;
				break;
			default:
				break;

	    };
	}
    else
    {
    	int _nOffsetY      = g_CscanDrawInfo[nWndIndex_].nOffsetY       ;
    	int _nHeight       = g_CscanDrawInfo[nWndIndex_].nHeight        ;
    	int _nPosX   =   nPosX_ + g_CscanDrawInfo[nWndIndex_].nOffsetX  ;
    	fbliney(pFrame, _nPosX , _nOffsetY, _nOffsetY + _nHeight , 0) ;
    }
}

void DrawHorizontalLineCscan( unsigned short *pFrame, int nWndIndex_ , int nPosX_ , int nIndex_)
{
	if(ScanDataMark[nIndex_])
	{
	    switch (g_CscanDrawInfo[nWndIndex_].nSourceType)
	    {
			case 0: //%a
				DrawGateHorizontalAmplitude( pFrame, nWndIndex_ ,  nPosX_ ,  nIndex_ ,  GATE_A);
				break;
			case 1: //%b
				DrawGateHorizontalAmplitude( pFrame, nWndIndex_ ,  nPosX_ ,  nIndex_ ,  GATE_B);
				break;
			case 2: //thickness
				switch(g_CscanDrawInfo[nWndIndex_].nDataType)
				{
				case 0:  //a^
					DrawGatePositionHorizontal( pFrame, nWndIndex_ , nPosX_ , nIndex_ , GATE_A) ;
					break;

				case 3: // b^
					DrawGatePositionHorizontal( pFrame, nWndIndex_ , nPosX_ , nIndex_ , GATE_B) ;
					break;
				case 7: // i^
				case 8: // i/
					DrawGatePositionHorizontal( pFrame, nWndIndex_ , nPosX_ , nIndex_ , GATE_I) ;
					break;
				case 1: // a^ - i^
				case 2: // a^ - i/
					DrawGatePositionHorizontalDistance( pFrame, nWndIndex_ , nPosX_ , nIndex_ , GATE_A , GATE_I) ;
					break;
				case 4: // b^ - i^
				case 5: // b^ - i/
					DrawGatePositionHorizontalDistance( pFrame, nWndIndex_ , nPosX_ , nIndex_ , GATE_B , GATE_I) ;
					break;
				case 6: // b^ - a^
					DrawGatePositionHorizontalDistance( pFrame, nWndIndex_ , nPosX_ , nIndex_ , GATE_B , GATE_A) ;
					break;
				default:
					break;
				}
				break;
			case 3:
				DrawGatePositionHorizontal( pFrame, nWndIndex_ , nPosX_ , nIndex_ , GATE_I) ;
				break;
			default:
				break;

	    };
	}
    else
    {
	    int _nOffsetX      = g_CscanDrawInfo[nWndIndex_].nOffsetX       ;
    	int _nOffsetY      = g_CscanDrawInfo[nWndIndex_].nOffsetY       ;
    	int _nHeight       = g_CscanDrawInfo[nWndIndex_].nHeight        ;
	    int _nWidth       = g_CscanDrawInfo[nWndIndex_].nWidth        ;
        fblinex(pFrame, _nOffsetY+_nHeight-nPosX_ , _nOffsetX, _nOffsetX+_nWidth, 0);
    }
}

// draw c scan when inspect source is timer
void CScanDisplayTimer ( unsigned short *pFrame, int nWndIndex_)
{
	int i ;
	int _nPos   ;
	int _nIndexStart   = g_CscanDrawInfo[nWndIndex_].nShowStart;
	int _nIndexEnd     = g_CscanDrawInfo[nWndIndex_].nShowStop ;
	int _nMaxIndex     = g_CscanDrawInfo[nWndIndex_].nMaxStoreIndex ;
	int _nBeamShowQty  = g_CscanDrawInfo[nWndIndex_].nShowBeamQty   ;

	if ( g_nScanTimmerCounter >= _nIndexEnd )
	{
		g_CscanDrawInfo[nWndIndex_].nShowStop  = g_nScanTimmerCounter ;
		g_CscanDrawInfo[nWndIndex_].nShowStart = g_nScanTimmerCounter - _nBeamShowQty + 1 ;
		_nIndexStart = g_CscanDrawInfo[nWndIndex_].nShowStart ;
		_nIndexEnd   = g_CscanDrawInfo[nWndIndex_].nShowStop  ;
		for(i = _nIndexStart ; i <= _nIndexEnd ; i++ )
		{
			_nPos = i - _nIndexStart ;
			DrawVerticalLineCscan(pFrame, nWndIndex_ , _nPos , i) ;
		}
	}
	else if(g_bScanTimmerCircled)
	{
		int _nIndex ;
		g_CscanDrawInfo[nWndIndex_].nShowStop  = _nBeamShowQty - 1;
		g_CscanDrawInfo[nWndIndex_].nShowStart = 0;

		_nIndexStart =  g_nScanTimmerCounter +  _nMaxIndex - _nBeamShowQty ;
		_nIndexEnd   =  g_nScanTimmerCounter +  _nMaxIndex ;
		for(i = _nIndexStart ; i <= _nIndexEnd ; i++ )
		{
			_nIndex = i % _nMaxIndex ;
			_nPos = i - _nIndexStart ;
			DrawVerticalLineCscan(pFrame, nWndIndex_ , _nPos , _nIndex) ;
		}
	}
	else if ( g_nScanTimmerCounter < _nIndexStart )
	{
		g_CscanDrawInfo[nWndIndex_].nShowStop  = g_nScanTimmerCounter + _nBeamShowQty - 1;
		g_CscanDrawInfo[nWndIndex_].nShowStart = g_nScanTimmerCounter  ;
		_nIndexStart = g_CscanDrawInfo[nWndIndex_].nShowStart ;
		_nIndexEnd   = g_CscanDrawInfo[nWndIndex_].nShowStop  ;
		for(i = _nIndexStart ; i <= _nIndexEnd ; i++ )
		{
			_nPos = i - _nIndexStart ;
			DrawVerticalLineCscan(pFrame, nWndIndex_ , _nPos , i) ;
		}
	}
	else
	{
		_nIndexStart = g_CscanDrawInfo[nWndIndex_].nShowStart ;
		_nIndexEnd   = g_CscanDrawInfo[nWndIndex_].nShowStop  ;
		for(i = _nIndexStart ; i <= _nIndexEnd ; i++ )
		{
			_nPos = i - _nIndexStart ;
			DrawVerticalLineCscan(pFrame, nWndIndex_ , _nPos , i) ;
		}
	}
}

void CScanVertDisplayTimer ( unsigned short *pFrame, int nWndIndex_)
{
	int i ;
	int _nPos   ;
	int _nIndexStart   = g_CscanDrawInfo[nWndIndex_].nShowStart;
	int _nIndexEnd     = g_CscanDrawInfo[nWndIndex_].nShowStop ;
	int _nMaxIndex     = g_CscanDrawInfo[nWndIndex_].nMaxStoreIndex ;
	int _nBeamShowQty  = g_CscanDrawInfo[nWndIndex_].nShowBeamQty   ;

	if ( g_nScanTimmerCounter >= _nIndexEnd )
	{
		g_CscanDrawInfo[nWndIndex_].nShowStop  = g_nScanTimmerCounter ;
		g_CscanDrawInfo[nWndIndex_].nShowStart = g_nScanTimmerCounter - _nBeamShowQty + 1 ;
		_nIndexStart = g_CscanDrawInfo[nWndIndex_].nShowStart ;
		_nIndexEnd   = g_CscanDrawInfo[nWndIndex_].nShowStop  ;
		for(i = _nIndexStart ; i <= _nIndexEnd ; i++ )
		{
			_nPos = i - _nIndexStart ;
			DrawHorizontalLineCscan(pFrame, nWndIndex_ , _nPos , i) ;
		}
	}
	else if(g_bScanTimmerCircled)
	{
		int _nIndex ;
		g_CscanDrawInfo[nWndIndex_].nShowStop  = _nBeamShowQty - 1;
		g_CscanDrawInfo[nWndIndex_].nShowStart = 0;

		_nIndexStart =  g_nScanTimmerCounter +  _nMaxIndex - _nBeamShowQty ;
		_nIndexEnd   =  g_nScanTimmerCounter +  _nMaxIndex ;
		for(i = _nIndexStart ; i <= _nIndexEnd ; i++ )
		{
			_nIndex = i % _nMaxIndex ;
			_nPos = i - _nIndexStart ;
			DrawHorizontalLineCscan(pFrame, nWndIndex_ , _nPos , _nIndex) ;
		}
	}
	else if ( g_nScanTimmerCounter < _nIndexStart )
	{
		g_CscanDrawInfo[nWndIndex_].nShowStop  = g_nScanTimmerCounter + _nBeamShowQty - 1;
		g_CscanDrawInfo[nWndIndex_].nShowStart = g_nScanTimmerCounter  ;
		_nIndexStart = g_CscanDrawInfo[nWndIndex_].nShowStart ;
		_nIndexEnd   = g_CscanDrawInfo[nWndIndex_].nShowStop  ;
		for(i = _nIndexStart ; i <= _nIndexEnd ; i++ )
		{
			_nPos = i - _nIndexStart ;
			DrawHorizontalLineCscan(pFrame, nWndIndex_ , _nPos , i) ;
		}
	}
	else
	{
		_nIndexStart = g_CscanDrawInfo[nWndIndex_].nShowStart ;
		_nIndexEnd   = g_CscanDrawInfo[nWndIndex_].nShowStop  ;
		for(i = _nIndexStart ; i <= _nIndexEnd ; i++ )
		{
			_nPos = i - _nIndexStart ;
			DrawHorizontalLineCscan(pFrame, nWndIndex_ , _nPos , i) ;
		}
	}
}

// draw C scan when inspect source is encoder
void CScanDisplayEncoderScrollWindow (unsigned short* pFrame , int nWndIndex_ , int nDataIndex_)
{
	int i ;
	int _nPos   ;
	int _nIndexStart   = g_CscanDrawInfo[nWndIndex_].nShowStart;
	int _nIndexEnd     = g_CscanDrawInfo[nWndIndex_].nShowStop ;
	int _nBeamShowQty  = g_CscanDrawInfo[nWndIndex_].nShowBeamQty   ;

    //printf("start:%d end:%d qty:%d, dataIndex:%d\n", _nIndexStart, _nIndexEnd, _nBeamShowQty, nDataIndex_);
	if ( nDataIndex_ > _nIndexEnd )
	{
		g_CscanDrawInfo[nWndIndex_].nShowStop  = nDataIndex_ ;
		g_CscanDrawInfo[nWndIndex_].nShowStart = nDataIndex_ - _nBeamShowQty + 1 ;
	}
    // move drawed range first to avoid scale process
	// then draw the vacc zoom
	else if ( nDataIndex_ < _nIndexStart )
	{
		g_CscanDrawInfo[nWndIndex_].nShowStart  = nDataIndex_ ;
		g_CscanDrawInfo[nWndIndex_].nShowStop   = nDataIndex_ + _nBeamShowQty - 1;
	}

	_nIndexStart = g_CscanDrawInfo[nWndIndex_].nShowStart ;
	_nIndexEnd   = g_CscanDrawInfo[nWndIndex_].nShowStop  ;

    //printf("start:%d end:%d\n", _nIndexStart, _nIndexEnd);

	//printf("sourceType:%d\n", g_CscanDrawInfo[nWndIndex_].nSourceType);
	//g_CscanDrawInfo[nWndIndex_].nSourceType = 0;
	for(i = _nIndexStart ; i <= _nIndexEnd ; i++ )
	{
		_nPos = i - _nIndexStart ;
		DrawVerticalLineCscan(pFrame, nWndIndex_ , _nPos , i) ;
	}
}

void CScanVerticalDisplayEncoderScrollWindow (unsigned short* pFrame , int nWndIndex_ , int nDataIndex_)
{
	int i ;
	int _nPos   ;
	int _nIndexStart   = g_CscanDrawInfo[nWndIndex_].nShowStart;
	int _nIndexEnd     = g_CscanDrawInfo[nWndIndex_].nShowStop ;
	int _nBeamShowQty  = g_CscanDrawInfo[nWndIndex_].nShowBeamQty   ;

	if ( nDataIndex_ > _nIndexEnd ) {
		g_CscanDrawInfo[nWndIndex_].nShowStop  = nDataIndex_ ;
		g_CscanDrawInfo[nWndIndex_].nShowStart = nDataIndex_ - _nBeamShowQty + 1 ;
	} else if ( nDataIndex_ < _nIndexStart ) {
		g_CscanDrawInfo[nWndIndex_].nShowStart  = nDataIndex_ ;
		g_CscanDrawInfo[nWndIndex_].nShowStop   = nDataIndex_ + _nBeamShowQty - 1;
	}

	_nIndexStart = g_CscanDrawInfo[nWndIndex_].nShowStart ;
	_nIndexEnd   = g_CscanDrawInfo[nWndIndex_].nShowStop  ;

	for(i = _nIndexStart ; i <= _nIndexEnd ; i++ )
	{
		_nPos = i - _nIndexStart ;
		DrawHorizontalLineCscan(pFrame, nWndIndex_ , _nPos , i) ;
	}
}


void CScanDisplayEncoderStaticWindow (unsigned short* pFrame , int nWndIndex_ , int nDataIndex_)
{
	int i , j ;
	int _nIndexStart  ;  // = g_CscanDrawInfo[nWndIndex_].nShowStart;
	int _nIndexEnd    ;  // = g_CscanDrawInfo[nWndIndex_].nShowStop ;
	//int _nDataSize     = g_CscanDrawInfo[nWndIndex_].nDataSize ;
	//int _nMaxIndex     = g_CscanDrawInfo[nWndIndex_].nMaxStoreIndex ;
	int _nBeamShowQty  = g_CscanDrawInfo[nWndIndex_].nShowBeamQty   ;
	int _nWidth        = g_CscanDrawInfo[nWndIndex_].nWidth         ;

	for(i = 0 ; i < _nBeamShowQty ; i++)
	{
	    // draw stored beam to mapped lines
		if(!ScanDataMark[i])
		{
			continue ;
		}

		_nIndexStart = i * _nWidth / _nBeamShowQty;
		_nIndexEnd   = (i + 1) * _nWidth /_nBeamShowQty ;

		for(j = _nIndexStart ;j < _nIndexEnd ; j++)
		{
			DrawVerticalLineCscan(pFrame, nWndIndex_ , j , i) ;
		}
	}
}

void CScanVertDisplayEncoderStaticWindow (unsigned short* pFrame , int nWndIndex_ , int nDataIndex_)
{
	int i , j ;
	int _nIndexStart  ;  // = g_CscanDrawInfo[nWndIndex_].nShowStart;
	int _nIndexEnd    ;  // = g_CscanDrawInfo[nWndIndex_].nShowStop ;
	int _nBeamShowQty  = g_CscanDrawInfo[nWndIndex_].nShowBeamQty   ;
	int _nHeight       = g_CscanDrawInfo[nWndIndex_].nHeight        ;

	for(i = 0 ; i < _nBeamShowQty ; i++)
	{
	    // draw stored beam to mapped lines
		if(!ScanDataMark[i])
		{
			continue ;
		}

		_nIndexStart = i * _nHeight / _nBeamShowQty;
		_nIndexEnd   = (i + 1) * _nHeight /_nBeamShowQty ;

		for(j = _nIndexStart ;j < _nIndexEnd ; j++)
		{
			DrawHorizontalLineCscan(pFrame, nWndIndex_ , j , i) ;
		}
	}
}

///////////////////////////////
//  CalcFanScan
//  功能         : 计算各个像素点的插值参数
//  image        : 像素缓存
//  startAngle   : 扫描起始角度
//  endAngle     : 扫描终止角度
//  StartWave    : 波形起始
//  WaveLength   : 波形长度
int CalcFanScanMatrix (double startAngle, double endAngle, double stepAngle, double StartFocusDepth,
	        double FocusEnd, double JunctionOffset, int DataLength, int width, int height, unsigned char group , int bSkewReverse_)
{
       double xLeftmost              ;     //     扇形 最左点坐标
       double xRightmost             ;     //     扇形 最右点坐标
       double yTopmost               ;     //     最上点
       double yBottonmost            ;     //     最下点
       double FocusRange             ;
       // 起始\终止\步进  角度
       double a1, a2 , a3            ;
       int RowNo                     ;
       //
       double _a3                    ;
       int     _RowNo                ;
       // 实际窗口 宽 高
       double tempW , tempH          ;
       // 实际窗口 入射点坐标
       double xOrg , yOrg            ;
       // 拉伸窗口 入射点坐标
       double xOrgS, yOrgS           ;
       // 拉伸窗口 信号起 止半径
       double startWaveS , endWaveS  ;
       double xxx  , yyy             ;
       double tLength                ;
       double xScale                 ;
       double yScale                 ;
       int i , j                     ;

       double* pScales = NULL        ;
       double* pScalesAngle =  NULL  ;
       double* _pScalesAngle=  NULL  ;
       double tempx, tempy           ;
       double tempScale              ;
       double tAngle                 ;
       int iAngle                    ;
      
	   double TempLength              ; 
	   double templength              ;
	   double BeamStartLength[256]    ;
	   double BeamEndLength[256]      ;
	   int _nPointer;


       // 为全局指针分配内存 
       memset(pDraw[group] , 0 , 605 * 390);
	   a1  = startAngle * _PI/180.0                                       ;
       a2  = endAngle   * _PI/180.0                                       ;
       a3  = stepAngle  * _PI/180.0                                       ;
       _a3 = MIN_DRAW_FAN_ANGLE_STEP * _PI/180.0                          ;
       //  扫描数据列数
	   RowNo = (int) ((endAngle-startAngle) / stepAngle +1)                ;
	   _RowNo= (int) ((endAngle-startAngle) / MIN_DRAW_FAN_ANGLE_STEP + 1) ;
       // 89 degree  solution ,  move to focallaw
	   //JunctionOffset = JunctionOffset * cos(MAX(fabs(a1) , fabs(a2))) ;
	   if(RowNo == 1)
	   {
           //****************************************************************************
           //keep the location for angle line drawing
           if(bSkewReverse_)
           {
                CurrentLine_bottom[group][0].x = 0;
                CurrentLine_bottom[group][0].y = height ;
                //keep the location for angle line drawing
                CurrentLine_top[group][0].x = width ;
                CurrentLine_top[group][0].y = 0 ;
           }
           else
           {
                CurrentLine_bottom[group][0].x = width;
                CurrentLine_bottom[group][0].y = height ;
                //keep the location for angle line drawing
                CurrentLine_top[group][0].x = 0 ;
                CurrentLine_top[group][0].y = 0 ;
           }
           //****************************************************************************
		   return 0;
	   }
       //  临时比例队列
       pScales = (double*)malloc(sizeof(double)*RowNo)                   ;
       pScalesAngle = (double*)malloc(sizeof(double)*RowNo)              ;
       // calculate the fan range in real coordinate
       FocusRange = FocusEnd - StartFocusDepth  ;
       yBottonmost = JunctionOffset +  FocusEnd  ;
	   
       if( a1 * a2 <= 0)
	   {
			xLeftmost   = JunctionOffset * tan(a1) + (StartFocusDepth + FocusRange)*sin(a1)  ;
			xRightmost  = JunctionOffset * tan(a2) + (StartFocusDepth + FocusRange)*sin(a2)  ;
			
            if( fabs(a2) > fabs(a1) )
			   yTopmost = JunctionOffset + StartFocusDepth * cos(a2)  ;
			else 
			   yTopmost = JunctionOffset + StartFocusDepth * cos(-a1) ;
			
            TempLength  = StartFocusDepth + FocusRange                ;
			templength  = StartFocusDepth                             ;
	   }
	   else if (a1 < 0 && a2 < 0)
	   {
		    yTopmost   = JunctionOffset + (StartFocusDepth / cos(-a2)) * cos(-a1)                    ; 
		    xLeftmost  = JunctionOffset * tan(a1) + sin(a1) * (StartFocusDepth + FocusRange)/cos(a2) ;
		    xRightmost = (JunctionOffset + StartFocusDepth) * tan(a2)                                ;
			
            TempLength = (StartFocusDepth + FocusRange) / cos(-a2)								     ;
			templength = StartFocusDepth / cos(-a2)                                                  ;
	   }
	   else if (a1 > 0 && a2 > 0)
	   {
	        yTopmost   = JunctionOffset + (StartFocusDepth / cos(a1)) * cos(a2)						 ; 
            xLeftmost  = (JunctionOffset + StartFocusDepth) * tan(a1)								 ;
		    xRightmost = JunctionOffset * tan(a2) + sin(a2) * (StartFocusDepth + FocusRange)/cos(a1) ;
			
            TempLength = (StartFocusDepth + FocusRange) / cos(a1)									 ;
			templength = StartFocusDepth / cos (a1)													 ;
	   }
       // 波型的实际宽度和高度
       // 虚拟发射点坐标
       tempW = (double)(xRightmost - xLeftmost)           ;
       tempH = (double)(yBottonmost- yTopmost)            ;

       xOrg  = 0 - xLeftmost                               ;
       yOrg  = 0 - yTopmost                                ;
       
       // 实际尺寸 转 显示尺寸 比例
       xScale = width/tempW                                ;
       yScale = height/tempH                               ;
  
       // 显示坐标系中 虚拟发射点坐标
       xOrgS  = xOrg*xScale                                ;
       yOrgS  = yOrg*yScale                                ;
       
       //求每一列波型的 比例尺
       //和拉伸后显示的扫描角度
       for(i = 0; i< RowNo ; i++)
       {
             tempx = xScale * (sin(a1+i*a3)*(JunctionOffset/cos(a1+i*a3)+TempLength)+xOrg)                  ;
             tempy = yScale * (cos(a1+i*a3)*(JunctionOffset/cos(a1+i*a3)+TempLength)+yOrg)                  ;
             //****************************************************************************
             //keep the location for angle line drawing
             if(bSkewReverse_)
             {
            	 CurrentLine_bottom[group][i].x = width - (int)tempx ;
             }
             else
             {
            	 CurrentLine_bottom[group][i].x = (int)tempx ;
             }

             CurrentLine_bottom[group][i].y = (int)tempy ;
             //****************************************************************************
             tempx = tempx  -  xOrgS																		;
             tempy = tempy  -  yOrgS																		;
             if(tempy != 0)
                    pScalesAngle[i] = atan(tempx/tempy)														;
             else pScalesAngle[i] = tempx>0 ? _PI/2.0 : -_PI/2.0											;
             pScales[i] = sqrt(tempx*tempx+tempy*tempy)/(JunctionOffset/cos(a1+i*a3)+TempLength)            ;
            
             BeamEndLength[i]   = sqrt(tempx*tempx+tempy*tempy)                                             ;
			 tempx = xScale * (sin(a1+i*a3)*(JunctionOffset/cos(a1+i*a3)+templength)+xOrg)                  ;
			 tempy = yScale * (cos(a1+i*a3)*(JunctionOffset/cos(a1+i*a3)+templength)+yOrg)                  ;
             //****************************************************************************
             //keep the location for angle line drawing
             if(bSkewReverse_)
             {
            	 CurrentLine_top[group][i].x = width - (int)tempx ;
             }
             else
             {
            	 CurrentLine_top[group][i].x = (int)tempx ;
             }
             CurrentLine_top[group][i].y = (int)tempy ;
             //****************************************************************************
             tempx = tempx  -  xOrgS																		;
             tempy = tempy  -  yOrgS																		;
			 BeamStartLength[i] = sqrt(tempx*tempx+tempy*tempy)                                             ;
       }
       // 当最小 步进角度 小于  MIN_DRAW_FAN_ANGLE_STEP   的时候
       // 取  MIN_DRAW_FAN_ANGLE_STEP 为最小步进角度
       // 实现 各个扇区 平稳过度  美观
       if( stepAngle > MIN_DRAW_FAN_ANGLE_STEP )
       {
                free( pScales);
                pScales = (double*) malloc(  sizeof(double) * _RowNo )         ;
                _pScalesAngle = ( double* ) malloc( sizeof( double) * _RowNo)  ;
                for(i = 0; i< _RowNo ; i++)
                {
                         tempx = xScale * (sin(a1+i*_a3)*(JunctionOffset/cos(a1+i*_a3)+TempLength)+xOrg)                  ;
                         tempy = yScale * (cos(a1+i*_a3)*(JunctionOffset/cos(a1+i*_a3)+TempLength)+yOrg)                  ;
                        
                         tempx = tempx  -  xOrgS	  ;
                         tempy = tempy  -  yOrgS	  ;
                         if(tempy != 0)
                         {
                        	 _pScalesAngle[i] = atan(tempx/tempy) ;
                         }
                         else
                         {
                        	 pScalesAngle[i] = tempx>0 ? _PI/2.0 : -_PI/2.0	 ;
                         }
                         pScales[i] = sqrt(tempx*tempx+tempy*tempy)/(JunctionOffset/cos(a1+i*_a3)+TempLength) ;
                         BeamEndLength[i]   = sqrt(tempx*tempx+tempy*tempy) ;
            			 tempx = xScale * (sin(a1+i*_a3)*(JunctionOffset/cos(a1+i*_a3)+templength)+xOrg) ;
            			 tempy = yScale * (cos(a1+i*_a3)*(JunctionOffset/cos(a1+i*_a3)+templength)+yOrg) ;
                         tempx = tempx  -  xOrgS ;
                         tempy = tempy  -  yOrgS ;
            			 BeamStartLength[i] = sqrt(tempx*tempx+tempy*tempy)  ;
                }
                _pScalesAngle[_RowNo - 1] = pScalesAngle[ RowNo - 1]  ;
       }

       a1 = pScalesAngle[0]          ;
       a2 = pScalesAngle[RowNo -1]   ;
	   for(i = 0 ; i< height ; i++)
       {
            for(j = 0; j< width; j++)
            {
				if(bSkewReverse_)
				{
					_nPointer = i * width + width - j ;
				}
				else
				{
				   _nPointer = i * width + j ;
				}

				xxx = j-xOrgS      ;
				yyy = i-yOrgS      ;
				tLength = sqrt(xxx*xxx + yyy*yyy)     ;
				if(tLength == 0) tLength = 0.01       ;
				tAngle  = asin(xxx/tLength)           ;
				if(tAngle > a2|| tAngle < a1)
				{
				  continue ;
				}
				iAngle = 0   ;
				while(tAngle >= pScalesAngle[iAngle]  &&  tAngle < a2)
				{
					iAngle++  ;
				}
				iAngle--  ;
				pAngleZoom[group][_nPointer] = iAngle        ;
				tempScale = (tAngle - pScalesAngle[iAngle])/(pScalesAngle[iAngle+1] - pScalesAngle[iAngle])         ;
				if(tempScale > 1) tempScale = 1                                                                     ;
				pDrawRate[group][_nPointer] = (unsigned char)( tempScale * COLOR_STEP )                             ;
				if(stepAngle <= MIN_DRAW_FAN_ANGLE_STEP)
				{
						  startWaveS = BeamStartLength[iAngle] * ( 1 - tempScale )  + BeamStartLength[iAngle+1] * tempScale      ;
						  endWaveS   = BeamEndLength[iAngle]   * ( 1 - tempScale )  + BeamEndLength[iAngle+1] * tempScale        ;
						  tempScale = pScales[iAngle]*(1-tempScale)+pScales[iAngle+1]*tempScale                                  ;
						  pDataNo[group][_nPointer] = (int)( DataLength * (tLength-startWaveS)/(endWaveS - startWaveS) )         ;
						  if(tLength > endWaveS || tLength<startWaveS)
						  {
								  pDraw[group][_nPointer]= 0 ;
								  continue  ;
						  }
						  pDraw[group][_nPointer]= 255       ;
				}
				else
				{
						  iAngle = 0     ;
						  while(tAngle>=_pScalesAngle[iAngle] && tAngle<a2)
						  {
								iAngle++ ;
						  }
						  iAngle--       ;
						  tempScale = (tAngle - _pScalesAngle[iAngle]) / (_pScalesAngle[iAngle+1] - _pScalesAngle[iAngle])  ;
						  if (tempScale > 1)
						  {
							  tempScale = 1  ;
						  }
						  startWaveS = BeamStartLength[iAngle] * ( 1 - tempScale )  + BeamStartLength[iAngle+1] * tempScale ;
						  endWaveS   = BeamEndLength[iAngle]   * ( 1 - tempScale )  + BeamEndLength[iAngle+1] * tempScale   ;
						  tempScale =  pScales[iAngle] * (1- tempScale) +  pScales[iAngle+1]*tempScale                      ;
						  pDataNo[group][_nPointer] = (int)(DataLength * (tLength-startWaveS)/(endWaveS - startWaveS) )     ;
						  if(tLength > endWaveS || tLength<startWaveS)
						  {
							  pDraw[group][ _nPointer]= 0 ;
							  continue  ;
						  }
						  pDraw [group][_nPointer]= 255  ;
				}
            }
       }

       free(pScales);
	   free(pScalesAngle);
	   free(_pScalesAngle) ;
	   return 0;
}

int calc_line_position (double startAngle, double endAngle, double stepAngle, double StartFocusDepth,
		                     double FocusEnd, double JunctionOffset, int DataLength, int width, int height, unsigned char group)
{
       // 实际坐标
       double xLeftmost              ;     //     扇形 最左点坐标
       double xRightmost             ;     //     扇形 最右点坐标
       double yTopmost               ;     //     最上点
       double yBottonmost            ;     //     最下点
       double FocusRange             ;
       // 起始\终止\步进  角度
       double a1, a2 , a3            ;
       int RowNo                     ;
       //
       // 实际窗口 宽 高
       double tempW , tempH          ;
       // 实际窗口 入射点坐标
       double xOrg , yOrg            ;
       // 拉伸窗口 入射点坐标
       double xOrgS, yOrgS           ;
       // 拉伸窗口 信号起 止半径
       double xScale                 ;
       double yScale                 ;
       int i                         ;
       double tempy                  ;
	   double TempLength             ;
	   double templength             ;
       // 为全局指针分配内存
       a1  = startAngle * _PI/180.0                                       ;
       a2  = endAngle   * _PI/180.0                                       ;
       a3  = stepAngle  * _PI/180.0                                       ;
       //  扫描数据列数
	   RowNo = (int) ((endAngle-startAngle) / stepAngle +1)               ;
       // calculate the fan range in real coordinate
       FocusRange = FocusEnd - StartFocusDepth  ;
       yBottonmost = JunctionOffset +  FocusEnd  ;
       if( a1 * a2 <= 0)
	   {
			xLeftmost   = JunctionOffset * tan(a1) + (StartFocusDepth + FocusRange)*sin(a1)  ;
			xRightmost  = JunctionOffset * tan(a2) + (StartFocusDepth + FocusRange)*sin(a2)  ;

            if( fabs(a2) > fabs(a1) )
			   yTopmost = JunctionOffset + StartFocusDepth * cos(a2)  ;
			else
			   yTopmost = JunctionOffset + StartFocusDepth * cos(-a1) ;

            TempLength  = StartFocusDepth + FocusRange                ;
			templength  = StartFocusDepth                             ;
	   }
	   else if (a1 < 0 && a2 < 0)
	   {
		    yTopmost   = JunctionOffset + (StartFocusDepth / cos(-a2)) * cos(-a1)                    ;
		    xLeftmost  = JunctionOffset * tan(a1) + sin(a1) * (StartFocusDepth + FocusRange)/cos(a2) ;
		    xRightmost = (JunctionOffset + StartFocusDepth) * tan(a2)                                ;
            TempLength = (StartFocusDepth + FocusRange) / cos(-a2)								     ;
			templength = StartFocusDepth / cos(-a2)                                                  ;
	   }
	   else if (a1 > 0 && a2 > 0)
	   {
	        yTopmost   = JunctionOffset + (StartFocusDepth / cos(a1)) * cos(a2)						 ;
            xLeftmost  = (JunctionOffset + StartFocusDepth) * tan(a1)								 ;
		    xRightmost = JunctionOffset * tan(a2) + sin(a2) * (StartFocusDepth + FocusRange)/cos(a1) ;
            TempLength = (StartFocusDepth + FocusRange) / cos(a1)									 ;
			templength = StartFocusDepth / cos (a1)													 ;
	   }
       // 波型的实际宽度和高度
       // 虚拟发射点坐标
       tempW = (double)(xRightmost - xLeftmost)           ;
       tempH = (double)(yBottonmost- yTopmost)            ;
       xOrg  = 0 - xLeftmost                               ;
       yOrg  = 0 - yTopmost                                ;
       // 实际尺寸 转 显示尺寸 比例
       xScale = width/tempW                                ;
       yScale = height/tempH                               ;
       // 显示坐标系中 虚拟发射点坐标
       xOrgS  = xOrg*xScale                                ;
       yOrgS  = yOrg*yScale                                ;
       //求每一列波型的 比例尺
       //和拉伸后显示的扫描角度
       for(i = 0; i< RowNo ; i++)
       {
             tempy = yScale * (cos(a1+i*a3)*(JunctionOffset/cos(a1+i*a3)+TempLength)+yOrg)                  ;
             //****************************************************************************
             //keep the location for angle line drawing
             AScanDrawRange[group][i].end = (int)tempy ;
			 tempy = yScale * (cos(a1+i*a3)*(JunctionOffset/cos(a1+i*a3)+templength)+yOrg)                  ;
             //****************************************************************************
             //keep the location for angle line drawing
			 AScanDrawRange[group][i].start = (int)tempy ;
             //****************************************************************************
       }
       return 0;
}

int DrawPixbuff(unsigned short *p, unsigned int xoffset, unsigned int yoffset, int width, int height,
		    int waveLength, DOT_TYPE *WaveData, unsigned char group , unsigned short* pColor)
{   
       int	i, j;
       int pointer ;
       int pointer2;
       int tempData ;

       for(i = 0; i< height; i++)
       {
             for(j = 0; j < width; j++)
             {
                      pointer = i * width + j ;
                      if(pDraw[group][pointer] != 0)
                      {
                           pointer2 = (int)(pAngleZoom[group][pointer] * (waveLength + 32) + pDataNo[group][pointer]);
                           tempData = (int)(WaveData[pointer2] * (COLOR_STEP - pDrawRate[group][pointer]) +
								   WaveData[pointer2 + waveLength + 32] * pDrawRate[group][pointer] ) ;
						   fbdot (p, xoffset + j, yoffset + i - 1, pColor[tempData>>COLOR_SHIFT]);
					  }
             }
       }

	   return 0;
}

#define FLOAT_ZERO_GATE 0.0000001
void CalcFanscanReal(FAN_SCAN_INFO_P pInfo_)
{
       int i , j , k  ;
       unsigned char t ;
       int _nTmpIndex  ;
       double _nPosCurrentY , _nPosCurrentX , _nTmp;
       double nPosJunction[256] , _nAngles[256];
       int width   =   pInfo_->width         ;    //  图像 宽    单位 像素
       int height  =   pInfo_->height        ;    //  图像 高    单位 像素
       int group   =   pInfo_->group       ;
       // get real window size
       double  _nAngleStart = pInfo_->nStartAngle ;
       double  _nAngleStop  = pInfo_->nStopAngle  ;
       double  _nAngleStep  = pInfo_->nStepAngle  ;
       int     _nPointQty   = pInfo_->nPointQty ;
       int     _nBeamQty    = (int)((_nAngleStop - _nAngleStart) / _nAngleStep + 1.1) ;
       double  _nSampleStart= pInfo_->nSampleStart;
       double  _nSampleRange= pInfo_->nSampleRange;
       double  _nSampleStop = _nSampleStart + _nSampleRange ;
       float* _pExitPoint  = pInfo_->pExitPoint  ;
       int     _nDirection  = pInfo_->direction   ;
       double _nStartX , _nStopX , _nStartY , _nStopY , _nStepX , _nStepY ;
       // get juction of each two beam
       double _nTopLocation[256]  ;
       memset((void*)(pDraw[group]) , 0 , width * height) ;
       for(i = 0 ; i < _nBeamQty - 1; i++)
       {
            _nTopLocation[i] = (_pExitPoint[i] + _pExitPoint[i+1]) / 2 ;
       }
       // get real window size
       if(_nAngleStart * _nAngleStop <= 0)
       {
            _nStartX = _pExitPoint[0] + _nSampleStop * sin(_nAngleStart) ;
            _nStopX  = _pExitPoint[_nBeamQty - 1] + _nSampleStop * sin(_nAngleStop) ;
            _nStopY  = _nSampleStop  ;
            if(fabs(_nAngleStart) > fabs(_nAngleStop))
            {
                _nStartY = _nSampleStart * cos(_nAngleStart)  ;
            }
            else
            {
                _nStartY = _nSampleStart * cos(_nAngleStop)   ;
            }
       }
       else if( _nAngleStart < 0 && _nAngleStop < 0)
       {
            _nStartX = _pExitPoint[0] + _nSampleStop * sin(_nAngleStart) ;
            _nStopX  = _pExitPoint[_nBeamQty -1] + _nSampleStart * sin(_nAngleStop) ;
            _nStartY = _nSampleStart * cos(_nAngleStart)  ;
            _nStopY  = _nSampleStop * cos(_nAngleStop) ;
       }
       else //( _nAngleStart > 0 && _nAngleStop > 0)
       {
            _nStartX = _pExitPoint[0] + _nSampleStart * sin(_nAngleStart) ;
            _nStopX  = _pExitPoint[_nBeamQty - 1] + _nSampleStop * sin(_nAngleStop);
            _nStartY = _nSampleStart * cos(_nAngleStop)  ;
            _nStopY  = _nSampleStop * cos(_nAngleStart)  ;
       }
       // get real step of each pixel
       _nStepX  = (_nStopX - _nStartX) / (width - 1) ;
       _nStepY  = (_nStopY - _nStartY) / (height - 1) ;
       if(_nStepX < FLOAT_ZERO_GATE || _nStepY < FLOAT_ZERO_GATE)
       {
           for(i = 0 ; i< _nBeamQty ; i++)
           {
				CurrentLine_top[group][i].x = 0 ;
				CurrentLine_bottom[group][i].x = width  ;

				CurrentLine_top[group][i].y = 0 ;
				CurrentLine_bottom[group][i].y = height  ;

				if(_nDirection)
				{
					CurrentLine_top[group][i].x    = width - CurrentLine_top[group][i].x ;
					CurrentLine_bottom[group][i].x = width - CurrentLine_bottom[group][i].x ;
				}
                return ;
           }
       }
       // get all values of angle
       for(i = 0 ; i < _nBeamQty ; i++)
       {
            _nAngles[i] = _nAngleStart + i * _nAngleStep ;
       }

       for(j = 0 ; j < height ; j++)
       {
            // current y position
            _nPosCurrentY = _nStartY + j * _nStepY ;
            // beam junctions with current y coordinates
            for(k = 0 ; k < _nBeamQty ; k++)
            {
                nPosJunction[k] = _pExitPoint[k] + tan(_nAngles[k]) * _nPosCurrentY  ;
            }
            // calculate matrix
            t = 0 ;
            for(i = 0 ; i < width ; i++)
            {
            	// get current pixel position in real coordinate
                _nPosCurrentX = _nStartX + _nStepX * i ;
                for( ; t< _nBeamQty - 1; t++)
                {
                	// if current position is less than minimun or bigger than maximum  get out
                    if(_nPosCurrentX > nPosJunction[_nBeamQty - 1] || _nPosCurrentX < nPosJunction[0] )
                    {
                        break;
                    }
                    if(_nPosCurrentX >= nPosJunction[t] && _nPosCurrentX <= nPosJunction[t+1] )
                    {
                    	if(_nDirection)
                    	{
                    		// if draw direction is inverse , transfer the x coordinate
                    		_nTmpIndex = width - i + j * width  ;
                    	}
                    	else
                    	{
                    		_nTmpIndex = i + j * width  ;
                    	}
                        pAngleZoom[group][_nTmpIndex]  = t ;
                        _nTmp = _nPosCurrentX - _nTopLocation[t]  ;
                        _nTmp = sqrt(_nPosCurrentY * _nPosCurrentY + _nTmp * _nTmp) ;
                        if(_nTmp > _nSampleStop || _nTmp < _nSampleStart)
                        { // if current position is out of sample range , get out
                            break;
                        }
                        // get current point data index in data block
                        pDataNo[group][_nTmpIndex]   = _nPointQty * (_nTmp - _nSampleStart) / _nSampleRange ;
                        if((nPosJunction[t+1] - _nPosCurrentX) < FLOAT_ZERO_GATE)
                        {
                            pDrawRate[group][_nTmpIndex] = COLOR_STEP  ;
                        }
                        else
                        {
                            _nTmp = cos(_nAngles[t]) * (_nPosCurrentX - nPosJunction[t]) / ( cos(_nAngles[t+1]) * (nPosJunction[t+1] - _nPosCurrentX))  ;
                            _nTmp = _nTmp / (1 + _nTmp) ;
                            pDrawRate[group][_nTmpIndex] =  _nTmp * COLOR_STEP ;
                        }
                        pDraw[group][_nTmpIndex]  = -1 ;
                        break ;
                    }
                }
            }
       }
       // get each beam start and end pixels
       for(i = 0 ; i< _nBeamQty ; i++)
       {
            _nTmp  = _pExitPoint[i] + _nSampleStart * sin( _nAngles[i] )   ;
            CurrentLine_top[group][i].x = (_nTmp - _nStartX ) / _nStepX  ;
            _nTmp  = _pExitPoint[i] + _nSampleStop * sin( _nAngles[i] )   ;
            CurrentLine_bottom[group][i].x = (_nTmp - _nStartX ) / _nStepX  ;

            _nTmp  =  _nSampleStart * cos( _nAngles[i] )   ;
            CurrentLine_top[group][i].y = (_nTmp - _nStartY ) / _nStepY  ;
            _nTmp  = _nSampleStop * cos( _nAngles[i] )   ;
            CurrentLine_bottom[group][i].y = (_nTmp - _nStartY ) / _nStepY  ;
        	if(_nDirection)
        	{
        		CurrentLine_top[group][i].x    = width - CurrentLine_top[group][i].x ;
        		CurrentLine_bottom[group][i].x = width - CurrentLine_bottom[group][i].x ;
        	}
       }

}

void GetAscanRLinePosition (FAN_SCAN_INFO_P pInfo_)
{
    int i  ;

    double _nTmp;
    double _nAngles[256];
    int height  =   pInfo_->height        ;    //  图像 高    单位 像素
    int group   =   pInfo_->group       ;
    // get real window size
    double  _nAngleStart = pInfo_->nStartAngle ;
    double  _nAngleStop  = pInfo_->nStopAngle  ;
    double  _nAngleStep  = pInfo_->nStepAngle  ;

    int     _nBeamQty    = (int)((_nAngleStop - _nAngleStart) / _nAngleStep + 1.1) ;
    double  _nSampleStart= pInfo_->nSampleStart;
    double  _nSampleRange= pInfo_->nSampleRange;
    double  _nSampleStop = _nSampleStart + _nSampleRange ;

    double _nStartY , _nStopY , _nStepY ;
    // get real window size
    if(_nAngleStart * _nAngleStop <= 0)
    {
         _nStopY  = _nSampleStop  ;
         if(fabs(_nAngleStart) > fabs(_nAngleStop))
         {
             _nStartY = _nSampleStart * cos(_nAngleStart)  ;
         }
         else
         {
             _nStartY = _nSampleStart * cos(_nAngleStop)   ;
         }
    }
    else if( _nAngleStart < 0 && _nAngleStop < 0)
    {
         _nStartY = _nSampleStart * cos(_nAngleStart)  ;
         _nStopY  = _nSampleStop * cos(_nAngleStop) ;
    }
    else //( _nAngleStart > 0 && _nAngleStop > 0)
    {
         _nStartY = _nSampleStart * cos(_nAngleStop)  ;
         _nStopY  = _nSampleStop * cos(_nAngleStart)  ;
    }
    // get real step of each pixel
    _nStepY  = (_nStopY - _nStartY) / (height - 1) ;
    if( _nStepY < FLOAT_ZERO_GATE)
    {
        for(i = 0 ; i< _nBeamQty ; i++)
        {
				AScanDrawRange[group][i].start = 0 ;
				AScanDrawRange[group][i].end = height  ;
                return ;
        }
    }
    // get all values of angle
    for(i = 0 ; i < _nBeamQty ; i++)
    {
         _nAngles[i] = _nAngleStart + i * _nAngleStep ;
    }

    // get each beam start and end pixels
    for(i = 0 ; i< _nBeamQty ; i++)
    {
         _nTmp  =  _nSampleStart * cos( _nAngles[i] )   ;
         AScanDrawRange[group][i].start = (_nTmp - _nStartY ) / _nStepY  ;
         _nTmp  = _nSampleStop * cos( _nAngles[i] )   ;
         AScanDrawRange[group][i].end = (_nTmp - _nStartY ) / _nStepY  ;
    }
}

// draw S scan when Linear model and Display as true depth
//  range:  real range in mm (range from several tens to several hundreds, or else will cause drawing not good)
//  width  height :  draw area width  and height
//  DataLength  :   wave data length of each beam
void CalcLinearScan(int start_element, int stop_element, int element_step, int element_qty, double _angle,
		int DataLength, double range, int width, int height, unsigned char group , int bSkewReverse_)
{
	   double real_width ;         // real coordinate width     ***  set element pitch to be 1 mm
	   double real_height;         // real coordinate height
	   double xScale     ;
	   double yScale     ;         // x y axis scale factor
	   double tan_angle_scaled ;   // tan value of reflect angle after scaled
	   int i ,  j   ;              // circle element
	   double angle ;              // reflect angle (arc)
	   double xVacc ;              // x axis vacant distance

	   double tmpX  ;

	   double xxx   ;
	   int offset   ;

	   double tmpDrawRate ;
	   double tmpDataNo   ;
	   double beam_width  ;
	   int    beam_qty       ;

	   angle = fabs(_angle * _PI /180.0);
       beam_qty  = (stop_element  - start_element - element_qty + 1 ) / element_step ;

	   memset(pDraw[group] , 0 , 605 * 390);

	   if(beam_qty == 0)
	   {
		   if((_angle >=0 && (!bSkewReverse_)) || ((_angle < 0 && bSkewReverse_)))
		   {
	           //****************************************************************************
	           //keep the location for angle line drawing
	           CurrentLine_bottom[group][0].x = width ;
	           CurrentLine_bottom[group][0].y = height ;

	           //keep the location for angle line drawing
	           CurrentLine_top[group][0].x = 0 ;
	           CurrentLine_top[group][0].y = 0 ;
		   }
		   else
		   {
	           //****************************************************************************
	           //keep the location for angle line drawing
	           CurrentLine_bottom[group][0].x = 0 ;
	           CurrentLine_bottom[group][0].y = height ;

	           //keep the location for angle line drawing
	           CurrentLine_top[group][0].x = width ;
	           CurrentLine_top[group][0].y = 0 ;
		   }
           //****************************************************************************
		   return ;
	   }
       real_height  = range ;
	   real_width   = stop_element - start_element + real_height * tan(angle) ;

	   xScale = width / real_width ;
	   yScale = height/ real_height ;

	   xVacc = tan(angle) * real_height * xScale ;
	   beam_width = width - xVacc ;
	   tan_angle_scaled =  xVacc / height ;
	   for( i = 0 ; i< height ; i++)
	   {
		   tmpX = i * tan_angle_scaled ;
		   tmpDataNo = ((double)i)/ height ;
		   for( j = 0 ; j< width ; j++ )
		   {
			   xxx = j - tmpX ;
			   if((_angle >=0 && (!bSkewReverse_)) || ((_angle < 0 && bSkewReverse_)))
			   {
				   offset  = j + i * width ;
			   }
			   else
			   {
				   offset  = width - j + i * width ;
			   }
			   //if (_angle >=0 )
			   //else
			   if( xxx >= 0 && xxx < (width - xVacc))
			   {
				   *(pDraw[group]+offset) = 255 ;
			   }
			   else
			   {
				   //*(pDraw[group]+offset) = 0 ;
				   continue ;
			   }
			   tmpDrawRate = ( xxx / beam_width ) *  beam_qty ;
			   pAngleZoom[group][offset] = (unsigned char)tmpDrawRate ;
			   tmpDrawRate =  tmpDrawRate - (int)tmpDrawRate  ;
			   pDrawRate[group][offset] = (unsigned char)( tmpDrawRate * COLOR_STEP ) ;
			   pDataNo[group][offset]   = (int) DataLength * tmpDataNo ;

		   }
	   }
	   //keep the points for drawing current beam line
	   beam_width = beam_width / beam_qty ;
	   for(i = 0 ; i<= beam_qty ; i++)
	   {
		   if((_angle >=0 && (!bSkewReverse_)) || ((_angle < 0 && bSkewReverse_)))
		   {
		        CurrentLine_top[group][i].x = (int) ( beam_width * i  + 0.5) ;
		        CurrentLine_bottom[group][i].x = (int)( beam_width * i  + 0.5 + xVacc );
		   }
		   else
		   {
		        CurrentLine_bottom[group][i].x =  (int) ( beam_width * i + 0.5);
		        CurrentLine_top[group][i].x   = (int)( beam_width * i  + xVacc + 0.5);
		   }
		   CurrentLine_top[group][i].y = 0 ;
		   CurrentLine_bottom[group][i].y =	height - 1 ;
	   }

}

void CalcLinearScanNew(LINEAR_SCAN_INFO* pInfo_)
{
	   double real_width ;         // real coordinate width     ***  set element pitch to be 1 mm
	   double real_height;         // real coordinate height
	   double xScale     ;
	   double yScale     ;         // x y axis scale factor
	   double tan_angle_scaled ;   // tan value of reflect angle after scaled
	   int i ,  j   ;              // circle element
	   double xVacc ;              // x axis vacant distance

	   double tmpX  ;
	   double xxx   ;
	   int offset   ;
	   double tmpDrawRate ;
	   double tmpDataNo   ;
	   double beam_width  ;

	   float angle = pInfo_->fAngle   ;
	   //int beam_qty = pInfo_->nLawQty  ;
	   int beam_no_mx = pInfo_->nLawQty  - 1   ;
	   int group     = pInfo_->nGroupId ;
	   int bSkewReverse_ = pInfo_->direction ;
	   int width = pInfo_->nWidth  ;
	   int height= pInfo_->nHeight ;
	   int DataLength   = pInfo_->nPointQty ;
       real_height  = pInfo_->fRange * cos(angle) ;
       float _fScape = pInfo_->pExitPoint[beam_no_mx] - pInfo_->pExitPoint[0]  ;
       int _nProbeDirection = _fScape < 0 ? -1 : 0 ;
	   real_width   = fabs(pInfo_->fRange * sin(angle)) + fabs(_fScape) ;

	   memset(pDraw[group] , 0 , 605 * 390);
	   if(beam_no_mx == 0)
	   {
		   if((angle >=0 && (!bSkewReverse_)) || ((angle < 0 && bSkewReverse_)))
		   {
	           //****************************************************************************
	           //keep the location for angle line drawing
	           CurrentLine_bottom[group][0].x = width ;
	           CurrentLine_bottom[group][0].y = height ;

	           //keep the location for angle line drawing
	           CurrentLine_top[group][0].x = 0 ;
	           CurrentLine_top[group][0].y = 0 ;
		   }
		   else
		   {
	           //****************************************************************************
	           //keep the location for angle line drawing
	           CurrentLine_bottom[group][0].x = 0 ;
	           CurrentLine_bottom[group][0].y = height ;

	           //keep the location for angle line drawing
	           CurrentLine_top[group][0].x = width ;
	           CurrentLine_top[group][0].y = 0 ;
		   }
        //****************************************************************************
		   return ;
	   }

	   xScale = width / real_width ;
	   yScale = height/ real_height ;

	   xVacc = fabs(tan(angle) * real_height * xScale) ;
	   beam_width = width - xVacc ;
	   tan_angle_scaled =  xVacc / height ;
	   for( i = 0 ; i< height ; i++)
	   {
		   tmpX = i * tan_angle_scaled ;
		   tmpDataNo = ((double)i)/ height ;
		   for( j = 0 ; j< width ; j++ )
		   {
			   if(!bSkewReverse_)
				   offset  = j + i * width ;
			   else
				   offset  = width - j + i * width ;
			   if(angle < 0)
				   xxx = j - xVacc + tmpX ;
			   else
				   xxx = j -  tmpX ;
			   if( xxx >= 0 && xxx < (width - xVacc))
			   {
				   *(pDraw[group]+offset) = 255 ;
			   }
			   else
			   {
				   continue ;
			   }
			   tmpDrawRate = ( xxx / beam_width ) *  beam_no_mx ;

               if(_nProbeDirection)
               {
            	   pAngleZoom[group][offset] = beam_no_mx - (unsigned char)tmpDrawRate ;
            	   tmpDrawRate =  (int)(tmpDrawRate + 1 ) - tmpDrawRate  ;
               }
               else
               {
				   pAngleZoom[group][offset] = (unsigned char)tmpDrawRate ;
				   tmpDrawRate =  tmpDrawRate - (int)tmpDrawRate  ;
               }
			   pDrawRate[group][offset] = (unsigned char)( tmpDrawRate * COLOR_STEP ) ;
			   pDataNo[group][offset]   = (int) DataLength * tmpDataNo ;
		   }
	   }
	   //keep the points for drawing current beam line
	   beam_width = beam_width / beam_no_mx ;
       int _nBeamNo ;
	   for(i = 0 ; i<= beam_no_mx ; i++)
	   {
		   if(_nProbeDirection) _nBeamNo = beam_no_mx - i;
		   else _nBeamNo = i ;
		   if((angle >=0 && (!bSkewReverse_)) || ((angle < 0 && bSkewReverse_)))
		   {
		        CurrentLine_top[group][_nBeamNo].x = (int) ( beam_width * i  + 0.5) ;
		        CurrentLine_bottom[group][_nBeamNo].x = (int)( beam_width * i  + 0.5 + xVacc );
		   }
		   else
		   {
		        CurrentLine_bottom[group][_nBeamNo].x =  (int) (width - beam_width * i - xVacc + 0.5);
		        CurrentLine_top[group][_nBeamNo].x   = (int)(width - beam_width * i + 0.5);
		   }
		   CurrentLine_top[group][_nBeamNo].y = 0 ;
		   CurrentLine_bottom[group][_nBeamNo].y =	height - 1 ;
	   }
}

/* 画S扫描 角度扫查 */
void _DrawSscanSoundPath (unsigned short *p, unsigned int width, unsigned int height, DOT_TYPE *data,
		unsigned int xoffset, unsigned int yoffset, unsigned char nGroupId_ )
{
	int i, j, k, temp;
	int beam_qty = TMP(beam_qty[nGroupId_]);
	int dot_qty  = pp->nPointQty[nGroupId_];
    unsigned char* pData ;
    unsigned char* buff  ;

    unsigned short* pColor;
    if(pp->p_config->group[nGroupId_].rectifier1)
	        pColor = TMP(color_amp)  ;
    else
    	    pColor = TMP(color_tofd) ;

	for(i = 0; i< beam_qty; i++)
	{
		pData = data + i * (dot_qty + 32) ;
		buff  = TMP(scan_data[nGroupId_])+width*i ;
		getDisplayData( pData, buff, dot_qty, width, GROUP_VAL_POS(nGroupId_ , rectifier1) == RF_WAVE);
	}

	if (height < beam_qty)
	{
		/* 压缩s扫描 */
		for (i = 0 ; i < height ; i++)
		{
			temp = width * i * beam_qty / height;
			for (k = 0; k < width - 1; k++)
			{
			   fbdot (p, xoffset + k, yoffset + i,
							pColor[TMP(scan_data[nGroupId_])[temp + k]]);
			}
		}
	}
	else if (height == beam_qty)
	{
		/* 不变 */
		for (i = 0; i < beam_qty; i++)
				for (k = 0; k < width - 1; k++)
					fbdot (p, xoffset + k, yoffset + i,
							pColor[TMP(scan_data[nGroupId_])[width * i + k]]);
	}
	else if (height > beam_qty)
	{
		/* 拉伸 */
		for (i = 0; i < beam_qty; i++)
			for (j = 0; j <= height / beam_qty; j++)
				for (k = 0; k < width - 1; k++)
				{
					fbdot (p, xoffset + k, yoffset + i * height / beam_qty + j,
							pColor[TMP(scan_data[nGroupId_])[width * i + k]]);
				}
	}


	return ;
}

void DrawCalibrationWedgeDelayLine (unsigned short *p, int width, int height, int xoffset,  int yoffset, unsigned char nGroupId_ ,
		unsigned int* pData , int beam_offset ,unsigned char threshold)
{
	int	i;
	int 	clb_x1, clb_x2;
	int 	clb_y1, clb_y2;
	double  s, s_1;
	unsigned int* pGate ;
	int temp_value ;
	int _nIndex ;
	int _nStart  ,  _nStep ;
	int _nGateSpace ;
	GetCalibrationStartAndStepLaw(nGroupId_ , &_nStart , &_nStep ) ;
	// clear frame buffer back ground
	for (i = 0; i < height; i++)
		memset (p + FB_WIDTH * (i + yoffset) + xoffset, 0x0, width * 2 );

	// get gate information
	for( i = _nStart ; i < _nStart + _nStep ; i++)
	{
		pGate++ ; // Gate a is offset 4 byte
		_nIndex = beam_offset + i  ;
	    temp_value = (pData[ _nIndex * 8 + 1 ] >> 20) & 0xfff ;
	    //((_nData>>20) & 0x00000fff) /20.47;//满屏时200% 4095
		// if DB is bigger than the max DB in history
	    // keep and refresh the gate position information
	    if(temp_value >= TMP(clb_wedge_max_DB[i]) && temp_value >= threshold)
	    {
	    	TMP(clb_wedge_max_DB[i]) = temp_value ;
	    	TMP(clb_wedge_position[i]) = pData[ _nIndex * 8 + 1 ] & 0xfffff ;
	    }
	}
    width = width - 30 ;
    _nStep = _nStep - 1 ;
	for (i = 0 ; i <  _nStep ; i++)
	{
		_nIndex = i + _nStart  ;
		clb_x1 = 15 + i * width / _nStep  + xoffset ;
		clb_x2 = 15 + (i + 1) * width / _nStep + xoffset ;
		_nGateSpace = pp->gate_a_end[nGroupId_][_nIndex]  - pp->gate_a_start[nGroupId_][_nIndex]  ;
		s   = height * (TMP(clb_wedge_position[_nIndex]) - pp->gate_a_start[nGroupId_][_nIndex]) /  _nGateSpace;
		_nGateSpace = pp->gate_a_end[nGroupId_][_nIndex + 1] - pp->gate_a_start[nGroupId_][_nIndex + 1]   ;
		s_1 = height * (TMP(clb_wedge_position[_nIndex + 1])  - pp->gate_a_start[nGroupId_][_nIndex + 1]) / _nGateSpace ;

		if(s < 0)           s = 0;
		if(s >= height)     s = height - 1 ;
		if(s_1 < 0)         s_1 = 0;
		if(s_1 >= height)   s_1 = height - 1 ;

		clb_y1 = yoffset + height - s ;
		clb_y2 = yoffset + height - s_1 ;
		fbline (p, clb_x1, clb_y1, clb_x2, clb_y2, all_col_16[2]);//包络线
	}
}

/* 画灵敏度校准包络线 */
void DrawSensitivityCalibrationLine(unsigned short *p, int width, int height, int xoffset, int yoffset,
		   unsigned char nGroupId_ , unsigned int* pData, int beam_offset)
{
	int	i;
	int 	clb_x1, clb_x2;
	int 	clb_y1, clb_y2;

	unsigned int* pGate ;
	int temp_value[setup_MAX_GROUP_LAW_QTY] ;

	for (i = 0; i < height; i++)
		memset (p + FB_WIDTH * (i + yoffset) + xoffset, 0x0, width * 2 );

	int _nStart  ,  _nStep , _nIndex ;
	GetCalibrationStartAndStepLaw(nGroupId_ , &_nStart , &_nStep ) ;

	// get gate information
	for( i = _nStart ; i < _nStart + _nStep ; i++)
	{
		pGate++ ; // Gate a is offset 4 byte
	    temp_value[i] = (int)((pData[ (beam_offset + i) * 8 + 1 ] >> 20) & 0xfff) ;
	    // if DB is bigger than the max DB in history
	    // keep and refresh the gate position information
	    if (temp_value[i] > 0x7ff) temp_value[i] = 0x7ff ;
	    if(temp_value[i] >= TMP(clb_sensation_max_DB[i]))
	    {
	    	TMP(clb_sensation_max_DB[i]) = temp_value[i];
	    }
	}

    width = width - 30 ;
    _nStep = _nStep - 1 ;
	for (i = 0; i < _nStep ; i++)
	{
		_nIndex = _nStart + i;
		clb_x1 = 15 + i * width / _nStep  + xoffset ;
		clb_x2 = 15 + (i + 1) * width / _nStep + xoffset ;

		clb_y1 = height * temp_value[_nIndex] / MAX_DB_12_BIT ;
		clb_y2 = height * temp_value[_nIndex+1] / MAX_DB_12_BIT ;
		clb_y1 = yoffset + height - clb_y1 ;
		clb_y2 = yoffset + height - clb_y2 ;
		if(clb_y1 < yoffset ) clb_y1 = yoffset ;
		if(clb_y2 < yoffset ) clb_y1 = yoffset ;
		if(clb_y1 > yoffset + height) clb_y1 = yoffset + height ;
		if(clb_y2 > yoffset + height) clb_y2 = yoffset + height ;
		fbline (p, clb_x1, clb_y1, clb_x2, clb_y2, all_col_16[0]);//包络线

		clb_y1 = height * TMP(clb_sensation_max_DB[_nIndex]) / MAX_DB_12_BIT ;
		clb_y2 = height * TMP(clb_sensation_max_DB[_nIndex + 1]) / MAX_DB_12_BIT ;
		clb_y1 = yoffset + height - clb_y1 ;
		clb_y2 = yoffset + height - clb_y2 ;
		if(clb_y1 < yoffset ) clb_y1 = yoffset ;
		if(clb_y2 < yoffset ) clb_y1 = yoffset ;
		if(clb_y1 > yoffset + height) clb_y1 = yoffset + height ;
		if(clb_y2 > yoffset + height) clb_y2 = yoffset + height ;
		fbline (p, clb_x1, clb_y1, clb_x2, clb_y2, all_col_16[2]);//包络线
	}
}

/* 画TCG校准包络线 */
void DrawTCGCalibrationLine(unsigned short *p, int width, int height, int xoffset, int yoffset,
		   unsigned char nGroupId_ , unsigned int* pData, int beam_offset)
{
	int	i;
	int 	clb_x1, clb_x2;
	int 	clb_y1, clb_y2;

	unsigned int* pGate ;
	int temp_value[setup_MAX_GROUP_LAW_QTY] ;

	for (i = 0; i < height; i++)
		memset (p + FB_WIDTH * (i + yoffset) + xoffset, 0x0, width * 2 );

	int _nStart  ,  _nStep , _nIndex ;
	GetCalibrationStartAndStepLaw(nGroupId_ , &_nStart , &_nStep ) ;
	// get gate information
	for( i = _nStart ; i < _nStart + _nStep ; i++)
	{
		pGate++ ; // Gate a is offset 4 byte
	    temp_value[i] = (int)((pData[ (beam_offset + i) * 8 + 1 ] >> 20) & 0xfff) ;
	    // if DB is bigger than the max DB in history
	    // keep and refresh the gate position information
	    // if (temp_value[i] > 0x7ff) temp_value[i] = 0x7ff ;
	    if(temp_value[i] >= TMP(clb_tcg_max_DB[i]))
	    {
	    	TMP(clb_tcg_max_DB[i]) = temp_value[i];
	    	TMP(clb_tcg_position[i]) = (int)(pData[ (beam_offset + i) * 8 + 1 ]  & 0xfffff) ;
	    }
	}

    width = width - 30 ;
    _nStep = _nStep - 1 ;
    int _nTmpValue1 , _nTmpValue2 ;
	for (i = 0; i < _nStep ; i++)
	{
		_nIndex = _nStart + i;
		clb_x1 = 15 + i * width / _nStep  + xoffset ;
		clb_x2 = 15 + (i + 1) * width / _nStep + xoffset ;

		_nTmpValue1 = MIN(MAX_DB_12_BIT , temp_value[_nIndex]);
		_nTmpValue2 = MIN(MAX_DB_12_BIT ,  temp_value[_nIndex+1] );
		_nTmpValue1 = MAX(_nTmpValue1 , 0);
		_nTmpValue2 = MAX(_nTmpValue2 , 0);

		clb_y1 = height * _nTmpValue1 / MAX_DB_12_BIT ;
		clb_y2 = height * _nTmpValue2 / MAX_DB_12_BIT ;

		//clb_y1 = height * temp_value[_nIndex] / MAX_DB_12_BIT ;
		//clb_y2 = height * temp_value[_nIndex+1] / MAX_DB_12_BIT ;
		clb_y1 = yoffset + height - clb_y1 ;
		clb_y2 = yoffset + height - clb_y2 ;
		//if(clb_y1 < yoffset ) clb_y1 = yoffset ;
		//if(clb_y2 < yoffset ) clb_y1 = yoffset ;
		//if(clb_y1 > yoffset + height) clb_y1 = yoffset + height ;
		//if(clb_y2 > yoffset + height) clb_y2 = yoffset + height ;
		fbline (p, clb_x1, clb_y1, clb_x2, clb_y2, all_col_16[0]);//包络线

		_nTmpValue1 = MIN(MAX_DB_12_BIT , TMP(clb_tcg_max_DB[_nIndex]) );
		_nTmpValue2 = MIN(MAX_DB_12_BIT , TMP(clb_tcg_max_DB[_nIndex + 1]) );
		_nTmpValue1 = MAX(_nTmpValue1 , 0);
		_nTmpValue2 = MAX(_nTmpValue2 , 0);

		clb_y1 = height * _nTmpValue1 / MAX_DB_12_BIT ;
		clb_y2 = height * _nTmpValue2 / MAX_DB_12_BIT ;
		clb_y1 = yoffset + height - clb_y1 ;
		clb_y2 = yoffset + height - clb_y2 ;
		//if(clb_y1 < yoffset ) clb_y1 = yoffset ;
		//if(clb_y2 < yoffset ) clb_y1 = yoffset ;
		//if(clb_y1 > yoffset + height) clb_y1 = yoffset + height ;
		//if(clb_y2 > yoffset + height) clb_y2 = yoffset + height ;
		fbline (p, clb_x1, clb_y1, clb_x2, clb_y2, all_col_16[2]);//包络线
	}
}

#include "lzk/fft.h"
int g_nDrawFftContexStart[640] ;
int g_nDrawFftContexEnd[640] ;
void DrawFft(unsigned short* pFrameBuffer_, int width , int height , unsigned char* pData , int nOffsetX_, int nOffsetY_ , unsigned char nGroupId_)
{
	int i , j ;
	int _nProStart , _nProEnd ;
	int _nCurrentStart , _nCurrentEnd ;
	int _nTmp1 , _nTmp2 ;
	int _nPointer ;
	unsigned short _nColor;
	int temp;
	switch((temp = get_fft_color(pp->p_config)))
	{
	case 0:
		_nColor = 0xffe0;//yellow;
		break;
	case 1:
		_nColor = 0x001f;//blue;
		break;
	case 2:
		_nColor = 0xffff;//white;
		break;
	case 3:
	default:
		_nColor = 0x0000;//black;
		break;
	}

	for(i = 0 ; i < width ; i++)
	{
		_nProStart = g_nDrawFftContexStart[i]  ;
		_nProEnd   = g_nDrawFftContexEnd[i]    ;
		_nTmp1 = height - ((height * pData[i]) / 255 ) ;
		_nTmp2   = height - ((height * pData[i + 1]) / 255 );
		_nCurrentStart = MIN(_nTmp1 , _nTmp2) ;
		_nCurrentEnd   = MAX(_nTmp1 , _nTmp2) ;

		for(j = g_nDrawFftContexStart[i] ;j <= g_nDrawFftContexEnd[i] ;++j)
		{
			_nPointer = (j + nOffsetY_) * FB_WIDTH  + i  + nOffsetX_ ;
			pFrameBuffer_[ _nPointer ]    = 0 ;
		}
		for(j = _nCurrentStart ;j <= _nCurrentEnd ;++j)
		{
			_nPointer = (j + nOffsetY_) * FB_WIDTH  + i  + nOffsetX_ ;
			pFrameBuffer_[ _nPointer ]    = _nColor;
		}

		g_nDrawFftContexStart[i]  = _nCurrentStart ;
		g_nDrawFftContexEnd[i]    = _nCurrentEnd   ;
	}
}

void DrawFftAscan(unsigned char WinIndex, unsigned short *pFrameBuffer , unsigned int pBeamData)
{
	unsigned char group     ;
	int xoff , yoff , width , height       ;
	group = TMP(_scan_group[WinIndex])     ;
	xoff = TMP(_scan_xpos[WinIndex])       ;
	yoff = TMP(_scan_ypos[WinIndex])       ;
	width = TMP(_scan_width[WinIndex])     ;
	height = TMP(_scan_height[WinIndex])   ;

	int ascanGateAStart = pp->draw_area[0].scanEx.gateDataA.start;//第一个窗口为ASCAN窗口
	int ascanGateAEnd = pp->draw_area[0].scanEx.gateDataA.end;
//下面这些条件下不画FFT,起点终点不在闸门内
	static char EnableRefreshDraw = TRUE;
	if( (ascanGateAStart < 0)
		||(ascanGateAStart > width)
		||(ascanGateAEnd < 0)
		||(ascanGateAEnd > width)
		)
	{
		if(EnableRefreshDraw)
		{
			pp->bRefreshDraw = TRUE;
			EnableRefreshDraw = FALSE;
		}
		return;
	}
	EnableRefreshDraw = TRUE;

	int FftGateAStart ,FftGateAWidth;
//挑选要转换的点
//	FftGateAStart = 0;
//	FftGateAWidth = pp->nPointQty[group];
	FftGateAStart = pp->nPointQty[group] * ascanGateAStart / width;
	FftGateAWidth = pp->nPointQty[group] * (ascanGateAEnd - ascanGateAStart) / width;
	if(FftGateAWidth > 0)
	{
		unsigned char* pData ;
		int k , offset ;
		for (offset = 0, k = 0 ; k < group ; k++)
		{
			offset += (pp->nPointQty[k] + 32) * TMP(beam_qty[k]);
		}
		pData = (unsigned char *)(pBeamData + offset );

		// beam data start address
		pData = pData + TMP(beam_num[group]) * (pp->nPointQty[group] + 32);

	//	unsigned char* temp_data = (unsigned char*)malloc(width * sizeof(unsigned char));
		unsigned char temp_data[640];
		memset(temp_data ,100 , 640);
	//	if(NULL == temp_data)
	//		return;
	//开始画FFT
		struct scanData_t in ,out;
		in.data = pData + FftGateAStart;
		in.numOfData = FftGateAWidth;
		out.data = temp_data;
		out.numOfData = width;
		fft(&gData->fft[group], &in ,&out);

		if (pp->bRefreshDraw)
		{
			memset(g_nDrawFftContexStart , 0 , 640 * 4);
			memset(g_nDrawFftContexEnd , 0 , 640 * 4);
		}
		DrawFft (pFrameBuffer, width, height ,temp_data, xoff, yoff, group);
	//	free(temp_data);
		//更新计算值
		gdk_threads_enter();
		gtk_widget_queue_draw (pp->vboxtable);
		gdk_threads_leave();
	}
}

void DrawAscanHorizontal(unsigned char WinIndex, unsigned short *pFrameBuffer , unsigned int pBeamData)
{
	unsigned char* pData ;
	int k , offset ;
	unsigned char group     ;
	int xoff , yoff , width , height       ;

	group = TMP(_scan_group[WinIndex])     ;
	xoff = TMP(_scan_xpos[WinIndex])       ;
	yoff = TMP(_scan_ypos[WinIndex])       ;
	width = TMP(_scan_width[WinIndex])     ;
	height = TMP(_scan_height[WinIndex])   ;

	if (pp->bRefreshDraw)
	{
		ascanClearAndRedraw(group);
		return;
	}

	for (offset = 0 ,k = 0 ;k < group ;k++)
	{
		offset += (pp->nPointQty[k] + 32) * TMP(beam_qty[k]);
	}
	pData = (unsigned char*)(pBeamData + offset);

	// beam data start address
	pData = pData + TMP(beam_num[group]) * (pp->nPointQty[group] + 32);

	//add by lzk
	gAscanData[WinIndex].wave.color = all_col_16[GROUP_VAL_POS(group, ascan_color)];
	gAscanData[WinIndex].wave.rfMode = !GROUP_VAL_POS(group , rectifier1);
	gAscanData[WinIndex].wave.fillMode = GROUP_VAL_POS(group , ascan_appearance);
	gAscanData[WinIndex].wave.start = 0;
	gAscanData[WinIndex].wave.end = width;
	gAscanData[WinIndex].wave.pData = pData;
	gAscanData[WinIndex].wave.dataPoints = pp->nPointQty[group];

	gAscanData[WinIndex].envelope.mode = GROUP_VAL_POS(group , ascan_envelope);

	gAscanData[WinIndex].ref.mode = GROUP_VAL_POS(group , ascan_overlay);

	if(
		(gAscanData[WinIndex].gateDataA.height != pp->draw_area[WinIndex].scanEx.gateDataA.height)
	||	(gAscanData[WinIndex].gateDataA.start != pp->draw_area[WinIndex].scanEx.gateDataA.start)
	||	(gAscanData[WinIndex].gateDataA.end != pp->draw_area[WinIndex].scanEx.gateDataA.end)

	)
	{
		ascanClearDisp(group);
	}
	gAscanData[WinIndex].axis.direction = DIRECTION_HORIZONTAL;
	gAscanData[WinIndex].axis.horizontal = width;
	gAscanData[WinIndex].axis.vertical = height;
	gAscanData[WinIndex].axis.x = xoff;
	gAscanData[WinIndex].axis.y = yoff;

	gAscanData[WinIndex].gateDataA.start = pp->draw_area[WinIndex].scanEx.gateDataA.start;
	gAscanData[WinIndex].gateDataA.end = pp->draw_area[WinIndex].scanEx.gateDataA.end;
	gAscanData[WinIndex].gateDataA.height = pp->draw_area[WinIndex].scanEx.gateDataA.height;

	unsigned int pMeasureData[8];
	memcpy(pMeasureData ,pData + pp->nPointQty[group] ,32);
//	unsigned int* pMeasureData = (unsigned int*)((unsigned int)(pData + pp->nPointQty[group]) & 0xfffffffc);
	if(gAscanData[WinIndex].wave.rfMode)
	{
		gAscanData[WinIndex].gateAmplitude = (signed short)(pMeasureData[1] >> 16);
		gAscanData[WinIndex].gateAmplitude /= 16;
	}
	else
	{
		unsigned int gateAmplitude = pMeasureData[1] >> 20;
		gAscanData[WinIndex].gateAmplitude = gateAmplitude;
	}
	gAscanData[WinIndex].group = group;
	GROUP *pGroup = get_group_by_id (pp->p_config, group);
	if(
        (gAscanData[WinIndex].start != group_get_start(group))
    ||	(gAscanData[WinIndex].range != group_get_range(group))
	)
	{
		ascanResetCurvesWhenStartOrRangeChanged(group);
	}
    gAscanData[WinIndex].start = group_get_start(group);
    gAscanData[WinIndex].range = group_get_range(group);

	ascanDrawWave(pFrameBuffer ,&gAscanData[WinIndex]);
//	_ascanDrawWave(pFrameBuffer ,&gAscanData[WinIndex]);
}

void DrawAscanVertical(unsigned char WinIndex, unsigned short *pFrameBuffer , unsigned int pBeamData)
{
	unsigned char* pData ;
	int k , offset ;
	unsigned char group     ;
	int xoff , yoff , width , height       ;

	group = TMP(_scan_group[WinIndex])     ;
	xoff = TMP(_scan_xpos[WinIndex])       ;
	yoff = TMP(_scan_ypos[WinIndex])       ;
	width = TMP(_scan_width[WinIndex])     ;
	height = TMP(_scan_height[WinIndex])   ;

	if (pp->bRefreshDraw)
	{
		ascanClearAndRedraw(group);
		return;
	}

	for (offset = 0, k = 0 ; k < group ; k++)
	{
		offset += (pp->nPointQty[k] + 32) * TMP(beam_qty[k]);
	}
	pData = (unsigned char *)(pBeamData + offset );
	// beam data start address
	pData = pData + TMP(beam_num[group]) * (pp->nPointQty[group] + 32);
	// draw a scan line position need the result calculated in the s scan calculation

	//add by lzk
	gAscanData[WinIndex].wave.color = all_col_16[GROUP_VAL_POS(group, ascan_color)];
	gAscanData[WinIndex].wave.rfMode = !GROUP_VAL_POS(group , rectifier1);
	gAscanData[WinIndex].wave.fillMode = GROUP_VAL_POS(group , ascan_appearance);
	gAscanData[WinIndex].wave.start = AScanDrawRange[group][TMP(beam_num[group])].start;
	gAscanData[WinIndex].wave.end = AScanDrawRange[group][TMP(beam_num[group])].end;
	gAscanData[WinIndex].wave.pData = pData;
	gAscanData[WinIndex].wave.dataPoints = pp->nPointQty[group];

	gAscanData[WinIndex].envelope.mode = GROUP_VAL_POS(group , ascan_envelope);

	gAscanData[WinIndex].ref.mode = GROUP_VAL_POS(group , ascan_overlay);

	if(
		(gAscanData[WinIndex].gateDataA.height != pp->draw_area[WinIndex].scanEx.gateDataA.height)
	||	(gAscanData[WinIndex].gateDataA.start != pp->draw_area[WinIndex].scanEx.gateDataA.start)
	||	(gAscanData[WinIndex].gateDataA.end != pp->draw_area[WinIndex].scanEx.gateDataA.end)

	)
	{
		ascanClearDisp(group);
	}

	gAscanData[WinIndex].axis.direction = DIRECTION_VERTICAL;
	gAscanData[WinIndex].axis.horizontal = height;
	gAscanData[WinIndex].axis.vertical = width;
	gAscanData[WinIndex].axis.x = xoff;
	gAscanData[WinIndex].axis.y = yoff;

	gAscanData[WinIndex].gateDataA.start = pp->draw_area[WinIndex].scanEx.gateDataA.start;
	gAscanData[WinIndex].gateDataA.end = pp->draw_area[WinIndex].scanEx.gateDataA.end;
	gAscanData[WinIndex].gateDataA.height = pp->draw_area[WinIndex].scanEx.gateDataA.height;

	unsigned int pMeasureData[8];
	memcpy(pMeasureData ,pData + pp->nPointQty[group] ,32);
	if(gAscanData[WinIndex].wave.rfMode)
	{
		gAscanData[WinIndex].gateAmplitude = (signed short)(pMeasureData[1] >> 16);
		gAscanData[WinIndex].gateAmplitude /= 16;
	}
	else
	{
		unsigned int gateAmplitude = pMeasureData[1] >> 20;
		gAscanData[WinIndex].gateAmplitude = gateAmplitude;
	}
	gAscanData[WinIndex].group = group;
	GROUP *p_grp = get_group_by_id (pp->p_config, group);
	if(
        (gAscanData[WinIndex].start != group_get_start(group))
    ||	(gAscanData[WinIndex].range != group_get_range(group))
	)
	{
		ascanResetCurvesWhenStartOrRangeChanged(group);
	}
    gAscanData[WinIndex].start = group_get_start(group);
    gAscanData[WinIndex].range = group_get_range(group);

	ascanDrawWave(pFrameBuffer ,&gAscanData[WinIndex]);

}

void ScanRefreshDrawFrame(int nWinIndex_)
{
	static unsigned int _nCounter = 0 ;
	if(_nCounter > 3)
	{
		_nCounter  =  0 ;
		DRAW_AREA_P _pDrawArea =  &pp->draw_area[nWinIndex_]  ;
		gdk_threads_enter();
		gtk_widget_queue_draw (_pDrawArea->drawing_area);
		gdk_threads_leave();

		return ;
	}
	_nCounter++ ;
}
void BscanRefreshScanRuler(int nWinIndex_)
{
	static unsigned int _nCounter = 0 ;
	if(_nCounter > 3)
	{
		_nCounter  =  0 ;
		//if(g_BscanDrawInfo.nPosRecorderStart == g_BscanDrawInfo.nShowStart &&
		//     g_BscanDrawInfo.nPosRecorderStop == g_BscanDrawInfo.nShowStop)
		//{
		//	return ;
		//}
		g_BscanDrawInfo.nPosRecorderStart = g_BscanDrawInfo.nShowStart    ;
	    g_BscanDrawInfo.nPosRecorderStop  = g_BscanDrawInfo.nShowStop     ;
	    DRAW_AREA_P _pDrawArea =  &pp->draw_area[nWinIndex_]  ;
	    double _nScanOffset  = GROUP_VAL_POS(g_BscanDrawInfo.nGroupId , scan_offset) / 10.0;
	    double _nScanStart   = pp->p_config->inspection_scan_start/1000.0   ;
	    double _nScanStop    = pp->p_config->inspection_scan_end  /1000.0   ;
	    double _nScanDist    = _nScanStop - _nScanStart  ;
	    _pDrawArea->hmax1    = _nScanOffset + _nScanStart + _nScanDist * g_BscanDrawInfo.nShowStart / g_BscanDrawInfo.nTotalSteps  ;
	    _pDrawArea->hmin1    = _nScanOffset + _nScanStart + _nScanDist * g_BscanDrawInfo.nShowStop  / g_BscanDrawInfo.nTotalSteps  ;

		gdk_threads_enter();
		gtk_widget_queue_draw (_pDrawArea->drawing_area);
		gdk_threads_leave();

		return ;
	}
	_nCounter++ ;
}

static pthread_mutex_t bscanRefeshDrawMutex = PTHREAD_MUTEX_INITIALIZER;
static int bscanRefeshDraw = FALSE;
inline void bscan_set_refresh(int flag)
{
    pthread_mutex_lock(&bscanRefeshDrawMutex);
    bscanRefeshDraw = flag;
    pthread_mutex_unlock(&bscanRefeshDrawMutex);
}
static inline gboolean bscan_need_refresh()
{
    int ret = FALSE;
    pthread_mutex_lock(&bscanRefeshDrawMutex);
    ret = bscanRefeshDraw;
    pthread_mutex_unlock(&bscanRefeshDrawMutex);
    return ret;
}

void DrawBscan(int WinIndex, unsigned short *pFrameBuffer , unsigned int pBeamData)
{
    if (pp->bRefreshDraw || bscan_need_refresh())
	{
		DrawBscanInit(WinIndex);
		return ;
	}

	if(pp->nEncoderType)  // timer
	{
		int _nEncoderIndex = TMP(measure_data[0][4]) ;
		int _nDataIndex = _nEncoderIndex / g_BscanDrawInfo.nEncoderResolution + g_BscanDrawInfo.nZeroOffset ;
		if (_nDataIndex >= g_BscanDrawInfo.nTotalSteps )  _nDataIndex = g_BscanDrawInfo.nTotalSteps -1  ;
	    if ( _nDataIndex < 0) _nDataIndex = 0 ;

		if( g_BscanDrawInfo.nWndType)
		{
			DrawBscanEncoderScrollWindow (pFrameBuffer , _nDataIndex)  ;
			BscanRefreshScanRuler(WinIndex);
		}
		else
		{
			DrawBscanEncoderStaticWindow (pFrameBuffer , _nDataIndex)  ;
			if(TMP(dataRecalling))  ScanRefreshDrawFrame(WinIndex) ;
		}
		//BscanRefreshScanRuler(WinIndex);
	}
	else /* 编码器同步 */
	{
		DrawBScanTimer   (pFrameBuffer)  ;
		if(TMP(dataRecalling))  ScanRefreshDrawFrame(WinIndex) ;
	}

}


void TofdBscanRefreshScanRuler(int nWinIndex_)
{
	static unsigned int _nCounter = 0 ;
	TOFDB_DRAW_INFO_P pTofdBscanDrawInfo = &(g_TofdBscanDrawInfo[nWinIndex_]);
	if(_nCounter > 3)
	{
		_nCounter  =  0 ;
		//if(pTofdBscanDrawInfo->nPosRecorderStart == pTofdBscanDrawInfo->nShowStart &&
		//		pTofdBscanDrawInfo->nPosRecorderStop == pTofdBscanDrawInfo->nShowStop)
		//{
		//	return ;
		//}
		pTofdBscanDrawInfo->nPosRecorderStart = pTofdBscanDrawInfo->nShowStart    ;
		pTofdBscanDrawInfo->nPosRecorderStop  = pTofdBscanDrawInfo->nShowStop     ;
	    DRAW_AREA_P _pDrawArea =  &pp->draw_area[nWinIndex_]  ;
	    double _nScanOffset  = GROUP_VAL_POS(pTofdBscanDrawInfo->nGroupId , scan_offset) / 10.0;
	    double _nScanStart   = pp->p_config->inspection_scan_start/1000.0   ;
	    double _nScanStop    = pp->p_config->inspection_scan_end  /1000.0   ;
	    double _nScanDist    = _nScanStop - _nScanStart  ;
	    _pDrawArea->wmin1    = _nScanOffset + _nScanStart + _nScanDist * pTofdBscanDrawInfo->nShowStart / pTofdBscanDrawInfo->nTotalSteps  ;
	    _pDrawArea->wmax1    = _nScanOffset + _nScanStart + _nScanDist * pTofdBscanDrawInfo->nShowStop  / pTofdBscanDrawInfo->nTotalSteps  ;

		gdk_threads_enter();
		gtk_widget_queue_draw (_pDrawArea->drawing_area);
		gdk_threads_leave();

		return ;
	}
	_nCounter++ ;
}

void TofdBscanRefreshScanRulerVer(int nWinIndex_)
{
    static unsigned int _nCounter = 0 ;
    TOFDB_DRAW_INFO_P pTofdBscanDrawInfo = &(g_TofdBscanDrawInfo[nWinIndex_]);
    if(_nCounter > 3) {
        _nCounter  =  0 ;
        pTofdBscanDrawInfo->nPosRecorderStart = pTofdBscanDrawInfo->nShowStart    ;
        pTofdBscanDrawInfo->nPosRecorderStop  = pTofdBscanDrawInfo->nShowStop     ;
        DRAW_AREA_P _pDrawArea =  &pp->draw_area[nWinIndex_]  ;
        double _nScanOffset  = GROUP_VAL_POS(pTofdBscanDrawInfo->nGroupId , scan_offset) / 10.0;
        double _nScanStart   = pp->p_config->inspection_scan_start/1000.0   ;
        double _nScanStop    = pp->p_config->inspection_scan_end  /1000.0   ;
        double _nScanDist    = _nScanStop - _nScanStart  ;
        _pDrawArea->hmax1    = _nScanOffset + _nScanStart + _nScanDist * pTofdBscanDrawInfo->nShowStart / pTofdBscanDrawInfo->nTotalSteps  ;
        _pDrawArea->hmin1    = _nScanOffset + _nScanStart + _nScanDist * pTofdBscanDrawInfo->nShowStop  / pTofdBscanDrawInfo->nTotalSteps  ;
        
        gdk_threads_enter();
        gtk_widget_queue_draw (_pDrawArea->drawing_area);
        gdk_threads_leave();
        
        return ;
    }
    _nCounter++ ;
}

void DrawBscanTOFD(int WinIndex, unsigned short *pFrameBuffer , unsigned int pBeamData)
{
    if (pp->bRefreshDraw || bscan_need_refresh())
	{
		DrawBscanTOFDInit(WinIndex);
		return ;
	}
	TOFDB_DRAW_INFO_P pTofdBscanDrawInfo = &(g_TofdBscanDrawInfo[WinIndex]);
	if(pp->nEncoderType)  // timer
	{
		int _nEncoderIndex = TMP(measure_data[0][4]) ;
		int _nDataIndex = _nEncoderIndex / pTofdBscanDrawInfo->nEncoderResolution + pTofdBscanDrawInfo->nZeroOffset ;
		if (_nDataIndex >= pTofdBscanDrawInfo->nTotalSteps )_nDataIndex = pTofdBscanDrawInfo->nTotalSteps  - 1;
		if (_nDataIndex < 0)  _nDataIndex  =  0 ;

		if( pTofdBscanDrawInfo->nWndType)
		{
    		DrawBscanTOFDEncoderScrollWindow (pFrameBuffer , _nDataIndex , WinIndex)  ;
	    	TofdBscanRefreshScanRuler(WinIndex)  ;
		}
		else
		{
			DrawBscanTOFDEncoderStaticWindow (pFrameBuffer , _nDataIndex , WinIndex)  ;
			if(TMP(dataRecalling))  ScanRefreshDrawFrame(WinIndex) ;
		}
	}
	else /* 编码器同步 */
	{
		DrawBScanTOFDTimer   (pFrameBuffer , WinIndex)  ;
		if(TMP(dataRecalling))  ScanRefreshDrawFrame(WinIndex) ;
	}

}

void DrawBscanTOFDVer(int WinIndex, unsigned short *pFrameBuffer , unsigned int pBeamData)
{
    if (pp->bRefreshDraw || bscan_need_refresh())
	{
		DrawBscanTOFDInit(WinIndex);
		return ;
	}
	TOFDB_DRAW_INFO_P pTofdBscanDrawInfo = &(g_TofdBscanDrawInfo[WinIndex]);
	if(pp->nEncoderType)  // timer
	{
		int _nEncoderIndex = TMP(measure_data[0][4]) ;
		int _nDataIndex = _nEncoderIndex / pTofdBscanDrawInfo->nEncoderResolution + pTofdBscanDrawInfo->nZeroOffset ;
		if (_nDataIndex >= pTofdBscanDrawInfo->nTotalSteps )_nDataIndex = pTofdBscanDrawInfo->nTotalSteps  - 1;
		if (_nDataIndex < 0)  _nDataIndex  =  0 ;

		if( pTofdBscanDrawInfo->nWndType)
		{
            DrawBscanTOFDEncoderScrollWindowVer(pFrameBuffer , _nDataIndex , WinIndex);
            TofdBscanRefreshScanRulerVer(WinIndex);
		} else {
			DrawBscanTOFDEncoderVerStaticWindow (pFrameBuffer , _nDataIndex , WinIndex)  ;
			if(TMP(dataRecalling))  ScanRefreshDrawFrame(WinIndex) ;
		}
	} else { /* 编码器同步 */
        DrawBScanTOFDTimerVertical(pFrameBuffer , WinIndex)  ;
		if(TMP(dataRecalling))  ScanRefreshDrawFrame(WinIndex) ;
	}
}


void CscanRefreshScanRuler(int nWinIndex_)
{
	static unsigned int _nCounter = 0 ;
	if(_nCounter > 3)
	{
		_nCounter  =  0 ;
		//if(g_CscanDrawInfo[nWinIndex_].nPosRecorderStart == g_CscanDrawInfo[nWinIndex_].nShowStart &&
		//		g_CscanDrawInfo[nWinIndex_].nPosRecorderStop == g_CscanDrawInfo[nWinIndex_].nShowStop)
		//{
		//	return ;
		//}
		g_CscanDrawInfo[nWinIndex_].nPosRecorderStart = g_CscanDrawInfo[nWinIndex_].nShowStart    ;
		g_CscanDrawInfo[nWinIndex_].nPosRecorderStop  = g_CscanDrawInfo[nWinIndex_].nShowStop     ;
	    DRAW_AREA_P _pDrawArea =  &pp->draw_area[nWinIndex_]  ;
	    double _nScanOffset  = GROUP_VAL_POS(g_CscanDrawInfo[nWinIndex_].nGroupId , scan_offset) / 10.0;
	    double _nScanStart   = pp->p_config->inspection_scan_start/1000.0   ;
	    double _nScanStop    = pp->p_config->inspection_scan_end  /1000.0   ;
	    double _nScanDist    = _nScanStop - _nScanStart  ;
	    _pDrawArea->wmin1    = _nScanOffset + _nScanStart + _nScanDist * g_CscanDrawInfo[nWinIndex_].nShowStart / g_CscanDrawInfo[nWinIndex_].nTotalSteps  ;
	    _pDrawArea->wmax1    = _nScanOffset + _nScanStart + _nScanDist * g_CscanDrawInfo[nWinIndex_].nShowStop  / g_CscanDrawInfo[nWinIndex_].nTotalSteps  ;

		gdk_threads_enter();
		gtk_widget_queue_draw (_pDrawArea->drawing_area);
		gdk_threads_leave();

		return ;
	}
	_nCounter++ ;
}

void CscanVerticalRefreshScanRuler(int nWinIndex_)
{
	static unsigned int _nCounter = 0 ;
	if(_nCounter > 3)
	{
		_nCounter  =  0 ;
		g_CscanDrawInfo[nWinIndex_].nPosRecorderStart = g_CscanDrawInfo[nWinIndex_].nShowStart    ;
		g_CscanDrawInfo[nWinIndex_].nPosRecorderStop  = g_CscanDrawInfo[nWinIndex_].nShowStop     ;
	    DRAW_AREA_P _pDrawArea =  &pp->draw_area[nWinIndex_]  ;
	    double _nScanOffset  = GROUP_VAL_POS(g_CscanDrawInfo[nWinIndex_].nGroupId , scan_offset) / 10.0;
	    double _nScanStart   = pp->p_config->inspection_scan_start/1000.0   ;
	    double _nScanStop    = pp->p_config->inspection_scan_end  /1000.0   ;
	    double _nScanDist    = _nScanStop - _nScanStart  ;
	    _pDrawArea->hmax1    = _nScanOffset + _nScanStart + _nScanDist * g_CscanDrawInfo[nWinIndex_].nShowStart / g_CscanDrawInfo[nWinIndex_].nTotalSteps  ;
	    _pDrawArea->hmin1    = _nScanOffset + _nScanStart + _nScanDist * g_CscanDrawInfo[nWinIndex_].nShowStop  / g_CscanDrawInfo[nWinIndex_].nTotalSteps  ;

		gdk_threads_enter();
		gtk_widget_queue_draw (_pDrawArea->drawing_area);
		gdk_threads_leave();

		return ;
	}
	_nCounter++ ;
}

void DrawCscan(int WinIndex, unsigned short *pFrame , unsigned int pBeamData , unsigned char nCScanSource_)
{

	if (pp->bRefreshDraw)
	{
		CScanDisplayInit (WinIndex , nCScanSource_) ;
		return ;
	}

	if(pp->nEncoderType)  // timer
	{
		int _nEncoderIndex = TMP(measure_data[0][4]) ;
		int _nDataIndex = _nEncoderIndex / g_CscanDrawInfo[WinIndex].nEncoderResolution + g_CscanDrawInfo[WinIndex].nZeroOffset ;
		if (_nDataIndex >= g_CscanDrawInfo[WinIndex].nTotalSteps)_nDataIndex = g_CscanDrawInfo[WinIndex].nTotalSteps  - 1;
		if (_nDataIndex < 0)                                     _nDataIndex = 0;
		if( g_CscanDrawInfo[WinIndex].nWndType)
		{
			CScanDisplayEncoderScrollWindow (pFrame ,  WinIndex , _nDataIndex)  ;
			CscanRefreshScanRuler(WinIndex) ;
		}
		else
		{
			CScanDisplayEncoderStaticWindow (pFrame ,  WinIndex , _nDataIndex)  ;
			if(TMP(dataRecalling))  ScanRefreshDrawFrame(WinIndex) ;
		}
	}
	else /* 编码器同步 */
	{
		CScanDisplayTimer ( pFrame , WinIndex)  ;
		if(TMP(dataRecalling))  ScanRefreshDrawFrame(WinIndex) ;
	}

}

void DrawCscanVer(int WinIndex, unsigned short *pFrame , unsigned int pBeamData , unsigned char nCScanSource_)
{

	if (pp->bRefreshDraw)
	{
        CScanVerDisplayInit(WinIndex , nCScanSource_);
		return ;
	}

	if(pp->nEncoderType)  // timer
	{
		int _nEncoderIndex = TMP(measure_data[0][4]) ;
		int _nDataIndex = _nEncoderIndex / g_CscanDrawInfo[WinIndex].nEncoderResolution + g_CscanDrawInfo[WinIndex].nZeroOffset ;
		if (_nDataIndex >= g_CscanDrawInfo[WinIndex].nTotalSteps)_nDataIndex = g_CscanDrawInfo[WinIndex].nTotalSteps  - 1;
		if (_nDataIndex < 0)                                     _nDataIndex = 0;
		if( g_CscanDrawInfo[WinIndex].nWndType)
		{
            CScanVerticalDisplayEncoderScrollWindow(pFrame ,  WinIndex , _nDataIndex)  ;
            CscanVerticalRefreshScanRuler(WinIndex);
		}
		else
		{
			CScanVertDisplayEncoderStaticWindow (pFrame ,  WinIndex , _nDataIndex)  ;
			if(TMP(dataRecalling))  ScanRefreshDrawFrame(WinIndex) ;
		}
	}
	else /* 编码器同步 */
	{
		CScanVertDisplayTimer ( pFrame , WinIndex)  ;
		if(TMP(dataRecalling))  ScanRefreshDrawFrame(WinIndex) ;
	}

}

void DrawSscanSoundPath(unsigned char WinIndex, unsigned short *pFrameBuffer , unsigned int pBeamData)
{
	unsigned char* pData ;
	int k , offset ;
	unsigned char group     ;
	int xoff , yoff , width , height       ;

	group = TMP(_scan_group[WinIndex])     ;
	if(GROUP_VAL_POS(group ,  group_mode) != PA_SCAN)
	{
		return ;
	}
	xoff   = TMP(_scan_xpos[WinIndex])     ;
	yoff   = TMP(_scan_ypos[WinIndex])     ;
	width  = TMP(_scan_width[WinIndex])    ;
	height = TMP(_scan_height[WinIndex])   ;

	for (offset = 0, k = 0 ; k < group ; k++)
	{
		offset += (pp->nPointQty[k] + 32) * TMP(beam_qty[k]);
	}
	pData = (unsigned char *)(pBeamData + offset );
	_DrawSscanSoundPath(pFrameBuffer, width, height, pData, xoff, yoff, group);
}

void DrawSscanAzimuthal(unsigned char WinIndex, unsigned short *pFrameBuffer , unsigned int pBeamData)
{
	unsigned char* pData ;
	int k , offset ;
	//unsigned char scan_type ;
	unsigned char group     ;
	int xoff , yoff , width , height       ;

    unsigned short* pColor;
    int _nPointQty ;
	group = TMP(_scan_group[WinIndex])     ;
	if(GROUP_VAL_POS(group ,  group_mode) != PA_SCAN)
	{
		return ;
	}
	//scan_type = TMP(_scan_type[WinIndex])  ;
	xoff = TMP(_scan_xpos[WinIndex])       ;
	yoff = TMP(_scan_ypos[WinIndex])       ;
	width = TMP(_scan_width[WinIndex])     ;
	height = TMP(_scan_height[WinIndex])   ;
    _nPointQty = pp->nPointQty[group] ;

	for (offset = 0, k = 0 ; k < group ; k++)
	{
		offset += (pp->nPointQty[k] + 32) * TMP(beam_qty[k]);
	}
	pData = (unsigned char *)(pBeamData + offset );

	if(GROUP_VAL_POS(group , rectifier1))
	        pColor = TMP(color_amp)  ;
    else
    	    pColor = TMP(color_tofd) ;

    DrawPixbuff(pFrameBuffer, xoff, yoff, width, height, _nPointQty ,  pData, group , pColor);
}

void DrawSscanlinear(unsigned char WinIndex, unsigned short *pFrameBuffer , unsigned int pBeamData)
{
	unsigned char* pData ;
	int k , offset ;
	//unsigned char scan_type ;
	unsigned char group     ;
	int xoff , yoff , width , height       ;
	//double RANGE    ;
	//double VELOCITY ;
	//double range ;
    unsigned short* pColor;
    int _nPointQty ;
	group = TMP(_scan_group[WinIndex])     ;
	if(GROUP_VAL_POS(group ,  group_mode) != PA_SCAN)
	{
		return ;
	}
	//scan_type = TMP(_scan_type[WinIndex])  ;
	xoff = TMP(_scan_xpos[WinIndex])       ;
	yoff = TMP(_scan_ypos[WinIndex])       ;
	width = TMP(_scan_width[WinIndex])     ;
	height = TMP(_scan_height[WinIndex])   ;
    _nPointQty = pp->nPointQty[group];

	for (offset = 0, k = 0 ; k < group ; k++)
	{
		offset += (pp->nPointQty[k] + 32) * TMP(beam_qty[k]);
	}
	pData = (unsigned char *)(pBeamData + offset );
	// chose color
	// maybe it is useless for there is a process in other place
    if(GROUP_VAL_POS(group , rectifier1))
	        pColor = TMP(color_amp)  ;
    else
    	    pColor = TMP(color_tofd) ;
    DrawPixbuff(pFrameBuffer, xoff, yoff, width, height, _nPointQty ,  pData, group , pColor);
}

void DrawAscanCalibration(unsigned char WinIndex, unsigned short *pFrameBuffer , unsigned int pBeamData)
{
	unsigned char* pData ;
	int k , offset ;
	unsigned char group     ;
	int xoff , yoff , width , height       ;
	int _nPointQty ;

	group     = TMP(_scan_group[WinIndex])    ;
	xoff 	  = TMP(_scan_xpos[WinIndex])     ;
	yoff      = TMP(_scan_ypos[WinIndex])     ;
	width     = TMP(_scan_width[WinIndex])    ;
	height    = TMP(_scan_height[WinIndex])   ;
	_nPointQty = pp->nPointQty[group] ;

	if (pp->bRefreshDraw)
	{
		ascanClearAndRedraw(group);
		return;
	}

	for (offset = 0, k = 0 ; k < group ; k++)
	{
		offset += (pp->nPointQty[k] + 32) * TMP(beam_qty[k]);
	}
	pData = (unsigned char *)(pBeamData + offset);
	// beam data start address
	pData = pData + TMP(beam_num[group]) * (_nPointQty + 32);

//	draw_a_scan_clb (pFrameBuffer, width, height, pData,  xoff, yoff, _nPointQty , group,
//			AScanDrawRange[group][TMP(beam_num[group])].start ,AScanDrawRange[group][TMP(beam_num[group])].end );

	//add by lzk
	gAscanData[WinIndex].wave.color = all_col_16[GROUP_VAL_POS(group, ascan_color)];
	gAscanData[WinIndex].wave.rfMode = !GROUP_VAL_POS(group , rectifier1);
	gAscanData[WinIndex].wave.fillMode = 0;//hollow
	gAscanData[WinIndex].wave.start = AScanDrawRange[group][TMP(beam_num[group])].start;
	gAscanData[WinIndex].wave.end = AScanDrawRange[group][TMP(beam_num[group])].end;
	gAscanData[WinIndex].wave.pData = pData;
	gAscanData[WinIndex].wave.dataPoints = pp->nPointQty[group];

	gAscanData[WinIndex].envelope.mode = 0;
	gAscanData[WinIndex].ref.mode = 0;

	gAscanData[WinIndex].axis.direction = DIRECTION_HORIZONTAL;
	gAscanData[WinIndex].axis.horizontal = width;
	gAscanData[WinIndex].axis.vertical = height;
	gAscanData[WinIndex].axis.x = xoff;
	gAscanData[WinIndex].axis.y = yoff;

	gAscanData[WinIndex].group = group;

	ascanDrawWave(pFrameBuffer ,&gAscanData[WinIndex]);
}

void DrawWedgeDelayCalibration(unsigned char WinIndex, unsigned short *pFrameBuffer , unsigned int pBeamData)
{
	int k , offset ;
	unsigned char group     ;
	int xoff , yoff , width , height       ;

	group  = TMP(_scan_group[WinIndex])    ;
	xoff   = TMP(_scan_xpos[WinIndex])     ;
	yoff   = TMP(_scan_ypos[WinIndex])     ;
	width  = TMP(_scan_width[WinIndex])    ;
	height = TMP(_scan_height[WinIndex])   ;

	for (offset = 0, k = 0 ; k < group ; k++)
	{
		offset +=  TMP(beam_qty[k]);
	}
	// change the percent Gate height to 12bit value ;
	k =  GROUP_VAL_POS(group, gate[0].height) * 0xfff / 200 ;

	DrawCalibrationWedgeDelayLine (pFrameBuffer ,width , height, xoff,  yoff, group,
			(unsigned int*)(TMP(measure_data)) , offset , k) ;
}

void DrawSensationCalibration(unsigned char WinIndex, unsigned short *pFrameBuffer , unsigned int pBeamData)
{
	int k , offset ;
	unsigned char group     ;
	int xoff , yoff , width , height       ;

	group  = TMP(_scan_group[WinIndex])    ;
	xoff   = TMP(_scan_xpos[WinIndex])     ;
	yoff   = TMP(_scan_ypos[WinIndex])     ;
	width  = TMP(_scan_width[WinIndex])    ;
	height = TMP(_scan_height[WinIndex])   ;

	for (offset = 0, k = 0 ; k < group ; k++)
	{
		offset +=  TMP(beam_qty[k]);
	}

	DrawSensitivityCalibrationLine(pFrameBuffer, width, height,
			xoff, yoff, group  , (unsigned int*)(TMP(measure_data)), offset) ;
}

void DrawTCGCalibration(unsigned char WinIndex, unsigned short *pFrameBuffer , unsigned int pBeamData)
{
	int k , offset ;
	unsigned char group     ;
	int xoff , yoff , width , height       ;

	group  = TMP(_scan_group[WinIndex])    ;
	xoff   = TMP(_scan_xpos[WinIndex])     ;
	yoff   = TMP(_scan_ypos[WinIndex])     ;
	width  = TMP(_scan_width[WinIndex])    ;
	height = TMP(_scan_height[WinIndex])   ;

	for (offset = 0, k = 0 ; k < group ; k++)
	{
		offset +=  TMP(beam_qty[k]);
	}
	DrawTCGCalibrationLine(pFrameBuffer, width, height,
			xoff, yoff, group  , (unsigned int*)(TMP(measure_data)), offset) ;
}

void draw_scan(unsigned char WinIndex, unsigned short *pFrameBuffer , unsigned int pBeamData)
{
    switch (TMP(_scan_type[WinIndex]))
	{
		case A_SCAN:
			//draw_a_scan(WinIndex, pFrameBuffer , pBeamData) ;
			DrawAscanHorizontal(WinIndex, pFrameBuffer , pBeamData) ;
			break;
		case A_SCAN_R:
			DrawAscanVertical(WinIndex, pFrameBuffer ,  pBeamData) ;
			break;
		case B_SCAN:
			DrawBscan(WinIndex, pFrameBuffer ,  pBeamData) ;
			break;
		case S_SCAN:
			DrawSscanSoundPath(WinIndex, pFrameBuffer ,  pBeamData) ;
			break;
		case S_SCAN_A:
			DrawSscanAzimuthal(WinIndex, pFrameBuffer , pBeamData) ;
			break;
		case S_SCAN_L: // Linear -- true depth
			DrawSscanlinear(WinIndex, pFrameBuffer , pBeamData) ;
			break;
		case C_SCAN: // first displayed c scan
			DrawCscan(WinIndex , pFrameBuffer , pBeamData , get_cscan_source(pp->p_config, 0)) ;
			break;
		case C_SCAN_VE: // first displayed c scan
			DrawCscanVer(WinIndex , pFrameBuffer , pBeamData , get_cscan_source(pp->p_config, 0)) ;
			break;
		case CC_SCAN: // second displayed c scan
			DrawCscan(WinIndex , pFrameBuffer , pBeamData , get_cscan_source(pp->p_config, 1)) ;
			break;
		case CCC_SCAN: // c scan displayed with A and S scan
			DrawCscan(WinIndex , pFrameBuffer , pBeamData , get_cscan_source(pp->p_config, 0)) ;
			break;
		case A_SCAN_CLB:
			DrawAscanCalibration(WinIndex, pFrameBuffer , pBeamData) ;
			break;
		case WEDGE_DELAY:
			DrawWedgeDelayCalibration(WinIndex, pFrameBuffer, pBeamData);
			break;
		case SENSITIVITY:
			DrawSensationCalibration(WinIndex, pFrameBuffer, pBeamData);
			break;
		case TCG:
			DrawTCGCalibration(WinIndex, pFrameBuffer, pBeamData);
			break;
		case TOFD_B_SCAN:
			DrawBscanTOFD(WinIndex, pFrameBuffer, pBeamData);
			break;
		case B_SCAN_VERT:
			DrawBscanTOFDVer(WinIndex, pFrameBuffer, pBeamData);
			break;
		case FFT_A_SCAN:
			DrawFftAscan(WinIndex ,pFrameBuffer ,pBeamData);
			break;
		default:break;
	}
	return ;
}


void RefreshDisplayMatrix(unsigned char WinIndex)
{
	unsigned char scan_type ;
	scan_type = TMP(scan_type[WinIndex])  ;
	unsigned char group     ;
	int xoff , yoff , width , height       ;
	double RANGE    ;
	double VELOCITY ;
	double START ;
	double start ;
	double range ;
	int _nPointQty   ;
	int k ;
	int bSkewReverse ;
	group  = TMP(scan_group[WinIndex])     ;
	xoff   = TMP(scan_xpos[WinIndex])       ;
	yoff   = TMP(scan_ypos[WinIndex])       ;
	width  = TMP(scan_width[WinIndex])     ;
	height = TMP(scan_height[WinIndex])   ;

	switch (scan_type)
	{
		case A_SCAN:
			break;
		case A_SCAN_R:
			if(GROUP_VAL_POS(group ,  group_mode) != PA_SCAN)
			{
				AScanDrawRange[group][0].start = 0;
				AScanDrawRange[group][0].end   = height;
				return ;
			}
			if (LAW_VAL_POS(group , Focal_type) == LINEAR_SCAN)
			{
				for(k = 0; k< TMP(beam_qty[group]); k++ )
				{
					AScanDrawRange[group][k].start = 0;
					AScanDrawRange[group][k].end   = height;
				}
			}
			else if (LAW_VAL_POS(group , Focal_type) == AZIMUTHAL_SCAN)
			{
				//_nPointQty = GROUP_VAL_POS(group , point_qty) ;
                RANGE = group_get_range(group) / 1000.0      ;
				VELOCITY = GROUP_VAL_POS(group , velocity) / 100.0 ;
                START  = group_get_start(group) / 1000.0     ;
				start = START * VELOCITY / 2000.0 ;
				range = RANGE * VELOCITY / 2000.0 ; // + start;

				FAN_SCAN_INFO DrawInfo  ;
				DrawInfo.nStartAngle  =  LAW_VAL_POS(group , Angle_min) * G_PI / 18000.0  ;
				DrawInfo.nStepAngle   =  LAW_VAL_POS(group, Angle_step) * G_PI / 18000.0   ;
				DrawInfo.nStopAngle   =  LAW_VAL_POS(group , Angle_max) * G_PI / 18000.0    ;
				DrawInfo.nSampleRange =  range  ;
				DrawInfo.nSampleStart =  start  ;
				DrawInfo.width        =  width ;
				DrawInfo.height       =  height ;
				DrawInfo.group        =  group  ;
				//DrawInfo.direction    =  bSkewReverse ;
				GetAscanRLinePosition(&DrawInfo)  ;
			}
			break;
		case B_SCAN:
			break;
		case S_SCAN:
			break;
		case S_SCAN_A:
			_nPointQty = GROUP_VAL_POS(group , point_qty) ;
            RANGE = group_get_range(group) / 1000.0      ;
			VELOCITY = GROUP_VAL_POS(group , velocity) / 100.0 ;
            START  = group_get_start(group) / 1000.0     ;
		    start = START * VELOCITY / 2000.0 ;
			range = RANGE * VELOCITY / 2000.0 ; // + start;
			if(GROUP_VAL_POS(group , skew) >= 18000)
			{
				bSkewReverse = 1 ;
			}
			else
			{
				bSkewReverse = 0 ;
			}

		   FAN_SCAN_INFO DrawInfo  ;
		   DrawInfo.nStartAngle  =  LAW_VAL_POS(group , Angle_min) * G_PI / 18000.0  ;
		   DrawInfo.nStepAngle   =  LAW_VAL_POS(group, Angle_step) * G_PI / 18000.0   ;
		   DrawInfo.nStopAngle   =  LAW_VAL_POS(group , Angle_max) * G_PI / 18000.0    ;
		   DrawInfo.nSampleRange =  range  ;
		   DrawInfo.nSampleStart =  start  ;
		   DrawInfo.nPointQty    =  _nPointQty  ;
		   DrawInfo.pExitPoint   =  (float*)(GROUP_VAL_POS(group , field_distance)) ;
		   DrawInfo.width        =  width ;
		   DrawInfo.height       =  height ;
		   DrawInfo.group        =  group  ;
		   DrawInfo.direction    = bSkewReverse ;
		   CalcFanscanReal(&DrawInfo)  ;

			break;
		case S_SCAN_L: // Linear -- true depth
			if(GROUP_VAL_POS(group , skew) >= 18000)
			{
				bSkewReverse = 1 ;
			}
			else
			{
				bSkewReverse = 0 ;
			}
            RANGE = group_get_range(group) / 1000.0      ;
			VELOCITY = GROUP_VAL_POS(group , velocity) / 100.0 ;
			range = RANGE * VELOCITY / 2000.0 ;

			LINEAR_SCAN_INFO _drawInfo ;
			_drawInfo.fAngle = LAW_VAL_POS(group , Angle_min) * G_PI / 18000.0  ;
			_drawInfo.fRange = range  ;
			_drawInfo.nLawQty = TMP(beam_qty[group]);
			_drawInfo.direction =  bSkewReverse;
			_drawInfo.nPointQty = GROUP_VAL_POS(group , point_qty) ;
			_drawInfo.nWidth    =  width;
			_drawInfo.nHeight   =  height;
			_drawInfo.nGroupId  =  group;
			_drawInfo.pExitPoint=  (float*)(GROUP_VAL_POS(group , field_distance)) ;
			CalcLinearScanNew(&_drawInfo) ;


			//_nPointQty = GROUP_VAL_POS(group , point_qty) ;
			//RANGE = GROUP_VAL_POS(group , range) / 1000.0 ;
			//VELOCITY = GROUP_VAL_POS(group , velocity) / 100.0 ;
		    //range = RANGE * VELOCITY / 2000.0 ;
			//if(GROUP_VAL_POS(group , skew) >= 18000)
			//{
			//	bSkewReverse = 1 ;
			//}
			//else
			//{
			//	bSkewReverse = 0 ;
			//}
			//CalcLinearScan(LAW_VAL_POS(group , First_tx_elem), LAW_VAL_POS (group , Last_tx_elem), LAW_VAL_POS(group ,Elem_step),
			//		        LAW_VAL_POS (group ,Elem_qty), LAW_VAL_POS(group , Angle_min)/100.0, _nPointQty,
			//	    		range , width , height ,  group , bSkewReverse);
			break;
		case C_SCAN: // first displayed c scan
			break;
		case CC_SCAN: // second displayed c scan
			break;
		case CCC_SCAN: // c scan displayed with A and S scan
			break;
		case A_SCAN_CLB:
			if (LAW_VAL_POS(group , Focal_type) == LINEAR_SCAN)
			{
				for(k = 0; k< TMP(beam_qty[group]); k++ )
				{
					AScanDrawRange[group][k].start = 0;
					AScanDrawRange[group][k].end   = width;
				}
			}
			else if (LAW_VAL_POS(group , Focal_type) == AZIMUTHAL_SCAN)
			{
                RANGE = group_get_range(group) / 1000.0      ;
				VELOCITY = GROUP_VAL_POS(group , velocity) / 100.0 ;
                START  = group_get_start(group) / 1000.0  ;
				start = START * VELOCITY / 2000.0 ;
				range = start + RANGE * VELOCITY / 2000.0 ;
				calc_line_position(LAW_VAL_POS(group , Angle_min)/100.0, LAW_VAL_POS(group , Angle_max)/100.0, LAW_VAL_POS(group , Angle_step)/100.0,
					start, range, TMP(Junction[group]), GROUP_VAL_POS(group, point_qty), width , width, group);
			}
			break;
		//case WEDGE_DELAY:
		//	break;
		//case SENSITIVITY:
		//	break;
		//case TCG:
		//	break;
		//case TOFD_B_SCAN:
		//	break;
		default:break;
	}
	return ;
}

int getCurrentBscanStart()
{
	return g_BscanDrawInfo.nShowStart  ;
}
int getCurrentBscanStop()
{
	return g_BscanDrawInfo.nShowStop   ;
}
int getCurrentTofdBscanStart(int nIndex_)
{
	TOFDB_DRAW_INFO_P pTofdBscanDrawInfo = &(g_TofdBscanDrawInfo[nIndex_]);
	return pTofdBscanDrawInfo->nShowStart  ;
}
int getCurrentTofdBscanStop(int nIndex_)
{
	TOFDB_DRAW_INFO_P pTofdBscanDrawInfo = &(g_TofdBscanDrawInfo[nIndex_]);
	return pTofdBscanDrawInfo->nShowStop   ;
}
int getCurrentCscanStart(int nIndex_)
{
	return g_CscanDrawInfo[nIndex_].nShowStart  ;
}
int getCurrentCscanStop(int nIndex_)
{
	return g_CscanDrawInfo[nIndex_].nShowStop  ;
}

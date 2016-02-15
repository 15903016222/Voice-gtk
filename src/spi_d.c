
/*345678901234567890123456789012345678901234567890123456789012345678901234567890
 *      10        20        30        40        50        60        70        80
 */

#include <stdio.h>
#include <stdlib.h>
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <math.h>

#include "callback.h"
#include "drawui.h"
#define ARRAY_DEVICE	"/dev/spidev1.0"
#define GPIO_DEVICE		"/dev/tt"

#define GPIO61_LOW	0x6000
#define GPIO61_HIGH	0x6001
#define GPIO43_LOW	0x6002
#define GPIO43_HIGH	0x6003


#include "spi_d.h"
#define TT_DEBUG 0
#define DEBUG 0

int fd_array;
static int fd_gpio;

/* n为siezof(p)/4 */
void little_to_big(unsigned int *p, int n)
{
	unsigned int i = 0;
	unsigned int *pt = p;
	for (i = 0; i < n; ++i, ++pt)
	{
		*pt = ((*pt) <<24) | ((*pt<<8) & 0x00ff0000)|((*pt>>8) & 0x0000ff00) | ((*pt) >>24);
	}
	return ;
}
#if 0
/* 16-64 小仪器专用 */
DWORD  channel_select(unsigned int nStart_)
{ 
	//unsigned int nStart_ = n - 1 ;
	if(nStart_ > 48) nStart_ = 48 ;

	int _nValue , _nShift;
	int i ;
	DWORD _select;
	_select.RX_TX_SEL_H = _select.RX_TX_SEL_L  = 0 ;
	for(i = nStart_ ; i < nStart_ + 16 ; i++)
	{
		_nValue = i / 16 ;

		switch ( _nValue)
		{
		case 1 :
			_nValue = 2 ;
			break;
		case 2 :
			_nValue = 1 ;
			break;
		default:
			break;
		}
		_nShift = 30 - 2 * i  ;
		_nShift = (_nShift + 128) % 32 ;
		_select.RX_TX_SEL_H = _select.RX_TX_SEL_H | (_nValue << _nShift ) ;
	}
	return _select ;
}
#endif

DWORD  channel_select(unsigned int n)
{
	int i;
	int temp[33];
	int L=0,H=0;
	DWORD tx_select;
	if(n <1)
		n = 1;

	if(n > 97)
		n = 97;

	for(i=n; i<n + 32; ++i)
	{
		if(i%32  == 0)
		{
			L = 32;
			H = i/32 -1;
		}
		else
		{
			L = i%32;
			H = i/32;
		}

		if(H == 1) 
			temp[L] = 2;
		else if(H == 2)
			temp[L] = 1;
		else
			temp[L] = H;  
	}    


	tx_select.RX_TX_SEL_H =  (temp[1]<<30) | (temp[2]<<28)  | (temp[3]<<26)  | (temp[4]<<24)  |
		(temp[5]<<22) | (temp[6]<<20)  | (temp[7]<<18)  | (temp[8]<<16)  |
		(temp[9]<<14) | (temp[10]<<12) | (temp[11]<<10) | (temp[12]<<8 ) |
		(temp[13]<<6) | (temp[14]<<4)  | (temp[15]<<2)  | temp[16];

	tx_select.RX_TX_SEL_L =  (temp[17]<<30) | (temp[18]<<28) | (temp[19]<<26) | (temp[24]<<24) |
		(temp[21]<<22) | (temp[22]<<20) | (temp[23]<<18) | (temp[24]<<16) |
		(temp[25]<<14) | (temp[26]<<12) | (temp[27]<<10) | (temp[28]<<8 ) |
		(temp[29]<<6)  | (temp[30]<<4)  | (temp[31]<<2)  | temp[32];

	return tx_select;
}

void init_spi ()
{
	int val = 0x01;

	if ((fd_gpio = open (GPIO_DEVICE, O_RDWR)) == -1)
	{
		perror(GPIO_DEVICE);
		return ;
	}

	if ((fd_array = open(ARRAY_DEVICE, O_RDWR)) == -1) 
	{
		perror(ARRAY_DEVICE);
		return ;
	}
	val = (val & 0xfc) | 0x00;
	if (ioctl(fd_array, SPI_IOC_RD_MODE, &val) == -1) 
	{
		perror("ioctl spi read error2\n");
		return ;
	}
	val = (val & 0xfc) | 0x00;
	if (ioctl(fd_array, SPI_IOC_WR_MODE, &val) == -1) 
	{
		perror("ioctl spi write error3\n");
		return ;
	}
	if (ioctl(fd_array, SPI_IOC_RD_MODE, &val) == -1) 
	{
		perror("ioctl spi read error4\n");
		return ;
	}
	ioctl (fd_gpio, GPIO61_LOW, &val);
	ioctl (fd_gpio, GPIO61_HIGH, &val);
	ioctl (fd_gpio, GPIO43_LOW, &val);
	ioctl (fd_gpio, GPIO43_HIGH, &val);
	return ;
}

int write_group_data (group_data_spi *p, unsigned int group)
{
	group_data_spi new, *p1;
	memcpy (&new, p, sizeof (group_data_spi));
	p1 = &new;
	p1->offset = 16 * group;
	p1->addr = 0x2;
	little_to_big ((unsigned int *)(p1), sizeof(group_data_spi) / 4);
#if ARM	
	int i;
	i = write (fd_array, (unsigned char *)(p1), sizeof(group_data_spi));
#endif
	return 0;
}

int write_focal_data (focal_data_spi *p, unsigned int beam_num , int reset)
{
	focal_data_spi new, *p1;
	memcpy (&new, p, sizeof (focal_data_spi));
	p1 = &new;
	//p1->offset = 64 * beam_num;
	p1->offset = 80 * beam_num;    // new version protocal
	p1->addr = 0x1;
	little_to_big ((unsigned int *)(p1), sizeof(focal_data_spi) / 4);
#if ARM
	int i;
	if (reset) ioctl (fd_gpio, GPIO43_LOW, &i);
	i = write (fd_array, (unsigned char *)(p1), sizeof(focal_data_spi));
	if (reset) ioctl (fd_gpio, GPIO43_HIGH, &i);
#endif
	return 0;
}

void WriteFocalSpiOfOneLaw(int grp , int nBeamNo_)
{
	int k ;
	int _nOffset = 0  ;
	for ( k = 0 ; k < grp ;  k++)
		_nOffset += TMP(beam_qty[k])   ;
	int _nIndexGlobal = _nOffset + nBeamNo_ ;
	write_focal_data (&TMP(focal_spi[_nIndexGlobal]), _nIndexGlobal , SPI_RESET_NO);
}

int enable_tcg(guint grp , int status)
{
	int point_count = (int)GROUP_VAL_POS(grp , SizingCurves.dac_point_qty) ;

	if(status)
	   TMP(group_spi[grp]).tcg_en = 1 ;
	else
	   TMP(group_spi[grp]).tcg_en = 0 ;
	TMP(group_spi[grp]).tcg_point_qty = point_count ;

	group_data_spi new, *p1;
	memcpy (&new, &TMP(group_spi[grp]), sizeof (group_data_spi));
	p1 = &new;
	p1->offset = 16 * grp ;
	p1->addr = 0x2;

	little_to_big ((unsigned int *)(p1),sizeof(group_data_spi) / 4);
#if ARM
	write (fd_array, (unsigned char *)(p1), sizeof(group_data_spi));
#endif
	return 0 ;
}
//static int g_nTcgCount = 0 ;
int write_tcg_data (unsigned int beam_num)
{
	//g_debug("write_tcg_data*****************%d" , ++g_nTcgCount);

	focal_data_spi* p = &TMP(focal_spi[beam_num]) ;
	focal_data_spi new, *p1;

	memcpy (&new, p, sizeof (focal_data_spi));
	p1 = &new;

	p1->offset = 80 * beam_num;
	p1->addr   = 0x1;
	little_to_big ((unsigned int *)(p1), sizeof(focal_data_spi) / 4);
#if ARM
	int i;
	i = write (fd_array, (unsigned char *)(p1), sizeof(focal_data_spi));
#endif
	return 0;
}

//******************************************************************************************
// init TCG parameters for FOCALLAW STRUCTURE
// input
//           grp :  group id
//		nBeamNo_ :  focallaw id in group
//******************************************************************************************
void InitTcgSpi(int grp , int nBeamNo_)
{
	double point_x[18] ;
    double point_y[18] ;
    double scale  , _nTmpScale;
	int i , _nLawId;
	int _nDistance  , _nValue ;
	int point_count = (int)GROUP_VAL_POS(grp , SizingCurves.dac_point_qty) ;
	int _nWedgeDelay = GROUP_VAL_POS(grp , wedge_delay) / 10 ;
    // get current group law offset in groups
    int _nBeamOffset = 0 ;
    for(i = 0 ; i < grp ; i++)
    {
    	_nBeamOffset += TMP(beam_qty[i]);
    }
    _nLawId = _nBeamOffset + nBeamNo_   ;

    int _nAmp[16] ;
    int _nPos[16] ;
    memcpy((void*)_nAmp , (void*)(GROUP_VAL_POS(grp , SizingCurves.amplitude[nBeamNo_])) , 16 * sizeof(int));
    memcpy((void*)_nPos , (void*)(GROUP_VAL_POS(grp , SizingCurves.position[nBeamNo_]))  , 16 * sizeof(int));

    point_x[0] = 0 ;
	point_y[0] = GROUP_VAL_POS(grp , SizingCurves.dac_ref_ampl[nBeamNo_]) ;

	TMP( focal_spi[_nLawId].point_info[0].position ) =  _nPos[0] / 10 + _nWedgeDelay;
	TMP( focal_spi[_nLawId].point_info[0].pregain )  =  0 ;
	TMP( focal_spi[_nLawId].point_info[0].slope   )  =  0 ;
	//TMP( focal_spi[_nLawId].point_info[1].pregain )  =  0 ;
	//处理位置相近的点,使两点增益以的那一点为准
	for( i = 1; i < point_count; i++ )
	{
		_nDistance = _nPos[i] - _nPos[i - 1];
		if(_nDistance <= 10)
		{
			if(_nAmp[i - 1] > _nAmp[i] )
				_nAmp[i] = _nAmp[i-1]  ;
			else
				_nAmp[i - 1] = _nAmp[i] ;
			_nPos[i]     = _nPos[i - 1] + 10 ;
		}
	}
	//************************************
	// calc every point tcg coefficients
	// calculate tcg point parameters of each point
	for( i = 1; i < point_count; i++ )
	{
		TMP( focal_spi[_nLawId].point_info[i].position ) =  _nPos[i] / 10 + _nWedgeDelay;
		scale = _nAmp[i - 1]/ ((double) _nAmp[i]) ;
		scale = log10 ( scale ) * 20 ;
		//printf("[*****************************scale %d %f]\n" , i , scale);
		_nDistance = _nPos[i] - _nPos[i - 1];
		_nTmpScale = _nAmp[0]/ ((double) _nAmp[i-1]) ;
		_nTmpScale = log10 ( _nTmpScale ) * 20 ;
		//printf("[_nTmpScale %d %f _nDistance %d]\n" , i , _nTmpScale , _nDistance);
		TMP( focal_spi[_nLawId].point_info[i].pregain )  =  (int)(_nTmpScale * 10);
		scale = 100 * scale / _nDistance ;
		_nValue = (int)(fabs(scale)  * 1024 * 1024) ;
		TMP( focal_spi[_nLawId].point_info[i].slope )  = _nValue ;
		TMP( focal_spi[_nLawId].point_info[i].flag ) = scale < 0 ? 1:0 ;
		//printf("TMP( focal_spi[_nLawId].point_info[i].slope )  %d \n" , TMP( focal_spi[_nLawId].point_info[i].slope ) );
	    //printf("TMP( focal_spi[_nLawId].point_info[i].flag )  %d \n" , TMP( focal_spi[_nLawId].point_info[i].flag ) );
	}
}


//******************************************************************************************
// send TCG parameters to FPGA
// input
//           grp :  group id
//		nBeamNo_ :  focallaw id in group, if small than 0 , then send parameters of all laws
//******************************************************************************************
void SendTcgSpi(int grp , int nBeamNo_)
{
    int i , j , _nBeamOffset = 0 ;
    for(i = 0 ; i < grp ; i++)
    {
    	_nBeamOffset += TMP(beam_qty[i]);
    }
    int _nStartLaw = nBeamNo_ ;
    int _nStopLaw  = nBeamNo_ + 1;
    if(nBeamNo_ < 0 || nBeamNo_ >= TMP(beam_qty[grp]))
    {
    	_nStartLaw = 0 ;
    	_nStopLaw  = TMP(beam_qty[i]) ;
    }
    for(i = _nStartLaw , j = _nBeamOffset + _nStartLaw; i< _nStopLaw ; j++ , i++)
    {
    	InitTcgSpi( grp , i) ;
    	//write_tcg_data (j) ;
    }
    for(i = _nStartLaw , j = _nBeamOffset + _nStartLaw; i< _nStopLaw ; j++ , i++)
    {
    	//InitTcgSpi( grp , i) ;
    	write_tcg_data (j) ;
    }
}



void write_fir_data ()
{
}


#include "lzk/output_spi.h"
void set_encoder_register(group_data_spi *p , int group_num , unsigned int enc_x, unsigned int enc_y)
{
	output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_ENCODE_Y ,enc_y ,0);
	output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_ENCODE_X ,enc_x ,0);
	output_write_one_reg_to_spi(0,OUTPUT_OTHER_COMMAND_ENCODE_Y);
}


void MultiGroupSendAllFocalSpi()
{
    int i ;
    int _nBeamQty = get_beam_qty() ;

	output_set_parameter(0 ,OUTPUT_OTHER_COMMAND_LAW_QTY ,GPOINTER_TO_UINT (_nBeamQty - 1) ,0);
	output_write_one_reg_to_spi(0 ,OUTPUT_OTHER_COMMAND_LAW_QTY);

    for(i = 0 ; i < _nBeamQty ; i++ )
    {
        write_focal_data (&TMP(focal_spi[i]), i , SPI_RESET_YES);
    }

    if (TMP(loadData)) {
        FreezingFPGA(TRUE);
        memcpy((void*)TMP(dma_data_add) ,(void*)TMP(StoreBuffAddr) , getDataBlockSize());
    }
}

void MultiGroupSendAllGroupSpi()
{
    int i ;
    int _nGroupQty = get_group_qty(pp->p_config);

    for(i = 0 ; i< _nGroupQty ; i++ )
    {
    	write_group_data (&TMP(group_spi[i]), i ) ;
    }
}


void FreezingFPGA(int bFreeze_)
{
#if ARM
	int i;
	if(bFreeze_)
	{
	    ioctl (fd_gpio, GPIO43_LOW, &i);
	}
	else
	{
		ioctl (fd_gpio, GPIO43_HIGH, &i);
	}
#endif
}

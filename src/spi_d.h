/**
 * @file spi_d.h
 * @brief spi接口
 * @author JakeYang <add358@gmail.com>
 * @date 2015-10-16
 */

#ifndef __SPI_D_H__
#define __SPI_D_H__

#include "group_spi.h"

#define FOCAL_DATA_ADD	0x10000000
#define GROUP_DATA_ADD	0x20000000
#define TGC_DATA_ADD	0x40000000
#define FIR_DATA_ADD	0x80000000

typedef struct _DWORD_OBJ
{
	unsigned int RX_TX_SEL_H;
	unsigned int RX_TX_SEL_L;	
} DWORD;

typedef struct _TCG_POINT_INFO
{
	/* TCG_SPI (0) */
    unsigned int    position:20   ;
    unsigned int    tt1:1         ;
    unsigned int    pregain:10    ;
    unsigned int    tt2:1         ;
	/* TCG_SPI (1) */
    //unsigned int    pregain:10    ;
    unsigned int    slope:22      ;
    unsigned int    tt:9          ;
    unsigned int    flag:1        ;
} TCG_POINT_INFO , *TCG_POINT_INFO_P;

typedef struct _DELAY_INFO
{
	/* TCG_SPI (0) */
    unsigned int    tx_time:14   ;
    unsigned int    reserved1:2  ;
    unsigned int    rx_time:12   ;
    unsigned int    reserved2:4  ;
} DELAY_INFO , *DELAY_INFO_P;

typedef struct _Focal_data
{
	/* s_group_reg (-1) 地址 */
	unsigned int	offset:16;		/* bit:0-15 地址的偏移 */
	unsigned int	TT:12;			/* bit:16-27 保留*/
	unsigned int	addr:4;			/* bit:28-31 片选 Group取值0010 */

	/* s_group_reg (0) 地址 */
	unsigned int	TT1:2;			/* bit:0-1 保留 */
	unsigned int	gain_offset:10;	/* bit:2-11 增益补偿单位0.1dB */
	unsigned int	TT2:3;			/* bit:12-14 保留 */
	unsigned int	group:5;		/* bit:15-18 groupId */
	unsigned int	all_beam_info:12;	/* bit:19-31 beam_qty */

	/* s_group_reg (1) 地址 */
	unsigned int	beam_delay:16;	/* bit:0-15 16 单位10ns */
	unsigned int	TT3:16;			/* bit 16-31 保留*/

	/* s_group_reg (2) 地址 */
	unsigned int	gate_a_start:20;/* bit:0-19  单位10ns */
	unsigned int	TT4:12;			/* bit 20-31 保留*/
	
	/* s_group_reg (3) 地址 */
	unsigned int	gate_a_end:20;  /* bit:0-19  单位10ns */
	unsigned int	TT5:12;			/* bit 20-31 保留*/

	/* s_group_reg (4) 地址 */
	unsigned int	gate_b_start:20;/* bit:0-19  单位10ns */
	unsigned int	TT6:12;			/* bit 20-31 保留*/
	
	/* s_group_reg (5) 地址 */
	unsigned int	gate_b_end:20;  /* bit:0-19  单位10ns */
	unsigned int	TT7:12;			/* bit 20-31 保留*/

	/* s_group_reg (6) 地址 */
	unsigned int	gate_i_start:20;/* bit:0-19  单位10ns */
	unsigned int	TT8:12;			/* bit 20-31 保留*/
	
	/* s_group_reg (7) 地址 */
	unsigned int	gate_i_end:20;  /* bit:0-19  单位10ns */
	unsigned int	TT9:12;			/* bit 20-31 保留*/

	/* s_group_reg (8-9) 地址 */
	unsigned int	TT10[2];			/* 保留 */

	unsigned int	tx_enable;
	unsigned int	rx_enable;

	/* s_group_reg (12-13) 地址 接收使能 */
	DWORD			rx_sel;

	/* s_group_reg (14-15) 地址 发射使能 */
	DWORD			tx_sel;
	/* s_group_reg (16-47) 地址 阵元发射信息 */
	//unsigned int	tx_info[32];		/* bit0-13 发射开始 bit:16-29 发射结束*/

	/* s_group_reg (48-63) 地址 阵元接收信息 */
	//unsigned int	rx_info[16];		/* bit0-31 控制 1-32 路 */
	/* s_group_reg (16-47) 地址 阵元发射信息 */
	DELAY_INFO	   delay[32];		/* bit0-13 发射延时 bit:16-27 接收延时*/
	TCG_POINT_INFO  point_info[16] ;
} focal_data_spi;

typedef struct TCG_DATA_STRUCT
{
	/* TCG_SPI (-1) 地址 */
	unsigned int	offset:16;		/* bit:0-15 地址的偏移 */
	unsigned int	TT:12;		/* bit:16-27 保留*/
	unsigned int	addr:4;		/* bit:28-31 片选 TCG 取值0100 */

	/* TCG_SPI (0) */
	TCG_POINT_INFO  point_info[16] ;
}tcg_data_spi ,*tcg_data_spi_p ;

extern void init_spi ();
extern DWORD channel_select(unsigned int n);
extern int write_group_data (group_data_spi *p, unsigned int group);
extern int write_focal_data (focal_data_spi *p, unsigned int beam_num , int reset);
extern int write_tcg_data (unsigned int group) ;
extern int enable_tcg(unsigned int grp , int status)  ;
extern void little_to_big(unsigned int *p, int n) ;
extern void set_encoder_register(group_data_spi *p , int group_num , unsigned int enc_x, unsigned int enc_y) ;
extern void MultiGroupSendAllFocalSpi();
extern void MultiGroupSendAllGroupSpi();
extern void FreezingFPGA(int bFreeze_) ;
extern void SendTcgSpi(int grp , int nBeamNo_);
extern void InitTcgSpi(int grp , int nBeamNo_);
extern void WriteFocalSpiOfOneLaw(int grp , int nBeamNo_);
extern int fd_array;

#endif

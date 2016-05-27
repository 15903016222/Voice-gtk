/**
 * @file group_spi.h
 * @brief group data spi operation
 * @author JakeYang <add358@gmail.com>
 * @version 3.1.4
 * @date 2016-02-17
 */

#ifndef __GROUP_SPI_H__
#define __GROUP_SPI_H__

#include <glib.h>

#include "version.h"

typedef struct _GroupDataSpi group_data_spi;
struct _GroupDataSpi {
    /* s_group_reg (-1) 地址 */
    guint32	offset:16;              /* bit:0-15 地址的偏移 */
    guint32	reserved:12;            /* bit:16-27 保留 */
    guint32	addr:4;                 /* bit:28-31 片选Group取值0010 */

    /* s_group_reg (0) */
    guint32	freq_band:4;			/* bit:0-3 频带选择 */
    guint32	video_filter:1;			/* bit:4   视频滤波 */
    guint32	rectifier:2;			/* bit:5-6 检波方式 */
#if HIGH_POWER
    guint32	compress_rato:14;		/* bit:7-20  压缩比	*/
    guint32	gain:11;				/* bit:21-31 Gain, 0.1 dB */
#else
    guint32 compress_rato:12;       /* bit:7-18 压缩比 */
    guint32 reserved0:2;            /* bit:19-20 保留位 */
    guint32 gain:11;                /* bit:21-31 Gain, 0.1 dB */
#endif

    /* s_group_reg (1) */
    guint32 thickness_factor:24;    /* bit:0-23 (2^24) / (thickness_max - thickness_min) 厚度差 */
    guint32 reserved1:5;            /* bit:24-28 保留位 */
    guint32	UT1:1;					/* bit:29  UT1使能位 */
    guint32	UT2:1;					/* bit:30 UT2使能位 */
    guint32	PA:1;					/* bit:31 PA使能位 */

    /* s_group_reg (2) */
    guint32	sum_gain:12;			/* bit:0-11 Sum gain */
    guint32	sample_range:20;		/* bit:12-31 capture end 采样范围 */

    /* s_group_reg (3) */
    guint32	point_qty:16;			/* bit:0-15 聚焦法则数量 */
    guint32	tcg_point_qty:8;		/* bit:16-23 TCG点个数 */
    guint32	tcg_en:1;				/* bit:24   TCG使能	 */
    guint32 reserved3:7;            /* bit:25-31 保留位 */

    /* s_group_reg (4) */
    guint32	rx_time:20;				/* bit:0-19 rx_time */
    guint32	reserved4:2;            /* bit:20-21 保留位*/
    guint32	gain1:10;				/* bit:22-31 gain1 */

    /* s_group_reg (5) */
    guint32	idel_time:27;			/* bit:0-26 idel time  */
    guint32	reserved5:5;            /* bit:27-31 保留位 */

    /* s_group_reg (6) */
    guint32	gate_a_height:12;		/* bit:0-11 闸门A 高度 */
    guint32	gate_a_start:20;		/* bit:12-31 闸门A 开始 */

    /* s_group_reg (7) */
    guint32	gate_a_logic:8;			/* bit:0-7 闸门A 逻辑 */
    guint32	reserved7:4;            /* bit:8-11 保留位 */
    guint32	gate_a_end:20;			/* bit:12-31 闸门A 结束 */

    /* s_group_reg (8) */
    guint32	gate_b_height:12;		/* bit:0-11 闸门B 高度 */
    guint32	gate_b_start:20;		/* bit:12-31 闸门B 开始 */

    /* s_group_reg (9) */
    guint32	gate_b_logic:8;			/* bit:0-7 闸门B 逻辑 */
    guint32	reserved9:4;            /* bit:8-11 保留位 */
    guint32	gate_b_end:20;			/* bit:12-31 闸门B 结束 */

    /* s_group_reg (10) */
    guint32	gate_i_height:12;		/* bit:0-11 闸门C 高度 */
    guint32	gate_i_start:20;		/* bit:12-31 闸门C 开始 */

    /* s_group_reg (11) */
    guint32	gate_i_logic:8;			/* bit:0-7 闸门C 逻辑 */
    guint32	reserved11:4;           /* bit:8-11 保留位 */
    guint32	gate_i_end:20;			/* bit:12-31 闸门C 结束 */

    /* s_group_reg (12) */
    guint32 thickness_min:20;       /* bit:0-19 */
    guint32 reject:12;              /* bit:20-31 闸门C 结束 */

    /* s_group_reg (13) */
    guint32 sample_start:21;        /* bit:0-20 采样起点 */
    guint32 reserved13:8;           /* bit:21-28 保留位 */
    guint32 average:3;              /* bit:29-31 求平均 */

    /* s_group_reg  (14)*/
    guint32 thickness_max:20;       /* bit:0-19 测量厚度最大值 */
    guint32 reserved14:8;           /* bit:20-27 保留位 */
    guint32 thickness_source:4;     /* bit:28-31 输出 TTL 模拟电压的条件源 */

    /* s_group_reg  (15)*/
    guint32 tx_end:14;              /* bit:0-13  UT tx end */
    guint32 reserved15:2;           /* bit:14-15 保留位 */
    guint32 tx_start:14;            /* bit:16-29 UT tx start */
    guint32 reserved15_:2;          /* bit:30-31 保留位 */

};

extern void group_spi_init();

/**
 * @brief group_spi_send    发送Group数据到FPGA
 * @param grp               组号
 */
extern void group_spi_send(gint grp);

/*get methods*/
extern guint group_spi_freq_band();
extern guint group_spi_video_filter();

/*set methods*/
extern void group_spi_set_freq_band(guint val);
extern void group_spi_set_video_filter(gboolean val);
extern void group_spi_set_rectifier(guint val);
extern void group_spi_set_gain(gint grp, gshort val);

#endif /* End of __GROUP_SPI_H__ */

/**
 * @file group_spi.c
 * @brief group data spi operation
 * @author JakeYang <add358@gmail.com>
 * @version 3.1.4
 * @date 2016-02-17
 */

#include "group_spi.h"

#include "main.h"
#include "dev/dev.h"

static group_data_spi *group_spi = NULL;

inline void group_spi_init()
{
    group_spi = TMP(group_spi);
}

inline void group_spi_send(gint grp)
{
    write_group_data(&group_spi[grp], grp);
}

inline void group_spi_set_gain(gint grp, gshort val)
{
    /* FPGA1：gain为10位； FPGA2：相控阵通道gain为10位，常规通道gain为11位 */
    if (dev_fpga_version() == 1
            || PA_SCAN == GROUP_VAL_POS(grp, group_mode)
            || UT_SCAN == GROUP_VAL_POS(grp, group_mode)) {
        group_spi[grp].gain = val << 1;
    } else {
        group_spi[grp].gain = val;
    }
}

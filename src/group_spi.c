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

void group_spi_init()
{
    group_spi = TMP(group_spi);
}

void group_spi_send(gint grp)
{
    write_group_data(&group_spi[grp], grp);
}

void group_spi_set_gain(gint grp, gshort val)
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

/**
 * @brief group_pw_correction  UT脉宽修正
 * @param grp   组号
 * @param val   设置的脉宽值
 * @return      返回修正后的脉宽值
 */
static guint32 group_ut_pw_correction(gint grp, guint32 val)
{
    gint voltage = get_voltage(pp->p_config, grp);

    if (get_damping_pos(pp->p_config)) {
        /* 50Ω 阻尼 */
        if (0 == voltage
                && 30*PW_DIV == val) {
            /* 100V */
            val += (2.5 * PW_DIV);
        } else if (1 == voltage) {
            /* 200V */
            if ( val == 30 * PW_DIV ) {
                val += (4 * PW_DIV);
            } else if ( val <= 50 *PW_DIV ) {
                val += (2.5 * PW_DIV);
            }
        } else if (2 == voltage) {
            /* 400V */
            if (val <= 45 * PW_DIV) {
                val += (4 *PW_DIV);
            } else if (val <= 50 * PW_DIV) {
                val += (2.5 * PW_DIV);
            }
        }
    } else {
        /* 200Ω 阻尼 */
        if (0 == voltage) {
            /* 100V */
        } else if (1 == voltage
                   && val <= 35 * PW_DIV) {
            /* 200V */
            val += (2.5 * PW_DIV);
        } else if (2 == voltage) {
            /* 400V */
            if (val <= 35 * PW_DIV) {
                val += (4 * PW_DIV);
            } else if (val <= 50 * PW_DIV) {
                val += (2.5 * PW_DIV);
            }
        }
    }
    val -= (15 * PW_DIV);

    return val;
}

void group_spi_set_tx_end(gint grp, guint32 val)
{
    if (dev_fpga_version() == 2
            && ( group_get_mode(grp) == UT1_SCAN
                 || group_get_mode(grp) == UT2_SCAN )) {
        val = group_ut_pw_correction(grp, val);
    }

    group_spi[grp].tx_end = (guint32)(val/2.5/PW_DIV + 0.5);
}

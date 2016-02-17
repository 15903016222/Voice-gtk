/**
 * @file group_spi.c
 * @brief group data spi operation
 * @author JakeYang <add358@gmail.com>
 * @version 3.1.4
 * @date 2016-02-17
 */

#include "group_spi.h"

#include "main.h"

inline void group_spi_set_gain(gint grp, gshort val)
{
#if (FPGA_VERSION > 1)
    if (PA_SCAN == GROUP_VAL_POS(grp, group_mode)
            || UT_SCAN == GROUP_VAL_POS(grp, group_mode)) {
        TMP(group_spi[grp]).gain = val << 1;
    } else {
        TMP(group_spi[grp]).gain = val;
    }
#else
    TMP(group_spi[grp]).gain = val;
#endif
}

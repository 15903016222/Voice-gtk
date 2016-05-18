/**
 * @file dev.c
 * @brief device
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-05-18
 */

#include "dev.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#ifdef ARM
#define MTD_DEVICE "/dev/mtdblock2"
#else
#define MTD_DEVICE "/dev/zero"
#endif

typedef struct _DevInfo DevInfo;
struct _DevInfo {
    gchar type[64];
    gint fpgaVersion;
    gchar serialNo[128];
};

static DevInfo devInfo;

void dev_init()
{
    int fd = open(MTD_DEVICE, O_RDONLY);
    if (fd < 0) {
        g_error("Read Phascan infomation failed");
    }
    read(fd, &devInfo, sizeof(DevInfo));
    devInfo.type[63] = 0;
    devInfo.serialNo[127] = 0;

    close(fd);
}

void dev_uninit()
{
}

DevType dev_type()
{
    if (!g_strcmp0(devInfo.type, "32-128-PR-TOFD")) {
        return DEV_32_128_PRO_TOFD;
    } else if(!g_strcmp0(devInfo.type, "32-128-TOFD")) {
        return DEV_32_128_TOFD;
    } else if(!g_strcmp0(devInfo.type, "32-64-TOFD")) {
        return DEV_32_64_TOFD;
    } else if (!g_strcmp0(devInfo.type, "16-64-TOFD")) {
        return DEV_16_64_TOFD;
    }

    return DEV_TYPE_INVAILD;
}

const gchar *dev_type_str()
{
    return devInfo.type;
}

const gchar *dev_serial_number()
{
    return devInfo.serialNo;
}

gint dev_fpga_version()
{
    return devInfo.fpgaVersion;
}

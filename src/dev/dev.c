/**
 * @file dev.c
 * @brief device
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-05-18
 */

#include "dev.h"

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
    read(fd, devInfo, sizeof(DevInfo));
    devInfo.type[64] = 0;
    devInfo.serialNo[128] = 0;
    close(fd);
}

void dev_uninit()
{
}

const gchar *dev_type()
{
    return devInfo->type;
}

const gchar *dev_serial_number()
{
    return devInfo->serialNo;
}

gint dev_fpga_version()
{
    return devInfo->fpgaVersion;
}

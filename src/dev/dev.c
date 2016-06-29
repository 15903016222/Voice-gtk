/**
 * @file dev.c
 * @brief device
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-05-18
 */

#include "dev.h"
#include "cert.h"
#include "../core/core.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <pthread.h>

#define INFO_FILE PHASCAN_PATH"/dev.info"
#define RUNCOUNT_FILE PHASCAN_PATH"/runcount.info"
#define CERT_FILE PHASCAN_PATH"/auth.cert"
#define PUBLIC_PEM "/home/tt/.secure/pub.pem"

typedef struct _DevInfo DevInfo;
struct _DevInfo {
    gchar *type;        /*设置类型*/
    gint fpgaVersion;   /*FPGA版本*/
    gchar *serialNo;    /*设备序列号*/
    gint runCount;      /*设备运行次数*/
    time_t fstTime;     /*第一次启动的时间戳*/
};

static DevInfo devInfo;
static Cert *cert = NULL;
static pthread_rwlock_t certRWLock = PTHREAD_RWLOCK_INITIALIZER;

#define MTD_DEVICE "/dev/mtdblock2"

#ifdef ARM
#define MOUNT_PHASCAN() system("mount /dev/mtdblock2 /home/tt/.phascan")
#define UMOUNT_PHASCAN() system("umount /home/tt/.phascan")
#else
#define MOUNT_PHASCAN() do {} while(0)
#define UMOUNT_PHASCAN() do {} while(0)
#endif

/**
 * @brief dev_load_cert   重新载入证书
 */
static void dev_load_cert();

static gchar *_dev_serial_number()
{
    int fd = open("/dev/mem", O_RDWR | O_NDELAY);
    unsigned char *base = NULL;
    gchar *id = NULL;

    if ( fd < 0) {
        perror ("/dev/mem");
        return NULL;
    }

    base= (unsigned char *)
        mmap (NULL,  4*1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x4830a000);

    id = g_strdup_printf("%08lx-%08lx-%08lx-%08lx",
                           *(volatile long *)(base+0x224),
                           *(volatile long *)(base+0x220),
                           *(volatile long *)(base+0x21c),
                           *(volatile long *)(base+0x218));
    munmap(base, 4*1024);
    close(fd);
    return id;
}

static void dev_save_runcount()
{
    MOUNT_PHASCAN();
    FILE *fp = fopen(RUNCOUNT_FILE, "r");
    if (fp) {
        fscanf(fp, "%d", &devInfo.runCount);
        fclose(fp);
    }

    devInfo.runCount += 1;
    gchar *cmd = g_strdup_printf("echo %d > %s", devInfo.runCount, RUNCOUNT_FILE);
    system(cmd);
    UMOUNT_PHASCAN();
    g_free(cmd);
}

static void _dev_read_info_xml(DevInfo *info)
{
    xmlDocPtr doc = NULL;
    xmlNodePtr curNode = NULL;
    gchar *buf = NULL;
    gsize len = 0;
    xmlKeepBlanksDefault(0);

    if ( !g_file_get_contents(INFO_FILE, &buf, &len, NULL) ) {
        return;
    }

    doc = xmlParseMemory(buf, len);
    if (NULL == doc) {
        g_warning("Cert Parse failed");
        return;
    }

    curNode = xmlDocGetRootElement(doc);
    if (NULL == curNode) {
        g_warning("Cert get root element failed");
        goto read_info_end0;
    }

    curNode = curNode->children;
    for (; curNode; curNode = curNode->next) {
        if (!xmlStrcmp(curNode->name, BAD_CAST"Type")) {
            info->type = xmlNodeGetContent(curNode);
        } else if (!xmlStrcmp(curNode->name, BAD_CAST"FPGA")) {
            xmlChar *tmpStr = xmlNodeGetContent(curNode);
            if (tmpStr) {
                info->fpgaVersion = atol(tmpStr);
            } else {
                info->fpgaVersion = 1;
            }
            xmlFree(tmpStr);
        } else if (!xmlStrcmp(curNode->name, BAD_CAST"Time")) {
            xmlChar *tmpStr = xmlNodeGetContent(curNode);
            if (tmpStr) {
                info->fstTime = atol(tmpStr);
            } else {
                info->fstTime = time(NULL);
            }
            xmlFree(tmpStr);
        }
    }
read_info_end0:
    xmlFreeDoc(doc);
}

/**
 * @brief _dev_read_info_raw  该函数用于兼容3.3.0以下版本保存数据的问题
 * @param info 指向DevInfo设备信息
 */
static void _dev_read_info_raw(DevInfo *info)
{
    gchar buf[100] = {0};
    gint mtd = open(MTD_DEVICE, O_RDWR | O_SYNC);

    if (mtd < 0) {
        info->type = NULL;
    }

    if ( read(mtd , buf , 30) < 0 ) {
       info->type = NULL;
    } else {
       info->type = g_strdup(buf);
    }
    close(mtd);

    info->fpgaVersion = 1;
    info->fstTime = time(NULL);

    gchar *msg = g_strdup_printf("<DevInfo><Type>%s</Type><FPGA>%d</FPGA><Time>%d</Time></DevInfo>",
                                 info->type,
                                 info->fpgaVersion,
                                 info->fstTime
                                 );
    system("mkfs.ext2 "MTD_DEVICE);
    MOUNT_PHASCAN();
    g_file_set_contents(INFO_FILE, msg, strlen(msg), NULL);
    g_free(msg);
}

static void dev_read_info(DevInfo *info)
{
    memset(info, 0, sizeof(DevInfo));

    if (MOUNT_PHASCAN() != 0) {
        _dev_read_info_raw(info);
    } else {
        _dev_read_info_xml(info);
    }
    devInfo.serialNo = _dev_serial_number();

    UMOUNT_PHASCAN();
}


void dev_init()
{
    dev_read_info(&devInfo);

    dev_save_runcount();

    dev_load_cert();
}

void dev_uninit()
{
    pthread_rwlock_wrlock(&certRWLock);
    cert_free(cert);
    cert = NULL;
    pthread_rwlock_unlock(&certRWLock);

    g_free(devInfo.serialNo);
    g_free(devInfo.type);
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

gint dev_run_count()
{
    return devInfo.runCount;
}

time_t dev_run_time()
{
    return (time(NULL) - devInfo.fstTime);
}

gboolean dev_is_valid()
{
    gboolean flag = FALSE;
    pthread_rwlock_rdlock(&certRWLock);
    if ( cert  && !g_strcmp0(cert_get_id(cert), devInfo.serialNo) ) {
        switch( cert_get_mode(cert) ) {
        case CERT_MODE_NONE:
            flag = TRUE;
            break;
        case CERT_MODE_RUNTIME:
            flag = cert_get_data(cert) >= (time(NULL) - devInfo.fstTime);
            break;
        case CERT_MODE_RUNCOUNT:
            flag = cert_get_data(cert) >= devInfo.runCount;
            break;
        case CERT_MODE_DATE:
            flag = cert_get_data(cert) > time(NULL);
            break;
        default:
            break;
        }
    }
    pthread_rwlock_unlock(&certRWLock);

    return flag;
}

inline void dev_load_cert()
{
    pthread_rwlock_wrlock(&certRWLock);
    cert_free(cert);
    MOUNT_PHASCAN();
    cert = cert_load(CERT_FILE, PUBLIC_PEM);
    UMOUNT_PHASCAN();
    pthread_rwlock_unlock(&certRWLock);
}

gboolean dev_import_cert(const gchar *cert)
{
    MOUNT_PHASCAN();
    gchar *cmd = g_strdup_printf("cp %s %s && sync", cert, CERT_FILE);
    if ( system(cmd) != 0 ) {
        UMOUNT_PHASCAN();
        return FALSE;
    }
    UMOUNT_PHASCAN();
    dev_load_cert();
    return TRUE;
}


gchar *dev_get_cert_mode()
{
    gchar *mode = NULL;
    pthread_rwlock_rdlock(&certRWLock);
    switch (cert_get_mode(cert)) {
    case CERT_MODE_NONE:
        mode = g_strdup("None");
        break;
    case CERT_MODE_RUNCOUNT:
        mode = g_strdup("Run Count");
        break;
    case CERT_MODE_RUNTIME:
        mode = g_strdup("Run Time");
        break;
    case CERT_MODE_DATE:
        mode = g_strdup("Date");
        break;
    default:
        mode = g_strdup("Invalid");
        break;
    }
    pthread_rwlock_unlock(&certRWLock);
    return mode;
}

gchar *dev_get_cert_data()
{
    gchar *data = NULL;
    pthread_rwlock_rdlock(&certRWLock);
    switch (cert_get_mode(cert)) {
    case CERT_MODE_RUNCOUNT:
    case CERT_MODE_RUNTIME:
        data = g_strdup_printf("%ld", cert_get_data(cert));
        break;
    case CERT_MODE_DATE: {
        time_t t = cert_get_data(cert);
        struct tm *tm = localtime(&t);
        data = g_strdup_printf("%d-%d-%d %d:%d", tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min);
    }
        break;
    default:
        break;
    }
    pthread_rwlock_unlock(&certRWLock);
    return data;
}

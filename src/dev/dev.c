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
#include <fcntl.h>

#include <stdio.h>

#include <pthread.h>

#define INFO_FILE PHASCAN_PATH"/dev.info"
#define CERT_FILE PHASCAN_PATH"/auth.cert"
#define PUBLIC_PEM "/home/tt/.secure/pub.pem"

typedef struct _DevInfo DevInfo;
struct _DevInfo {
    gchar type[64];
    gint fpgaVersion;
    gchar serialNo[128];
    gint runCount;
    time_t runTime;
};

static DevInfo devInfo;
static Cert *cert = NULL;
static pthread_rwlock_t certRWLock = PTHREAD_RWLOCK_INITIALIZER;
static pthread_rwlock_t mountRWlock = PTHREAD_RWLOCK_INITIALIZER;

#ifdef ARM
#define MOUNT_PHASCAN() { pthread_rwlock_wrlock(&mountRWlock); system("mount /dev/mtdblock2 /home/tt/.phascan"); }
#define UMOUNT_PHASCAN() { pthread_rwlock_unlock(&mountRWlock); system("umount /home/tt/.phascan"); }
#else
#define MOUNT_PHASCAN() pthread_rwlock_wrlock(&mountRWlock)
#define UMOUNT_PHASCAN() pthread_rwlock_unlock(&mountRWlock)
#endif

/**
 * @brief dev_load_cert   重新载入证书
 */
static void dev_load_cert();

static void dev_save_info()
{
    MOUNT_PHASCAN();
    FILE *fp = fopen(INFO_FILE, "w");
    if (NULL == fp) {
        UMOUNT_PHASCAN();
        g_error("Read Phascan infomation failed");
    }
    fwrite(&devInfo, sizeof(DevInfo), 1, fp);
    fclose(fp);
    UMOUNT_PHASCAN();
}

static gboolean dev_increase_runtime(gpointer data)
{
    devInfo.runTime += 60;
    dev_save_info();
    return TRUE;
}

void dev_init()
{
    MOUNT_PHASCAN();
    FILE *fp = fopen(INFO_FILE, "r");
    if (NULL == fp) {
        UMOUNT_PHASCAN();
        g_warning("Read Phascan infomation failed");
        return;
    }
    fread(&devInfo, sizeof(DevInfo), 1, fp);
    fclose(fp);
    UMOUNT_PHASCAN();

    devInfo.runCount += 1;
    dev_save_info();

    g_timeout_add(60*1000, dev_increase_runtime, NULL);

    dev_load_cert();
}

void dev_uninit()
{
    pthread_rwlock_wrlock(&certRWLock);
    cert_free(cert);
    cert = NULL;
    pthread_rwlock_unlock(&certRWLock);
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
    return devInfo.runTime;
}

gboolean dev_is_valid()
{
    gboolean flag = FALSE;
    pthread_rwlock_rdlock(&certRWLock);
    if ( cert ) {
        switch( cert_get_mode(cert) ) {
        case CERT_MODE_NONE:
            flag = TRUE;
            break;
        case CERT_MODE_RUNTIME:
            flag = cert_get_data(cert) >= devInfo.runTime;
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

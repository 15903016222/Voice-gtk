/**
 * @file dev.h
 * @brief device
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-05-18
 */


#ifndef __DEV_H__
#define __DEV_H__

#include <glib.h>

G_BEGIN_DECLS

typedef enum {
    DEV_TYPE_INVAILD = -1,
    DEV_32_128_PRO_TOFD = 0,
    DEV_32_128_TOFD,
    DEV_32_64_TOFD,
    DEV_16_64_TOFD
} DevType;

extern void dev_init();
extern void dev_uninit();
extern DevType dev_type();
extern const gchar *dev_type_str();
extern const gchar *dev_serial_number();
extern gint dev_fpga_version();
extern gint dev_run_count();

/**
 * @brief dev_run_time  获取设备累积运行时间
 * @return 设备运行时间，单位s
 */
extern time_t dev_run_time();

/**
 * @brief dev_import_cert   导入证书
 * @param cert              证书名称
 * @return 导入成功返回TRUE， 失败返回FALSE
 */
extern gboolean dev_import_cert(const gchar *cert);

/**
 * @brief dev_is_valid  设备是否有效
 * @return 有效返回TRUE，否则返回FALSE
 */
extern gboolean dev_is_valid();

extern gchar *dev_get_cert_mode();
extern gchar *dev_get_cert_data();

G_END_DECLS

#endif /* end of __DEV_H__ */

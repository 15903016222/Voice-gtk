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

G_END_DECLS

#endif /* end of __DEV_H__ */

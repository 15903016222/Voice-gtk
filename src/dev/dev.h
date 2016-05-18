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

extern void dev_init();
extern void dev_uninit();
extern const gchar *dev_type();
extern const gchar *dev_serial_number();
extern gint dev_fpga_version();

G_END_DECLS

#endif /* end of __DEV_H__ */

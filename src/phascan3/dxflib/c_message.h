/**
 * @file c_message.h
 * @brief debug tool
 * @author JakeYang <add358@gmail.com>
 * @version 1.0
 * @date 2015-04-14
 */

#ifndef __C_MESSAGE_H__
#define __C_MESSAGE_H__

#ifdef C_DEBUG
#ifndef C_MESSAGE_STREAM
#define C_MESSAGE_STREAM    stderr
#endif
#include <stdio.h>
#define c_message(_fmt, ...)    fprintf(C_MESSAGE_STREAM,_fmt,##__VA_ARGS__)
#define c_debug(_fmt, ...) \
    c_message("** Debug [%s]: " _fmt "\n",__func__, ##__VA_ARGS__)
#define c_log_start(_logfile)       do{}while(0)
#define c_log_end() do{}while(0)
#define c_log_info(_fmt, ...) \
    c_message("** Log info: " _fmt "\n", ##__VA_ARGS__)
#define c_log_warning(_fmt, ...) \
    c_message("** Log Warning: " _fmt "\n", ##__VA_ARGS__)
#define c_log_err(_fmt, ...) \
    c_message("** Log Error: " _fmt "\n", ##__VA_ARGS__)
#else
#include <syslog.h>
#define c_debug(_fmt, ...) {}
#define c_log_begin(_logflag)   openlog(_logfile, LOG_CONS|LOG_PID, 0)
#define c_log_end() closelog()
#define c_log_info(_fmt, ...)   syslog(LOG_USER|LOG_INFO, _fmt, ##__VA_ARGS__)
#define c_log_warning(_fmt, ...) \
    syslog(LOG_LOCAL0|LOG_WARNING, _fmt, ##__VA_ARGS__)
#define c_log_err(_fmt, ...)    syslog(LOG_LOCAL0|LOG_ERR, _fmt, ##__VA_ARGS__)
#endif

#endif /* end of __C_MESSAGE_H__ */

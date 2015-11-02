#include <stdio.h>
#include <syslog.h>
int main()
{
    int test = 10;

    //openlog("MySysLogTest", LOG_PID|LOG_CONS, LOG_LOCAL7);

    syslog(LOG_LOCAL0|LOG_NOTICE, "ERROR: error msg syslog(test val=)");
    //syslog(LOG_LOCAL7|LOG_DEBUG, "ERROR: error msg syslog(test val=)");
    //closelog(); 
    return 0;
}

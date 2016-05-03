/**
 * @file report_law.c
 * @brief focal law
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-04-29
 */

#include "report_law.h"

void report_law_free(ReportLaw *law)
{

    g_return_if_fail( law != NULL );
    g_free(law->elementQty);
    g_free(law->firstTxElement);
    g_free(law->lastTxElement);
    g_free(law->firstRxElement);
    g_free(law->lastRxElement);
    g_free(law->elementResolution);
    g_free(law->startAngle);
    g_free(law->stopAngle);
    g_free(law->angleResolution);
    g_free(law->type);
    g_free(law->focalType);
    g_free(law);

}

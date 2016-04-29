/**
 * @file report_setup.c
 * @brief setup
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-04-29
 */

#include "report_setup.h"

void report_setup_free(ReportSetup *s)
{
    g_return_if_fail( s != NULL );

    g_free(s->beamDelay);
    g_free(s->halfPathStart);
    g_free(s->halfPathRange);
    g_free(s->prf);
    g_free(s->inspectionType);
    g_free(s->averagingFactor);
    g_free(s->scaleFactor);
    g_free(s->videoFilter);
    g_free(s->rectification);
    g_free(s->bandPassFilter);
    g_free(s->voltage);
    g_free(s->gain);
    g_free(s->rxTxMode);
    g_free(s->waveType);
    g_free(s->soundVelocity);
    g_free(s->pulseWidth);
    g_free(s->scanOffset);
    g_free(s->indexOffset);
    g_free(s->skew);

    report_gate_free(s->gateA);
    report_gate_free(s->gateB);
    report_gate_free(s->gateI);

    g_free(s);
}

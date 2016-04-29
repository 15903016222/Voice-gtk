/**
 * @file report_fft.c
 * @brief fft
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-04-28
 */

#include "report_fft.h"

void report_fft_free(ReportFFT *fft)
{
    g_free(fft->peakFreq);
    g_free(fft->lowerFreq_6dB);
    g_free(fft->higherFreq_6dB);
    g_free(fft->centerFreq_6dB);
    g_free(fft->bandwidth_6dB);
    g_free(fft->bandwidthPercent_6dB);
    g_free(fft->lowerFreq_20dB);
    g_free(fft->higherFreq_20dB);
    g_free(fft->centerFreq_20dB);
    g_free(fft->bandwidth_20dB);
    g_free(fft->bandwidthPercent_20dB);

    g_free(fft);
}

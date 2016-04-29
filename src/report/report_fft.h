/**
 * @file report_fft.h
 * @brief fft
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-04-28
 */

#ifndef __REPORT_FFT_H__
#define __REPORT_FFT_H__

#include "report_base.h"

G_BEGIN_DECLS

typedef struct _ReportFFT ReportFFT;

struct _ReportFFT {
    gchar *peakFreq;
    gchar *lowerFreq_6dB;
    gchar *higherFreq_6dB;
    gchar *centerFreq_6dB;
    gchar *bandwidth_6dB;
    gchar *bandwidthPercent_6dB;
    gchar *lowerFreq_20dB;
    gchar *higherFreq_20dB;
    gchar *centerFreq_20dB;
    gchar *bandwidth_20dB;
    gchar *bandwidthPercent_20dB;
};

static inline ReportFFT *report_fft_new()
{
    return g_malloc0(sizeof(ReportFFT));
}

extern void report_fft_free(ReportFFT *fft);

static inline gchar *_report_strdup_printf(gdouble freq)
{
    if (freq < 0.000001) {
        return g_strdup_printf("-");
    }
    return g_strdup_printf("%0.2f", freq);
}

static inline void _report_fft_set_double(gchar **var, gdouble val)
{
    g_free(*var);
    if (val < 0.000001 && val > -0.000001) {
        *var = g_strdup_printf("-");
    } else {
        *var = g_strdup_printf("%0.2f", val);
    }
}

static inline void report_fft_set_peak_freq(ReportFFT *fft, gdouble freq)
{
    g_return_if_fail( fft != NULL );
    _report_fft_set_double(&fft->peakFreq, freq);
}

static inline void report_fft_set_lower_freq_6dB(ReportFFT *fft, gdouble freq)
{
    g_return_if_fail( fft != NULL );
    _report_fft_set_double(&fft->lowerFreq_6dB, freq);
}

static inline void report_fft_set_higher_freq_6dB(ReportFFT *fft, gdouble freq)
{
    g_return_if_fail( fft != NULL );
    _report_fft_set_double(&fft->higherFreq_6dB, freq);
}

static inline void report_fft_set_center_freq_6dB(ReportFFT *fft, gdouble freq)
{
    g_return_if_fail( fft != NULL );
    _report_fft_set_double(&fft->centerFreq_6dB, freq);
}

static inline void report_fft_set_bandwidth_6dB(ReportFFT *fft, gdouble bandwidth)
{
    g_return_if_fail( fft != NULL );
    _report_fft_set_double(&fft->bandwidth_6dB, bandwidth);
}

static inline void report_fft_set_bandwidth_percent_6dB(ReportFFT *fft, gdouble percent)
{
    g_return_if_fail( fft != NULL );
    _report_fft_set_double(&fft->bandwidthPercent_6dB, percent);
}

static inline void report_fft_set_lower_freq_20dB(ReportFFT *fft, gdouble freq)
{
    g_return_if_fail( fft != NULL );
    _report_fft_set_double(&fft->lowerFreq_20dB, freq);
}

static inline void report_fft_set_higher_freq_20dB(ReportFFT *fft, gdouble freq)
{
    g_return_if_fail( fft != NULL );
    _report_fft_set_double(&fft->higherFreq_20dB, freq);
}

static inline void report_fft_set_center_freq_20dB(ReportFFT *fft, gdouble freq)
{
    g_return_if_fail( fft != NULL );
    _report_fft_set_double(&fft->centerFreq_20dB, freq);
}

static inline void report_fft_set_bandwidth_20dB(ReportFFT *fft, gdouble bandwidth)
{
    g_return_if_fail( fft != NULL );
    _report_fft_set_double(&fft->bandwidth_20dB, bandwidth);
}

static inline void report_fft_set_bandwidth_percent_20dB(ReportFFT *fft, gdouble percent)
{
    g_return_if_fail( fft != NULL );
    _report_fft_set_double(&fft->bandwidthPercent_20dB, percent);
}

G_END_DECLS

#endif /* end of __REPORT_FFT_H__ */

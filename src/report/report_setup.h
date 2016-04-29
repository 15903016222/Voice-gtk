/**
 * @file report_setup.h
 * @brief setup information
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-04-28
 */

#ifndef __REPORT_SETUP_H__
#define __REPORT_SETUP_H__

#include "report_gate.h"

G_BEGIN_DECLS

typedef struct _ReportSetup ReportSetup;


struct _ReportSetup {
    gchar *beamDelay;
    gchar *halfPathStart;
    gchar *halfPathRange;
    gchar *prf;
    gchar *inspectionType;
    gchar *averagingFactor;
    gchar *scaleFactor;
    gchar *videoFilter;
    gchar *rectification;
    gchar *bandPassFilter;
    gchar *voltage;
    gchar *gain;
    gchar *rxTxMode;
    gchar *waveType;
    gchar *soundVelocity;
    gchar *pulseWidth;
    gchar *scanOffset;
    gchar *indexOffset;
    gchar *skew;
    ReportGate *gateA;
    ReportGate *gateB;
    ReportGate *gateI;
};

static inline ReportSetup *report_setup_new()
{
    return g_malloc0(sizeof(ReportSetup));
}

extern void report_setup_free(ReportSetup *s);

/**
 * @brief report_setup_set_beam_delay
 * @param s
 * @param val Unit is μs
 */
static inline void report_setup_set_beam_delay(ReportSetup *s, gdouble val)
{
    g_return_if_fail( s != NULL );
    _report_set_double(&s->beamDelay, "%.2f", val);
}

/**
 * @brief report_setup_set_half_path_start
 * @param s
 * @param val Unit is mm
 */
static inline void report_setup_set_half_path_start(ReportSetup *s, gdouble val)
{
    g_return_if_fail( s != NULL );
    _report_set_double(&s->halfPathStart, "%.2f", val);
}

/**
 * @brief report_setup_set_half_path_range
 * @param s
 * @param val Unit is mm
 */
static inline void report_setup_set_half_path_range(ReportSetup *s, gdouble val)
{
    g_return_if_fail( s != NULL );
    _report_set_double(&s->halfPathRange, "%.2f", val);
}

/**
 * @brief report_setup_set_prf
 * @param s
 * @param val   Unit is kHz
 */
static inline void report_setup_set_prf(ReportSetup *s, gint val)
{
    g_return_if_fail( s != NULL );
    _report_set_int(&s->prf, val);
}

static inline void report_setup_set_inspection_type(ReportSetup *s, const gchar *val)
{
    g_return_if_fail( s != NULL );
    _report_set_str(&s->inspectionType, val);
}

static inline void report_setup_set_averaging_factor(ReportSetup *s, gint val)
{
    g_return_if_fail( s != NULL );
    _report_set_int(&s->averagingFactor, val);
}

static inline void report_setup_set_scale_factor(ReportSetup *s, gint val)
{
    g_return_if_fail( s != NULL );
    _report_set_int(&s->scaleFactor, val);
}

static inline void report_setup_set_video_filter(ReportSetup *s, const gchar *val)
{
    g_return_if_fail( s != NULL );
    _report_set_str(&s->videoFilter, val);
}

static inline void report_setup_set_rectification(ReportSetup *s, const gchar *val)
{
    g_return_if_fail( s != NULL );
    _report_set_str(&s->rectification, val);
}

static inline void report_setup_set_band_pass_filter(ReportSetup *s, const gchar *val)
{
    g_return_if_fail( s != NULL );
    _report_set_str(&s->bandPassFilter, val);
}

static inline void report_setup_set_voltage(ReportSetup *s, const gchar *val)
{
    g_return_if_fail( s != NULL );
    _report_set_str(&s->voltage, val);
}

/**
 * @brief report_setup_set_gain
 * @param s
 * @param val   Unit is dB
 */
static inline void report_setup_set_gain(ReportSetup *s, gdouble val)
{
    g_return_if_fail( s != NULL );
    _report_set_double(&s->gain, "%.1f", val);
}

static inline void report_setup_set_rx_tx_mode(ReportSetup *s, const gchar *val)
{
    g_return_if_fail( s != NULL );
    _report_set_str(&s->rxTxMode, val);
}

static inline void report_setup_set_wave_type(ReportSetup *s, const gchar *val)
{
    g_return_if_fail( s != NULL );
    _report_set_str(&s->waveType, val);
}

/**
 * @brief report_setup_set_soundVelocity
 * @param s
 * @param val   Unit is m/s
 */
static inline void report_setup_set_sound_velocity(ReportSetup *s, gdouble val)
{
    g_return_if_fail( s != NULL );
    _report_set_double(&s->soundVelocity, "%.1f", val);
}

/**
 * @brief report_setup_set_pulse_width
 * @param s
 * @param val   Unit is ns
 */
static inline void report_setup_set_pulse_width(ReportSetup *s, gdouble val)
{
    g_return_if_fail( s != NULL );
    _report_set_double(&s->pulseWidth, "%.1f", val);
}

/**
 * @brief report_setup_set_scanOffset
 * @param s
 * @param val   Unit is mm
 */
static inline void report_setup_set_scan_offset(ReportSetup *s, gdouble val)
{
    g_return_if_fail( s != NULL );
    _report_set_double(&s->scanOffset, "%.1f", val);
}

static inline void report_setup_set_index_offset(ReportSetup *s, gdouble val)
{
    g_return_if_fail( s != NULL );
    _report_set_double(&s->indexOffset, "%.1f", val);
}

static inline void report_setup_set_skew(ReportSetup *s, gdouble val)
{
    g_return_if_fail( s != NULL );
    _report_set_double(&s->skew, "%.1f", val);
}

static inline void report_setup_set_gate_a(ReportSetup *s, ReportGate *a)
{
    g_return_if_fail( s != NULL );
    _report_set_member((gpointer *)&s->gateA, a, (GDestroyNotify)report_gate_free);
}

static inline void report_setup_set_gate_b(ReportSetup *s, ReportGate *b)
{
    g_return_if_fail( s != NULL );
    _report_set_member((gpointer *)&s->gateB, b, (GDestroyNotify)report_gate_free);
}

static inline void report_setup_set_gate_i(ReportSetup *s, ReportGate *i)
{
    g_return_if_fail( s != NULL );
    _report_set_member((gpointer *)&s->gateI, i, (GDestroyNotify)report_gate_free);
}
G_END_DECLS

#endif /* end of __REPORT_SETUP_H__ */

#ifndef __AO_METER_H__
#define __AO_METER_H__

#include "qpn.h"
#include "es232.h"

typedef enum _meter_mode_t
{
    meter_mode_acv,
    meter_mode_dcv,
    meter_mode_mv_ac,
    meter_mode_mv_dc,
    meter_mode_om_om,
    meter_mode_om_buz,
    meter_mode_om_dio,
    meter_mode_om_cap,
    meter_mode_hz_freq,
    meter_mode_hz_duty,
    meter_mode_hfe,
    meter_mode_ua_dc,
    meter_mode_ua_ac,
    meter_mode_ma_dc,
    meter_mode_ma_ac,
    meter_mode_a_dc,
    meter_mode_a_ac,
    meter_mode_ncv,
} meter_mode_t;

typedef enum _ao_meter_signal_t
{
    AO_METER_READY_SIG = Q_USER_SIG, //
    AO_METER_ADC_DONE_SIG,
    AO_METER_KEY_SIG,

    AO_METER_MAX_SIG
} ao_meter_signal_t;

typedef struct _ao_meter_t
{
    QActive super;
    uint8_t ready_count;
    uint8_t eeprom[256];
    meter_mode_t mode;
    es232_write_cmd_t *mode_config;
} ao_meter_t;

extern ao_meter_t ao_meter;

void ao_meter_ctor(void);

#endif

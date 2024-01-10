#ifndef __METER_MODE_HZ_FREQ_H__
#define __METER_MODE_HZ_FREQ_H__

#include "ao_meter.h"

QState meter_hz_freq_adc(ao_meter_t *const me);
QState meter_hz_freq_key(ao_meter_t *const me);

void meter_hz_freq_init(ao_meter_t *const me);

#endif

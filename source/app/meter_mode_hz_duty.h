#ifndef __METER_MODE_HZ_DUTY_H__
#define __METER_MODE_HZ_DUTY_H__

#include "ao_meter.h"

QState meter_hz_duty_adc(ao_meter_t *const me);
QState meter_hz_duty_key(ao_meter_t *const me);

void meter_hz_duty_init(ao_meter_t *const me);

#endif

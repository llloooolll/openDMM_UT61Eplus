#ifndef __METER_MODE_OHM_OHM_H__
#define __METER_MODE_OHM_OHM_H__

#include "ao_meter.h"

QState meter_ohm_ohm_adc(ao_meter_t *const me);
QState meter_ohm_ohm_key(ao_meter_t *const me);

void meter_ohm_ohm_init(ao_meter_t *const me);

#endif

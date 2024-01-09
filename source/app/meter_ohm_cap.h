#ifndef __METER_OHM_CAP_H__
#define __METER_OHM_CAP_H__

#include "ao_meter.h"

QState meter_ohm_cap_adc(ao_meter_t *const me);
QState meter_ohm_cap_key(ao_meter_t *const me);

void meter_ohm_cap_init(ao_meter_t *const me);

#endif

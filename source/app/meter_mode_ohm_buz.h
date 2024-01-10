#ifndef __METER_MODE_OHM_BUZ_H__
#define __METER_MODE_OHM_BUZ_H__

#include "ao_meter.h"

QState meter_ohm_buz_adc(ao_meter_t *const me);
QState meter_ohm_buz_key(ao_meter_t *const me);

void meter_ohm_buz_init(ao_meter_t *const me);

#endif

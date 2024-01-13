#ifndef __METER_MODE_HFE_H__
#define __METER_MODE_HFE_H__

#include "ao_meter.h"

QState meter_hfe_adc(ao_meter_t *const me);
QState meter_hfe_key(ao_meter_t *const me);

void meter_hfe_init(ao_meter_t *const me);

#endif

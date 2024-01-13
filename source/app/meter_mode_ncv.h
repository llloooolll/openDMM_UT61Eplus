#ifndef __METER_MODE_NCV_H__
#define __METER_MODE_NCV_H__

#include "ao_meter.h"

QState meter_ncv_adc(ao_meter_t *const me);
QState meter_ncv_key(ao_meter_t *const me);

void meter_ncv_init(ao_meter_t *const me);

#endif

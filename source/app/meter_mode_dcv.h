#ifndef __METER_MODE_DCV_H__
#define __METER_MODE_DCV_H__

#include "ao_meter.h"

QState meter_dcv_adc(ao_meter_t *const me);
QState meter_dcv_key(ao_meter_t *const me);

void meter_dcv_init(ao_meter_t *const me);

#endif

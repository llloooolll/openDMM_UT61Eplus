#ifndef __METER_MODE_UA_DC_H__
#define __METER_MODE_UA_DC_H__

#include "ao_meter.h"

QState meter_ua_dc_adc(ao_meter_t *const me);
QState meter_ua_dc_key(ao_meter_t *const me);

void meter_ua_dc_init(ao_meter_t *const me);

#endif

#ifndef __METER_MODE_MA_DC_H__
#define __METER_MODE_MA_DC_H__

#include "ao_meter.h"

QState meter_ma_dc_adc(ao_meter_t *const me);
QState meter_ma_dc_key(ao_meter_t *const me);

void meter_ma_dc_init(ao_meter_t *const me);

#endif

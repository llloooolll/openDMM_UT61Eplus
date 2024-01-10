#ifndef __METER_MV_DC_H__
#define __METER_MV_DC_H__

#include "ao_meter.h"

QState meter_mv_dc_adc(ao_meter_t *const me);
QState meter_mv_dc_key(ao_meter_t *const me);

void meter_mv_dc_init(ao_meter_t *const me);

#endif

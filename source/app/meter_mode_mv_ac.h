#ifndef __METER_MODE_MV_AC_H__
#define __METER_MODE_MV_AC_H__

#include "ao_meter.h"

QState meter_mv_ac_adc(ao_meter_t *const me);
QState meter_mv_ac_key(ao_meter_t *const me);

void meter_mv_ac_init(ao_meter_t *const me);

#endif

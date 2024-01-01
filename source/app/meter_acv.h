#ifndef __METER_ACV_H__
#define __METER_ACV_H__

#include "ao_meter.h"

QState meter_acv_adc(ao_meter_t *const me);
QState meter_acv_key(ao_meter_t *const me);

void meter_acv_lcd_init(ao_meter_t *const me);

#endif

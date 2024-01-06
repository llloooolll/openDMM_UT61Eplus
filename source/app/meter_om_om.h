#ifndef __METER_OM_OM_H__
#define __METER_OM_OM_H__

#include "ao_meter.h"

QState meter_om_om_adc(ao_meter_t *const me);
QState meter_om_om_key(ao_meter_t *const me);

void meter_om_om_lcd_init(ao_meter_t *const me);

#endif

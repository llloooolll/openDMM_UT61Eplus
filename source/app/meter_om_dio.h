#ifndef __METER_OM_DIO_H__
#define __METER_OM_DIO_H__

#include "ao_meter.h"

QState meter_om_dio_adc(ao_meter_t *const me);
QState meter_om_dio_key(ao_meter_t *const me);

void meter_om_dio_init(ao_meter_t *const me);

#endif

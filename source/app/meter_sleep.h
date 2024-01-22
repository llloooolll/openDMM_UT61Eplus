#ifndef __METER_SLEEP_H__
#define __METER_SLEEP_H__

#include "ao_meter.h"
#include "hc32l13x.h"

void meter_sleep_init(ao_meter_t *const me);
void meter_sleep_alarm_set_delay(uint8_t time_minute);

#endif

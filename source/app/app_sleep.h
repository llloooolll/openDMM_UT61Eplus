#ifndef __APP_SLEEP_H__
#define __APP_SLEEP_H__

#include "ao_meter.h"
#include "hc32l13x.h"

void app_sleep_init(void);
void app_sleep_set_time(uint8_t time_minutes);
void app_sleep_enty(void);
void app_sleep_exit(void);

#endif

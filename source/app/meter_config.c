#include "meter_config.h"

#include "ao_meter.h"
#include "gpio.h"
#include "io_config.h"
#include "ulog.h"

meter_config_t config;

void meter_config_reset(void) {
    PAR_VALUE(config, sleep_time) = 0;
    PAR_VALUE(config, sleep_cycle) = 0;
}

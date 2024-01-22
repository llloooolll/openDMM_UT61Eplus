#include "app_config.h"

#include "ao_meter.h"
#include "es232.h"
#include "gpio.h"
#include "io_config.h"
#include "ulog.h"

app_config_t glob_config;

void app_config_reset(void) {
    PAR_VALUE_GLOB(glob_sleep_time_minute) = 20;           //
    PAR_VALUE_GLOB(irda_baudrate) = 9600;                  //
    PAR_VALUE_GLOB(es232_buzzer_frequency) = F_1_00K;      //
    PAR_VALUE_GLOB(es232_polling_time_ms) = 10;            //
    PAR_VALUE_GLOB(lcd_backlight_once_time_sec) = 1 * 60;  //
}

void app_config_read(void) {
    //
}

void app_config_write(void) {
    //
}

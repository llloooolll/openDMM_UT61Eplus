#include "app_config.h"

#include "ao_meter.h"
#include "es232.h"
#include "gpio.h"
#include "io_config.h"
#include "ulog.h"

app_config_t glob_config;

void app_config_reset(void) {
    glob_config.glob_sleep_time_minute = 20;           //
    glob_config.irda_baudrate = 9600;                  //
    glob_config.es232_buzzer_frequency = F_1_00K;      //
    glob_config.es232_polling_time_ms = 10;            //
    glob_config.lcd_backlight_once_time_sec = 1 * 60;  //
}

void app_config_read(void) {
    //
}

void app_config_write(void) {
    //
}

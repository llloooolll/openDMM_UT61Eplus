#include "app_config.h"

#include "ao_meter.h"
#include "es232.h"
#include "gpio.h"
#include "io_config.h"
#include "ulog.h"

app_config_t glob_config;

uint16_t cal_value[128];

void app_config_reset(void) {
    glob_config.es232_calibration_value = &cal_value[0];  //
    glob_config.glob_auto_sleep_enable = 1;               //
    glob_config.glob_sleep_time_minute = 15;              //
    glob_config.irda_baudrate = 9600;                     //
    glob_config.es232_buzzer_frequency = F_1_00K;         // 蜂鸣频率
    glob_config.es232_polling_time_ms = 10;               //
    glob_config.lcd_backlight_once_time_sec = 2 * 60;     //
    glob_config.buzzer_short_ms = 150;                    //
    glob_config.buzzer_long_ms = 400;                     //
}

void app_config_read(void) {
    glob_config.glob_config_valid = 0;
    //
}

void app_config_write(void) {
    //
}

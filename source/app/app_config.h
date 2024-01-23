#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

#include "hc32l13x.h"

typedef struct _app_config_t {
    bool glob_auto_sleep_enable;           // 自动休眠开启
    uint8_t glob_sleep_time_minute;        // 自动休眠时间
    uint32_t irda_baudrate;                // 红外串口波特率
    uint8_t es232_buzzer_frequency;        // ES232控制的的蜂鸣器档位
    uint8_t es232_polling_time_ms;         // ES232数据轮询间隔
    uint32_t lcd_backlight_once_time_sec;  // LCD背光点亮时间
} app_config_t;

extern app_config_t glob_config;

void app_config_reset(void);
void app_config_read(void);
void app_config_write(void);

#endif

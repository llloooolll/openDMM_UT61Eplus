#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

#include "hc32l13x.h"

#define ADD_PAR(name, type) \
    struct _par_##name {    \
        char *id;           \
        type value;         \
    } par_##name

#define PAR_VALUE(cfg, name) cfg.par_##name.value

typedef struct _app_config_t {
    ADD_PAR(glob_sleep_time_minute, uint8_t);  // 自动休眠时间
    ADD_PAR(irda_baudrate, uint32_t);          // 红外串口波特率
    ADD_PAR(es232_buzzer_frequency, uint8_t);  // ES232控制的的蜂鸣器频率
    ADD_PAR(es232_polling_time_ms, uint8_t);         // ES232数据轮询间隔
    ADD_PAR(lcd_backlight_once_time_sec, uint32_t);  // LCD背光点亮时间
} app_config_t;

#define PAR_VALUE_GLOB(name) glob_config.par_##name.value

extern app_config_t glob_config;

void app_config_reset(void);
void app_config_read(void);
void app_config_write(void);

#endif

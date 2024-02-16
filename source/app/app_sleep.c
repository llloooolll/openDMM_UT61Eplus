#include "app_sleep.h"

#include "ao_meter.h"
#include "app_config.h"
#include "gpio.h"
#include "io_config.h"
#include "rtc.h"
#include "sysctrl.h"
#include "ulog.h"

static uint8_t sleep_ticks = 0;

void RTC_IRQHandler(void) {
    if (rtc_get_prd_status()) {
        rtc_clean_prd_status();
        if (sleep_ticks > 0) {
            sleep_ticks--;
        } else {
            QACTIVE_POST_ISR(&ao_meter, AO_METER_ALARM_SIG, 0U);
        }
    }

    if (rtc_get_alarm_status()) {
        rtc_clean_alarm_status();
    }
}

void app_sleep_set_time(uint8_t time_minutes) {
    sleep_ticks = time_minutes;  //
}

void app_sleep_init(void) {
    sysctrl_enable_peripheral_clk(sysctrl_peripheral_clk_rtc, 1);
    sysctrl_enable_clock(sysctrl_clk_rcl, 1);
    rtc_enable_count(0);
    rtc_set_rtc_clk(rtc_clk_rcl);
    rtc_enable_24count(1);

    rtc_time_t time = {
        .second = 0,
        .minute = 0,
        .hour = 0,
        .day = DEC2BCD(14),
        .week = DEC2BCD(0),
        .month = DEC2BCD(1),
        .year = DEC2BCD(24),
    };
    rtc_set_time(&time);

    rtc_set_prd_source(0);
    rtc_set_prd_mult(rtc_mult_prd_minute);  // 分钟
    rtc_clean_prd_status();                 // 清除
    rtc_enable_irq(1);                      // 和闹钟共用开关

    rtc_enable_count(1);  // 运行

    NVIC_EnableIRQ(RTC_IRQn);
}

/**
 * @brief 进入休眠
 *
 */
void app_sleep_enty(void) {
    //
}

/**
 * @brief 退出休眠
 *
 */
void app_sleep_exit(void) {
    //
}
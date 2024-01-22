#include "meter_sleep.h"

#include "ao_meter.h"
#include "app_config.h"
#include "gpio.h"
#include "io_config.h"
#include "rtc.h"
#include "sysctrl.h"
#include "ulog.h"

void RTC_IRQHandler(void) {
    rtc_clean_alarm_status();
    rtc_enable_alarm(0);
    rtc_enable_count(0);
    QACTIVE_POST_ISR(&ao_meter, AO_METER_RTC_ALARM_SIG, 0U);
}

void meter_sleep_init(ao_meter_t *const me) {
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
}

void meter_sleep_alarm_set_delay(uint8_t time_minute) {
    rtc_enable_alarm_irq(0);
    rtc_enable_alarm(0);

    if (time_minute == 0) {
        return;
    }

    rtc_time_t time_now;
    rtc_read_time(&time_now);

    rtc_alarm_t alarm_time = {
        .minute = time_now.minute,
        .hour = time_now.hour,
    };

    rtc_alarm_t alarm_time_delay = {
        .minute = DEC2BCD(time_minute % 60),
        .hour = DEC2BCD(time_minute / 60),
        .alarm_en = alarm_en_all,
    };

    rtc_alarm_time_add(&alarm_time, &alarm_time_delay);
    rtc_set_alarm(&alarm_time);

    ULOG_INFO("init sleep after %d minutes\r\n", time_minute);
    rtc_enable_alarm_irq(1);
    rtc_enable_alarm(1);
    NVIC_EnableIRQ(RTC_IRQn);
}

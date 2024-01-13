#ifndef __RTC_H__
#define __RTC_H__

#include "binary.h"
#include "hc32l13x.h"

typedef enum _rtc_clk_t {
    rtc_clk_xtl = B000,      // 外部低速晶振
    rtc_clk_rcl = B010,      // 内部低速RC
    rtc_clk_xth128 = B100,   // XTH 4M
    rtc_clk_xth256 = B101,   // XTH 8M
    rtc_clk_xth512 = B110,   // XTH 16M
    rtc_clk_xth1024 = B111,  // XTH 32M
} rtc_clk_t;

typedef struct _rtc_time_t {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t week;
    uint8_t month;
    uint8_t year;
} rtc_time_t;

typedef enum _rtc_alarm_en_t {
    alarm_en_sunday = 1U << 0,     // 周日
    alarm_en_monday = 1U << 1,     // 周一
    alarm_en_tuesday = 1U << 2,    // 周二
    alarm_en_wednesday = 1U << 3,  // 周三
    alarm_en_thursday = 1U << 4,   // 周四
    alarm_en_friday = 1U << 5,     // 周五
    alarm_en_saturday = 1U << 6,   // 周六
    alarm_en_all = B1111111,
} rtc_alarm_en_t;

typedef struct _rtc_alarm_t {
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    rtc_alarm_en_t alarm_en;
} rtc_alarm_t;

void rtc_set_rtc_clk(rtc_clk_t clk);
void rtc_enable_alarm(bool flag);
void rtc_enable_24count(bool flag);
void rtc_enable_count(bool flag);
void rtc_enable_alarm_irq(bool flag);
bool rtc_get_alarm_status(void);
void rtc_clean_alarm_status(void);
void rtc_set_time(rtc_time_t *time);
void rtc_set_alarm(rtc_alarm_t *time);

#endif

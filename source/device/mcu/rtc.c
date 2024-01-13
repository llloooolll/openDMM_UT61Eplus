#include "rtc.h"

#include "binary.h"
#include "bits.h"
#include "ctype.h"
#include "sysctrl.h"

/**
 * @brief 设置RTC时钟
 *
 * @param clk
 */
void rtc_set_rtc_clk(rtc_clk_t clk) { M0P_RTC->CR1_f.CKSEL = clk; }

/**
 * @brief 使能闹钟
 *
 * @param flag
 */
void rtc_enable_alarm(bool flag) { M0P_RTC->CR1_f.ALMEN = flag; }

void rtc_enable_24count(bool flag) { M0P_RTC->CR0_f.AMPM = flag; }

void rtc_enable_count(bool flag) { M0P_RTC->CR0_f.START = flag; }

void rtc_enable_alarm_irq(bool flag) { M0P_RTC->CR1_f.ALMIE = flag; }

bool rtc_get_alarm_status(void) { return M0P_RTC->CR1_f.ALMF; }

void rtc_clean_alarm_status(void) {
    if (M0P_RTC->CR1_f.ALMF) {
        M0P_RTC->CR1_f.ALMF = 0;
    }
}

void rtc_set_time(rtc_time_t *time) {
    if (M0P_RTC->CR0_f.START == 1) {
        M0P_RTC->CR1_f.WAIT = 1;  // 暂停寄存器更新
        while (M0P_RTC->CR1_f.WAITF == 0) {
        }
    }
    M0P_RTC->SEC = time->second;
    M0P_RTC->MIN = time->minute;
    M0P_RTC->HOUR = time->hour;
    M0P_RTC->DAY = time->day;
    M0P_RTC->WEEK = time->week;
    M0P_RTC->MON = time->month;
    M0P_RTC->YEAR = time->year;
    M0P_RTC->CR1_f.WAIT = 0;  // 继续寄存器更新
    if (M0P_RTC->CR0_f.START == 1) {
        while (M0P_RTC->CR1_f.WAITF == 1) {
        }
    }
    // 整个函数需要在1秒内运行完
}

void rtc_set_alarm(rtc_alarm_t *time) {
    M0P_RTC->CR1_f.ALMEN = 0;
    M0P_RTC->CR1_f.ALMIE = 1;
    M0P_RTC->ALMMIN = time->minute;
    M0P_RTC->ALMHOUR = time->hour;
    M0P_RTC->ALMWEEK = time->alarm_en;
    M0P_RTC->CR1_f.ALMEN = 1;
}

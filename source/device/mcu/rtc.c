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
void rtc_set_rtc_clk(rtc_clk_t clk) {
    M0P_RTC->CR1_f.CKSEL = clk;  //
}

/**
 * @brief 使能闹钟
 *
 * @param flag
 */
void rtc_enable_alarm(bool flag) {
    M0P_RTC->CR1_f.ALMEN = flag;  //
}

/**
 * @brief 使能RTC低功耗模式
 *
 * @param flag
 */
void rtc_enable_lpm(bool flag) {
    // M0P_RTC->CR1_f.WAIT = 1;             // 暂停寄存器更新
    // while (M0P_RTC->CR1_f.WAITF == 0) {  // 查询直到1
    // }
    // M0P_RTC->CR1_f.WAIT = 0;             // 暂停寄存器更新
    // while (M0P_RTC->CR1_f.WAITF == 1) {  // 查询直到0
    // }

    M0P_SYSCTRL->SYSCTRL1_f.RTC_LPW = flag;
}

/**
 * @brief 设置24小时计数模式
 *
 * @param flag
 */
void rtc_enable_24count(bool flag) {
    M0P_RTC->CR0_f.AMPM = flag;  //
}

/**
 * @brief 使能计数
 *
 * @param flag
 */
void rtc_enable_count(bool flag) {
    M0P_RTC->CR0_f.START = flag;  //
}

/**
 * @brief 使能闹钟和周期中断
 *
 * @param flag
 */
void rtc_enable_irq(bool flag) {
    M0P_RTC->CR1_f.ALMIE = flag;  //
}

/**
 * @brief 读取闹钟标志
 *
 * @return true
 * @return false
 */
bool rtc_get_alarm_status(void) {
    return M0P_RTC->CR1_f.ALMF;  //
}

/**
 * @brief 读取周期标志
 *
 * @return true
 * @return false
 */
bool rtc_get_prd_status(void) {
    return M0P_RTC->CR1_f.PRDF;  //
}

/**
 * @brief 清除闹钟标志
 *
 */
void rtc_clean_alarm_status(void) {
    if (M0P_RTC->CR1_f.ALMEN) {
        M0P_RTC->CR1_f.ALMF = 0;
    }
}

/**
 * @brief 清除周期标志
 *
 */
void rtc_clean_prd_status(void) {
    M0P_RTC->CR1_f.PRDF = 0;  //
}

/**
 * @brief 设置RTC时间
 *
 * @param time
 */
void rtc_set_time(rtc_time_t *time) {
    if (M0P_RTC->CR0_f.START == 1) {
        M0P_RTC->CR1_f.WAIT = 1;             // 暂停寄存器更新
        while (M0P_RTC->CR1_f.WAITF == 0) {  // 查询直到1
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
        while (M0P_RTC->CR1_f.WAITF == 1) {  // 查询直到0
        }
    }
    // 整个函数需要在1秒内运行完
}

/**
 * @brief 读取RTC时间
 *
 * @param time
 */
void rtc_read_time(rtc_time_t *time) {
    if (M0P_RTC->CR0_f.START == 1) {
        M0P_RTC->CR1_f.WAIT = 1;             // 暂停寄存器更新
        while (M0P_RTC->CR1_f.WAITF == 0) {  // 查询直到1
        }
    }

    time->second = M0P_RTC->SEC;
    time->minute = M0P_RTC->MIN;
    time->hour = M0P_RTC->HOUR;
    time->day = M0P_RTC->DAY;
    time->week = M0P_RTC->WEEK;
    time->month = M0P_RTC->MON;
    time->year = M0P_RTC->YEAR;

    M0P_RTC->CR1_f.WAIT = 0;  // 继续寄存器更新
    if (M0P_RTC->CR0_f.START == 1) {
        while (M0P_RTC->CR1_f.WAITF == 1) {  // 查询直到0
        }
    }
}

/**
 * @brief 设置闹钟
 *
 * @param time
 */
void rtc_set_alarm(rtc_alarm_t *time) {
    M0P_RTC->CR1_f.ALMEN = 0;
    M0P_RTC->CR1_f.ALMIE = 1;
    M0P_RTC->ALMMIN = time->minute;
    M0P_RTC->ALMHOUR = time->hour;
    M0P_RTC->ALMWEEK = time->alarm_en;
    M0P_RTC->CR1_f.ALMEN = 1;
}

/**
 * @brief 闹钟时间相加，保存到第一个里
 *
 * @param addend1
 * @param addend2
 */
void rtc_alarm_time_add(rtc_alarm_t *addend1, rtc_alarm_t *addend2) {
    uint8_t minute = BCD2DEC(addend1->minute) + BCD2DEC(addend2->minute);
    uint8_t minute_carry = 0;
    if (minute > 59) {
        minute_carry = 1;
    }
    minute %= 60;  // 取余

    uint8_t hour = BCD2DEC(addend1->hour) + BCD2DEC(addend2->hour) + minute_carry;
    hour %= 24;  // 取余

    addend1->minute = DEC2BCD(minute);
    addend1->hour = DEC2BCD(hour);
}

/**
 * @brief 设置多钟单位的周期中断
 *
 * @param mult_prd 周期
 */
void rtc_set_prd_mult(rtc_mult_prd_t mult_prd) { M0P_RTC->CR0_f.PRDS = mult_prd; }

/**
 * @brief 设置周期设置来源
 *
 * @param flag 1PRDX，0PRDS
 */
void rtc_set_prd_source(bool flag) {
    M0P_RTC->CR0_f.PRDSEL = flag;  //
}
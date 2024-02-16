#ifndef __AO_METER_H__
#define __AO_METER_H__

#include "es232.h"
#include "lcd.h"
#include "qpn.h"

typedef struct _cal_value_t {
    // 相比于EEPROM中储存的数据，读出的顺序是反的，为了方便凑2字节的uint16_t值
    uint16_t value[128];  // 校准值 65535 > 22000 足够
    uint8_t cal;          // 校准标志 0 == 校准过

    // 可能的校准值
    /*
    [127 - 0] 直流电压校准值1V
    [127 - 1] 二极管档电压3V，精度0.001V
    [127 - 2]
    [127 - 3]
    [127 - 4]
    */

} cal_value_t;

typedef enum _meter_mode_t {
    meter_mode_acv = 0U,
    meter_mode_dcv,
    meter_mode_mv_ac,
    meter_mode_mv_dc,
    meter_mode_ohm_ohm,
    meter_mode_ohm_buz,
    meter_mode_ohm_dio,
    meter_mode_ohm_cap,
    meter_mode_hz_freq,
    meter_mode_hz_duty,
    meter_mode_hfe,
    meter_mode_ua_dc,
    meter_mode_ua_ac,
    meter_mode_ma_dc,
    meter_mode_ma_ac,
    meter_mode_a_dc,
    meter_mode_a_ac,
    meter_mode_ncv,
    meter_mode_max,
} meter_mode_t;

typedef enum _ao_meter_signal_t {
    AO_METER_READY_SIG = Q_USER_SIG,  //
    AO_METER_ADC_DONE_SIG,            // ADC转换完成
    AO_METER_KEY_SIG,                 // 按键处理
    AO_METER_MODE_SIG,                // 测量模式
    AO_METER_ALARM_SIG,               // RTC闹钟
    AO_METER_SLEEP_SIG,               //

    AO_METER_MAX_SIG
} ao_meter_signal_t;

typedef struct _ao_meter_t {
    QActive super;
    // 变量
    meter_mode_t mode;                 // ES232测量模式
    es232_write_t es232_write_buffer;  // ES232写入缓存
    es232_read_t es232_read_buffer;    // ES232读出缓存
    lcd_pixel_t lcd_pixel_buffer;      // LCD缓存

    uint32_t es232_range_value_max;  // 换挡上限测量值
    uint32_t es232_range_value_min;  // 换挡下限测量值
    uint8_t es232_range_max;         // 最大量程
    uint8_t es232_range_min;         // 最小量程
    bool es232_range_auto;           // 自动量程切换使能
    uint8_t delay_cycle_count;       // 自动切换计数
    bool es232_range_delay_dir;      // 量程切换方向

    bool es232_rel_flag;   // 相对测量标志
    bool es232_hold_flag;  // 暂停测量标志

    int32_t es232_value_rel;   // 相对测量值
    int8_t es232_power_rel;    // 相对测量值的幂
    int32_t es232_value_now;   // 测量值
    int8_t es232_power_now;    // 测量值的幂
    int32_t es232_show_value;  // 实际显示值
    int8_t es232_show_power;   // 实际显示值的幂

    uint8_t es232_range_delay_cycle;  // 量程切换延迟周期
} ao_meter_t;

extern ao_meter_t ao_meter;

void ao_meter_ctor(void);

#endif

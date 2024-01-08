#ifndef __AO_METER_H__
#define __AO_METER_H__

#include "es232.h"
#include "lcd.h"
#include "qpn.h"

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
    AO_METER_KEY_SIG,                 // 按键
    AO_METER_MODE_SIG,                // 测量模式

    AO_METER_MAX_SIG
} ao_meter_signal_t;

typedef struct _ao_meter_t {
    QActive super;
    uint8_t ready_count;
    uint8_t eeprom[256];               // EEPROM校准值
    meter_mode_t mode;                 // ES232测量模式
    es232_write_t es232_write_buffer;  // ES232写入缓存
    es232_read_t es232_read_buffer;    // ES232读出缓存
    lcd_pixel_t lcd_pixel_buffer;      // LCD缓存

    uint32_t es232_range_value_max;   // 换挡上限测量值
    uint32_t es232_range_value_min;   // 换挡下限测量值
    uint8_t es232_range_max;          // 最大量程
    uint8_t es232_range_min;          // 最小量程
    bool es232_range_auto;            // 自动量程切换使能
    uint8_t delay_cycle_count;        // 自动切换计数
    uint8_t es232_range_delay_cycle;  // 量程切换延迟周期
    bool es232_range_delay_dir;       // 量程切换方向

} ao_meter_t;

extern ao_meter_t ao_meter;

void ao_meter_ctor(void);

#endif

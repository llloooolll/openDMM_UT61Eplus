#include "meter_mode_ohm_dio.h"

#include <stdlib.h>
#include <string.h>

#include "ao_es232.h"
#include "ao_lcd.h"
#include "app_button.h"
#include "app_config.h"
#include "meter_range.h"
#include "ulog.h"

static int32_t meter_help_ohm_dio_cal(ao_meter_t *const me, int32_t value);
static int8_t meter_help_ohm_dio_get_power(ao_meter_t *const me, uint8_t range);

/**
 * @brief 初始化
 *
 * @param me
 */
void meter_ohm_dio_init(ao_meter_t *const me) {
    // LCD显示
    me->lcd_pixel_buffer.volt = 1;   // 单位伏特
    me->lcd_pixel_buffer.diode = 1;  // 二极管档

    me->es232_range_max = B000;
    me->es232_range_min = B000;
    me->es232_value_rel = 0;
    me->es232_power_rel = meter_help_ohm_dio_get_power(me, me->es232_range_min);

    // ES232设置
    me->es232_write_buffer.mode_msb = ES232_MODE_DIO;
    me->es232_write_buffer.range_msb = me->es232_range_min;
    me->es232_write_buffer.shbp_dcsel = 1;
    QACTIVE_POST(&ao_es232, AO_ES232_WRITE_CONFIG_SIG, &me->es232_write_buffer);
}

/**
 * @brief 数据处理
 *
 * @param me
 * @return QState
 */
QState meter_ohm_dio_adc(ao_meter_t *const me) {
    int32_t sadc_data = abs(es232_get_D0(&me->es232_read_buffer));  //

    if (!me->es232_hold_flag) {
        me->es232_value_now = meter_help_ohm_dio_cal(me, sadc_data);
        me->es232_power_now = meter_help_ohm_dio_get_power(me, me->es232_write_buffer.range_msb);
    }

    meter_help_calculate_relative_value(me);  // 计算相对值
    if ((me->es232_value_now > 29000) &&
        (me->es232_write_buffer.range_msb == me->es232_range_max)) {
        lcd_show_ol(&me->lcd_pixel_buffer);  // 显示OL
    } else {
        lcd_show_value(&me->lcd_pixel_buffer, me->es232_show_value, me->es232_show_power);
    }
    QACTIVE_POST(&ao_lcd, AO_LCD_REFRESH_SIG, (uint32_t)&me->lcd_pixel_buffer);

    return Q_HANDLED();
}

/**
 * @brief 按键处理
 *
 * @param me
 * @return QState
 */
QState meter_ohm_dio_key(ao_meter_t *const me) {
    QState status;
    switch (Q_PAR(me)) {
        case button_select_id << 4 | SINGLE_CLICK:
            QACTIVE_POST(me, AO_METER_MODE_SIG, meter_mode_ohm_cap);
            QACTIVE_POST((QActive *)&ao_es232, AO_ES232_ENABLE_BUZ_SIG,
                         glob_config.buzzer_short_ms);
            break;
        case button_rel_id << 4 | SINGLE_CLICK:
            me->es232_rel_flag = 1;
            me->es232_value_rel = me->es232_value_now;
            me->es232_power_rel = me->es232_power_now;
            me->lcd_pixel_buffer.delta = 1;
            QACTIVE_POST((QActive *)&ao_es232, AO_ES232_ENABLE_BUZ_SIG,
                         glob_config.buzzer_short_ms);
            break;
        case button_hold_id << 4 | SINGLE_CLICK:
            me->es232_hold_flag = !me->es232_hold_flag;
            me->lcd_pixel_buffer.hold = me->es232_hold_flag;
            QACTIVE_POST((QActive *)&ao_es232, AO_ES232_ENABLE_BUZ_SIG,
                         glob_config.buzzer_short_ms);
            break;
        default:
            ULOG_DEBUG("key don't care\r\n");
            break;
    }
    status = Q_HANDLED();
    return status;
}

/**
 * @brief 校准DCV档位
 *
 * @param value
 * @param range
 * @return int32_t
 */
static int32_t meter_help_ohm_dio_cal(ao_meter_t *const me, int32_t value) {
    if (glob_config.es232_calibration_valid) {
        value *= 3000;
        value += (glob_config.es232_calibration_value[1] / 2);  // 四舍五入
        value /= glob_config.es232_calibration_value[1];
    }
    return value;
}

/**
 * @brief 计算值的幂
 *
 * @param me
 * @param range
 * @return int8_t
 */
static int8_t meter_help_ohm_dio_get_power(ao_meter_t *const me, uint8_t range) {
    return -4 + (int8_t)range;
}
#include "meter_mode_mv_ac.h"

#include <stdlib.h>
#include <string.h>

#include "ao_es232.h"
#include "ao_lcd.h"
#include "app_button.h"
#include "meter_range.h"
#include "ulog.h"

static int32_t meter_help_mv_ac_cal(ao_meter_t *const me, int32_t value);
static int8_t meter_help_mv_ac_get_power(ao_meter_t *const me, uint8_t range);

/**
 * @brief 初始化
 *
 * @param me
 */
void meter_mv_ac_init(ao_meter_t *const me) {
    // LCD显示
    me->lcd_pixel_buffer.ac = 1;    // 交流档
    me->lcd_pixel_buffer.volt = 1;  // 单位伏特

    me->es232_range_max = B000;
    me->es232_range_min = B000;
    me->es232_value_rel = 0;
    me->es232_power_rel = meter_help_mv_ac_get_power(me, me->es232_range_min);

    // ES232设置
    me->es232_write_buffer.mode_msb = ES232_MODE_ADP;
    me->es232_write_buffer.ac = 1;
    me->es232_write_buffer.range_msb = me->es232_range_min;
    me->es232_write_buffer.ext_adp = 1;
    QACTIVE_POST(&ao_es232, AO_ES232_WRITE_CONFIG_SIG, &me->es232_write_buffer);
}

/**
 * @brief 数据处理
 *
 * @param me
 * @return QState
 */
QState meter_mv_ac_adc(ao_meter_t *const me) {
    int32_t sadc_data = es232_get_D0(&me->es232_read_buffer);
    // int32_t fadc_data = es232_get_D1(&me->es232_read_buffer);  // 不换量程

    if (!me->es232_hold_flag) {
        me->es232_value_now = meter_help_mv_ac_cal(me, sadc_data);
        me->es232_power_now =
            meter_help_mv_ac_get_power(me, me->es232_write_buffer.range_msb);
    }

    calculate_rel_result(me);  // 计算相对值
    if (abs(me->es232_value_now) > 30000) {
        lcd_show_ol(&me->lcd_pixel_buffer);
    } else {
        lcd_show_value(&me->lcd_pixel_buffer, me->es232_show_value,
                       me->es232_show_power);
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
QState meter_mv_ac_key(ao_meter_t *const me) {
    QState status;
    switch (Q_PAR(me)) {
        case button_select_id << 4 | SINGLE_CLICK:
            QACTIVE_POST(me, AO_METER_MODE_SIG, meter_mode_mv_dc);
            break;
        case button_rel_id << 4 | SINGLE_CLICK:
            me->es232_rel_flag = 1;
            me->es232_value_rel = me->es232_value_now;
            me->es232_power_rel = me->es232_power_now;
            me->lcd_pixel_buffer.delta = 1;
            break;
        case button_hold_id << 4 | SINGLE_CLICK:
            me->es232_hold_flag = !me->es232_hold_flag;
            me->lcd_pixel_buffer.hold = me->es232_hold_flag;
            break;
        default:
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
static int32_t meter_help_mv_ac_cal(ao_meter_t *const me, int32_t value) {
    return value;
}

/**
 * @brief 计算结果的幂
 *
 * @param me
 * @param range
 * @return int8_t
 */
static int8_t meter_help_mv_ac_get_power(ao_meter_t *const me, uint8_t range) {
    return -5 + (int8_t)range;
}

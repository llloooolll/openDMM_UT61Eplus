#include "meter_ohm_ohm.h"

#include <stdlib.h>
#include <string.h>

#include "ao_es232.h"
#include "ao_lcd.h"
#include "meter_button.h"
#include "meter_help_range.h"
#include "ulog.h"

static int32_t meter_help_ohm_ohm_cal(ao_meter_t *const me, int32_t value);
static int8_t meter_help_ohm_ohm_get_power(ao_meter_t *const me, uint8_t range);

/**
 * @brief 初始化
 *
 * @param me
 */
void meter_ohm_ohm_init(ao_meter_t *const me) {
    me->es232_write_buffer.mode_msb = ES232_MODE_RES;
    QACTIVE_POST(&ao_es232, AO_ES232_WRITE_CONFIG_SIG, &me->es232_write_buffer);

    me->lcd_pixel_buffer.ohm = 1;         // 单位欧姆
    me->lcd_pixel_buffer.range_auto = 1;  // 自动档
    lcd_set_ol_threshold(30000);

    me->es232_range_value_max = 30000;  // 最大
    me->es232_range_value_min = 2900;   // 最小
    me->es232_range_max = B110;         //
    me->es232_range_min = B000;         //

    me->es232_value_rel = 0;
    me->es232_power_rel = meter_help_ohm_ohm_get_power(me, me->es232_range_min);
}

/**
 * @brief 数据处理
 *
 * @param me
 * @return QState
 */
QState meter_ohm_ohm_adc(ao_meter_t *const me) {
    int32_t sadc_data = es232_get_D0(&me->es232_read_buffer);  //
    int32_t fadc_data = es232_get_D1(&me->es232_read_buffer);  //

    me->es232_value_now = meter_help_ohm_ohm_cal(me, sadc_data);
    me->es232_power_now =
        meter_help_ohm_ohm_get_power(me, me->es232_write_buffer.range_msb);

    int32_t show_value = 0;
    int8_t show_power = 0;
    calculate_rel_result(me, &show_value, &show_power);

    // ULOG_DEBUG("sadc = %d\n", abs(fadc_data));
    if (meter_help_range_sel(me, fadc_data * 100)) {
        lcd_show_value(&me->lcd_pixel_buffer, show_value, show_power);
        QACTIVE_POST(&ao_lcd, AO_LCD_REFRESH_SIG,
                     (uint32_t)&me->lcd_pixel_buffer);
    }

    return Q_HANDLED();
}

/**
 * @brief 按键处理
 *
 * @param me
 * @return QState
 */
QState meter_ohm_ohm_key(ao_meter_t *const me) {
    QState status;
    switch (Q_PAR(me)) {
        case button_select_id << 4 | SINGLE_CLICK:
            QACTIVE_POST(me, AO_METER_MODE_SIG, meter_mode_ohm_buz);
            break;
        case button_rel_id << 4 | SINGLE_CLICK:
            me->es232_rel_flag = 1;
            me->es232_value_rel = me->es232_value_now;
            me->es232_power_rel = me->es232_power_now;
            me->lcd_pixel_buffer.delta = 1;
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
static int32_t meter_help_ohm_ohm_cal(ao_meter_t *const me, int32_t value) {
    return value;
}

static int8_t meter_help_ohm_ohm_get_power(ao_meter_t *const me,
                                           uint8_t range) {
    return -2 + (int8_t)range;
}
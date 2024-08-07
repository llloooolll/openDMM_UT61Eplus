#include "meter_mode_dcv.h"

#include <stdlib.h>
#include <string.h>

#include "ao_es232.h"
#include "ao_lcd.h"
#include "app_button.h"
#include "app_config.h"
#include "meter_range.h"
#include "ulog.h"

/*
 * 直流电压测量模式
 * F[3:0] = B0000
 * AC = 0
 *
 * 电压档位
 * Q[2:0] =
 * B000: 0.30000V
 * B001:  3.0000V
 * B010:  30.000V
 * B011:  300.00V
 * B100:  1000.0V (max)
 *
 * 电压结果
 * SADC D0[18:0] 3cnvs/s
 *
 * 电压快速结果
 * FADC D1[9:0] 30cnvs/s
 *
 */

static int32_t meter_help_dcv_cal(ao_meter_t *const me, int32_t value);
static int8_t meter_help_dcv_get_power(ao_meter_t *const me, uint8_t range);

/**
 * @brief 初始化
 *
 * @param me
 */
void meter_dcv_init(ao_meter_t *const me) {
    // LCD显示
    me->lcd_pixel_buffer.dc = 1;          // 直流档
    me->lcd_pixel_buffer.volt = 1;        // 单位伏特
    me->lcd_pixel_buffer.range_auto = 1;  // 自动档

    me->es232_range_value_max = 30000;  // 最大
    me->es232_range_value_min = 2900;   // 最小
    me->es232_range_max = B100;         // 1000.0V
    me->es232_range_min = B001;         // 3.0000V
    me->es232_value_rel = 0;
    me->es232_power_rel = meter_help_dcv_get_power(me, me->es232_range_min);

    // ES232设置
    me->es232_write_buffer.mode_msb = ES232_MODE_V;
    me->es232_write_buffer.range_msb = me->es232_range_min;
    QACTIVE_POST(&ao_es232, AO_ES232_WRITE_CONFIG_SIG, &me->es232_write_buffer);
}

/**
 * @brief 数据处理
 *
 * @param me
 * @return QState
 */
QState meter_dcv_adc(ao_meter_t *const me) {
    int32_t sadc_data = es232_get_D0(&me->es232_read_buffer);
    int32_t fadc_data = es232_get_D1(&me->es232_read_buffer);

    /* hold 模式不更新显示值 */
    if (!me->es232_hold_flag) {
        me->es232_value_now = meter_help_dcv_cal(me, sadc_data);
        me->es232_power_now = meter_help_dcv_get_power(me, me->es232_write_buffer.range_msb);
    }

    /* 计算相对值 */
    meter_help_calculate_relative_value(me);

    /* 更新显示值 */
    if (abs(me->es232_value_now > 10000) &&
        (me->es232_write_buffer.range_msb == me->es232_range_max)) {
        lcd_show_ol(&me->lcd_pixel_buffer);  // 1000.0V 显示OL
    } else {
        if (meter_help_select_range(me, fadc_data * 100)) {
            lcd_show_value(&me->lcd_pixel_buffer, me->es232_show_value, me->es232_show_power);
        }
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
QState meter_dcv_key(ao_meter_t *const me) {
    QState status;
    switch (Q_PAR(me)) {
        case button_rel_id << 4 | SINGLE_CLICK:
            me->es232_rel_flag = 1;
            /* 保存当前测量结果作为相对值 */
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
static int32_t meter_help_dcv_cal(ao_meter_t *const me, int32_t value) {
    if (glob_config.es232_calibration_valid) {
        value *= 10000;
        value += (glob_config.es232_calibration_value[0] / 2);  // 四舍五入
        value /= glob_config.es232_calibration_value[0];
    }
    return value;
}

/**
 * @brief 根据档位计算ADC结果的幂
 *
 * @param me
 * @param range
 * @return int8_t
 */
static int8_t meter_help_dcv_get_power(ao_meter_t *const me, uint8_t range) {
    return -5 + (int8_t)range;
}

#include "meter_mode_ohm_cap.h"

#include <stdlib.h>
#include <string.h>

#include "ao_es232.h"
#include "ao_lcd.h"
#include "app_button.h"
#include "app_config.h"
#include "meter_range.h"
#include "ulog.h"

/*
 * 电容测量模式
 * F[3:0] = B1000
 * 
 * 电容容量档位
 * Q[2:0] = 
 * B000: 30.000nF 0.3s
 * B001: 300.00nF 0.75s
 * B010: 3.0000uF 0.75s
 * B011: 30.000uF 2.4s(max)
 * B100: 300.00uF 3.0s(max)
 * B101: 3.0000mF 6.0s(max)
 * B110: 30.000mF 6.0s(max)
 * B111: 300.00mF 30s(max)
 * 
 * 电容测量结果
 * D0[18:0] 3cnvs/s
 * 
 * 档位快速跳跃标志
 * ALARM = 1
 * 
 */

static int32_t meter_help_ohm_cap_cal(ao_meter_t *const me, int32_t value);
static int8_t meter_help_ohm_cap_get_power(ao_meter_t *const me, uint8_t range);

/**
 * @brief 初始化
 *
 * @param me
 */
void meter_ohm_cap_init(ao_meter_t *const me) {
    // LCD显示
    me->lcd_pixel_buffer.farad = 1;       // 单位法拉
    me->lcd_pixel_buffer.range_auto = 1;  // 自动档

    me->es232_range_delay_cycle = 0;    // 电容档FADC不工作，每个结果都是准的
    me->es232_range_value_max = 30000;  // 最大
    me->es232_range_value_min = 2900;   // 最小
    me->es232_range_max = B111;         // 300.00M
    me->es232_range_min = B000;         // 300.00
    me->es232_value_rel = 0;
    me->es232_power_rel = meter_help_ohm_cap_get_power(me, me->es232_range_min);

    // ES232设置
    me->es232_write_buffer.mode_msb = ES232_MODE_CAP;
    me->es232_write_buffer.range_msb = me->es232_range_min;
    QACTIVE_POST(&ao_es232, AO_ES232_WRITE_CONFIG_SIG, &me->es232_write_buffer);
}

/**
 * @brief 数据处理
 *
 * @param me
 * @return QState
 */
QState meter_ohm_cap_adc(ao_meter_t *const me) {
    int32_t sadc_data = abs(es232_get_D0(&me->es232_read_buffer));  //

    if (!me->es232_hold_flag) {
        me->es232_value_now = meter_help_ohm_cap_cal(me, sadc_data);
        me->es232_power_now = meter_help_ohm_cap_get_power(me, me->es232_write_buffer.range_msb);
    }

    calculate_rel_result(me);  // 计算相对值
    if (abs(me->es232_value_now > 30000) &&
        (me->es232_write_buffer.range_msb == me->es232_range_max)) {
        lcd_show_ol(&me->lcd_pixel_buffer);
    } else {
        if (meter_range_sel(me, me->es232_value_now)) {
            if (me->es232_read_buffer.STA0 == 0) {
                lcd_show_value(&me->lcd_pixel_buffer, me->es232_show_value, me->es232_show_power);
            }
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
QState meter_ohm_cap_key(ao_meter_t *const me) {
    QState status;
    switch (Q_PAR(me)) {
        case button_select_id << 4 | SINGLE_CLICK:
            QACTIVE_POST(me, AO_METER_MODE_SIG, meter_mode_ohm_ohm);
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
static int32_t meter_help_ohm_cap_cal(ao_meter_t *const me, int32_t value) { return value; }

/**
 * @brief 根据档位计算ADC结果的幂
 *
 * @param me
 * @param range
 * @return int8_t
 */
static int8_t meter_help_ohm_cap_get_power(ao_meter_t *const me, uint8_t range) {
    return -12 + (int8_t)range;
}

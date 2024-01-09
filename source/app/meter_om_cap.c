#include "meter_om_cap.h"

#include <stdlib.h>
#include <string.h>

#include "ao_es232.h"
#include "ao_lcd.h"
#include "meter_button.h"
#include "meter_help_range.h"
#include "ulog.h"

static int32_t meter_help_om_cap_cal(ao_meter_t *const me, int32_t value);

/**
 * @brief 初始化
 *
 * @param me
 */
void meter_om_cap_init(ao_meter_t *const me) {
    memcpy(&me->es232_write_buffer, &me->es232_config_list[me->mode * 4],
           sizeof(es232_write_t));
    QACTIVE_POST(&ao_es232, AO_ES232_WRITE_CONFIG_SIG, &me->es232_write_buffer);

    // 清除显示
    memset(&me->lcd_pixel_buffer, 0x00, sizeof(lcd_pixel_t));
    me->lcd_pixel_buffer.farad = 1;       // 单位法拉
    me->lcd_pixel_buffer.range_auto = 1;  // 自动档
    lcd_set_ol_threshold(30000);

    me->es232_range_delay_cycle = 0;  // 电容档FADC不工作，每个结果都是准的
    me->es232_range_value_max = 30000;  // 最大
    me->es232_range_value_min = 2900;   // 最小
    me->es232_range_max = B111;         // 300.00M
    me->es232_range_min = B010;         // 300.00
}

/**
 * @brief 数据处理
 *
 * @param me
 * @return QState
 */
QState meter_om_cap_adc(ao_meter_t *const me) {
    int32_t sadc_data = es232_get_D0(&me->es232_read_buffer);  //
    // int32_t fadc_data = es232_get_D1(&me->es232_read_buffer); // FADC不工作
    sadc_data = meter_help_om_cap_cal(me, sadc_data);

    // ULOG_DEBUG("sadc = %d\n", abs(fadc_data));
    if (meter_help_range_sel(me, sadc_data)) {
        if (me->es232_read_buffer.STA0 == 1) {
            lcd_show_value(&me->lcd_pixel_buffer, 0, 0);
        } else {
            lcd_show_value(&me->lcd_pixel_buffer, sadc_data,
                           -12 + (int8_t)me->es232_write_buffer.q_msb);
        }
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
QState meter_om_cap_key(ao_meter_t *const me) {
    QState status;
    switch (Q_PAR(me)) {
        case button_select_id << 4 | SINGLE_CLICK:
            QACTIVE_POST(me, AO_METER_MODE_SIG, meter_mode_ohm_ohm);
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
static int32_t meter_help_om_cap_cal(ao_meter_t *const me, int32_t value) {
    return value;
}

#include "meter_dcv.h"

#include <string.h>

#include "ao_es232.h"
#include "ao_lcd.h"
#include "meter_help_range.h"
#include "ulog.h"

static int32_t meter_help_dcv_cal(ao_meter_t *const me, int32_t value);

/**
 * @brief 初始化
 *
 * @param me
 */
void meter_dcv_init(ao_meter_t *const me) {
    memcpy(&me->es232_write_buffer, &me->es232_config_list[me->mode * 4],
           sizeof(es232_write_t));
    QACTIVE_POST(&ao_es232, AO_ES232_WRITE_CONFIG_SIG, &me->es232_write_buffer);

    // 清除显示
    memset(&me->lcd_pixel_buffer, 0x00, sizeof(lcd_pixel_t));
    me->lcd_pixel_buffer.dc = 1;          // 直流档
    me->lcd_pixel_buffer.volt = 1;        // 单位伏特
    me->lcd_pixel_buffer.range_auto = 1;  // 自动档
    lcd_set_ol_threshold(30000);

    me->es232_range_value_max = 30000;  // 最大
    me->es232_range_value_min = 2900;   // 最小
    me->es232_range_max = B100;         // 1000.0V
    me->es232_range_min = B001;         // 3.0000V
}

/**
 * @brief 数据处理
 *
 * @param me
 * @return QState
 */
QState meter_dcv_adc(ao_meter_t *const me) {
    int32_t sadc_data = es232_get_D0(&me->es232_read_buffer);  //
    int32_t fadc_data = es232_get_D1(&me->es232_read_buffer);  //
    sadc_data = meter_help_dcv_cal(me, sadc_data);

    // ULOG_DEBUG("sadc = %d\n", abs(fadc_data));
    if (meter_help_range_sel(me, fadc_data * 100)) {
        lcd_show_value(&me->lcd_pixel_buffer, sadc_data,
                       -5 + (int8_t)me->es232_write_buffer.q_msb);
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
QState meter_dcv_key(ao_meter_t *const me) {
    QState status;
    switch (Q_PAR(me)) {
        case 0:
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
static int32_t meter_help_dcv_cal(ao_meter_t *const me, int32_t value) {
    return value;
}

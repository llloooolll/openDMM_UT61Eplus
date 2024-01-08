#include "meter_om_dio.h"

#include <stdlib.h>

#include "ao_es232.h"
#include "ao_lcd.h"
#include "meter_help_range.h"
#include "ulog.h"

static int32_t meter_help_om_dio_cal(ao_meter_t *const me, int32_t value);

/**
 * @brief 初始化
 *
 * @param me
 */
void meter_om_dio_lcd_init(ao_meter_t *const me) {
    lcd_set_ol_threshold(25000);
    me->lcd_pixel_buffer.volt = 1;   // 伏特
    me->lcd_pixel_buffer.diode = 1;  // 二极管档
}

/**
 * @brief 数据处理
 *
 * @param me
 * @return QState
 */
QState meter_om_dio_adc(ao_meter_t *const me) {
    int32_t sadc_data = es232_get_D0(&me->es232_read_buffer);  //
    // int32_t fadc_data = es232_get_D1(&me->es232_read_buffer); // FADC不工作
    sadc_data = meter_help_om_dio_cal(me, sadc_data);

    lcd_show_value(&me->lcd_pixel_buffer, sadc_data,
                   -4 + (int8_t)me->es232_write_buffer.q_msb);
    QACTIVE_POST(&ao_lcd, AO_LCD_REFRESH_SIG, (uint32_t)&me->lcd_pixel_buffer);

    return Q_HANDLED();
}

/**
 * @brief 按键处理
 *
 * @param me
 * @return QState
 */
QState meter_om_dio_key(ao_meter_t *const me) {
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
static int32_t meter_help_om_dio_cal(ao_meter_t *const me, int32_t value) {
    return value;
}

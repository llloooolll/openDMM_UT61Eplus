#include "meter_om_om.h"
#include "ao_lcd.h"
#include "ulog.h"
#include <stdlib.h>
#include "ao_es232.h"
#include "meter_help_range.h"

static int32_t meter_help_dcv_cal(ao_meter_t *const me, int32_t value, uint8_t range);

/**
 * @brief 初始化
 *
 * @param me
 */
void meter_om_om_lcd_init(ao_meter_t *const me)
{
    me->lcd_pixel_buffer.ohm = 1;        // 欧姆
    me->lcd_pixel_buffer.range_auto = 1; // 自动档位

    me->es232_range_value_max = 30000; // 最大
    me->es232_range_value_min = 2900;  // 最小
    me->es232_range_max = B110;        // 300.00M
    me->es232_range_min = B000;        // 300.00
}

/**
 * @brief 数据处理
 *
 * @param me
 * @return QState
 */
QState meter_om_om_adc(ao_meter_t *const me)
{
    int32_t adc_data = es232_get_D0(&me->es232_read_buffer);  //
    int32_t fadc_data = es232_get_D1(&me->es232_read_buffer); //
    adc_data = meter_help_dcv_cal(me, adc_data, me->es232_write_buffer.q_msb);
    lcd_show_value(&me->lcd_pixel_buffer, adc_data, -4 + me->es232_write_buffer.q_msb + 2U);
    QACTIVE_POST(&ao_lcd, AO_LCD_REFRESH_SIG, (uint32_t)&me->lcd_pixel_buffer);

    // ULOG_DEBUG("sadc = %d\n", abs(fadc_data));
    meter_help_range_sel(me, fadc_data * 100); // 有效值

    return Q_HANDLED();
}

/**
 * @brief 按键处理
 *
 * @param me
 * @return QState
 */
QState meter_om_om_key(ao_meter_t *const me)
{
    QState status;
    switch (Q_PAR(me))
    {
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
static int32_t meter_help_dcv_cal(ao_meter_t *const me, int32_t value, uint8_t range)
{
    return value;
}
#include "meter_acv.h"
#include "ao_lcd.h"
#include "ulog.h"

static int32_t meter_acv_cal(ao_meter_t *const me, int32_t value, uint8_t range);

/**
 * @brief 初始化
 *
 * @param me
 */
void meter_acv_lcd_init(ao_meter_t *const me)
{
    me->lcd_pixel_buffer.dc = 1;
    me->lcd_pixel_buffer.volt = 1;
}

/**
 * @brief 数据处理
 *
 * @param me
 * @return QState
 */
QState meter_acv_adc(ao_meter_t *const me)
{
    int32_t data = es232_get_D0(&me->es232_read_buffer);
    data = meter_acv_cal(me, data, 0);
    lcd_show_value(&me->lcd_pixel_buffer, data, 4);
    QACTIVE_POST(&ao_lcd, AO_LCD_REFRESH_SIG, (uint32_t)&me->lcd_pixel_buffer);

    return Q_HANDLED();
}

/**
 * @brief 按键处理
 *
 * @param me
 * @return QState
 */
QState meter_acv_key(ao_meter_t *const me)
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
 * @brief 校准
 *
 * @param value
 * @param range
 * @return int32_t
 */
static int32_t meter_acv_cal(ao_meter_t *const me, int32_t value, uint8_t range)
{
    return value;
}

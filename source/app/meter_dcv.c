#include "meter_dcv.h"
#include "ao_lcd.h"
#include "ulog.h"
#include <stdlib.h>
#include "ao_es232.h"

#define RANGE_VALUE_DCV_TOP 30000U
#define RANGE_VALUE_DCV_BOT 3100U

static int32_t meter_dcv_cal(ao_meter_t *const me, int32_t value, uint8_t range);
static void meter_dcv_range_sel(ao_meter_t *const me, int32_t value);

/**
 * @brief 初始化
 *
 * @param me
 */
void meter_dcv_lcd_init(ao_meter_t *const me)
{
    me->lcd_pixel_buffer.dc = 1;
    me->lcd_pixel_buffer.volt = 1;
    me->lcd_pixel_buffer.range_auto = 1;

    me->es232_range_max = B100; // 1000.0V
    me->es232_range_min = B001; // 3.0000V
}

/**
 * @brief 数据处理
 *
 * @param me
 * @return QState
 */
QState meter_dcv_adc(ao_meter_t *const me)
{
    int32_t adc_data = es232_get_D0(&me->es232_read_buffer);
    int32_t sadc_data = es232_get_D1(&me->es232_read_buffer);
    adc_data = meter_dcv_cal(me, adc_data, 0);
    lcd_show_value(&me->lcd_pixel_buffer, adc_data, 5U - me->es232_write_buffer.q_msb);
    QACTIVE_POST(&ao_lcd, AO_LCD_REFRESH_SIG, (uint32_t)&me->lcd_pixel_buffer);

    // ULOG_DEBUG("sadc = %d\n", abs(sadc_data));
    if (me->es232_range_auto)
    {
        meter_dcv_range_sel(me, sadc_data * 100);
    }

    return Q_HANDLED();
}

/**
 * @brief 按键处理
 *
 * @param me
 * @return QState
 */
QState meter_dcv_key(ao_meter_t *const me)
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
static int32_t meter_dcv_cal(ao_meter_t *const me, int32_t value, uint8_t range)
{
    return value;
}

static void meter_dcv_range_sel(ao_meter_t *const me, int32_t value)
{
    static uint8_t delay_cycle;

    uint32_t u_value = abs(value);
    if (u_value > RANGE_VALUE_DCV_TOP)
    {
        delay_cycle++;
        if (delay_cycle > me->es232_range_delay_cycle)
        {
            delay_cycle = 0;
            if (me->es232_write_buffer.q_msb < me->es232_range_max)
            {
                me->es232_write_buffer.q_msb++;
                QACTIVE_POST(&ao_es232, AO_ES232_WRITE_CONFIG_SIG, &me->es232_write_buffer);
                ULOG_DEBUG("%d > %d\n", u_value, RANGE_VALUE_DCV_TOP);
                ULOG_DEBUG("value too large change range: %d\n", me->es232_write_buffer.q_msb);
            }
        }
    }
    else if (u_value < RANGE_VALUE_DCV_BOT)
    {
        delay_cycle++;
        if (delay_cycle > me->es232_range_delay_cycle)
        {
            delay_cycle = 0;
            if (me->es232_write_buffer.q_msb > me->es232_range_min)
            {
                me->es232_write_buffer.q_msb--;
                QACTIVE_POST(&ao_es232, AO_ES232_WRITE_CONFIG_SIG, &me->es232_write_buffer);
                ULOG_DEBUG("%d < %d\n", u_value, RANGE_VALUE_DCV_BOT);
                ULOG_DEBUG("value too small change range: %d\n", me->es232_write_buffer.q_msb);
            }
        }
    }
}

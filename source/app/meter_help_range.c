#include "meter_help_range.h"
#include "qpn.h"
#include "ao_es232.h"
#include "stdlib.h"

/**
 * @brief 检查并切换量程
 *
 * @param me
 * @param value 值
 * @return true 数据有效
 * @return false
 */
bool meter_help_range_sel(ao_meter_t *const me, int32_t value)
{
    bool value_meaningful = 0;

    if (!me->es232_range_auto)
    {
        return 1;
    }

    uint32_t u_value = abs(value);
    if (u_value > me->es232_range_value_max)
    {
        me->delay_cycle_count++;
        if (me->delay_cycle_count > me->es232_range_delay_cycle)
        {
            me->delay_cycle_count = 0;
            if (me->es232_write_buffer.q_msb < me->es232_range_max)
            {
                me->es232_write_buffer.q_msb++;
                QACTIVE_POST(&ao_es232, AO_ES232_WRITE_CONFIG_SIG, &me->es232_write_buffer);
                // ULOG_DEBUG("%d > %d\n", u_value, RANGE_VALUE_DCV_TOP);
                // ULOG_DEBUG("value too large change range: %d\n", me->es232_write_buffer.q_msb);
            }
        }
    }
    else if (u_value < me->es232_range_value_min)
    {
        me->delay_cycle_count++;
        if (me->delay_cycle_count > me->es232_range_delay_cycle)
        {
            me->delay_cycle_count = 0;
            if (me->es232_write_buffer.q_msb > me->es232_range_min)
            {
                me->es232_write_buffer.q_msb--;
                QACTIVE_POST(&ao_es232, AO_ES232_WRITE_CONFIG_SIG, &me->es232_write_buffer);
                // ULOG_DEBUG("%d < %d\n", u_value, RANGE_VALUE_DCV_BOT);
                // ULOG_DEBUG("value too small change range: %d\n", me->es232_write_buffer.q_msb);
            }
        }
    }
    else
    {
        value_meaningful = 1;
    }

    return value_meaningful;
}
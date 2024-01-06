#include "meter_help_range.h"
#include "qpn.h"
#include "ao_es232.h"
#include "ulog.h"
#include "stdlib.h"

#define CAP_RANGE_FAST 0 // 电容档快速换挡功能

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
    bool value_meaningful = 1;

    if (!me->es232_range_auto)
    {
        return 1;
    }

    // static int32_t last_value;
    // if (last_value == value)
    // {
    //     last_value = value;
    //     return 0;
    // }

#if (CAP_RANGE_FAST)
    if ((me->es232_read_buffer.ALARM == 1) && (me->mode == meter_mode_om_cap))
    {
        bool range_change = 0;
        switch (me->es232_write_buffer.q_msb)
        {
        case B000:
        case B001:
        case B010:
            me->es232_write_buffer.q_msb = B011;
            range_change = 1;
            break;
        case B011:
            me->es232_write_buffer.q_msb = B101;
            range_change = 1;
            break;
        // case B100:
        case B101:
            me->es232_write_buffer.q_msb = B111;
            range_change = 1;
            break;
        // case B110:
        // case B111:
        default:
            break;
        }

        if (range_change == 1)
        {
            me->delay_cycle_count = 0;
            QACTIVE_POST(&ao_es232, AO_ES232_WRITE_CONFIG_SIG, &me->es232_write_buffer);
            ULOG_DEBUG("faster change range: %d\n", me->es232_write_buffer.q_msb);
            return 0;
        }
    }
#endif

    uint32_t u32_value = abs(value);
    if (u32_value > me->es232_range_value_max)
    {
        if (me->es232_range_delay_dir == 0)
        {
            me->es232_range_delay_dir = 1;
            me->delay_cycle_count = 0;
        }
        me->delay_cycle_count++;

        if (me->delay_cycle_count > me->es232_range_delay_cycle)
        {
            me->delay_cycle_count = 0;
            if (me->es232_write_buffer.q_msb < me->es232_range_max) // 不超
            {
                // value_meaningful = 0;
                me->es232_write_buffer.q_msb++;
                QACTIVE_POST(&ao_es232, AO_ES232_WRITE_CONFIG_SIG, &me->es232_write_buffer);
                // ULOG_DEBUG("%d > %d\n", u32_value, me->es232_range_value_max);
                // ULOG_DEBUG("value too large change range: %d\n", me->es232_write_buffer.q_msb);
            }
        }
    }
    else if (u32_value < me->es232_range_value_min)
    {
        if (me->es232_range_delay_dir == 1)
        {
            me->es232_range_delay_dir = 0;
            me->delay_cycle_count = 0;
        }
        me->delay_cycle_count++;

        if (me->delay_cycle_count > me->es232_range_delay_cycle)
        {
            me->delay_cycle_count = 0;
            if (me->es232_write_buffer.q_msb > me->es232_range_min) // 不超
            {
                // value_meaningful = 0;
                me->es232_write_buffer.q_msb--;
                QACTIVE_POST(&ao_es232, AO_ES232_WRITE_CONFIG_SIG, &me->es232_write_buffer);
                // ULOG_DEBUG("%d < %d\n", u32_value, me->es232_range_value_min);
                // ULOG_DEBUG("value too small change range: %d\n", me->es232_write_buffer.q_msb);
            }
        }
    }
    return value_meaningful;
}
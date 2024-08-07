#include "meter_mode_hfe.h"

#include <stdlib.h>
#include <string.h>

#include "ao_es232.h"
#include "ao_lcd.h"
#include "app_button.h"
#include "app_config.h"
#include "meter_range.h"
#include "ulog.h"

/*
 * HFE测量模式
 * F[3:0] = B1001
 * AC = 1
 *
 * 信号源
 * Q[2:0] =
 * B000: ADP
 *
 * 电压结果
 * SADC D0[18:0] 3cnvs/s
 *
 * 电压快速结果
 * FADC D1[9:0] 30cnvs/s
 *
 */

static int32_t meter_help_hfe_cal(ao_meter_t *const me, int32_t value);
static int8_t meter_help_hfe_get_power(ao_meter_t *const me, uint8_t range);

/**
 * @brief 初始化
 *
 * @param me
 */
void meter_hfe_init(ao_meter_t *const me) {
    // LCD显示
    me->lcd_pixel_buffer.beta = 1;  // 放大倍数
    me->lcd_pixel_buffer.hfe = 1;

    me->es232_range_max = B000;
    me->es232_range_min = B000;
    me->es232_value_rel = 0;
    me->es232_power_rel = meter_help_hfe_get_power(me, me->es232_range_min);

    // ES232设置
    me->es232_write_buffer.mode_msb = ES232_MODE_ADP;
    me->es232_write_buffer.range_msb = me->es232_range_min;
    QACTIVE_POST(&ao_es232, AO_ES232_WRITE_CONFIG_SIG, &me->es232_write_buffer);
}

/**
 * @brief 数据处理
 *
 * @param me
 * @return QState
 */
QState meter_hfe_adc(ao_meter_t *const me) {
    // TODO 正确读取数据
    meter_help_hfe_cal(me, 0);

    return Q_HANDLED();
}

/**
 * @brief 按键处理
 *
 * @param me
 * @return QState
 */
QState meter_hfe_key(ao_meter_t *const me) {
    QState status;
    switch (Q_PAR(me)) {
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
static int32_t meter_help_hfe_cal(ao_meter_t *const me, int32_t value) { return value; }

/**
 * @brief 根据档位计算ADC结果的幂
 *
 * @param me
 * @param range
 * @return int8_t
 */
static int8_t meter_help_hfe_get_power(ao_meter_t *const me, uint8_t range) {
    return 0 + (int8_t)range;
}

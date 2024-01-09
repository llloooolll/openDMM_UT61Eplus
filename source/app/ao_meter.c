#include "ao_meter.h"

#include <string.h>

#include "ao_es232.h"
#include "ao_irda.h"
#include "ao_knob.h"
#include "ao_lcd.h"
#include "eeprom.h"
#include "meter_acv.h"
#include "meter_button.h"
#include "meter_dcv.h"
#include "meter_ohm_ohm.h"
#include "meter_ohm_buz.h"
#include "meter_ohm_cap.h"
#include "meter_ohm_dio.h"
#include "ulog.h"

// 对象
ao_meter_t ao_meter;

// 状态机
static QState ao_meter_init(ao_meter_t *const me);
static QState ao_meter_idle(ao_meter_t *const me);
static QState ao_meter_active(ao_meter_t *const me);

// 构造
void ao_meter_ctor(void) {
    ao_meter_t *const me = &ao_meter;
    QActive_ctor(&me->super, Q_STATE_CAST(&ao_meter_init));
}

static QState ao_meter_init(ao_meter_t *const me) {
    me->ready_count = 0;
    me->es232_range_delay_cycle = 2;
    me->mode = meter_mode_acv;
    me->es232_range_auto = 1;  // 自动换挡
    me->es232_buz_frq = F_1_00K;
    return Q_TRAN(&ao_meter_idle);
}

static QState ao_meter_idle(ao_meter_t *const me) {
    QState status;
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG:
            status = Q_HANDLED();
            break;
        case AO_METER_READY_SIG:
            if (Q_PAR(me) == 0) {
                me->ready_count++;
                if (me->ready_count >= 3) {
                    ULOG_DEBUG("ALL hardware done\n");
                    ULOG_DEBUG("build: %s %s\n", __DATE__, __TIME__);
                    me->cal_value.cal = eeprom_read_byte(0x00);
                    if (me->cal_value.cal == 0x00) {
                        ULOG_DEBUG("eeprom value exit %d\n",
                                   sizeof(cal_value_t));
                        eeprom_read_all((uint8_t *)&me->cal_value.value[0]);

                        ULOG_DEBUG("cal_value_0: %d\n",
                                   me->cal_value.value[127 - 0]);
                        ULOG_DEBUG("cal_value_1: %d\n",
                                   me->cal_value.value[127 - 1]);
                    }
                    QACTIVE_POST(&ao_es232, AO_ES232_ACTIVE_SIG, 1U);
                    QACTIVE_POST(&ao_lcd, AO_LCD_ACTIVE_SIG, 1U);
                    QACTIVE_POST(&ao_knob, AO_KNOB_ACTIVE_SIG, 1U);
                    meter_button_init();
                    status = Q_TRAN(&ao_meter_active);
                    break;
                }
            } else {
                ULOG_ERROR("hardware error\n");
            }
            status = Q_HANDLED();
            break;
        default:
            status = Q_SUPER(&QHsm_top);
            break;
    }
    return status;
}

static QState ao_meter_active(ao_meter_t *const me) {
    QState status;
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG:
            QActive_armX((QActive *)me, 1U, 10U, 0U);  // 按键循环
            status = Q_HANDLED();
            break;
        case Q_TIMEOUT1_SIG:
            button_ticks();
            QActive_armX((QActive *)me, 1U, 10U, 0U);
            status = Q_HANDLED();
            break;
        case AO_METER_MODE_SIG:
            // 档位有效
            if (Q_PAR(me) < meter_mode_max) {
                me->es232_value_rel = 0;
                me->es232_power_rel = 0;
                me->es232_rel_flag = 0;

                me->mode = Q_PAR(me);
                ULOG_DEBUG("ES232 mode: %d\n", me->mode);
                memset(&me->lcd_pixel_buffer, 0x00, sizeof(lcd_pixel_t));
                memset(&me->es232_write_buffer, 0x00, sizeof(es232_write_t));
                me->es232_write_buffer.buzzer_freq_lsb = me->es232_buz_frq;
                switch (me->mode) {
                    case meter_mode_acv:
                        meter_acv_init(me);
                        break;
                    case meter_mode_dcv:
                        meter_dcv_init(me);
                        break;
                    case meter_mode_ohm_ohm:
                        meter_ohm_ohm_init(me);
                        break;
                    case meter_mode_ohm_cap:
                        meter_ohm_cap_init(me);
                        break;
                    case meter_mode_ohm_buz:
                        meter_ohm_buz_init(me);
                        break;
                    case meter_mode_ohm_dio:
                        meter_ohm_dio_init(me);
                        break;
                    default:
                        break;
                }
            }
            QACTIVE_POST(&ao_knob, AO_KNOB_ACTIVE_SIG, 1U);  // 再探再报
            status = Q_HANDLED();
            break;
        case AO_METER_ADC_DONE_SIG:  // 数据
            memcpy(&me->es232_read_buffer, (es232_read_t *)Q_PAR(me),
                   sizeof(es232_read_t));
            switch (me->mode) {
                case meter_mode_acv:
                    status = meter_acv_adc(me);
                    break;
                case meter_mode_dcv:
                    status = meter_dcv_adc(me);
                    break;
                case meter_mode_ohm_ohm:
                    status = meter_ohm_ohm_adc(me);
                    break;
                case meter_mode_ohm_cap:
                    status = meter_ohm_cap_adc(me);
                    break;
                case meter_mode_ohm_buz:
                    status = meter_ohm_buz_adc(me);
                    break;
                case meter_mode_ohm_dio:
                    status = meter_ohm_dio_adc(me);
                    break;
                default:
                    status = Q_HANDLED();
                    break;
            }
            break;
        case AO_METER_KEY_SIG:  // 按键
            do {
                // 处理
                switch (Q_PAR(me)) {
                    case button_range_id << 4 | SINGLE_CLICK:
                        ULOG_DEBUG("key: range, event: click\n");
                        status = Q_HANDLED();
                        break;
                    case button_hold_id << 4 | LONG_PRESS_START:
                        QACTIVE_POST(&ao_lcd, AO_LCD_BL_SIG, 1000 * 60);
                        status = Q_HANDLED();
                        break;
                    default:  // 现在不处理的进各个档位
                        switch (me->mode) {
                            case meter_mode_acv:
                                status = meter_acv_key(me);
                                break;
                            case meter_mode_dcv:
                                status = meter_dcv_key(me);
                                break;
                            case meter_mode_ohm_ohm:
                                status = meter_ohm_ohm_key(me);
                                break;
                            case meter_mode_ohm_cap:
                                status = meter_ohm_cap_key(me);
                                break;
                            case meter_mode_ohm_buz:
                                status = meter_ohm_buz_key(me);
                                break;
                            case meter_mode_ohm_dio:
                                status = meter_ohm_dio_key(me);
                                break;
                            default:
                                status = Q_HANDLED();
                                break;
                        }
                }
            } while (0);
            break;
        default:
            status = Q_SUPER(&QHsm_top);
            break;
    }
    return status;
}

#include "ao_meter.h"

#include <string.h>

#include "ao_es232.h"
#include "ao_irda.h"
#include "ao_knob.h"
#include "ao_lcd.h"
#include "app_button.h"
#include "app_config.h"
#include "app_sleep.h"
#include "eeprom.h"
#include "meter_mode_a_ac.h"
#include "meter_mode_a_dc.h"
#include "meter_mode_acv.h"
#include "meter_mode_dcv.h"
#include "meter_mode_hfe.h"
#include "meter_mode_hz_duty.h"
#include "meter_mode_hz_freq.h"
#include "meter_mode_ma_ac.h"
#include "meter_mode_ma_dc.h"
#include "meter_mode_mv_ac.h"
#include "meter_mode_mv_dc.h"
#include "meter_mode_ncv.h"
#include "meter_mode_ohm_buz.h"
#include "meter_mode_ohm_cap.h"
#include "meter_mode_ohm_dio.h"
#include "meter_mode_ohm_ohm.h"
#include "meter_mode_ua_ac.h"
#include "meter_mode_ua_dc.h"
#include "ulog.h"

static const char *meter_mode_string[] = {
    "acv",      //
    "dcv",      //
    "mv_ac",    //
    "mv_dc",    //
    "ohm_ohm",  //
    "ohm_buz",  //
    "ohm_dio",  //
    "ohm_cap",  //
    "hz_freq",  //
    "hz_duty",  //
    "hfe",      //
    "ua_dc",    //
    "ua_ac",    //
    "ma_dc",    //
    "ma_ac",    //
    "a_dc",     //
    "a_ac",     //
    "ncv",      //
    "error",    //
};

// 对象
ao_meter_t ao_meter;

// 状态机
static QState ao_meter_init(ao_meter_t *const me);
static QState ao_meter_idle(ao_meter_t *const me);
static QState ao_meter_active(ao_meter_t *const me);
static QState ao_meter_sleep(ao_meter_t *const me);

static void meter_help_write_es232_global(ao_meter_t *const me);
static void meter_help_write_lcd_global(ao_meter_t *const me);
static void meter_help_read_calibration_value(ao_meter_t *const me);
static void meter_help_sleep_init(ao_meter_t *const me);

// 构造
void ao_meter_ctor(void) {
    ao_meter_t *const me = &ao_meter;
    QActive_ctor(&me->super, Q_STATE_CAST(&ao_meter_init));
}

static QState ao_meter_init(ao_meter_t *const me) {
    me->es232_range_auto = 1;  // 自动换挡
    return Q_TRAN(&ao_meter_idle);
}

static QState ao_meter_idle(ao_meter_t *const me) {
    QState status;
    static uint8_t init_status;
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG:
            init_status = 0;
            QACTIVE_POST(&ao_irda, AO_IRDA_READY_SIG, 0U);  //
            status = Q_HANDLED();
            break;
        case AO_METER_READY_SIG:
            if (Q_PAR(me) == 0) {
                switch (init_status) {
                    case 0:
                        QACTIVE_POST(&ao_lcd, AO_LCD_READY_SIG, 0U);  //
                        status = Q_HANDLED();
                        break;
                    case 1:
                        QACTIVE_POST(&ao_es232, AO_ES232_READY_SIG, 0U);  //
                        status = Q_HANDLED();
                        break;
                    case 2:
                        QACTIVE_POST(&ao_knob, AO_KNOB_READY_SIG, 0U);  //
                        status = Q_HANDLED();
                        break;
                    case 3:
                        ULOG_INFO("ALL hardware done\r\n");
                        QACTIVE_POST(&ao_es232, AO_ES232_ACTIVE_SIG, 1U);
                        QACTIVE_POST(&ao_lcd, AO_LCD_ACTIVE_SIG, 1U);
                        QACTIVE_POST(&ao_knob, AO_KNOB_ACTIVE_SIG, 1U);
                        QACTIVE_POST(&ao_irda, AO_IRDA_ACTIVE_SIG, 1U);
                        ULOG_INFO("build: %s %s\r\n", __DATE__, __TIME__);

                        // 读取校准值
                        meter_help_read_calibration_value(me);

                        meter_help_sleep_init(me);

                        status = Q_TRAN(&ao_meter_active);
                        break;
                    default:
                        status = Q_HANDLED();
                        break;
                }
                init_status++;
            } else {
                ULOG_ERROR("hardware error\r\n");
                status = Q_HANDLED();
            }
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
            QActive_armX((QActive *)me, 1U, 10U, 0U);  // 按键一次启动
            status = Q_HANDLED();
            break;
        case Q_TIMEOUT1_SIG:
            button_ticks();  // 按键循环
            QActive_armX((QActive *)me, 1U, 10U, 0U);
            status = Q_HANDLED();
            break;
        case AO_METER_ALARM_SIG:
            if (glob_config.glob_auto_sleep_enable) {
                QActive_disarmX((QActive *)me, 0U);
                QActive_disarmX((QActive *)me, 1U);
                status = Q_TRAN(&ao_meter_sleep);  // 休眠流程
            } else {
                // TODO 多段蜂鸣提醒
                app_sleep_set_time(glob_config.glob_sleep_time_minute);
                QACTIVE_POST(&ao_es232, AO_ES232_ENABLE_BUZ_SIG,
                             glob_config.buzzer_long_ms * 2);
                status = Q_HANDLED();
            }
            break;
        case AO_METER_MODE_SIG:
            if (Q_PAR(me) < meter_mode_max) {
                // 变量
                me->es232_value_rel = 0;
                me->es232_power_rel = 0;
                me->es232_rel_flag = 0;
                me->es232_hold_flag = 0;
                // 写入
                me->mode = Q_PAR(me);
                ULOG_INFO("ES232 mode: %s, id =%d\r\n",
                          meter_mode_string[me->mode], me->mode);
                // 重置休眠时间
                app_sleep_set_time(glob_config.glob_sleep_time_minute);
                // 清理配置
                memset(&me->lcd_pixel_buffer, 0x00, sizeof(lcd_pixel_t));
                memset(&me->es232_write_buffer, 0x00, sizeof(es232_write_t));
                // 写入全局配置
                meter_help_write_es232_global(me);
                meter_help_write_lcd_global(me);
                me->es232_range_delay_cycle = 2;
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
                    case meter_mode_mv_ac:
                        meter_mv_ac_init(me);
                        break;
                    case meter_mode_mv_dc:
                        meter_mv_dc_init(me);
                        break;
                    case meter_mode_hz_freq:
                        meter_hz_freq_init(me);
                        break;
                    case meter_mode_hz_duty:
                        meter_hz_duty_init(me);
                        break;
                    case meter_mode_ma_dc:
                        meter_ma_dc_init(me);
                        break;
                    case meter_mode_ma_ac:
                        meter_ma_ac_init(me);
                        break;
                    case meter_mode_ua_dc:
                        meter_ua_dc_init(me);
                        break;
                    case meter_mode_ua_ac:
                        meter_ua_ac_init(me);
                        break;
                    case meter_mode_a_dc:
                        meter_a_dc_init(me);
                        break;
                    case meter_mode_a_ac:
                        meter_a_ac_init(me);
                        break;
                    case meter_mode_ncv:
                        meter_ncv_init(me);
                        break;
                    case meter_mode_hfe:
                        meter_hfe_init(me);
                        break;
                    default:
                        break;
                }
            }
            // 再探再报
            QACTIVE_POST(&ao_knob, AO_KNOB_ACTIVE_SIG, 1U);
            status = Q_HANDLED();
            break;
        case AO_METER_ADC_DONE_SIG:
            // 数据
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
                case meter_mode_mv_ac:
                    status = meter_mv_ac_adc(me);
                    break;
                case meter_mode_mv_dc:
                    status = meter_mv_dc_adc(me);
                    break;
                case meter_mode_hz_freq:
                    status = meter_hz_freq_adc(me);
                    break;
                case meter_mode_hz_duty:
                    status = meter_hz_duty_adc(me);
                    break;
                case meter_mode_ma_dc:
                    status = meter_ma_dc_adc(me);
                    break;
                case meter_mode_ma_ac:
                    status = meter_ma_ac_adc(me);
                    break;
                case meter_mode_ua_dc:
                    status = meter_ua_dc_adc(me);
                    break;
                case meter_mode_ua_ac:
                    status = meter_ua_ac_adc(me);
                    break;
                case meter_mode_a_dc:
                    status = meter_a_dc_adc(me);
                    break;
                case meter_mode_a_ac:
                    status = meter_a_ac_adc(me);
                    break;
                case meter_mode_ncv:
                    status = meter_ncv_adc(me);
                    break;
                case meter_mode_hfe:
                    status = meter_hfe_adc(me);
                    break;
                default:
                    status = Q_HANDLED();
                    break;
            }
            break;
        case AO_METER_KEY_SIG:
            // 重置休眠时间
            app_sleep_set_time(glob_config.glob_sleep_time_minute);
            do {
                // 处理
                switch (Q_PAR(me)) {
                    case button_range_id << 4 | SINGLE_CLICK:
                        QACTIVE_POST((QActive *)&ao_es232,
                                     AO_ES232_ENABLE_BUZ_SIG,
                                     glob_config.buzzer_short_ms);
                        status = Q_HANDLED();
                        break;
                    case button_hold_id << 4 | LONG_PRESS_START:
                        QACTIVE_POST((QActive *)&ao_es232,
                                     AO_ES232_ENABLE_BUZ_SIG,
                                     glob_config.buzzer_short_ms);
                        QACTIVE_POST(
                            &ao_lcd, AO_LCD_BL_SIG,
                            glob_config.lcd_backlight_once_time_sec * 1000);
                        status = Q_HANDLED();
                        break;
                    case button_hz_id << 4 | LONG_PRESS_START:
                        // QACTIVE_POST(me, AO_METER_ALARM_SIG, 0U);

                        // 休眠测试
                        // QACTIVE_POST_ISR(&ao_meter, AO_METER_ALARM_SIG, 0U);

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
                            case meter_mode_mv_ac:
                                status = meter_mv_ac_key(me);
                                break;
                            case meter_mode_mv_dc:
                                status = meter_mv_dc_key(me);
                                break;
                            case meter_mode_hz_freq:
                                status = meter_hz_freq_key(me);
                                break;
                            case meter_mode_hz_duty:
                                status = meter_hz_duty_key(me);
                                break;
                            case meter_mode_ma_dc:
                                status = meter_ma_dc_key(me);
                                break;
                            case meter_mode_ma_ac:
                                status = meter_ma_ac_key(me);
                                break;
                            case meter_mode_ua_dc:
                                status = meter_ua_dc_key(me);
                                break;
                            case meter_mode_ua_ac:
                                status = meter_ua_ac_key(me);
                                break;
                            case meter_mode_a_dc:
                                status = meter_a_dc_key(me);
                                break;
                            case meter_mode_a_ac:
                                status = meter_a_ac_key(me);
                                break;
                            case meter_mode_ncv:
                                status = meter_ncv_key(me);
                                break;
                            case meter_mode_hfe:
                                status = meter_hfe_key(me);
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

static QState ao_meter_sleep(ao_meter_t *const me) {
    QState status;
    static uint8_t sleep_step;
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG:
            sleep_step = 0;
            QACTIVE_POST(&ao_es232, AO_ES232_ACTIVE_SIG, 0U);
            QACTIVE_POST(&ao_es232, AO_ES232_ENABLE_BUZ_SIG,
                         glob_config.buzzer_long_ms * 4);
            ULOG_DEBUG("meter sleep\r\n");
            status = Q_HANDLED();
            break;
        case AO_METER_SLEEP_SIG:
            switch (sleep_step) {
                case 0:
                    QACTIVE_POST(&ao_lcd, AO_LCD_ACTIVE_SIG, 0U);
                    status = Q_HANDLED();
                    break;
                case 1:
                    QACTIVE_POST(&ao_knob, AO_KNOB_ACTIVE_SIG, 0U);
                    status = Q_HANDLED();
                    break;
                case 2:
                    QACTIVE_POST(&ao_irda, AO_IRDA_ACTIVE_SIG, 0U);
                    status = Q_HANDLED();
                    break;
                case 3:
                    app_sleep_enty();
                    app_sleep_exit();
                    status = Q_HANDLED();
                    break;
                default:
                    status = Q_HANDLED();
                    break;
            }
            sleep_step++;
            break;
        default:
            status = Q_SUPER(&QHsm_top);
            break;
    }
    return status;
}

/**
 * @brief 写入ES232的全局配置
 *
 * @param me
 */
static void meter_help_write_es232_global(ao_meter_t *const me) {
    me->es232_write_buffer.buzzer_freq_lsb = glob_config.es232_buzzer_frequency;
}

/**
 * @brief 写入LCD的全局配置
 *
 * @param me
 */
static void meter_help_write_lcd_global(ao_meter_t *const me) {
    if (glob_config.glob_auto_sleep_enable) {
        me->lcd_pixel_buffer.time = 1;
    }
}

/**
 * @brief 写入LCD的全局配置
 *
 * @param me
 */
static void meter_help_read_calibration_value(ao_meter_t *const me) {
    glob_config.es232_calibration_valid = (eeprom_read_byte(0x00) == 0x00);
    if (glob_config.es232_calibration_valid) {
        ULOG_WARN("eeprom value exit\r\n");
        eeprom_read_all(glob_config.es232_calibration_value);
    } else {
        ULOG_ERROR("eeprom value non-exist\r\n");
    }
}

/**
 * @brief 休眠初始化
 *
 * @param me
 */
static void meter_help_sleep_init(ao_meter_t *const me) {
    app_sleep_set_time(glob_config.glob_sleep_time_minute);
    if (glob_config.glob_auto_sleep_enable) {
        QACTIVE_POST(&ao_es232, AO_ES232_ENABLE_BUZ_SIG,
                     glob_config.buzzer_short_ms);
        ULOG_INFO("auto sleep after %d minutes\r\n",
                  glob_config.glob_sleep_time_minute);
    } else {
        QACTIVE_POST(&ao_es232, AO_ES232_ENABLE_BUZ_SIG,
                     glob_config.buzzer_long_ms);
        ULOG_INFO("auto sleep off\r\n");
    }
}

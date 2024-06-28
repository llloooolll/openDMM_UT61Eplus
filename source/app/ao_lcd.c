#include "ao_lcd.h"

#include <string.h>

#include "ao_meter.h"
#include "ulog.h"

// 对象
ao_lcd_t ao_lcd;

// 状态机
static QState ao_lcd_init(ao_lcd_t *const me);
static QState ao_lcd_ready(ao_lcd_t *const me);
static QState ao_lcd_active(ao_lcd_t *const me);

// 构造
void ao_lcd_ctor(void) {
    ao_lcd_t *const me = &ao_lcd;
    QActive_ctor(&me->super, Q_STATE_CAST(&ao_lcd_init));
}

static QState ao_lcd_init(ao_lcd_t *const me) {
    memset(&me->lcd_pixel_buffer, 0U, sizeof(lcd_pixel_t));
    return Q_TRAN(&ao_lcd_ready);
}

static QState ao_lcd_ready(ao_lcd_t *const me) {
    QState status;
    switch (Q_SIG(me)) {
        case AO_LCD_READY_SIG:
            QActive_armX((QActive *)me, 0U, 5U, 0U);
            status = Q_HANDLED();
            break;
        case Q_TIMEOUT_SIG:
            bool lcd_init_result = lcd_init();
            if (!lcd_init_result) {
                lcd_test(1);  // 正常，满像素测试
            }
            ULOG_DEBUG("LCD init done\r\n");
            QACTIVE_POST(&ao_meter, AO_METER_READY_SIG, (uint32_t)lcd_init_result);
            status = Q_TRAN(&ao_lcd_active);
            break;
        default:
            status = Q_SUPER(&QHsm_top);
            break;
    }
    return status;
}

// 运行
static QState ao_lcd_active(ao_lcd_t *const me) {
    QState status;
    switch (Q_SIG(me)) {
        case AO_LCD_REFRESH_SIG:
            // 刷新
            memcpy(&me->lcd_pixel_buffer, (uint32_t *)Q_PAR(me), sizeof(lcd_pixel_t));
            lcd_refresh(&me->lcd_pixel_buffer);
            status = Q_HANDLED();
            break;
        case AO_LCD_BL_SIG:
            // 背光
            if (Q_PAR(me) > 0) {
                QActive_armX((QActive *)me, 0U, Q_PAR(me), 0U);
                lcd_enable_bl(1);
                ULOG_DEBUG("LCD backlight enable %d s\r\n", Q_PAR(me) / 1000);
            } else {
                lcd_enable_bl(0);
                ULOG_DEBUG("LCD backlight disable\r\n");
            }
            status = Q_HANDLED();
            break;
        case Q_TIMEOUT_SIG:
            // 背光到期
            QACTIVE_POST(me, AO_LCD_BL_SIG, 0);
            status = Q_HANDLED();
            break;
        case AO_LCD_ACTIVE_SIG:
            if (Q_PAR(me) == 0U) {
                ULOG_DEBUG("LCD sleep\r\n");
                QActive_disarmX((QActive *)me, 0U);
                QActive_disarmX((QActive *)me, 1U);
                lcd_enable_bl(0);
                lcd_enable(0);
                lcd_lpm();
                QACTIVE_POST(&ao_meter, AO_METER_SLEEP_SIG, 0U);
            } else {
                ULOG_DEBUG("LCD running\r\n");
                lcd_test(0);
                lcd_enable(1);
            }
            status = Q_HANDLED();
            break;
        default:
            status = Q_SUPER(&QHsm_top);
            break;
    }
    return status;
}

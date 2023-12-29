#include <string.h>
#include "qpn.h"
#include "ao_lcd.h"
#include "ao_meter.h"

// 对象
ao_lcd_t ao_lcd;

// 状态机
static QState ao_lcd_init(ao_lcd_t *const me);
static QState ao_lcd_idle(ao_lcd_t *const me);
static QState ao_lcd_active(ao_lcd_t *const me);

// 构造
void ao_lcd_ctor(void)
{
    ao_lcd_t *const me = &ao_lcd;
    QActive_ctor(&me->super, Q_STATE_CAST(&ao_lcd_init));
}

static QState ao_lcd_init(ao_lcd_t *const me)
{
    memset(&me->lcd_pixel_buffer, 0U, sizeof(lcd_pixel_t));
    return Q_TRAN(&ao_lcd_idle);
}

static QState ao_lcd_idle(ao_lcd_t *const me)
{
    QState status;
    switch (Q_SIG(me))
    {
    case Q_ENTRY_SIG:
    {
        bool lcd_init_result = lcd_init();
        if (!lcd_init_result)
        {
            lcd_test(1); // 正常，满像素测试
        }
        QACTIVE_POST(&ao_meter, AO_METER_READY_SIG, (uint32_t)lcd_init_result);
        status = Q_HANDLED();
        break;
    }
    case AO_LCD_ACTIVE_SIG:
    {
        lcd_test(0);
        status = Q_TRAN(&ao_lcd_active);
        break;
    }
    default:
    {
        status = Q_SUPER(&QHsm_top);
        break;
    }
    }
    return status;
}

// 运行
static QState ao_lcd_active(ao_lcd_t *const me)
{
    QState status;
    switch (Q_SIG(me))
    {
    case Q_ENTRY_SIG:
    {
        lcd_refresh(&me->lcd_pixel_buffer);
        status = Q_HANDLED();
        break;
    }
    default:
    {
        status = Q_SUPER(&QHsm_top);
        break;
    }
    }
    return status;
}

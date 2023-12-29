#include "qpn.h"
#include "ao_meter.h"
#include "ao_lcd.h"
#include "ao_es232.h"

// 对象
ao_meter_t ao_meter;

// 状态机
static QState ao_meter_init(ao_meter_t *const me);
static QState ao_meter_idle(ao_meter_t *const me);
static QState ao_meter_active(ao_meter_t *const me);

// 构造
void ao_meter_ctor(void)
{
    ao_meter_t *const me = &ao_meter;
    QActive_ctor(&me->super, Q_STATE_CAST(&ao_meter_init));
}

static QState ao_meter_init(ao_meter_t *const me)
{
    return Q_TRAN(&ao_meter_idle);
}

static QState ao_meter_idle(ao_meter_t *const me)
{
    QState status;
    static uint8_t ready_count;

    switch (Q_SIG(me))
    {
    case Q_ENTRY_SIG:
    {
        ready_count = 0;
        status = Q_HANDLED();
        break;
    }
    case AO_METER_READY_SIG:
    {
        if (Q_PAR(me) == 0)
        {
            ready_count++;
            if (ready_count >= 2)
            {
                QACTIVE_POST(&ao_lcd, AO_LCD_ACTIVE_SIG, 0U);
                QACTIVE_POST(&ao_es232, AO_ES232_ACTIVE_SIG, 0U);
                status = Q_TRAN(&ao_meter_active);
                break;
            }
        }
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

static QState ao_meter_active(ao_meter_t *const me)
{
    QState status;
    switch (Q_SIG(me))
    {
    default:
    {
        status = Q_SUPER(&QHsm_top);
        break;
    }
    }
    return status;
}

#include "qpn.h"
#include "ao_meter.h"
#include "ao_lcd.h"

// 对象
ao_meter_t ao_meter;

// 状态机
static QState ao_meter_init(ao_meter_t *const me);
static QState ao_meter_idle(ao_meter_t *const me);

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
    switch (Q_SIG(me))
    {
    case AO_METER_READY_SIG:
    {
        if (Q_PAR(me))
        {
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

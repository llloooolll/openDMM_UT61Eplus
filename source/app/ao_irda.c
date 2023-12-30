#include <string.h>
#include "qpn.h"
#include "ao_irda.h"
#include "ao_meter.h"

// 对象
ao_irda_t ao_irda;

// 状态机
static QState ao_irda_init(ao_irda_t *const me);
static QState ao_irda_idle(ao_irda_t *const me);
static QState ao_irda_active(ao_irda_t *const me);

// 构造
void ao_irda_ctor(void)
{
    ao_irda_t *const me = &ao_irda;
    QActive_ctor(&me->super, Q_STATE_CAST(&ao_irda_init));
}

static QState ao_irda_init(ao_irda_t *const me)
{
    return Q_TRAN(&ao_irda_idle);
}

static QState ao_irda_idle(ao_irda_t *const me)
{
    QState status;
    switch (Q_SIG(me))
    {
    case Q_ENTRY_SIG:
    {
        irda_init(9600);
        QACTIVE_POST(&ao_meter, AO_METER_READY_SIG, 0U);
        status = Q_HANDLED();
        break;
    }
    case AO_IRDA_ACTIVE_SIG:
    {
        status = Q_TRAN(&ao_irda_active);
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
static QState ao_irda_active(ao_irda_t *const me)
{
    QState status;
    switch (Q_SIG(me))
    {
    case Q_ENTRY_SIG:
    {
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

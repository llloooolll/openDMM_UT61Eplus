#include "ao_key.h"
#include "ulog.h"
#include "ao_meter.h"
#include "key_button.h"

// 对象
ao_key_t ao_key;

// 状态机
static QState ao_key_init(ao_key_t *const me);
static QState ao_key_ready(ao_key_t *const me);
static QState ao_key_idle(ao_key_t *const me);
static QState ao_key_active(ao_key_t *const me);

// 构造
void ao_key_ctor(void)
{
    ao_key_t *const me = &ao_key;
    QActive_ctor(&me->super, Q_STATE_CAST(&ao_key_init));
}

static QState ao_key_init(ao_key_t *const me)
{
    return Q_TRAN(&ao_key_ready);
}

static QState ao_key_ready(ao_key_t *const me)
{
    QState status;
    switch (Q_SIG(me))
    {
    case Q_ENTRY_SIG:
        QACTIVE_POST(me, AO_KEY_READY_SIG, 0U);
        status = Q_HANDLED();
        break;
    case AO_KEY_READY_SIG:
        status = Q_TRAN(&ao_key_idle);
        break;
    default:
        status = Q_SUPER(&QHsm_top);
        break;
    }
    return status;
}

static QState ao_key_idle(ao_key_t *const me)
{
    QState status;
    switch (Q_SIG(me))
    {
    case AO_KEY_ACTIVE_SIG:
        if (Q_PAR(me) > 0U)
        {
            
            status = Q_TRAN(&ao_key_active);
        }
        else
        {
            status = Q_HANDLED();
        }
        break;
    default:
        status = Q_SUPER(&QHsm_top);
        break;
    }
    return status;
}

// 运行
static QState ao_key_active(ao_key_t *const me)
{
    QState status;
    switch (Q_SIG(me))
    {
    case Q_ENTRY_SIG:
        ULOG_DEBUG("KEY active\n");
        QActive_armX((QActive *)me, 0U, 10U, 0U);
        status = Q_HANDLED();
        break;
    case Q_TIMEOUT_SIG:
        
        QActive_armX((QActive *)me, 0U, 10U, 0U);
        status = Q_HANDLED();
        break;
    default:
        status = Q_SUPER(&QHsm_top);
        break;
    }
    return status;
}

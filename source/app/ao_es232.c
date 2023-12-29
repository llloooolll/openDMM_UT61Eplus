#include <string.h>
#include "qpn.h"
#include "es232.h"
#include "ao_es232.h"
#include "ao_meter.h"

// 对象
ao_es232_t ao_es232;

// 状态机
static QState ao_es232_init(ao_es232_t *const me);
static QState ao_es232_idle(ao_es232_t *const me);
static QState ao_es232_active(ao_es232_t *const me);

// 构造
void ao_es232_ctor(void)
{
    ao_es232_t *const me = &ao_es232;
    QActive_ctor(&me->super, Q_STATE_CAST(&ao_es232_init));
}

static QState ao_es232_init(ao_es232_t *const me)
{
    memset(&me->es232_write_cmd, 0, sizeof(es232_write_cmd_t));
    return Q_TRAN(&ao_es232_idle);
}

// idle
static QState ao_es232_idle(ao_es232_t *const me)
{
    QState status;
    switch (Q_SIG(me))
    {
    case Q_ENTRY_SIG: // 进入
    {
        es232_gpio_init();
        es232_enable_power(1);
        QActive_armX((QActive *)me, 0U, 200U, 0U);
        status = Q_HANDLED();
        break;
    }
    case Q_TIMEOUT_SIG: // 等待上电
    {
        bool es232_init_result = es232_init();
        if (!es232_init_result)
        {
            es232_write(&me->es232_write_cmd);
        }
        QACTIVE_POST(&ao_meter, AO_METER_READY_SIG, (uint32_t)es232_init_result);
        status = Q_HANDLED();
        break;
    }
    case AO_ES232_ACTIVE_SIG:
    {
        status = Q_TRAN(&ao_es232_active);
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
static QState ao_es232_active(ao_es232_t *const me)
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

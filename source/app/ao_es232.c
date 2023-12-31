#include "ao_es232.h"
#include <string.h>
#include "ulog.h"
#include "ao_meter.h"

// 对象
ao_es232_t ao_es232;

// 状态机
static QState ao_es232_init(ao_es232_t *const me);
static QState ao_es232_ready(ao_es232_t *const me);
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
    return Q_TRAN(&ao_es232_ready);
}

// ready
static QState ao_es232_ready(ao_es232_t *const me)
{
    QState status;
    switch (Q_SIG(me))
    {
    case Q_ENTRY_SIG: // 进入
    {
        QACTIVE_POST(me, AO_ES232_READY_SIG, 0U);
    }
    case AO_ES232_READY_SIG:
    {
        es232_gpio_init();
        es232_enable_power(1);
        QActive_armX((QActive *)me, 0U, 200U, 0U); // 等待上电
        status = Q_HANDLED();
        break;
    }
    case Q_TIMEOUT_SIG:
    {
        bool es232_init_result = es232_init();
        if (!es232_init_result)
        {
            es232_write(&me->es232_write_cmd);
        }
        ULOG_DEBUG("es232 done\n");
        QACTIVE_POST(&ao_meter, AO_METER_READY_SIG, (uint32_t)es232_init_result);
        status = Q_TRAN(&ao_es232_idle);
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

// idle
static QState ao_es232_idle(ao_es232_t *const me)
{
    QState status;
    switch (Q_SIG(me))
    {
    case AO_ES232_ACTIVE_SIG:
    {
        ULOG_DEBUG("es232 active\n");
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
    case Q_ENTRY_SIG:
    {
        QActive_armX((QActive *)me, 0U, 1000U, 1000U);
        status = Q_HANDLED();
        break;
    }
    case Q_TIMEOUT_SIG:
    {
        es232_read(&me->es232_read_cmd);
        ULOG_DEBUG("ADC done\n");
        QACTIVE_POST_X(&ao_meter, 4U, AO_METER_ADC_DONE_SIG, (uint32_t)&me->es232_read_cmd);
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

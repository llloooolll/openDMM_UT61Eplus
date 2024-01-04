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
    me->es232_read_interval_time = 45;
    memset(&me->es232_write_buffer, 0, sizeof(es232_write_t));
    return Q_TRAN(&ao_es232_ready);
}

// ready
static QState ao_es232_ready(ao_es232_t *const me)
{
    QState status;
    switch (Q_SIG(me))
    {
    case Q_ENTRY_SIG: // 进入
        QACTIVE_POST(me, AO_ES232_READY_SIG, 0U);
        status = Q_HANDLED();
        break;
    case AO_ES232_READY_SIG:
        es232_gpio_init();
        es232_enable_buz(0);                       // 关哔哔
        es232_enable_power(1);                     // 开电源
        QActive_armX((QActive *)me, 0U, 200U, 0U); // 等待上电
        status = Q_HANDLED();
        break;
    case Q_TIMEOUT_SIG:
        bool es232_init_result = es232_init();
        if (!es232_init_result)
        {
            es232_write(&me->es232_write_buffer);
        }
        ULOG_DEBUG("ES232 init done\n");
        QACTIVE_POST(&ao_meter, AO_METER_READY_SIG, (uint32_t)es232_init_result);
        status = Q_TRAN(&ao_es232_idle);
        break;
    default:
        status = Q_SUPER(&QHsm_top);
        break;
    }
    return status;
}

// idle
static QState ao_es232_idle(ao_es232_t *const me)
{
    QState status;
    switch (Q_SIG(me))
    {
    case Q_ENTRY_SIG:
        es232_enable_buz(0);
        es232_read(&me->es232_read_buffer);
        status = Q_HANDLED();
        break;
    case AO_ES232_ACTIVE_SIG:
        ULOG_DEBUG("ES232 active\n");
        status = Q_TRAN(&ao_es232_active);
        break;
    default:
        status = Q_SUPER(&QHsm_top);
        break;
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
        QACTIVE_POST(me, AO_ES232_WRITE_CONFIG_SIG, &me->es232_write_buffer);
        status = Q_HANDLED();
        break;
    case Q_TIMEOUT_SIG:
        if (es232_is_data_ready())
        {
            es232_read(&me->es232_read_buffer);
            // ULOG_DEBUG("ADC done\n");
            QACTIVE_POST_X(&ao_meter, 4U, AO_METER_ADC_DONE_SIG,
                           (uint32_t)&me->es232_read_buffer);
        }
        status = Q_HANDLED();
        break;
    case AO_ES232_WRITE_CONFIG_SIG: // 配置
        QActive_disarmX((QActive *)me, 0U);
        memcpy(&me->es232_write_buffer, (es232_write_t *)Q_PAR(me),
               sizeof(es232_write_t));
        es232_write(&me->es232_write_buffer);
        QActive_armX((QActive *)me, 0U, me->es232_read_interval_time,
                     me->es232_read_interval_time);
        status = Q_HANDLED();
        break;
    case AO_ES232_ENABLE_BUZ_SIG:
        if (Q_PAR(me) > 0)
        {
            es232_enable_buz(1);
            QActive_armX((QActive *)me, 1U, Q_PAR(me), 0U);
        }
        else
        {
            es232_enable_buz(0);
        }
        status = Q_HANDLED();
        break;
    case Q_TIMEOUT1_SIG:
        QACTIVE_POST(me, AO_ES232_ENABLE_BUZ_SIG, 0);
        status = Q_HANDLED();
        break;
    default:
        status = Q_SUPER(&QHsm_top);
        break;
    }
    return status;
}

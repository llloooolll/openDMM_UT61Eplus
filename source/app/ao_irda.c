#include "ao_irda.h"

#include "ao_meter.h"
#include "irda.h"
#include "ulog.h"

// 对象
ao_irda_t ao_irda;

// 状态机
static QState ao_irda_init(ao_irda_t *const me);
static QState ao_irda_ready(ao_irda_t *const me);
static QState ao_irda_idle(ao_irda_t *const me);
static QState ao_irda_active(ao_irda_t *const me);

// 构造
void ao_irda_ctor(void) {
    ao_irda_t *const me = &ao_irda;
    QActive_ctor(&me->super, Q_STATE_CAST(&ao_irda_init));
}

static QState ao_irda_init(ao_irda_t *const me) {
    return Q_TRAN(&ao_irda_ready);
}

static QState ao_irda_ready(ao_irda_t *const me) {
    QState status;
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG:
            QACTIVE_POST(me, AO_IRDA_READY_SIG, 0U);
            status = Q_HANDLED();
            break;
        case AO_IRDA_READY_SIG:
            irda_init(9600);
            if (irda_is_exist()) {
                // ULOG_DEBUG("irda USB device exist\n");
                ulog_set_level(ulog_level_debug);
            }
            ULOG_DEBUG("IRDA done\n");
            QACTIVE_POST(&ao_meter, AO_METER_READY_SIG, 0U);
            status = Q_TRAN(&ao_irda_idle);
            break;
        default:
            status = Q_SUPER(&QHsm_top);
            break;
    }
    return status;
}

static QState ao_irda_idle(ao_irda_t *const me) {
    QState status;
    switch (Q_SIG(me)) {
        case AO_IRDA_ACTIVE_SIG:
            if (Q_PAR(me) > 0U) {
                status = Q_TRAN(&ao_irda_active);
            } else {
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
static QState ao_irda_active(ao_irda_t *const me) {
    QState status;
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG:
            ULOG_DEBUG("IRDA active\n");
            status = Q_HANDLED();
            break;
        default:
            status = Q_SUPER(&QHsm_top);
            break;
    }
    return status;
}

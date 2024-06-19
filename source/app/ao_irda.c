#include "ao_irda.h"

#include "ao_meter.h"
#include "irda.h"
#include "ulog.h"

// 对象
ao_irda_t ao_irda;

// 状态机
static QState ao_irda_init(ao_irda_t *const me);
static QState ao_irda_ready(ao_irda_t *const me);
static QState ao_irda_active(ao_irda_t *const me);

// 构造
void ao_irda_ctor(void) {
    ao_irda_t *const me = &ao_irda;
    QActive_ctor(&me->super, Q_STATE_CAST(&ao_irda_init));
}

static QState ao_irda_init(ao_irda_t *const me) {
    return Q_TRAN(&ao_irda_ready);  //
}

static QState ao_irda_ready(ao_irda_t *const me) {
    QState status;
    switch (Q_SIG(me)) {
        case AO_IRDA_READY_SIG:
            ULOG_INFO("IRDA done\r\n");
            QACTIVE_POST(&ao_meter, AO_METER_READY_SIG, 0U);
            status = Q_TRAN(&ao_irda_active);
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
        case AO_IRDA_CHAR_SIG:

            status = Q_HANDLED();
            break;
        case AO_IRDA_ACTIVE_SIG:
            if (Q_PAR(me) == 0U) {
                ULOG_INFO("IRDA sleep\r\n");
                irda_deinit();
                QACTIVE_POST(&ao_meter, AO_METER_SLEEP_SIG, 0U);
            } else {
                ULOG_INFO("IRDA running\r\n");
            }
        default:
            status = Q_SUPER(&QHsm_top);
            break;
    }
    return status;
}

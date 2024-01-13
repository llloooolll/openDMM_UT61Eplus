#include "ao_knob.h"

#include "ao_meter.h"
#include "knob_knob.h"
#include "ulog.h"

// 档位映射
static const uint8_t knob_mode_map[16] = {
    0xFF,                // NC
    0xFF,                // NC
    0xFF,                // NC
    meter_mode_hfe,      // 0x03
    0xFF,                // NC
    meter_mode_ncv,      // 0x05
    meter_mode_a_dc,     // 0x06
    meter_mode_ohm_ohm,  // 0x07
    0xFF,                // NC
    meter_mode_ma_dc,    // 0x09
    meter_mode_ua_dc,    // 0x0A
    meter_mode_mv_ac,    // 0x0B
    meter_mode_hz_freq,  // 0x0C
    meter_mode_dcv,      // 0x0D
    meter_mode_acv,      // 0x0E
    0xFF,                // NC
};

// 对象
ao_knob_t ao_knob;

// 状态机
static QState ao_knob_init(ao_knob_t *const me);
static QState ao_knob_ready(ao_knob_t *const me);
static QState ao_knob_idle(ao_knob_t *const me);
static QState ao_knob_active(ao_knob_t *const me);

// 构造
void ao_knob_ctor(void) {
    ao_knob_t *const me = &ao_knob;
    QActive_ctor(&me->super, Q_STATE_CAST(&ao_knob_init));
}

static QState ao_knob_init(ao_knob_t *const me) {
    me->knob_status = 0xFF;
    return Q_TRAN(&ao_knob_ready);
}

static QState ao_knob_ready(ao_knob_t *const me) {
    QState status;
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG:
            ULOG_DEBUG("KNOB init\n");
            knob_knob_init();
            QACTIVE_POST(me, AO_KNOB_READY_SIG, 0U);
            status = Q_HANDLED();
            break;
        case AO_KNOB_READY_SIG:
            ULOG_DEBUG("KNOB done\n");
            status = Q_TRAN(&ao_knob_idle);
            break;
        default:
            status = Q_SUPER(&QHsm_top);
            break;
    }
    return status;
}

static QState ao_knob_idle(ao_knob_t *const me) {
    QState status;
    switch (Q_SIG(me)) {
        case AO_KNOB_ACTIVE_SIG:
            if (Q_PAR(me) > 0U) {
                status = Q_TRAN(&ao_knob_active);
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
static QState ao_knob_active(ao_knob_t *const me) {
    QState status;
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG:
            ULOG_DEBUG("KNOB active\n");
            QActive_armX((QActive *)me, 0U, 100U, 0U);
            status = Q_HANDLED();
            break;
        case Q_TIMEOUT_SIG:
            do {
                uint8_t knob_status_now = knob_knob_ticks();
                if (knob_status_now != me->knob_status) {
                    me->knob_status = knob_status_now;
                    // 状态映射
                    QACTIVE_POST(&ao_meter, AO_METER_MODE_SIG,
                                 knob_mode_map[me->knob_status]);
                    status = Q_TRAN(&ao_knob_idle);
                } else {
                    QActive_armX((QActive *)me, 0U, 100U, 0U);
                    status = Q_HANDLED();
                }
            } while (0);
            break;
        case AO_KNOB_ACTIVE_SIG:
            if (Q_PAR(me) == 0U) {
                ULOG_DEBUG("KNOB sleep\n");
            }
            status = Q_HANDLED();
            break;
        default:
            status = Q_SUPER(&QHsm_top);
            break;
    }
    return status;
}

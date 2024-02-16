#include "ao_knob.h"

#include "ao_meter.h"
#include "app_knob.h"
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

// 档位名映射
static const char *knob_mode_string[16] = {
    " ",    //
    " ",    //
    " ",    //
    "hfe",  //
    " ",    //
    "ncv",  //
    "a",    //
    "ohm",  //
    " ",    //
    "ma",   //
    "ua",   //
    "mv",   //
    "hz",   //
    "dcv",  //
    "acv",  //
    " ",    //
};

// 对象
ao_knob_t ao_knob;

// 状态机
static QState ao_knob_init(ao_knob_t *const me);
static QState ao_knob_ready(ao_knob_t *const me);
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
        case AO_KNOB_READY_SIG:
            QACTIVE_POST(&ao_meter, AO_METER_READY_SIG, 0U);
            ULOG_INFO("KNOB done\r\n");
            status = Q_TRAN(&ao_knob_active);
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
        case Q_TIMEOUT_SIG:
            uint8_t knob_status_now = app_knob_ticks();
            if (knob_status_now != me->knob_status) {
                me->knob_status = knob_status_now;

                ULOG_INFO("knob change: %s, id =%d\r\n",
                          knob_mode_string[me->knob_status], me->knob_status);
                // 状态映射
                QACTIVE_POST(&ao_meter, AO_METER_MODE_SIG,
                             knob_mode_map[me->knob_status]);
                QActive_disarmX((QActive *)me, 0U);
            } else {
                QActive_armX((QActive *)me, 0U, 100U, 0U);
            }
            status = Q_HANDLED();
            break;
        case AO_KNOB_ACTIVE_SIG:
            if (Q_PAR(me) == 0U) {
                ULOG_INFO("KNOB sleep\r\n");
                QActive_disarmX((QActive *)me, 0U);
                QACTIVE_POST(&ao_meter, AO_METER_SLEEP_SIG, 0U);
            } else {
                ULOG_INFO("KNOB running\r\n");
                QActive_armX((QActive *)me, 0U, 100U, 0U);
            }
            status = Q_HANDLED();
            break;
        default:
            status = Q_SUPER(&QHsm_top);
            break;
    }
    return status;
}

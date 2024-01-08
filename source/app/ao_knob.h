#ifndef __AO_KNOB_H__
#define __AO_KNOB_H__

#include "qpn.h"

typedef enum _ao_knob_signal_t {
    AO_KNOB_READY_SIG = Q_USER_SIG,  // 初始化
    AO_KNOB_ACTIVE_SIG,

    AO_KNOB_MAX_SIG
} ao_knob_signal_t;

typedef struct _ao_knob_t {
    QActive super;
    uint8_t knob_status;
} ao_knob_t;

extern ao_knob_t ao_knob;

void ao_knob_ctor(void);

#endif

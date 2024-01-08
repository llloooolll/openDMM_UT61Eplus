#ifndef __AO_KEY_H__
#define __AO_KEY_H__

#include "qpn.h"

typedef enum _ao_key_signal_t {
    AO_KEY_READY_SIG = Q_USER_SIG,  // 初始化
    AO_KEY_ACTIVE_SIG,

    AO_KEY_MAX_SIG
} ao_key_signal_t;

typedef struct _ao_key_t {
    QActive super;
} ao_key_t;

extern ao_key_t ao_key;

void ao_key_ctor(void);

#endif

#ifndef __AO_IRDA_H__
#define __AO_IRDA_H__

#include "qpn.h"

typedef enum _ao_irda_signal_t
{
    AO_IRDA_READY_SIG = Q_USER_SIG, // 初始化
    AO_IRDA_ACTIVE_SIG,

    AO_IRDA_MAX_SIG
} ao_irda_signal_t;

typedef struct _ao_irda_t
{
    QActive super;
} ao_irda_t;

extern ao_irda_t ao_irda;

void ao_irda_ctor(void);

#endif

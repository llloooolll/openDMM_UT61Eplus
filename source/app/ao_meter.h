#ifndef __AO_METER_H__
#define __AO_METER_H__

#include "qpn.h"

typedef enum _ao_meter_signal_t
{
    AO_METER_READY_SIG = Q_USER_SIG, //

    AO_METER_MAX_SIG
} ao_meter_signal_t;

typedef struct _ao_meter_t
{
    QActive super;
} ao_meter_t;

extern ao_meter_t ao_meter;

void ao_meter_ctor(void);

#endif

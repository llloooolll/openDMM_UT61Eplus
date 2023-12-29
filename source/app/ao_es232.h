#ifndef __AO_ES232_H__
#define __AO_ES232_H__

#include "qpn.h"
#include "es232.h"

typedef enum _ao_es232_signal_t
{
    AO_ES232_ACTIVE_SIG = Q_USER_SIG, // 激活

    AO_ES232_MAX_SIG
} ao_es232_signal_t;

typedef struct _ao_es232_t
{
    QActive super;
    es232_write_cmd_t es232_write_cmd;
} ao_es232_t;

extern ao_es232_t ao_es232;

void ao_es232_ctor(void);

#endif

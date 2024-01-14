#ifndef __METER_CONFIG_H__
#define __METER_CONFIG_H__

#include "hc32l13x.h"

#define ADD_PAR(name, type) \
    struct _par_##name {    \
        char *id;           \
        type value;         \
    } par_##name

#define PAR_VALUE(cfg, name) cfg.par_##name.value

typedef struct _meter_config_t {
    ADD_PAR(sleep_time, uint32_t);
    ADD_PAR(sleep_cycle, uint32_t);
} meter_config_t;

#endif

#ifndef __APP_KNOB_H__
#define __APP_KNOB_H__

#include "hc32l13x.h"

enum {
    knob_acv_id,
    knob_dcv_id,
    knob_mv_id,
    knob_ohm_id,
    knob_hz_id,
    knob_hfe_id,
    knob_ua_id,
    knob_ma_id,
    knob_a_id,
    knob_ncv_id,
};

void app_knob_init(void);
void app_knob_deinit(void);
uint8_t app_knob_ticks(void);

#endif

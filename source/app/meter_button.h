#ifndef __METER_BUTTON_H__
#define __METER_BUTTON_H__

#include "hc32l13x.h"
#include "multi_button.h"

enum {
    button_select_id,
    button_range_id,
    button_hz_id,
    button_rel_id,
    button_peak_id,
    button_hold_id,
};

void meter_button_init(void);

#endif

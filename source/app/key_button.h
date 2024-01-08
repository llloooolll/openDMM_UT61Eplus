#ifndef __KEY_BUTTON_H__
#define __KEY_BUTTON_H__

#include "hc32l13x.h"
#include "multi_button.h"

enum
{
    button_select_id,
    button_range_id,
    button_hz_id,
    button_rel_id,
    button_peak_id,
    button_hold_id,
};

void key_init(void);

#endif

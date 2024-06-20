#ifndef __METER_RANGE_H__
#define __METER_RANGE_H__

#include "ao_meter.h"

bool meter_help_select_range(ao_meter_t *const me, int32_t value);

void meter_help_calculate_relative_value(ao_meter_t *const me);

#endif

#ifndef __METER_HELP_RANGE_H__
#define __METER_HELP_RANGE_H__

#include "ao_meter.h"

bool meter_help_range_sel(ao_meter_t *const me, int32_t value);

void calculate_rel_result(ao_meter_t *const me, int32_t *result_value,
                          int8_t *result_power);

#endif

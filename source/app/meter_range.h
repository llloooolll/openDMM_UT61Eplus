#ifndef __METER_RANGE_H__
#define __METER_RANGE_H__

#include "ao_meter.h"

bool meter_range_sel(ao_meter_t *const me, int32_t value);

void calculate_rel_result(ao_meter_t *const me);

#endif

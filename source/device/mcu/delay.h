#ifndef __DELAY_H__
#define __DELAY_H__

#include <stdint.h>
#include <stdbool.h>

void delay_init(void);
void delay_cycle(uint32_t cycle);

#endif

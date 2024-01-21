#ifndef __IRDA_H__
#define __IRDA_H__

#include "hc32l13x.h"

void irda_test(void);
void irda_send_byte(uint8_t data);
void irda_init(uint32_t baud);
bool irda_is_exist(void);
void irda_enable(bool flag);

#endif

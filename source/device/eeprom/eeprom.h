#ifndef __EEPROM_H__
#define __EEPROM_H__

#include <stdint.h>
#include <stdbool.h>

/* LCD地址 */
#define EEPROM_I2C_ADDR 0xA0

bool eeprom_init(void);
uint8_t eeprom_read_byte(uint8_t addr);
void eeprom_write_byte(uint8_t addr, uint8_t data);

#endif

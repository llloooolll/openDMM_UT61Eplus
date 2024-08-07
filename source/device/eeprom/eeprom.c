#include "eeprom.h"

#include "gpio.h"
#include "hy2613_port.h"  // 共用I2C
#include "si2c.h"

// AT24C02 256*8 byte

#define EEPROM_VALUE_OFFSET 1U

uint8_t ee_data[256];

/**
 * @brief 初始化
 *
 * @return true
 * @return false 成功
 */
bool eeprom_init(void) {
    si2c_init(&hy2613_si2c_pin);
    si2c_trans_begin(EEPROM_I2C_ADDR);  // 试探
    si2c_status_t result = si2c_trans_end();

    return !(result == si2c_status_ok);
}

/**
 * @brief 读
 *
 * @param addr
 * @return uint8_t
 */
uint8_t eeprom_read_byte(uint8_t addr) {
    si2c_init(&hy2613_si2c_pin);
    si2c_trans_begin(EEPROM_I2C_ADDR);
    si2c_write_byte(addr);
    si2c_trans_end();

    si2c_request_from(EEPROM_I2C_ADDR, 1);
    si2c_trans_end();
    return si2c_read_byte();
}

/**
 * @brief 写
 *
 * @param addr
 * @param data
 */
void eeprom_write_byte(uint8_t addr, uint8_t data) {
    si2c_init(&hy2613_si2c_pin);
    si2c_trans_begin(EEPROM_I2C_ADDR);
    si2c_write_byte(addr);
    si2c_write_byte(data);
    si2c_trans_end();
}

void eeprom_read_all(uint16_t *data) {
    uint16_t i;
    uint8_t value_h;
    uint8_t value_l;

    i = 0;
    while (i < 128) {
        value_h = eeprom_read_byte((i * 2U) + EEPROM_VALUE_OFFSET);
        value_l = eeprom_read_byte((i * 2U) + 1U + EEPROM_VALUE_OFFSET);

        data[i] = (value_h << 8) + (value_l);  // 倒端序
        i++;
    }
}

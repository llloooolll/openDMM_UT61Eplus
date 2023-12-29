#include "hy2613_port.h" // 共用I2C
#include "eeprom.h"
#include "gpio.h"
#include "si2c.h"

// AT24C02 256*8 byte

/**
 * @brief 初始化
 *
 * @return true
 * @return false 成功
 */
bool eeprom_init(void)
{
    si2c_init(&hy2613_si2c_pin);
    si2c_trans_begin(EEPROM_I2C_ADDR); // 试探
    si2c_status_t result = si2c_trans_end();

    return !((bool)(result == si2c_status_ok));
}

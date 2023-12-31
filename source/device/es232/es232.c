#include "es232.h"
#include "io_config.h"
#include "gpio.h"
#include "es232_port.h"

void es232_gpio_init(void)
{
    // 电源使能
    gpio_enable_output(ES232_POWER_EN_PORT, ES232_POWER_EN_PIN, 1);
    gpio_clear_pin(ES232_POWER_EN_PORT, ES232_POWER_EN_PIN);

    gpio_enable_output(ES232_I2C_CS_PORT, ES232_I2C_CS_PIN, 1);
    gpio_set_pin(ES232_I2C_CS_PORT, ES232_I2C_CS_PIN);

    gpio_enable_output(ES232_I2C_SCL_PORT, ES232_I2C_SCL_PIN, 1);
    gpio_enable_output(ES232_I2C_SDA_PORT, ES232_I2C_SDA_PIN, 1);
    gpio_set_pin(ES232_I2C_SCL_PORT, ES232_I2C_SCL_PIN);
    gpio_set_pin(ES232_I2C_SDA_PORT, ES232_I2C_SDA_PIN);
    gpio_enable_pullup(ES232_I2C_SCL_PORT, ES232_I2C_SCL_PIN, 1);
    gpio_enable_pullup(ES232_I2C_SDA_PORT, ES232_I2C_SDA_PIN, 1);
    gpio_enable_od(ES232_I2C_SCL_PORT, ES232_I2C_SCL_PIN, 1);
    gpio_enable_od(ES232_I2C_SDA_PORT, ES232_I2C_SDA_PIN, 1);

    gpio_enable_output(ES232_DATA_NEW_PORT, ES232_DATA_NEW_PIN, 0);
    gpio_enable_output(ES232_STBEEP_PORT, ES232_STBEEP_PIN, 0);
    gpio_enable_pullup(ES232_DATA_NEW_PORT, ES232_DATA_NEW_PIN, 1);
    gpio_enable_pullup(ES232_STBEEP_PORT, ES232_STBEEP_PIN, 1);
}

/**
 * @brief 初始化
 *
 * @return true
 * @return false 成功
 */
bool es232_init(void)
{
    si2c_init(&es232_si2c_pin);
    si2c_trans_begin(ES232_I2C_ADDR); // 试探
    si2c_status_t result = si2c_trans_end();
    return !(result == si2c_status_ok);
}

/**
 * @brief 电源使能
 *
 * @param flag
 */
void es232_enable_power(bool flag)
{
    gpio_write_pin(ES232_POWER_EN_PORT, ES232_POWER_EN_PIN, flag);
}

void es232_write(es232_write_cmd_t *es232_write_cmd)
{
    gpio_clear_pin(ES232_I2C_CS_PORT, ES232_I2C_CS_PIN);
    si2c_init(&es232_si2c_pin);
    si2c_trans_begin(ES232_I2C_ADDR);
    si2c_write_bytes((uint8_t *)&es232_write_cmd, sizeof(es232_write_cmd_t));
    si2c_trans_end();
    gpio_set_pin(ES232_I2C_CS_PORT, ES232_I2C_CS_PIN);
}

void es232_read(es232_read_cmd_t *es232_read_cmd)
{
    gpio_clear_pin(ES232_I2C_CS_PORT, ES232_I2C_CS_PIN);
    si2c_init(&es232_si2c_pin);
    si2c_request_from(ES232_I2C_ADDR, sizeof(es232_read_cmd_t));
    si2c_trans_end();
    gpio_set_pin(ES232_I2C_CS_PORT, ES232_I2C_CS_PIN);
    si2c_read_bytes((uint8_t *)es232_read_cmd, sizeof(es232_read_cmd_t));
}

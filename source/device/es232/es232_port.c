#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "binary.h"
#include "io_config.h"
#include "gpio.h"
#include "si2c.h"
#include "es232.h"

static bool es232_scl_option(bool flag)
{
    gpio_write_pin(ES232_I2C_SCL_PORT, ES232_I2C_SCL_PIN, flag);
    return gpio_read_pin(ES232_I2C_SCL_PORT, ES232_I2C_SCL_PIN);
}

static bool es232_sda_option(bool flag)
{
    gpio_write_pin(ES232_I2C_SDA_PORT, ES232_I2C_SDA_PIN, flag);
    return gpio_read_pin(ES232_I2C_SDA_PORT, ES232_I2C_SDA_PIN);
}

si2c_pin_t es232_si2c_pin = {
    .scl_option = es232_scl_option,
    .sda_option = es232_sda_option,
};

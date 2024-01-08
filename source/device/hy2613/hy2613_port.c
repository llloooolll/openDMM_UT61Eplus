#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "gpio.h"
#include "hy2613.h"
#include "io_config.h"
#include "si2c.h"

static bool hy2613_scl_option(bool flag) {
    gpio_write_pin(HY2613_I2C_SCL_PORT, HY2613_I2C_SCL_PIN, flag);
    return gpio_read_pin(HY2613_I2C_SCL_PORT, HY2613_I2C_SCL_PIN);
}

static bool hy2613_sda_option(bool flag) {
    gpio_write_pin(HY2613_I2C_SDA_PORT, HY2613_I2C_SDA_PIN, flag);
    return gpio_read_pin(HY2613_I2C_SDA_PORT, HY2613_I2C_SDA_PIN);
}

si2c_pin_t hy2613_si2c_pin = {
    .scl_option = hy2613_scl_option,
    .sda_option = hy2613_sda_option,
};

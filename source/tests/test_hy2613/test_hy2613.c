#include "gpio.h"
#include "hc32l13x.h"
#include "hy2613.h"
#include "io_config.h"
#include "si2c.h"
#include "sysctrl.h"

static void gpio_init(void) {
    sysctrl_enable_peripheral_clk(sysctrl_peripheral_clk_gpio, 1);
    sysctrl_set_rch_trim(sysctrl_rch_freq_24M);

    gpio_set_mux(HY2613_I2C_SCL_PORT, HY2613_I2C_SCL_PIN, gpio_mux_0);
    gpio_set_mux(HY2613_I2C_SDA_PORT, HY2613_I2C_SDA_PIN, gpio_mux_0);
    gpio_enable_pullup(HY2613_I2C_SCL_PORT, HY2613_I2C_SCL_PIN, 1);
    gpio_enable_pullup(HY2613_I2C_SDA_PORT, HY2613_I2C_SDA_PIN, 1);
    gpio_enable_output(HY2613_I2C_SCL_PORT, HY2613_I2C_SCL_PIN, 1);
    gpio_enable_output(HY2613_I2C_SDA_PORT, HY2613_I2C_SDA_PIN, 1);
    gpio_enable_od(HY2613_I2C_SCL_PORT, HY2613_I2C_SCL_PIN, 1);
    gpio_enable_od(HY2613_I2C_SDA_PORT, HY2613_I2C_SDA_PIN, 1);
    gpio_set_pin(HY2613_I2C_SCL_PORT, HY2613_I2C_SCL_PIN);
    gpio_set_pin(HY2613_I2C_SDA_PORT, HY2613_I2C_SDA_PIN);
}

int main(void) {
    gpio_init();
    hy2613_init();
    hy2613_test(1);

    while (1) {
    }
    return 0;
}

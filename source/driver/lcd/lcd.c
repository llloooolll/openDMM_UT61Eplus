#include "hc32l13x.h"
#include "gpio.h"
#include "hy2613.h"
#include "io_config.h"

static void lcd_gpio_init(void)
{
    gpio_set_mux(HY2613_I2C_SCL_PORT, HY2613_I2C_SCL_PIN, gpio_mux_0); // IO
    gpio_set_mux(HY2613_I2C_SDA_PORT, HY2613_I2C_SDA_PIN, gpio_mux_0); // IO
    gpio_enable_output(HY2613_I2C_SCL_PORT, HY2613_I2C_SCL_PIN, 1);
    gpio_enable_output(HY2613_I2C_SDA_PORT, HY2613_I2C_SDA_PIN, 1);
    gpio_set_pin(HY2613_I2C_SCL_PORT, HY2613_I2C_SCL_PIN);
    gpio_set_pin(HY2613_I2C_SDA_PORT, HY2613_I2C_SDA_PIN);
    gpio_enable_pullup(HY2613_I2C_SCL_PORT, HY2613_I2C_SCL_PIN, 1);
    gpio_enable_pullup(HY2613_I2C_SDA_PORT, HY2613_I2C_SDA_PIN, 1);
    gpio_enable_od(HY2613_I2C_SCL_PORT, HY2613_I2C_SCL_PIN, 1);
    gpio_enable_od(HY2613_I2C_SDA_PORT, HY2613_I2C_SDA_PIN, 1);
}

bool lcd_init(void)
{
    lcd_gpio_init();
    return hy2613_init();
}

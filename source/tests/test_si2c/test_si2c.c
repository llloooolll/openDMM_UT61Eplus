#include "hc32l13x.h"
#include "gpio.h"
#include "sysctrl.h"
#include "si2c.h"

#define EE_I2C_SDA_PORT gpio_port_b //
#define EE_I2C_SDA_PIN gpio_pin_12
#define EE_I2C_SCL_PORT gpio_port_b //
#define EE_I2C_SCL_PIN gpio_pin_13

#define LCD_I2C_ADDR 0x7C

static bool lcd_scl_option(bool flag)
{
    gpio_write_pin(EE_I2C_SCL_PORT, EE_I2C_SCL_PIN, flag);
    return gpio_read_pin(EE_I2C_SCL_PORT, EE_I2C_SCL_PIN);
}

static bool lcd_sda_option(bool flag)
{
    gpio_write_pin(EE_I2C_SDA_PORT, EE_I2C_SDA_PIN, flag);
    return gpio_read_pin(EE_I2C_SDA_PORT, EE_I2C_SDA_PIN);
}

static si2c_pin_t lcd_si2c_pin = {
    .scl_option = lcd_scl_option,
    .sda_option = lcd_sda_option,
};

int main(void)
{
    sysctrl_enable_peripheral_clk(sysctrl_peripheral_clk_gpio, 1);
    sysctrl_set_rch_trim(sysctrl_rch_freq_24M);

    gpio_set_mux(gpio_port_c, gpio_pin_14, gpio_mux_0);
    gpio_enable_output(gpio_port_c, gpio_pin_14, 1);
    gpio_set_pin(gpio_port_c, gpio_pin_14);

    gpio_set_mux(EE_I2C_SCL_PORT, EE_I2C_SCL_PIN, gpio_mux_0);
    gpio_set_mux(EE_I2C_SDA_PORT, EE_I2C_SDA_PIN, gpio_mux_0);
    gpio_set_mux(gpio_port_b, gpio_pin_00, gpio_mux_3);
    gpio_enable_pullup(EE_I2C_SCL_PORT, EE_I2C_SCL_PIN, 1);
    gpio_enable_pullup(EE_I2C_SDA_PORT, EE_I2C_SDA_PIN, 1);
    gpio_enable_output(EE_I2C_SCL_PORT, EE_I2C_SCL_PIN, 1);
    gpio_enable_output(EE_I2C_SDA_PORT, EE_I2C_SDA_PIN, 1);
    gpio_enable_od(EE_I2C_SCL_PORT, EE_I2C_SCL_PIN, 1);
    gpio_enable_od(EE_I2C_SDA_PORT, EE_I2C_SDA_PIN, 1);
    gpio_set_pin(EE_I2C_SCL_PORT, EE_I2C_SCL_PIN);
    gpio_set_pin(EE_I2C_SDA_PORT, EE_I2C_SDA_PIN);

    si2c_init(&lcd_si2c_pin);

    while (1)
    {
        si2c_trans_begin(LCD_I2C_ADDR);
        si2c_write_byte(0xff);
        si2c_write_byte(0xff);
        si2c_trans_end();
        __NOP();
    }
    return 0;
}

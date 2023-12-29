#include "hc32l13x.h"
#include "gpio.h"
#include "hy2613.h"
#include "eeprom.h"
#include "io_config.h"
#include "lcd_pixel.h"

static void lcd_gpio_init(void)
{
    gpio_enable_output(HY2613_I2C_SCL_PORT, HY2613_I2C_SCL_PIN, 1);
    gpio_enable_output(HY2613_I2C_SDA_PORT, HY2613_I2C_SDA_PIN, 1);
    gpio_enable_output(EEPROM_WP_PORT, EEPROM_WP_PIN, 1);

    gpio_set_pin(HY2613_I2C_SCL_PORT, HY2613_I2C_SCL_PIN);
    gpio_set_pin(HY2613_I2C_SDA_PORT, HY2613_I2C_SDA_PIN);
    gpio_set_pin(EEPROM_WP_PORT, EEPROM_WP_PIN);

    gpio_enable_pullup(HY2613_I2C_SCL_PORT, HY2613_I2C_SCL_PIN, 1);
    gpio_enable_pullup(HY2613_I2C_SDA_PORT, HY2613_I2C_SDA_PIN, 1);

    gpio_enable_od(HY2613_I2C_SCL_PORT, HY2613_I2C_SCL_PIN, 1);
    gpio_enable_od(HY2613_I2C_SDA_PORT, HY2613_I2C_SDA_PIN, 1);
}

/**
 * @brief 初始化
 *
 * @return true 失败
 * @return false
 */
bool lcd_init(void)
{
    lcd_gpio_init();
    return (hy2613_init() || eeprom_init());
}

/**
 * @brief 像素测试
 *
 * @param flag
 */
void lcd_test(bool flag)
{
    hy2613_test(flag);
}

/**
 * @brief 开启显示
 *
 * @param flag
 */
void lcd_enable(bool flag)
{
    if (flag)
    {
        hy2613_on();
    }
    else
    {
        hy2613_off();
    }
}

/**
 * @brief 刷新
 *
 * @param lcd_pixel
 */
void lcd_refresh(lcd_pixel_t *lcd_pixel)
{
    hy2613_refresh(lcd_pixel);
}

#include "lcd.h"
#include "io_config.h"
#include "gpio.h"
#include "hy2613.h"
#include "eeprom.h"
#include "stdlib.h"

// 积极前移小数点
#define LCD_POINT_ACTIVE 1

static const uint8_t digitron_mapping[128] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0-7
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 8-15
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 16-23
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 24-31
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 32-39
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 40-47
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, // 01234567 48-55
    0x7F, 0x6F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 89       56-63
    0x00, 0x00, 0x00, 0x00, 0x00, 0x79, 0x00, 0x00, // @ABCDEFG 64-71
    0x76, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x5C, // HIJKLMNO 72-79
    0x00, 0x00, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, // PQRSTUVW 80-87
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // XYZ      88-95
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // `abcdefg 96-103
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // hijklmno 104-111
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // pqrstuvw 112-119
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // xyz      120-127
};

static const char digitron_show_overflow[] = {'L', 'O', ' ', ' ', ' '};

static void
lcd_gpio_init(void)
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

    gpio_enable_output(LCD_BL_EN_PORT, LCD_BL_EN_PIN, 1);
    gpio_clear_pin(LCD_BL_EN_PORT, LCD_BL_EN_PIN);
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

void lcd_show_char(lcd_pixel_t *lcd_pixel, uint8_t index, char value)
{
    uint8_t temp = digitron_mapping[(uint8_t)value];
    switch (index)
    {
    case 0: //
        lcd_pixel->digitron_0_a = ((temp & 0x01) > 0) ? 1 : 0;
        lcd_pixel->digitron_0_b = ((temp & 0x02) > 0) ? 1 : 0;
        lcd_pixel->digitron_0_c = ((temp & 0x04) > 0) ? 1 : 0;
        lcd_pixel->digitron_0_d = ((temp & 0x08) > 0) ? 1 : 0;
        lcd_pixel->digitron_0_e = ((temp & 0x10) > 0) ? 1 : 0;
        lcd_pixel->digitron_0_f = ((temp & 0x20) > 0) ? 1 : 0;
        lcd_pixel->digitron_0_g = ((temp & 0x40) > 0) ? 1 : 0;
        break;
    case 1: //
        lcd_pixel->digitron_1_a = ((temp & 0x01) > 0) ? 1 : 0;
        lcd_pixel->digitron_1_b = ((temp & 0x02) > 0) ? 1 : 0;
        lcd_pixel->digitron_1_c = ((temp & 0x04) > 0) ? 1 : 0;
        lcd_pixel->digitron_1_d = ((temp & 0x08) > 0) ? 1 : 0;
        lcd_pixel->digitron_1_e = ((temp & 0x10) > 0) ? 1 : 0;
        lcd_pixel->digitron_1_f = ((temp & 0x20) > 0) ? 1 : 0;
        lcd_pixel->digitron_1_g = ((temp & 0x40) > 0) ? 1 : 0;
        break;
    case 2: //
        lcd_pixel->digitron_2_a = ((temp & 0x01) > 0) ? 1 : 0;
        lcd_pixel->digitron_2_b = ((temp & 0x02) > 0) ? 1 : 0;
        lcd_pixel->digitron_2_c = ((temp & 0x04) > 0) ? 1 : 0;
        lcd_pixel->digitron_2_d = ((temp & 0x08) > 0) ? 1 : 0;
        lcd_pixel->digitron_2_e = ((temp & 0x10) > 0) ? 1 : 0;
        lcd_pixel->digitron_2_f = ((temp & 0x20) > 0) ? 1 : 0;
        lcd_pixel->digitron_2_g = ((temp & 0x40) > 0) ? 1 : 0;
        break;
    case 3: //
        lcd_pixel->digitron_3_a = ((temp & 0x01) > 0) ? 1 : 0;
        lcd_pixel->digitron_3_b = ((temp & 0x02) > 0) ? 1 : 0;
        lcd_pixel->digitron_3_c = ((temp & 0x04) > 0) ? 1 : 0;
        lcd_pixel->digitron_3_d = ((temp & 0x08) > 0) ? 1 : 0;
        lcd_pixel->digitron_3_e = ((temp & 0x10) > 0) ? 1 : 0;
        lcd_pixel->digitron_3_f = ((temp & 0x20) > 0) ? 1 : 0;
        lcd_pixel->digitron_3_g = ((temp & 0x40) > 0) ? 1 : 0;
        break;
    case 4: //
        lcd_pixel->digitron_4_a = ((temp & 0x01) > 0) ? 1 : 0;
        lcd_pixel->digitron_4_b = ((temp & 0x02) > 0) ? 1 : 0;
        lcd_pixel->digitron_4_c = ((temp & 0x04) > 0) ? 1 : 0;
        lcd_pixel->digitron_4_d = ((temp & 0x08) > 0) ? 1 : 0;
        lcd_pixel->digitron_4_e = ((temp & 0x10) > 0) ? 1 : 0;
        lcd_pixel->digitron_4_f = ((temp & 0x20) > 0) ? 1 : 0;
        lcd_pixel->digitron_4_g = ((temp & 0x40) > 0) ? 1 : 0;
        break;
    default:
        break;
    }
}

void lcd_show_point(lcd_pixel_t *lcd_pixel, uint8_t index, bool flag)
{
    switch (index)
    {
    case 1: //
        lcd_pixel->digitron_1_dp = flag;
        break;
    case 2: //
        lcd_pixel->digitron_2_dp = flag;
        break;
    case 3: //
        lcd_pixel->digitron_3_dp = flag;
        break;
    case 4: //
        lcd_pixel->digitron_4_dp = flag;
        break;
    default:
        break;
    }
}

/**
 * @brief
 *
 * @param lcd_pixel
 * @param i32_value 显示值
 * @param i8_power_relative 值的相对幂
 */
void lcd_show_value(lcd_pixel_t *lcd_pixel, int32_t i32_value, int8_t i8_power_relative)
{
    int8_t i8_significant_number = 0;    // 有效的位数
    uint32_t u32_value = abs(i32_value); // 数字绝对值
    uint8_t u8_point_position = 0;

    lcd_pixel->minus = (bool)(i32_value < 0); // 显示正负号

    /* 缩减有效数字 */
    while (u32_value > 99999)
    {
        u32_value /= 10;
        i8_power_relative++;
    }

    /* 计算有效数字位数 */
    for (uint32_t temp = u32_value; temp > 0; i8_significant_number++)
    {
        temp /= 10;
    }

    /* 适配合适的千位分隔符 */
    int8_t i8_thousands_extern = i8_power_relative / 3; // 额外的千分隔符
    if (i8_power_relative > 0)
    {
        i8_thousands_extern++;                              // 整数继续除千
        u8_point_position = abs(3 - i8_power_relative % 3); // 小数点往前
    }
    else if (i8_power_relative < 0)
    {
        u8_point_position = abs(i8_power_relative % 3); // 小数点往前
    }

#if (LCD_POINT_ACTIVE)
    if ((i8_significant_number > 3) && (u8_point_position < 2))
    {
        i8_thousands_extern++;
        u8_point_position += 3;
    }
#endif

    /* 显示额外小数点 */
    lcd_pixel->nanon = (bool)(i8_thousands_extern == -3);   // 纳 -9
    lcd_pixel->micron = (bool)(i8_thousands_extern == -2);  // 微 -6
    lcd_pixel->milli = (bool)(i8_thousands_extern == -1);   // 毫 -3
    lcd_pixel->thousand = (bool)(i8_thousands_extern == 1); // 千 +3
    lcd_pixel->omen = (bool)(i8_thousands_extern == 2);     // 兆 +6

    if ((i8_thousands_extern > 2) || (i8_thousands_extern < -3) || (u32_value > 30000))
    {
        for (uint8_t i = 0; i < 5; i++)
        {
            lcd_show_point(lcd_pixel, i, 1);
            lcd_show_char(lcd_pixel, i, digitron_show_overflow[i]);
        }
        return;
    }

    /* 显示有效位 */
    for (uint8_t i = 0; i < 5; i++)
    {
        lcd_show_char(lcd_pixel, i, u32_value % 10 + '0');
        u32_value /= 10;
        lcd_show_point(lcd_pixel, i, (bool)(i == u8_point_position));
    }
}

void lcd_enable_bl(bool flag)
{
    gpio_write_pin(LCD_BL_EN_PORT, LCD_BL_EN_PIN, flag);
}

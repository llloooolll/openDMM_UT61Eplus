#include "lcd.h"
#include "io_config.h"
#include "gpio.h"
#include "hy2613.h"
#include "eeprom.h"
#include "stdlib.h"

static uint32_t lcd_positive_power(uint32_t u32x, uint32_t u32y);

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

static uint32_t lcd_positive_power(uint32_t u32x, uint32_t u32y)
{
    uint32_t result = 1;
    for (; u32y > 0; u32y--)
    {
        result *= u32x;
    }
    return result;
}

void lcd_show_value(lcd_pixel_t *lcd_pixel, int32_t Value, uint8_t u8Power)
{
    uint8_t u8MaxValidIndex = 0; // 最高有效位

    uint32_t u32ShowValue; // 实际显示的数字
    uint8_t u8ShowPower;   // 实际显示的幂

    lcd_pixel->minus = (bool)(Value < 0);
    u32ShowValue = abs(Value);
    u8ShowPower = u8Power;

    // 寻找最高有效位
    for (uint32_t temp = u32ShowValue; temp > 0; u8MaxValidIndex++)
    {
        temp /= 10;
    }

    if (u8Power >= u8MaxValidIndex)
    {
        // 没有整数 0.123
        if (u8Power > 4)
        { // 有效位数超屏幕，需要减位数
            u32ShowValue =
                u32ShowValue / lcd_positive_power(10, u8Power - 4);
            /*小数点后退*/
            u8ShowPower = u8Power - (u8Power - 4);
        }
        else
        {
            u32ShowValue = u32ShowValue;
            u8ShowPower = u8Power;
        }
        /* 显示五个数字 */
        for (uint8_t i = 0; i < 5; i++)
        {
            char cNum;
            if (i < u8ShowPower)
            { // 有效位
                cNum = ((u32ShowValue % lcd_positive_power(10, i + 1)) /
                        lcd_positive_power(10, i)) +
                       '0';
            }
            else if (i == u8ShowPower)
            {
                cNum = '0';
            }
            else
            {
                cNum = ' '; // 显示空
            }
            lcd_show_char(lcd_pixel, i, cNum);
            lcd_show_point(lcd_pixel, i, (i == u8ShowPower) ? true : false);
        }
    }
    else
    {
        // 有整数 1.23
        if ((u8MaxValidIndex - u8Power) > 5)
        {
            // 整数超过5位，显示不开
            lcd_show_char(lcd_pixel, 0, 'L');
            lcd_show_char(lcd_pixel, 1, 'O');
            lcd_show_char(lcd_pixel, 2, ' ');
            lcd_show_char(lcd_pixel, 3, ' ');
            lcd_show_char(lcd_pixel, 4, ' ');
            lcd_show_point(lcd_pixel, 1, true);
            lcd_show_point(lcd_pixel, 2, false);
            lcd_show_point(lcd_pixel, 3, false);
            lcd_show_point(lcd_pixel, 4, false);
        }
        else
        {
            // 可以显示
            if (u8MaxValidIndex > 5)
            { // 有效位数超屏幕，需要减位数
                u32ShowValue = u32ShowValue /
                               lcd_positive_power(10, u8MaxValidIndex - 5);
                /*小数点后退*/
                u8ShowPower = u8Power - (u8MaxValidIndex - 5);
            }
            /* 显示五个数字 */
            for (uint8_t i = 0; i < 5; i++)
            {
                char cNum;
                if (i < u8MaxValidIndex)
                { // 有效位
                    cNum = ((u32ShowValue % lcd_positive_power(10, i + 1)) /
                            lcd_positive_power(10, i)) +
                           '0';
                }
                else
                {
                    cNum = ' '; // 显示空
                }
                lcd_show_char(lcd_pixel, i, cNum);
                lcd_show_point(lcd_pixel, i,
                               (i == u8ShowPower) ? true : false);
            }
        }
    }
}
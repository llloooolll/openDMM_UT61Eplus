#include "lcd.h"

#include "eeprom.h"
#include "gpio.h"
#include "hy2613.h"
#include "io_config.h"
#include "stdlib.h"

#define LCD_POINT_ACTIVE 1  // 积极前移小数点

static const uint8_t digitron_mapping[128] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 0-7
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 8-15
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 16-23
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 24-31
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 32-39
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 40-47
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,  // 01234567 48-55
    0x7F, 0x6F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 89       56-63
    0x00, 0x00, 0x00, 0x00, 0x00, 0x79, 0x00, 0x00,  // @ABCDEFG 64-71
    0x76, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x5C,  // HIJKLMNO 72-79
    0x00, 0x00, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00,  // PQRSTUVW 80-87
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // XYZ      88-95
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // `abcdefg 96-103
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // hijklmno 104-111
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // pqrstuvw 112-119
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // xyz      120-127
};

static const char digitron_show_overflow[] = {'L', 'O', ' ', ' ', ' '};

static void lcd_gpio_init(void) {
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
bool lcd_init(void) {
    lcd_gpio_init();
    return (hy2613_init() || eeprom_init());
}

/**
 * @brief 像素测试
 *
 * @param flag
 */
void lcd_test(bool flag) {  //
    hy2613_test(flag);
}

/**
 * @brief 低功耗
 *
 */
void lcd_lpm(void) {  //
    hy2613_lpm();
}

/**
 * @brief 开启显示
 *
 * @param flag
 */
void lcd_enable(bool flag) {
    if (flag) {
        hy2613_on();
    } else {
        hy2613_off();
    }
}

/**
 * @brief 刷新
 *
 * @param lcd_pixel
 */
void lcd_refresh(lcd_pixel_t *lcd_pixel) {  //
    hy2613_refresh(lcd_pixel);
}

/**
 * @brief LCD数码管显示字符
 *
 * @param lcd_pixel
 * @param index 位置
 * @param value 字符
 */
void lcd_show_char(lcd_pixel_t *lcd_pixel, uint8_t index, char value) {
    uint8_t temp = digitron_mapping[(uint8_t)value];
    switch (index) {
        case 0:  //
            lcd_pixel->digitron_0_a = ((temp & 0x01) > 0) ? 1 : 0;
            lcd_pixel->digitron_0_b = ((temp & 0x02) > 0) ? 1 : 0;
            lcd_pixel->digitron_0_c = ((temp & 0x04) > 0) ? 1 : 0;
            lcd_pixel->digitron_0_d = ((temp & 0x08) > 0) ? 1 : 0;
            lcd_pixel->digitron_0_e = ((temp & 0x10) > 0) ? 1 : 0;
            lcd_pixel->digitron_0_f = ((temp & 0x20) > 0) ? 1 : 0;
            lcd_pixel->digitron_0_g = ((temp & 0x40) > 0) ? 1 : 0;
            break;
        case 1:  //
            lcd_pixel->digitron_1_a = ((temp & 0x01) > 0) ? 1 : 0;
            lcd_pixel->digitron_1_b = ((temp & 0x02) > 0) ? 1 : 0;
            lcd_pixel->digitron_1_c = ((temp & 0x04) > 0) ? 1 : 0;
            lcd_pixel->digitron_1_d = ((temp & 0x08) > 0) ? 1 : 0;
            lcd_pixel->digitron_1_e = ((temp & 0x10) > 0) ? 1 : 0;
            lcd_pixel->digitron_1_f = ((temp & 0x20) > 0) ? 1 : 0;
            lcd_pixel->digitron_1_g = ((temp & 0x40) > 0) ? 1 : 0;
            break;
        case 2:  //
            lcd_pixel->digitron_2_a = ((temp & 0x01) > 0) ? 1 : 0;
            lcd_pixel->digitron_2_b = ((temp & 0x02) > 0) ? 1 : 0;
            lcd_pixel->digitron_2_c = ((temp & 0x04) > 0) ? 1 : 0;
            lcd_pixel->digitron_2_d = ((temp & 0x08) > 0) ? 1 : 0;
            lcd_pixel->digitron_2_e = ((temp & 0x10) > 0) ? 1 : 0;
            lcd_pixel->digitron_2_f = ((temp & 0x20) > 0) ? 1 : 0;
            lcd_pixel->digitron_2_g = ((temp & 0x40) > 0) ? 1 : 0;
            break;
        case 3:  //
            lcd_pixel->digitron_3_a = ((temp & 0x01) > 0) ? 1 : 0;
            lcd_pixel->digitron_3_b = ((temp & 0x02) > 0) ? 1 : 0;
            lcd_pixel->digitron_3_c = ((temp & 0x04) > 0) ? 1 : 0;
            lcd_pixel->digitron_3_d = ((temp & 0x08) > 0) ? 1 : 0;
            lcd_pixel->digitron_3_e = ((temp & 0x10) > 0) ? 1 : 0;
            lcd_pixel->digitron_3_f = ((temp & 0x20) > 0) ? 1 : 0;
            lcd_pixel->digitron_3_g = ((temp & 0x40) > 0) ? 1 : 0;
            break;
        case 4:  //
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

/**
 * @brief 在数字后显示小数点
 *
 * @param lcd_pixel
 * @param index 数字的位置[1,2,3,4]
 * @param flag
 */
void lcd_show_point(lcd_pixel_t *lcd_pixel, uint8_t index, bool flag) {
    switch (index) {
        case 1:  //
            lcd_pixel->digitron_1_dp = flag;
            break;
        case 2:  //
            lcd_pixel->digitron_2_dp = flag;
            break;
        case 3:  //
            lcd_pixel->digitron_3_dp = flag;
            break;
        case 4:  //
            lcd_pixel->digitron_4_dp = flag;
            break;
        default:
            break;
    }
}

/**
 * @brief 显示数字值，值大于上限显示OL
 *
 * @param lcd_pixel
 * @param i32_value 显示值
 * @param i8_power_relative 值的相对幂
 */
void lcd_show_value(lcd_pixel_t *lcd_pixel, int32_t i32_value,
                    int8_t i8_power_relative) {
    int8_t i8_significant_number = 0;     // 有效的位数
    uint32_t u32_value = abs(i32_value);  // 数字绝对值
    uint8_t u8_point_position = 0;

    lcd_pixel->minus = (bool)(i32_value < 0);  // 显示正负号

    /* 缩减有效数字，必然小于5位 */
    while (u32_value > 99999) {
        u32_value /= 10;
        i8_power_relative++;
    }

    /* 计算有效数字位数 */
    for (uint32_t temp = u32_value; temp > 0; i8_significant_number++) {
        temp /= 10;
    }

    /* 适配合适的千位分隔符 */
    int8_t i8_thousands_extern = i8_power_relative / 3;  // 额外的千分隔符
    if (i8_power_relative > 0) {
        i8_thousands_extern++;  // 整数继续除千
        u8_point_position = abs(3 - i8_power_relative % 3);  // 小数点往前
    } else if (i8_power_relative < 0) {
        u8_point_position = abs(i8_power_relative % 3);  // 小数点往前
        if (i8_thousands_extern < -3)                    // 超出n
        {
            if (((u8_point_position + 3) < 5) &&
                ((i8_thousands_extern + 1) >= -3))  // 尝试移动小数抵消幂
            {
                i8_thousands_extern++;
                u8_point_position += 3;
            } else {
                i8_thousands_extern = -3;
                u32_value = 0;          // 归零
                u8_point_position = 0;  // 小数点无意义
            }
        }
    }

#if (LCD_POINT_ACTIVE)
    if ((i8_significant_number > 2) && ((u8_point_position + 3) < 5)) {
        i8_thousands_extern++;
        u8_point_position += 3;
    }
#endif

    /* 显示额外小数点 */
    lcd_pixel->nanon = (bool)(i8_thousands_extern == -3);    // 纳 -9
    lcd_pixel->micron = (bool)(i8_thousands_extern == -2);   // 微 -6
    lcd_pixel->milli = (bool)(i8_thousands_extern == -1);    // 毫 -3
    lcd_pixel->thousand = (bool)(i8_thousands_extern == 1);  // 千 +3
    lcd_pixel->omen = (bool)(i8_thousands_extern == 2);      // 兆 +6

    /* 显示有效位 */
    for (uint8_t i = 0; i < 5; i++) {
        lcd_show_char(lcd_pixel, i, u32_value % 10 + '0');
        u32_value /= 10;
        lcd_show_point(lcd_pixel, i, (bool)(i == u8_point_position));
    }
}

/**
 * @brief 显示OL
 *
 * @param lcd_pixel
 */
void lcd_show_ol(lcd_pixel_t *lcd_pixel) {
    for (uint8_t i = 0; i < 5; i++) {
        lcd_show_point(lcd_pixel, i, 1);
        lcd_show_char(lcd_pixel, i, digitron_show_overflow[i]);
    }
}

/**
 * @brief 使能LCD背光
 *
 * @param flag
 */
void lcd_enable_bl(bool flag) {
    gpio_write_pin(LCD_BL_EN_PORT, LCD_BL_EN_PIN, flag);
}

/**
 * @brief 显示模拟条
 *
 * @param lcd_pixel
 * @param Value 值
 */
void lcd_show_scale(lcd_pixel_t *lcd_pixel, int32_t Value) {
    lcd_pixel->s_minus = (bool)(Value < 0);
    uint32_t u32Value = abs(Value);
    /* 可显示范围 */
    while (u32Value > 22) {
        u32Value /= 10;
    }

    switch (u32Value) {
        case 44:
            lcd_pixel->s_ol = 1;
        case 43:
            lcd_pixel->s_43 = 1;
        case 42:
            lcd_pixel->s_42 = 1;
        case 41:
            lcd_pixel->s_41 = 1;
        case 40:
            lcd_pixel->s_40 = 1;
        case 39:
            lcd_pixel->s_39 = 1;
        case 38:
            lcd_pixel->s_38 = 1;
        case 37:
            lcd_pixel->s_37 = 1;
        case 36:
            lcd_pixel->s_36 = 1;
        case 35:
            lcd_pixel->s_35 = 1;
        case 34:
            lcd_pixel->s_34 = 1;
        case 33:
            lcd_pixel->s_33 = 1;
        case 32:
            lcd_pixel->s_32 = 1;
        case 31:
            lcd_pixel->s_31 = 1;
        case 30:
            lcd_pixel->s_30 = 1;
        case 29:
            lcd_pixel->s_29 = 1;
        case 28:
            lcd_pixel->s_28 = 1;
        case 27:
            lcd_pixel->s_27 = 1;
        case 26:
            lcd_pixel->s_26 = 1;
        case 25:
            lcd_pixel->s_25 = 1;
        case 24:
            lcd_pixel->s_24 = 1;
        case 23:
            lcd_pixel->s_23 = 1;
        case 22:
            lcd_pixel->s_22 = 1;
        case 21:
            lcd_pixel->s_21 = 1;
        case 20:
            lcd_pixel->s_20 = 1;
        case 19:
            lcd_pixel->s_19 = 1;
        case 18:
            lcd_pixel->s_18 = 1;
        case 17:
            lcd_pixel->s_17 = 1;
        case 16:
            lcd_pixel->s_16 = 1;
        case 15:
            lcd_pixel->s_15 = 1;
        case 14:
            lcd_pixel->s_14 = 1;
        case 13:
            lcd_pixel->s_13 = 1;
        case 12:
            lcd_pixel->s_12 = 1;
        case 11:
            lcd_pixel->s_11 = 1;
        case 10:
            lcd_pixel->s_10 = 1;
        case 9:
            lcd_pixel->s_9 = 1;
        case 8:
            lcd_pixel->s_8 = 1;
        case 7:
            lcd_pixel->s_7 = 1;
        case 6:
            lcd_pixel->s_6 = 1;
        case 5:
            lcd_pixel->s_5 = 1;
        case 4:
            lcd_pixel->s_4 = 1;
        case 3:
            lcd_pixel->s_3 = 1;
        case 2:
            lcd_pixel->s_2 = 1;
        case 1:
            lcd_pixel->s_1 = 1;
            break;
        default:
            break;
    }

    switch (u32Value) {
        case 0:
            lcd_pixel->s_1 = 0;
        case 1:
            lcd_pixel->s_2 = 0;
        case 2:
            lcd_pixel->s_3 = 0;
        case 3:
            lcd_pixel->s_4 = 0;
        case 4:
            lcd_pixel->s_5 = 0;
        case 5:
            lcd_pixel->s_6 = 0;
        case 6:
            lcd_pixel->s_7 = 0;
        case 7:
            lcd_pixel->s_8 = 0;
        case 8:
            lcd_pixel->s_9 = 0;
        case 9:
            lcd_pixel->s_10 = 0;
        case 10:
            lcd_pixel->s_11 = 0;
        case 11:
            lcd_pixel->s_12 = 0;
        case 12:
            lcd_pixel->s_13 = 0;
        case 13:
            lcd_pixel->s_14 = 0;
        case 14:
            lcd_pixel->s_15 = 0;
        case 15:
            lcd_pixel->s_16 = 0;
        case 16:
            lcd_pixel->s_17 = 0;
        case 17:
            lcd_pixel->s_18 = 0;
        case 18:
            lcd_pixel->s_19 = 0;
        case 19:
            lcd_pixel->s_20 = 0;
        case 20:
            lcd_pixel->s_21 = 0;
        case 21:
            lcd_pixel->s_22 = 0;
        case 22:
            lcd_pixel->s_23 = 0;
        case 23:
            lcd_pixel->s_24 = 0;
        case 24:
            lcd_pixel->s_25 = 0;
        case 25:
            lcd_pixel->s_26 = 0;
        case 26:
            lcd_pixel->s_27 = 0;
        case 27:
            lcd_pixel->s_28 = 0;
        case 28:
            lcd_pixel->s_29 = 0;
        case 29:
            lcd_pixel->s_30 = 0;
        case 30:
            lcd_pixel->s_31 = 0;
        case 31:
            lcd_pixel->s_32 = 0;
        case 32:
            lcd_pixel->s_33 = 0;
        case 33:
            lcd_pixel->s_34 = 0;
        case 34:
            lcd_pixel->s_35 = 0;
        case 35:
            lcd_pixel->s_36 = 0;
        case 36:
            lcd_pixel->s_37 = 0;
        case 37:
            lcd_pixel->s_38 = 0;
        case 38:
            lcd_pixel->s_39 = 0;
        case 39:
            lcd_pixel->s_40 = 0;
        case 40:
            lcd_pixel->s_41 = 0;
        case 41:
            lcd_pixel->s_42 = 0;
        case 42:
            lcd_pixel->s_43 = 0;
        case 43:
            lcd_pixel->s_ol = 0;
            break;
        default:
            break;
    }
}

#include "hy2613_port.h"
#include "hy2613.h"
#include "gpio.h"
#include "si2c.h"

// HY2613_B/SF2613B

/**
 * @brief 初始化
 *
 * @return true
 * @return false 成功
 */
bool hy2613_init(void)
{
    si2c_init(&hy2613_si2c_pin);
    si2c_trans_begin(HY2613_I2C_ADDR); // 试探
    si2c_status_t result = si2c_trans_end();

    if (result == si2c_status_ok)
    {
        si2c_trans_begin(HY2613_I2C_ADDR);
        // 软复位
        si2c_write_byte(HY2613_NEXT_CMD | HY2613_CMD_ICSET |
                        HY2613_CMD_ICSET_RST | // 复位像素数据
                        HY2613_CMD_ICSET_OSC_IN |
                        HY2613_CMD_ICSET_ADDR_MSB_L);
        si2c_trans_end();
        // 初始化
        si2c_trans_begin(HY2613_I2C_ADDR);
        si2c_write_byte(HY2613_NEXT_CMD | HY2613_CMD_MODESET |
                        HY2613_CMD_MODESET_DIS_OFF |
                        HY2613_CMD_MODESET_BIAS_1D3);
        si2c_write_byte(HY2613_NEXT_CMD | HY2613_CMD_DISCTL |
                        HY2613_CMD_DISCTL_PWR_FR_MODE_0 |
                        HY2613_CMD_DISCTL_WAVE_LINE |
                        HY2613_CMD_DISCTL_PWR_SR_MODE_N);
        si2c_write_byte(HY2613_NEXT_CMD | HY2613_CMD_ICSET |
                        HY2613_CMD_ICSET_NRST |
                        HY2613_CMD_ICSET_OSC_IN |
                        HY2613_CMD_ICSET_ADDR_MSB_L);
        si2c_write_byte(HY2613_NEXT_CMD | HY2613_CMD_BLKCTL |
                        HY2613_CMD_BLKCTL_BLK_OFF);
        si2c_write_byte(HY2613_NEXT_CMD | HY2613_CMD_APCTL |
                        HY2613_CMD_APCTL_PIXEL_N);
        si2c_write_byte(HY2613_NEXT_CMD | HY2613_CMD_EXCTL |
                        HY2613_CMD_EXCTL_BL_OFF |
                        HY2613_CMD_EXCTL_RTC_OFF |
                        HY2613_CMD_EXCTL_SR_OFF);
        si2c_trans_end();
    }
    return !((bool)(result == si2c_status_ok));
}

/**
 * @brief 关显示
 *
 * @param lcd_pixel
 */
void hy2613_off(void)
{
    si2c_init(&hy2613_si2c_pin);
    si2c_trans_begin(HY2613_I2C_ADDR);
    si2c_write_byte(HY2613_NEXT_CMD | HY2613_CMD_MODESET |
                    HY2613_CMD_MODESET_DIS_OFF |
                    HY2613_CMD_MODESET_BIAS_1D3);
    si2c_trans_end();
}

/**
 * @brief 开显示
 *
 * @param lcd_pixel
 */
void hy2613_on(void)
{
    si2c_init(&hy2613_si2c_pin);
    si2c_trans_begin(HY2613_I2C_ADDR);
    si2c_write_byte(HY2613_NEXT_CMD | HY2613_CMD_MODESET |
                    HY2613_CMD_MODESET_DIS_ON |
                    HY2613_CMD_MODESET_BIAS_1D3);
    si2c_trans_end();
}

/**
 * @brief 刷新显示内容
 *
 * @param lcd_pixel
 */
void hy2613_refresh(void *lcd_pixel)
{
    si2c_init(&hy2613_si2c_pin);
    uint8_t *lcd_temp = (uint8_t *)lcd_pixel;
    si2c_trans_begin(HY2613_I2C_ADDR);
    si2c_write_byte(HY2613_NEXT_DATA | HY2613_CMD_ADSET |
                    HY2613_CMD_ADSET_ADDR);
    si2c_write_bytes(lcd_temp, 16);
    si2c_trans_end();
}

/**
 * @brief 测试像素点
 *
 * @param flag
 */
void hy2613_test(bool flag)
{
    si2c_init(&hy2613_si2c_pin);
    si2c_trans_begin(HY2613_I2C_ADDR);
    if (flag)
    {
        si2c_write_byte(HY2613_NEXT_CMD | HY2613_CMD_MODESET |
                        HY2613_CMD_MODESET_DIS_ON |
                        HY2613_CMD_MODESET_BIAS_1D3);
        si2c_write_byte(HY2613_NEXT_CMD | HY2613_CMD_APCTL |
                        HY2613_CMD_APCTL_PIXEL_ON);
    }
    else
    {
        si2c_write_byte(HY2613_NEXT_CMD | HY2613_CMD_MODESET |
                        HY2613_CMD_MODESET_DIS_OFF |
                        HY2613_CMD_MODESET_BIAS_1D3);
        si2c_write_byte(HY2613_NEXT_CMD | HY2613_CMD_APCTL |
                        HY2613_CMD_APCTL_PIXEL_N);
    }

    si2c_trans_end();
}

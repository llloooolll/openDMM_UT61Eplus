#include "es232.h"
#include "io_config.h"
#include "gpio.h"
#include "es232_port.h"

static uint32_t es232_data_invert(uint32_t src);

void es232_gpio_init(void)
{
    // 电源使能
    gpio_enable_output(ES232_POWER_EN_PORT, ES232_POWER_EN_PIN, 1);
    gpio_clear_pin(ES232_POWER_EN_PORT, ES232_POWER_EN_PIN);

    gpio_enable_output(ES232_I2C_CS_PORT, ES232_I2C_CS_PIN, 1);
    gpio_set_pin(ES232_I2C_CS_PORT, ES232_I2C_CS_PIN);

    gpio_enable_output(ES232_I2C_SCL_PORT, ES232_I2C_SCL_PIN, 1);
    gpio_enable_output(ES232_I2C_SDA_PORT, ES232_I2C_SDA_PIN, 1);
    gpio_set_pin(ES232_I2C_SCL_PORT, ES232_I2C_SCL_PIN);
    gpio_set_pin(ES232_I2C_SDA_PORT, ES232_I2C_SDA_PIN);
    gpio_enable_pullup(ES232_I2C_SCL_PORT, ES232_I2C_SCL_PIN, 1);
    gpio_enable_pullup(ES232_I2C_SDA_PORT, ES232_I2C_SDA_PIN, 1);
    gpio_enable_od(ES232_I2C_SCL_PORT, ES232_I2C_SCL_PIN, 1);
    gpio_enable_od(ES232_I2C_SDA_PORT, ES232_I2C_SDA_PIN, 1);

    gpio_enable_output(ES232_DATA_NEW_PORT, ES232_DATA_NEW_PIN, 0);
    gpio_enable_output(ES232_STBEEP_PORT, ES232_STBEEP_PIN, 0);
    gpio_enable_pullup(ES232_DATA_NEW_PORT, ES232_DATA_NEW_PIN, 1);
    gpio_enable_pullup(ES232_STBEEP_PORT, ES232_STBEEP_PIN, 1);
}

/**
 * @brief 初始化
 *
 * @return true
 * @return false 成功
 */
bool es232_init(void)
{
    si2c_init(&es232_si2c_pin);
    si2c_trans_begin(ES232_I2C_ADDR); // 试探
    si2c_status_t result = si2c_trans_end();
    return !(result == si2c_status_ok);
}

/**
 * @brief 电源使能
 *
 * @param flag
 */
void es232_enable_power(bool flag)
{
    gpio_write_pin(ES232_POWER_EN_PORT, ES232_POWER_EN_PIN, flag);
}

/**
 * @brief 写ES232配置
 *
 * @param es232_write
 */
void es232_write(es232_write_t *es232_write)
{
    si2c_init(&es232_si2c_pin);
    si2c_trans_begin(ES232_I2C_ADDR);
    si2c_write_bytes((uint8_t *)es232_write, 4U);
    si2c_trans_end();
}

/**
 * @brief 读ES232数据
 *
 * @param es232_read
 */
void es232_read(es232_read_t *es232_read)
{
    si2c_init(&es232_si2c_pin);
    si2c_request_from(ES232_I2C_ADDR, 10U);
    si2c_trans_end();
    si2c_read_bytes((uint8_t *)es232_read, 10U);
}

/**
 * @brief 翻转ES232数据
 *
 * @param src
 * @return uint32_t
 */
static uint32_t es232_data_invert(uint32_t src)
{
    uint32_t temp = 0;
    for (uint8_t i = 0; i < 32; i++)
    {
        temp <<= 1U;
        temp |= (src & 0x00000001);
        src >>= 1U;
    }
    return temp;
}

/**
 * @brief 获取D0
 *
 * @param es232_rea_temp
 * @return uint32_t
 */
int32_t es232_get_D0(es232_read_t *es232_read_temp)
{
    uint32_t temp = 0;
    int32_t result;
    /* 拼成19位数字 */
    temp = (es232_read_temp->D0_0_2 << (24U + 5U)) |
           (es232_read_temp->D0_3_10 << (16U + 5U)) |
           (es232_read_temp->D0_11_18 << (8U + 5U));
    result = es232_data_invert(temp);
    return (es232_read_temp->ASIGN == 0) ? result : (-result);
}

int32_t es232_get_D1(es232_read_t *es232_read_temp)
{
    uint32_t temp = 0;
    int32_t result;
    /* 拼成10位数字 */
    temp = (es232_read_temp->D1_0_7 << (24U)) |
           (es232_read_temp->D1_8_9 << (16U + 6U));
    result = es232_data_invert(temp);
    return (es232_read_temp->BSIGN == 0) ? result : (-result);
}

bool es232_is_data_ready(void)
{
    return gpio_read_pin(ES232_DATA_NEW_PORT, ES232_DATA_NEW_PIN);
}

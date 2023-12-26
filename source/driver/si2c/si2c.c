#include "si2c.h"
#include "hc32l13x.h"
#include "gpio.h"
#include "delay.h"

#define SOFT_I2C_BUFFER_SIZE 16U

static si2c_status_t _i2c_status;                // 错误类型
static uint8_t _rx_buffer[SOFT_I2C_BUFFER_SIZE]; // 读缓冲区
static uint8_t _rx_buffer_put;                   // 第一个空字节
static uint8_t _rx_buffer_get;                   // 第一个非空字节
static si2c_pin_t *_si2c_pin;

static void ll_si2c_delay(void);                   // 等待
static void ll_si2c_init(void);                    // 初始化
static void ll_si2c_start(void);                   // 开始
static void ll_si2c_stop(void);                    // 结束
static bool ll_si2c_read_bit(void);                // 读位
static void ll_si2c_write_bit(bool data_bit);      // 写位
static uint8_t ll_si2c_read_byte(bool ack);        // 读字节
static bool ll_si2c_write_byte(uint8_t data_byte); // 写字节

/**
 * @brief 总线操作延时
 *
 */
static void ll_si2c_delay(void)
{
    // 延时
    delay_cycle(60);
}

/**
 * @brief 总线初始化
 *
 */
static void ll_si2c_init(void)
{
    ll_si2c_start();
    ll_si2c_stop();
}

/**
 * @brief 发开始位
 *
 */
static void ll_si2c_start(void)
{
    bool sda_status = _si2c_pin->sda_option(1);
    bool scl_status = _si2c_pin->scl_option(1);
    if (sda_status && scl_status) // 确认都是高
    {
        _si2c_pin->sda_option(0);
        ll_si2c_delay();
        _si2c_pin->scl_option(0);
        ll_si2c_delay();
    }
    else
    {
        _i2c_status = si2c_status_shortcut; // 总线电平错误
    }
}

/**
 * @brief 发停止位
 *
 */
static void ll_si2c_stop(void)
{
    _si2c_pin->scl_option(0);
    ll_si2c_delay();
    _si2c_pin->sda_option(0);
    ll_si2c_delay();
    _si2c_pin->scl_option(1);
    ll_si2c_delay();
    _si2c_pin->sda_option(1);
    ll_si2c_delay();
}

/**
 * @brief 从总线读一个位
 *
 * @return bool
 */
static bool ll_si2c_read_bit(void)
{
    _si2c_pin->scl_option(0);
    ll_si2c_delay();
    _si2c_pin->sda_option(1); // 释放SDA
    ll_si2c_delay();
    _si2c_pin->scl_option(1);
    ll_si2c_delay();
    bool rusult = _si2c_pin->sda_option(1);
    ll_si2c_delay();
    return rusult;
}
/**
 * @brief 写一个位到总线
 *
 * @param data_bit
 */
static void ll_si2c_write_bit(bool data_bit)
{
    _si2c_pin->scl_option(0);
    ll_si2c_delay();
    _si2c_pin->sda_option(data_bit);
    ll_si2c_delay();
    _si2c_pin->scl_option(1);
    ll_si2c_delay();
    ll_si2c_delay(); // 凑够四个延时周期，保证时钟周期稳定
}

/**
 * @brief 从总线读字节
 *
 * @param ack
 * @return uint8_t
 */
static uint8_t ll_si2c_read_byte(bool ack)
{
    uint8_t res = 0;

    for (uint8_t i = 0; i < 8; i++)
    {
        res <<= 1;
        res |= ll_si2c_read_bit();
    }
    ll_si2c_write_bit(!ack);
    ll_si2c_delay();

    return res;
}

/**
 * @brief 向总线写字节
 *
 * @param data_byte
 * @return bool
 */
static bool ll_si2c_write_byte(uint8_t data_byte)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        ll_si2c_write_bit((bool)(data_byte & 0x80)); // MSB
        data_byte <<= 1;
    }

    return ll_si2c_read_bit();
}

/**
 * @brief 初始化
 *
 */
void si2c_init(si2c_pin_t *si2c_pin)
{
    if (_si2c_pin != si2c_pin)
    {
        delay_init();
        _si2c_pin = si2c_pin;
        ll_si2c_init();
    }
}

/**
 * @brief 开始传输
 *
 * @param address 从机地址
 */
void si2c_trans_begin(uint8_t address)
{
    // Reset error returned by endTransmission.
    _i2c_status = si2c_status_ok;

    // check return value of the start condition.
    // It indicates if the i2c bus is okay.
    ll_si2c_start();
    if (_i2c_status == si2c_status_ok) //
    {
        bool addr_ack = ll_si2c_write_byte(address); // The r/w bit is zero for write
        if (addr_ack)                                // a sda zero from Slave for the 9th bit is ack
        {
            _i2c_status = si2c_status_addr_nack;
        }
    }
}
/**
 * @brief 停止传输
 *
 * @return si2c_status_t 错误状态
 */
si2c_status_t si2c_trans_end(void)
{
    ll_si2c_stop();
    return _i2c_status;
}

/**
 * @brief 请求读
 *
 * @param address 从机地址
 * @param size 长度，单位字节8bit
 * @return uint8_t 读取字节数
 */
uint8_t si2c_request_from(uint8_t address, uint8_t size)
{
    uint8_t n = 0; // number of valid received bytes. Start with 0 bytes.

    _i2c_status = si2c_status_ok;
    _rx_buffer_get = 0;
    _rx_buffer_put = 0;

    ll_si2c_start();

    if (!_i2c_status)
    {
        bool rc = ll_si2c_write_byte(address | 0x01); // The r/w bit is '1' to read

        if (!rc) // a sda zero from Slave for the 9th bit is ack
        {
            for (; n < size; n++)
            {
                if (n < (size - 1))
                {
                    _rx_buffer[n] = ll_si2c_read_byte(1); // read with ack
                }
                else
                {
                    _rx_buffer[n] = ll_si2c_read_byte(0); // last byte, read with nack
                }
            }
            _rx_buffer_put = n;
        }
        else
        {
            _i2c_status = si2c_status_addr_nack;
        }

        ll_si2c_stop();
    }
    return (n);
}

/**
 * @brief 写一个字节
 *
 * @param data
 */
void si2c_write_byte(uint8_t data)
{
    if (_i2c_status == si2c_status_ok)
    {
        if (ll_si2c_write_byte(data))
        {
            _i2c_status = si2c_status_data_nack;
        }
    }
}

/**
 * @brief 写入多个字节
 *
 * @param data
 * @param size
 */
void si2c_write_bytes(uint8_t *data, uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        si2c_write_byte(data[i]);
    }
}

/**
 * @brief 是否读到数据
 *
 * @return int
 */
int si2c_available(void)
{
    return (_rx_buffer_put - _rx_buffer_get);
}

/**
 * @brief 从缓冲区读数据，但不销毁
 *
 * @return uint8_t
 */
uint8_t si2c_peek(void)
{
    uint8_t data;

    if (_rx_buffer_put > _rx_buffer_get)
    {
        data = _rx_buffer[_rx_buffer_get];
    }
    else
    {
        data = -1;
    }

    return (data);
}

/**
 * @brief 从缓冲区读数据，不是从总线上
 *
 * @return int
 */
int si2c_read_byte(void)
{
    uint8_t data;

    if (_rx_buffer_put > _rx_buffer_get)
    {
        data = _rx_buffer[_rx_buffer_get];
        _rx_buffer_get++;
    }
    else
    {
        data = -1;
    }

    return (data);
}

/**
 * @brief 从缓冲区连续读
 *
 * @param buf 写入位置
 * @param size
 * @return int
 */
int HAL_SI2C_read_bytes(uint8_t *buf, uint8_t size)
{
    uint8_t data;
    uint8_t n;

    for (n = 0; n < size; n++)
    {
        data = si2c_read_byte();
        if (data == -1)
        {
            break;
        }
        else
        {
            buf[n] = (uint8_t)data;
        }
    }

    return (n);
}

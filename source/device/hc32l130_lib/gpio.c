#include "gpio.h"
#include "hc32l13x.h"
#include "binary.h"
#include "bits.h"

/**
 * @brief 设置IO复用功能
 *
 * @param port
 * @param pin
 * @param mux
 */
void gpio_mux_set(gpio_port_t port, gpio_pin_t pin, gpio_mux_t mux)
{
    for (uint8_t i = 0; (i < 16) && (pin > 0);)
    {
        *(&(M0P_GPIO->PA00_SEL) + port + (4 * i)) = mux;
        pin >>= 1;
        i++;
    }
}

/**
 * @brief 使能输出
 *
 * @param port
 * @param pin
 * @param flag
 */
void gpio_output_enable(gpio_port_t port, gpio_pin_t pin, bool flag)
{
    if (!flag)
    {
        BITS_SET(*(&(M0P_GPIO->PADIR) + port), pin);
    }
    else
    {
        BITS_CLR(*(&(M0P_GPIO->PADIR) + port), pin);
    }
}

/**
 * @brief 使能模拟功能
 *
 * @param port
 * @param pin
 * @param flag
 */
void gpio_analog_enable(gpio_port_t port, gpio_pin_t pin, bool flag)
{
    if (flag)
    {
        BITS_SET(*(&(M0P_GPIO->PAADS) + port), pin);
    }
    else
    {
        BITS_CLR(*(&(M0P_GPIO->PAADS) + port), pin);
    }
}

/**
 * @brief 使能上拉
 *
 * @param port
 * @param pin
 * @param flag
 */
void gpio_pullup_enable(gpio_port_t port, gpio_pin_t pin, bool flag)
{
    if (flag)
    {
        BITS_SET(*(&(M0P_GPIO->PAPU) + port), pin);
    }
    else
    {
        BITS_CLR(*(&(M0P_GPIO->PAPU) + port), pin);
    }
}

/**
 * @brief 使能下拉
 *
 * @param port
 * @param pin
 * @param flag
 */
void gpio_pulldown_enable(gpio_port_t port, gpio_pin_t pin, bool flag)
{
    if (flag)
    {
        BITS_SET(*(&(M0P_GPIO->PAPD) + port), pin);
    }
    else
    {
        BITS_CLR(*(&(M0P_GPIO->PAPD) + port), pin);
    }
}

/**
 * @brief 使能开漏
 *
 * @param port
 * @param pin
 * @param flag
 */
void gpio_od_enable(gpio_port_t port, gpio_pin_t pin, bool flag)
{
    if (flag)
    {
        BITS_SET(*(&(M0P_GPIO->PAOD) + port), pin);
    }
    else
    {
        BITS_CLR(*(&(M0P_GPIO->PAOD) + port), pin);
    }
}

/**
 * @brief 输出
 *
 * @param port
 * @param pin
 * @param flag
 */
void gpio_output_write(gpio_port_t port, gpio_pin_t pin, bool flag)
{
    if (flag)
    {
        BITS_SET(*(&(M0P_GPIO->PAOD) + port), pin);
    }
    else
    {
        BITS_CLR(*(&(M0P_GPIO->PAOD) + port), pin);
    }
}

/**
 * @brief 输出置位
 *
 * @param port
 * @param pin
 */
void gpio_output_set(gpio_port_t port, gpio_pin_t pin)
{
    BITS_SET(*(&(M0P_GPIO->PABSET) + port), pin);
}

/**
 * @brief 输出复位
 *
 * @param port
 * @param pin
 */
void gpio_output_clear(gpio_port_t port, gpio_pin_t pin)
{
    BITS_CLR(*(&(M0P_GPIO->PABCLR) + port), pin);
}

/**
 * @brief 输入
 *
 * @param port
 * @param pin
 */
bool gpio_input_read(gpio_port_t port, gpio_pin_t pin)
{
    return BIT_GET(*(&(M0P_GPIO->PAIN) + port), pin) > 0;
}

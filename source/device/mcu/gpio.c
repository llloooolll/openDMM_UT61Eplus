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
void gpio_set_mux(gpio_port_t port, gpio_pin_t pin, gpio_mux_t mux)
{
    for (uint8_t i = 0; i < 16; i++)
    {
        if (pin & 0x01)
        {
            REG_OFFSET(M0P_GPIO->PA00_SEL, port + (4 * i)) = mux;
        }
        pin >>= 1;
    }
}

/**
 * @brief 使能增强驱动能力输出
 *
 * @param port
 * @param pin
 * @param flag
 */
void gpio_enable_powerful(gpio_port_t port, gpio_pin_t pin, bool flag)
{
    if (flag)
    {
        CLR_BITS(REG_OFFSET(M0P_GPIO->PADR, port), pin);
    }
    else
    {
        SET_BITS(REG_OFFSET(M0P_GPIO->PADR, port), pin);
    }
}

/**
 * @brief 使能输出
 *
 * @param port
 * @param pin
 * @param flag
 */
void gpio_enable_output(gpio_port_t port, gpio_pin_t pin, bool flag)
{
    if (flag)
    {
        CLR_BITS(REG_OFFSET(M0P_GPIO->PADIR, port), pin);
    }
    else
    {
        SET_BITS(REG_OFFSET(M0P_GPIO->PADIR, port), pin);
    }
}

/**
 * @brief 使能模拟功能
 *
 * @param port
 * @param pin
 * @param flag
 */
void gpio_enable_analog(gpio_port_t port, gpio_pin_t pin, bool flag)
{
    if (flag)
    {
        SET_BITS(REG_OFFSET(M0P_GPIO->PAADS, port), pin);
    }
    else
    {
        CLR_BITS(REG_OFFSET(M0P_GPIO->PAADS, port), pin);
    }
}

/**
 * @brief 使能上拉
 *
 * @param port
 * @param pin
 * @param flag
 */
void gpio_enable_pullup(gpio_port_t port, gpio_pin_t pin, bool flag)
{
    if (flag)
    {
        SET_BITS(REG_OFFSET(M0P_GPIO->PAPU, port), pin);
    }
    else
    {
        CLR_BITS(REG_OFFSET(M0P_GPIO->PAPU, port), pin);
    }
}

/**
 * @brief 使能下拉
 *
 * @param port
 * @param pin
 * @param flag
 */
void gpio_enable_pulldown(gpio_port_t port, gpio_pin_t pin, bool flag)
{
    if (flag)
    {
        SET_BITS(REG_OFFSET(M0P_GPIO->PAPD, port), pin);
    }
    else
    {
        CLR_BITS(REG_OFFSET(M0P_GPIO->PAPD, port), pin);
    }
}

/**
 * @brief 使能开漏
 *
 * @param port
 * @param pin
 * @param flag
 */
void gpio_enable_od(gpio_port_t port, gpio_pin_t pin, bool flag)
{
    if (flag)
    {
        SET_BITS(REG_OFFSET(M0P_GPIO->PAOD, port), pin);
    }
    else
    {
        CLR_BITS(REG_OFFSET(M0P_GPIO->PAOD, port), pin);
    }
}

/**
 * @brief 输出
 *
 * @param port
 * @param pin
 * @param flag
 */
void gpio_write_pin(gpio_port_t port, gpio_pin_t pin, bool flag)
{
    if (flag)
    {
        REG_OFFSET(M0P_GPIO->PABSET, port) = pin;
    }
    else
    {
        REG_OFFSET(M0P_GPIO->PABCLR, port) = pin;
    }
}

/**
 * @brief 输出置位
 *
 * @param port
 * @param pin
 */
void gpio_set_pin(gpio_port_t port, gpio_pin_t pin)
{
    REG_OFFSET(M0P_GPIO->PABSET, port) = pin;
}

/**
 * @brief 输出复位
 *
 * @param port
 * @param pin
 */
void gpio_clear_pin(gpio_port_t port, gpio_pin_t pin)
{
    REG_OFFSET(M0P_GPIO->PABCLR, port) = pin;
}

/**
 * @brief 输入
 *
 * @param port
 * @param pin
 */
bool gpio_read_pin(gpio_port_t port, gpio_pin_t pin)
{
    return ((REG_OFFSET(M0P_GPIO->PAIN, port) & pin) > 0);
}

#include "irda.h"
#include "gpio.h"
#include "uart.h"
#include "sysctrl.h"
#include "io_config.h"

#define IRDA_UART M0P_UART1

static void irda_gpio_init(void)
{
    gpio_set_mux(IRDA_UART_TXD_PORT, IRDA_UART_TXD_PIN, gpio_mux_1);
    gpio_set_mux(IRDA_UART_RXD_PORT, IRDA_UART_RXD_PIN, gpio_mux_1);

    gpio_enable_output(IRDA_HALL_PWR_PORT, IRDA_HALL_PWR_PIN, 1);
    gpio_enable_output(IRDA_HALL_INT_PORT, IRDA_HALL_INT_PIN, 0);
    gpio_enable_output(IRDA_UART_TXD_PORT, IRDA_UART_TXD_PIN, 1);
    gpio_enable_output(IRDA_UART_RXD_PORT, IRDA_UART_RXD_PIN, 0);
    gpio_enable_output(IRDA_POWER_EN_PORT, IRDA_POWER_EN_PIN, 1);

    gpio_set_pin(IRDA_HALL_PWR_PORT, IRDA_HALL_PWR_PIN); // 霍尔电源
    gpio_set_pin(IRDA_POWER_EN_PORT, IRDA_HALL_PWR_PIN); // 红外电源

    gpio_enable_pullup(IRDA_HALL_INT_PORT, IRDA_HALL_INT_PIN, 1);
}

void irda_init(uint32_t baud)
{
    sysctrl_enable_peripheral_clk(sysctrl_peripheral_clk_uart1, 1);
    irda_gpio_init();
    uart_set_mode(IRDA_UART, uart_mode_1);
    uart_set_baud(IRDA_UART, baud);
    uart_enable_func(IRDA_UART, uart_func_ren, 1);
}

void irda_send_byte(uint8_t data)
{
    uart_send_data_poll(IRDA_UART, data);
}

void irda_test(void)
{
    if (uart_get_status(IRDA_UART, uart_status_rc))
    {
        uint8_t uart_data = uart_receive_data(IRDA_UART);
        uart_clr_status(IRDA_UART, uart_status_rc);
        uart_send_data_poll(IRDA_UART, uart_data);
    }
}
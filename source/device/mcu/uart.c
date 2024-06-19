#include "uart.h"

#include "bits.h"
#include "sysctrl.h"

bool uart_get_status(M0P_UART_TypeDef *UARTx, uart_status_t status) {
    return GET_BIT(UARTx->ISR, status);
}

void uart_clr_status(M0P_UART_TypeDef *UARTx, uart_status_t status) { CLR_BIT(UARTx->ICR, status); }

void uart_send_data_poll(M0P_UART_TypeDef *UARTx, uint8_t data) {
    while (false == uart_get_status(UARTx, uart_status_txe)) {
    }
    UARTx->SBUF_f.DATA = data;
    while (false == uart_get_status(UARTx, uart_status_tc)) {
    }
    uart_clr_status(UARTx, uart_status_tc);
}

void uart_send_data_it(M0P_UART_TypeDef *UARTx, uint8_t data) { UARTx->SBUF_f.DATA = data; }

uint8_t uart_receive_data(M0P_UART_TypeDef *UARTx) { return (UARTx->SBUF_f.DATA); }

void uart_enable_irq(M0P_UART_TypeDef *UARTx, uart_irq_t uart_irq, bool flag) {
    if (flag) {
        SET_BIT(UARTx->SCON, uart_irq);
    } else {
        CLR_BIT(UARTx->SCON, uart_irq);
    }
}

void uart_enable_func(M0P_UART_TypeDef *UARTx, uart_func_t uart_func, bool flag) {
    if (flag) {
        SET_BIT(UARTx->SCON, uart_func);
    } else {
        CLR_BIT(UARTx->SCON, uart_func);
    }
}

/**
 * @brief 设置工作模式
 *
 * @param UARTx
 * @param mode
 */
void uart_set_mode(M0P_UART_TypeDef *UARTx, uart_mode_t mode) {
    CLR_BITS(UARTx->SCON, uart_mode_3);
    SET_BITS(UARTx->SCON, mode);
}

/**
 * @brief 设置分频
 *
 * @param UARTx
 * @param over mode1/3 0:16;1:8, mode2 0:32;1:16
 */
void uart_set_over(M0P_UART_TypeDef *UARTx, bool over) { UARTx->SCON_f.OVER = over; }

void uart_set_baud(M0P_UART_TypeDef *UARTx, uint32_t baud) {
    float pclk_freq = sysctrl_get_pclk_freq();
    uint32_t freq_div = (UARTx->SCON_f.OVER) ? 8U : 16U;

    UARTx->SCNT_f.SCNT = (uint32_t)(pclk_freq / baud / freq_div);
}

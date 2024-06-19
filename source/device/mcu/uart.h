#ifndef __UART_H__
#define __UART_H__

#include "hc32l13x.h"

typedef enum _uart_status_t {
    uart_status_rc = 0U,     // 接收数据完成标记
    uart_status_tc = 1U,     // 发送数据完成标记
    uart_status_fe = 2U,     // 帧错误标记
    uart_status_txe = 3U,    // TXbuff空标记
    uart_status_pe = 4U,     // 奇偶校验错误标记
    uart_status_ctsif = 5U,  // CTS中断标记
    uart_status_cts = 6U,    // CTS信号标记
} uart_status_t;

typedef enum _uart_irq_t {
    uart_irq_rx = 0u,    // 接收中断使能
    uart_irq_tx = 1u,    // 发送中断使能
    uart_irq_txe = 8u,   // TX空中断使能
    uart_irq_pe = 13u,   // 奇偶校验中断使能
    uart_irq_cts = 20u,  // CTS信号翻转中断使能
    uart_irq_fe = 21u,   // 帧错误中断使能
} uart_irq_t;

typedef enum _uart_func_t {
    uart_func_ren = 4u,     // 0-TX; ///<1-非mode0模式代表RX&TX ,mode0模式代表RX;
    uart_func_dmarx = 16u,  // DMA接收功能
    uart_func_dmatx = 17u,  // DMA发送功能
    uart_func_rts = 18u,    // 硬件流RTS功能
    uart_func_cts = 19u,    // 硬件流CTS功能
    uart_func_hd = 22u,     // 单线半双工功能
} uart_func_t;

typedef enum _uart_mode_t {
    uart_mode_0 = 0x00u,  // PCLK/12
    uart_mode_1 = 0x40u,  // PCLK/OVER/SCNT
    uart_mode_2 = 0x80u,  // PCLK/OVER
    uart_mode_3 = 0xc0u,  // PCLK/OVER/SCNT 9BIT
} uart_mode_t;

bool uart_get_status(M0P_UART_TypeDef *UARTx, uart_status_t status);
void uart_clr_status(M0P_UART_TypeDef *UARTx, uart_status_t status);
void uart_send_data_poll(M0P_UART_TypeDef *UARTx, uint8_t data);
void uart_send_data_it(M0P_UART_TypeDef *UARTx, uint8_t data);
uint8_t uart_receive_data(M0P_UART_TypeDef *UARTx);
void uart_enable_irq(M0P_UART_TypeDef *UARTx, uart_irq_t uart_irq, bool flag);
void uart_enable_func(M0P_UART_TypeDef *UARTx, uart_func_t uart_func, bool flag);
void uart_set_mode(M0P_UART_TypeDef *UARTx, uart_mode_t mode);
void uart_set_over(M0P_UART_TypeDef *UARTx, bool over);
void uart_set_baud(M0P_UART_TypeDef *UARTx, uint32_t baud);

#endif

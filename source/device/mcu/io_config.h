#ifndef __CONFIG_H__
#define __CONFIG_H__

#define HY2613_I2C_SDA_PORT gpio_port_b // SDA
#define HY2613_I2C_SDA_PIN gpio_pin_12
#define HY2613_I2C_SCL_PORT gpio_port_b // SCL
#define HY2613_I2C_SCL_PIN gpio_pin_13

#define IRDA_HALL_PWR_PORT gpio_port_a // 霍尔电源
#define IRDA_HALL_PWR_PIN gpio_pin_14
#define IRDA_HALL_INT_PORT gpio_port_c // 红外插入检测霍尔
#define IRDA_HALL_INT_PIN gpio_pin_13
#define IRDA_UART_TXD_PORT gpio_port_a // 串口发射
#define IRDA_UART_TXD_PIN gpio_pin_02
#define IRDA_UART_RXD_PORT gpio_port_a // 串口接收
#define IRDA_UART_RXD_PIN gpio_pin_03
#define IRDA_POWER_EN_PORT gpio_port_a // 红外电源
#define IRDA_POWER_EN_PIN gpio_pin_06

#endif

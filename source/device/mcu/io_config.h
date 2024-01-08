#ifndef __CONFIG_H__
#define __CONFIG_H__

#define HY2613_I2C_SDA_PORT gpio_port_b // SDA
#define HY2613_I2C_SDA_PIN gpio_pin_12
#define HY2613_I2C_SCL_PORT gpio_port_b // SCL
#define HY2613_I2C_SCL_PIN gpio_pin_13

#define LCD_BL_EN_PORT gpio_port_a // 背光
#define LCD_BL_EN_PIN gpio_pin_07

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

#define LED_RED_ENB_PORT gpio_port_a // 红色LED驱动
#define LED_RED_ENB_PIN gpio_pin_04
#define LED_GREEN_ENB_PORT gpio_port_a // 绿色LED驱动
#define LED_GREEN_ENB_PIN gpio_pin_05

#define EEPROM_WP_PORT gpio_port_a //
#define EEPROM_WP_PIN gpio_pin_09

#define ES232_POWER_EN_PORT gpio_port_c // ES232正负电源开关
#define ES232_POWER_EN_PIN gpio_pin_14
#define ES232_I2C_SDA_PORT gpio_port_b // I2C数据信号
#define ES232_I2C_SDA_PIN gpio_pin_06
#define ES232_I2C_SCL_PORT gpio_port_b // I2C时钟信号
#define ES232_I2C_SCL_PIN gpio_pin_05
#define ES232_I2C_CS_PORT gpio_port_b // I2C片选信号
#define ES232_I2C_CS_PIN gpio_pin_03
#define ES232_DATA_NEW_PORT gpio_port_b // ES232数据准备信号
#define ES232_DATA_NEW_PIN gpio_pin_04
#define ES232_STBEEP_PORT gpio_port_a // ES232低电阻信号输出
#define ES232_STBEEP_PIN gpio_pin_15

#define KNOB_CH1_PORT gpio_port_c // 旋钮档位检测
#define KNOB_CH1_PIN gpio_pin_15
#define KNOB_CH2_PORT gpio_port_a // 旋钮档位检测
#define KNOB_CH2_PIN gpio_pin_00
#define KNOB_CH3_PORT gpio_port_a // 旋钮档位检测
#define KNOB_CH3_PIN gpio_pin_01
#define KNOB_CH4_PORT gpio_port_d // 旋钮档位检测
#define KNOB_CH4_PIN gpio_pin_00

#define KEY_SELECT_PORT gpio_port_d // SELECT
#define KEY_SELECT_PIN gpio_pin_06
#define KEY_RANGE_PORT gpio_port_b // RANGE
#define KEY_RANGE_PIN gpio_pin_15
#define KEY_HZ_PORT gpio_port_a // HZ/USB
#define KEY_HZ_PIN gpio_pin_12
#define KEY_REL_PORT gpio_port_a // REL
#define KEY_REL_PIN gpio_pin_11
#define KEY_PEAK_PORT gpio_port_a // PEAK(MAX/MIN)
#define KEY_PEAK_PIN gpio_pin_08
#define KEY_HOLD_PORT gpio_port_d // HOLD
#define KEY_HOLD_PIN gpio_pin_07

#endif

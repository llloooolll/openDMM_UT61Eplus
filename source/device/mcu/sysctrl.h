#ifndef __SYSCTRL_H__
#define __SYSCTRL_H__

#include "hc32l13x.h"

#define SYSTEM_XTH 8000000UL
#define SYSTEM_XTL 32768UL

typedef enum _sysctrl_clk_t {
    sysctrl_clk_rch = 0U,
    sysctrl_clk_xth = 1U,
    sysctrl_clk_rcl = 2U,
    sysctrl_clk_xtl = 3U,
    sysctrl_clk_pll = 4U,
} sysctrl_clk_t;

typedef enum _sysctrl_rcl_cycle_t {
    sysctrl_rcl_stable_cycle_4 = 0U,    // 4 个周期数
    sysctrl_rcl_stable_cycle_16 = 1U,   // 16 个周期数
    sysctrl_rcl_stable_cycle_64 = 2U,   // 64 个周期数
    sysctrl_rcl_stable_cycle_256 = 3U,  // 256 个周期数
} sysctrl_rcl_cycle_t;

typedef enum _sysctrl_clk_source_t {
    sysctrl_clk_source_rch = 0U,
    sysctrl_clk_source_xth = 1U,
    sysctrl_clk_source_rcl = 2U,
    sysctrl_clk_source_xtl = 3U,
    sysctrl_clk_source_pll = 4U,
} sysctrl_clk_source_t;

typedef enum _sysctrl_rch_freq_t {
    sysctrl_rch_freq_24M = 0U,
    sysctrl_rch_freq_22_12M = 2U,
    sysctrl_rch_freq_16M = 4U,
    sysctrl_rch_freq_8M = 6U,
    sysctrl_rch_freq_4M = 8U,
} sysctrl_rch_freq_t;

typedef enum _sysctrl_rcl_freq_t {
    sysctrl_rcl_freq_38_4K = 0U,
    sysctrl_rcl_freq_32_768K = 2U,
} sysctrl_rcl_freq_t;

typedef enum _sysctrl_hclk_div_t {
    sysctrl_hclk_div_1 = 0U,
    sysctrl_hclk_div_2 = 1U,
    sysctrl_hclk_div_4 = 2U,
    sysctrl_hclk_div_8 = 3U,
    sysctrl_hclk_div_16 = 4U,
    sysctrl_hclk_div_32 = 5U,
    sysctrl_hclk_div_64 = 6U,
    sysctrl_hclk_div_128 = 7U,
} sysctrl_hclk_div_t;

typedef enum _sysctrl_pclk_div_t {
    sysctrl_pclk_div_1 = 0U,
    sysctrl_pclk_div_2 = 1U,
    sysctrl_pclk_div_4 = 2U,
    sysctrl_pclk_div_8 = 3U,
} sysctrl_pclk_div_t;

typedef enum _sysctrl_peripheral_clk_t {
    sysctrl_peripheral_clk_uart0 = 0u,     ///< 串口0
    sysctrl_peripheral_clk_uart1 = 1u,     ///< 串口1
    sysctrl_peripheral_clk_lpUart0 = 2u,   ///< 低功耗串口0
    sysctrl_peripheral_clk_lpUart1 = 3u,   ///< 低功耗串口1
    sysctrl_peripheral_clk_i2c0 = 4u,      ///< I2C0
    sysctrl_peripheral_clk_i2c1 = 5u,      ///< I2C1
    sysctrl_peripheral_clk_spi0 = 6u,      ///< SPI0
    sysctrl_peripheral_clk_spi1 = 7u,      ///< SPI1
    sysctrl_peripheral_clk_base_tim = 8u,  ///< 基础定时器TIM0/1/2
    sysctrl_peripheral_clk_lpTim = 9u,     ///< 低功耗定时器
    sysctrl_peripheral_clk_adv_Tim = 10u,  ///< 高级定时器TIM4/5/6
    sysctrl_peripheral_clk_tim3 = 11u,     ///< 定时器3
    sysctrl_peripheral_clk_opa = 13u,      ///< OPA
    sysctrl_peripheral_clk_pca = 14u,      ///< 可编程计数阵列
    sysctrl_peripheral_clk_wdt = 15u,      ///< 看门狗
    sysctrl_peripheral_clk_adc_bgr = 16u,  ///< ADC&BGR
    sysctrl_peripheral_clk_vc_lvd = 17u,   ///< VC和LVD
    sysctrl_peripheral_clk_rng = 18u,      ///< RNG
    sysctrl_peripheral_clk_pcnt = 19u,     ///< PCNT
    sysctrl_peripheral_clk_rtc = 20u,      ///< RTC
    sysctrl_peripheral_clk_trim = 21u,     ///< 时钟校准
    sysctrl_peripheral_clk_lcd = 22u,      ///< LCD
    sysctrl_peripheral_clk_tick = 24u,     ///< 系统定时器
    sysctrl_peripheral_clk_swd = 25u,      ///< SWD
    sysctrl_peripheral_clk_crc = 26u,      ///< CRC
    sysctrl_peripheral_clk_aes = 27u,      ///< AES
    sysctrl_peripheral_clk_gpio = 28u,     ///< GPIO
    sysctrl_peripheral_clk_dma = 29u,      ///< DMA
    sysctrl_peripheral_clk_div = 30u,      ///< 除法器
    sysctrl_peripheral_clk_flash = 31u,    ///< Flash
} sysctrl_peripheral_clk_t;

#define RCH_TRIM_24M (*((volatile uint16_t *)(0x00100C00UL)))
#define RCH_TRIM_22_12M (*((volatile uint16_t *)(0x00100C02UL)))
#define RCH_TRIM_16M (*((volatile uint16_t *)(0x00100C04UL)))
#define RCH_TRIM_8M (*((volatile uint16_t *)(0x00100C06UL)))
#define RCH_TRIM_4M (*((volatile uint16_t *)(0x00100C08UL)))

#define RCL_TRIM_38_4K (*((volatile uint16_t *)(0x00100C20UL)))
#define RCL_TRIM_32_768K (*((volatile uint16_t *)(0x00100C21UL)))

void sysctrl_enable_clock(sysctrl_clk_t clk, bool flag);
void sysctrl_set_rcl_stable_time(sysctrl_rcl_cycle_t cycle);
void sysctrl_switch_sys_clk_source(sysctrl_clk_source_t source);
uint32_t sysctrl_get_hclk_freq(void);
uint32_t sysctrl_get_pclk_freq(void);
void sysctrl_set_rch_trim(sysctrl_rch_freq_t freq);
void sysctrl_set_rcl_trim(sysctrl_rcl_freq_t freq);
void sysctrl_set_hclk_div(sysctrl_hclk_div_t div);
void sysctrl_set_pclk_div(sysctrl_pclk_div_t div);
void sysctrl_enable_peripheral_clk(sysctrl_peripheral_clk_t peripheral,
                                   bool flag);

#endif

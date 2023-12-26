#ifndef __GPIO_H__
#define __GPIO_H__

#include <stdint.h>
#include <stdbool.h>

typedef enum _gpio_port_t
{
    gpio_port_a = 0x00UL,
    gpio_port_b = 0x40UL,
    gpio_port_c = 0x80UL,
    gpio_port_d = 0xC0UL,
} gpio_port_t;

typedef enum _gpio_pin_t
{
    gpio_pin_00 = 1UL << 0,
    gpio_pin_01 = 1UL << 1,
    gpio_pin_02 = 1UL << 2,
    gpio_pin_03 = 1UL << 3,
    gpio_pin_04 = 1UL << 4,
    gpio_pin_05 = 1UL << 5,
    gpio_pin_06 = 1UL << 6,
    gpio_pin_07 = 1UL << 7,
    gpio_pin_08 = 1UL << 8,
    gpio_pin_09 = 1UL << 9,
    gpio_pin_10 = 1UL << 10,
    gpio_pin_11 = 1UL << 11,
    gpio_pin_12 = 1UL << 12,
    gpio_pin_13 = 1UL << 13,
    gpio_pin_14 = 1UL << 14,
    gpio_pin_15 = 1UL << 15,
} gpio_pin_t;

typedef enum _gpio_mux_t
{
    gpio_mux_0 = 0U,
    gpio_mux_1 = 1U,
    gpio_mux_2 = 2U,
    gpio_mux_3 = 3U,
    gpio_mux_4 = 4U,
    gpio_mux_5 = 5U,
    gpio_mux_6 = 6U,
    gpio_mux_7 = 7U,
} gpio_mux_t;

void gpio_set_mux(gpio_port_t port, gpio_pin_t pin, gpio_mux_t mux);
void gpio_enable_output(gpio_port_t port, gpio_pin_t pin, bool flag);
void gpio_enable_powerful(gpio_port_t port, gpio_pin_t pin, bool flag);
void gpio_enable_analog(gpio_port_t port, gpio_pin_t pin, bool flag);
void gpio_enable_pullup(gpio_port_t port, gpio_pin_t pin, bool flag);
void gpio_enable_pulldown(gpio_port_t port, gpio_pin_t pin, bool flag);
void gpio_enable_od(gpio_port_t port, gpio_pin_t pin, bool flag);
void gpio_write_pin(gpio_port_t port, gpio_pin_t pin, bool flag);
void gpio_set_pin(gpio_port_t port, gpio_pin_t pin);
void gpio_clear_pin(gpio_port_t port, gpio_pin_t pin);
bool gpio_read_pin(gpio_port_t port, gpio_pin_t pin);

#endif

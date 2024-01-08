#include "gpio.h"
#include "hc32l13x.h"
#include "sysctrl.h"

int main(void) {
    gpio_set_mux(gpio_port_a, gpio_pin_00, gpio_mux_0);
    gpio_enable_output(gpio_port_a, gpio_pin_00, 1);
    gpio_set_pin(gpio_port_a, gpio_pin_00);
    gpio_clear_pin(gpio_port_a, gpio_pin_00);

    while (1) {
    }
    return 0;
}

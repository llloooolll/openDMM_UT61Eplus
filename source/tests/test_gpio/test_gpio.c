#include "hc32l13x.h"
#include "gpio.h"
#include "sysctrl.h"

int main(void)
{
    gpio_mux_set(gpio_port_a, gpio_pin_00, gpio_mux_0);
    gpio_output_enable(gpio_port_a, gpio_pin_00, 1);
    gpio_output_set(gpio_port_a, gpio_pin_00);
    gpio_output_clear(gpio_port_a, gpio_pin_00);

    while (1)
    {
    }
    return 0;
}

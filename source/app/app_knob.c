#include "app_knob.h"

#include "ao_knob.h"
#include "gpio.h"
#include "io_config.h"
#include "ulog.h"

static void knob_gpio_init(void) {
    gpio_enable_output(KNOB_CH1_PORT, KNOB_CH1_PIN, 0);
    gpio_enable_output(KNOB_CH2_PORT, KNOB_CH2_PIN, 0);
    gpio_enable_output(KNOB_CH3_PORT, KNOB_CH3_PIN, 0);
    gpio_enable_output(KNOB_CH4_PORT, KNOB_CH4_PIN, 0);

    gpio_enable_pullup(KNOB_CH1_PORT, KNOB_CH1_PIN, 1);
    gpio_enable_pullup(KNOB_CH2_PORT, KNOB_CH2_PIN, 1);
    gpio_enable_pullup(KNOB_CH3_PORT, KNOB_CH3_PIN, 1);
    gpio_enable_pullup(KNOB_CH4_PORT, KNOB_CH4_PIN, 1);
}

static void knob_gpio_deinit(void) {
    if (gpio_read_pin(KNOB_CH1_PORT, KNOB_CH1_PIN) == 0) {
        gpio_enable_pullup(KNOB_CH1_PORT, KNOB_CH1_PIN, 0);
    }
    if (gpio_read_pin(KNOB_CH2_PORT, KNOB_CH2_PIN) == 0) {
        gpio_enable_pullup(KNOB_CH2_PORT, KNOB_CH2_PIN, 0);
    }
    if (gpio_read_pin(KNOB_CH3_PORT, KNOB_CH3_PIN) == 0) {
        gpio_enable_pullup(KNOB_CH3_PORT, KNOB_CH3_PIN, 0);
    }
    if (gpio_read_pin(KNOB_CH4_PORT, KNOB_CH4_PIN) == 0) {
        gpio_enable_pullup(KNOB_CH4_PORT, KNOB_CH4_PIN, 0);
    }
}

void app_knob_init(void) {  //
    knob_gpio_init();
}

uint8_t app_knob_ticks(void) {
    uint8_t status = 0;
    status |= gpio_read_pin(KNOB_CH1_PORT, KNOB_CH1_PIN) ? 0x01 : 0x00;
    status |= gpio_read_pin(KNOB_CH2_PORT, KNOB_CH2_PIN) ? 0x02 : 0x00;
    status |= gpio_read_pin(KNOB_CH3_PORT, KNOB_CH3_PIN) ? 0x04 : 0x00;
    status |= gpio_read_pin(KNOB_CH4_PORT, KNOB_CH4_PIN) ? 0x08 : 0x00;

    return status;
}

void app_knob_deinit(void) {  //
    knob_gpio_deinit();
}
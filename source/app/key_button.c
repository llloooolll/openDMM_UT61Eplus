#include "key_button.h"

#include "ao_meter.h"
#include "gpio.h"
#include "io_config.h"
#include "ulog.h"

static struct Button button_select;
static struct Button button_range;
static struct Button button_hz;
static struct Button button_rel;
static struct Button button_peak;
static struct Button button_hold;

static void key_gpio_init(void) {
    gpio_enable_output(KNOB_CH1_PORT, KNOB_CH1_PIN, 0);
    gpio_enable_output(KNOB_CH2_PORT, KNOB_CH2_PIN, 0);
    gpio_enable_output(KNOB_CH3_PORT, KNOB_CH3_PIN, 0);
    gpio_enable_output(KNOB_CH4_PORT, KNOB_CH4_PIN, 0);
    gpio_enable_output(KEY_SELECT_PORT, KEY_SELECT_PIN, 0);
    gpio_enable_output(KEY_RANGE_PORT, KEY_RANGE_PIN, 0);
    gpio_enable_output(KEY_HZ_PORT, KEY_HZ_PIN, 0);
    gpio_enable_output(KEY_REL_PORT, KEY_REL_PIN, 0);
    gpio_enable_output(KEY_PEAK_PORT, KEY_PEAK_PIN, 0);
    gpio_enable_output(KEY_HOLD_PORT, KEY_HOLD_PIN, 0);

    gpio_enable_pullup(KNOB_CH1_PORT, KNOB_CH1_PIN, 1);
    gpio_enable_pullup(KNOB_CH2_PORT, KNOB_CH2_PIN, 1);
    gpio_enable_pullup(KNOB_CH3_PORT, KNOB_CH3_PIN, 1);
    gpio_enable_pullup(KNOB_CH4_PORT, KNOB_CH4_PIN, 1);
    gpio_enable_pullup(KEY_SELECT_PORT, KEY_SELECT_PIN, 1);
    gpio_enable_pullup(KEY_RANGE_PORT, KEY_RANGE_PIN, 1);
    gpio_enable_pullup(KEY_HZ_PORT, KEY_HZ_PIN, 1);
    gpio_enable_pullup(KEY_REL_PORT, KEY_REL_PIN, 1);
    gpio_enable_pullup(KEY_PEAK_PORT, KEY_PEAK_PIN, 1);
    gpio_enable_pullup(KEY_HOLD_PORT, KEY_HOLD_PIN, 1);
}

static uint8_t key_read_button(uint8_t button_id) {
    switch (button_id) {
        case button_select_id:
            return gpio_read_pin(KEY_SELECT_PORT, KEY_SELECT_PIN);
            break;
        case button_range_id:
            return gpio_read_pin(KEY_RANGE_PORT, KEY_RANGE_PIN);
            break;
        case button_hz_id:
            return gpio_read_pin(KEY_HZ_PORT, KEY_HZ_PIN);
            break;
        case button_rel_id:
            return gpio_read_pin(KEY_REL_PORT, KEY_REL_PIN);
            break;
        case button_peak_id:
            return gpio_read_pin(KEY_PEAK_PORT, KEY_PEAK_PIN);
            break;
        case button_hold_id:
            return gpio_read_pin(KEY_HOLD_PORT, KEY_HOLD_PIN);
            break;

        default:
            return 1;
            break;
    }
}

void key_button_call_back(void *btn) {
    QACTIVE_POST((QActive *)&ao_meter, AO_METER_KEY_SIG,
                 (((uint32_t)((Button *)btn)->button_id) << 4) |
                     (((Button *)btn)->event));
}

void key_init(void) {
    key_gpio_init();

    button_init(&button_select, key_read_button, 0, button_select_id);
    button_init(&button_range, key_read_button, 0, button_range_id);
    button_init(&button_hz, key_read_button, 0, button_hz_id);
    button_init(&button_rel, key_read_button, 0, button_rel_id);
    button_init(&button_peak, key_read_button, 0, button_peak_id);
    button_init(&button_hold, key_read_button, 0, button_hold_id);

    button_attach(&button_select, SINGLE_CLICK, key_button_call_back);
    button_attach(&button_range, SINGLE_CLICK, key_button_call_back);
    button_attach(&button_hz, SINGLE_CLICK, key_button_call_back);
    button_attach(&button_rel, SINGLE_CLICK, key_button_call_back);
    button_attach(&button_peak, SINGLE_CLICK, key_button_call_back);
    button_attach(&button_hold, SINGLE_CLICK, key_button_call_back);

    button_attach(&button_select, LONG_PRESS_START, key_button_call_back);
    button_attach(&button_range, LONG_PRESS_START, key_button_call_back);
    button_attach(&button_hz, LONG_PRESS_START, key_button_call_back);
    button_attach(&button_rel, LONG_PRESS_START, key_button_call_back);
    button_attach(&button_peak, LONG_PRESS_START, key_button_call_back);
    button_attach(&button_hold, LONG_PRESS_START, key_button_call_back);

    button_start(&button_select);
    button_start(&button_range);
    button_start(&button_hz);
    button_start(&button_rel);
    button_start(&button_peak);
    button_start(&button_hold);
}

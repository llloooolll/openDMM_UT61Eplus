#include "hc32l13x.h"
#include "hy2613.h"
#include "io_config.h"
#include "lcd.h"
#include "lcd_pixel.h"
#include "sysctrl.h"

lcd_pixel_t lcd_pixel;

int main(void) {
    sysctrl_set_rch_trim(sysctrl_rch_freq_24M);
    sysctrl_enable_peripheral_clk(sysctrl_peripheral_clk_gpio, 1);

    lcd_init();
    hy2613_test(1);
    hy2613_test(0);
    lcd_enable(1);

    // lcd_show_value(&lcd_pixel, 12345, -1 - 9);
    // lcd_refresh(&lcd_pixel);

    while (1) {
#if 1
        for (int8_t i = -10; i < 10; i++) {
            lcd_show_value(&lcd_pixel, 123456, i);
            lcd_refresh(&lcd_pixel);
            __NOP();
        }
#endif
    }
    return 0;
}

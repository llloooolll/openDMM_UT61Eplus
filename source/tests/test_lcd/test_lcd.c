#include "hc32l13x.h"
#include "sysctrl.h"
#include "hy2613.h"
#include "lcd_pixel.h"
#include "lcd.h"
#include "io_config.h"

int main(void)
{
    sysctrl_set_rch_trim(sysctrl_rch_freq_24M);
    sysctrl_enable_peripheral_clk(sysctrl_peripheral_clk_gpio, 1);

    lcd_init();
    hy2613_test(1);

    while (1)
    {
    }
    return 0;
}

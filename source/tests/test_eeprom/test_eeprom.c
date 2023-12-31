#include "hc32l13x.h"
#include "gpio.h"
#include "sysctrl.h"
#include "eeprom.h"
#include "lcd_pixel.h"
#include "lcd.h"
#include "irda.h"
#include "io_config.h"

int main(void)
{
    sysctrl_enable_peripheral_clk(sysctrl_peripheral_clk_gpio, 1);
    sysctrl_set_rch_trim(sysctrl_rch_freq_24M);

    uint8_t data;

    if (lcd_init())
    {
        irda_init(9600);

        for (uint32_t i = 0; i < 256; i++)
        {
            data = eeprom_read_byte((uint8_t)i);
            irda_send_byte(data);
        }
    }
    while (1)
    {
    }
    return 0;
}

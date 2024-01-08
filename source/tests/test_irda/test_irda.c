#include "hc32l13x.h"
#include "io_config.h"
#include "irda.h"
#include "sysctrl.h"

int main(void) {
    sysctrl_set_rch_trim(sysctrl_rch_freq_24M);
    sysctrl_enable_peripheral_clk(sysctrl_peripheral_clk_gpio, 1);

    irda_init(9600);

    while (1) {
        irda_test();
    }
    return 0;
}

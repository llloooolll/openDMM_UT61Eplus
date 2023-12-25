#include "hc32l13x.h"
#include "gpio.h"
#include "sysctrl.h"

int main(void)
{
    sysctrl_set_rch_trim(sysctrl_rch_freq_24M);
    sysctrl_switch_sys_clk_source(sysctrl_clk_source_rch);

    volatile uint32_t i = 0;

    while (1)
    {
        i++;
        if (i > 1000000)
        {
            sysctrl_set_rch_trim(sysctrl_rch_freq_4M);
            i = 0;
        }
    }
    return 0;
}

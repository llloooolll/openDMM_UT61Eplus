#include "hc32l13x.h"
#include "gpio.h"
#include "sysctrl.h"

int main(void)
{
    sysctrl_rch_trim_set(sysctrl_rch_freq_24M);
    sysctrl_clk_source_switch(sysctrl_clk_source_rch);

    volatile uint32_t i = 0;

    while (1)
    {
        i++;
        if (i > 1000000)
        {
            sysctrl_rch_trim_set(sysctrl_rch_freq_4M);
            i = 0;
        }
    }
    return 0;
}

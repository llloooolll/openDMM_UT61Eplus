#include "hc32l13x.h"
#include "gpio.h"
#include "sysctrl.h"

int main(void)
{
    sysctrl_rch_trim_set(sysctrl_rch_freq_24M);
    sysctrl_clk_source_switch(sysctrl_clk_source_rch);

    while (1)
    {
    }
    return 0;
}

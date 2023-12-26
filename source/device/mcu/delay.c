#include "delay.h"
#include "hc32l13x.h"
#include "sysctrl.h"
#include "binary.h"
#include "bits.h"

void delay_init(void)
{
    sysctrl_enable_peripheral_clk(sysctrl_peripheral_clk_base_tim, 1);

    M0P_TIM0_MODE0->M0CR = 0x00;
    M0P_TIM0_MODE0->ICLR = 0x00;
}

void delay_cycle(uint32_t cycle)
{
    M0P_TIM0_MODE0->CNT32 = UINT32_MAX - cycle;
    M0P_TIM0_MODE0->M0CR = 0x01;
    while (!(M0P_TIM0_MODE0->IFR))
    {
        ;
    }
    M0P_TIM0_MODE0->M0CR = 0x00;
    M0P_TIM0_MODE0->ICLR = 0x00;
}

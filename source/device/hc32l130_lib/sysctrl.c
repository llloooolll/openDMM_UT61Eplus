#include "sysctrl.h"
#include "hc32l13x.h"
#include "binary.h"
#include "bits.h"

static void sysctrl_unlock(void);

/**
 * @brief 解锁SYSCTRL寄存器
 *
 */
static void sysctrl_unlock(void)
{
    M0P_SYSCTRL->SYSCTRL2 = 0x5A5A;
    M0P_SYSCTRL->SYSCTRL2 = 0xA5A5;
}

/**
 * @brief 使能时钟源
 *
 * @param clk 时钟源
 * @param flag
 */
void sysctrl_source_enable(sysctrl_clk_t clk, bool flag)
{
    sysctrl_unlock(); // 解锁SYSCTRL0
    switch (clk)
    {
    case sysctrl_clk_rch:
        M0P_SYSCTRL->SYSCTRL0_f.RCH_EN = flag;
        while (flag && (M0P_SYSCTRL->RCH_CR_f.STABLE != 1U))
        {
            ;
        }
        break;
    case sysctrl_clk_xth:
        M0P_SYSCTRL->SYSCTRL0_f.XTH_EN = flag;
        break;
    case sysctrl_clk_rcl:
        M0P_SYSCTRL->SYSCTRL0_f.RCL_EN = flag;
        while (flag && (M0P_SYSCTRL->RCL_CR_f.STABLE != 1U))
        {
            ;
        }
        break;
    case sysctrl_clk_xtl:
        M0P_SYSCTRL->SYSCTRL0_f.XTL_EN = flag;
        break;
    case sysctrl_clk_pll:
        M0P_SYSCTRL->SYSCTRL0_f.PLL_EN = flag;
        break;
    default:
        break;
    }
}

/**
 * @brief 设置RCL稳定时间
 *
 * @param cycle 时钟周期
 */
void sysctrl_rcl_stable_time(sysctrl_rcl_cycle_t cycle)
{
    M0P_SYSCTRL->RCL_CR_f.STARTUP = cycle;
}

/**
 * @brief 设置系统时钟源
 *
 * @param source 时钟源
 */
void sysctrl_clk_source_switch(sysctrl_clk_source_t source)
{
    sysctrl_unlock();
    M0P_SYSCTRL->SYSCTRL0_f.CLKSW = source;
}

/**
 * @brief 获取HCLK
 *
 * @return uint32_t 频率(Hz)
 */
uint32_t sysctrl_hclk_get(void)
{
    uint32_t hclk_freq = 0;
    const uint32_t rch_freq_table[] = {24000000, 22120000, 16000000, 8000000, 4000000};
    const uint32_t rcl_freq_table[] = {32768, 38400};

    sysctrl_clk_source_t corrent_clk_source = (sysctrl_clk_source_t)(M0P_SYSCTRL->SYSCTRL0_f.CLKSW);

    switch (corrent_clk_source)
    {
    case sysctrl_clk_source_rch:
        if ((M0P_SYSCTRL->RCH_CR_f.TRIM) == RCH_TRIM_24M)
        {
            hclk_freq = rch_freq_table[0];
        }
        else if ((M0P_SYSCTRL->RCH_CR_f.TRIM) == RCH_TRIM_22_12M)
        {
            hclk_freq = rch_freq_table[1];
        }
        else if ((M0P_SYSCTRL->RCH_CR_f.TRIM) == RCH_TRIM_16M)
        {
            hclk_freq = rch_freq_table[2];
        }
        else if ((M0P_SYSCTRL->RCH_CR_f.TRIM) == RCH_TRIM_8M)
        {
            hclk_freq = rch_freq_table[3];
        }
        else
        {
            hclk_freq = rch_freq_table[4];
        }
        break;
    case sysctrl_clk_source_xth:
        hclk_freq = SYSTEM_XTH;
        break;
    case sysctrl_clk_source_rcl:
        if (RCL_TRIM_38_4K == (M0P_SYSCTRL->RCL_CR_f.TRIM))
        {
            hclk_freq = rcl_freq_table[1];
        }
        else
        {
            hclk_freq = rcl_freq_table[0];
        }
        break;
    case sysctrl_clk_source_xtl:
        hclk_freq = SYSTEM_XTL;
        break;
    case sysctrl_clk_source_pll:
        if (sysctrl_clk_source_rch == M0P_SYSCTRL->PLL_CR_f.REFSEL)
        {
            if ((M0P_SYSCTRL->RCH_CR_f.TRIM) == RCH_TRIM_24M)
            {
                hclk_freq = rch_freq_table[0];
            }
            else if ((M0P_SYSCTRL->RCH_CR_f.TRIM) == RCH_TRIM_22_12M)
            {
                hclk_freq = rch_freq_table[1];
            }
            else if ((M0P_SYSCTRL->RCH_CR_f.TRIM) == RCH_TRIM_16M)
            {
                hclk_freq = rch_freq_table[2];
            }
            else if ((M0P_SYSCTRL->RCH_CR_f.TRIM) == RCH_TRIM_8M)
            {
                hclk_freq = rch_freq_table[3];
            }
            else
            {
                hclk_freq = rch_freq_table[4];
            }
        }
        else
        {
            hclk_freq = SYSTEM_XTH;
        }

        hclk_freq = (hclk_freq * (M0P_SYSCTRL->PLL_CR_f.DIVN));
        break;
    default:
        hclk_freq = 0u;
        break;
    }

    hclk_freq = (hclk_freq >> (M0P_SYSCTRL->SYSCTRL0_f.HCLK_PRS));

    return hclk_freq;
}

/**
 * @brief 获取PCLK频率
 *
 * @return uint32_t 频率(Hz)
 */
uint32_t sysctrl_pclk_get(void)
{
    uint32_t pclk_freq = sysctrl_hclk_get();
    pclk_freq = (pclk_freq >> (M0P_SYSCTRL->SYSCTRL0_f.PCLK_PRS));

    return pclk_freq;
}

/**
 * @brief 设置RCH校准值
 *
 * @param freq
 */
void sysctrl_rch_trim_set(sysctrl_rch_freq_t freq)
{
    M0P_SYSCTRL->RCH_CR_f.TRIM = *(&(RCH_TRIM_24M) + freq);
}

/**
 * @brief 设置RCL校准值
 *
 * @param freq
 */
void sysctrl_rcl_trim_set(sysctrl_rcl_freq_t freq)
{
    M0P_SYSCTRL->RCL_CR_f.TRIM = *(&(RCL_TRIM_38_4K) + freq);
}

/**
 * @brief 设置HCLK分频
 *
 * @param div
 */
void sysctrl_hclk_div_set(sysctrl_hclk_div_t div)
{
    sysctrl_unlock();
    M0P_SYSCTRL->SYSCTRL0_f.HCLK_PRS = div;
}

/**
 * @brief 设置PCLK分频
 *
 * @param div
 */
void sysctrl_pclk_div_set(sysctrl_pclk_div_t div)
{
    sysctrl_unlock();
    M0P_SYSCTRL->SYSCTRL0_f.PCLK_PRS = div;
}

/**
 * @brief 使能外设时钟
 *
 * @param peripheral
 */
void sysctrl_peripheral_clk_enable(sysctrl_peripheral_clk_t peripheral, bool flag)
{
    if (flag)
    {
        BIT_SET(M0P_SYSCTRL->PERI_CLKEN, peripheral);
    }
    else
    {
        BIT_CLR(M0P_SYSCTRL->PERI_CLKEN, peripheral);
    }
}

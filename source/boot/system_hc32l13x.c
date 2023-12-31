/*******************************************************************************
 * Copyright (C) 2019, Xiaohua Semiconductor Co.,Ltd All rights reserved.
 *
 * This software is owned and published by:
 * Xiaohua Semiconductor Co.,Ltd ("XHSC").
 *
 * BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
 * BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
 *
 * This software contains source code for use with XHSC
 * components. This software is licensed by XHSC to be adapted only
 * for use in systems utilizing XHSC components. XHSC shall not be
 * responsible for misuse or illegal use of this software for devices not
 * supported herein. XHSC is providing this software "AS IS" and will
 * not be responsible for issues arising from incorrect user implementation
 * of the software.
 *
 * Disclaimer:
 * XHSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
 * REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS),
 * ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
 * WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
 * WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
 * WARRANTY OF NONINFRINGEMENT.
 * XHSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
 * NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
 * LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
 * LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
 * INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
 * SAVINGS OR PROFITS,
 * EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
 * INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
 * FROM, THE SOFTWARE.
 *
 * This software may be replicated in part or whole for the licensed use,
 * with the restriction that this Disclaimer and Copyright notice must be
 * included with each copy of this software, whether used in part or whole,
 * at all times.
 */

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "hc32l13x.h"
#include "system_hc32l13x.h"

#define SYSTEM_XTH 8000000U
#define SYSTEM_XTL 32768U

/**
 ******************************************************************************
 ** System Clock Frequency (Core Clock) Variable according CMSIS
 ******************************************************************************/
uint32_t SystemCoreClock = 4000000;

#ifndef RCH_TRIM_24M
#define RCH_TRIM_24M (*((volatile uint16_t *)(0x00100C00UL)))
#define RCH_TRIM_22_12M (*((volatile uint16_t *)(0x00100C02UL)))
#define RCH_TRIM_16M (*((volatile uint16_t *)(0x00100C04UL)))
#define RCH_TRIM_8M (*((volatile uint16_t *)(0x00100C06UL)))
#define RCH_TRIM_4M (*((volatile uint16_t *)(0x00100C08UL)))

#define RCL_TRIM_38_4K (*((volatile uint16_t *)(0x00100C20UL)))
#define RCL_TRIM_32_768K (*((volatile uint16_t *)(0x00100C22UL)))
#endif

/**
 * @brief Update SystemCoreClock variable
 *
 */
void SystemCoreClockUpdate(void)
{
	typedef enum _sysctrl_clk_source_t
	{
		sysctrl_clk_source_rch = 0U,
		sysctrl_clk_source_xth = 1U,
		sysctrl_clk_source_rcl = 2U,
		sysctrl_clk_source_xtl = 3U,
		sysctrl_clk_source_pll = 4U,
	} sysctrl_clk_source_t;

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

	SystemCoreClock = hclk_freq;
}

/**
 * @brief Setup the microcontroller system. Initialize the System and update
 * the SystemCoreClock variable.
 *
 */
void SystemInit(void)
{
	M0P_SYSCTRL->RCL_CR_f.TRIM = RCL_TRIM_32_768K;
	M0P_SYSCTRL->RCH_CR_f.TRIM = RCH_TRIM_4M;
	M0P_SYSCTRL->RCH_CR_f.TRIM = RCH_TRIM_8M;
	M0P_SYSCTRL->RCH_CR_f.TRIM = RCH_TRIM_16M;
	M0P_SYSCTRL->RCH_CR_f.TRIM = RCH_TRIM_24M;

	SystemCoreClockUpdate();
}

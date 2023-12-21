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

/**
 * @brief 获得系统时钟（HCLK）频率值
 *
 * @return uint32_t HCLK频率值
 */
static uint32_t Sysctrl_GetHClkFreq(void)
{
	uint32_t u32Val = 0;
	const uint32_t u32hcr_tbl[] = {4000000, 8000000, 16000000, 22120000, 24000000};
	const uint16_t u32lcr_tbl[] = {32768, 38400};
	uint16_t u16Trim[5] = {
		(*((volatile uint16_t *)(0x00100C00ul))),
		(*((volatile uint16_t *)(0x00100C02ul))),
		(*((volatile uint16_t *)(0x00100C04ul))),
		(*((volatile uint16_t *)(0x00100C06ul))),
		(*((volatile uint16_t *)(0x00100C20ul))),
	};

	// 获取当前系统时钟
	uint8_t enSrc = M0P_SYSCTRL->SYSCTRL0_f.CLKSW;

	switch (enSrc)
	{
	case 0: // RCH
	{
		if ((M0P_SYSCTRL->RCH_CR_f.TRIM) == (u16Trim[4]))
		{
			u32Val = u32hcr_tbl[4];
		}
		else if ((M0P_SYSCTRL->RCH_CR_f.TRIM) == (u16Trim[3]))
		{
			u32Val = u32hcr_tbl[3];
		}
		else if ((M0P_SYSCTRL->RCH_CR_f.TRIM) == (u16Trim[2]))
		{
			u32Val = u32hcr_tbl[2];
		}
		else if ((M0P_SYSCTRL->RCH_CR_f.TRIM) == (u16Trim[1]))
		{
			u32Val = u32hcr_tbl[1];
		}
		else
		{
			u32Val = u32hcr_tbl[0];
		}
	}
	break;
	case 1: // XTH
		u32Val = SYSTEM_XTH;
		break;
	case 2: // RCL
	{
		if (u16Trim[0] == (M0P_SYSCTRL->RCL_CR_f.TRIM))
		{
			u32Val = u32lcr_tbl[1];
		}
		else
		{
			u32Val = u32lcr_tbl[0];
		}
	}
	break;
	case 3: // XTL
		u32Val = SYSTEM_XTL;
		break;
	case 4: // PLL
	{
		if (0 == M0P_SYSCTRL->PLL_CR_f.REFSEL)
		{
			if (u16Trim[4] == M0P_SYSCTRL->RCH_CR_f.TRIM)
			{
				u32Val = u32hcr_tbl[4];
			}
			else if (u16Trim[3] == M0P_SYSCTRL->RCH_CR_f.TRIM)
			{
				u32Val = u32hcr_tbl[3];
			}
			else if (u16Trim[2] == M0P_SYSCTRL->RCH_CR_f.TRIM)
			{
				u32Val = u32hcr_tbl[2];
			}
			else if (u16Trim[1] == M0P_SYSCTRL->RCH_CR_f.TRIM)
			{
				u32Val = u32hcr_tbl[1];
			}
			else
			{
				u32Val = u32hcr_tbl[0];
			}
		}
		else
		{
			u32Val = SYSTEM_XTH;
		}

		u32Val = (u32Val * M0P_SYSCTRL->PLL_CR_f.DIVN);
	}
	break;

	default:
		u32Val = 0u;
		break;
	}

	u32Val = (u32Val >> M0P_SYSCTRL->SYSCTRL0_f.HCLK_PRS);

	return u32Val;
}

/**
 * @brief Update SystemCoreClock variable
 *
 */
void SystemCoreClockUpdate(void)
{
	SystemCoreClock = Sysctrl_GetHClkFreq();
}

/**
 * @brief Setup the microcontroller system. Initialize the System and update
 * the SystemCoreClock variable.
 *
 */
void SystemInit(void)
{
	M0P_SYSCTRL->RCL_CR_f.TRIM = (*((volatile uint16_t *)(0x00100C22ul))); // 32768
	M0P_SYSCTRL->RCH_CR_f.TRIM = (*((volatile uint16_t *)(0x00100C08ul))); // 4M
}

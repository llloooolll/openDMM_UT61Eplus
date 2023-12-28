/*****************************************************************************
 * Product: "Blinky" on EK-TM4C123GXL board, cooperative QV kernel
 * Last Updated for Version: 5.5.1
 * Date of the Last Update:  2015-10-05
 *
 *                    Q u a n t u m     L e a P s
 *                    ---------------------------
 *                    innovating embedded systems
 *
 * Copyright (C) Quantum Leaps, LLC. All rights reserved.
 *
 * This program is open source software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Alternatively, this program may be distributed and modified under the
 * terms of Quantum Leaps commercial licenses, which expressly supersede
 * the GNU General Public License and are specifically designed for
 * licensees interested in retaining the proprietary status of their code.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <www.gnu.org/licenses/>.
 *
 * Contact information:
 * <www.state-machine.com/licensing>
 * <info@state-machine.com>
 *****************************************************************************/
#include "qpn.h"
#include "blinky.h"
#include "bsp.h"

#include "hc32l13x.h"
#include "sysctrl.h"
#include "gpio.h"
#include "io_config.h"
/* add other drivers if necessary... */

// Q_DEFINE_THIS_FILE

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! CAUTION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * Assign a priority to EVERY ISR explicitly by calling NVIC_SetPriority().
 * DO NOT LEAVE THE ISR PRIORITIES AT THE DEFAULT VALUE!
 */
enum KernelUnawareISRs
{ /* see NOTE00 */
  /* ... */
  MAX_KERNEL_UNAWARE_CMSIS_PRI /* keep always last */
};
/* "kernel-unaware" interrupts can't overlap "kernel-aware" interrupts */
Q_ASSERT_COMPILE(MAX_KERNEL_UNAWARE_CMSIS_PRI <= QF_AWARE_ISR_CMSIS_PRI);

enum KernelAwareISRs
{
    SYSTICK_PRIO = QF_AWARE_ISR_CMSIS_PRI, /* see NOTE00 */
    /* ... */
    MAX_KERNEL_AWARE_CMSIS_PRI /* keep always last */
};
/* "kernel-aware" interrupts should not overlap the PendSV priority */
Q_ASSERT_COMPILE(MAX_KERNEL_AWARE_CMSIS_PRI <= (0xFF >> (8 - __NVIC_PRIO_BITS)));

/* ISRs defined in this BSP ------------------------------------------------*/
void SysTick_Handler(void);

/* Local-scope objects -----------------------------------------------------*/

/* ISRs used in this project ===============================================*/
void SysTick_Handler(void)
{
    QF_tickXISR(0U); /* process time events for rate 0 */
}

/* BSP functions ===========================================================*/
void BSP_init(void)
{
    sysctrl_enable_peripheral_clk(sysctrl_peripheral_clk_gpio, 1);
    sysctrl_enable_peripheral_clk(sysctrl_peripheral_clk_tick, 1);
    /* NOTE: SystemInit() already called from the startup code
     *  but SystemCoreClock needs to be updated
     */
    SystemCoreClockUpdate();

    gpio_enable_output(LED_GREEN_ENB_PORT, LED_GREEN_ENB_PIN, 1);
    gpio_set_pin(LED_GREEN_ENB_PORT, LED_GREEN_ENB_PIN);
}
/*..........................................................................*/
void BSP_ledOff(void)
{
    gpio_set_pin(LED_GREEN_ENB_PORT, LED_GREEN_ENB_PIN);
}
/*..........................................................................*/
void BSP_ledOn(void)
{
    gpio_clear_pin(LED_GREEN_ENB_PORT, LED_GREEN_ENB_PIN);
}

/* QF callbacks ============================================================*/
void QF_onStartup(void)
{
    /* set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate */
    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);

    /* assing all priority bits for preemption-prio. and none to sub-prio. */
    NVIC_SetPriorityGrouping(0U);

    /* set priorities of ALL ISRs used in the system, see NOTE00
     *
     * !!!!!!!!!!!!!!!!!!!!!!!!!!!! CAUTION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
     * Assign a priority to EVERY ISR explicitly by calling NVIC_SetPriority().
     * DO NOT LEAVE THE ISR PRIORITIES AT THE DEFAULT VALUE!
     */
    NVIC_SetPriority(SysTick_IRQn, SYSTICK_PRIO);
    /* ... */

    /* enable IRQs... */
}
/*..........................................................................*/
void QV_onIdle(void)
{ /* CATION: called with interrupts DISABLED, NOTE01 */

#ifdef NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
     * you might need to customize the clock management for your application,
     * see the datasheet for your particular Cortex-M MCU.
     */
    QV_CPU_SLEEP(); /* atomically go to sleep and enable interrupts */
#else
    QF_INT_ENABLE(); /* just enable interrupts */
#endif
}

/*..........................................................................*/
Q_NORETURN Q_onAssert(char const Q_ROM *const module, int loc)
{
    /*
     * NOTE: add here your application-specific error handling
     */
    (void)module;
    (void)loc;

    NVIC_SystemReset();
}

/*****************************************************************************
 * NOTE00:
 * The QF_AWARE_ISR_CMSIS_PRI constant from the QF port specifies the highest
 * ISR priority that is disabled by the QF framework. The value is suitable
 * for the NVIC_SetPriority() CMSIS function.
 *
 * Only ISRs prioritized at or below the QF_AWARE_ISR_CMSIS_PRI level (i.e.,
 * with the numerical values of priorities equal or higher than
 * QF_AWARE_ISR_CMSIS_PRI) are allowed to call any QF services. These ISRs
 * are "QF-aware".
 *
 * Conversely, any ISRs prioritized above the QF_AWARE_ISR_CMSIS_PRI priority
 * level (i.e., with the numerical values of priorities less than
 * QF_AWARE_ISR_CMSIS_PRI) are never disabled and are not aware of the kernel.
 * Such "QF-unaware" ISRs cannot call any QF services. The only mechanism
 * by which a "QF-unaware" ISR can communicate with the QF framework is by
 * triggering a "QF-aware" ISR, which can post/publish events.
 *
 * NOTE01:
 * The QV_onIdle() callback is called with interrupts disabled, because the
 * determination of the idle condition might change by any interrupt posting
 * an event. QV_onIdle() must internally enable interrupts, ideally
 * atomically with putting the CPU to the power-saving mode.
 *
 * NOTE02:
 * One of the LEDs is used to visualize the idle loop activity. The brightness
 * of the LED is proportional to the frequency of invcations of the idle loop.
 * Please note that the LED is toggled with interrupts locked, so no interrupt
 * execution time contributes to the brightness of the User LED.
 */

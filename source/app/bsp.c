#include "qpn.h"
#include "bsp.h"

#include "hc32l13x.h"
#include "sysctrl.h"
#include "irda.h"

// 中断优先级，数字越大优先级越低
enum KernelAwareISRs
{
    SYSTICK_PRIO = QF_AWARE_ISR_CMSIS_PRI, // tick优先级最高
    UART0_PRIO = QF_AWARE_ISR_CMSIS_PRI + 1U,
    GPIO_PRIO = QF_AWARE_ISR_CMSIS_PRI + 2U,
    // ...
    MAX_KERNEL_AWARE_CMSIS_PRI
};
/* 最低优先级不能比Pend_SV更低 */
Q_ASSERT_COMPILE(MAX_KERNEL_AWARE_CMSIS_PRI <= (0xFF >> (8 - __NVIC_PRIO_BITS)));

void SysTick_Handler(void)
{
    QF_tickXISR(0U);
}

void bsp_init(void)
{
    SystemCoreClockUpdate();

    sysctrl_enable_peripheral_clk(sysctrl_peripheral_clk_gpio, 1);
    sysctrl_enable_peripheral_clk(sysctrl_peripheral_clk_uart0, 1);
    sysctrl_enable_peripheral_clk(sysctrl_peripheral_clk_tick, 1);
    sysctrl_enable_peripheral_clk(sysctrl_peripheral_clk_base_tim, 1);
}

/**
 * @brief 状态机启动前处理
 *
 */
void QF_onStartup(void)
{
    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);
    NVIC_SetPriorityGrouping(0U);
    NVIC_SetPriority(SysTick_IRQn, SYSTICK_PRIO);
}

/**
 * @brief 空闲回调函数
 *
 */
void QV_onIdle(void)
{
    QF_INT_ENABLE();
}

/**
 * @brief 断言失败回调函数
 *
 * @param module
 * @param loc
 * @return Q_NORETURN
 */
Q_NORETURN Q_onAssert(char const Q_ROM *const module, int loc)
{
    (void)module;
    (void)loc;

    NVIC_SystemReset();
}

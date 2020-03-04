


#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_tim.h"

#include <touchgfx/hal/HAL.hpp>
#include <STM32H7Instrumentation.hpp>

namespace touchgfx
{
static TIM_HandleTypeDef tim;

void STM32H7Instrumentation::init()
{
    uint32_t uwTimclock, uwD2Prescaler = 0U;

    /* TIM2 is on ABP1 in D2 domain */

    __HAL_RCC_TIM2_CLK_ENABLE();

    tim.Instance = TIM2;
    tim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    tim.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim.Init.Period = 0xFFFFFFFF;
    tim.Init.Prescaler = 0;
    tim.Init.RepetitionCounter = 1;

    uwD2Prescaler = D1CorePrescTable[(RCC->D2CFGR & RCC_D2CFGR_D2PPRE1)>> POSITION_VAL(RCC_D2CFGR_D2PPRE1_0)];
    if (!uwD2Prescaler)
        uwTimclock = HAL_RCC_GetHCLKFreq();
    else
        uwTimclock = 2 * HAL_RCC_GetPCLK1Freq();

    m_sysclkRatio = HAL_RCC_GetSysClockFreq() / uwTimclock;

    HAL_TIM_Base_Init(&tim);
    HAL_TIM_Base_Start(&tim);
}

unsigned int STM32H7Instrumentation::getElapsedUS(unsigned int start, unsigned int now, unsigned int clockfrequency)
{
    return ((now - start) + (clockfrequency / 2)) / clockfrequency;
}

unsigned int STM32H7Instrumentation::getCPUCycles()
{
    return __HAL_TIM_GET_COUNTER(&tim) * m_sysclkRatio;
}

void STM32H7Instrumentation::setMCUActive(bool active)
{
    if (active) //idle task sched out
    {
        uint32_t current_cc = cc_in;
        cc_consumed += getCPUCycles() - current_cc;
    }
    else //idle task sched in
    {
        cc_in = getCPUCycles();
    }
}

}

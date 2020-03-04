#include "stm32h7b3i_eval.h"

#include <STM32H7Instrumentation.hpp>

#include <stdio.h>
#include <string.h>

#include <stm32h7xx_hal.h>
#include <stm32h7xx_hal_uart.h>

extern UART_HandleTypeDef UartHandle;

extern "C"
{
#include "stm32h7xx_hal_rcc.h"
#include "stm32h7xx_hal_rcc_ex.h"
#include "stm32h7xx_hal_tim.h"
}
namespace touchgfx
{
TIM_HandleTypeDef tim;

#define __TIM2_CLK_ENABLE __HAL_RCC_TIM2_CLK_ENABLE

void STM32H7Instrumentation::init()
{
    __TIM2_CLK_ENABLE();
    tim.Instance = TIM2;
    tim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    tim.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim.Init.Period = 0xFFFFFFFF;
    tim.Init.Prescaler = 0;
    tim.Init.RepetitionCounter = 1;
    HAL_TIM_Base_Init(&tim);
    HAL_TIM_Base_Start(&tim);
}

//Board specific clockfrequency
unsigned int STM32H7Instrumentation::getElapsedUS(unsigned int start, unsigned int now, unsigned int clockfrequency)
{
    return ((now - start) + (clockfrequency / 2)) / clockfrequency;
}


unsigned int STM32H7Instrumentation::getCPUCycles()
{
    return __HAL_TIM_GET_COUNTER(&tim);
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

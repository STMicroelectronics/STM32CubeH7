/**
  @page PWR_STANDBY PWR standby example

  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_STANDBY/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the PWR STANDBY example.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

This example shows how to enter the system in STANDBY mode and wake-up from this
mode using external RESET or WKUP pin connected to the User Button.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H743xx Devices :
 - The CPU at 400MHz
 - The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals
   and D3 Domain AHB4  peripherals at 200MHz.
 - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals
   and D3 Domain APB4 peripherals to  run at 100MHz

The SysTick is programmed to generate an interrupt each 1 ms.

After a reset (or wake-up from STANDBY mode), LED1 is ON and MCU is in RUN mode.
The system enters STANDBY mode after 5s, LED1 is OFF.
A rising edge on User Button PC.13 will wake-up the system from STANDBY.
Alternatively, an external RESET of the board will lead to a system wake-up as well.
The LED1 blinks once in order to indicate the system was resumed from Standby mode.
These steps are repeated in an infinite loop.

LED2 ON: configuration failed (system will go to an infinite loop)

@note This example use clock provide by STLink (HSE bypass mode).
In order to reduce the power consumption SB13 and SB112 must be opened.
As consequence the example clock source must be changed to internal HSI :
  -  in SystemClock_Config :
       - Change RCC_OscInitStruct.OscillatorType to RCC_OSCILLATORTYPE_HSI
       - Change RCC_OscInitStruct.HSIState to RCC_HSI_ON
       - Change the RCC_OscInitStruct.PLL.PLLSource to RCC_PLLSOURCE_HSI
       - Change RCC_OscInitStruct.PLL.PLLM to 32


@note This example can not be used in DEBUG mode due to the fact
      that the Cortex-M7 core is no longer clocked during low power mode
      so debugging features are disabled.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes).
 
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

System, Power, PWR, Standby mode, Interrupt, EXTI, Wakeup, Wakeup pin, Low Power, External reset, Button

@par Directory contents

  - PWR/PWR_STANDBY/Inc/stm32h7xx_conf.h         HAL Configuration file
  - PWR/PWR_STANDBY/Inc/stm32h7xx_it.h           Header for stm32h7xx_it.c
  - PWR/PWR_STANDBY/Inc/main.h                   Header file for main.c
  - PWR/PWR_STANDBY/Src/system_stm32h7xx.c       STM32H7xx system clock configuration file
  - PWR/PWR_STANDBY/Src/stm32h7xx_it.c           Interrupt handlers
  - PWR/PWR_STANDBY/Src/main.c                   Main program
  - PWR/PWR_STANDBY/Src/stm32h7xx_hal_msp.c      HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32H743xx devices

  - This example has been tested with STMicroelectronics NUCLEO-H743ZI
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */

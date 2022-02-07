/**
  @page PWR_STOP_RTC Power Stop Mode Example 
  
  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_STOP_RTC/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Power Stop Mode using RTC example.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description 

How to enter the Stop mode and wake up from this mode by using the RTC wakeup 
timer event connected to an interrupt.

At the beginning of the main program the SystemClock_Config() function is called to configure the system clock for STM32H735xx Devices :
The CPU at 520 MHz.
The HCLK for D1 Domain AXI and AHB3 peripherals, D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4 peripherals at 520 MHz/2.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 520 MHz/4.

This event is connected to EXTI_Line19.

LED1 is ON during RUN mode and OFF during STOP mode.
LED2 is ON when the configuration is failed (system will go to an infinite loop).

In the associated software
  - the system clock is set to 520 MHz.
  - the Low Speed Internal (LSI) clock is used as RTC clock source by default.
  - the EXTI_Line19 connected internally to the RTC Wakeup event is configured
    to generate an interrupt on rising edge each 20s.
  - the SysTick is programmed to generate an interrupt each 1 ms. 

LED1 is ON. The system enters STOP mode after 5s and will wait for the RTC Wakeup event to be
generated each 20s, LED1 is OFF. 
After the system woken up from STOP, the clock system is reconfigured because the default clock 
after wake up is the HSI.

The above scenario is repeated in an infinite loop.
 
    - STOP Mode with RTC clocked by LSI
    ===================================  
            - RTC Clocked by LSI
            - Regulator in LP mode
            - HSI as SysClk after Wake Up
            - No IWDG
            - Automatic Wakeup using RTC on LSI (after ~20s)


@note This example can not be used in DEBUG mode, this is due to the fact 
      that the Cortex-M7 core is no longer clocked during low power mode 
       so debugging features are disabled.

@note Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select 
      the RTC clock source; in this case the Backup domain will be reset in  
      order to modify the RTC Clock source, as consequence RTC registers (including 
      the backup registers) and RCC_CSR register are set to their reset values.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

Power, PWR, Stop mode, Interrupt, EXTI, Wakeup, Low Power, RTC, External reset, LSI, 

@par Directory contents 

  - PWR/PWR_STOP_RTC/Inc/stm32h7xx_conf.h         HAL Configuration file
  - PWR/PWR_STOP_RTC/Inc/stm32h7xx_it.h           Header for stm32h7xx_it.c
  - PWR/PWR_STOP_RTC/Inc/main.h                   Header file for main.c
  - PWR/PWR_STOP_RTC/Inc/stm32h735g_discovery_conf.h   STM32H735G-DK board configuration file
  - PWR/PWR_STOP_RTC/Src/system_stm32h7xx.c       STM32H7xx system clock configuration file
  - PWR/PWR_STOP_RTC/Src/stm32h7xx_it.c           Interrupt handlers
  - PWR/PWR_STOP_RTC/Src/main.c                   Main program
  - PWR/PWR_STOP_RTC/Src/stm32h7xx_hal_msp.c      HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32H735xx devices
      
  - This example has been tested with STMicroelectronics STM32H735G-DK
    board and can be easily tailored to any other supported device 
    and development board.
  - STM32H735G-DK Set-up :
   - None
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example



 */

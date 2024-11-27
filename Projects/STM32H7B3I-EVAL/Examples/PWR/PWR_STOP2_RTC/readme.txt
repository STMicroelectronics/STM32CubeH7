/**
  @page PWR_STOP2_RTC example

  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_STOP2_RTC/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the PWR STOP2 RTC example.
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

This example shows how to enter the system in STOP mode with main domain in DSTOP2 and wake-up from this mode
using RTC Wake-up Timer with memory shut-off option enabled.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7B3xxQ Devices :
The CPU at 280MHz
The HCLK for CD Domain AXI and AHB3 peripherals , CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4  peripherals at 280MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 140MHz.

The SysTick is programmed to generate an interrupt each 1 ms.

The Low Speed Internal (LSI) clock is used as RTC clock source by default.
EXTI_Line19 is internally connected to the RTC Wakeup event.

The memory shut-off option is enabled for AXI SRAM1 Memory block which is previously filled in.

The system automatically enters to STOP mode with main domain in DSTOP2 5 seconds after start-up. The RTC wake-up is configured to generate an interrupt on rising
edge about 20 seconds afterwards. Current consumption in STOP mode with RTC feature enabled can be measured during that time.
These 20 seconds are chosen to ensure current convergence to its lowest operating point.

After wake-up from STOP mode, CPU resumes execution from where it stopped. The system clock is reconfigured as the default clock
after wake-up is the HSI (STOPWUCK = 0). SYSCLKConfig_STOP() function is called to re-configure Oscillator and System Clock
according to internal RCC registers (enable HSE, PLL and select PLL as system clock source) and voltage scale is set to
scale 0 as it is reset to default scale 3 when exiting from Stop mode. Finally, the content of AXI SRAM1 memory is checked.

Two leds GREEN LED and RED LED are used to monitor the system state as follows:
      - RED LED ON: configuration failed or AXI SRAM memory block shut-off failed (system will go to an infinite loop)
      - GREEN LED toggling: system in RUN mode
      - GREEN LED off : system in STOP mode

These steps are repeated in an infinite loop.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application  needs  to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@note It is recommended to enable the cache and maintain its coherence.
      Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
      Please refer to the AN4839 “Level 1 cache on STM32F7 Series”

@par Keywords

System, Power, PWR, Stop mode, Stop2, Interrupt, EXTI, Wakeup, Low Power, RTC, LSI

@par Directory contents

  - PWR/PWR_STOP2_RTC/Inc/stm32h7xx_conf.h         HAL Configuration file
  - PWR/PWR_STOP2_RTC/Inc/stm32h7xx_it.h           Header for stm32h7xx_it.c
  - PWR/PWR_STOP2_RTC/Inc/stm32h7b3i_eval_conf.h   STM32H7B3I_EVAL board configuration file
  - PWR/PWR_STOP2_RTC/Inc/main.h                   Header file for main.c

  - PWR/PWR_STOP2_RTC/Src/system_stm32h7xx.c       STM32H7xx system clock configuration file
  - PWR/PWR_STOP2_RTC/Src/stm32h7xx_it.c           Interrupt handlers
  - PWR/PWR_STOP2_RTC/Src/main.c                   Main program
  - PWR/PWR_STOP2_RTC/Src/stm32h7xx_hal_msp.c      HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32H7B3xxQ devices

  - This example has been tested with STMicroelectronics STM32H7B3I-EVAL (rev B)
    board and can be easily tailored to any other supported device
    and development board.

  - STM32H7B3I-EVAL Set-up :
       - Use GREEN LED and RED LED.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example



 */

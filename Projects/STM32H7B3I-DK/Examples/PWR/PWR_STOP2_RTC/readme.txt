/**
  @page PWR_STOP2_RTC PWR STOP2 RTC example
  
  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_STOP2_RTC/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Power Stop 2 RTC example.
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

How to enter the system in STOP mode with main domain in DSTOP2 and wake-up from this mode
using RTC Wake-up Timer with memory shut-off option enabled.
It allows to measure the current consumption in Stop 2 mode with RTC enabled.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H7B3xxQ Devices :
The CPU at 280 MHz.
The HCLK for CD Domain AXI and AHB3 peripherals, CD Domain AHB1/AHB2 peripherals and SRD Domain AHB4 peripherals at 280 MHz.
The APB clock dividers for CD Domain APB3 peripherals, CD Domain APB1 and APB2 peripherals and SRD Domain APB4 peripherals to run at 280 MHz/2.
In the associated software, the system clock is set to 280 MHz and the SysTick is 
programmed to generate an interrupt each 1 ms.
The Low Speed Internal (LSI) clock is used as RTC clock source by default.
EXTI_Line19 is internally connected to the RTC Wakeup event.

The system automatically enters Stop 2 mode 5 sec. after start-up. The RTC wake-up 
is configured to generate an interrupt on rising edge about 33 sec. afterwards.
Current consumption in Stop 2 mode with RTC feature enabled can be measured during that time.
More than half a minute is chosen to ensure current convergence to its lowest operating point.

After wake-up from Stop 2 mode, program execution is resumed.

Two leds LED2 and LED3 are used to monitor the system state as following:
 - LED3 ON: configuration failed (system will go to an infinite loop)
 - LED2 toggling: system in Run mode
 - LED2 off : system in Stop 2 mode

These steps are repeated in an infinite loop.

@note To measure the current consumption in Stop 2 mode, remove JP2 jumper 
      and connect an amperemeter to JP2 to measure IDD current.

@note This example can not be used in DEBUG mode due to the fact 
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
      by the Cortex M7 and the MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA (or other masters) based access or requires more RAM, then the user has to:
              - Use a non TCM SRAM. (example : CD AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters (DMAs, DMA2D, LTDC, MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence.
              Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
              Please refer to the AN4839 "Level 1 cache on STM32F7 Series and STM32H7 Series"

@par Keywords

System, Power, PWR, Stop mode, Stop2, Interrupt, EXTI, Wakeup, Low Power, RTC, LSI

@par Directory contents 

  - PWR/PWR_STOP2_RTC/Inc/stm32h7xx_conf.h         HAL Configuration file
  - PWR/PWR_STOP2_RTC/Inc/stm32h7xx_it.h           Header for stm32h7xx_it.c
  - PWR/PWR_STOP2_RTC/Inc/main.h                   Header file for main.c
  - PWR/PWR_STOP2_RTC/Inc/stm32h7b3i_discovery_conf.h  STM32H7B3I-DK board configuration file
  - PWR/PWR_STOP2_RTC/Src/system_stm32h7xx.c       STM32H7xx system clock configuration file
  - PWR/PWR_STOP2_RTC/Src/stm32h7xx_it.c           Interrupt handlers
  - PWR/PWR_STOP2_RTC/Src/main.c                   Main program
  - PWR/PWR_STOP2_RTC/Src/stm32h7xx_hal_msp.c      HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32H7B3xxQ devices

  - This example has been tested with STMicroelectronics STM32H7B3I-DISCO
    board and can be easily tailored to any other supported device 
    and development board.

  - STM32H7B3I-DISCO Set-up
    - Use LED2 and LED3 connected respectively to PG2 and PG11 pins

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example



 */

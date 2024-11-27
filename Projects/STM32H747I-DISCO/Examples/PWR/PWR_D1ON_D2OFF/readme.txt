/**
  @page PWR_D1ON_D2OFF Power D1 domain ON and D2 domain OFF Example 
  
  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_D1ON_D2OFF/readme.txt
  * @author  MCD Application Team
  * @brief   Description of PWR_D1ON_D2OFF example.
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

This example shows how to run the system with only D1 domain in Run mode 
while D2 domain is in Standby mode.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.

The SystemClock_Config() function is used to set the Flash latency and  to 
configure the system clock :
  - The Cortex-M7 at 400MHz 
  - The Cortex-M4 at 200MHz.
  - The HCLK for D1 Domain AXI/AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals 
    and D3 Domain AHB4  peripherals at 200MHz.
  - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1/APB2  
    peripherals and D3 Domain APB4 peripherals to run at 100MHz.

After a system RESET D2 domain will enter STANDBY mode whereas D1 Domain 
will be kept in RUN mode.
Then D1 domain will check if D2 is OFF by using __HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) 
macro.
            
Two LEDs are used in this program:
  - LED1: Toggle each 100 ms by CM7 core if D2 domain is in Standby mode (test passed).
  - LED3: Set ON by CM7 core if D2 domain is in RUN mode or if a configuration
          error occurred (test failed).

@note This example can not be used in DEBUG mode due to the fact that 
      the Cortex-M7 and Cortex-M4 cores are no longer clocked during 
      low power mode so debugging features are disabled.

@note Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select 
      the RTC clock source; in this case the Backup domain will be reset in  
      order to modify the RTC Clock source, as consequence RTC registers (including 
      the backup registers) and RCC_CSR register are set to their reset values.

@Note For the Cortex-M7, if the application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex-M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be properly defined to be aligned to L1-CACHE line size (32 bytes). 
 
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
      Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
      Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Power, STANDBY mode, RUN mode, multiprocessor, Domains

@par Directory contents 

  - PWR/PWR_D1ON_D2OFF/Common/Src/system_stm32h7xx.c    STM32H7xx system configuration source file
  - PWR/PWR_D1ON_D2OFF/Common/Inc/Common.h              CM4 - CM7 common items

  - PWR/PWR_D1ON_D2OFF/CM7/Inc/stm32h7xx_conf.h         HAL Configuration file
  - PWR/PWR_D1ON_D2OFF/CM7/Inc/stm32h7xx_it.h           Header for stm32h7xx_it.c
  - PWR/PWR_D1ON_D2OFF/CM7/Inc/main.h                   Header file for main.c
  - PWR/PWR_D1ON_D2OFF/CM7/Src/system_stm32h7xx.c       STM32H7xx system clock configuration file
  - PWR/PWR_D1ON_D2OFF/CM7/Src/stm32h7xx_it.c           Interrupt handlers
  - PWR/PWR_D1ON_D2OFF/CM7/Src/main.c                   Main program
  - PWR/PWR_D1ON_D2OFF/CM7/Src/stm32h7xx_hal_msp.c      HAL MSP module

  - PWR/PWR_D1ON_D2OFF/CM4/Inc/stm32h7xx_conf.h         HAL Configuration file
  - PWR/PWR_D1ON_D2OFF/CM4/Inc/stm32h7xx_it.h           Header for stm32h7xx_it.c
  - PWR/PWR_D1ON_D2OFF/CM4/Inc/main.h                   Header file for main.c
  - PWR/PWR_D1ON_D2OFF/CM4/Src/system_stm32h7xx.c       STM32H7xx system clock configuration file
  - PWR/PWR_D1ON_D2OFF/CM4/Src/stm32h7xx_it.c           Interrupt handlers
  - PWR/PWR_D1ON_D2OFF/CM4/Src/main.c                   Main program
  - PWR/PWR_D1ON_D2OFF/CM4/Src/stm32h7xx_hal_msp.c      HAL MSP module
  
@par Hardware and Software environment  

  - This example runs on STM32H747xx/STM32H757xx devices.
  
  - This example has been tested with STM32H747I-DISCO board and can be
    easily tailored to any other supported device and development board.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - For each target configuration (STM32H747I_DISCO_CM7 and STM32H747I_DISCO_CM4) : 
     - Rebuild all files 
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) and CPU2 (Cortex-M4) at once.
 - Run the example



 */

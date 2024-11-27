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
  * Copyright (c) 2018 STMicroelectronics.
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
and __HAL_PWR_GET_FLAG(PWR_FLAG_SB_D2) macros.
            
Two LEDs are used in this program:
  - LED1: Toggle each 100 ms by CM7 core if D2 domain is in Standby mode (test passed).
  - LED2: Set ON by CM7 core if D2 domain is in RUN mode or if a configuration
          error occurred (test failed).

@note This example can not be used in DEBUG mode due to the fact that 
      the Cortex-M7 and Cortex-M4 cores are no longer clocked during 
      low power mode so debugging features are disabled.

@note Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select 
      the RTC clock source; in this case the Backup domain will be reset in  
      order to modify the RTC Clock source, as consequence RTC registers (including 
      the backup registers) and RCC_CSR register are set to their reset values.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Power, STANDBY mode, RUN mode, Domains, multiprocessor

@par Directory contents 

  - PWR/PWR_D1ON_D2OFF/Common/Src/system_stm32h7xx.c    STM32H7xx system configuration source file
  - PWR/PWR_D1ON_D2OFF/Common/Inc/Common.h              CM4 - CM7 common items

  - PWR/PWR_D1ON_D2OFF/CM7/Inc/stm32h7xx_conf.h         HAL Configuration file
  - PWR/PWR_D1ON_D2OFF/CM7/Inc/stm32h7xx_it.h           Header for stm32h7xx_it.c
  - PWR/PWR_D1ON_D2OFF/CM7/Inc/main.h                   Header file for main.c
  - PWR/PWR_D1ON_D2OFF/CM7/Src/stm32h7xx_it.c           Interrupt handlers
  - PWR/PWR_D1ON_D2OFF/CM7/Src/main.c                   Main program

  - PWR/PWR_D1ON_D2OFF/CM4/Inc/stm32h7xx_conf.h         HAL Configuration file
  - PWR/PWR_D1ON_D2OFF/CM4/Inc/stm32h7xx_it.h           Header for stm32h7xx_it.c
  - PWR/PWR_D1ON_D2OFF/CM4/Inc/main.h                   Header file for main.c
  - PWR/PWR_D1ON_D2OFF/CM4/Src/stm32h7xx_it.c           Interrupt handlers
  - PWR/PWR_D1ON_D2OFF/CM4/Src/main.c                   Main program
  

  
@par Hardware and Software environment  

  - This example runs on STM32H747xx/STM32H745xx devices.
  
  - This example has been tested with STMicroelectronics NUCLEO-H745ZI-Q
    boards and can be easily tailored to any other supported device 
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - For each target configuration (STM32H745ZI_Nucleo_CM7 and STM32H745ZI_Nucleo_CM4) : 
     - Rebuild all files 
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) 
   and CPU2 (Cortex-M4) at once.
 - Run the example


 */

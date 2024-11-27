/**
  @page PWR_STANDBY PWR STANDBY Example
  
  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_STANDBY/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the PWR STANDBY example.
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

This example shows how to enter the system in STANDBY mode and wake-up from this 
mode using the external RESET or the Wakeup pin event.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.

The SystemClock_Config() function is used to set the Flash latency and  to configure the system clock :
  - The Cortex-M7 at 400MHz 
  - The Cortex-M4 at 200MHz.
  - The HCLK for D1 Domain AXI/AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals 
    and D3 Domain AHB4  peripherals at 200MHz.
  - The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1/APB2 peripherals 
    and D3 Domain APB4 peripherals to run at 100MHz.

CPU1 (Cortex-M7) and CPU2 (Cortex-M4) are booting at once (with respect to configured boot Flash options)
System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7).
In the meantime Domain D2 is put in STOP mode (CPU2: Cortex-M4 in deep sleep mode) to save power consumption.
When system initialization is finished, CPU1 (Cortex-M7) could release CPU2 (Cortex-M4) when needed
by means of HSEM notification or by any D2 Domain wakeup source (SEV,EXTI..).

The above will guarantee that CPU2 (Cortex-M4) code execution starts after system initialization :
(system clock config, external memory configuration..).

After Domain D2 wakeup, if  CPU1 attempts to use any resource from such a domain, 
the access will not be safe until “clock ready flag” of such a domain is set (by hardware). 
The check could be done using this macro : __HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY).

The system configures the Wakeup pin (PA0) as wakeup source from STANDBY mode 
by using the HAL_PWREx_EnableWakeUpPin function.
This pin is internally connected to PA0 and to the EXTI_Line55 and it is configured 
to generate an interrupt on rising edge when pressed.

Two LEDs are used in this program:
  - LED1: To indicate the CM7 status. LED1 is ON when CM7 is in RUN mode
          or a configuration error occurred during program execution.
          LED1 is OFF when CM7 enters STANDBY mode.
  - LED2: To indicate the CM4 status. LED2 is ON when CM4 is in RUN mode  
          and LED2 is OFF when CM4 enters STANDBY mode.
          
After RESET LED1 is turned ON during 5 seconds to indicate that CM7 core is in RUN mdoe
and LED2 is turned ON during 3 seconds to indicate that CM4 core is in RUN mode.
Then the system will automatically enters STANDBY mode when:
  - D1 Domain enters DSTANDBY mode after calling the HAL_PWREx_EnterSTANDBYMode function 
    by CM7 core => LED1 turns OFF.
  - D2 Domain enters DSTANDBY mode after calling the HAL_PWREx_EnterSTANDBYMode function 
    by CM4 core => LED2 turns OFF.
  - D3 Domain is configured by CM4 core through the HAL_PWREx_EnterSTANDBYMode function 
    to enter DSTANDBY mode when the system enters STANDBY mode.

When the system enters STANDBY mode then current consumption can be measured.

When the system is in STANDBY mode, if the WAKEUP pin is pressed then LED1 will turn ON 
to indicate that CM7 is woken up and LED2 will turn ON to indicate that CM4 is woken up.
After 3 seconds LED2 will turn OFF and CM4 re-enters STANDBY mode and after 5 seconds 
LED1 will turn OFF and CM7 re-enters STANDBY mode.
Note that after a wake-up from STANDBY mode the program execution restarts in the 
same way as after a software RESET.

These steps are repeated in an infinite loop.
 
@note This example can not be used in DEBUG mode due to the fact that 
      the Cortex-M7 and Cortex-M4 cores are no longer clocked during 
      low power mode so debugging features are disabled.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.      

@par Keywords

System, Power, PWR, multiprocessor, Standby mode, Interrupt, EXTI, Wakeup, Wakeup pin, Low Power, External reset, Button

@par Directory contents

  - PWR/PWR_STANDBY/Common/Src/system_stm32h7xx.c    STM32H7xx system configuration source file
  - PWR/PWR_STANDBY/Common/Inc/Common.h              CM4 - CM7 common items

  - PWR/PWR_STANDBY/CM7/Inc/stm32h7xx_conf.h         HAL Configuration file
  - PWR/PWR_STANDBY/CM7/Inc/stm32h7xx_it.h           Header for stm32h7xx_it.c
  - PWR/PWR_STANDBY/CM7/Inc/main.h                   Header file for main.c
  - PWR/PWR_STANDBY/CM7/Src/system_stm32h7xx.c       STM32H7xx system clock configuration file
  - PWR/PWR_STANDBY/CM7/Src/stm32h7xx_it.c           Interrupt handlers
  - PWR/PWR_STANDBY/CM7/Src/main.c                   Main program
  - PWR/PWR_STANDBY/CM7/Src/stm32h7xx_hal_msp.c      HAL MSP module
  
  - PWR/PWR_STANDBY/CM4/Inc/stm32h7xx_conf.h         HAL Configuration file
  - PWR/PWR_STANDBY/CM4/Inc/stm32h7xx_it.h           Header for stm32h7xx_it.c
  - PWR/PWR_STANDBY/CM4/Inc/main.h                   Header file for main.c
  - PWR/PWR_STANDBY/CM4/Src/system_stm32h7xx.c       STM32H7xx system clock configuration file
  - PWR/PWR_STANDBY/CM4/Src/stm32h7xx_it.c           Interrupt handlers
  - PWR/PWR_STANDBY/CM4/Src/main.c                   Main program
  - PWR/PWR_STANDBY/CM4/Src/stm32h7xx_hal_msp.c      HAL MSP module

@par Hardware and Software environment  

  - This example runs on STM32H747xx/STM32H745xx devices.
  
  - This example has been tested with STMicroelectronics STM32H745I-DISCO
    boards and can be easily tailored to any other supported device 
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - For each target configuration (STM32H745I_DISCO_CM7 and STM32H745I_DISCO_CM4) : 
     - Rebuild all files 
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) 
   and CPU2 (Cortex-M4) at once.
 - Run the example


 */

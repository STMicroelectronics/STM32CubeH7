/**
  @page PWR_Hold_Mechanism PWR Hold Mechanism Example
  
  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_Hold_Mechanism/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the PWR Hold Mechanism example.
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

This example shows how to use Hold Mechanism to allow the system to be 
re-initialized by a master CPU. This allows the master CPU to be woken up on both 
its own wakeup sources and the wakeup sources of the slave CPU, and keep the slave
CPU in hold until released by the master CPU.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.

The SystemClock_Config() function is used to set the Flash latency and  to 
configure the system clock :
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

In the main.c file there are two flags to select one of the following scenarios:
  - Scenario 1 selected by CM4_HOLD_TEST flag: The CM7 core configured as master CPU 
    will hold the CM4 core configured as slave CPU. 
  - Scenario 2 selected by CM7_HOLD_TEST flag: The CM4 core configured as master CPU 
    will hold the CM7 core configured as slave CPU. 

The system clock source is configured only by CM7. So when CM7 is the master CPU 
the system clock source will be reconfigured after exiting STOP mode. But when CM4
is the master CPU the system clock source is not reconfigured and the HSI is selected
since it is the default clock after wakeup from STOP mode.
    
The system uses the RTC wakeup timer interrupt to wake up from STOP mode.
The RTC is configured as follow:
  - The Low Speed Internal (LSI) clock is used as RTC clock source by default.
  - The EXTI_Line19 connected internally to the RTC Wakeup event is configured
    to generate an interrupt on rising edge for both CPU each 20s. 
  - The EXTI_Line19 is configured through the HAL_EXTI_D1_EventInputConfig function
    to enable CM7 wakeup line and HAL_EXTI_D2_EventInputConfig function to enable CM4 wakeup line.
    
Two LEDs are used in this program:
  - LED1: To indicate the CM7 status. LED1 is ON when CM7 is in RUN mode
          and LED1 is OFF when CM7 enters STOP mode.       
  - LED2: To indicate the CM4 status. LED2 is ON when CM4 is in RUN mode  
          and LED2 is OFF when CM4 enters STOP mode.

After RESET LED1 is turned ON during 5 seconds to indicate that CM7 core is in RUN mode
and LED2 is turned ON during 3 seconds to indicate that CM4 core is in RUN mode.
Then the system will automatically enters STOP mode when:
  - D1 Domain enters DSTOP mode after calling the HAL_PWREx_EnterSTOPMode function 
    by CM7 core => LED1 turns OFF.
  - D2 Domain enters DSTOP mode after calling the HAL_PWREx_EnterSTOPMode function 
    by CM4 core => LED2 turns OFF.
  - D3 Domain is configured by CM4 core through the HAL_PWREx_EnterSTOPMode function 
    to enter DSTOP mode when the system enters STOP mode.
    
After a system wake up from STOP mode following the RTC timer interrupt (i.e after 20 sec):
  If scenario 1 is selected (CM4_HOLD_TEST) then:
    LED1 will turn ON during 5 seconds to indicate that CM7 core is woken up,
    then LED1 turns OFF and HAL_PWREx_ReleaseCore function is called
    to release CM4 core. Consequenctly, LED2 turns ON to indicate that CM4 core 
    is released and woken up from STOP mode.
  If scenario 2 is selected (CM7_HOLD_TEST) then:
    LED2 will turn ON during 3 seconds to indicate that CM4 core is woken up,
    then LED2 turns OFF and HAL_PWREx_ReleaseCore function is called
    to release CM7 core. Consequenctly, LED1 turns ON to indicate that CM7 core 
    is released and woken up from STOP mode.
    When CM7 core is woken up from STOP mode the clock system is reconfigured since 
    the default system core after wake up from STOP mode is the HSI. 
Then both LED1 and LED2 are turn OFF to indicate that the system is entered STOP mode.
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

System, Power, PWR, multiprocessor, Stop mode, Hold, Low Power, Wakeup, Dual core, Master, Slave, RTC, LSI, Interrupt, EXTI, Led

@par Directory contents 

  - PWR/PWR_Hold_Mechanism/Common/Src/system_stm32h7xx.c    STM32H7xx system configuration source file
  - PWR/PWR_Hold_Mechanism/Common/Inc/Common.h              CM4 - CM7 common items

  - PWR/PWR_Hold_Mechanism/CM7/Inc/stm32h7xx_conf.h         HAL Configuration file
  - PWR/PWR_Hold_Mechanism/CM7/Inc/stm32h7xx_it.h           Header for stm32h7xx_it.c
  - PWR/PWR_Hold_Mechanism/CM7/Inc/main.h                   Header file for main.c
  - PWR/PWR_Hold_Mechanism/CM7/Src/stm32h7xx_it.c           Interrupt handlers
  - PWR/PWR_Hold_Mechanism/CM7/Src/main.c                   Main program
  - PWR/PWR_Hold_Mechanism/CM7/Src/stm32h7xx_hal_msp.c      HAL MSP module
  
  - PWR/PWR_Hold_Mechanism/CM4/Inc/stm32h7xx_conf.h         HAL Configuration file
  - PWR/PWR_Hold_Mechanism/CM4/Inc/stm32h7xx_it.h           Header for stm32h7xx_it.c
  - PWR/PWR_Hold_Mechanism/CM4/Inc/main.h                   Header file for main.c
  - PWR/PWR_Hold_Mechanism/CM4/Src/stm32h7xx_it.c           Interrupt handlers
  - PWR/PWR_Hold_Mechanism/CM4/Src/main.c                   Main program

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

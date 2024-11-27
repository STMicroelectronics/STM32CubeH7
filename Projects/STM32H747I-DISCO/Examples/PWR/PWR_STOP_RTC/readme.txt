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

This example shows how to enter the system in Stop mode and wakes up from this 
mode by using:
  - The RTC wakeup timer as wakeup source event. This event is connected to 
    EXTI_Line19.
  - Multi wakeup targets (CM7 only, CM4 only or both of them)
  - Multi wakeup clock source (PLL, HSI, CSI).

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

In the common.h file there are three flags to select one of the following scenarios:
  - Scenario 1 selected by WAKEUP_TARGET_CM7 flag: Here the CM7 core will be the 
    wakeup target for the RTC timer event so only CM7 will wake up while CM4 still
    always in STOP mode. The system clock source is 400MHz so when exiting from STOP 
    mode the system clock source is reconfigured to still always 400MHz.
  - Scenario 2 selected by WAKEUP_TARGET_CM4 flag: Here the CM4 core will be the 
    wakeup target for the RTC timer event so only CM4 will wake up while CM7 still
    always in STOP mode. The system clock source is the HSI 64MHZ so when exiting from 
    STOP mode the clock source is not reconfigured since the default one is the HSI.    
  - Scenario 3 selected by WAKEUP_TARGET_ALL_CPU flag: Here both CM7 and CM4 cores
    will be the wakeup targets for the RTC timer event so CM7 and CM4 will wakeup 
    from STOP mode at the same time. The system clock source is the HSI 64MHZ 
    but when exiting from STOP mode clock source is the CSI 4MHz so HAL_InitTick 
    function is called to reconfigure the Tick since the system clock is switched 
    to the CSI.
    
The system uses the RTC wakeup timer interrupt to wake up from STOP mode.
The RTC is configured as follow:
  - The Low Speed Internal (LSI) clock is used as RTC clock source by default.
  - The EXTI_Line19 connected internally to the RTC Wakeup event is configured
    to generate an interrupt on rising edge each 20s. 
  - The EXTI_Line19 is configured through the HAL_EXTI_D1_EventInputConfig function
    to enable or disable CM7 wakeup line and HAL_EXTI_D2_EventInputConfig function 
    to enable or disable CM4 wakeup line.
        
    STOP Mode with RTC clocked by LSI
    ===================================  
            - RTC Clocked by LSI
            - Regulator in LP mode
            - VREFINT OFF with fast wakeup enabled
            - HSI as SysClk after Wake Up
            - No IWDG
            - Automatic Wakeup using RTC on LSI (after ~20s)
            
Three LEDs are used in this program:
  - LED1: To indicate the CM7 status. LED1 is ON when CM7 is in RUN mode
          and LED1 is OFF when CM7 enters STOP mode.       
  - LED2: To indicate the CM4 status. LED2 is ON when CM4 is in RUN mode  
          and LED2 is OFF when CM4 enters STOP mode. 
  - LED3: To indicate if a configuration error occurred during program execution.

After RESET LED1 is turned ON during 5 seconds to indicate that CM7 core is in RUN 
mode and LED2 is turned ON during 3 seconds to indicate that CM4 core is in RUN mode.
Then the system will automatically enters STOP mode when:
  - D1 Domain enters DSTOP mode after calling the HAL_PWREx_EnterSTOPMode function 
    by CM7 core => LED1 turns OFF.
  - D2 Domain enters DSTOP mode after calling the HAL_PWREx_EnterSTOPMode function 
    by CM4 core => LED2 turns OFF.
  - D3 Domain is configured by CM4 core through the HAL_PWREx_EnterSTANDBYMode  
    function to enter DSTANDBY mode when the system enters STOP mode.
    
After a system wake up from STOP mode following the RTC timer interrupt:
  If scenario 1 is selected (WAKEUP_TARGET_CM7) then:
    LED1 will turn ON during 5 seconds to indicate that CM7 core is woken up.
    Next after 5 seconds LED1 turns OFF to indicate that CM7 core is re-entering
    STOP mode.
  If scenario 2 is selected (WAKEUP_TARGET_CM4) then:
    LED2 will turn ON during 3 seconds to indicate that CM4 core is woken up.
    Next after 3 seconds LED2 turns OFF to indicate that CM4 core is re-entering
    STOP mode.
  If scenario 3 is selected (WAKEUP_TARGET_ALL_CPU) then:
    LED1 will turn ON during 5 seconds and LED2 will turn ON during 3 seconds
    to indicate that both CM7 and CM4 cores are woken up.
    Next after 3 seconds LED2 turns OFF and LED1 turns OFF after 5 seconds to  
    indicate that the system is re-entering STOP mode. 

The above scenario is repeated in an infinite loop.

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

Power, PWR, multiprocessor, Stop mode, Interrupt, EXTI, Wakeup, Low Power, RTC, External reset, LSI, 

@par Directory contents 

  - PWR/PWR_STOP_RTC/Common/Src/system_stm32h7xx.c    STM32H7xx system configuration source file
  - PWR/PWR_STOP_RTC/Common/Inc/Common.h              CM4 - CM7 common items

  - PWR/PWR_STOP_RTC/CM7/Inc/stm32h7xx_conf.h         HAL Configuration file
  - PWR/PWR_STOP_RTC/CM7/Inc/stm32h7xx_it.h           Header for stm32h7xx_it.c
  - PWR/PWR_STOP_RTC/CM7/Inc/main.h                   Header file for main.c
  - PWR/PWR_STOP_RTC/CM7/Src/system_stm32h7xx.c       STM32H7xx system clock configuration file
  - PWR/PWR_STOP_RTC/CM7/Src/stm32h7xx_it.c           Interrupt handlers
  - PWR/PWR_STOP_RTC/CM7/Src/main.c                   Main program
  - PWR/PWR_STOP_RTC/CM7/Src/stm32h7xx_hal_msp.c      HAL MSP module

  - PWR/PWR_STOP_RTC/CM4/Inc/stm32h7xx_conf.h         HAL Configuration file
  - PWR/PWR_STOP_RTC/CM4/Inc/stm32h7xx_it.h           Header for stm32h7xx_it.c
  - PWR/PWR_STOP_RTC/CM4/Inc/main.h                   Header file for main.c
  - PWR/PWR_STOP_RTC/CM4/Src/system_stm32h7xx.c       STM32H7xx system clock configuration file
  - PWR/PWR_STOP_RTC/CM4/Src/stm32h7xx_it.c           Interrupt handlers
  - PWR/PWR_STOP_RTC/CM4/Src/main.c                   Main program
  - PWR/PWR_STOP_RTC/CM4/Src/stm32h7xx_hal_msp.c      HAL MSP module
  
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

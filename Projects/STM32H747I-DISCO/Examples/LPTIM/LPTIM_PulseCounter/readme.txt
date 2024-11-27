/**
  @page LPTIM_PulseCounter Low power timer pulse counter example
  
  @verbatim
  ******************************************************************************
  * @file    LPTIM/LPTIM_PulseCounter/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LPTIM Pulse counter example.
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

This example describes how to configure and use LPTIM to count pulses through
the LPTIM HAL API.

The SystemClock_Config() function is used to set the Flash latency and  to configure the system clock :
  - The Cortex-M7 at 400MHz 
  - Cortex-M4 at 200MHz.
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

To reduce power consumption, MCU enters stop mode after starting counting. Each
time the counter reaches the maximum value (Period/Autoreload), an interruption
is generated, the MCU is woke up from stop mode and LED1 toggles the last state.
  
In this example Period value is set to 1000, so each time the counter counts
(1000 + 1) rising edges on LPTIM1_IN1 pin, an interrupt is generated and LED1
toggles.

In this example the internal clock provided to the LPTIM1 is LSI (32 Khz),
so the external input is sampled with LSI clock. In order not to miss any event,
the frequency of the changes on the external Input1 signal should never exceed the
frequency of the internal clock provided to the LPTIM1 (LSI for the
present example).
         
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
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

Timer, Low Power, LSI, Pulse Counter, External signal generator, Stop mode, Wake up, Interrupt, LED

@par Directory contents

    - LPTIM/LPTIM_PulseCounter/CM7/Inc/main.h                Main configuration file for Cortex-M7
    - LPTIM/LPTIM_PulseCounter/CM7/Inc/stm32h7xx_it.h        Interrupt handlers header file for Cortex-M7
    - LPTIM/LPTIM_PulseCounter/CM7/Inc/stm32h7xx_hal_conf.h  HAL configuration file for Cortex-M7
    - LPTIM/LPTIM_PulseCounter/CM7/Src/main.c                Main program  for Cortex-M7
    - LPTIM/LPTIM_PulseCounter/CM7/Src/stm32h7xx_it.c        Interrupt handlers for Cortex-M7
    - LPTIM/LPTIM_PulseCounter/CM7/Src/stm32h7xx_hal_msp.c   HAL MSP module for Cortex-M7

    - LPTIM/LPTIM_PulseCounter/CM4/Inc/main.h                Main configuration file for Cortex-M4
    - LPTIM/LPTIM_PulseCounter/CM4/Inc/stm32h7xx_it.h        Interrupt handlers header file for Cortex-M4
    - LPTIM/LPTIM_PulseCounter/CM4/Inc/stm32h7xx_hal_conf.h  HAL configuration file for Cortex-M4
    - LPTIM/LPTIM_PulseCounter/CM4/Src/main.c                Main program  for Cortex-M4
    - LPTIM/LPTIM_PulseCounter/CM4/Src/stm32h7xx_it.c        Interrupt handlers for Cortex-M4
    - LPTIM/LPTIM_PulseCounter/CM4/Src/stm32h7xx_hal_msp.c   HAL MSP module for Cortex-M4

@par Hardware and Software environment  

  - This example runs on STM32H747xx devices.
  
  - This example has been tested with STM32H747I_DISCO  board with SMPS (SD Converter) power supply config and can be
    easily tailored to any other supported device and development board.
    
  - Generate pulses on PD12 (pin 10 on CN5 connector). (Connect a square waveform).    

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - For each target configuration (STM32H747I_DISCO_CM7 and STM32H747I_DISCO_CM4) : 
     - Rebuild all files 
     - Load images into target memory
 - Run the example


 */

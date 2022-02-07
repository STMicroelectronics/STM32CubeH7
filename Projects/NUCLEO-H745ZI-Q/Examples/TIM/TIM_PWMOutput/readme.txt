/**
  @page TIM_PWMOutput TIM PWM Output example
  
  @verbatim
  ******************** (C) COPYRIGHT 2018 STMicroelectronics *******************
  * @file    TIM/TIM_PWMOutput/readme.txt  
  * @author  MCD Application Team
  * @brief   Description of the PWM signals generation using TIM1.
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

This example shows how to configure the TIM peripheral in PWM (Pulse Width Modulation) 
mode.

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
the access will not be safe until �clock ready flag� of such a domain is set (by hardware). 
The check could be done using this macro : __HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY).

In this example TIM1 input clock (TIM1CLK) is set to APB2 clock (PCLK2),
since APB2 prescaler is equal to 2.
  TIM1CLK = 2*PCLK2
  PCLK2 = HCLK/2
  => TIM1CLK = HCLK

To get TIM1 counter clock at 20 MHz, the prescaler is computed as follows:
  Prescaler = (TIM1CLK / TIM1 counter clock) - 1
  Prescaler = ((HCLK) /2*20 MHz) - 1

To get TIM1 output clock at 20 KHz, the period (ARR)) is computed as follows:
  ARR = (TIM1 counter clock / TIM1 output clock) - 1
      = 999

    TIM1 Channel1 duty cycle = (TIM1_CCR1/ TIM1_ARR + 1)* 100 = 50%
    TIM1 Channel2 duty cycle = (TIM1_CCR2/ TIM1_ARR + 1)* 100 = 37.5%
    TIM1 Channel3 duty cycle = (TIM1_CCR3/ TIM1_ARR + 1)* 100 = 25%
    TIM1 Channel4 duty cycle = (TIM1_CCR4/ TIM1_ARR + 1)* 100 = 12.5%


The PWM waveforms can be displayed using an oscilloscope.
LED3 is ON when there are an error.
         
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the �MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
����� If the application needs to use DMA(or other masters) based access or requires more RAM, then �the user has to:
����������� � - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
����������� � - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
�������       - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly�defined to be aligned to L1-CACHE line size (32 bytes). 
�
@Note It is recommended to enable the cache and maintain its coherence.
      Depending on the use case it is also possible to configure the cache attributes using the MPU.
������Please refer to the AN4838 "Managing memory protection unit (MPU) in STM32 MCUs"
������Please refer to the AN4839 "Level 1 cache on STM32F7 Series"

@par Keywords

Timer, Output, signal, PWM, Oscilloscope, Frequency, Duty cycle, Waveform

@par Directory contents
  
    - TIM/TIM_PWMOutput/CM7/Inc/main.h                Main configuration file for Cortex-M7
    - TIM/TIM_PWMOutput/CM7/Inc/stm32h7xx_it.h        Interrupt handlers header file for Cortex-M7
    - TIM/TIM_PWMOutput/CM7/Inc/stm32h7xx_hal_conf.h  HAL configuration file for Cortex-M7
    - TIM/TIM_PWMOutput/CM7/Src/main.c                Main program  for Cortex-M7
    - TIM/TIM_PWMOutput/CM7/Src/stm32h7xx_it.c        Interrupt handlers for Cortex-M7
    - TIM/TIM_PWMOutput/CM7/Src/stm32h7xx_hal_msp.c   HAL MSP module for Cortex-M7

    - TIM/TIM_PWMOutput/CM4/Inc/main.h                Main configuration file for Cortex-M4
    - TIM/TIM_PWMOutput/CM4/Inc/stm32h7xx_it.h        Interrupt handlers header file for Cortex-M4
    - TIM/TIM_PWMOutput/CM4/Inc/stm32h7xx_hal_conf.h  HAL configuration file for Cortex-M4
    - TIM/TIM_PWMOutput/CM4/Src/main.c                Main program  for Cortex-M4
    - TIM/TIM_PWMOutput/CM4/Src/stm32h7xx_it.c        Interrupt handlers for Cortex-M4
    - TIM/TIM_PWMOutput/CM4/Src/stm32h7xx_hal_msp.c   HAL MSP module for Cortex-M4
	
@par Hardware and Software environment  

  - This example runs on STM32H745xx devices.
  
  - This example has been tested with NUCLEO-H745ZI-Q  board with SMPS (SD Convertor) power supply config and can be
    easily tailored to any other supported device and development board.
    
  - NUCLEO-H745ZI-Q Set-up
   Connect the following pins to an oscilloscope to monitor the different waveforms:
        - TIM1_CH1 : PE.09 (PIN 50 CN12)
        - TIM1_CH2 : PE.11 (PIN 54 CN12)
        - TIM1_CH3 : PE.13 (PIN 53 CN12)
        - TIM1_CH4 : PE.14 (PIN 49 CN12)  

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - For each target configuration (STM32H745ZI_NUCLEO_CM7 and STM32H745I_NUCLEO_CM4) : 
     - Rebuild all files 
     - Load images into target memory
 - Run the example
                          

 */
                                   

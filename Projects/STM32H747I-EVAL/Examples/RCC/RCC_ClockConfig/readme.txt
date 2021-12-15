/**
  @page RCC_ClockConfig RCC Clock Config example
  
  @verbatim
  ********************* COPYRIGHT(c) 2019 STMicroelectronics *******************
  * @file    RCC/RCC_ClockConfig/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the RCC Clock Config example.
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

Configuration of the system clock (SYSCLK) and modification of the clock settings in Run mode, using the RCC HAL API.

CPU1 (Cortex-M7) and CPU2 (Cortex-M4) are booting at once (with respect to configured boot Flash options)
System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7).
In the meantime Domain D2 is put in STOP mode (CPU2: Cortex-M4 in deep sleep mode) to save power consumption.
When system initialization is finished, CPU1 (Cortex-M7) will release CPU2 (Cortex-M4) when needed
by means of HSEM notification but release could be ensured by any Domain D2 wakeup source (SEV,EXTI..).

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32H747xx Devices :
The CPU at 400MHz 
The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 200MHz.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to  run at 100MHz.

In this example, after startup SYSCLK is configured to the max frequency using the PLL with
HSE as clock source, the Tamper push-button (connected to EXTI line 13) will be 
used to change the PLL source: 
- from HSE to HSI
- from HSI to CSI
- from CSI to HSE

Each time the Tamper push-button is pressed; EXTI15_10 interrupt interrupt is generated and in the ISR
the PLL oscillator source is checked using __HAL_RCC_GET_PLL_OSCSOURCE() macro:

- If the HSE oscillator is selected as PLL source, the following steps will be followed to switch 
   the PLL source to HSI oscillator:
     a- Switch the system clock source to HSE to allow modification of the PLL configuration
     b- Enable HSI Oscillator, select it as PLL source and finally activate the PLL
     c- Select the PLL as system clock source and configure bus clocks dividers
     d- Disable the HSE oscillator (optional, if the HSE is no more needed by the application)

- If the CSI oscillator is selected as PLL source, the following steps will be followed to switch 
   the PLL source to HSE oscillator:
     a- Switch the system clock source to CSI to allow modification of the PLL configuration
     b- Enable HSE Oscillator, select it as PLL source and finally activate the PLL
     c- Select the PLL as system clock source and configure bus clocks dividers
     d- Disable the CSI oscillator (optional, if the CSI is no more needed by the application)

- If the HSI oscillator is selected as PLL source, the following steps will be followed to switch 
   the PLL source to CSI oscillator:
     a- Switch the system clock source to HSI to allow modification of the PLL configuration
     b- Enable CSI Oscillator, select it as PLL source and finally activate the PLL
     c- Select the PLL as system clock source and configure bus clocks dividers
     d- Disable the HSI oscillator (optional, if the HSI is no more needed by the application)

In this example the SYSCLK /4 is outputted on the MCO2 pin(PC.09).

LED3 is ON when there is an error.

LED1 is toggled with a timing defined by the HAL_Delay() API.

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

RCC, System, Clock Configuration, HSE bypass mode, HSI, CSI, System clock, Oscillator, PLL, Button, MCO

@par Directory contents 

  - RCC/RCC_ClockConfig/Common/Src/system_stm32h7xx.c     STM32H7xx system configuration file
  
  - RCC/RCC_ClockConfig/CM7/Src/main.c                 Main program for Cortex-M7
  - RCC/RCC_ClockConfig/CM7/Src/stm32h7xx_it.c         Interrupt handlers for Cortex-M7
  - RCC/RCC_ClockConfig/CM7/Inc/main.h                 Main program header file for Cortex-M7  
  - RCC/RCC_ClockConfig/CM7/Inc/stm32h7xx_hal_conf.h   HAL Configuration file for Cortex-M7
  - RCC/RCC_ClockConfig/CM7/Inc/stm32h7xx_it.h         Interrupt handlers header file for Cortex-M7

  - RCC/RCC_ClockConfig/CM4/Src/main.c                 Main program for Cortex-M4
  - RCC/RCC_ClockConfig/CM4/Src/stm32h7xx_it.c         Interrupt handlers for Cortex-M4
  - RCC/RCC_ClockConfig/CM4/Inc/stm32h7xx_hal_conf.h   HAL Configuration file for Cortex-M4
  - RCC/RCC_ClockConfig/CM4/Inc/stm32h7xx_it.h         Interrupt handlers header file for Cortex-M4

@par Hardware and Software environment

  - This example runs on STM32H747xx devices.
    
  - This example has been tested with STM32H747I-EVAL  board with SMPS (SD Converter) power supply  config and can be easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred tool chain 
 - For each target configuration (STM32H747I_EVAL_CM7 and STM32H747I_EVAL_CM4) : 
     - Rebuild all files 
     - Load images into target memory
 - After loading the two images, you have to reset the board in order to boot (Cortex-M7) and CPU2 (Cortex-M4) at once.
 - Run the example
 

 */
